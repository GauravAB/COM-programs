#include <Windows.h>
#include "register.h"
#include "exe_server.h"
#include "Util.h"
#include <cmath>

void trace(const char* msg)
{
	Util::Trace("EXE Server: ", msg, S_OK);
}

void trace(const char* msg, HRESULT hr)
{
	Util::Trace("EXE Server: ", msg,hr);
}

class CTrigo : public ITrigo
{
private:
	long m_cRef;
public:
	//IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID , void**);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//class methods
	CTrigo(void);
	~CTrigo(void);

	//Interface methods
	virtual HRESULT __stdcall getSine(float, float*);
	virtual HRESULT __stdcall getCosine(float, float*);
	virtual HRESULT __stdcall getTangent(float, float*);
};

class CTrigoClassFactory : public IClassFactory
{
private:
	long m_cRef;	
public:
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID, void**);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	//class methods
	CTrigoClassFactory();
	~CTrigoClassFactory();

	//Interface methods
	virtual HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	virtual HRESULT __stdcall LockServer(BOOL);
};


long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
HWND ghwnd;
IClassFactory* gpIClassFactory = NULL;
DWORD dwRegister;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	//function declarations
	HRESULT StartMyClassFactory(void);
	void StopMyClassFactory(void);

	//COM needed variables

	int iDontShowWindow = 0;
	HRESULT hr;
	TCHAR szTokens[] = TEXT("-/");
	TCHAR *pszTokens;
	TCHAR lpszCmdLine[255];
	wchar_t *next_token = NULL;




	WNDCLASSEX wndclass;
	MSG msg;
	HWND hwnd;
	TCHAR szAppName[] = L"ExeServertoCom";

	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		trace("CoInitialize FAILED");
		return 0;
	}
	MultiByteToWideChar(CP_ACP, 0, lpCmdLine, 255, lpszCmdLine, 255);

	pszTokens = wcstok_s(lpszCmdLine, szTokens, &next_token);

	while (pszTokens != NULL)
	{
		if (_wcsicmp(pszTokens, L"Embedding") == 0)
		{
			iDontShowWindow = 1;
			break;
		}
		else if (_wcsicmp(pszTokens, L"RegSvr") == 0)
		{
			//trace("registering...");
			RegisterServer(L"ExeComServer210318", (HMODULE)hInstance, CLSID_CTrigo);
		}
		else if (_wcsicmp(pszTokens, L"UnRegSvr") == 0)
		{
			//trace("unregistering...");
			UnregisterServer(CLSID_CTrigo);
		}
		else
		{
			trace("cmdline option incorrect");
			exit(0);
		}
		pszTokens = wcstok_s(NULL, szTokens, &next_token);
	}



	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);


	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, L"ExeServerWhichYouWontSee", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ghwnd = hwnd;

	if (iDontShowWindow != 1)
	{
		ShowWindow(hwnd, SW_HIDE);
	}
	else
	{
		hr = StartMyClassFactory();

		if (FAILED(hr))
		{
			DestroyWindow(hwnd);
			exit(0);
		}
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (iDontShowWindow == 1)
	{
		StopMyClassFactory();
	}

	CoUninitialize();

	return ((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		case WM_CREATE:
			break;
		case WM_DESTROY:
			if((glNumberOfActiveComponents ==0) && (glNumberOfServerLocks ==0))
			{
				PostQuitMessage(0);
			}
			break;
		case WM_CLOSE:
			--glNumberOfServerLocks;
			break;
		default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

//Implementation of CLASSES


CTrigo::CTrigo()
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CTrigo::~CTrigo()
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT __stdcall CTrigo::QueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ITrigo*>(this);
	}	
	else if (riid == IID_ITrigo)
	{
		*ppv = static_cast<ITrigo*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	
	return S_OK;
}

ULONG __stdcall CTrigo::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CTrigo::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
		{
			PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0);
		}
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall CTrigo::getSine(float angle, float*res)
{
	*res = sin(angle);
	return S_OK;

}
HRESULT __stdcall CTrigo::getCosine(float angle, float*res)
{
	*res = cos(angle);
	return S_OK;

}
HRESULT __stdcall CTrigo::getTangent(float angle, float*res)
{
	*res = tan(angle);
	return S_OK;

}


CTrigoClassFactory::CTrigoClassFactory()
{
	m_cRef = 1;

}

CTrigoClassFactory::~CTrigoClassFactory()
{
	//nothing yet
}

HRESULT __stdcall CTrigoClassFactory::QueryInterface(REFIID riid, void**ppv)
{
	if ((riid == IID_IUnknown) || (riid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall CTrigoClassFactory::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CTrigoClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}



HRESULT __stdcall CTrigoClassFactory::CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv)
{
	HRESULT hr;
	CTrigo *pCTrigo;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCTrigo = new CTrigo;

	hr = pCTrigo->QueryInterface(riid, ppv);

	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCTrigo->Release();

	return hr;
}


HRESULT __stdcall CTrigoClassFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&glNumberOfServerLocks);
	}
	else
	{
		InterlockedDecrement(&glNumberOfServerLocks);
	}

	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
	{
		PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0);
	}

	return S_OK;
}

HRESULT StartMyClassFactory(void)
{
	HRESULT hr;
	gpIClassFactory = new CTrigoClassFactory;
	
	if (gpIClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}
	gpIClassFactory->AddRef();

	hr = CoRegisterClassObject(CLSID_CTrigo, (IUnknown*)gpIClassFactory, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &dwRegister);
	if (FAILED(hr))
	{
		gpIClassFactory->Release();
		return(E_FAIL);
	}
	return S_OK;
}

void StopMyClassFactory(void)
{
	if (dwRegister != 0)
	{
		CoRevokeClassObject(dwRegister);
	}

	if (gpIClassFactory)
	{
		gpIClassFactory->Release();
	}
}


















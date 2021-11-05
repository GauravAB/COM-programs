#include <Windows.h>
#include <stdio.h>
#include "ExeServerWithoutRegFile.h"
#include "Util.h"
#include "register.h"
#include <cmath>
#include "CreateProcess.h"

void trace(const char*msg)
{
	Util::Trace("ExeServer: ", msg, S_OK);
}

void trace(const char*msg, HRESULT hr)
{
	Util::Trace("ExeServer: ", msg, hr);
}


//globals 
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
HMODULE ghModule;
HWND ghwnd;
IClassFactory* gpIClassFactory;
DWORD dwRegister;


//prototypes
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

class CTrigoFunctions : public ITrigoFunctions
{
private:
	//component reference count (counts all interfaces)
	long m_cRef;
public:
	
	//costructor and destructor
	CTrigoFunctions();
	~CTrigoFunctions();

	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void**ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	//Interface methods
	virtual HRESULT __stdcall getSine(float, float*);
	virtual HRESULT __stdcall getCosine(float, float*);
	virtual HRESULT __stdcall getTangent(float, float*);
};


class ClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	ClassFactory();
	~ClassFactory();
	//IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void**ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	
	//Interface methods
	virtual HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	virtual HRESULT __stdcall LockServer(BOOL);
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//function signatures
	HRESULT StartMyClassFactories(void);
	void StopMyClassFactories(void);


	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;
	TCHAR szAppName[] = L"ExeServerForCOM";
	HRESULT hr;
	TCHAR szTokens[] = L"-/";
	TCHAR *pszTokens;
	TCHAR lpszCmdLine[255];
	wchar_t *next_token = NULL;
	int iDontShowWindow = 0;


	RegisterServer(L"ExeServerCOM", (HMODULE)hInstance, CLSID_CTrigoFunctions);

	//Initialize COM Library

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		trace("CoInitialize failed");
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
		else if (_wcsicmp(pszTokens, L"Register") ==0)
		{
			trace("Registering ExeServer");
			hr = RegisterServer(L"ExeServer for COM", (HMODULE)hInstance, CLSID_CTrigoFunctions);
			exit(0);
		}
		else if (_wcsicmp(pszTokens, L"Unregister") == 0)
		{
			trace("Unregistering ExeServer");
			UnregisterServer(CLSID_CTrigoFunctions);
			exit(0);
		}

		pszTokens = wcstok_s(NULL, szTokens, &next_token);
	}

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = wndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(L"ExeServerWithoutRegFile", szAppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ghwnd = hwnd;

	ShowWindow(hwnd, SW_HIDE);
	//UpdateWindow(hwnd);

	//initialize class factories
	if (iDontShowWindow == 1)
	{
		hr = StartMyClassFactories();
	//	trace("initialized ClassFactory");
	}

	if (FAILED(hr))
	{
		trace("StartMyClassFactories failed");
		exit(0);
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	if (iDontShowWindow == 1)
	{
		StopMyClassFactories();
	}

	//Uninitialize COM library
	CoUninitialize();
	return ((int)msg.wParam);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_CREATE:
		trace("server running..");
		break;
	case WM_DESTROY:
		if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		{
			PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		--glNumberOfActiveComponents;
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

//IMPLEMENTATION OF COCLASS METHODS

CTrigoFunctions::CTrigoFunctions()
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CTrigoFunctions::~CTrigoFunctions()
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT __stdcall CTrigoFunctions:: QueryInterface(REFIID riid, void**ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ITrigoFunctions*>(this); 
	}
	else if (riid == IID_ITrigoFunctions)
	{
		*ppv = static_cast<ITrigoFunctions*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	
	return S_OK;
}

ULONG __stdcall CTrigoFunctions:: AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall CTrigoFunctions:: Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete this;
		if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		{
			PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L);
		}
	}
	return m_cRef;
}


//Interface methods
HRESULT __stdcall  CTrigoFunctions::getSine(float angle , float*result)
{
	*result = sin(angle);
	return S_OK;
}

HRESULT __stdcall CTrigoFunctions::getCosine(float angle, float*result)
{
	*result = cos(angle);
	return S_OK;
}

HRESULT __stdcall CTrigoFunctions::getTangent(float angle, float*result)
{
	*result = tan(angle);

	return S_OK;
}

//CLASSFACTORY METHODS

ClassFactory::ClassFactory()
{
	m_cRef = 1;
}

ClassFactory::~ClassFactory()
{
	//no code for now 
}


HRESULT __stdcall ClassFactory::QueryInterface(REFIID riid, void**ppv)
{
	if ((riid == IID_IUnknown) && (riid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return 0;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return S_OK;
}

ULONG __stdcall ClassFactory::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall ClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}

	return m_cRef;
}

HRESULT __stdcall ClassFactory::CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void**ppv)
{
	HRESULT hr;
	CTrigoFunctions* pCTrigoFunc = NULL;

	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCTrigoFunc = new CTrigoFunctions;

	if (pCTrigoFunc == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCTrigoFunc ->QueryInterface(riid, ppv);

	if (FAILED(hr))
	{
		return E_NOINTERFACE;
	}

	pCTrigoFunc ->Release();
	return hr;
}

HRESULT __stdcall ClassFactory::LockServer(BOOL bLock) 
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
		PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L);
	}

	return S_OK;
}


HRESULT StartMyClassFactories(void)
{
	//variable declarations
	HRESULT hr;
	gpIClassFactory = new ClassFactory;
		
	if (gpIClassFactory == NULL)
	{
		E_OUTOFMEMORY;
	}

	gpIClassFactory->AddRef();

	//register the class factory  in COM's private database
	hr = CoRegisterClassObject(CLSID_CTrigoFunctions, static_cast<IUnknown*>(gpIClassFactory), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &dwRegister);

	if (FAILED(hr))
	{
		gpIClassFactory->Release();
		return E_FAIL;
	}

	return S_OK;
}

void StopMyClassFactories(void)
{
	// un register the classfactory

	if (dwRegister != 0)
	{
		CoRevokeClassObject(dwRegister);
	}

	if (gpIClassFactory != NULL)
	{
		gpIClassFactory->Release();
	}
}


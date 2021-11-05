#include <windows.h>
#include "exeServerWithRegFile.h"


LRESULT CALLBACK WndProc(HWND, UINT , WPARAM ,LPARAM);

class CSumSubtract : public ISum, ISubtract
{
private:
	long m_cRef;
public:
	CSumSubtract(void);
	~CSumSubtract(void);

	//IUnknown methods to be implemented
	HRESULT __stdcall QueryInterface(const IID &iid, void** ppv);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
};

class CSumSubtractClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	CSumSubtractClassFactory(void):m_cRef(1) {}
	~CSumSubtractClassFactory(void) {}

	//IUnknown methods to be implemented
	HRESULT __stdcall QueryInterface(const IID &iid, void** ppv);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	//IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

//some global variable declarations
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;
IClassFactory* gpIClassFactory = NULL;
HWND ghwnd = NULL;
DWORD dwRegister;


//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Function declarations
	HRESULT StartMyClassFactories(void);
	void StopMyClassFactories(void);
	DWORD GetParentProcessID(void);

	//variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	HRESULT hr;
	MSG msg;
	int iDontShowWindow = 0;
	TCHAR AppName[] = TEXT("ExeServer");
	wchar_t szTokens[] = L"-/";
	wchar_t *pszTokens;
	wchar_t lpszCmdLine[255];
	wchar_t* next_token = NULL;

	//com library initialization
	//GetParentProcessID();

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return 0;
	}

	MultiByteToWideChar(CP_ACP, 0, lpCmdLine, 255, lpszCmdLine, 255);

	pszTokens = wcstok_s(lpszCmdLine, szTokens, &next_token);

	while (pszTokens != NULL)
	{
		//embedding is when COM library calls this exe searching for CLSID
		if (_wcsicmp(pszTokens,L"embedding") == 0)
		{
			iDontShowWindow = 1;		//do not show the window but continue message loop
			break;
		}
		else
		{
			MessageBox(NULL, TEXT("Bad Command Line Arguments.\nExitting the Application."), TEXT("error"), MB_OK);
			exit(0);
		}
		pszTokens = wcstok_s(NULL, szTokens, &next_token);
	}

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = AppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	RegisterClassEx(&wndclass);

	//create Window
	hwnd = CreateWindow(AppName, TEXT("exe server with reg file"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);

	ghwnd = hwnd;

	if (iDontShowWindow != 1)
	{
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
		++glNumberOfServerLocks;
	}
	if (iDontShowWindow == 1)		//when com calls the program
	{
		hr = StartMyClassFactories();
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

		StopMyClassFactories();
	}

	CoUninitialize();
	return (int)msg.wParam;
}

//Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;


	switch (iMsg)
	{
		case WM_PAINT:

			GetClientRect(hwnd, &rc);
			hdc = BeginPaint(hwnd, &ps);
			SetBkColor(hdc, RGB(1, 1, 1));
			SetTextColor(hdc, RGB(0, 255, 0));
			DrawText(hdc, TEXT("COM Server exe only"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
			{
				PostQuitMessage(0);
			}
			break;
		case WM_CLOSE:
			--glNumberOfServerLocks;
			ShowWindow(hwnd, SW_HIDE);
			break;
		default:
			break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


//Implementation of class methods
//CSumSubtract methods

CSumSubtract::CSumSubtract(void):m_cRef(1)
{
	InterlockedIncrement(&glNumberOfActiveComponents);	//Increment global counter
}

CSumSubtract::~CSumSubtract(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);
}

//Implementation of CSumSubtract's IUnknown's Methods
HRESULT CSumSubtract::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (iid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (iid == IID_ISubtract)
	{
		*ppv = static_cast<ISubtract*>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return (S_OK);
}

ULONG __stdcall CSumSubtract::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}


ULONG __stdcall CSumSubtract::Release(void)
{

	if ( (InterlockedDecrement(&m_cRef)) == 0)
	{
		delete (this);	//delete before posting WM_Quit message
		if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
		{
			PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L);
		}

		return (0);
	}

	return m_cRef;
}

//ISUM

HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
	*pSum = num1 + num2;
	return(S_OK);
}

//Implementation of ISubtracts methods

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
	*pSubtract = num1 - num2;
	return (S_OK);
}

//Class factory methods
HRESULT CSumSubtractClassFactory::QueryInterface(const IID &iid, void **ppv)
{
	if (iid == IID_IUnknown || iid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CSumSubtractClassFactory::AddRef(void)
{
	return (InterlockedIncrement(&m_cRef));
}


ULONG CSumSubtractClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}

	return(m_cRef);
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, const IID &iid, void **ppv)
{
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;
	if (pUnkOuter != NULL)
	{
		return (CLASS_E_NOAGGREGATION);
	}
	
	//create the instance of component i.e of CSumSubtract class 
	pCSumSubtract = new CSumSubtract;

	if (pCSumSubtract == NULL)
	{
		return (E_OUTOFMEMORY);
	}
	
	//get requested interface
	hr = pCSumSubtract->QueryInterface(iid, ppv);
	pCSumSubtract->Release();

	return (hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&glNumberOfServerLocks);
	}
	else
	{
		InterlockedDecrement(&glNumberOfServerLocks);
	}

	if (glNumberOfActiveComponents == 0 && glNumberOfServerLocks == 0)
	{
		PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L);
	}

	return (S_OK);
}

HRESULT StartMyClassFactories(void)
{
	HRESULT hr;

	gpIClassFactory = new CSumSubtractClassFactory;
	if (gpIClassFactory == NULL)
	{
		return (E_OUTOFMEMORY);
	}

	gpIClassFactory->AddRef();
	//register the class factory in COMS private database
	
	hr = CoRegisterClassObject(CLSID_SumSubtract, static_cast<IUnknown*>(gpIClassFactory), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &dwRegister);

	if (FAILED(hr))
	{
		gpIClassFactory->Release();
		return(E_FAIL);
	}

	return (S_OK);
}


void StopMyClassFactories(void)
{
	if (dwRegister != 0)
	{
		CoRevokeClassObject(dwRegister);
	}
	if (gpIClassFactory != NULL)
	{
		gpIClassFactory->Release();
	}
}

/*
DWORD GetParentProcess(void)
{
	HANDLE hProcessSnapShot = NULL;
	BOOL bRetCode = FALSE;
	PROCESSENTRY32 ProcessEntry = { 0 };
	DWORD
}
*/




















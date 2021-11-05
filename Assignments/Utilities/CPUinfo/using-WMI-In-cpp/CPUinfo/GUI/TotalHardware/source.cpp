#include <Windows.h>
#include <WbemIdl.h> //for IWbemLocator,CLSID_WbemLocator,IID_IWbemLocator
#include <string.h>
#include <iostream> //wstring
#include <Commctrl.h>	//tabbed controls
#include "resource.h"
#include <comdef.h> //_bstr_t 


using std::wstring;
using std::string;

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000
#define TYPES_OF_TABS 7
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"wbemuuid.lib")

HINSTANCE ghInstance;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct CpuInfo
{
	TCHAR caption[32];
	TCHAR description[64];
	TCHAR name[32];
	TCHAR codename[32];
	TCHAR type[32];
	unsigned int stepping;
	TCHAR architecture[32];
	unsigned int addresswidth;
	unsigned int numberofcores;
	TCHAR assettag[32];
	TCHAR availabilitystatus[32];
	unsigned int datawidth;
	unsigned int maximumclockspeed;
	unsigned int characteristics;
	TCHAR cpustatus[64];
	unsigned int currentvoltage;
	unsigned int currentclockspeed;
	unsigned int deviceid;
	unsigned int extclock;
	unsigned int L2cachesize;
	unsigned int L2cachespeed;
	unsigned int L3cachesize;
	unsigned int L3cachespeed;
	unsigned int numberof_enabledcores;
	unsigned int numberof_logicalprocessors;
	TCHAR role[32];
	TCHAR systemname[32];
	BOOL virtualization_enabled;
};

struct GpuInfo
{
	TCHAR* caption[2];
	unsigned int acceleratorCapabilities[2];
	TCHAR* adapterCompatibility[2];
	TCHAR* adapterDACType[2];
	unsigned int adapterRAM[2];
	unsigned int currentHorizontalResolution[2];
	unsigned int currentVerticalResolution[2];
	TCHAR* Description[2];
	unsigned int currentBitsPerPixel[2];
	unsigned long long currentNumberOfColors[2];
	unsigned int currentRefreshRate[2];
	unsigned int currentScanMode[2];
	TCHAR* driverData[2];
	TCHAR* driverVersion[2];
	TCHAR* status[2];
	unsigned int videoArchitechture[2];
	unsigned int videoMemType[2];
	TCHAR* videoModeDescription[2];
	TCHAR* videoProcessor[2];
	TCHAR* deviceID[2];
};



struct MotherBoard
{
	TCHAR caption[32];
	TCHAR  PrimaryBusType[128];
	TCHAR  SecondaryBusType[128];
	TCHAR  Status[128];
};

struct BiosInfo
{
	TCHAR caption[32];
	TCHAR SMBiosBiosVersion[64];
	TCHAR Manufacturer[64];
	TCHAR Name[64];
	TCHAR SerialNumber[64];
	TCHAR Version[64];
};

struct NetworkInfo
{
	TCHAR Caption[64];
	TCHAR DefaultIPGateway[64];
	TCHAR IPAddress[64];
	TCHAR MACAddress[64];
	TCHAR IPSubnet[64];
	TCHAR DNSHostName[64];
	TCHAR ServiceName[64];
	TCHAR Description[64];

};


struct memory
{
	TCHAR caption[64];
	unsigned long capacity;
	unsigned int configuredClockSpeed;
	unsigned int configuredVoltage;
	TCHAR deviceLocator[64];
	unsigned int formFactor;
	unsigned int memoryType;
	TCHAR manufacturer[64];
	TCHAR status[64];
	unsigned int totalWidth;
	TCHAR bankLabel[64];
};




CpuInfo cpuinfo;
MotherBoard motherboard;
BiosInfo biosinfo;
GpuInfo gpuinfo;
NetworkInfo netinfo;
memory meminfo;
static HWND hwndTab;
static HWND hwndStatic;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	
	HWND hwnd;
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = TEXT("myTabbedWindow");

	HWND createTabbedWindow(HWND);
	HWND createTabContent(HWND);

	ghInstance = hInstance;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("myWindow"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		MessageBox(NULL, TEXT("createWindow failed"), TEXT("error"), MB_OK);
		return 1;
	}

	hwndTab = createTabbedWindow(hwnd);
	hwndStatic = createTabContent(hwndTab);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg , WPARAM wParam, LPARAM lParam)
{
	
	HRESULT hres;
	
	BOOL onNotifyResponse(HWND, HWND, LPARAM);
	BOOL getCpuInfo(IWbemLocator*, IWbemServices*);
	BOOL getGraphicsInfo(IWbemLocator* pLoc,IWbemServices* pSvc);
	BOOL getExtGraphicsInfo(IWbemLocator*pLoc, IWbemServices*pSvc);
	BOOL getMotherBoardInfo(IWbemLocator* pLoc, IWbemServices*  pSvc);
	BOOL getBiosInfo(IWbemLocator* pLoc, IWbemServices* pSvc);
	BOOL getNetworkInfo(IWbemLocator* pLoc, IWbemServices* pSvc);
	BOOL getMemoryInfo(IWbemLocator* pLoc, IWbemServices* pSvc);
	void initializeFirstTab(HWND);


	void uninitialize(void);

	IWbemServices *pSvc = 0;
	IWbemLocator *pLoc = 0;

	switch (iMsg)
	{
		case WM_CREATE:
			
			//Initialize COM
			hres = CoInitialize(NULL);
			if (FAILED(hres))
			{
				MessageBox(NULL, TEXT("Coinitialize failed"), TEXT("error"), MB_OK);
				break;
			}

			//general COM security settings

			hres = CoInitializeSecurity(
				NULL,
				-1,		//COM negotiates services
				NULL,	//Authentication servies
				NULL,	//Reserved
				RPC_C_AUTHN_LEVEL_DEFAULT, //type of authentication : default
				RPC_C_IMP_LEVEL_IMPERSONATE, //Type of impersonation : default
				NULL, //Authentication info
				EOAC_NONE,	//Additional capabilities
				NULL);		//reserved

			if (FAILED(hres))
			{
				MessageBox(NULL, TEXT("failed to initialize security"), TEXT("error"), MB_OK);
				CoUninitialize();
				break;
			}

			//create WMI connection 
			//obtain initial location to windows management instrumentation on local machine
			
			
			hres = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
			if (FAILED(hres))
			{
				MessageBox(NULL, TEXT("CoCreateInstance for wbemLocator failed"), TEXT("error"), MB_OK);
				CoUninitialize();
				break;
			}

			//Interface pointer
			
			//using pLoc connect to the root namespace and obtain IWbemServices interface pointer

			hres = pLoc->ConnectServer(
				_bstr_t(L"ROOT\\CIMV2"), //WMI namespace path
				NULL,//Username
				NULL,//User password
				0,//Locale
				NULL,//Security Flags
				0,//Authority
				0,//Context Object
				&pSvc//IWbemServices proxy
			);

			if (FAILED(hres))
			{
				MessageBox(NULL, TEXT("failed to get IWbemServices Interface "), TEXT("error"), MB_OK);
				pLoc->Release();
				CoUninitialize();
				break;
			}
			else
			{
				MessageBox(NULL, TEXT("connection to root\\cmv2 wmi namespace success !"), TEXT("status report"), MB_OK);
			}

			//setting protocol rules for WMI connection
			// proxy security
			// IWbemServices proxy so that impersonation of user (client) occurs

			hres = CoSetProxyBlanket(
				pSvc, //the proxy about to be set
				RPC_C_AUTHN_WINNT,	//authentication services
				RPC_C_AUTHZ_NONE,	//authorization services
				NULL,				//Server principal name
				RPC_C_AUTHN_LEVEL_CALL,	//authentication level
				RPC_C_IMP_LEVEL_IMPERSONATE,	//impersonation level
				NULL,				//client identity
				EOAC_NONE			//proxy capabilities
			);

			if (FAILED(hres))
			{
				MessageBox(NULL, TEXT("failed to set proxy for WMI connection"), TEXT("report"), MB_OK);
				pSvc->Release();
				pLoc->Release();
				CoUninitialize();
				break;
			}
			/*
			//now use IWbemServices pointer to make requests to WMI
			if ( (getCpuInfo(pLoc, pSvc)) == FALSE)
			{
				MessageBox(NULL, TEXT("getCPUinfo failed"), TEXT("failed"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}
			*/
			if ((getGraphicsInfo(pLoc, pSvc)) == FALSE )
			{
				MessageBox(NULL, TEXT("getGraphicsinfo failed"), TEXT("failed"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}

			/*
			if ( (getMotherBoardInfo(pLoc, pSvc)) == FALSE)
			{
				MessageBox(NULL, TEXT("getMOtherboardinfo failed"), TEXT("failed"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}
			if ( (getBiosInfo(pLoc, pSvc)) == FALSE )
			{
				MessageBox(NULL, TEXT("getBIOSinfo failed"), TEXT("failed"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}

			if ( (getNetworkInfo(pLoc, pSvc)) == FALSE )
			{
				MessageBox(NULL, TEXT("getNetworkinfo failed"), TEXT("failed"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}

			if ( (getMemoryInfo(pLoc, pSvc)) == FALSE )
			{
				MessageBox(NULL, TEXT("getMemoryinfo failed"), TEXT("failed"), MB_OK);
				DestroyWindow(hwnd);
				exit(0);
			}
			*/
			//setup tabs

			break;
		case WM_SIZE:			
			initializeFirstTab(hwndStatic);

			SetWindowPos(hwndTab, HWND_TOP, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_SHOWWINDOW);
			break;
		case WM_NOTIFY:
			 return (onNotifyResponse(hwndTab,hwndStatic,lParam));
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;
				default:
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}


HWND createTabbedWindow(HWND hwnd)
{
	HWND hTab;
	TCHAR achTemp[256];
	RECT rect;
	INITCOMMONCONTROLSEX icex; //size and structure 
	TCITEM tie; //Each tab in a tab control consists of an icon, a label, and application-defined data.
				//This information is provided by this structure 
	int i;
	
	//Initializing common controls
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	//now get parent windows client area 
	//create tabbed child window............
	GetClientRect(hwnd, &rect);

	hTab = CreateWindow(WC_TABCONTROL, TEXT("tabbedWindowChild"), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, rect.right, rect.bottom, hwnd, NULL, ghInstance, NULL);
	if (!hTab)
	{
		MessageBox(NULL, TEXT("failed to create tabbed child window"), TEXT("error"), MB_OK);
	}

	//Add extra tabs here ..................
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = achTemp;

	for (i = 0; i < TYPES_OF_TABS; i++)
	{
		LoadString(ghInstance, IDS_CPU + i, achTemp, sizeof(achTemp) / sizeof(achTemp[0]));

		if (TabCtrl_InsertItem(hTab, i, &tie) == -1)
		{
			DestroyWindow(hTab);
		}
	}
	
	return (hTab);
}


HWND createTabContent(HWND hwndTab)
{
	HWND hwndStatic;

	hwndStatic = CreateWindow(WC_STATIC, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 50, 800, 700, hwndTab, NULL, ghInstance, NULL);

	if (!hwndStatic)
	{
		MessageBox(NULL, TEXT("failed to create window inside tabbed window"), TEXT("error"), MB_OK);
	}

	return (hwndStatic);
}

void initializeFirstTab(HWND hwndStatic)
{
	MessageBox(NULL, TEXT("call reaching here"), TEXT("error"), MB_OK);
	void printCPUInfo(HDC);
	RECT rect;
	HDC hdc;
	hdc = GetDC(hwndStatic);
	GetClientRect(hwndStatic, &rect);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 0));

	FillRect(hdc, &rect, CreateSolidBrush(RGB(0,0,0)));
	printCPUInfo(hdc);
}


BOOL onNotifyResponse(HWND hwndTab, HWND hwndStatic, LPARAM lParam)
{
	void initializeFirstTab(HWND);
	void printCPUInfo(HDC);
	void printGPUInfo(HDC);
	void printBIOSInfo(HDC);
	void printMEMInfo(HDC);
	void printNETInfo(HDC);
	void printCacheInfo(HDC);
	void printBoardInfo(HDC);


	TCHAR achTemp[256]; //temp buffer for strings
	LRESULT result;
	static int iPage;
	LPCSTR sometext = _bstr_t("this is my text");
	HDC hdc;
	TCHAR struint[10] = {" "};
	TCHAR stringp[32] = {" "};
	RECT rect;

	HFONT largeFont = CreateFont(20, 20, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, NULL);
	HFONT smallFont = CreateFont(5, 25, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, NULL);

	
	//Sent by a common control to its parent window when an event has occurred in the 
	//control or the control requires some kind of information. 
	//idCtrl = (int)wParam;
	//pnmh = (LPNMHDR)lParam
	/*	typedef struct tagNMHDR {
	HWND hwndFrom;
	UINT idFrom;
	UINT code;
	} NMHDR
	*/
	switch (((LPNMHDR)lParam)->code)
	{

	case TCN_SELCHANGING:
		return FALSE;
	case TCN_SELCHANGE:
		
		iPage = TabCtrl_GetCurSel(hwndTab);
		hdc = GetDC(hwndStatic);
		GetClientRect(hwndStatic, &rect);
		FillRect(hdc,&rect,CreateSolidBrush(RGB(0, 0, 0)));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 0));

		switch (iPage)
		{
		case 0:
			//printCPUInfo(hdc);
			break;
		case 1:
			//printCacheInfo(hdc);
			break;
		case 2:
			//printBoardInfo(hdc);
			break;
		case 3:
			//printMEMInfo(hdc);
			break;
		case 4:
			printGPUInfo(hdc);
			break;
		case 5:
			//printNETInfo(hdc);
			break;
		case 6:
			//printBIOSInfo(hdc);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return TRUE;
}


BOOL getCpuInfo(IWbemLocator* pLoc, IWbemServices* pSvc)
{
	IWbemClassObject* pclsObject;
	ULONG uReturn = 0;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hres;


	hres = pSvc->ExecQuery
	(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_Processor"), //class from wmi which has all cpu info
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hres))
	{
		MessageBox(NULL, TEXT("failed to execQuery for Win32_processor class"), TEXT("report"), MB_OK);
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return FALSE;
	}

	//finally fetch the data from query


	while (pEnumerator)
	{
		hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		hres = pclsObject->Get(L"Name", 0, &vtProp, 0, 0);

		if (SUCCEEDED(hres))
		{		
			strcpy(cpuinfo.name, _bstr_t(vtProp.bstrVal));
		}

		hres = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);

		if (SUCCEEDED(hres))
		{
			strcpy(cpuinfo.caption, _bstr_t(vtProp.bstrVal));
		}


		hres = pclsObject->Get(L"Description", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hres))
		{
			strcpy(cpuinfo.description, _bstr_t(vtProp.bstrVal));
		}

		hres = pclsObject->Get(L"ProcessorID", 0, &vtProp, 0, 0);

		if (SUCCEEDED(hres))
		{
			wstring tmp(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));

			size_t found;
			if ((found = tmp.find(L"506")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Skylake");

			}
			else if ((found = tmp.find(L"606")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Kabylake");

			}
			else if ((found = tmp.find(L"406")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Haswell");
			}
			else if ((found = tmp.find(L"306")) != string::npos)
			{
			
				strcpy(cpuinfo.codename, "Ivybridge");

			}
			else if ((found = tmp.find(L"206")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Sandybride");
			}
		}


		hres = pclsObject->Get(L"ProcessorType", 0, &vtProp, 0, 0);
		switch (vtProp.uintVal)
		{
		case 1:
			
			strcpy(cpuinfo.type, "other");

			break;
		case 2:
	
			strcpy(cpuinfo.type, "Unknown");

			break;
		case 3:

			strcpy(cpuinfo.type, "Central Processor");

			break;
		case 4:
			
			strcpy(cpuinfo.type, "Math Processor");

			break;
		case 5:
			strcpy(cpuinfo.type, "DSP processor");
			break;

		case 6:
			strcpy(cpuinfo.type, "video processor");
		
			break;
		default:
			break;
		}

		hres = pclsObject->Get(L"Stepping ", 0, &vtProp, 0, 0);
		cpuinfo.stepping = vtProp.uintVal;
		
		hres = pclsObject->Get(L"Architecture", 0, &vtProp, 0, 0);
		switch (vtProp.uintVal)
		{
		case 0:
			strcpy(cpuinfo.architecture, "x86");
			
			break;
		case 1:
		
			strcpy(cpuinfo.architecture, "MIPS");
			
			break;
		case 2:

			strcpy(cpuinfo.architecture, "Alpha");

			break;
		
		case 3:
			
			strcpy(cpuinfo.architecture, "PowerPC");
			break;
		case 5:
			
			strcpy(cpuinfo.architecture, "ARM");

			break;
		case 6:
			
			strcpy(cpuinfo.architecture, "ia64");

			break;
		case 9:
			strcpy(cpuinfo.architecture, "x64");

			break;
		}

		
		hres = pclsObject->Get(L"AddressWidth", 0, &vtProp, 0, 0);
		cpuinfo.addresswidth = vtProp.uintVal;

		hres = pclsObject->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
		cpuinfo.numberofcores = vtProp.uintVal;
		
		hres = pclsObject->Get(L"AssetTag", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.assettag, _bstr_t(vtProp.bstrVal));

		hres = pclsObject->Get(L"Availability", 0, &vtProp, 0, 0);
		switch (vtProp.uintVal)
		{
		case 3:
			
			strcpy(cpuinfo.availabilitystatus, "(3) Running Full power");

			break;
		case 4:

			strcpy(cpuinfo.availabilitystatus, "(4) Warning");

			break;
		case 5:
			
			strcpy(cpuinfo.availabilitystatus, "(5) InTest");

			break;
		case 6:

			strcpy(cpuinfo.availabilitystatus, "Not Applicable");

			break;
		case 7:
			strcpy(cpuinfo.availabilitystatus, "Power Off");

			break;
		case 8:
			
			strcpy(cpuinfo.availabilitystatus, "Off Line");

			break;
		case 9:
			
			strcpy(cpuinfo.availabilitystatus, "Off Duty");

			break;
		case 10:
			
			strcpy(cpuinfo.availabilitystatus, "Degraded");

			break;
		case 11:
			strcpy(cpuinfo.availabilitystatus, "Not Installed");

			break;
		case 12:
			strcpy(cpuinfo.availabilitystatus, "Install Error");
			break;
		case 13:
			strcpy(cpuinfo.availabilitystatus, "Power save unknown");

			break;
		case 14:
			strcpy(cpuinfo.availabilitystatus, "power save low power mode");

			break;
		case 15:
			
			strcpy(cpuinfo.availabilitystatus, "Power save stand by");

			break;
		case 16:
			
			strcpy(cpuinfo.availabilitystatus, "Power Cycle");

			break;
		case 17:
			
			strcpy(cpuinfo.availabilitystatus, "Power save Warning state");

			break;
		case 18:
			
			strcpy(cpuinfo.availabilitystatus, "Paused");

			break;
		case 19:
			
			strcpy(cpuinfo.availabilitystatus, "Not Ready");

			break;
		case 20:
			
			strcpy(cpuinfo.availabilitystatus, "Not configured");

			break;
		default:
			
			strcpy(cpuinfo.availabilitystatus, "other");
			break;
		}


		hres = pclsObject->Get(L"DataWidth", 0, &vtProp, 0, 0);
		cpuinfo.datawidth = vtProp.uintVal;


		hres = pclsObject->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
		cpuinfo.maximumclockspeed = vtProp.uintVal;

		hres = pclsObject->Get(L"Characteristics", 0, &vtProp, 0, 0);
		cpuinfo.characteristics = vtProp.uintVal;
		

		hres = pclsObject->Get(L"CpuStatus", 0, &vtProp, 0, 0);

		switch (vtProp.uintVal)
		{
		case 0:
			
			strcpy(cpuinfo.cpustatus, "Unknown");

			break;
		case 1:
			
			strcpy(cpuinfo.cpustatus, "Cpu Enabled");

			break;
		case 2:
			
			strcpy(cpuinfo.cpustatus, "Cpu Disabled by user using BIOS setup");

			break;
		case 3:
			
			strcpy(cpuinfo.cpustatus, "Cpu Disabled by BIOS POST Error");

			break;
		case 4:
			
			strcpy(cpuinfo.cpustatus, "Cpu is Idle");

			break;
		case 5:
		case 6:
			
			strcpy(cpuinfo.cpustatus, "Reserved");

			break;
		default:
			
			strcpy(cpuinfo.cpustatus, "Other");

			break;
		}

		

		hres = pclsObject->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
		cpuinfo.currentclockspeed = vtProp.uintVal;

		hres = pclsObject->Get(L"CurrentVoltage", 0, &vtProp, 0, 0);
		cpuinfo.currentvoltage = vtProp.uintVal;

		hres = pclsObject->Get(L"Device ID", 0, &vtProp, 0, 0);
		cpuinfo.deviceid = vtProp.uintVal;

		hres = pclsObject->Get(L"ExtClock", 0, &vtProp, 0, 0);
		cpuinfo.extclock = vtProp.uintVal;


			

		hres = pclsObject->Get(L"L2CacheSize", 0, &vtProp, 0, 0);
		cpuinfo.L2cachesize = vtProp.uintVal;

		hres = pclsObject->Get(L"L2CacheSpeed", 0, &vtProp, 0, 0);
		cpuinfo.L2cachespeed = vtProp.uintVal;

		hres = pclsObject->Get(L"L3CacheSize", 0, &vtProp, 0, 0);
		cpuinfo.L3cachesize = vtProp.uintVal;

		hres = pclsObject->Get(L"L3CacheSpeed", 0, &vtProp, 0, 0);
		cpuinfo.L3cachespeed = vtProp.uintVal;

		hres = pclsObject->Get(L"NumberOfEnabledCore", 0, &vtProp, 0, 0);
		cpuinfo.numberof_enabledcores = vtProp.uintVal;

		hres = pclsObject->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
		cpuinfo.numberof_logicalprocessors = vtProp.uintVal;

		hres = pclsObject->Get(L"Role", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.role, _bstr_t(vtProp.bstrVal));

		hres = pclsObject->Get(L"SystemName", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.systemname, _bstr_t(vtProp.bstrVal));

		hres = pclsObject->Get(L"VirtualizationFirmwareEnabled ", 0, &vtProp, 0, 0);
		cpuinfo.virtualization_enabled = vtProp.boolVal;

		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;
	}

	return TRUE;
}

BOOL getMotherBoardInfo(IWbemLocator* pLoc, IWbemServices* pSvc)
{
	IWbemClassObject* pclsObject;
	ULONG uReturn = 0;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hr;
	unsigned int uCount = 0;

	hr = pSvc->ExecQuery
	(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_MotherboardDevice"), //class from wmi which has all cpu info
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("failed to execQuery for Win32_MotherBoard class"), TEXT("report"), MB_OK);
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return FALSE;
	}

	//finally fetch the data from query


	while (pEnumerator)
	{
		hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);

		uCount += uReturn;
		if (0 == uReturn)
		{
			if (uCount > 0)
			{
				return TRUE;
			}
			else
			{
				MessageBox(NULL, TEXT("here"), TEXT("here"), MB_OK);
				return FALSE;
			}
		}

		VARIANT vtProp;
		TCHAR str[64];

		hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(motherboard.caption, _bstr_t(vtProp.bstrVal));
			VariantClear(&vtProp);

		hr = pclsObject->Get(L"PrimaryBusType", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			wsprintf(str, "%ls", vtProp.bstrVal);
			strcpy(motherboard.PrimaryBusType, str);
			VariantClear(&vtProp);
		}
			
		hr = pclsObject->Get(L"SecondaryBusType", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			wsprintf(str, "%ls", vtProp.bstrVal);
			strcpy(motherboard.SecondaryBusType, str);
			VariantClear(&vtProp);

		}
	
		hr = pclsObject->Get(L"Status",0, &vtProp, 0 , 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			wsprintf(str, "%ls", vtProp.bstrVal);
			strcpy(motherboard.Status, str);
		}
			
		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;
	}

	return TRUE;
}


BOOL getGraphicsInfo(IWbemLocator* pLoc, IWbemServices* pSvc)
{
	IWbemClassObject* pclsObject;
	ULONG uReturn = 0;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hr;
	int uCount = 0;
	TCHAR str[10];

	hr = pSvc->ExecQuery
	(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_VideoController"), //class from wmi which has all cpu info
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("failed to execQuery for Win32_videocontroller class"), TEXT("report"), MB_OK);
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return FALSE;
	}

	//finally fetch the data from query


	while (pEnumerator)
	{

		hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);
		uCount += uReturn;
		wsprintf(str, "%d", uCount);
		MessageBox(NULL, str, TEXT("pEnumerator"), MB_OK);

		if (0 == uReturn)
		{
			if (uCount > 0)
			{
				//MessageBox(NULL, TEXT("returning from here"), TEXT("true"), MB_OK);
				return TRUE;
			}
			else
			{
				//MessageBox(NULL, TEXT("returning from here"), TEXT("false"), MB_OK);
				return FALSE;
			}
		}

		VARIANT vtProp;
		
		hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{

				gpuinfo.caption[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.caption[0], _bstr_t(vtProp.bstrVal));
			
			}
			else
			{
				gpuinfo.caption[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.caption[1], _bstr_t(vtProp.bstrVal));
			
			}

			VariantClear(&vtProp);

		}
	
		
		hr = pclsObject->Get(L"AdapterCompatibility", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.adapterCompatibility[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.adapterCompatibility[0], _bstr_t(vtProp.bstrVal));
			
			}
			else
			{
				gpuinfo.adapterCompatibility[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.adapterCompatibility[1], _bstr_t(vtProp.bstrVal));
			}
		
			VariantClear(&vtProp);
		}	

		
		hr = pclsObject->Get(L"AdapterDACType", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.adapterDACType[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.adapterDACType[0], _bstr_t(vtProp.bstrVal));
			}
			else
			{
				gpuinfo.adapterDACType[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.adapterDACType[1], _bstr_t(vtProp.bstrVal));
			
			}
			VariantClear(&vtProp);
		}

		
		hr = pclsObject->Get(L"AdapterRAM", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.adapterRAM[0] = vtProp.uintVal;

			}
			else
			{
				gpuinfo.adapterRAM[1] = vtProp.uintVal;
			}

			VariantClear(&vtProp);
		}

		
		
		hr = pclsObject->Get(L"Description", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.Description[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.Description[0], _bstr_t(vtProp.bstrVal));
			}
			else
			{
				gpuinfo.Description[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.Description[1], _bstr_t(vtProp.bstrVal));
			}

			VariantClear(&vtProp);
		}


		
		hr = pclsObject->Get(L"VideoModeDescription", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{

			if (uCount == 1)
			{
				gpuinfo.videoModeDescription[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.videoModeDescription[0], _bstr_t(vtProp.bstrVal));

			}
			else
			{
				gpuinfo.videoModeDescription[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.videoModeDescription[1], _bstr_t(vtProp.bstrVal));
			}
			
			VariantClear(&vtProp);
		}
		else
		{
			if (uCount == 1)
			{
				gpuinfo.videoModeDescription[1] = (TCHAR*)malloc(4);

				strcpy(gpuinfo.videoModeDescription[0], "NIL");
			}
			else
			{
				gpuinfo.videoModeDescription[1] = (TCHAR*)malloc(4);

				strcpy(gpuinfo.videoModeDescription[1], "NIL");
			}
		}
		
		
		hr = pclsObject->Get(L"CurrentBitsPerPixel", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.currentBitsPerPixel[0] = vtProp.intVal;

			}
			else
			{
				gpuinfo.currentBitsPerPixel[1] = vtProp.intVal;

			}
			VariantClear(&vtProp);

		}
		

		hr = pclsObject->Get(L"CurrentHorizontalResolution", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.currentHorizontalResolution[0] = vtProp.uintVal;
			}
			else
			{
				gpuinfo.currentHorizontalResolution[1] = vtProp.uintVal;
			}

			VariantClear(&vtProp);

		}
		
		
		hr = pclsObject->Get(L"CurrentVerticalResolution", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.currentVerticalResolution[0] = vtProp.uintVal;

			}
			else
			{
				gpuinfo.currentVerticalResolution[1] = vtProp.uintVal;
			}
			VariantClear(&vtProp);

		}


		hr = pclsObject->Get(L"CurrentNumberOfColors", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.currentNumberOfColors[0] = vtProp.ullVal;

			}
			else 
			{
				gpuinfo.currentNumberOfColors[1] = vtProp.ullVal;
			}

			VariantClear(&vtProp);

		}

		hr = pclsObject->Get(L"CurrentRefreshRate", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.currentRefreshRate[0] = vtProp.uintVal;

			}
			else
			{
				gpuinfo.currentRefreshRate[1] = vtProp.uintVal;

			}

			VariantClear(&vtProp);

		}

		hr = pclsObject->Get(L"CurrentScanMode", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.currentScanMode[0] = vtProp.uintVal;

			}
			else
			{
				gpuinfo.currentScanMode[1] = vtProp.uintVal;
			}

			VariantClear(&vtProp);

		}

		hr = pclsObject->Get(L"DriverVersion", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.driverVersion[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.driverVersion[0], _bstr_t(vtProp.bstrVal));

			}
			else
			{
				gpuinfo.driverVersion[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.driverVersion[1], _bstr_t(vtProp.bstrVal));
			}

			VariantClear(&vtProp);

		}

		hr = pclsObject->Get(L"DeviceID", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.deviceID[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.deviceID[0], _bstr_t(vtProp.bstrVal));


			}
			else
			{
				gpuinfo.deviceID[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.deviceID[1], _bstr_t(vtProp.bstrVal));

			}

			VariantClear(&vtProp);

		}

		hr = pclsObject->Get(L"Status", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.status[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.status[0], _bstr_t(vtProp.bstrVal));

			}
			else
			{
				gpuinfo.status[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.status[1], _bstr_t(vtProp.bstrVal));

			}
			
			VariantClear(&vtProp);

		}

		hr = pclsObject->Get(L"DriverVersion", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.driverVersion[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.driverVersion[0], _bstr_t(vtProp.bstrVal));

			}
			else
			{
				gpuinfo.driverVersion[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.driverVersion[1], _bstr_t(vtProp.bstrVal));

			}

			VariantClear(&vtProp);
		}
		
		
		hr = pclsObject->Get(L"VideoProcessor", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.videoProcessor[0] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.videoProcessor[0], _bstr_t(vtProp.bstrVal));

			}
			else
			{
				gpuinfo.videoProcessor[1] = (TCHAR*)malloc(SysStringByteLen(vtProp.bstrVal));
				strcpy(gpuinfo.videoProcessor[1], _bstr_t(vtProp.bstrVal));

			}

			VariantClear(&vtProp);

		}
			

		hr = pclsObject->Get(L"VideoArchitecture", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.videoArchitechture[0] = vtProp.intVal;

			}
			else
			{
				gpuinfo.videoArchitechture[1] = vtProp.intVal;

			}
				VariantClear(&vtProp);
		}
			
		hr = pclsObject->Get(L"VideoMemoryType", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
		{
			if (uCount == 1)
			{
				gpuinfo.videoMemType[0] = vtProp.intVal;

			}
			else
			{
				gpuinfo.videoMemType[1] = vtProp.intVal;
			}
		}


		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;
	}

	return TRUE;
}



BOOL getBiosInfo(IWbemLocator* pLoc, IWbemServices* pSvc)
{
	IWbemClassObject* pclsObject;
	ULONG uReturn = 0;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hr;
	unsigned int uCount = 0;

	hr = pSvc->ExecQuery
	(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_BIOS"), //class from wmi which has all cpu info
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);
		
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("failed to execQuery for Win32_baseBoard class"), TEXT("report"), MB_OK);
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return FALSE;
	}

	//finally fetch the data from query


	while (pEnumerator)
	{
		hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);
		uCount += uReturn;

		if (0 == uReturn)
		{
			if (uCount > 0)
			{
				return TRUE;
			}
			else
			{
				return FALSE;

			}
		}

		VARIANT vtProp;

		hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(biosinfo.caption, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"SMBIOSBIOSVersion", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(biosinfo.SMBiosBiosVersion, _bstr_t(vtProp.bstrVal));
		
		hr = pclsObject->Get(L"Name", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(biosinfo.Name, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"Version", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(biosinfo.Version, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(biosinfo.SerialNumber, _bstr_t(vtProp.bstrVal));
	
		hr = pclsObject->Get(L"Manufacturer", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(biosinfo.Manufacturer, _bstr_t(vtProp.bstrVal));

		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;
	}

	return TRUE;
}



BOOL getNetworkInfo(IWbemLocator* pLoc, IWbemServices* pSvc)
{
	IWbemClassObject* pclsObject;
	ULONG uReturn = 0;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hr;
	unsigned int uCount = 0;

	hr = pSvc->ExecQuery
	(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_NetworkAdapterConfiguration where IPEnabled= 'TRUE' "), //class from wmi which has all cpu info
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("failed to execQuery for Win32_baseBoard class"), TEXT("report"), MB_OK);
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return FALSE;
	}

	//finally fetch the data from query


	while (pEnumerator)
	{
		hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);
		uCount += uReturn;

		if (0 == uReturn)
		{
			if (uCount > 0)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		VARIANT vtProp;

		hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.Caption, _bstr_t(vtProp.bstrVal));
		
		hr = pclsObject->Get(L"IPAddress", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.IPAddress, _bstr_t(((BSTR*)(vtProp.parray->pvData))[0]));

		hr = pclsObject->Get(L"IPSubnet", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.IPSubnet, _bstr_t(((BSTR*)(vtProp.parray->pvData))[0]));

		hr = pclsObject->Get(L"MACAddress", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.MACAddress, _bstr_t(vtProp.bstrVal));

		
		hr = pclsObject->Get(L"DNSHostName", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.DNSHostName, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"ServiceName", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.ServiceName, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"Description", 0, &vtProp, 0, 0);
		if (hr == WBEM_S_NO_ERROR && V_VT(&vtProp) != VT_NULL)
			strcpy(netinfo.Description, _bstr_t(vtProp.bstrVal));

		
		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;

	}


	return TRUE;
}


BOOL getMemoryInfo(IWbemLocator* pLoc, IWbemServices* pSvc)
{
	IWbemClassObject* pclsObject;
	ULONG uReturn = 0;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hr;
	unsigned int uCount = 0;

	hr = pSvc->ExecQuery
	(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_PhysicalMemory"), //class from wmi which has all cpu info
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("failed to execQuery for Win32_baseBoard class"), TEXT("report"), MB_OK);
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return FALSE;
	}

	//finally fetch the data from query


	while (pEnumerator)
	{
		hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);
		uCount += uReturn;

		if (0 == uReturn)
		{
			if (uCount > 0)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		VARIANT vtProp;

		hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
		strcpy(meminfo.caption, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"Capacity", 0, &vtProp, 0, 0);
		meminfo.capacity = vtProp.ulVal;

		hr = pclsObject->Get(L"ConfiguredClockSpeed", 0, &vtProp, 0, 0);
		meminfo.configuredClockSpeed = vtProp.intVal;

		hr = pclsObject->Get(L"ConfiguredVoltage", 0, &vtProp, 0, 0);
		meminfo.configuredVoltage = vtProp.intVal;

		hr = pclsObject->Get(L"DeviceLocator", 0, &vtProp, 0, 0);
		strcpy(meminfo.deviceLocator, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"FormFactor", 0, &vtProp, 0, 0);
		meminfo.formFactor = vtProp.intVal;

		hr = pclsObject->Get(L"TotalWidth", 0, &vtProp, 0, 0);
		meminfo.totalWidth = vtProp.intVal;

		hr = pclsObject->Get(L"Manufacturer", 0, &vtProp, 0, 0);
		strcpy(meminfo.manufacturer, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"MemoryType", 0, &vtProp, 0, 0);
		meminfo.memoryType = vtProp.intVal;

		hr = pclsObject->Get(L"DeviceLocator", 0, &vtProp, 0, 0);
		strcpy(meminfo.deviceLocator, _bstr_t(vtProp.bstrVal));

		hr = pclsObject->Get(L"BankLabel", 0, &vtProp, 0, 0);
		strcpy(meminfo.bankLabel, _bstr_t(vtProp.bstrVal));

		
		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;

	}


	return TRUE;
}

void printCPUInfo(HDC hdc)
{
	TCHAR struint[10];

	TextOut(hdc, 300, 0, cpuinfo.name, sizeof(cpuinfo.name));
	SetTextColor(hdc, RGB(255, 255, 255));

	TextOut(hdc, 200, 60, TEXT("Specification:"), sizeof(TEXT("Specification:")));
	TextOut(hdc, 450, 60, cpuinfo.description, sizeof(cpuinfo.description));

	TextOut(hdc, 200, 80, TEXT("CodeName:"), sizeof(TEXT("CodeName:")));
	TextOut(hdc, 450, 80, cpuinfo.codename, sizeof(cpuinfo.codename));

	TextOut(hdc, 200, 100, TEXT("Type:"), sizeof(TEXT("Type:")));
	TextOut(hdc, 450, 100, cpuinfo.type, sizeof(cpuinfo.type));


	strcpy(struint, "          ");
	wsprintf(struint, "%d", cpuinfo.stepping);
	TextOut(hdc, 200, 120, TEXT("Stepping: "), sizeof(TEXT("Stepping: ")));
	TextOut(hdc, 450, 120, struint, sizeof(struint));


	TextOut(hdc, 200, 140, TEXT("Architechture: "), sizeof(TEXT("Architechture: ")));
	TextOut(hdc, 450, 140, cpuinfo.architecture, sizeof(cpuinfo.architecture));

	strcpy(struint, "          ");
	wsprintf(struint, "%d", cpuinfo.addresswidth);
	TextOut(hdc, 200, 160, TEXT("AddressWidth: "), sizeof(TEXT("AddressWidth: ")));
	TextOut(hdc, 450, 160, struint, sizeof(struint));


	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.numberofcores);
	TextOut(hdc, 200, 180, TEXT("NumberOfCores: "), sizeof(TEXT("NumberOfCores: ")));
	TextOut(hdc, 450, 180, struint, sizeof(struint));


	TextOut(hdc, 200, 200, TEXT("AvailabilityStatus:"), sizeof(TEXT("AvailabilityStatus:")));
	TextOut(hdc, 450, 200, cpuinfo.availabilitystatus, sizeof(cpuinfo.availabilitystatus));



	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.datawidth);
	TextOut(hdc, 200, 220, TEXT("DataWidth (bits): "), sizeof(TEXT("DataWidth (bits): ")));
	TextOut(hdc, 450, 220, struint, sizeof(struint));


	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.maximumclockspeed);
	TextOut(hdc, 200, 240, TEXT("MaximumClockSpeed: "), sizeof(TEXT("MaximumClockSpeed: ")));
	TextOut(hdc, 450, 240, struint, sizeof(struint));
	TextOut(hdc, 490, 240, TEXT("MHz"), sizeof(TEXT("MHz")));



	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.characteristics);
	TextOut(hdc, 200, 260, TEXT("Characteristics: "), sizeof(TEXT("Characteristics: ")));
	TextOut(hdc, 450, 260, struint, sizeof(struint));

	TextOut(hdc, 200, 280, TEXT("CPU Status:"), sizeof(TEXT("CPU Status:")));
	TextOut(hdc, 450, 280, cpuinfo.cpustatus, sizeof(cpuinfo.cpustatus));



	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.currentclockspeed);
	TextOut(hdc, 200, 300, TEXT("CurrentClock Speed: "), sizeof(TEXT("CurrentClock Speed: ")));
	TextOut(hdc, 450, 300, struint, sizeof(struint));
	TextOut(hdc, 490, 300, TEXT("MHz"), sizeof(TEXT("MHz")));



	strcpy(struint, "			");
	wsprintf(struint, "%d", cpuinfo.currentvoltage);
	TextOut(hdc, 200, 320, TEXT("Current Voltage: "), sizeof(TEXT("Current Voltage: ")));
	TextOut(hdc, 450, 320, struint, sizeof(struint));
	TextOut(hdc, 480, 320, TEXT("(tenth-volts)"), sizeof(TEXT("(tenth-volt)")));



	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.deviceid);
	TextOut(hdc, 200, 340, TEXT("Device ID: "), sizeof(TEXT("Device ID: ")));
	TextOut(hdc, 450, 340, struint, sizeof(struint));


	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.extclock);
	TextOut(hdc, 200, 360, TEXT("ExternalClockFrequency: "), sizeof(TEXT("ExternalClockFrequency: ")));
	TextOut(hdc, 450, 360, struint, sizeof(struint));
	TextOut(hdc, 480, 360, TEXT("MHz"), sizeof(TEXT("MHz")));

	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.numberof_enabledcores);
	TextOut(hdc, 200, 380, TEXT("Enabled Cores: "), sizeof(TEXT("Enabled Cores: ")));
	TextOut(hdc, 450, 380, struint, sizeof(struint));

	strcpy(struint, "		");
	wsprintf(struint, "%d", cpuinfo.numberof_logicalprocessors);
	TextOut(hdc, 200, 400, TEXT("Logical Cores: "), sizeof(TEXT("Logical Cores: ")));
	TextOut(hdc, 450, 400, struint, sizeof(struint));

	strcpy(struint, "		");
	TextOut(hdc, 200, 420, TEXT("Vitualization Enabled:"), sizeof(TEXT("Virtualization enabled:")));
	if (cpuinfo.virtualization_enabled == TRUE)
	{
		strcpy(struint, "TRUE");
	}
	else
	{
		strcpy(struint, "FALSE");
	}
	TextOut(hdc, 450, 420, struint, sizeof(struint));
}

void printCacheInfo(HDC hdc)
{
	TCHAR struint[8] = {"       "};

	TextOut(hdc, 300, 0, TEXT("Cache memory") , sizeof(TEXT("Cache memory")));

	SetTextColor(hdc, RGB(255, 255, 255));

	wsprintf(struint, "%d", cpuinfo.L2cachesize);
	TextOut(hdc, 200, 60, TEXT("L2 cache size"), sizeof(TEXT("L2 cache Size")));
	TextOut(hdc, 450, 60, struint, sizeof(struint));
	TextOut(hdc, 500, 60, TEXT("KB"), sizeof(TEXT("LB")));

	wsprintf(struint, "%d", cpuinfo.L2cachespeed);
	TextOut(hdc, 200, 80, TEXT("L2 cache speed"), sizeof(TEXT("L2 cache Speed")));
	TextOut(hdc, 450, 80, struint, sizeof(struint));
	TextOut(hdc, 500, 80, TEXT("MHz"), sizeof(TEXT("MHz")));

	wsprintf(struint, "%d", cpuinfo.L3cachesize);
	TextOut(hdc, 200, 100, TEXT("L3 cache size"), sizeof(TEXT("L3 cache Size")));
	TextOut(hdc, 450, 100, struint, sizeof(struint));
	TextOut(hdc, 500, 100, TEXT("KB"), sizeof(TEXT("KB")));

	wsprintf(struint, "%d", cpuinfo.L3cachespeed);
	TextOut(hdc, 200, 120, TEXT("L3 cache size"), sizeof(TEXT("L3 cache Size")));
	TextOut(hdc, 450, 120, struint, sizeof(struint));
	TextOut(hdc, 500, 120, TEXT("MHz"), sizeof(TEXT("MHz")));
}


void printBoardInfo(HDC hdc)
{
	SetTextColor(hdc, RGB(255, 255, 0));
	TextOut(hdc, 300, 0, motherboard.caption, sizeof(motherboard.caption));

	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 200, 60, TEXT("Primary Bus Type:"), sizeof(TEXT("Primary Bus Type:")));
	TextOut(hdc, 380, 60, motherboard.PrimaryBusType, sizeof(motherboard.PrimaryBusType));

	TextOut(hdc, 200, 80, TEXT("Secondary Bus Type:"), sizeof(TEXT("Secondary Bus Type:")));
	TextOut(hdc, 380, 80, motherboard.SecondaryBusType, sizeof(motherboard.SecondaryBusType));

	TextOut(hdc, 200, 100, TEXT("Status:"), sizeof(TEXT("Status:")));
	TextOut(hdc, 380, 100, motherboard.Status, sizeof(motherboard.Status));

}

void printMEMInfo(HDC hdc)
{
	TCHAR struint[10] = {" "};
	TCHAR stringp[32] = {" "};

	SetTextColor(hdc, RGB(255, 255, 0));
	TextOut(hdc, 300, 0, meminfo.caption, sizeof(meminfo.caption));

	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 200, 60, TEXT("Manufacturer:"), sizeof(TEXT("Manufacturer:")));
	TextOut(hdc, 450, 60, meminfo.manufacturer, sizeof(meminfo.manufacturer));

	TextOut(hdc, 200, 80, TEXT("Status:"), sizeof(TEXT("Status:")));
	TextOut(hdc, 450, 80, meminfo.status, sizeof(meminfo.status));

	TCHAR str1[10];
	TextOut(hdc, 200, 100, TEXT("Capacity:"), sizeof(TEXT("Capacity:")));
	sprintf(str1, "%lu", meminfo.capacity);
	TextOut(hdc, 450, 100, str1, sizeof(str1));

	TextOut(hdc, 200, 120, TEXT("ConfiguredClockSpeed:"), sizeof(TEXT("ConfiguredClockSpeed:")));
	wsprintf(struint, "%lu", meminfo.configuredClockSpeed);
	TextOut(hdc, 450, 120, struint, sizeof(struint));
	TextOut(hdc, 490, 120, TEXT("MHz"), sizeof(TEXT("MHz")));

	TextOut(hdc, 200, 140, TEXT("ConfiguredVoltage:"), sizeof(TEXT("ConfiguredVoltage:")));
	wsprintf(struint, "%u", meminfo.configuredVoltage);
	TextOut(hdc, 450, 140, struint, sizeof(struint));
	TextOut(hdc, 490, 140, TEXT("mV"), sizeof(TEXT("mV")));

	TextOut(hdc, 200, 160, TEXT("DeviceLocator:"), sizeof(TEXT("DeviceLocator:")));
	TextOut(hdc, 450, 160, meminfo.deviceLocator, sizeof(meminfo.deviceLocator));
	strcpy(struint, "      ");
	TextOut(hdc, 200, 180, TEXT("TotalWidth:"), sizeof(TEXT("TotalWidth:")));
	wsprintf(struint, "%u", meminfo.totalWidth);
	TextOut(hdc, 450, 180, struint, sizeof(struint));
	TextOut(hdc, 470, 180, TEXT("bits"), sizeof(TEXT("bits")));

	switch (meminfo.formFactor)
	{
	case 0:
		strcpy(str1, "Unknown");
		break;
	case 1:
		strcpy(str1, "Other");
		break;
	case 2:
		strcpy(str1, "SIP");

		break;
	case 3:
		strcpy(str1, "DIP");

		break;
	case 4:
		strcpy(str1, "ZIP");

		break;
	case 5:
		strcpy(str1, "SOJ");

		break;
	case 6:
		strcpy(str1, "Proprietary");

		break;
	case 7:
		strcpy(str1, "SIMM");

		break;
	case 8:
		strcpy(str1, "DIMM");

		break;
	case 9:
		strcpy(str1, "TSOP");

		break;
	case 10:
		strcpy(str1, "PGA");

		break;
	case 11:
		strcpy(str1, "RIMM");

		break;
	case 12:
		strcpy(str1, "SODIMM");

		break;
	case 13:
		strcpy(str1, "SRIMM");

		break;
	case 14:
		strcpy(str1, "SMD");

		break;
	case 15:
		strcpy(str1, "SSMP");

		break;
	case 16:
		strcpy(str1, "QFP");
		break;
	default:
		strcpy(str1, "Unknown");
		break;
	}

	TextOut(hdc, 200, 200, TEXT("FormFactor:"), sizeof(TEXT("FormFactor:")));
	wsprintf(stringp, "(%u) %s", meminfo.formFactor, str1);
	TextOut(hdc, 450, 200, stringp, sizeof(stringp));

	TextOut(hdc, 200, 220, TEXT("Bank Label:"), sizeof(TEXT("Bank Label:")));
	TextOut(hdc, 450, 220, meminfo.bankLabel, sizeof(meminfo.bankLabel));

}

void printGPUInfo(HDC hdc)
{
	TCHAR str[64] = {" "};
	
	TextOut(hdc, 250, 0, gpuinfo.caption[0], sizeof(TCHAR)*strlen(gpuinfo.caption[0]) );
	TextOut(hdc, 500, 0, gpuinfo.caption[1], sizeof(TCHAR)*strlen(gpuinfo.caption[1]));

	
	SetTextColor(hdc,(RGB(255, 255, 255)));
	TextOut(hdc, 50, 60, TEXT("Chipset:"), sizeof(TEXT("Chipset:")));
	TextOut(hdc, 250, 60, gpuinfo.adapterCompatibility[0], sizeof(TCHAR)*strlen(gpuinfo.adapterCompatibility[0]));
	TextOut(hdc, 500, 60, gpuinfo.adapterCompatibility[1], sizeof(TCHAR)*strlen(gpuinfo.adapterCompatibility[1]));

	
	TextOut(hdc, 50, 80, TEXT("adapterDACType:"), sizeof(TEXT("adapterDACType:")));
	TextOut(hdc, 250, 80, gpuinfo.adapterDACType[0], sizeof(TCHAR)*strlen(gpuinfo.adapterDACType[0]));
	TextOut(hdc, 500, 80, gpuinfo.adapterDACType[1], sizeof(TCHAR)*strlen(gpuinfo.adapterDACType[1]));

	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.adapterRAM[0] / (1024*1024));
	TextOut(hdc, 50, 100, TEXT("AdapterRAM:"), sizeof(TEXT("AdapterRAM:")));
	TextOut(hdc, 250, 100, str, sizeof(str));
	sprintf(str, "%u", gpuinfo.adapterRAM[1] / (1024 * 1024));
	TextOut(hdc, 500, 100, str, sizeof(str));

	
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentBitsPerPixel[0]);
	TextOut(hdc, 50, 120, TEXT("CurrentBitsPerPixel:"), sizeof(TEXT("CurrentBitsPerPixel:")));
	TextOut(hdc, 250, 120, str, sizeof(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentBitsPerPixel[1]);
	TextOut(hdc, 500, 120, str, sizeof(str));
	
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentHorizontalResolution[0]);
	TextOut(hdc, 50, 140, TEXT("CurrentHorizontalResolution:"), sizeof(TEXT("currentHorizontalResolution:")));
	TextOut(hdc, 250, 140, str, sizeof(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentHorizontalResolution[1]);
	TextOut(hdc, 500, 140, str, sizeof(str));

	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentVerticalResolution[0]);
	TextOut(hdc, 50, 160, TEXT("CurrentVerticalResolution:"), sizeof(TEXT("currentVerticalResolution:")));
	TextOut(hdc, 250, 160, str, sizeof(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentVerticalResolution[1]);
	TextOut(hdc, 500, 160, str, sizeof(str));

	memset(str, 0, sizeof(str));
	sprintf(str, "%llu", gpuinfo.currentNumberOfColors[0]);
	TextOut(hdc, 50, 180, TEXT("CurrentNumberOfColors:"), sizeof(TEXT("currentNumberOfColors:")));
	TextOut(hdc, 250, 180, str, sizeof(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%llu", gpuinfo.currentNumberOfColors[1]);
	TextOut(hdc, 500, 180, str, sizeof(str));
	
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentRefreshRate[0]);
	TextOut(hdc, 50, 200, TEXT("CurrentRefreshRate:"), sizeof(TEXT("currentRefreshRate:")));
	TextOut(hdc, 250, 200, str, sizeof(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentRefreshRate[1]);
	TextOut(hdc, 500, 200, str, sizeof(str));

	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentScanMode[0]);
	TextOut(hdc, 50, 220, TEXT("CurrentScanMode:"), sizeof(TEXT("currentScanMode:")));
	TextOut(hdc, 250, 220, str, sizeof(str));
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.currentScanMode[1]);
	TextOut(hdc, 500, 220, str, sizeof(str));
	
	TextOut(hdc, 50, 240, TEXT("Description:"), sizeof(TEXT("Description:")));
	TextOut(hdc, 250, 240, gpuinfo.Description[0], sizeof(TCHAR)*strlen(gpuinfo.Description[0]));
	TextOut(hdc, 500, 240, gpuinfo.Description[1], sizeof(TCHAR)*strlen(gpuinfo.Description[1]));

	TextOut(hdc, 50, 260, TEXT("Status:"), sizeof(TEXT("Status:")));
	TextOut(hdc, 250, 260, gpuinfo.status[0], sizeof(TCHAR)*strlen(gpuinfo.status[0]));
	TextOut(hdc, 500, 260, gpuinfo.status[1], sizeof(TCHAR)*strlen(gpuinfo.status[1]));

	TextOut(hdc, 50, 280, TEXT("DriverVersion:"), sizeof(TEXT("driverVersion:")));
	TextOut(hdc, 250, 280, gpuinfo.driverVersion[0], sizeof(TCHAR)*strlen(gpuinfo.driverVersion[0]));
	TextOut(hdc, 500, 280, gpuinfo.driverVersion[1], sizeof(TCHAR)*strlen(gpuinfo.driverVersion[1]));

	TextOut(hdc, 50, 300, TEXT("VideoProcessor:"), sizeof(TEXT("videoProcessor:")));
	TextOut(hdc, 250, 300, gpuinfo.videoProcessor[0], sizeof(TCHAR)*strlen(gpuinfo.videoProcessor[0]));
	TextOut(hdc, 500, 300, gpuinfo.videoProcessor[1], sizeof(TCHAR)*strlen(gpuinfo.videoProcessor[1]));

	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.videoArchitechture[0]);
	TextOut(hdc, 50, 320, TEXT("VideoArchitechture:"), sizeof(TEXT("videoArchitechture:")));
	TextOut(hdc, 250, 320, str, sizeof(str));
	sprintf(str, "%u", gpuinfo.videoArchitechture[1]);
	TextOut(hdc, 500, 320, str, sizeof(str));
	
	memset(str, 0, sizeof(str));
	sprintf(str, "%u", gpuinfo.videoMemType[0]);
	TextOut(hdc, 50, 340, TEXT("VideoMemoryType:"), sizeof(TEXT("VideomemoryType:")));
	TextOut(hdc, 250, 340, str, sizeof(str));
	sprintf(str, "%u", gpuinfo.videoMemType[1]);
	TextOut(hdc, 500, 340, str, sizeof(str));
	
	TextOut(hdc, 50, 360, TEXT("VideoModeDescription:"), sizeof(TEXT("VideoModeDescription:")));
	TextOut(hdc, 250, 360, gpuinfo.videoModeDescription[0], sizeof(TCHAR)*strlen(gpuinfo.videoModeDescription[0]));
	TextOut(hdc, 500, 360, gpuinfo.videoModeDescription[1], sizeof(TCHAR)*strlen(gpuinfo.videoModeDescription[1]));

	TextOut(hdc, 50, 380, TEXT("DeviceID:"), sizeof(TEXT("DeviceID:")));
	TextOut(hdc, 250, 380, gpuinfo.deviceID[0], sizeof(TCHAR)*strlen(gpuinfo.deviceID[0]));
	TextOut(hdc, 500, 380, gpuinfo.deviceID[1], sizeof(TCHAR)*strlen(gpuinfo.deviceID[1]));


}


void printNETInfo(HDC hdc)
{
	
	TextOut(hdc, 250, 20, netinfo.Caption, sizeof(netinfo.Caption));
	SetTextColor(hdc, RGB(255, 255, 255));

	TextOut(hdc, 200, 60, TEXT("Description:"), sizeof(TEXT("Description:")));
	TextOut(hdc, 450, 60, netinfo.Description, sizeof(netinfo.Description));

	TextOut(hdc, 200, 80, TEXT("IPAddress:"), sizeof(TEXT("IPAddress:")));
	TextOut(hdc, 450, 80, netinfo.IPAddress, sizeof(netinfo.IPAddress));

	TextOut(hdc, 200, 100, TEXT("IPSubnet:"), sizeof(TEXT("IPSubnet:")));
	TextOut(hdc, 450, 100, netinfo.IPSubnet, sizeof(netinfo.IPSubnet));

	TextOut(hdc, 200, 120, TEXT("MACAddress:"), sizeof(TEXT("MACAddress:")));
	TextOut(hdc, 450, 120, netinfo.MACAddress, sizeof(netinfo.MACAddress));

	TextOut(hdc, 200, 140, TEXT("DNSHostName:"), sizeof(TEXT("DNSHostName:")));
	TextOut(hdc, 450, 140, netinfo.DNSHostName, sizeof(netinfo.DNSHostName));

	TextOut(hdc, 200, 160, TEXT("ServiceName:"), sizeof(TEXT("ServiceName:")));
	TextOut(hdc, 450, 160, netinfo.ServiceName, sizeof(netinfo.ServiceName));

}

void printBIOSInfo(HDC hdc)
{

	TextOut(hdc, 300, 20, biosinfo.caption, sizeof(biosinfo.caption));

	SetTextColor(hdc, RGB(255, 255, 255));

	TextOut(hdc, 200, 60, TEXT("SMBiosBiosVersion:"), sizeof(TEXT("SMBiosBiosVersion:")));
	TextOut(hdc, 450, 60, biosinfo.SMBiosBiosVersion, sizeof(biosinfo.SMBiosBiosVersion));

	TextOut(hdc, 200, 80, TEXT("Manufacturer:"), sizeof(TEXT("Manufacturer:")));
	TextOut(hdc, 450, 80, biosinfo.Manufacturer, sizeof(biosinfo.Manufacturer));
	
	TextOut(hdc, 200, 100, TEXT("Name:"), sizeof(TEXT("Name:")));
	TextOut(hdc, 450, 100, biosinfo.Name, sizeof(biosinfo.Name));
	
	TextOut(hdc, 200, 120, TEXT("SerialNumber:"), sizeof(TEXT("SerialNumber:")));
	TextOut(hdc, 450, 120, biosinfo.SerialNumber, sizeof(biosinfo.SerialNumber));
	
	TextOut(hdc, 200, 140, TEXT("Version:"), sizeof(TEXT("Version:")));
	TextOut(hdc, 450, 140, biosinfo.Version, sizeof(biosinfo.Version));

}

void uninitialize(void)
{
	CoUninitialize();

	if (gpuinfo.adapterCompatibility[0])
	{
		free(gpuinfo.adapterCompatibility[0]);
		free(gpuinfo.adapterCompatibility[1]);
	}


}

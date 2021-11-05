#include <Windows.h>
#include <WbemIdl.h> //for IWbemLocator,CLSID_WbemLocator,IID_IWbemLocator
#include <string.h>
#include <iostream> //wstring
#include <Commctrl.h>	//tabbed controls
#include "resource.h"
#include <comdef.h> //_bstr_t 
//for __cpuid extraction
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>




using std::wstring;
using std::string;

#define DAYS_IN_WEEK 7
#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"wbemuuid.lib")

HINSTANCE ghInstance;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct CpuInfo
{
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
	TCHAR deviceID[64];
	unsigned int extclock;
	unsigned int L2cachesize;
	unsigned int L2cachespeed;
	unsigned int L3cachesize;
	unsigned int L3cachespeed;
	unsigned int numberof_enabledcores;
	unsigned int numberof_logicalprocessors;
	TCHAR role[32];
	TCHAR systemname[32];
	TCHAR virtualization_enabled[32];
};

class InstructionSet
{
	class InstructionSet_Internal;	

public:
	//getters
	static std::string Vendor(void) { return CPU_Rep.vendor_; }
	static std::string Brand(void) { return CPU_Rep.brand_; }	

	static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
private:
	static const InstructionSet_Internal CPU_Rep;
											
	class InstructionSet_Internal
	{
	public:
		InstructionSet_Internal()
			:nIds_{ 0 },
			nExIds_{ 0 },
			isIntel_{ false },
			isAMD_{ false },
			f_1_ECX_{ 0 },
			f_1_EDX_{ 0 },
			f_7_EBX_{ 0 },
			f_7_ECX_{ 0 },
			f_81_ECX_{ 0 },
			f_81_EDX_{ 0 },
			data_{},
			extdata_{}
		{
			//int cpuInfo[4] = {-1}
			std::array<int, 4> cpui;

			//call _cpuid with 0x0 as the function_id argument 
			//gets the number of the highest valid function ID.

			__cpuid(cpui.data(), 0);
			nIds_ = cpui[0];

			for (int i = 0; i <= nIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				data_.push_back(cpui);
			}

			//Capture vendor string
			char vendor[0x20];
			memset(vendor, 0, sizeof(vendor));
			*reinterpret_cast<int*>(vendor) = data_[0][1];
			*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
			*reinterpret_cast<int*>(vendor + 8) = data_[0][2];

			vendor_ = vendor;
			if (vendor_ == "GenuineIntel")
			{
				isIntel_ = true;
			}
			else if (vendor_ == "AuthenticAMD")
			{
				isAMD_ = true;
			}

			//load bit with flags for function 0x00000001

			if (nIds_ >= 1)
			{
				f_1_ECX_ = data_[1][2];
				f_1_EDX_ = data_[1][3];
			}

			//load bitset with flags for function 0x00000007
			if (nIds_ >= 7)
			{
				f_7_EBX_ = data_[7][1];
				f_7_ECX_ = data_[7][2];
			}

			//calling __cpuid with 0x00000008 as the function_id argument
			//gets the number of the highest valid extended ID

			__cpuid(cpui.data(), 0x80000000);
			nExIds_ = cpui[0];

			char brand[0x40];
			memset(brand, 0, sizeof(brand));

			for (int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				extdata_.push_back(cpui);
			}

			//load bitset with flags for function 0x80000001

			if (nExIds_ >= 0x80000001)
			{
				f_81_ECX_ = extdata_[1][2];
				f_81_EDX_ = extdata_[1][3];
			}

			//Interpret CPU brand string if reported
			if (nExIds_ >= 0x80000004)
			{
				memcpy(brand, extdata_[2].data(), sizeof(cpui));
				memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
				memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
				brand_ = brand;
			}
		};
	
	int nIds_;
	int nExIds_;
	std::string vendor_;
	std::string brand_;
	bool isIntel_;
	bool isAMD_;
	std::bitset<32> f_1_ECX_;
	std::bitset<32> f_1_EDX_;
	std::bitset<32> f_7_EBX_;
	std::bitset<32> f_7_ECX_;
	std::bitset<32> f_81_ECX_;
	std::bitset<32> f_81_EDX_;

	std::vector<std::array<int, 4>>data_;
	std::vector<std::array<int, 4>>extdata_;
	
	};

};


//Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

CpuInfo cpuinfo;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	
	HWND hwnd;
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR szAppName[] = TEXT("myTabbedWindow");

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
	static HWND hwndTab;
	static HWND hwndStatic;
	HRESULT hres;


	HWND createTabbedWindow(HWND);
	HWND createTabContent(HWND);
	BOOL onNotifyResponse(HWND, HWND, LPARAM);
	BOOL getCpuInfo(IWbemLocator*, IWbemServices*);
	
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

			//now use IWbemServices pointer to make requests to WMI
			

			if (getCpuInfo( pLoc,pSvc))
			{
				//prompt
			}

		
			//setup tabs
			hwndTab = createTabbedWindow(hwnd);
			hwndStatic = createTabContent(hwndTab);
			break;
		case WM_SIZE:
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

	for (i = 0; i < DAYS_IN_WEEK; i++)
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

	hwndStatic = CreateWindow(WC_STATIC, TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 100, 600, 600, hwndTab, NULL, ghInstance, NULL);

	if (!hwndStatic)
	{
		MessageBox(NULL, TEXT("failed to create window inside tabbed window"), TEXT("error"), MB_OK);
	}

	return (hwndStatic);
}



BOOL onNotifyResponse(HWND hwndTab, HWND hwndStatic, LPARAM lParam)
{
	TCHAR achTemp[256]; //temp buffer for strings
	LRESULT result;
	static int iPage;
	 LPCSTR sometext = _bstr_t("this is my text");
	 HDC hdc;
	 TCHAR struint[6];
	 RECT rect;
	 TCHAR str[32];

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
			
			TextOut(hdc, 0, 0, cpuinfo.name, sizeof(cpuinfo.name));
			TextOut(hdc, 0, 20, cpuinfo.description, sizeof(cpuinfo.description));
			TextOut(hdc, 0, 40, cpuinfo.codename, sizeof(cpuinfo.codename));
			TextOut(hdc, 0, 60, cpuinfo.type, sizeof(cpuinfo.type));
			wsprintf(struint, "%d", cpuinfo.stepping);

			TextOut(hdc, 0, 80, TEXT("Stepping: "), sizeof(TEXT("Stepping: ")));
			TextOut(hdc, 80, 80, struint, sizeof(struint));
			
			TextOut(hdc, 0, 100, cpuinfo.architecture, sizeof(cpuinfo.architecture));
			
			wsprintf(struint, "%d", cpuinfo.addresswidth);
			TextOut(hdc, 0, 120, TEXT("AddressWidth: "), sizeof(TEXT("AddressWidth: ")));
			TextOut(hdc, 100, 120, struint, sizeof(struint));
			
			wsprintf(struint, "%d", cpuinfo.numberofcores);
			TextOut(hdc, 0, 140, TEXT("NumberOfCores: "), sizeof(TEXT("NumberOfCores: ")));
			TextOut(hdc, 120, 140, struint, sizeof(struint));

			TextOut(hdc, 0, 160, cpuinfo.assettag, sizeof(cpuinfo.assettag));

			TextOut(hdc, 0, 180, cpuinfo.availabilitystatus, sizeof(cpuinfo.availabilitystatus));


			wsprintf(struint, "%d", cpuinfo.datawidth);
			TextOut(hdc, 0, 200, TEXT("DataWidth (bits): "), sizeof(TEXT("DataWidth (bits): ")));
			TextOut(hdc, 120, 200, struint, sizeof(struint));

			wsprintf(struint, "%d", cpuinfo.maximumclockspeed);
			TextOut(hdc, 0, 220, TEXT("MaximumClockSpeed (MHz) : "), sizeof(TEXT("MaximumClockSpeed (MHz): ")));
			TextOut(hdc, 210, 220, struint, sizeof(struint));

			wsprintf(struint, "%d", cpuinfo.characteristics);
			TextOut(hdc, 0, 240, TEXT("Characteristics: "), sizeof(TEXT("Characteristics: ")));
			TextOut(hdc, 110, 240, struint, sizeof(struint));

			TextOut(hdc, 0, 260, cpuinfo.cpustatus, sizeof(cpuinfo.cpustatus));

			wsprintf(struint, "%d", cpuinfo.currentclockspeed);
			TextOut(hdc, 0, 280, TEXT("CurrentClock Speed (MHz): "), sizeof(TEXT("CurrentClock Speed (MHz): ")));
			TextOut(hdc, 210, 280, struint, sizeof(struint));

			wsprintf(struint, "%d", cpuinfo.currentvoltage);
			TextOut(hdc, 0, 300, TEXT("Current Voltage (tenth-volts): "), sizeof(TEXT("Current Voltage (tenth-volt) : ")));
			TextOut(hdc, 210, 300, struint, sizeof(struint));

			TextOut(hdc, 0, 320, cpuinfo.deviceID, sizeof(cpuinfo.deviceID));
			
			wsprintf(struint, "%d", cpuinfo.extclock);
			TextOut(hdc, 0, 340, TEXT("ExternalClockFrequency (MHz): "), sizeof(TEXT("ExternalClockFrequency (MHz): ")));
			TextOut(hdc, 220, 340, struint, sizeof(struint));
			
			if (InstructionSet::SSE3())
			{
				TextOut(hdc, 0, 360, TEXT("SSE "), sizeof(TEXT("SSE")));
			}
			
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
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
	TCHAR str[64];
	OLECHAR strr[64];

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
			return FALSE;
		}

		VARIANT vtProp;

		hres = pclsObject->Get(L"Name", 0, &vtProp, 0, 0);

		if (SUCCEEDED(hres))
		{
			std::wstring tmp(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
			strcpy(cpuinfo.name, "Name: ");
			strcat(cpuinfo.name, _bstr_t(vtProp.bstrVal));

		}

		hres = pclsObject->Get(L"Description", 0, &vtProp, 0, 0);

		if (SUCCEEDED(hres))
		{
			//std::wstring tmp(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
			strcpy(cpuinfo.description, "Specification: ");
			strcat(cpuinfo.description, _bstr_t(vtProp.bstrVal));
		}

		hres = pclsObject->Get(L"ProcessorID", 0, &vtProp, 0, 0);

		if (SUCCEEDED(hres))
		{
			wstring tmp(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));

			size_t found;
			if ((found = tmp.find(L"506")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Codename: ");
				strcat(cpuinfo.codename, "Skylake");

			}
			else if ((found = tmp.find(L"606")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Codename: ");
				strcat(cpuinfo.codename, "Kabylake");

			}
			else if ((found = tmp.find(L"406")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Codename: ");
				strcat(cpuinfo.codename, "Haswell");

			}
			else if ((found = tmp.find(L"306")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Codename: ");
				strcat(cpuinfo.codename, "Ivybridge");

			}
			else if ((found = tmp.find(L"206")) != string::npos)
			{
				strcpy(cpuinfo.codename, "Codename: ");
				strcat(cpuinfo.codename, "Sandybride");

			}
		}


		hres = pclsObject->Get(L"ProcessorType", 0, &vtProp, 0, 0);
		switch (vtProp.uintVal)
		{
		case 1:

			strcpy(cpuinfo.type, "Type: ");
			strcat(cpuinfo.type, "other");

			break;
		case 2:

			strcpy(cpuinfo.type, "Type: ");
			strcat(cpuinfo.type, "Unknown");

			break;
		case 3:

			strcpy(cpuinfo.type, "Type: ");
			strcat(cpuinfo.type, "Central Processor");

			break;
		case 4:

			strcpy(cpuinfo.type, "Type: ");
			strcat(cpuinfo.type, "Math Processor");

			break;
		case 5:
			strcpy(cpuinfo.type, "Type: ");
			strcat(cpuinfo.type, "DSP processor");
			break;

		case 6:
			strcpy(cpuinfo.type, "Type: ");
			strcat(cpuinfo.type, "video processor");

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
			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "x86");

			break;
		case 1:

			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "MIPS");

			break;
		case 2:

			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "Alpha");

			break;

		case 3:

			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "PowerPC");
			break;
		case 5:

			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "ARM");

			break;
		case 6:

			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "ia64");

			break;
		case 9:
			strcpy(cpuinfo.architecture, "Architecture: ");
			strcat(cpuinfo.architecture, "x64");

			break;
		}


		hres = pclsObject->Get(L"AddressWidth", 0, &vtProp, 0, 0);
		cpuinfo.addresswidth = vtProp.uintVal;

		hres = pclsObject->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
		cpuinfo.numberofcores = vtProp.uintVal;

		hres = pclsObject->Get(L"AssetTag", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.assettag, "AssetTag: ");
		strcat(cpuinfo.assettag, _bstr_t(vtProp.bstrVal));

		hres = pclsObject->Get(L"Availability", 0, &vtProp, 0, 0);
		switch (vtProp.uintVal)
		{
		case 3:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "(3) Running Full power");

			break;
		case 4:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "(4) Warning");

			break;
		case 5:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "(5) InTest");

			break;
		case 6:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Not Applicable");

			break;
		case 7:
			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Power Off");

			break;
		case 8:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Off Line");

			break;
		case 9:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Off Duty");

			break;
		case 10:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Degraded");

			break;
		case 11:
			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Not Installed");

			break;
		case 12:
			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Install Error");
			break;
		case 13:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Power save unknown");

			break;
		case 14:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "power save low power mode");

			break;
		case 15:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Power save stand by");

			break;
		case 16:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Power Cycle");

			break;
		case 17:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Power save Warning state");

			break;
		case 18:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Paused");

			break;
		case 19:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Not Ready");

			break;
		case 20:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "Not configured");

			break;
		default:

			strcpy(cpuinfo.availabilitystatus, "AvailabilityStatus: ");
			strcat(cpuinfo.availabilitystatus, "other");
			break;
		}


		hres = pclsObject->Get(L"DataWidth", 0, &vtProp, 0, 0);
		cpuinfo.datawidth = vtProp.uintVal;


		hres = pclsObject->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
		cpuinfo.maximumclockspeed = vtProp.intVal;

		hres = pclsObject->Get(L"Characteristics", 0, &vtProp, 0, 0);
		cpuinfo.characteristics = vtProp.uintVal;


		hres = pclsObject->Get(L"CpuStatus", 0, &vtProp, 0, 0);

		switch (vtProp.uintVal)
		{
		case 0:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Unknown");

			break;
		case 1:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Cpu Enabled");

			break;
		case 2:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Cpu Disabled by user using BIOS setup");

			break;
		case 3:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Cpu Disabled by BIOS POST Error");

			break;
		case 4:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Cpu is Idle");

			break;
		case 5:
		case 6:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Reserved");

			break;
		default:

			strcpy(cpuinfo.cpustatus, "CpuStatus: ");
			strcat(cpuinfo.cpustatus, "Other");

			break;
		}



		hres = pclsObject->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
		cpuinfo.currentclockspeed = vtProp.uintVal;

		hres = pclsObject->Get(L"CurrentVoltage", 0, &vtProp, 0, 0);
		cpuinfo.currentvoltage = vtProp.uintVal;


		hres = pclsObject->Get(L"DeviceID", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.deviceID, "DeviceID: ");
		strcat(cpuinfo.deviceID, _bstr_t(vtProp.bstrVal));
		
		//wsprintfW(strr,L"%ls",vtProp.bstrVal);
		//MessageBoxW(NULL, strr, L"test", MB_OK);

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
		strcpy(cpuinfo.role, "Role: ");
		strcat(cpuinfo.role, _bstr_t(vtProp.bstrVal));

		hres = pclsObject->Get(L"SystemName", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.systemname, "SystemName: ");
		strcat(cpuinfo.systemname, _bstr_t(vtProp.bstrVal));

		hres = pclsObject->Get(L"VirtualizationFirmwareEnabled ", 0, &vtProp, 0, 0);
		strcpy(cpuinfo.virtualization_enabled, "VirtualizationFirmwareEnabled: ");
		strcat(cpuinfo.virtualization_enabled, _bstr_t(vtProp.bstrVal));

		VariantClear(&vtProp);
		pclsObject->Release();
		pclsObject = NULL;

	}

	return TRUE;
}

void uninitialize(void)
{
	CoUninitialize();
}

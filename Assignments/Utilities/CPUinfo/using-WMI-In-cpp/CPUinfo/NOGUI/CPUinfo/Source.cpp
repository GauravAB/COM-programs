#include <iostream>
	#include <WbemIdl.h>
#include <comdef.h>
#include <Winbase.h>
#include <string.h>


using std wstring;


#pragma comment(lib,"wbemuuid.lib")
#pragma comment(lib,"kernel32.lib")

char* cpu_family = NULL;

int main(int argc, char **argv)
{
	HRESULT hr;


	//Initialize COM first
	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		cout << "failed to Initialize COM Engine" << endl;
		return 1;
	}

	//set secutity level on WMI namespace

	hr = CoInitializeSecurity(
		NULL,
		-1,		//COM negotiates services
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT, //type of authentication
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL);

	if (FAILED(hr))
	{
		cout << "failed to initialize security" << endl;
		CoUninitialize();
		return 1;
	}


	//create WMI connection
	//Obtain intial location to windows management
	//on particular host computer

	IWbemLocator *pLoc = 0;

	hr = CoCreateInstance(
		CLSID_WbemLocator,// CLSID of class which holds interface IWbemLocator interface to obtain the initial namespace pointer to the IWbemServices interface for WMI
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,	//Interface IID
		(LPVOID*)&pLoc		//outparameter to hold the interface pointer
	);

	if (FAILED(hr))
	{
		cout << " CoCreateInstance failed to get the WbemLocator class object" << endl;
		CoUninitialize();
		return 1;
	}

	//Interface pointer
	IWbemServices *pSvc = 0;

	//Connect to root namespace of current user and obtain pSvc
	//pSvc lets us make IWbemServices calls

	hr = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), //WMI namespace
		NULL, //User name
		NULL,	//User password
		0,		//Locale
		NULL,	//Security flags
		0,		//Authority
		0,		//Context Object
		&pSvc	//IWbemServices proxy
	);


	if (FAILED(hr))
	{
		cout << "could not connect not WMI" << endl;
		pLoc->Release();
		CoUninitialize();
		return 1;
	}

	cout << "connected to root\cmv2 WMI namespace" << endl;

	//Set IWbemServices proxy so that impersonation of user	 (client) occurs

	hr = CoSetProxyBlanket(
		pSvc,				//the proxy to set
		RPC_C_AUTHN_WINNT,	//authentication services
		RPC_C_AUTHZ_NONE,	//authorization services
		NULL,				//Server principal name
		RPC_C_AUTHN_LEVEL_CALL,	//authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,	//impersonation level
		NULL,				//client identity
		EOAC_NONE			//proxy capabilities
	);

	if (FAILED(hr))
	{
		cout << " could not set proxy blanket" << endl;

		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;			//program has failed
	}

	//using the IWbemServices pointer to make requests of WMI

	IEnumWbemClassObject* pEnumerator = NULL;

	hr = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hr))
	{
		cout << "Query for processes failed" << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;
	}
	else
	{
		IWbemClassObject *pclsObject;
		ULONG uReturn = 0;

		while (pEnumerator)
		{
			hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);


			if (0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;

			//Get the value of name property
			cout << "--------------------------------------CPUINFO------------------------------------------" << endl;
			cout << "--------------------------------------------------------------------------------------" << endl;

			hr = pclsObject->Get(L"Manufacturer", 0, &vtProp, 0, 0);
			wcout << "Manufacturer: " << vtProp.bstrVal << endl;
			hr = pclsObject->Get(L"Name", 0, &vtProp, 0, 0);
			wcout << "Name: " << vtProp.bstrVal << endl;
			hr = pclsObject->Get(L"Description", 0, &vtProp, 0, 0);
			wcout << "Description: " << vtProp.bstrVal << endl;
			hr = pclsObject->Get(L"ProcessorID", 0, &vtProp, 0, 0);
			
			wcout << "ProcessorID: " << vtProp.bstrVal << endl;
			wstring tmp(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
	
			size_t found;
			if ( (found = tmp.find(L"506")) != string::npos)
			{
				cout << "microarchitechture: " << "Skylake" << endl;
			}
			else if ((found = tmp.find(L"606")) != string::npos)
			{
				cout << "microarchitechture: " << "Kabylake" << endl;
			}
			else if ((found = tmp.find(L"406")) != string::npos)
			{
				cout << "microarchitechture: " <<  "HassWell" << endl;
			}
			else if ((found = tmp.find(L"306")) != string::npos)
			{
				cout << "microarchitechture: " << "IvyBridge" << endl;
			}
			else if ((found = tmp.find(L"206")) != string::npos)
			{
				cout << "microarchitechture: " << "SandyBridge" << endl;
			}




			
			hr = pclsObject->Get(L"ProcessorType", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
			case 1:
				cout << "Processor Type: "<< endl;
				break;
			case 2:
				cout << "Processor Type: " <<"Unknown "<< endl;
				break;
			case 3:
				cout << "Processor Type: " << "Central Processor "<< endl;
				break;
			case 4:
				cout << "Processor Type: " <<"Math Processor " << endl;
				break;
			case 5:
				cout << "Processor Type: " <<"DSP Processor "<< endl;
				break;
			case 6:
				cout << "Processor Type: " <<"video Processor"<< endl;
				break;
			default:
				break;
			}

			hr = pclsObject->Get(L"Stepping ", 0, &vtProp, 0, 0);
			wcout << "Stepping: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"Architecture", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
				case 0:
					cout << "Architecture: " << "x86" << endl;
					break;
				case 1:
					cout << "Architecture: " << "MIPS" << endl;
					break;
				case 2:
					cout << "Architecture: " << "Alpha" << endl;
					break;
				case 3:
					cout << "Architecture: " << "PowerPC" << endl;
					break;
				case 5:
					cout << "Architecture: " << "ARM" << endl;
					break;
				case 6:
					cout << "Architecture: " << "ia64" << endl;
					break;
				case 9:
					cout << "Architecture: " << "x64" << endl;
					break;
			}


			hr = pclsObject->Get(L"AddressWidth", 0, &vtProp, 0, 0);
			wcout << "AddressWidth: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
			wcout << "NumberOfCores: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"AssetTag", 0, &vtProp, 0, 0);
			wcout << "AssetTag: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"Availability", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
			case 3:
				wcout << "Availability status: " << "(3) Running Full power" << endl;
				break;
			case 4:
				wcout << "Availability status: " << "(4) Warning" << endl;
				break;
			case 5:
				wcout << "Availability status: " << "(5) InTest" << endl;
				break;
			case 6:
				wcout << "Availability status: " << "Not Applicable" << endl;
				break;
			case 7:
				wcout << "Availability status: " << "Power Off" << endl;
				break;
			case 8:
				wcout << "Availability status: " << "Off Line" << endl;
				break;
			case 9:
				wcout << "Availability status: " << "Off Duty" << endl;
				break;
			case 10:
				wcout << "Availability status: " << "Degraded" << endl;
				break;
			case 11:
				wcout << "Availability status: " << "Not Installled" << endl;
				break;
			case 12:
				wcout << "Availability status: " << "Install Error" << endl;
				break;
			case 13:
				wcout << "Availability status: " << "Power save unknown" << endl;
				break;
			case 14:
				wcout << "Availability status: " << "Power save Low Power mode" << endl;
				break;
			case 15:
				wcout << "Availability status: " << "Power save Stand by" << endl;
				break;
			case 16:
				wcout << "Availability status: " << "Power Cycle" << endl;
				break;
			case 17:
				wcout << "Availability status: " << "Power save ,Warning state" << endl;
				break;
			case 18:
				wcout << "Availability status: " << "Paused" << endl;
				break;
			case 19:
				wcout << "Availability status: " << "Not ready" << endl;
				break;
			case 20:
				wcout << "Availability status: " << "Not configured" << endl;
				break;
			default:
				cout << "Availability status:" << "other" << endl;
				break;
			}

			hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
			wcout << "Caption: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"DataWidth", 0, &vtProp, 0, 0);
			wcout << "DataWidth: " << vtProp.uintVal << endl;


			hr = pclsObject->Get(L"ConfigManagerErrorCode", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
			case 0:
				cout << "ConfigManagerErrorCode: " << "The device is working properly" << endl;
				break;
			case 1:
				cout << "ConfigManagerErrorCode: " << "The device not configured correctly" << endl;
				break;
			case 2:
				cout << "ConfigManagerErrorCode: " << "Windows cannot load driver for this device" << endl;
				break;
			case 3:
				cout << "ConfigManagerErrorCode: " << "Driver for devide might be corrupted, or your system my be running low on memory or other resources" << endl;
				break;
			case 4:
				cout << "ConfigManagerErrorCode: " << "Device not working properly , One of its drivers or your registry might be corrupted" << endl;
				break;
			case 5:
				cout << "ConfigManagerErrorCode: " << "Driver for this device needs a resource that Windows cannot manage" << endl;
				break;
			case 6:
				cout << "ConfigManagerErrorCode: " << "The boot configuration of this device conflicts with other resources" << endl;
				break;
			case 7:
				cout << "ConfigManagerErrorCode: " << "Cannot filter" << endl;
				break;
			case 8:
				cout << "ConfigManagerErrorCode: " << "Driver loader for the device is missing" << endl;
				break;
			case 9:
				cout << "ConfigManagerErrorCode: " << "Driver not working properly because the controlling firmware is reporting the resources for the device incorrectly" << endl;
				break;
			case 10:
				cout << "ConfigManagerErrorCode: " << "The device cannot start" << endl;
				break;
			case 11:
				cout << "ConfigManagerErrorCode: " << "This device failed" << endl;
				break;
			case 12:
				cout << "ConfigManagerErrorCode: " << "This device cannot find free resources that it can use." << endl;
				break;
			case 13:
				cout << "ConfigManagerErrorCode: " << "Windows cannot verify this devices resources" << endl;
				break;
			case 14:
				cout << "ConfigManagerErrorCode: " << "Device cannot work properly until restarted" << endl;
				break;
			case 15:
				cout << "ConfigManagerErrorCode: " << "Device not working because of re - enumeration problem" << endl;
				break;
			case 16:
				cout << "ConfigManagerErrorCode: " << "Windows cannot identify all the resources of this device" << endl;
				break;
			case 17:
				cout << "ConfigManagerErrorCode: " << "The device is asking for unknown resource types" << endl;
				break;
			case 18:
				cout << "ConfigManagerErrorCode: " << "Reinstall driver for this device" << endl;
				break;
			case 19:
				cout << "ConfigManagerErrorCode: " << "Failure using VxD loader" << endl;
				break;
			case 20:
				cout << "ConfigManagerErrorCode: " << "your registry might be corrupted" << endl;
				break;
			case 21:
			case 23:
				cout << "ConfigManagerErrorCode: " << "System failure" << endl;
				break;
			case 22:
				cout << "ConfigManagerErrorCode: " << "Device is disabled" << endl;
				break;
			case 24:
				cout << "ConfigManagerErrorCode: " << "This device is not present, is not working properly, or does not have all its drivers installed." << endl;
				break;
			case 25:
				cout << "ConfigManagerErrorCode: " << "Windows is still setting up this device." << endl;
				break;
			case 26:
				cout << "ConfigManagerErrorCode: " << "Windows is still setting up this device." << endl;
				break;
			case 27:
				cout << "ConfigManagerErrorCode: " << "This device does not have valid log configuration" << endl;
				break;
			case 28:
				cout << "ConfigManagerErrorCode: " << "Drivers are not installed" << endl;
				break;
			case 29:
				cout << "ConfigManagerErrorCode: " << "This device is disabled because the firmware of the device did not give it the required resources." << endl;
				break;
			case 30:
				cout << "ConfigManagerErrorCode: " << "This device is using an Interrupt Request (IRQ) resource that another device is using. " << endl;
				break;
			case 31:
				cout << "ConfigManagerErrorCode: " << "This device is not working properly because Windows cannot load the drivers required for this device " << endl;
				break;
			default:
				cout << "other" << endl;
				break;
			}

			hr = pclsObject->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
			wcout << "MaximumClockSpeed: " << vtProp.uintVal << " MHz" << endl;

			hr = pclsObject->Get(L"Characteristics", 0, &vtProp, 0, 0);
			wcout << "Characteristics: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"CpuStatus", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
			case 0:
				wcout << "CpuStatus: " << "Unknown" << endl;

				break;
			case 1:
				wcout << "CpuStatus: " << "Cpu Enabled" << endl;
				break;
			case 2:
				wcout << "CpuStatus: " << "Cpu Disabled by user using BIOS setup" << endl;
				break;
			case 3:
				wcout << "CpuStatus: " << "Cpu Disabled by BIOS POST Error" << endl;
				break;
			case 4:
				wcout << "CpuStatus: " << "Cpu Is Idle" << endl;
				break;
			case 5:
			case 6:
				wcout << "CpuStatus: " << "Reserved" << endl;
				break;
			default:
				wcout << "Other" << endl;
				break;
			}

			hr = pclsObject->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
			wcout << "CurrentClockSpeed: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"CurrentVoltage", 0, &vtProp, 0, 0);

			wcout << "CurrentVoltage: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"Device ID", 0, &vtProp, 0, 0);
			wcout << "DeviceID: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"ExtClock", 0, &vtProp, 0, 0);
			wcout << "ExternalClockFrequency: " << vtProp.uintVal << " MHz" << endl;
			
			/*
			hr = pclsObject->Get(L"Family", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
				cpu_family = (char*)malloc(128 * sizeof(char));
			case 1:
				cpu_family = "Other";
				break;
			case 2:
				cpu_family = "Unknown";
				break;
			case 3:
				cpu_family = "8086 processor";
				break;
			case 4:
				cpu_family = "80286 processor";
				break;
			case 5:
				cpu_family = "Intel386™ Processor";
				break;
			case 6:
				cpu_family = "Intel486™ Processor";
				break;
			case 7:
				cpu_family = "8087 processor";
				break;
			case 8:
				cpu_family = "80287 processor";
				break;
			case 9:
				cpu_family = "80387 processor";
				break;
			case 10:
				cpu_family = "80487 processor";
				break;
			case 11:
				cpu_family = "Pentium(R) brand";
				break;
			case 12:
				cpu_family = "Pentium(R) pro";
				break;
			case 13:
				cpu_family = "Pentium(R) II";
				break;
			case 14:
				cpu_family = "Pentium(R) processor with MMX(TM) technology";
				break;
			case 15:
				cpu_family = "Celeron™";
				break;
			case 16:
				cpu_family = "Pentium II Xeon™";
				break;
			case 17:
				cpu_family = "Pentium(R) III ";
				break;
			case 18:
				cpu_family = "M1 Family ";
				break;
			case 19:
				cpu_family = "M2 Family ";
				break;
			case 24:
				cpu_family = "K5 Family ";
				break;
			case 25:
				cpu_family = "AMD Duron™ Processor Family ";
				break;
			case 26:
				cpu_family = "K6-2 ";
				break;
			case 27:
				cpu_family = "K6-3 ";
				break;
			case 28:
				cpu_family = "AMD Athlon(TM) Processor Family ";
				break;
			case 29:
				cpu_family = "AMD(R) Duron(TM) Processor  ";
				break;
			case 30:
				cpu_family = "AMD29000 Family";
				break;
			case 31:
				cpu_family = "K6-2+";
				break;
			case 32:
				cpu_family = "Power PC Family";
				break;
			case 33:
				cpu_family = "Power PC 601";
				break;
			case 34:
				cpu_family = "Power PC 603";
				break;
			case 35:
				cpu_family = "Power PC 603+";
				break;
			case 36:
				cpu_family = "Power PC 604";
				break;
			case 37:
				cpu_family = "Power PC 620";
				break;
			case 38:
				cpu_family = "Power PC X704";
				break;
			case 39:
				cpu_family = "Power PC 750";
				break;
			case 48:
				cpu_family = "Alpha Family";
				break;
			case 49:
				cpu_family = "Alpha 21064 (49)";
				break;
			case 50:
				cpu_family = "Alpha 21066 (50)";
				break;
			case 51:
				cpu_family = "Alpha 21164 (51)";
				break;
			case 52:
				cpu_family = "Alpha 21064PC (52)";
				break;
			case 53:
				cpu_family = "Alpha 21164a (53)";
				break;
			case 54:
				cpu_family = "Alpha 21264 (54)";
				break;
			case 55:
				cpu_family = "Alpha 21364 (55)";
				break;
			case 64:
				cpu_family = "MIPS Family (64)";
				break;
			case 65:
				cpu_family = "MIPS R4000 (65)";
				break;
			case 66:
				cpu_family = "MIPS R4200 (66)";
				break;
			case 67:
				cpu_family = "MIPS R4400 (67)";
				break;
			case 68:
				cpu_family = "MIPS R4600 (65)";
				break;
			case 69:
				cpu_family = "MIPS R10000 (69)";
				break;
			case 80:
				cpu_family = "SPARC family";
				break;
			case 81:
				cpu_family = "superSPARC";
				break;
			case 82:
				cpu_family = "microSPARC II";
				break;
			case 83:
				cpu_family = "microSPARC IIep";
				break;
			case 84:
				cpu_family = "ultraSPARC ";
				break;
			case 85:
				cpu_family = "ultraSPARC II";
				break;
			case 86:
				cpu_family = "ultraSPARC IIi";
				break;
			case 87:
				cpu_family = "ultraSPARC III";
				break;
			case 88:
				cpu_family = "ultraSPARC IIIi";
				break;
			case 96:
				cpu_family = "68040 ";
				break;
			case 97:
				cpu_family = "68xxx Family ";
				break;
			case 98:
				cpu_family = "68000";
				break;
			case 99:
				cpu_family = "68010";
				break;
			case 100:
				cpu_family = "68020";
				break;
			case 101:
				cpu_family = "68030";
				break;
			case 112:
				cpu_family = "Hobbit family";
				break;
			case 120:
				cpu_family = "Crusoe(TM) TM5000 Family";
				break;
			case 121:
				cpu_family = "Crusoe(TM) TM3000 Family";
				break;
			case 122:
				cpu_family = "Efficeon(TM) TM8000 Family ";
				break;
			case 128:
				cpu_family = "Weitek  ";
				break;
			case 130:
				cpu_family = "Itanium(TM) Processor";
				break;
			case 131:
				cpu_family = "AMD Athlon(TM) 64 Processor Family";
				break;
			case 132:
				cpu_family = "AMD Opteron(TM) Family ";
				break;
			case 144:
				cpu_family = "PA-RISC Family";
				break;
			case 145:
				cpu_family = "PA-RISC 8500";
				break;
			case 146:
				cpu_family = "PA-RISC 8000";
				break;
			case 147:
				cpu_family = "PA-RISC 7300LC ";
				break;
			case 176:
				cpu_family = "Pentium(R) III Xeon(TM)";
				break;
			case 178:
				cpu_family = "Pentium(R) 4 ";
				break;
			case 179:
				cpu_family = "Intel(R) Xeon(TM)";
				break;
			case 181:
				cpu_family = "Intel(R) Xeon(TM) processor MP";
				break;
			case 182:
				cpu_family = "AMD AthlonXP(TM) Family";
				break;
			case 183:
				cpu_family = "AMD AthlonMP(TM) Family";
				break;
			case 184:
				cpu_family = "Intel(R) Itanium(R) 2";
				break;
			case 185:
				cpu_family = "Intel Pentium M Processor";
				break;
			case 190:
				cpu_family = "K7";
				break;
			case 198:
				cpu_family = "Intel Core™ i7-2760QM";
				break;
			case 500:
				cpu_family = "video processor";
				break;
			default:
				break;
			}
			
			wcout << "CPU Family: " << cpu_family << endl;
			*/
			hr = pclsObject->Get(L"L2CacheSize", 0, &vtProp, 0, 0);
			wcout << " L2 Cache Size: " << vtProp.uintVal << " KB" << endl;

			hr = pclsObject->Get(L"L2CacheSpeed ", 0, &vtProp, 0, 0);
			wcout << " L2 Cache Speed: " << vtProp.uintVal << " MHz" << endl;

			hr = pclsObject->Get(L"L3CacheSize", 0, &vtProp, 0, 0);
			wcout << " L3 Cache Size: " << vtProp.uintVal << " KB" << endl;

			hr = pclsObject->Get(L"L3CacheSpeed ", 0, &vtProp, 0, 0);
			wcout << " L3 Cache Speed: " << vtProp.uintVal << " MHz" << endl;


			hr = pclsObject->Get(L"NumberOfEnabledCore", 0, &vtProp, 0, 0);
			wcout << "Number of Enabled Core: " << vtProp.uintVal << endl;

			hr = pclsObject->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
			wcout << "NumberOfLogicalProcessors: " << vtProp.uintVal << endl;

			wcout << "HyperThreading: " << "Enabled" << endl;

			hr = pclsObject->Get(L"Role", 0, &vtProp, 0, 0);
			wcout << "Role: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"SystemName", 0, &vtProp, 0, 0);
			wcout << "SystemName: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"VirtualizationFirmwareEnabled ", 0, &vtProp, 0, 0);
			wcout << "VirtualizationFirmwareEnabled:" << vtProp.bstrVal << endl;


			VariantClear(&vtProp);
			pclsObject->Release();
			pclsObject = NULL;


		}
	}
	cout << "--------------------------------------------------------------------------------------" << endl;

	hr = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_BaseBoard"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);


	if (FAILED(hr))
	{
		cout << "Query for Win32_BaseBoard failed" << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;
	}
	else
	{
		IWbemClassObject *pclsObject;
		ULONG uReturn = 0;

		while (pEnumerator)
		{
			hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObject, &uReturn);

			if (0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;

			//Get the value of name property
			hr = pclsObject->Get(L"Caption", 0, &vtProp, 0, 0);
			wcout << "Caption: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"ConfigOptions ", 0, &vtProp, 0, 0);
			wcout << "ConfigOptions: " << vtProp.pbstrVal << endl;


			hr = pclsObject->Get(L"Manufacturer", 0, &vtProp, 0, 0);
			wcout << "BaseBoard Manufacturer: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"Model ", 0, &vtProp, 0, 0);
			wcout << "Model ID: " << vtProp.bstrVal << endl;

			hr = pclsObject->Get(L"Powered-On", 0, &vtProp, 0, 0);
			switch (vtProp.uintVal)
			{
			case 0:
				cout << "power on status: " << "TRUE" << endl;
				break;
			case 1:
				cout << "power on status: " << "FALSE" << endl;
				break;
			}
			hr = pclsObject->Get(L"Version", 0, &vtProp, 0, 0);
			wcout << "Version: " << vtProp.uintVal << endl;


			VariantClear(&vtProp);
			pclsObject->Release();
			pclsObject = NULL;

		}
	}

	cout << "-----------------------------------------------------------------------------------------" << endl;
	cout << "Some specific processor specs not exposed by WMI" <<endl;


	if (IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE))
	{
		cout << "The MMX instruction set is available." << endl;
	}
	
	if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
	{
		cout << "The SSE instruction set is available " << endl;
	}

	if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
	{
		cout << "The SSE2 instruction set is available " << endl;
	}

	if (IsProcessorFeaturePresent(PF_SSE3_INSTRUCTIONS_AVAILABLE))
	{
		cout << "The SSE3 instruction set is available."<< endl;
	}




	//Cleanup

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();

	CoUninitialize();

	getchar();

	return 0;
}


//cou , ram , bios  , hdd , vcard , sound card , ncard 



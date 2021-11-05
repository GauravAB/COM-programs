#include <Windows.h>
#include <TlHelp32.h>
#include <Wtsapi32.h>
#include <iostream>
#include <Userenv.h>

using std::cout;
using std::endl;


int myCreateProcess(TCHAR*szPath)
{

	HANDLE hProcess , hUserToken, hPToken , hUserTokenDup;
	STARTUPINFO stInfo;
	PROCESS_INFORMATION pInfo;
	DWORD dwSessionId, winLogonPid , dwCreationFlags;

	ZeroMemory(&stInfo, sizeof(STARTUPINFO));

	stInfo.cb = sizeof(STARTUPINFO);
	stInfo.dwFlags = STARTF_USESHOWWINDOW;
	stInfo.wShowWindow = SW_SHOWDEFAULT;

	
	//retrieves the session identifier of the console session
	dwSessionId = WTSGetActiveConsoleSessionId();

	////////////////////////////////find winlogon process////////////////////////////////////////////////////////////////////
	PROCESSENTRY32 procEntry;
	//Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
	//param:Includes all processes in the system in the snapshot. To enumerate the processes, see Process32First.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	//enumerated proceses in procEntry
	if (!Process32First(hSnap, &procEntry))
	{
		return 1;
	}

	while (Process32Next(hSnap, &procEntry))
	{
		if (wcscmp(procEntry.szExeFile,L"winlogon.exe") == 0)
		{
			//we have found the process
			//make sure it is running in console session
			DWORD winlogonSessId = 0;
			//Retrieves the Remote Desktop Services session associated with a specified process.
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				winLogonPid = procEntry.th32ProcessID;
				break;
			}
		}
	}

//---------------------------------------------------------------------------------------------------------------------
	//Obtains the primary access token of the logged-on user specified by the session ID.
	WTSQueryUserToken(dwSessionId, &hUserToken);

	//open process handle to impersonate
	hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, winLogonPid);

	
	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID | TOKEN_READ | TOKEN_WRITE, &hPToken))
	{
		cout << "process token open Error" << endl;
	}

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);

	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		cout << "Lookup privilege value failed to return" << endl;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


	//Adjust token privileges
	SetTokenInformation(hUserTokenDup, TokenSessionId, (void*)dwSessionId, sizeof(DWORD));


	if (!AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), PTOKEN_PRIVILEGES(NULL), NULL))
	{
		cout << "Adjust privilege failed" << endl;
	}

	LPVOID pEnv = NULL;

	if (!CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT;
	}
	else
	{
		pEnv = NULL;
	}

	//launch process in the clients login session

	int bResult = CreateProcessAsUser(
		hUserTokenDup,	//client's access token
		szPath,
		NULL,
		NULL,
		NULL,
		FALSE,
		dwCreationFlags,
		NULL,
		NULL,
		&stInfo,
		&pInfo
	);

	if (bResult == 0)
	{
		cout << "CreateProcess failed" << endl;
		return 0;
	}


	//close all handles
	CloseHandle(hProcess);
	CloseHandle(hUserToken);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);
	
	return 3; 
}

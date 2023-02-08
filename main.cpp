#include "injection.h"



const char szDllFile[] = "";//PUT THE DLL LOCATION HERE C://desktop//dll.dll
const char szProc[] = ""; //CHANGE THIS TO THE CORRESPONDING GAME Name with .exe


int main()
{
	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		DWORD Err = GetLastError();
		printf("[!] Process SnapShot Error: 0x%X\n", Err);
		system("PAUSE");
		return 0;
	}
	else
	{
		printf("[+] Process SnapShot Success\n");
	}

	DWORD PID = 0;
	BOOL bRet = Process32First(hSnap, &PE32);
	while (bRet)
	{
		if (!strcmp(szProc, PE32.szExeFile))
		{
			PID = PE32.th32ProcessID;
			break;

		}
		bRet = Process32Next(hSnap, &PE32);
	}

	CloseHandle(hSnap);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc)
	{
		DWORD Err = GetLastError();
		printf("[!] Open Process Error: 0x%X\n", Err);
		system("PAUSE");
		return 0;
	}

	if (!ManualMap(hProc, szDllFile))
	{
		CloseHandle(hProc);
		printf("[!] Failed To Inject into %s", szProc);
		system("PAUSE");
		return 0;
	}
	else
	{
		printf("[+] Injection Successful...\n");
	}

	CloseHandle(hProc);
	return 0;



}
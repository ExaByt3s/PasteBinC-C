#include <windows.h>
#include "C Runtime.h"
#define BUFSIZE 512
#define PIPE_NAME L"\\\\.\\pipe\\PipeName"
typedef struct _RootKit_Config {
	HANDLE hPipe;
	DWORD dwProcessId;
	LPWSTR lpszFilePath;
	LPWSTR lpszRegKey;
} Rootkit_Config, *PRootkit_Config;
BOOL My_CreatePipe(PRootkit_Config PipeConfig) {
	PipeConfig->hPipe = CreateNamedPipeW(
		PIPE_NAME,             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFSIZE,                  // output buffer size 
		BUFSIZE,                  // input buffer size 
		0,                        // client time-out 
		NULL);                    // default security attribute 
	if (PipeConfig->hPipe == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	return TRUE;

}

DWORD WINAPI InstanceThread(LPVOID lpParam)
{
	PRootkit_Config Config = lpParam;
	
//	WriteToPipe(Config->hPipe, szFilePath, sizeof(szFilePath));
	//WriteToPipe(Config->hPipe, szRegistryPath, sizeof(szRegistryPath));


	FlushFileBuffers(Config->hPipe);
	DisconnectNamedPipe(Config->hPipe);
	CloseHandle(Config->hPipe);

	return TRUE;
}

int pipe_main(VOID)
{
	for (;;)
	{
		HANDLE hThread = NULL;
		PRootkit_Config Config = HeapAlloc(GetProcessHeap(), 0, sizeof(Rootkit_Config));
		_ZeroMemory(Config, sizeof(Rootkit_Config));
		if (Config) {
			My_CreatePipe(Config);
			BOOL bConnectionStatus = ConnectNamedPipe(Config->hPipe, NULL);
			if (!bConnectionStatus) {
				if (GetLastError() == ERROR_PIPE_CONNECTED)
					bConnectionStatus = TRUE;
			}
			if (bConnectionStatus)
			{
				//printf("Client connected, creating a processing thread.\n");
				hThread = CreateThread(NULL, 0, InstanceThread, (LPVOID)Config, 0, NULL);
				if (!hThread)
				{
					CloseHandle(hThread);
				}
			}
			else {
				CloseHandle(Config->hPipe);
				HeapFree(GetProcessHeap(), 0, Config);
			}
		}
	}

	return 0;
}
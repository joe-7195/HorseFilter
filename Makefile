GCC=x86_64-w64-mingw32-gcc
WINDRES=x86_64-w64-mingw32-windres

GCC_ARGS=-shared -lws2_32 -Ishared

clean:
	$(info cleaning horsefilter)
	- rm shared/horsefilter.o network-provider/horsefilter.dll notification-package/horsefilter.dll

build: clean
	$(info building horsefilter)
	$(WINDRES) shared/horsefilter.rc -o shared/horsefilter.o
	$(GCC) network-provider/horsefilter.c shared/horsefilter.o -o network-provider/horsefilter.dll $(GCC_ARGS)
	$(GCC) notification-package/horsefilter.c shared/horsefilter.o -o notification-package/horsefilter.dll $(GCC_ARGS)
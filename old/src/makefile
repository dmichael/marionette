DESTDIR=..

current: 
	@echo "[unnature build]: please use one of the following configurations:"; echo "   make linux-alsa, make linux-jack, make linux-oss,"; echo "   make osx, make osx-ub, or make win32"

install:
	cp $(wildcard unnature unnature.exe) $(DESTDIR)/
	chmod 755 $(DESTDIR)/$(wildcard unnature unnature.exe)

osx:
	-make -f makefile.osx

osx-ppc:
	-make -f makefile.osx

osx-intel:
	-make -f makefile.osx-old

linux-gumstix:
	-make -f makefile.gumstix
	
# Mac OS X universal binary
osx-ub:
	-make -f makefile.osx-ub

linux-oss: 
	-make -f makefile.oss 

linux-jack:
	-make -f makefile.jack

linux-alsa: 
	-make -f makefile.alsa

win32: 
	-make -f makefile.win32

osx-rl:
	-make -f makefile.rl

probe:
	g++ -Wall -D__LINUX_ALSA__ -o probe probe.cpp RtAudio.cpp -lasound -lpthread

clean:
	rm -f *.o unnature.tab.c unnature.tab.h unnature.yy.c unnature.output $(wildcard unnature unnature.exe)

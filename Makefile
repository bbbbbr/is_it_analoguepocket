CC	= ~/git/gbdev/gbdk2020/gbdk-2020-git/build/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -v -debug

# BINS	= sound.gb sound.duck
BINS	= is_it_analoguepocket.gb is_it_analoguepocket.duck is_it_analoguepocket.pocket is_it_analoguepocket.gbc

all:	$(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile and link single file in one pass
%.gb: main.c
	$(CC) -msm83:gb -o $@ $<

%.gbc: main.c
	$(CC) -msm83:gb -Wm-yc -o $@ $<

# Build megaduck version with -m port:plat flag
%.duck:	main.c
	$(CC) -msm83:duck -o $@ $<

# Build analogue pocket version with -m port:plat flag
%.pocket: main.c
	$(CC) -msm83:ap -o $@ $<

clean:
	rm -f *.o *.lst *.map *.duck *.pocket *.gb *.gbc *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi
 

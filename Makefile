# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
GBDK_HOME = ../../../
endif

LCC = $(GBDK_HOME)bin/lcc

# .md1 is Megaduck with the extension that AP core wants (normally .duck in GBDK)
BINS	= is_it_analoguepocket.gb is_it_analoguepocket.md1 is_it_analoguepocket.gbc is_it_analoguepocket_DMG.pocket is_it_analoguepocket_GBC.pocket

all:	$(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile and link single file in one pass
%.gb: main.c
	$(LCC) -msm83:gb -o $@ $<

%.gbc: main.c
	$(LCC) -msm83:gb -Wm-yc -o $@ $<

# Build megaduck version with -m port:plat flag
%.md1:	main.c
	$(LCC) -msm83:duck -o $@ $<

# Build analogue pocket version with -m port:plat flag
is_it_analoguepocket_DMG.pocket: main.c
	$(LCC) -msm83:ap -o $@ $<

# Build analogue pocket version with -m port:plat flag
is_it_analoguepocket_GBC.pocket: main.c
	$(LCC) -msm83:ap -Wm-yc -o $@ $<

clean:
	rm -f *.o *.lst *.map *.md1 *.duck *.pocket *.gb *.gbc *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi
 

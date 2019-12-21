# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        ?= DINO
COMPRESSED  ?= YES
ICON        ?= iconc.png
DESCRIPTION ?= "Dino Run CE"

# ----------------------------
# Other Options (Advanced)
# ----------------------------

GIT_COMMIT := $(shell git rev-parse --short HEAD)
GIT_DIFF_STATUS := $(shell git diff-index --quiet HEAD; echo $$?)

EXTRA_CFLAGS        += COMMIT=$(GIT_COMMIT) DIFF_STATUS=$(GIT_DIFF_STATUS)
#USE_FLASH_FUNCTIONS ?= YES|NO
#OUTPUT_MAP          ?= YES|NO
#ARCHIVED            ?= YES|NO
#OPT_MODE            ?= -optsize|-optspeed
#SRCDIR              ?= src
#OBJDIR              ?= obj
#BINDIR              ?= bin
#GFXDIR              ?= src/gfx
#V                   ?= 1

include $(CEDEV)/include/.makefile

$(OBJDIR)/font/font.src: $(SRCDIR)/font/font.inc

$(SRCDIR)/font/font.inc: $(SRCDIR)/font/dino.fnt
	convfont -o carray -f $(SRCDIR)/font/dino.fnt $(SRCDIR)/font/font.inc
	
clean_gfx:
	-rm src/gfx/convpng.log
	-rm src/gfx/*.c
	-rm src/gfx/*.h

usb:
	make clean
	make EXTRA_CFLAGS=USE_USB=1

no_usb:
	echo $(GIT_COMMIT)
	make clean
	make EXTRA_CFLAGS=USE_USB=0

# Format a release for the Cemetech archives
release:
	make clean
	make clean_gfx

	-rm -r dino_run
	mkdir dino_run
	cp readme.md dino_run
	markdown readme.md > dino_run/readme.html

	mkdir dino_run/source
	cp -r src iconc.png makefile dino_run/source

	make gfx
	make no_usb
	cp bin/DINO.8xp dino_run/DINO.8xp

	-rm dino_run.zip
	zip -r dino_run.zip dino_run
	rm -r dino_run

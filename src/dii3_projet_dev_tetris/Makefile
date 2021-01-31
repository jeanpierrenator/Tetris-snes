# path to snesdev root directory (for emulators, devkitsnes, libsnes)
export DEVKITSNES := /c/snesdev/

# path to devkitsnes root directory for compiler
export DEVKIT65XX := /c/snesdev/devkitsnes

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PVSNESLIB)
LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))

export AUDIOFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.it)))

ifneq ($(AUDIOFILES),)
ifeq ($(wildcard soundbank.asm),)
	SFILES		:=	soundbank.asm $(SFILES)
endif
endif

#---------------------------------------------------------------------------------
export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
SMCONVFLAGS	:= -l -s -o soundbank -v -b 5

GTITLE 		:= -ht"$(TARGET)"
 
.PHONY: bitmaps all
 
#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc
	sed 's/://' <$(TARGET).sym >$(TARGET).tmp
	mv $(TARGET).tmp $(TARGET).sym

clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).sfc $(TARGET).sym *.map *.pic *.pal *.bnk soundbank.asm soundbank.h

#---------------------------------------------------------------------------------
pvsneslibfont.pic: pvsneslibfont.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -pc4 -n -gs8 -po2 -pe0 -mR! -m $<

#---------------------------------------------------------------------------------
sprite8.pic: sprite8.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs8 -pc16 -po16 -pe1 -n -pr -m $<
	#cp sprite8.map.new sprite8.map

fond.pic: bg0.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs8 -pc16 -po16 -pe2 -n -pr -m $<

soundbank.asm : $(AUDIOFILES)
	@echo Compiling Soundbank ... 
	@$(SMCONV) $(SMCONVFLAGS) $^

line.brr: sfxJump.wav
	@echo convert wav file ... $(notdir $<)
	$(BRCONV) -e $< $@

place.brr: dano.wav
	@echo convert wav file ... $(notdir $<)
	$(BRCONV) -e $< $@

bmpmenuup.pic: bmpmenuup.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe2  -fbmp -m $<

bmpmenucenter.pic: bmpmenucenter.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe1  -fbmp -m $<

bmpmenudown.pic: bmpmenudown.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc4 -n -gs8 -pe0  -fbmp -m $<


bmpmenutitle.pic: bmpmenutitle.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe1  -fbmp -m $<

bitmaps : pvsneslibfont.pic sprite8.pic fond.pic pvsneslib.pic logo_polytech_1.pic soundbank.asm line.brr place.brr bmpmenudown.pic bmpmenuup.pic bmpmenucenter.pic  bmpmenutitle.pic

#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)

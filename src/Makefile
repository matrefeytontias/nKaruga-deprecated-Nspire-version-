DEBUG = FALSE
GCC = nspire-gcc
AS = nspire-as
GXX=nspire-g++
LD = nspire-ld-bflt
GCCFLAGS = -Wall -W -marm -Ofast
LDFLAGS = 
ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif
CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS = $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.S,%.o,$(wildcard *.S)) $(CPPOBJS)
ifneq ($(strip $(CPPOBJS)),)
	LDFLAGS += --cpp
endif
EXE = nKaruga.tns
EXE_EMU = nKaruga_emu.tns
DISTDIR = ../
vpath %.tns $(DISTDIR)

all: $(EXE) $(EXE_EMU)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $<
	
%.o: %.S
	$(AS) -c $<

main_emu.o: GCCFLAGS+= -D DEBUG_NKARUGA
main_emu.o: main.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@
$(EXE): $(OBJS)

$(EXE_EMU): main_emu.o  $(filter-out main.o,$(OBJS))

%.tns:
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)
ifeq ($(DEBUG),FALSE)
	@rm -f $(DISTDIR)/*.gdb
endif

clean:
	rm -f *.o *.elf $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE) $(DISTDIR)/$(EXE_EMU)

###########################
##  Headers dependencies ##
###########################
Particles.cpp : common.h
Player.cpp : common.h
main.cpp : common.h levels.h misc_data.h
BulletArray.cpp : common.h
utils.cpp : common.h
ExplosionAnim.cpp : common.h
levels.h : common.h
buildGameLUTs.cpp : common.h
PowerFragment.cpp : common.h
Enemy.cpp : common.h patterns.h
Laser.cpp : common.h
Bullet.cpp : common.h
ChainNotif.cpp : common.h
DestroyedEnemies : common.h
n2DLib.c : n2DLib_font.h n2DLib.h
common.h : n2DLib.h

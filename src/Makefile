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

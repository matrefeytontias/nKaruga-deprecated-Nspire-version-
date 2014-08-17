DEBUG = FALSE
GCC = nspire-gcc
AS = nspire-as
GXX=nspire-g++
LD = nspire-ld-bflt
GCCFLAGS = -Wall -W -marm -Ofast
HEADER_DEPENDS_DIR = .depends
HEADER_DEPENDS_FLAG = -MM -MF $(HEADER_DEPENDS_DIR)/$<.d
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

#	include *.d
#endif

all: $(EXE) $(EXE_EMU)

$(OBJS) : $(HEADER_DEPENDS_DIR)
$(HEADER_DEPENDS_DIR):
	mkdir $(HEADER_DEPENDS_DIR)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< $(HEADER_DEPENDS_FLAG)
	$(GCC) $(GCCFLAGS) -c $< -o $@

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $< $(HEADER_DEPENDS_FLAG)
	$(GXX) $(GCCFLAGS) -c $< -o $@

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
	rm -f *.o *.elf $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE) $(DISTDIR)/$(EXE_EMU) .depends/*.d
	rmdir .depends

ifneq ($(ls .depends/*.d),)
	include .depends/*.d
endif

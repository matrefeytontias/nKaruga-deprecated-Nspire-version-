DEBUG = FALSE

GCC = nspire-gcc
AS  = nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Wall -W -marm
LDFLAGS =
ZEHNFLAGS = --name "$(EXE)"
ZEHNFLAGSEMU = --name "$(EXE_EMU)"

# test
HEADER_DEPENDS_DIR = .depends
HEADER_DEPENDS_FLAG = -MM -MF $(HEADER_DEPENDS_DIR)/$<.d
DEPEND_FILE_NAMES = $(patsubst %.cpp,%.cpp.d,$(wildcard *.cpp)) $(patsubst %.c,%.c.d,$(wildcard *.c))
# Append the directory to depend files
DEPEND_FILES = $(foreach file, $(DEPEND_FILE_NAMES), $(HEADER_DEPENDS_DIR)/$(file))
# test

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
endif

OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))
EXE = nKaruga
EXE_EMU = nKaruga_emu
DISTDIR = ../
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(HEADER_DEPENDS_DIR) header_dependencies $(EXE).prg.tns $(EXE_EMU).prg.tns

$(HEADER_DEPENDS_DIR):
	mkdir $(HEADER_DEPENDS_DIR)

header_dependencies: $(DEPEND_FILES)

include $(wildcard $(HEADER_DEPENDS_DIR)/*.d)

$(HEADER_DEPENDS_DIR)/%.cpp.d : %.cpp
	$(GCC) $(GCCFLAGS) -c $< $(HEADER_DEPENDS_FLAG)

$(HEADER_DEPENDS_DIR)/%.c.d : %.c
	$(GCC) $(GCCFLAGS) -c $< $(HEADER_DEPENDS_FLAG)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $<
	
%.o: %.S
	$(AS) -c $<

main_emu.o: GCCFLAGS += -D DEBUG_NKARUGA
main_emu.o: main.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGS)

$(EXE).prg.tns: $(EXE).tns
	make-prg $(DISTDIR)/$^ $(DISTDIR)/$@
	
$(EXE_EMU).elf: main_emu.o $(filter-out ./main.o,$(OBJS))
# $(warning Linking $(filter-out main.o,$(OBJS)) using $(LD))
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

$(EXE_EMU).tns: $(EXE_EMU).elf
	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGSEMU)

$(EXE_EMU).prg.tns: $(EXE_EMU).tns
	make-prg $(DISTDIR)/$^ $(DISTDIR)/$@

clean:
	rm -f *.o $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).prg.tns $(DISTDIR)/$(EXE_EMU).tns $(DISTDIR)/$(EXE_EMU).elf $(DISTDIR)/$(EXE_EMU).prg.tns $(HEADER_DEPENDS_DIR)/*.d
	rm -rf .depends

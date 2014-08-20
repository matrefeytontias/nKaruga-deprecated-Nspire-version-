DEBUG = FALSE
GCC = nspire-gcc
AS = nspire-as
GXX=nspire-g++
LD = nspire-ld-bflt
GCCFLAGS = -Wall -W -marm -Ofast
HEADER_DEPENDS_DIR = .depends
HEADER_DEPENDS_FLAG = -MM -MF $(HEADER_DEPENDS_DIR)/$<.d
DEPEND_FILE_NAMES = $(patsubst %.cpp,%.cpp.d,$(wildcard *.cpp)) $(patsubst %.c,%.c.d,$(wildcard *.c))
# Append the directory to depend files
DEPEND_FILES = $(foreach file, $(DEPEND_FILE_NAMES), $(HEADER_DEPENDS_DIR)/$(file))
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

all: $(HEADER_DEPENDS_DIR) header_dependencies  $(EXE) $(EXE_EMU)

$(HEADER_DEPENDS_DIR):
	mkdir $(HEADER_DEPENDS_DIR)

header_dependencies: $(DEPEND_FILES)

include $(wildcard $(HEADER_DEPENDS_DIR)/*.d)

$(HEADER_DEPENDS_DIR)/%.cpp.d : %.cpp
	$(GCC) $(GCCFLAGS) -c $< $(HEADER_DEPENDS_FLAG)

$(HEADER_DEPENDS_DIR)/%.c.d : %.c
	$(GCC) $(GCCFLAGS) -c $< $(HEADER_DEPENDS_FLAG)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@

%.o: %.S $(AS) -c $<

main_emu.o: GCCFLAGS+= -D DEBUG_NKARUGA
main_emu.o: main.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@
$(EXE): $(OBJS)

$(EXE_EMU): main_emu.o  $(filter-out main.o,$(OBJS))

%.tns:
	mkdir -p $(DISTDIR)
	$(LD) $(filter-out header_dependencies,$^) -o $(DISTDIR)/$@ $(LDFLAGS)
ifeq ($(DEBUG),FALSE)
	@rm -f $(DISTDIR)/*.gdb
endif

clean:
	rm -f *.o *.elf $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE) $(DISTDIR)/$(EXE_EMU) $(HEADER_DEPENDS_DIR)/*.d
	rm -rf .depends

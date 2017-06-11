GCC = nspire-g++
LD = nspire-ld
GCCFLAGS = -Os -Wall -W -marm
LDFLAGS = 
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = nYahtzee.tns
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.cpp
	$(GCC) $(GCCFLAGS) -c $<

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $(@:.tns=.elf) -lRGB
	mkdir -p $(DISTDIR)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(DISTDIR)/$@

clean:
	rm -f *.o *.elf
	rm -f $(DISTDIR)/$(EXE)

CXXFILES := CpuArithmetic.cpp CpuRegisters.cpp CpuState.cpp MemoryState.cpp NESEmulator.cpp 
PROG := nesemulator
CXXFLAGS := -g -Wall -Wextra
LDFLAGS := -Wall -Wextra -lstdc++

CXXFLAGS += -MMD
CXX := g++

OBJFILES := $(CXXFILES:.cpp=.o)
DEPFILES := $(CXXFILES:.cpp=.d)

$(PROG) : $(OBJFILES)
	$(LINK.o) $(LDFLAGS) -o $@ $^

clean :
	rm -f $(PROG) $(PROG).exe $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)

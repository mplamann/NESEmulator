CXXFILES := CpuArithmetic.cpp CpuRegisters.cpp CpuState.cpp MemoryState.cpp NESEmulator.cpp CpuBoolean.cpp CpuBranches.cpp mappers/Mapper.cpp mappers/Mapper0.cpp
PROG := nesemulator
CXXFLAGS := -gstabs -Wall -Wextra -Imappers
LDFLAGS := -Wall -Wextra -lstdc++

CXXFLAGS += -MMD
CXX := g++
CC = g++

OBJFILES := $(CXXFILES:.cpp=.o)
DEPFILES := $(CXXFILES:.cpp=.d)

$(PROG) : $(OBJFILES)
	$(LINK.o) $(LDFLAGS) -o $@ $^

clean :
	rm -f $(PROG) $(PROG).exe $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)

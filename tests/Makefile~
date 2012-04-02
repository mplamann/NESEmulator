CXXFILES := CppUnitLite/Failure.cpp CppUnitLite/SimpleString.cpp CppUnitLite/Test.cpp CppUnitLite/TestRegistry.cpp CppUnitLite/TestResult.cpp RegistersTest.cpp BooleanTest.cpp
CXXFILES += ../src/CpuArithmetic.cpp ../src/CpuBoolean.cpp ../src/CpuBranches.cpp ../src/CpuRegisters.cpp ../src/CpuState.cpp ../src/MemoryState.cpp
PROG := test

INC_PATH = -I. -I../src
LIB_PATH = -L../src
LD_PATH = ../src

CXXFLAGS := -g -Wall -Wextra
LDFLAGS := -Wall -Wextra -lstdc++ $(LIB_PATH) 

CXXFLAGS += -MMD $(INC_PATH)
CXX := g++
CC = g++

OBJFILES := $(CXXFILES:.cpp=.o)
DEPFILES := $(CXXFILES:.cpp=.d)

$(PROG) : $(OBJFILES)
	$(LINK.o) -o $@ $^

clean :
	rm -f $(PROG).exe $(PROG) $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)
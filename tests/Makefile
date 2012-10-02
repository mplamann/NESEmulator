CXXFILES := CppUnitLite/Failure.cpp CppUnitLite/SimpleString.cpp CppUnitLite/Test.cpp CppUnitLite/TestRegistry.cpp CppUnitLite/TestResult.cpp RegistersTest.cpp BooleanTest.cpp ArithmeticTest.cpp BranchesTest.cpp
CXXFILES += ../src/CpuArithmetic.cpp ../src/CpuBoolean.cpp ../src/CpuBranches.cpp ../src/CpuRegisters.cpp ../src/CpuState.cpp ../src/MemoryState.cpp ../src/GamepadState.cpp ../src/serial.cpp ../src/mappers/Mapper.cpp ../src/mappers/Mapper0.cpp
PROG := test

INC_PATH = -I. -I../src -I../src/mappers
LIB_PATH = -L../src -L../src/mappers
LD_PATH = ../src

CXXFLAGS := -g -Wall -Wextra `pkg-config --cflags allegro-5.0 allegro_main-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_ttf-5.0 allegro_audio-5.0 allegro_primitives-5.0`
LDFLAGS := -Wall -Wextra -lstdc++ $(LIB_PATH) `pkg-config --libs allegro-5.0 allegro_main-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_ttf-5.0 allegro_audio-5.0 allegro_primitives-5.0`

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

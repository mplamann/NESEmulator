CXXFILES := CpuArithmetic.cpp CpuRegisters.cpp CpuState.cpp MemoryState.cpp NESEmulator.cpp CpuBoolean.cpp CpuBranches.cpp Mapper.cpp Mapper0.cpp Mapper1.cpp Mapper2.cpp Mapper4.cpp Mapper7.cpp PpuState.cpp GamepadState.cpp ApuState.cpp nes_apu/Blip_Buffer.cpp nes_apu/Multi_Buffer.cpp nes_apu/Nes_Apu.cpp nes_apu/apu_snapshot.cpp nes_apu/Nes_Namco.cpp nes_apu/Nes_Oscs.cpp nes_apu/Nes_Vrc6.cpp nes_apu/Nonlinear_Buffer.cpp serial.cpp CpuV2.cpp
PROG := nesemulator
CXXFLAGS := -gstabs -O3 -Wall -Wextra -Ines_apu -I. `sdl-config --cflags`
LDFLAGS := -Wall -Wextra -lstdc++ `sdl-config --cflags --libs`

CXXFLAGS += -MMD
CXX := g++
CC = g++

OBJFILES := $(CXXFILES:.cpp=.o)
DEPFILES := $(CXXFILES:.cpp=.d)

$(PROG) : $(OBJFILES)
	$(LINK.o) -o $@ $^ $(LDFLAGS)

clean :
	rm -f $(PROG) $(PROG).exe $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)

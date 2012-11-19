CXXFILES := CpuArithmetic.cpp CpuRegisters.cpp CpuState.cpp MemoryState.cpp NESEmulator.cpp CpuBoolean.cpp CpuBranches.cpp mappers/Mapper.cpp mappers/Mapper0.cpp mappers/Mapper2.cpp PpuState.cpp GamepadState.cpp ApuState.cpp nes_apu/Blip_Buffer.cpp nes_apu/Multi_Buffer.cpp nes_apu/Nes_Apu.cpp nes_apu/apu_snapshot.cpp nes_apu/Nes_Namco.cpp nes_apu/Nes_Oscs.cpp nes_apu/Nes_Vrc6.cpp nes_apu/Nonlinear_Buffer.cpp serial.cpp
PROG := nesemulator
CXXFLAGS := -gstabs -Wall -Wextra -Imappers -Ines_apu -I. `pkg-config --cflags allegro-debug-5.1 allegro_main-debug-5.1 allegro_dialog-debug-5.1 allegro_font-debug-5.1 allegro_ttf-debug-5.1 allegro_audio-debug-5.1 allegro_primitives-debug-5.1`
LDFLAGS := -Wall -Wextra -lstdc++ `pkg-config --libs allegro-debug-5.1 allegro_main-debug-5.1 allegro_dialog-debug-5.1 allegro_font-debug-5.1 allegro_ttf-debug-5.1 allegro_audio-debug-5.1 allegro_primitives-debug-5.1`

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

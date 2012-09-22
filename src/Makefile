CXXFILES := CpuArithmetic.cpp CpuRegisters.cpp CpuState.cpp MemoryState.cpp NESEmulator.cpp CpuBoolean.cpp CpuBranches.cpp mappers/Mapper.cpp mappers/Mapper0.cpp PpuState.cpp GamepadState.cpp ApuState.cpp nes_apu/Blip_Buffer.cpp nes_apu/Multi_Buffer.cpp nes_apu/Nes_Apu.cpp nes_apu/apu_snapshot.cpp nes_apu/Nes_Namco.cpp nes_apu/Nes_Oscs.cpp nes_apu/Nes_Vrc6.cpp nes_apu/Nonlinear_Buffer.cpp
PROG := nesemulator
CXXFLAGS := -gstabs -Wall -Wextra -Imappers -Ines_apu -I. `pkg-config --cflags allegro-5.0 allegro_main-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_ttf-5.0 allegro_audio-5.0 allegro_primitives-5.0`
LDFLAGS := -Wall -Wextra -lstdc++ `pkg-config --libs allegro-5.0 allegro_main-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_ttf-5.0 allegro_audio-5.0 allegro_primitives-5.0`

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

#pragma once

#include "Nes_Apu.h" // Thanks to Blargg's Audio Libraries
#include <fstream>
using namespace std;

#define SAMPLES_PER_BUFFER 1024

class MemoryState;
class CpuV2;

class ApuState
{
 public:
  Nes_Apu* apu;
  ApuState(void);
  ~ApuState(void);
  bool initializeAudio();
  void audioStreamFragment();
  void setMemory(MemoryState* memory);
  void setCpu(CpuV2* cpu);

  void write_register(unsigned address, int data);
  int read_status(long cycles);
  void finishFrame();

  void saveState(ofstream& file);
  void loadState(ifstream& file);

 private:
  Blip_Buffer* buf;
  MemoryState* memory;
  CpuV2* cpu;

  void output_samples(const blip_sample_t*, size_t count);
  blip_sample_t out_buf[SAMPLES_PER_BUFFER];

  //ALLEGRO_AUDIO_STREAM* stream;
};

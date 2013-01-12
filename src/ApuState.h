#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "Nes_Apu.h" // Thanks to Blargg's Audio Libraries

#define SAMPLES_PER_BUFFER 1024

class MemoryState;
class CpuState;

class ApuState
{
 public:
  Nes_Apu* apu;
  ApuState(void);
  ~ApuState(void);
  bool initializeAudio(ALLEGRO_EVENT_QUEUE* event_queue);
  void audioStreamFragment();
  void setMemory(MemoryState* memory);
  void setCpu(CpuState* cpu);

  void write_register(unsigned address, int data);
  int read_status(long cycles);
  void finishFrame();

 private:
  Blip_Buffer* buf;
  MemoryState* memory;
  CpuState* cpu;

  void output_samples(const blip_sample_t*, size_t count);
  blip_sample_t out_buf[SAMPLES_PER_BUFFER];

  ALLEGRO_AUDIO_STREAM* stream;
};

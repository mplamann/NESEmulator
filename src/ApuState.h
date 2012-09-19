#pragma once
#import <allegro5/allegro.h>
#import <allegro5/allegro_audio.h>

#include "Nes_Apu.h" // Thanks to Blargg's Audio Libraries

#define SAMPLES_PER_BUFFER 1024

class ApuState
{
 public:
  ApuState(void);
  ~ApuState(void);
  bool initializeAudio(ALLEGRO_EVENT_QUEUE* event_queue);
  void audioStreamFragment();

 private:
  Blip_Buffer buf;
  Nes_Apu apu;

  void output_samples(const blip_sample_t*, size_t count);
  blip_sample_t out_buf[SAMPLES_PER_BUFFER];

  ALLEGRO_AUDIO_STREAM* stream;
};

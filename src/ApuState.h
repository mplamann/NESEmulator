#pragma once
#import <allegro5/allegro.h>
#import <allegro5/allegro_audio.h>

#define SAMPLES_PER_BUFFER 1024

class ApuState
{
 public:
  ApuState(void);
  ~ApuState(void);
  bool initializeAudio(ALLEGRO_EVENT_QUEUE* event_queue);
  void audioStreamFragment();

 private:
  char* buf;
  int pitch;
  int val;
  int i;

  ALLEGRO_AUDIO_STREAM* stream;
};

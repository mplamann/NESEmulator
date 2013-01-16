#include "ApuState.h"
#include "MemoryState.h"
#include "CpuV2.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <math.h>
using namespace std;

int dmc_read(void* memory, cpu_addr_t addr)
{
  return ((MemoryState*)memory)->apuReadByteFrom(NULL, addr);
}

ApuState::ApuState(void)
{
  apu = new Nes_Apu();
  buf = new Blip_Buffer();
  blargg_err_t error = buf->sample_rate(44100, 1000);  // Without a length (ms), this allocates 4GB of RAM. Not so good.
  if (error)
    cout << "Error setting sample rate.\n";
  buf->clock_rate(1789773);
  apu->output(buf);
}

void ApuState::setMemory(MemoryState* _memory)
{
  memory = _memory;
  apu->dmc_reader(dmc_read, memory);
}

void ApuState::setCpu(CpuV2* _cpu)
{ cpu = _cpu; }

ApuState::~ApuState(void)
{
  al_drain_audio_stream(stream);
  al_destroy_audio_stream(stream);
  al_uninstall_audio();
}

bool ApuState::initializeAudio(ALLEGRO_EVENT_QUEUE* event_queue)
{
  cout << "Initializing audio...";
  if (!al_install_audio())
    {
      cout << "Error! Failed to initialize audio.\n";
      return false;
    }

  al_reserve_samples(0); // Creates a mixer for us
  
  stream = al_create_audio_stream(8, SAMPLES_PER_BUFFER, 44100,
				  ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_1);
  if (!stream)
    {
      cout << "Error! Failed to create audio stream.\n";
      return false;
    }

  if (!al_attach_audio_stream_to_mixer(stream, al_get_default_mixer()))
    {
      cout << "Error! Failed to attach stream to mixer.\n";
      return false;
    }

  al_register_event_source(event_queue, al_get_audio_stream_event_source(stream));
  cout << "Done.\n";
  return true;
}

void ApuState::finishFrame()
{
  apu->end_frame(cpu->elapsed());
  buf->end_frame(cpu->elapsed());
  cpu->cycles -= cpu->elapsed(); //incrementTotalCycles(-cpu->elapsed());
}

void ApuState::audioStreamFragment()
{
  blip_sample_t* fragment = (blip_sample_t*)al_get_audio_stream_fragment(stream);
  if (!fragment)
    return;

  if (buf->samples_avail() >= SAMPLES_PER_BUFFER)
    {
      buf->read_samples(fragment, SAMPLES_PER_BUFFER);
    }
  
  for (int i = 0; i < SAMPLES_PER_BUFFER; i++) {
    //cout << dec << "fragment[" << i << "] = " << fragment[i] << "\n";
  }
  
  if (!al_set_audio_stream_fragment(stream, fragment))
    {
      cout << "Error setting stream fragment.\n";
    }
}

void ApuState::write_register(unsigned address, int data)
{
  apu->write_register(cpu->elapsed(), address, data);
}

int ApuState::read_status(long cycles)
{
  return apu->read_status(cycles);
}


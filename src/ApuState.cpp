#include "ApuState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
using namespace std;

ApuState::ApuState(void)
{

}

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
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize audio!", NULL, NULL);
      return false;
    }

  al_reserve_samples(0); // Creates a mixer for us
  
  stream = al_create_audio_stream(8, SAMPLES_PER_BUFFER, 22050,
				  ALLEGRO_AUDIO_DEPTH_UINT8, ALLEGRO_CHANNEL_CONF_1);
  if (!stream)
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to create stream!", NULL, NULL);
      return false;
    }

  if (!al_attach_audio_stream_to_mixer(stream, al_get_default_mixer()))
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to attach stream to mixer!", NULL, NULL);
      return false;
    }

  al_register_event_source(event_queue, al_get_audio_stream_event_source(stream));
  cout << "Done.\n";
  return true;
}

void ApuState::audioStreamFragment()
{
  /*buf = (char*)al_get_audio_stream_fragment(stream);
  if (!buf)
    return;
  /*for (i = 0; i < SAMPLES_PER_BUFFER; i++)
    {
      // Create saw waves
      buf[i] = ((val >> 16) & 0xFF);
      val += pitch;
      pitch++;
      }*/
  /*if (!al_set_audio_stream_fragment(stream, buf))
    {
      cout << "Error setting stream fragment.\n";
      }*/
}

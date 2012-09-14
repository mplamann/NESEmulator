#import "PpuState.h"
#import "PpuColors.h"
#import <iostream>
using namespace std;

bool PpuState::initializeDisplay(ALLEGRO_EVENT_QUEUE* event_queue)
{
  int height = 640;
  int width = 480;
  display = NULL;
  if (!al_init())
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize allegro!", NULL, NULL);
      return false;
    }
  display = al_create_display(width, height);
  if (!display)
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to initialize display!", NULL,NULL);
      return false;
    }
  al_register_event_source(event_queue, al_get_display_event_source(display));
  return true;
}

void PpuState::startFrame()
{
  al_clear_to_color(al_map_rgb(0,0,0));
}

void PpuState::renderScanline(int scanline)
{
  // Assume VBlank is done
  memory->PPUSTATUS &= 0x7F;

  if (memory->PPUMASK & 0x10)
    {
      for (int i = 0; i < 64; i++)
	{
	  // Check if sprite is visible on this scanline
	  int yCoord = memory->oamReadByteFrom(i*4);
	  if ((yCoord > scanline || scanline-yCoord >= 8) || yCoord > 0xEF)
	    continue;
	  int spriteLine = scanline-yCoord;
	  int patternTableTile = memory->oamReadByteFrom(i*4+1);
	  int patternTableIndex = patternTableTile*16;
	  int patternTablePlane1 = memory->ppuReadByteFrom(patternTableIndex + spriteLine);
	  int patternTablePlane2 = memory->ppuReadByteFrom(patternTableIndex + spriteLine + 8);
	  int xOffset = memory->oamReadByteFrom(i*4+3);
	  int paletteIndex = memory->oamReadByteFrom(i*4+2) & 0x3;
	  for (int x = 0; x < 8; x++)
	    {
	      int andOperator = 1<<(7-x);
	      int colorIndex = (patternTablePlane1 & andOperator) + 2*(patternTablePlane2 & andOperator);
	      colorIndex = colorIndex >> (7-x);
	      if (colorIndex != 0)
		{
		  char paletteColorIndex = memory->colorForPaletteIndex(true, paletteIndex, colorIndex);
		  ALLEGRO_COLOR* paletteColors = getPaletteColors();
		  ALLEGRO_COLOR color = paletteColors[paletteColorIndex];
		  al_put_pixel(xOffset+x,scanline,color);
		}
	    }
        }
    }
      else
	{
	  //cout << "Not Rendering\n";
	}
}

void PpuState::endFrame()
{
  // Assume VBlank is starting
  memory->PPUSTATUS |= 0x80;
  al_flip_display();
}

PpuState::PpuState()
{
  display = NULL;
}

PpuState::~PpuState()
{
  if (display != NULL)
    al_destroy_display(display);
  
}

void PpuState::setMemory(MemoryState* mem)
{
  memory = mem;
}

int PpuState::getCycles() {return cycles;}
MemoryState* PpuState::getMemory() {return memory;}

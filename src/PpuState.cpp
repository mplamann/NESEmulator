#import "PpuState.h"
#import "PpuColors.h"
#import <iostream>
using namespace std;

bool PpuState::initializeDisplay(ALLEGRO_EVENT_QUEUE* event_queue)
{
  int height = 224;
  int width = 256;
  cout << "Initializing display...";
  display = al_create_display(width, height);
  if (!display)
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to initialize display!", NULL,NULL);
      cout << "Error!\n";
      return false;
    }
  al_register_event_source(event_queue, al_get_display_event_source(display));

  if (!al_init_primitives_addon())
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to initialize primitives!", NULL,NULL);
      cout << "Error!\n";
      return false;
    }
  
  cout << "Done.\n";
  return true;
}

void PpuState::setDisplayTitle(const char* title)
{
  al_set_window_title(display, title);
}

void PpuState::startFrame()
{
  //al_clear_to_color(al_map_rgb(0,0,0));
  //  ALLEGRO_BITMAP* bitmap = al_get_target_bitmap();
  //  al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), ALLEGRO_LOCK_WRITEONLY);
  //  ALLEGRO_COLOR black = al_map_rgb(0,0,0);
  /*for (int x = 0; x < al_get_bitmap_width(bitmap); x++)
    for (int y = 0; y < al_get_bitmap_height(bitmap); y++)
    al_put_pixel(x,y,black);*/
}

void PpuState::renderScanline(int scanline)
{
  // Assume VBlank is done
  memory->PPUSTATUS &= 0x7F;

  ALLEGRO_VERTEX scanlinePoints[256];
  for (int i = 0; i < 256; i++)
    {
      scanlinePoints[i].x = i;
      scanlinePoints[i].y = scanline-8;
      scanlinePoints[i].z = 0;
      scanlinePoints[i].color = al_map_rgb(0,0,0);
    }

  if (memory->PPUMASK & 0x08) // If background enabled
    {
      // Screen is 32x30 tiles
      int tileY = scanline / 8;
      int firstTile = tileY*32;
      int tileLine = scanline - (tileY * 8);
      int attributeY = tileY / 4;
      int attrShift = 2*(tileY%4 < 2);
      int firstAttribute = attributeY * 8;
      double totalTimeSpentPuttingPixels = 0;
      for (int i = firstTile; i < firstTile + 32; i++)
	{
	  double start_time = al_get_time();
	  int nameTableAddress = 0x2000 + i;
	  int attributeAddress = 0x23C0 + firstAttribute + (firstTile / 4);

	  int patternTableTile = memory->ppuReadByteFrom(nameTableAddress);
	  cout << patternTableTile << "\n";
	  int patternTableIndex = patternTableTile*16;
	  int patternTablePlane1 = memory->ppuReadByteFrom(patternTableIndex + tileLine);
	  int patternTablePlane2 = memory->ppuReadByteFrom(patternTableIndex +  tileLine + 8);
	  int xOffset = (i-firstTile)*8;
	  for (int x = 0; x < 8; x++)
	    {
	      int andOperator = 1<<(7-x);
	      int colorIndex = (patternTablePlane1 & andOperator) + 2*(patternTablePlane2 & andOperator);
	      colorIndex = colorIndex >> (7-x);

	      int paletteIndex = 0x01; // TODO: obtain from attribute table
	      char paletteColorIndex = memory->colorForPaletteIndex(true, paletteIndex, colorIndex);
	      ALLEGRO_COLOR* paletteColors = getPaletteColors();
	      ALLEGRO_COLOR color = paletteColors[paletteColorIndex];
	      //al_put_pixel(xOffset+x,scanline,color);
	      scanlinePoints[(xOffset+x)&0xFF].color=color;
	      totalTimeSpentPuttingPixels += al_get_time()-start_time;
	    }
	}
      //      cout << "Spent " << totalTimeSpentPuttingPixels << " seconds on scanline " << scanline << "\n";
    } // End if background enabled
  if (memory->PPUMASK & 0x10) // If sprites enabled
    {
      for (int i = 0; i < 64; i++)
	{
	  // Check if sprite is visible on this scanline
	  int yCoord = memory->oamReadByteFrom(i*4);
	  if ((yCoord > scanline || scanline-yCoord >= 8) || yCoord > 0xEF)
	    continue;
	  if (scanline == 0)
	    cout << "What?\n";
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
		  //al_put_pixel(xOffset+x,scanline,color);
		  scanlinePoints[(xOffset+x)&0xFF].color=color;
		}
	    }
        }
    } // End if sprites enabled

  al_draw_prim(scanlinePoints, NULL, 0, 0, 256, ALLEGRO_PRIM_POINT_LIST);
}

void PpuState::endFrame()
{
  //al_draw_prim(pointList, NULL, 0, 0, 256*224, ALLEGRO_PRIM_POINT_LIST);
  // Assume VBlank is starting
  memory->PPUSTATUS |= 0x80;
  //al_unlock_bitmap(al_get_target_bitmap());
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

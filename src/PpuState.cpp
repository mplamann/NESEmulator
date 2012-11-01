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
  nametableDisplay = al_create_display(2*width, height);
  if (!nametableDisplay)
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
  vScroll = memory->PPUSCROLLY;
}

inline int attributeOffsetForTile(int x, int y)
{
  int first = (y >> 2)*8;
  return first + (x >> 2);
}

inline int attributeValueFromByteXY(int byte, int x, int y)
{
  bool xMod = (x%4)<2;
  bool yMod = (y%4)<2;
  if (xMod && yMod)
    return byte & 0x03;
  if (xMod)
    return (byte & 0x30) >> 4;
  if (yMod)
    return (byte & 0x0C) >> 2;
  return (byte & 0xC0) >> 6;
}

void PpuState::renderScanline(int scanline)
{
  scanline += vScroll;
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

  al_set_target_backbuffer(display);
  if (memory->PPUMASK & 0x08) // If background enabled
    {
      int tileY = scanline / 8;
      int lineInTile = scanline - (tileY*8);
      int attributeY = tileY / 4;
      int basePatternTable = (memory->PPUCTRL & 0x10) ? 0x1000 : 0x0000;
      int upperLimit = (memory->PPUSCROLLX & 0x07) ? 33 : 32;
      for (int i = 0; i < upperLimit; i++)
	{
	  int patternTableTile = memory->getNametableEntryForTile(i,tileY,memory->PPUSCROLLX,vScroll);
	  int patternTableIndex = patternTableTile*16;
	  int patternTablePlane1 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + lineInTile);
	  int patternTablePlane2 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex +  lineInTile + 8);
	  int xOffset = i*8 - (memory->PPUSCROLLX & 0x07); // Account for both tile width and X scrolling
	  int paletteIndex = attributeValueFromByteXY(memory->attributeEntryForXY(i,tileY,memory->PPUSCROLLX,vScroll),i,tileY);
	  for (int x = 0; x < 8; x++)
	    {
	      if (i == 0 && x == 0)
		x = (memory->PPUSCROLLX & 0x07);
	      int andOperator = 1<<(7-x);
	      int colorIndex = (patternTablePlane1 & andOperator) + 2*(patternTablePlane2 & andOperator);
	      colorIndex = colorIndex >> (7-x);

	      unsigned char paletteColorIndex = memory->colorForPaletteIndex(false, paletteIndex, colorIndex);
	      ALLEGRO_COLOR* paletteColors = getPaletteColors();
	      ALLEGRO_COLOR color = paletteColors[paletteColorIndex];
	      scanlinePoints[(xOffset+x)&0xFF].color=color;
	    }
	}
    } // End if background enabled

  if (memory->PPUMASK & 0x10) // If sprites enabled
    {
      int basePatternTable = (memory->PPUCTRL & 0x08) ? 0x1000 : 0x0000;
      for (int i = 0; i < 64; i++)
	{
	  // Check if sprite is visible on this scanline
	  int yCoord = memory->oamReadByteFrom(i*4);
	  if ((yCoord > scanline || scanline-yCoord >= 8) || yCoord > 0xEF)
	    continue;
	  int spriteFlags = memory->oamReadByteFrom(i*4+2);
	  
	  int spriteLine = scanline-yCoord;
	  if (spriteFlags & 0x80) // Check for vertical flip
	    spriteLine = 8-spriteLine;
	  int patternTableTile = memory->oamReadByteFrom(i*4+1);
	  int patternTableIndex = patternTableTile*16;
	  int patternTablePlane1 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + spriteLine);
	  int patternTablePlane2 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + spriteLine + 8);
	  int xOffset = memory->oamReadByteFrom(i*4+3);
	  int paletteIndex = spriteFlags & 0x3;
	  
	  for (int x = 0; x < 8; x++)
	    {
	      int andOperator = 1<<(7-x);      
	      if (spriteFlags&0x40) // Check for horizontal flip
		andOperator = 1<<x;
	      int colorIndex = (patternTablePlane1 & andOperator) + 2*(patternTablePlane2 & andOperator);
	      if (spriteFlags & 0x40) // Horizontal flip again
		colorIndex = colorIndex >> x;
	      else
		colorIndex = colorIndex >> (7-x);
	      if (colorIndex != 0)
		{
		  unsigned char paletteColorIndex = memory->colorForPaletteIndex(true, paletteIndex, colorIndex);
		  ALLEGRO_COLOR* paletteColors = getPaletteColors();
		  ALLEGRO_COLOR color = paletteColors[paletteColorIndex];
		  scanlinePoints[(xOffset+x)&0xFF].color=color;
		}
	    }
        }
    } // End if sprites enabled

  al_draw_prim(scanlinePoints, NULL, 0, 0, 256, ALLEGRO_PRIM_POINT_LIST);

  al_set_target_backbuffer(nametableDisplay);
  for (int nametable = 0; nametable < 2; nametable++)
    {
      int tileY = scanline / 8;
      int lineInTile = scanline - (tileY*8);
      int attributeY = tileY / 4;
      int basePatternTable = (memory->PPUCTRL & 0x10) ? 0x1000 : 0x0000;
      for (int i = 0; i < 32; i++)
	{
	  int patternTableTile = memory->getNametableEntryForTile(i,tileY,nametable*256,nametable*240);
	  int patternTableIndex = patternTableTile*16;
	  int patternTablePlane1 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + lineInTile);
	  int patternTablePlane2 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex +  lineInTile + 8);
	  int xOffset = i*8;
	  int paletteIndex = attributeValueFromByteXY(memory->attributeEntryForXY(i,tileY,nametable*256,nametable*240),i,tileY);
	  for (int x = 0; x < 8; x++)
	    {
	      int andOperator = 1<<(7-x);
	      int colorIndex = (patternTablePlane1 & andOperator) + 2*(patternTablePlane2 & andOperator);
	      colorIndex = colorIndex >> (7-x);
	      
	      unsigned char paletteColorIndex = memory->colorForPaletteIndex(false, paletteIndex, colorIndex);
	      ALLEGRO_COLOR* paletteColors = getPaletteColors();
	      ALLEGRO_COLOR color = paletteColors[paletteColorIndex];
	      scanlinePoints[(xOffset+x)&0xFF].color=color;
	      scanlinePoints[(xOffset+x)&0xFF].x += nametable*256;
	    }
	}
      al_draw_prim(scanlinePoints,NULL,0,0,256,ALLEGRO_PRIM_POINT_LIST);
    }
}

void PpuState::endFrame()
{
  // Assume VBlank is starting
  memory->PPUSTATUS |= 0x80;
  al_set_target_backbuffer(display);
  al_flip_display();
  al_set_target_backbuffer(nametableDisplay);
  al_draw_line(memory->PPUSCROLLX,0,memory->PPUSCROLLX,256,al_map_rgb(255,0,0),3);
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

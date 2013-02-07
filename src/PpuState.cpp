#include "PpuState.h"
#include "PpuColors.h"
#include <iostream>
using namespace std;

bool PpuState::initializeDisplay(ALLEGRO_EVENT_QUEUE* event_queue)
{
  height = 224;
  width = 256;
  blackColor = al_map_rgb(0,0,0);
  cout << "Initializing display...";
  display = al_create_display(width*scale, height*scale);
  if (!display)
    {
      cout << "Error! Failed to initialize display.\n";
      return false;
    }
  al_register_event_source(event_queue, al_get_display_event_source(display));

#ifdef PPU_DEBUG
  al_set_new_window_position(513,0);
  nametableDisplay = al_create_display(2*width, height);
  if (!nametableDisplay)
    {
      cout << "Error! Failed to initialize ppu debug display.\n";
      return false;
      }
  al_set_window_title(nametableDisplay, "Nametables");
    
  al_set_new_window_position(0,0);
  paletteDisplay = al_create_display(512,64);
  if (!paletteDisplay)
    {
      cout << "Error! Failed to initialize palette display.\n";
      return false;
    }
  al_set_window_title(paletteDisplay, "Palettes");
#endif

  if (!al_init_primitives_addon())
    {
      cout << "Error! Failed to initialize primitives\n";
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
  if (memory->PPUMASK & 0x18)
    {
      memory->PPUADDR = memory->loopyT;  
      memory->PPUSTATUS &= 0xBF;
    }

  for (int i = 0; i < 256*scale*240; i++)
    {
      int scanline = i/(256*scale);
      framePoints[i].x = i % (256*scale);
      framePoints[i].y = (scanline)*scale;
      framePoints[i].z = 0;
      framePoints[i].color = blackColor;
    }
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

inline void PpuState::incrementX()
{
  if ((memory->PPUADDR & 0x001F) == 31)
    {
      memory->PPUADDR &= ~(0x001F);
      memory->PPUADDR ^= 0x0400;
    }
  else
    memory->PPUADDR += 1;
}

inline void PpuState::incrementY()
{
  if ((memory->PPUADDR & 0x7000) != 0x7000)
    memory->PPUADDR += 0x1000;
  else
    {
      memory->PPUADDR &= 0x0FFF;
      int y = (memory->PPUADDR & 0x03E0) >> 5;
      if (y == 29)
	{
	  y = 0;
	  memory->PPUADDR ^= 0x0800;
	}
      else if (y == 31)
	y = 0;
      else
	y++;
      memory->PPUADDR = (memory->PPUADDR & ~0x03E0) | (y << 5);
    }
}

inline void PpuState::renderBackground(int scanline)
{
  ALLEGRO_BITMAP* bitmap = al_get_backbuffer(display);
  al_set_target_bitmap(bitmap);
  if (memory->PPUMASK & 0x08) // If background enabled
    {
      int fineY = (memory->PPUADDR & 0x7000) >> 12;
      int basePatternTable = (memory->PPUCTRL & 0x10) ? 0x1000 : 0x0000;
      int upperLimit = (memory->loopyX) ? 33 : 32;
      for (int i = 0; i < upperLimit; i++)
	{
	  int coarseX = memory->PPUADDR & 0x1F;
	  int coarseY = (memory->PPUADDR & 0x03E0) >> 5;
	  int patternTableTile = memory->ppuReadByteFrom(0x2000 | (memory->PPUADDR & 0x0FFF));
	  int patternTableIndex = patternTableTile*16;
	  int patternTablePlane1 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + fineY);
	  int patternTablePlane2 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + fineY + 8);
	  int xOffset = i*8 - (memory->loopyX); // Account for both tile width and X scrolling
	  int attrAddr = 0x23C0 | (memory->PPUADDR & 0x0C00) | ((memory->PPUADDR >> 4) & 0x38) | ((memory->PPUADDR >> 2) & 0x07);
	  int paletteIndex = attributeValueFromByteXY(memory->ppuReadByteFrom(attrAddr),coarseX,coarseY);
	  for (int x = 0; x < 8; x++)
	    {
	      if (i == 0 && x == 0)
		x = (memory->loopyX);
	      if (!(memory->PPUMASK & 0x02) && ((xOffset+x)&0xFF) < 8)
		continue;
	      int andOperator = 1<<(7-x);
	      int colorIndex = (patternTablePlane1 & andOperator) + 2*(patternTablePlane2 & andOperator);
	      colorIndex = colorIndex >> (7-x);

	      unsigned char paletteColorIndex = memory->colorForPaletteIndex(false, paletteIndex, colorIndex);
	      ALLEGRO_COLOR* paletteColors = getPaletteColors();
	      ALLEGRO_COLOR color = paletteColors[paletteColorIndex];
	      if (xOffset + x > 0xFF)
		continue;
	      for (int j = 0; j < scale; j++)
		scanlinePoints[(xOffset+x)*scale+j].color=color;
	      if (colorIndex != 0)
		{
		  backgroundPoints[(xOffset+x)] = true;
		}
	    }
	  incrementX();
	}
      incrementY();
      memory->PPUADDR &= ~(0x041F);
      memory->PPUADDR |= (memory->loopyT & 0x041F);
    } // End if background enabled
}

inline void PpuState::renderSprites(int scanline)
{
  if (memory->PPUMASK & 0x10) // If sprites enabled
    {
      int spritesOnScanline = 0;
      memory->PPUSTATUS &= 0xDF; // Reset sprite overflow flag
      int spriteHeight = 8;
      if (memory->PPUCTRL & 0x20)
	spriteHeight = 16;
      int basePatternTable = (memory->PPUCTRL & 0x08) ? 0x1000 : 0x0000;
      for (int i = 0; i < 64; i++)
	{
	  // Check if sprite is visible on this scanline
	  int yCoord = memory->oamReadByteFrom(i*4);
	  if ((yCoord > scanline || scanline-yCoord >= spriteHeight) || yCoord > 0xEF)
	    continue;
	  if (++spritesOnScanline > 8)  // Check for sprite overflow
	    {
	      memory->PPUSTATUS |= 0x20;
	      break;
	    }
	  int spriteFlags = memory->oamReadByteFrom(i*4+2);
	  
	  int spriteLine = scanline-yCoord;
	  if (spriteFlags & 0x80) // Check for vertical flip
	    spriteLine = spriteHeight-spriteLine-1;
	  int xOffset = memory->oamReadByteFrom(i*4+3);
	  int paletteIndex = spriteFlags & 0x3;

	  int patternTablePlane1, patternTablePlane2;
	  int patternTableTile = memory->oamReadByteFrom(i*4+1);
	  
	  if (spriteHeight == 8)
	    {
	      int patternTableIndex = patternTableTile*16;
	      patternTablePlane1 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + spriteLine);
	      patternTablePlane2 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + spriteLine + 8);
	    }
	  else if (spriteHeight == 16)
	    {
	      if (patternTableTile & 0x01)
		patternTableTile |= 0x100;
	      patternTableTile &= 0x1FE;
	      int patternTableIndex = patternTableTile*16;
	      if (spriteLine >= 8)
		patternTableIndex += 8;
	      patternTablePlane1 = memory->ppuReadByteFrom(patternTableIndex + spriteLine);
	      patternTablePlane2 = memory->ppuReadByteFrom(patternTableIndex + spriteLine + 8);
	    }
	  else
	    { cout << "Error! Unknown sprite height: " << spriteHeight << "\n"; }
	  
	  for (int x = 0; x < 8; x++)
	    {
	      // Check if this is hidden by PPUMASK
	      if (!(memory->PPUMASK & 0x02) && (xOffset+x) < 8)
		continue;
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
		  if (xOffset + x > 0xFF)
		    continue;
		  if (!((spriteFlags & 0x20) && backgroundPoints[xOffset+x]))
		    {
		      for (int j = 0; j < scale; j++)
			scanlinePoints[(xOffset+x)*scale+j].color=color;
		    }
		  if (backgroundPoints[xOffset+x] && i == 0)
		    {
		      memory->PPUSTATUS |= 0x40;
#ifdef PPU_WRITE_LOG
		      cout << "Sprite #0 Hit!\n";
#endif
		    }
		}
	    }
        }
    } // End if sprites enabled
}

void PpuState::renderScanline(int scanline)
{ 
  if (scanline >= 232)
    return;
  for (int i = 0; i < 256; i++)
      backgroundPoints[i] = false;
  scanlinePoints = framePoints + (scanline)*256*scale;
  scanline += vScroll & 0x07;
  
  renderBackground(scanline);
  renderSprites(scanline);

  if (memory->PPUMASK & 0x18)
    {
      memory->PPUADDR &= ~(0x041F);
      memory->PPUADDR |= (memory->loopyT & 0x041F);
      memory->scanlineCounter();
    }

#ifdef PPU_DEBUG
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
	  int patternTablePlane2 = memory->ppuReadByteFrom(basePatternTable + patternTableIndex + lineInTile + 8);
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
#endif
}

void PpuState::endFrame()
{
  al_set_target_backbuffer(display);

  // Make sure that we are not drawing to a null bitmap  
  // Don't know why this is a problem, but it seems to be
  if (al_get_target_bitmap())
    {
      for (int j = 0; j < scale; j++)
	{
	  al_draw_prim(framePoints, NULL, 0, 0, 256*scale*240, ALLEGRO_PRIM_POINT_LIST);
	  for (int point = 0; point < 256*scale*240; point++)
	    framePoints[point].y++;
	}
    }
  
  al_flip_display();

#ifdef PPU_DEBUG
  al_set_target_backbuffer(nametableDisplay);
  al_draw_line(memory->PPUSCROLLX,0,memory->PPUSCROLLX,256,al_map_rgb(255,0,0),3);
  al_flip_display();

  // Draw palettes
  al_set_target_backbuffer(paletteDisplay);
  for (int y = 0; y < 2; y++)
    {
      for (int x = 0; x < 16; x++)
	{
	  ALLEGRO_COLOR color = getPaletteColors()[memory->colorForPaletteIndex(y,x>>2,x&0x3)];
	  al_draw_filled_rectangle(x*32,y*32,x*32+32,y*32+32,color);
	}
    }
    al_flip_display();
#endif
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

char* PpuState::stateData(size_t* size)
{
  *size = sizeof(int);
  char* buffer = (char*)malloc(sizeof(char)*(*size));
  memcpy(buffer, &cycles, *size);
  return buffer;
}

void PpuState::loadState(char* buffer, size_t size)
{
  memcpy(&cycles, buffer, size);
}

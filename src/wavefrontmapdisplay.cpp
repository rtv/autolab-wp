/***************************************************************************
 *   Copyright (C) 2009 by Jens
 *   jwawerla@sfu.ca
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 * $Log: wavefrontmapdisplay.cpp,v $
 * Revision 1.5  2009-03-24 01:04:47  jwawerla
 * switching cost robot added
 *
 * Revision 1.4  2009-03-21 02:31:50  jwawerla
 * *** empty log message ***
 *
 * Revision 1.3  2009-03-20 03:05:23  jwawerla
 * dynamic obstacles added to wavefront
 *
 * Revision 1.2  2009-03-16 14:27:19  jwawerla
 * robots still get stuck
 *
 * Revision 1.1.1.1  2009-03-15 03:52:02  jwawerla
 * First commit
 *
 *
 ***************************************************************************/
#include "wavefrontmapdisplay.h"
#include "error.h"



//-----------------------------------------------------------------------------
CWaveFrontMapDisplay::CWaveFrontMapDisplay ( CWaveFrontMap* map, const char* title )
{
  mMap = map;

  if ( SDL_Init ( SDL_INIT_AUDIO | SDL_INIT_VIDEO ) < 0 ) {
    PRT_ERR1 ( "Unable to initialize SDL: %s", SDL_GetError() );
    exit ( 1 );
  }
  atexit(SDL_Quit);

  mSdlScreen = SDL_SetVideoMode ( mMap->getNumCellsX(),
                                  mMap->getNumCellsY(), 16, SDL_SWSURFACE );
  if ( mSdlScreen == NULL ) {
    PRT_ERR1 ( "Error while opening SDL window: %s", SDL_GetError() );
  }

  if ( title == NULL )
    strncpy ( mWindowTitle, "Wave front map", 25 );
  else
    strncpy ( mWindowTitle, title, 25 );
}
//-----------------------------------------------------------------------------
CWaveFrontMapDisplay::~CWaveFrontMapDisplay()
{
}
//-----------------------------------------------------------------------------
void CWaveFrontMapDisplay::update()
{
  std::list<CMapMarker>::iterator it;
  int maxY;
  char value;
  float normalizer;
  int x;
  int y;
  int centerX;
  int centerY;
  int height;
  int NX, NY;

  SDL_WM_SetCaption ( mWindowTitle, "" );

  //clearScreen();

  normalizer = mMap->getMaxCellValue() / 255;
  height = mSdlScreen->h - 1;
  centerX = mSdlScreen->w / 2;
  centerY = mSdlScreen->h / 2;

  if ( SDL_MUSTLOCK ( mSdlScreen ) ) {
    if ( SDL_LockSurface ( mSdlScreen ) < 0 ) {
      return;
    }
  }

  // draw gradient
  maxY = mMap->getNumCellsY();
  NX = mMap->getNumCellsX();
  NY = mMap->getNumCellsY();
  for ( x = 0; x < NX; x ++ ) {
    for ( y = 0; y < NY; y ++ ) {
      value =  (char)floor ( mMap->mMapData[x][maxY-y] / normalizer );
      if (mMap->mSensorMap[x][maxY-y] - mMap->mSensorMapTimeOffset > 0.5) {
        value = 0;
      }
      setPixel ( x, y, value, value, value );
    }
  }

  // draw markers
  for ( it = mMap->mMapMarkerList.begin(); it != mMap->mMapMarkerList.end(); it++ ) {
    x = ( int ) round ( ( it->getLocation().x ) / mMap->getCellSize() );
    y = ( int ) round ( ( it->getLocation().y ) / mMap->getCellSize() );
    setPixel ( centerX + x, height - ( centerY + y ), it->getColor().mRed,
               it->getColor().mGreen, it->getColor().mBlue );
  }

  if ( SDL_MUSTLOCK ( mSdlScreen ) ) {
    SDL_UnlockSurface ( mSdlScreen );
  }
  // update entire screen
  SDL_UpdateRect ( mSdlScreen, 0, 0, 0, 0 );

}
//-----------------------------------------------------------------------------
void CWaveFrontMapDisplay::clearScreen()
{
  if ( SDL_MUSTLOCK ( mSdlScreen ) ) {
    if ( SDL_LockSurface ( mSdlScreen ) < 0 ) {
      return;
    }
  }

  // draw everything black
  for (int x = 0; x < mMap->getNumCellsX(); x ++ ) {
    for (int y = 0; y < mMap->getNumCellsY(); y ++ ) {
      setPixel ( x, y, 0, 0, 0 );
    }
  }

  if ( SDL_MUSTLOCK ( mSdlScreen ) ) {
    SDL_UnlockSurface ( mSdlScreen );
  }
  // update entire screen
  SDL_UpdateRect ( mSdlScreen, 0, 0, 0, 0 );
}
//-----------------------------------------------------------------------------
void CWaveFrontMapDisplay::setPixel ( int x, int y, char red, char green, char blue )
{
  int bpp;
  Uint32 color;
  color = SDL_MapRGB ( mSdlScreen->format, red, green, blue );

  bpp = mSdlScreen->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = ( Uint8 * ) mSdlScreen->pixels + y * mSdlScreen->pitch + x * bpp;

  switch ( bpp ) {
    case 1:
      *p = color;
      break;

    case 2:
      * ( Uint16 * ) p = color;
      break;

    case 3:
      if ( SDL_BYTEORDER == SDL_BIG_ENDIAN ) {
        p[0] = ( color >> 16 ) & 0xff;
        p[1] = ( color >> 8 ) & 0xff;
        p[2] = color & 0xff;
      } else {
        p[0] = color & 0xff;
        p[1] = ( color >> 8 ) & 0xff;
        p[2] = ( color >> 16 ) & 0xff;
      }
      break;

    case 4:
      * ( Uint32 * ) p = color;
      break;
  }
}
//-----------------------------------------------------------------------------



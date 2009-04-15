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
 * $Log: wavefrontmapdisplay.h,v $
 * Revision 1.1.1.1  2009-03-15 03:52:02  jwawerla
 * First commit
 *
 *
 **************************************************************************/
#ifndef WAVEFRONTMAPDISPLAY_H
#define WAVEFRONTMAPDISPLAY_H

#include "wavefrontmap.h"
#include <SDL.h>

/**
*/
class CWaveFrontMapDisplay{
  public:
    /**
     * Default constructor
     * @param map to be displayed
     * @param title of window
     */
    CWaveFrontMapDisplay(CWaveFrontMap* map, const char* title = NULL);
    /** Default destructor */
    ~CWaveFrontMapDisplay();
    /** Updates the display */
    void update();

  protected:
    /**
     * Sets a pixel
     * @param x
     * @param y
     * @param red
     * @param green
     * @param blue
     */
    inline void setPixel(int x, int y, char red, char green, char blue  );
    /** Clears the screen by setting all pixel to black */
    void clearScreen();

  private:
    /** Wave front map */
    CWaveFrontMap* mMap;
    /** SDL screen */
    SDL_Surface* mSdlScreen;
    /** Window title */
    char mWindowTitle[25];
};

#endif

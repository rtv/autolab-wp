/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: mapmarker.h,v 1.1.1.1 2009-03-15 03:52:03 jwawerla Exp $
 ***************************************************************************
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
 * $Log: mapmarker.h,v $
 * Revision 1.1.1.1  2009-03-15 03:52:03  jwawerla
 * First commit
 *
 * Revision 1.1.1.1  2008/02/02 22:19:54  jwawerla
 * new to cvs
 *
 * Revision 1.1.1.1  2008/01/10 19:43:58  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#ifndef CMAPMARKER_H
#define CMAPMARKER_H

#include "ccolor.h"
#include "common.h"
#include <cstring>


/**
 * Marks a location on a map
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class CMapMarker
{

  public:
    /**
     * Default constructor, constructs a marker at (x,y)
     * @param color of marker in map visualization
     * @param text of the marker (max 30 char)
     * @param x [m]
     * @param y [m]
     */
    CMapMarker(CColor color, char* text, float x=0, float y=0);
    /**
     * Default constructor, constructs a marker at (x,y)
     * @param color of marker in map visualization
     * @param text of the marker (max 30 char)
     * @param point to mark
     */
    CMapMarker(CColor color, char* text, tPoint2d point);
    /** Default destructor */
    ~CMapMarker();
    /**
     * Gets the location of the marker
     * @return location
     */
    tPoint2d getLocation() {return mLocation; };
    /**
     * Sets the location of the marker
     * @param loc location to be set
     */
    void setLocation(tPoint2d loc);
    /**
     * Gets the color of the marker
     * @return color
     */
    CColor getColor() { return mColor; };
    /**
     * Gets the text of the marker
     * @return text
     */
    char* getText() { return mText; };

  private:
    /** Location of the marker [m][m] */
    tPoint2d mLocation;
    /** Color of the marker */
    CColor mColor;
    /** Text of the marker */
    char mText[30];


};

#endif

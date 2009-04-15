/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: ccolor.h,v 1.1.1.1 2009-03-15 03:52:02 jwawerla Exp $
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
 * $Log: ccolor.h,v $
 * Revision 1.1.1.1  2009-03-15 03:52:02  jwawerla
 * First commit
 *
 * Revision 1.1.1.1  2008/02/02 22:19:54  jwawerla
 * new to cvs
 *
 * Revision 1.1.1.1  2008/01/10 19:44:02  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#ifndef CCOLOR_H
#define CCOLOR_H

#include "utilities.h"

/**
 * Defines a rgb color
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 12/2007
 */

class CColor
{
  public:
    /**
     * Default constructor
     * @param r red
     * @param g green
     * @param b blue
     */
    CColor( int r=0, int g=0, int b=0 );
    /**
     * Copy constructor 
     */
    CColor(CColor const &color);
    /** Default destructor */
    ~CColor();
    /**
     * Assignment operator
     */
    void operator = (CColor const &color);
    /** Red value */
    int mRed;
    /** Green value */
    int mGreen;
    /** Blue value */
    int mBlue;

};

#endif

/***************************************************************************
 * Project: autolab-wp                                                     *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: mapmarker.cpp,v 1.1.1.1 2009-03-15 03:52:02 jwawerla Exp $
 ***************************************************************************
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
 * $Log: $
 *
 *
 ***************************************************************************/
#include "mapmarker.h"
//---------------------------------------------------------------------------
CMapMarker::CMapMarker(CRgbColor color,  char* text, float x, float y)
{
  mLocation.mX = x;
  mLocation.mY = y;
  mColor = color;
  strncpy(mText, text, 30);
}
//---------------------------------------------------------------------------
CMapMarker::CMapMarker(CRgbColor color,  char* text, Rapi::CPoint2d point)
{
  mLocation.mX = point.mX;
  mLocation.mY = point.mY;
  mColor = color;
  strncpy(mText, text, 30);
}
//---------------------------------------------------------------------------
CMapMarker::~CMapMarker()
{
}
//---------------------------------------------------------------------------
void CMapMarker::setLocation(Rapi::CPoint2d loc)
{
  mLocation = loc;
}
//---------------------------------------------------------------------------



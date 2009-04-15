/***************************************************************************
 * Project: RAPI                                                           *
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
 * $Log: mapmarker.cpp,v $
 * Revision 1.1.1.1  2009-03-15 03:52:02  jwawerla
 * First commit
 *
 * Revision 1.1.1.1  2008/02/02 22:19:53  jwawerla
 * new to cvs
 *
 * Revision 1.1.1.1  2008/01/10 19:43:57  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#include "mapmarker.h"
//---------------------------------------------------------------------------
CMapMarker::CMapMarker(CColor color,  char* text, float x, float y)
{
  mLocation.x = x;
  mLocation.y = y;
  mColor = color;
  strncpy(mText, text, 30);
}
//---------------------------------------------------------------------------
CMapMarker::CMapMarker(CColor color,  char* text, tPoint2d point)
{
  mLocation.x = point.x;
  mLocation.y = point.y;
  mColor = color;
  strncpy(mText, text, 30);
}
//---------------------------------------------------------------------------
CMapMarker::~CMapMarker()
{
}
//---------------------------------------------------------------------------
void CMapMarker::setLocation(tPoint2d loc)
{
  mLocation = loc;
}
//---------------------------------------------------------------------------



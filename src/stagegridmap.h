/***************************************************************************
 * Project: autolab-wp                                                     *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
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
 * $Log:  $
 *
 *
 ***************************************************************************/
#ifndef STAGEGRIDMAP_H
#define STAGEGRIDMAP_H

#include "gridmap.h"
#include "stage.hh"
#include "wavefrontmapvis.h"

/** Instance of the map visualization tool for stage */
static MapVis stgMapVis;

/**
 * This class in an extention to CGridMap in that it provides an
 * interface to Stage. It is able to load a map directly from Stage using
 * the rasterizer.
 * @author Jens Wawerla
 */
class CStageGridMap : public CGridMap
{
  public:
    /**
     * Constructs a map from a Stage model grid (0=empty, >0=occupied).
     * Map cell values are set to 0.0=empty, 1.0=occupied.
     * @param model Stage model to get map data from
     */
    CStageGridMap(Stg::Model* model);
    /** Default destructor */
    virtual ~CStageGridMap();

};

#endif

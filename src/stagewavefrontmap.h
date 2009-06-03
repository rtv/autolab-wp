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
#ifndef STAGEWAVEFRONTMAP_H
#define STAGEWAVEFRONTMAP_H

#include "wavefrontmap.h"
#include "stagegridmap.h"
#include "wavefrontmapvis.h"

/** Instance of the map visualization tool for stage */
static CWaveFrontMapVis stageWaveFrontMapVis;

/**
 * A Stage version of the wavefront map. This class is directly derived from
 * CWaveFrontMap and just extends it by registering a visualizer with Stage
 * @author Jens Wawerla
*/
class CStageWaveFrontMap : public CWaveFrontMap
{
public:
    /**
     * Default constructor
     * @param obstacleMap map with obstacle to use for wave front
     * @param name of map
     */
    CStageWaveFrontMap(CStageGridMap* obstacleMap, std::string name="Wavefront" );
     /** Default destructor */
    ~CStageWaveFrontMap();

};

#endif

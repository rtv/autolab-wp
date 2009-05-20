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
#include "stagegridmap.h"

//-----------------------------------------------------------------------------
CStageGridMap::CStageGridMap ( Stg::Model* stgModel )
    : CGridMap()
{
  float wavefrontCellSize;
  char* mapName = NULL;
  Stg::Model* mapModel;
  Stg::Geom geom;
  int width;
  int height;

  assert ( stgModel );

  if ( ! stgModel->GetPropertyStr ( "mapmodel", &mapName,  NULL ) ) {
    PRT_ERR1 ( "Stage model %s has no mapmodel string specified in worldfile.",
               stgModel->Token() );
    exit ( -1 );
  }

  mapModel = stgModel->GetWorld()->GetModel ( mapName );
  if ( ! mapModel ) {
    PRT_ERR1 ( "worldfile has no model named \"%s\"", mapName );
    exit ( -1 );
  }

  if ( ! stgModel->GetPropertyFloat ( "wavefrontcellsize", &wavefrontCellSize,
    0.1 ) )
    PRT_WARN2 ( "robot %s has no parameter wavefrontcellsize specified in "\
                "worldfile. Using %.2f meters",
                stgModel->Token(), wavefrontCellSize );

  // get an occupancy grid from the Stage model
  geom = mapModel->GetGeom();
  width = ( int ) ( geom.size.x / wavefrontCellSize );
  height = ( int ) ( geom.size.y / wavefrontCellSize );

  // make sure we have an odd numer of cells
  if ( ( width % 2 ) == 0 )
    width++;

  if ( ( height % 2 ) == 0 )
    height++;

  uint8_t* cells = new uint8_t[ width * height ];

  // pre-set cells with 0, Stage only sets obstacles but not free areas
  bzero(cells, width * height);
  mapModel->Rasterize ( cells,
                        width, height,
                        wavefrontCellSize, wavefrontCellSize );

  createMap ( cells, width, height, wavefrontCellSize );
  delete[] cells;

}
//-----------------------------------------------------------------------------
CStageGridMap::~CStageGridMap()
{
}
//-----------------------------------------------------------------------------




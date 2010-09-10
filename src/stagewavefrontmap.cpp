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
#include "stagewavefrontmap.h"
#include <stage.hh>
#include "wavefrontmapvis.h"

//-----------------------------------------------------------------------------
CStageWaveFrontMap::CStageWaveFrontMap ( CStageGridMap* obstacleMap,
    std::string name )
    : CWaveFrontMap ( obstacleMap, name )
{
  Stg::Model* stgModel;
  Stg::Model* bg;

  stgModel = obstacleMap->getStageModel();
  assert(stgModel);
  std::string wavefront_map_key = std::string( "wavefront_map" );
  stgModel->SetProperty( wavefront_map_key, static_cast<void*>( this ) );

  bg = stgModel->GetWorld()->GetModel( "background" );
  assert( bg );
  bg->AddVisualizer( &stageWaveFrontMapVis, false );
}
//-----------------------------------------------------------------------------
CStageWaveFrontMap::~CStageWaveFrontMap()
{
  // nothing to do
}
//-----------------------------------------------------------------------------


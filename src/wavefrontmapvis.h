/***************************************************************************
 *   Copyright (C) 2009 by Richard Vaughan
 *   vaughan@sfu.ca
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
 * $Log: wavefrontmapvis.h,v $
 * Revision 1.10  2009-04-11 05:27:53  vaughan
 * re-enabled forcing off odd-dimensioned map grids
 *
 * Revision 1.9  2009-04-11 00:39:21  vaughan
 * fixed rasterizer scaling bug
 *
 * Revision 1.8  2009-04-09 22:49:49  vaughan
 * fixed dodgy void* handling
 *
 * Revision 1.7  2009-04-09 00:37:05  vaughan
 * debugging
 *
 * Revision 1.6  2009-04-08 22:40:41  jwawerla
 * Hopefully ND interface issue solved
 *
 * Revision 1.5  2009-04-08 00:07:07  vaughan
 * working for RTV
 *
 * Revision 1.4  2009-04-05 01:16:51  jwawerla
 * still debuging wavefront map
 *
 * Revision 1.3  2009-03-31 23:52:59  jwawerla
 * Moved cell index from float to int math
 *
 * Revision 1.2  2009-03-31 04:27:33  jwawerla
 * Some bug fixing
 *
 * Revision 1.1  2009-03-28 00:29:22  vaughan
 * added missing file
 *
 ***************************************************************************/

#include "wavefrontmap.h"
#include "stage.hh"
#include <iostream>

/**
 * A Stage visualizer for a wavefront map
 * @author Richard Vaughan
 */
class MapVis : public Stg::Visualizer
{
  private:
    Stg::Model* current_mod;
    CWaveFrontMap* map;

  public:
    MapVis() :
        Stg::Visualizer ( "Wavefront planner", "show_wavefront" ),
        current_mod ( NULL ),
        map ( NULL ) {}

    virtual ~MapVis() {}

    virtual void Visualize ( Stg::Model* displaymod, Stg::Camera* cam ) {
      // has the currently selected robot changed?
      Stg::Model* now = displaymod->GetWorld()->RecentlySelectedModel();

      // if the model changed
      if ( now != current_mod ) {
        current_mod = now;
        // fetch the map out of the model
        // map = (CWaveFrontMap*) current_mod->GetProperty ( "wavefront_map" );
        map = static_cast<CWaveFrontMap*> ( current_mod->GetProperty ( "wavefront_map" ) );
      }

      if ( map == NULL )
        return;

      Stg::Geom geom = displaymod->GetGeom();

      int NX = map->getNumCellsX();
      int NY = map->getNumCellsY();

      double bg_width = geom.size.x;
      double bg_height = geom.size.y;

      double scalex = map->mObstacleMap->getCellSize();//bg_width / (double)NX;
      double scaley = scalex;////bg_height / (double)NY;

      glPushMatrix();
      glTranslatef ( -bg_width / 2.0, -bg_height / 2.0, 0.01 );
      glScalef ( scalex, scaley, 0 );

      float normalizer = map->getMaxCellValue();

      for ( int x = 0; x < NX; x++ )
        for ( int y = 0; y < NY; y++ ) {
          // all obstacle data
          //float mapval = 1.0 - MIN ( map->mSOGMap[x][y].sog, 1 );
          //float mapval = 1.0 - MIN ( map->mObstacleMap->mMapData[x][y], 1 );
          //displaymod->PushColor ( mapval, mapval, mapval, 0.8 );
          //glRectf ( x, y, x + 1, y + 1 );
          //displaymod->PopColor();

          displaymod->PushColor ( 0,0,0,0.5 );
          displaymod->PopColor();


          float mapval = 1.0 - MIN ( map->mMapData[x][y] / normalizer, 1 );
          if ( mapval > 0.00 ) {
            displaymod->PushColor ( mapval, mapval, mapval, 0.8 );
            glRectf ( x, y, x + 1, y + 1 );
            displaymod->PopColor();
          }

          // dynamic data
          float obstacleval = MAX ( 0, map->mSensorMap[x][y] - map->mSensorMapTimeOffset );
          if ( obstacleval > 0.00 ) {
            displaymod->PushColor ( 0, obstacleval, 0, 1.0 );
            glRectf ( x, y, x + 1, y + 1 );
            displaymod->PopColor();
          }
        }


#if 0
      // outline all cells in grid (debug)
      displaymod->PushColor ( 0,0,0,1 );
      glBegin ( GL_LINES );
      for ( int x = 0; x < NX; x++ ) {
        glVertex2f ( x, 0 );
        glVertex2f ( x, NY );
      }
      for ( int y = 0; y < NY; y++ ) {
        glVertex2f ( 0, y );
        glVertex2f ( NX, y );
      }
      glEnd();
      displaymod->PopColor();
#endif

      glPopMatrix();
    }
};



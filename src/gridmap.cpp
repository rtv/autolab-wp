/***************************************************************************
 * Project: autolab-wp                                                     *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: gridmap.cpp,v 1.8 2009-04-09 17:03:37 vaughan Exp $
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
 * $Log: gridmap.cpp,v $
 * Revision 1.8  2009-04-09 17:03:37  vaughan
 * removed QT and SDL dependencies. Added install target for libraries
 *
 * Revision 1.7  2009-04-08 22:40:41  jwawerla
 * Hopefully ND interface issue solved
 *
 * Revision 1.6  2009-04-05 01:16:51  jwawerla
 * still debuging wavefront map
 *
 * Revision 1.5  2009-04-04 00:20:18  vaughan
 * get map from Stage model
 *
 * Revision 1.4  2009-04-03 16:21:30  jwawerla
 * Move wavefront map specification to world file
 *
 * Revision 1.3  2009-03-31 23:52:59  jwawerla
 * Moved cell index from float to int math
 *
 * Revision 1.2  2009-03-20 03:05:23  jwawerla
 * dynamic obstacles added to wavefront
 *
 * Revision 1.1.1.1  2009-03-15 03:52:02  jwawerla
 * First commit
 *
 * Revision 1.1.1.1  2008/02/02 22:19:54  jwawerla
 * new to cvs
 *
 * Revision 1.4  2008/01/17 23:41:10  jwawerla
 * fixed some bugs
 *
 * Revision 1.3  2008/01/12 01:16:59  jwawerla
 * working on wavefront waypoint list
 *
 * Revision 1.2  2008/01/11 02:05:16  jwawerla
 * Added local coordinate system
 *
 * Revision 1.1.1.1  2008/01/10 19:44:01  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#include "gridmap.h"

//---------------------------------------------------------------------------
CGridMap::CGridMap ( int numCellsX, int numCellsY, float cellSize )
{
  mNumCellsX = numCellsX;
  mNumCellsY = numCellsY;
  mCenterCellX = ( mNumCellsX - 1 ) / 2;
  mCenterCellY = ( mNumCellsY - 1 ) / 2;
  mCellSize = cellSize;

  mMapData = new float*[mNumCellsX];

  for ( int x = 0; x < mNumCellsX; x++ ) {
    mMapData[x] = new float[mNumCellsY];
  }

  preSetMap ( 0 );

  mMinCellValue = 0.0f;
  mMaxCellValue = 1.0f;
}

//---------------------------------------------------------------------------
CGridMap::CGridMap ( uint8_t* data, int numCellsX, int numCellsY,
                     float cellSize )
{
  createMap ( data, numCellsX, numCellsY, cellSize );
}
//---------------------------------------------------------------------------
CGridMap::CGridMap()
{
  // empty constructor for derived classes only
}
//---------------------------------------------------------------------------
/*
CGridMap::CGridMap ( Stg::Model* model, float cellSize )
{
  Stg::Geom geom;
  float value;
  float cellsPerMeter = 1.0/cellSize;
  int i, j;
  int stageNumCellsX;
  int stageNumCellsY;
  uint8_t* cells;

  // define cells
  mCellSize = cellSize;
  mMinCellValue = 0.0;
  mMaxCellValue = 1.0;

  // get data from stage
  geom = model->GetGeom();
  stageNumCellsX = ( int )ceil( geom.size.x * cellsPerMeter );
  stageNumCellsY = ( int )ceil( geom.size.y * cellsPerMeter );
  cells = new uint8_t[ stageNumCellsX * stageNumCellsY ];
  model->Rasterize ( cells, stageNumCellsX, stageNumCellsY );
  // make sure we have an odd number of cells in each dimension
  mNumCellsX = ( int ) ceil ( stageNumCellsX / cellsPerMeter / mCellSize );
  mNumCellsY = ( int ) ceil ( stageNumCellsY / cellsPerMeter / mCellSize );

  if ( mNumCellsX % 2 == 0 )
    mNumCellsX++;
  if ( mNumCellsY % 2 == 0 )
    mNumCellsY++;

  // find center cell
  mCenterCellX = ( mNumCellsX - 1 ) / 2;
  mCenterCellY = ( mNumCellsY - 1 ) / 2;

  // create map data structure
  mMapData = new float*[mNumCellsX];
  for ( int x = 0; x < mNumCellsX; x++ ) {
    mMapData[x] = new float[mNumCellsY];
  }

  preSetMap ( mMinCellValue );

  // fill map data structure
  for ( int x = 0; x < stageNumCellsX; x++ )
    for ( int y = 0; y < stageNumCellsY; y++ ) {
      i = ( int ) round ( x / cellsPerMeter / mCellSize );
      j = ( int ) round ( y / cellsPerMeter / mCellSize );
      //value = ( cells[ x + y*stageNumCellsX ] == 0 ) ? 0.0 : 1.0;
      value = cells[ x + y*stageNumCellsX ];
      mMapData[i][j] = MAX ( mMapData[i][j], value );
      if ( (x < 15)  && (y < 25) ) {
        //printf(" x %d y %d %f\n", x, y, value);
      }
    }
}
*/
//---------------------------------------------------------------------------
CGridMap::~CGridMap()
{
  for ( int x = 0; x < mNumCellsX; x++ ) {
    delete[] mMapData[x];
  }
}
//---------------------------------------------------------------------------
void CGridMap::createMap ( uint8_t* data, int numCellsX, int numCellsY,
                           float cellSize )
{
  mNumCellsX = numCellsX;
  mNumCellsY = numCellsY;
  mCenterCellX = ( mNumCellsX - 1 ) / 2;
  mCenterCellY = ( mNumCellsY - 1 ) / 2;
  mCellSize = cellSize;

  mMapData = new float*[mNumCellsX];

  for ( int x = 0; x < mNumCellsX; x++ ) {
    mMapData[x] = new float[mNumCellsY];
  }

  // initialize the grid
  for ( int x = 0; x < mNumCellsX; x++ )
    for ( int y = 0; y < mNumCellsY; y++ )
      mMapData[x][y] = ( data[ x + y*numCellsX ] == 0 ) ? 0.0 : 1.0;

  mMinCellValue = 0.0;
  mMaxCellValue = 1.0;
}
//---------------------------------------------------------------------------
float CGridMap::getMapHeight()
{
  return ( mNumCellsY -1 ) * mCellSize;
}
//---------------------------------------------------------------------------
float CGridMap::getMapWidth()
{
  return ( mNumCellsX -1 ) * mCellSize;
}
//---------------------------------------------------------------------------
float CGridMap::getCellValue ( CPose2d pose )
{
  CPoint2d pos;

  pos.mX = pose.mX;
  pos.mY = pose.mY;
  return getCellValue ( pos );
}
//---------------------------------------------------------------------------
float CGridMap::getCellValue ( CPoint2d pos )
{
  int x, y;

  x = mCenterCellX + ( int ) round ( pos.mX / mCellSize );
  y = mCenterCellY + ( int ) round ( pos.mY / mCellSize );

  if ( ( x > 0 ) && ( x < mNumCellsX ) && ( y > 0 ) && ( y < mNumCellsY ) )
    return mMapData[x][y];

  PRT_ERR2 ( "Location outside of map %f %f \n", pos.mX, pos.mY );

  return 0;
}
//---------------------------------------------------------------------------
void CGridMap::setRobotPosition ( CPoint2d robotPos )
{
  mRobotCell.mX = mCenterCellX + ( int ) round ( robotPos.mX / mCellSize );
  mRobotCell.mY = mCenterCellY + ( int ) round ( robotPos.mY / mCellSize );
}
//---------------------------------------------------------------------------
void CGridMap::preSetMap ( float value )
{
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      mMapData[x][y] = value;
    }
  }
}
//-----------------------------------------------------------------------------
void CGridMap::preSetMap ( float value, CPose2d center, float halfBoxLenght )
{
  int minX, minY, maxX, maxY;

  minX = ( int ) floor ( ( center.mX - halfBoxLenght ) / mCellSize );
  maxX = ( int ) ceil ( ( center.mX + halfBoxLenght ) / mCellSize );
  minY = ( int ) floor ( ( center.mY - halfBoxLenght ) / mCellSize );
  maxY = ( int ) ceil ( ( center.mY + halfBoxLenght ) / mCellSize );

  minX = LIMIT ( minX, 0, mNumCellsX - 1 );
  maxX = LIMIT ( maxX, 0, mNumCellsX - 1 );
  minX = LIMIT ( minY, 0, mNumCellsY - 1 );
  maxY = LIMIT ( maxY, 0, mNumCellsY - 1 );

  for ( int x = minX; x <= maxX; x++ ) {
    for ( int y = minY; y <= maxY; y++ ) {
      mMapData[x][y] = value;
    }
  }
}
//-----------------------------------------------------------------------------
CPoint2d CGridMap::centerToLocal ( CPoint2d gPos )
{
  CPoint2d lPos2;

  lPos2.mX = gPos.mX  + ( mNumCellsX * 0.5 * mCellSize );
  lPos2.mY = gPos.mY + ( mNumCellsY * 0.5 * mCellSize );

  return lPos2;
}
//---------------------------------------------------------------------------
float CGridMap::findMaxCellValue()
{
  float m = -INFINITY;

  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      m = MAX ( m, mMapData[x][y] );
    }
  }
  return m;
}
//---------------------------------------------------------------------------





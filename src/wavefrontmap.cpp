/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: wavefrontmap.cpp,v 1.18 2009-04-09 00:37:05 vaughan Exp $
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
 * $Log: wavefrontmap.cpp,v $
 * Revision 1.18  2009-04-09 00:37:05  vaughan
 * debugging
 *
 * Revision 1.17  2009-04-08 22:40:41  jwawerla
 * Hopefully ND interface issue solved
 *
 * Revision 1.16  2009-04-04 01:21:38  vaughan
 * working on large-scale demo
 *
 * Revision 1.15  2009-04-03 16:21:30  jwawerla
 * Move wavefront map specification to world file
 *
 * Revision 1.14  2009-04-03 15:10:03  jwawerla
 * *** empty log message ***
 *
 * Revision 1.13  2009-03-31 23:52:59  jwawerla
 * Moved cell index from float to int math
 *
 * Revision 1.12  2009-03-31 04:27:33  jwawerla
 * Some bug fixing
 *
 * Revision 1.11  2009-03-31 01:42:01  jwawerla
 * Task definitions moved to task manager and stage world file
 *
 * Revision 1.10  2009-03-29 00:54:27  jwawerla
 * Replan ctrl seems to work now
 *
 * Revision 1.9  2009-03-28 00:54:39  jwawerla
 * *** empty log message ***
 *
 * Revision 1.8  2009-03-27 00:27:37  jwawerla
 * *** empty log message ***
 *
 * Revision 1.7  2009-03-24 03:59:54  jwawerla
 * Pause button works now
 *
 * Revision 1.6  2009-03-24 01:04:46  jwawerla
 * switching cost robot added
 *
 * Revision 1.5  2009-03-23 06:24:21  jwawerla
 * RobotVis works now for data, maps still need more work
 *
 * Revision 1.4  2009-03-21 02:31:50  jwawerla
 * *** empty log message ***
 *
 * Revision 1.3  2009-03-20 03:05:23  jwawerla
 * dynamic obstacles added to wavefront
 *
 * Revision 1.2  2009-03-17 03:11:30  jwawerla
 * Another day no glory
 *
 * Revision 1.1.1.1  2009-03-15 03:52:02  jwawerla
 * First commit
 *
 * Revision 1.2  2008/03/17 23:58:41  jwawerla
 * Added logwrite support
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
 * Revision 1.2  2008/01/11 02:05:17  jwawerla
 * Added local coordinate system
 *
 * Revision 1.1.1.1  2008/01/10 19:44:02  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#include "wavefrontmap.h"

#include "utilities.h"
#include "printerror.h"
#include "string.h"

/** Default speed to initialize map [m/s] */
const float DEFAULT_SPEED = 0.6;
/** Maximum allowable speed [m/s] */
const float MAX_SPEED = 1.0;
/** Kernel size */
const int KERNEL_SIZE = 3;



/*
//-----------------------------------------------------------------------------
#ifdef STAGE
CWaveFrontMap::CWaveFrontMap ( CStageGridMap* obstacleMap, std::string name)
    : CGridMap ( obstacleMap->getNumCellsX(),
                 obstacleMap->getNumCellsY(),
                 obstacleMap->getCellSize() )
{

  Stg::Model* stgModel;
  Stg::Model* bg;

  constructorBase(obstacleMap, name);

  stgModel = ((CStageGridMap*)obstacleMap)->getStageModel();
  assert(stgModel);
  stgModel->SetProperty( "wavefront_map", static_cast<void*>( this ) );

  bg = stgModel->GetWorld()->GetModel( "background" );
  assert( bg );
  bg->AddVisualizer( &stgMapVis, false );
}
#endif
*/
//---------------------------------------------------------------------------
CWaveFrontMap::CWaveFrontMap ( CGridMap* obstacleMap, std::string name )
    : CGridMap ( obstacleMap->getNumCellsX(),
                 obstacleMap->getNumCellsY(),
                 obstacleMap->getCellSize() )
{
  int kernelCenter;

  mObstacleMap = obstacleMap;

  mObstacleGrowth = 0.4;
  mObstacleThreshold = 0.5;
  mMinCellValue = 0;
  mMaxPathLength = 3000;  // [m]
  mMaxNumWaypoints = 2048;
  mMaxWaypointDistance = 20.0;  // [m]
  mMinWaypointDistance = 1.0;  // [m]
  mAngleWaypointThreshold = D2R ( 5.0 );
  mSensorMapTimeOffset = 0.0;
  mSensorMapForgetRate = 0.01;
  mRobotPose = NULL;
  mTimeStampOfLastUpdate = 0.0;

  mName = name;

  mDistanceMap = new float*[mNumCellsX];
  mSensorMap = new float*[mNumCellsX];

  for ( int x = 0; x < mNumCellsX; x++ ) {
    mDistanceMap[x] = new float[mNumCellsY];
    mSensorMap[x] = new float[mNumCellsY];
  }

  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      mMapData[x][y] = mMinCellValue;
      mSensorMap[x][y] = mMinCellValue;
    }
  }

  generateDistanceMap();

#ifdef SPEED_OG
  mSOGMap = new tSOGData*[mNumCellsX];
  mFgSOGEnabled = false;
  for ( int x = 0; x < mNumCellsX; x++ ) {
    mSOGMap[x] = new tSOGData[mNumCellsY];
  }

  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold )
        mSOGMap[x][y].sog = 0.0;
      else
        mSOGMap[x][y].sog = DEFAULT_SPEED;
    }
  }
#endif

  mKernel = new float*[KERNEL_SIZE];
  // generate kernel
  kernelCenter = ( KERNEL_SIZE - 1 ) / 2;
  for ( int x = 0; x < KERNEL_SIZE; x++ ) {
    mKernel[x] = new float[KERNEL_SIZE];
    for ( int y = 0; y < KERNEL_SIZE; y++ ) {
      mKernel[x][y] = sqrt ( pow2 ( ( x - kernelCenter ) ) +
                             pow2 ( ( y - kernelCenter ) ) );
    }
  }
}
//-----------------------------------------------------------------------------
CWaveFrontMap::~CWaveFrontMap()
{
  for ( int x = 0; x < mNumCellsX; x++ ) {
    delete[] mDistanceMap[x];
    delete[] mSensorMap[x];
#ifdef SPEED_OG
    delete[] mSOGMap[x];
#endif
  }
}
//---------------------------------------------------------------------------
void CWaveFrontMap::setAngleWaypointThreshold ( const float angle )
{
  mAngleWaypointThreshold = angle;
}
//---------------------------------------------------------------------------
void CWaveFrontMap::setObstacleGrowth ( const float growth )
{
  mObstacleGrowth = growth;
}
//---------------------------------------------------------------------------
void CWaveFrontMap::setWaypointDistance ( const float minDist, const float maxDist )
{
  mMinWaypointDistance = minDist;
  mMaxWaypointDistance = maxDist;
}
//---------------------------------------------------------------------------
void CWaveFrontMap::generateDistanceMap()
{
  float d = sqrt ( 2.0 ) * mCellSize;
  mMaxObstacleDistance = 0.0;

  // from left to right
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( x > 0 ) {
          mDistanceMap[x][y] = mDistanceMap[x-1][y] + mCellSize;
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }
  // from right to left
  for ( int x = mNumCellsX - 1; x >= 0; x-- ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( x < mNumCellsX - 1 ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y],
           mDistanceMap[x+1][y] + mCellSize );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // from bottom to top
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( y > 0 ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y],
           mDistanceMap[x][y-1] + mCellSize );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // from top to bottom
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = mNumCellsY - 1; y >= 0; y-- ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( y < mNumCellsY - 1 ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y],
           mDistanceMap[x][y+1] + mCellSize );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // from bottom left to top right
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( ( x > 0 ) && ( y > 0 ) ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y],
           mDistanceMap[x-1][y-1] + d );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // from bottom right to top left
  for ( int x = mNumCellsX - 1; x >= 0; x-- ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( ( x < mNumCellsX - 1 ) && ( y > 0 ) ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y],
           mDistanceMap[x+1][y-1] + d );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // from top right to bottom left
  for ( int x = mNumCellsX - 1; x >= 0; x-- ) {
    for ( int y = mNumCellsY - 1; y >= 0; y-- ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( ( x < mNumCellsX - 1 ) && ( y < mNumCellsY - 1 ) ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y], 
          mDistanceMap[x+1][y+1] + d );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // from top left to bottom right
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) {
        mDistanceMap[x][y] = 0.0f;
      } else {
        if ( ( x > 0 ) && ( y < mNumCellsY - 1 ) ) {
          mDistanceMap[x][y] = min ( mDistanceMap[x][y],
           mDistanceMap[x-1][y+1] + d );
        } else
          mDistanceMap[x][y] = 0.0f;
      }
    }
  }

  // find the maximum distance
  mMaxObstacleDistance = 0.0f;
  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      mMaxObstacleDistance = max ( mMaxObstacleDistance, mDistanceMap[x][y] );
    }
  }
}
//---------------------------------------------------------------------------
float CWaveFrontMap::getDistanceToClosestObstacle (const CPoint2d localPos )
{
  int x, y;

  x = mCenterCellX + ( int ) ( localPos.mX / mCellSize );
  y = mCenterCellY + ( int ) ( localPos.mY / mCellSize );

  // check boundaries
  if ( ( x < 0 ) || ( x >= mNumCellsX ) ||
       ( y < 0 ) || ( y >= mNumCellsY ) ) {
    PRT_ERR2 ( "Coordinates out of bounds %f %f", localPos.mX, localPos.mY );
    return -1; // error
  }
  return  mDistanceMap[x][y] * mCellSize;
}
//-----------------------------------------------------------------------------
float CWaveFrontMap::getGradient (const CPoint2d localPos )
{
  int x, y;

  x = mCenterCellX + ( int ) ( localPos.mX / mCellSize );
  y = mCenterCellY + ( int ) ( localPos.mY / mCellSize );

  // check boundaries
  if ( ( x < 0 ) || ( x >= mNumCellsX ) ||
       ( y < 0 ) || ( y >= mNumCellsY ) ) {
    PRT_ERR2 ( "Coordinates out of bounds %f %f", localPos.mX, localPos.mY );
    return 0; // error
  }
  // check if there is an obstacle at this location
  if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold )
    return WAVEFRONT_MAP_UNDEFINED;

  return  mMapData[x][y];
}
//----------------------------------------------------------------------------
float CWaveFrontMap::getGradientDirection ( const CPoint2d localPos )
{
  int x, y;
  float minValue = INFINITY;
  float direction = 0;

  x = mCenterCellX + ( int ) ( localPos.mX / mCellSize );
  y = mCenterCellY + ( int ) ( localPos.mY / mCellSize );

  // check boundaries
  if ( ( x < 0 ) || ( x >= mNumCellsX ) ||
       ( y < 0 ) || ( y >= mNumCellsY ) ) {
    PRT_ERR2 ( "Coordinates out of bounds %f %f", localPos.mX, localPos.mY );
    return 0; // error
  }

  for ( int i = x - 1; i <= x + 1; i++ ) {
    for ( int j = y - 1; j <= y + 1; j++ ) {
      if ( ( i >= 0 ) && ( i < mNumCellsX ) &&
           ( j >= 0 ) && ( j < mNumCellsY ) ) {

        if ( minValue > mMapData[i][j] ) {
          minValue = mMapData[i][j];
          direction = atan2 ( j - y, i - x );
        }
      }
    }
  }
  return direction;
}
//----------------------------------------------------------------------------
void CWaveFrontMap::setRobotPose ( CPose2d* pose )
{
  mRobotPose = pose;
}
//----------------------------------------------------------------------------
void CWaveFrontMap::update ( const float timestamp )
{
  mSensorMapTimeOffset = mSensorMapTimeOffset + fabs ( timestamp -
                         mTimeStampOfLastUpdate )
                         * mSensorMapForgetRate;

  //if ( useSensorData ) {
  generateSensorMap();
// }
  mSpeedOverGround = fabs ( mRobotPose->distance ( mLastRobotPose ) /
                            ( timestamp - mTimeStampOfLastUpdate ) ) ;

  mLastRobotPose = *mRobotPose;
  mTimeStampOfLastUpdate = timestamp;

#ifdef SPEED_OG
  int robotCellX, robotCellY;
  if ( mFgSOGEnabled ) {
    robotCellX = mCenterCellX + ( int ) ( mRobotPose->mX  / mCellSize );
    robotCellY = mCenterCellY + ( int ) ( mRobotPose->mY  / mCellSize );

    mSOGMap[robotCellX][robotCellY].sog = mSOGMap[robotCellX][robotCellY].sog +
                                          0.2 * ( mSpeedOverGround -
                                                  mSOGMap[robotCellX][robotCellY].
                                                  sog );
    mSOGMap[robotCellX][robotCellY].sog = timestamp;
  }
#endif
}
//----------------------------------------------------------------------------
void CWaveFrontMap::generateSensorMap()
{
  //float odds,
  float fx, fy;
  //int boundingBox;
  int cellGrowth;
  //int robotCellX, robotCellY;
  ARangeFinder* rf = NULL;
  std::list<ARangeFinder*>::iterator it;

  // to prevent overflow we have to reset the map everyonce in a while
  if ( mSensorMapTimeOffset > 1e6 ) {
    for ( int x = 0; x < mNumCellsX; x++ ) {
      for ( int y = 0; y < mNumCellsY; y++ ) {
        mSensorMap[x][y] = max ( mSensorMap[x][y] - mSensorMapTimeOffset, 0.0 );
      }
    }
    mSensorMapTimeOffset = 0.0;
  }

  if ( mRobotPose == NULL ) {
    PRT_ERR0 ( "Robot pose was not set" );
    return;
  }

  // check if we have any sensors registered
  if ( mRangeFinderList.empty() ) {
    PRT_WARN0 ( "No range finders available" );
    return;
  }

  cellGrowth = ( int ) round ( mObstacleGrowth / mCellSize );
  cellGrowth = 0;
  float r, yaw, dx,dy;
  int stepCount, xC, yC;
  float stepSize = floor ( mCellSize / 2.0 );

  for ( it = mRangeFinderList.begin(); it != mRangeFinderList.end(); it++ ) {
    rf = *it;

    // check if this sensor is actually enabled
    if ( rf->isEnabled() ) {
      for ( unsigned int i = 0; i < rf->getNumSamples(); i++ ) {

        r = rf->mRangeData[i].range;
        yaw = normalizeAngle ( mRobotPose->mYaw  +
                                rf->mRelativeBeamPose[i].mYaw );

        stepCount = ( int ) floor ( r/stepSize ) + 2;
        dx = ( cos ( yaw ) * r ) / ( float ) ( stepCount - 1 );
        dy = ( sin ( yaw ) * r ) / ( float ) ( stepCount - 1 );

        for ( int j=0; j < stepCount; j++ ) {
          fx = mRobotPose->mX + dx * j;
          fy = mRobotPose->mY + dy * j;
          xC = meterToCellIndexX ( fx );
          yC = meterToCellIndexY ( fy );


          if ( isCellIndexValid ( xC, yC ) ) {
            mSensorMap[xC][yC] = max ( mSensorMap[xC][yC], mSensorMapTimeOffset );
            mSensorMap[xC][yC] = mSensorMap[xC][yC] - 0.025;
          }
        } // for step count

        if ( r < rf->getMaxRange() ) {
          fx = mRobotPose->mX + dx * stepCount;
          fy = mRobotPose->mY + dy * stepCount;
          xC = meterToCellIndexX ( fx );
          yC = meterToCellIndexY ( fy );
          for ( int x = xC - cellGrowth; x <= xC + cellGrowth; x++ ) {
            for ( int y = yC - cellGrowth; y <= yC + cellGrowth; y++ ) {
              if ( isCellIndexValid ( x, y ) ) {
                mSensorMap[x][y] = max ( mSensorMap[x][y], mSensorMapTimeOffset );
                mSensorMap[x][y] = mSensorMap[x][y] + 0.1;
                // make sure everything stays in the limits
                mSensorMap[x][y] = min ( mSensorMap[x][y], mSensorMapTimeOffset +
                                         1.0 );
              }
            }
          }
        }

      } // for readings
    } // if enabled
  } // for rangefinder
  /*
    for ( it = mRangeFinderList.begin(); it != mRangeFinderList.end(); it++ ) {
      rf = *it;
      boundingBox = ( int ) ceil ( rf->getMaxRange() / mCellSize );

      for ( int x = robotCellX - boundingBox; x < robotCellX + boundingBox; x++ ) {
        for ( int y = robotCellY - boundingBox; y < robotCellY + boundingBox; y++ )
        {
          if ( isCellIndexValid(x, y) ) {
            // calculate metric position of cell
            fx = ( x - mCenterCellX ) * mCellSize;
            fy = ( y - mCenterCellY ) * mCellSize;
            // query inverse sensor model
            odds = rf->localInverseRangeSensorModel ( fx, fy, *mRobotPose );

            // make sure the time offset corrected map cell is at least 0
            mSensorMap[x][y] = MAX ( mSensorMap[x][y], mSensorMapTimeOffset );

            // add new information
            mSensorMap[x][y] = mSensorMap[x][y] + odds;

            // make sure everything stays in the limits
            mSensorMap[x][y] = MIN ( mSensorMap[x][y], mSensorMapTimeOffset + 1.0 );
          }
        } // for y
      } // for x
    } // for rangerfinder
  */
}
//----------------------------------------------------------------------------
int CWaveFrontMap::calculateWaveFront ( const CPose2d goal,
                                        tDataSource useSensorData )
{
  CPoint2d point;

  point.mX = goal.mX;
  point.mY = goal.mY;
  return calculateWaveFront ( point, useSensorData );
}
//----------------------------------------------------------------------------
int CWaveFrontMap::calculateWaveFront ( const float x, const float y,
                                        tDataSource useSensorData )
{
  CPoint2d point;

  point.mX = x;
  point.mY = y;
  return calculateWaveFront ( point, useSensorData );
}
//----------------------------------------------------------------------------
int CWaveFrontMap::calculateWaveFront ( const CPoint2d goal,
                                        tDataSource useSensorData )
{
  int count = 0;
  tCellCoordinate cell;
  tCellCoordinate newCell;
  std::list<tCellCoordinate> cellList;
  float m;
  int x, y;
  int halfKernelLenght;

  // clear old wavefront map
  preSetMap ( WAVEFRONT_MAP_UNDEFINED );
  mMaxCellValue = 0.0;

  mGoalPosition = goal;
  halfKernelLenght = ( KERNEL_SIZE - 1 ) / 2;

  x = mCenterCellX + ( int ) round ( goal.mX / mCellSize );
  y = mCenterCellY + ( int ) round ( goal.mY / mCellSize );

  // check boundaries
  if ( ( x < 0 ) || ( x >= mNumCellsX ) ||
       ( y < 0 ) || ( y >= mNumCellsY ) ) {
    PRT_ERR2 ( "Coordinates out of bounds %f %f", goal.mX, goal.mY );
    return 0; // error
  }

  cell.x = x;
  cell.y = y;
  cellList.push_back ( cell );

  mMapData[x][y] = 0.0;


  // iterate over cell list and perform wavefront calculation
  while ( !cellList.empty() ) {
    count ++;
    cell = cellList.front();
    cellList.pop_front();
    for ( int i = 0; i < KERNEL_SIZE; i ++ ) {
      for ( int j = 0; j < KERNEL_SIZE; j ++ ) {
        x = cell.x + i - halfKernelLenght;
        y = cell.y + j - halfKernelLenght;
        // check boundary conditions
        if ( x < 0 ) continue;
        if ( x >= mNumCellsX ) continue;
        if ( y < 0 ) continue;
        if ( y >= mNumCellsY ) continue;
        if ( mObstacleMap->mMapData[x][y] >= mObstacleThreshold ) continue;
        if ( mDistanceMap[x][y] <= mObstacleGrowth ) continue;
        if ( ( useSensorData == USE_SENSOR_DATA ) &&
             ( mSensorMap[x][y] - mSensorMapTimeOffset >= mObstacleThreshold ) )
          continue;


        // we ruled out obstacles etc, so this cell must be clear
        if ( mMapData[x][y] == WAVEFRONT_MAP_UNDEFINED ) {
          newCell.x = x;
          newCell.y = y;
          cellList.push_back ( newCell );
        }

#ifdef SPEED_OG
        if ( mFgSOGEnabled ) {
          m = mMapData[cell.x][cell.y] +
              mKernel[i][j] * ( MAX_SPEED - mSOGMap[x][y].sog )
              / MAX_SPEED;
        } else {
          m = mMapData[cell.x][cell.y] + mKernel[i][j];
        }
#else
        m = mMapData[cell.x][cell.y] + mKernel[i][j];
#endif
        if ( mMapData[x][y] > m ) {
          mMapData[x][y] = m;
          mMaxCellValue = max ( mMaxCellValue, m );
        }
      } // for
    } // for
  } // while

  return 1; // success
}
//----------------------------------------------------------------------------
/* This doesn't work yet
int CWaveFrontMap::calculateLocalWaveFront ( )
{
  int X, Y;
  int dir = 0;
  //float minCellValue = INFINITY;
  float halfBoundingBox = 4.0;
  //tCellCoordinate cell;
  //tCellCoordinate newCell;
  //std::list<tCellCoordinate> cellList;
  float m;
  int x, y = 0;
  int halfKernelLenght;
  int minX, minY, maxX, maxY;

  halfKernelLenght = ( mKernelSize - 1 ) / 2;

  //cellList.clear();
  //cell.x = -1;
  //cell.y = -1;

  // update sensor map
  generateSensorMap();

  // limit of bounding box
  minX = ( int ) floor ( mCenterCellX + ( mRobotPose->mX - halfBoundingBox ) /
   mCellSize );
  maxX = ( int ) ceil ( mCenterCellX + ( mRobotPose->mX + halfBoundingBox ) /
   mCellSize );
  minY = ( int ) floor ( mCenterCellY + ( mRobotPose->mY - halfBoundingBox ) /
   mCellSize );
  maxY = ( int ) ceil ( mCenterCellY + ( mRobotPose->mY + halfBoundingBox ) /
   mCellSize );

  minX = LIMIT ( minX, 0, mNumCellsX - 1 );
  maxX = LIMIT ( maxX, 0, mNumCellsX - 1 );
  minY = LIMIT ( minY, 0, mNumCellsY - 1 );
  maxY = LIMIT ( maxY, 0, mNumCellsY - 1 );

  // clear data and add boundary cells to update list
  for ( x = minX + 1; x <= maxX - 1; x++ ) {
    for ( y = minY + 1; y <= maxY - 1; y++ ) {
      mMapData[x][y] = WAVEFRONT_MAP_UNDEFINED;
    }
  }


  // insert goal location in list of all locations
  //mMapMarkerList.clear();
  //mMapMarkerList.push_back ( CMapMarker ( CColor ( 0, 255, 0 ), ( char* ) "Goal",
  // mGoalPosition ) );

  // iterate over cell list and perform wavefront calculation

  X = minX;
  Y = minY;
  while  (maxX > minX)  {
    //printf("dir %d (%d %d) (%d %d) (%d %d) \n", dir, X, Y, minX, maxX, minY,
     maxY);
    switch ( dir ) {
      case 0:   // east
        X = X + 1;
        if ( X >= maxX )
          dir = 1;
        break;
      case 1:   // south
        Y = Y + 1;
        if ( Y >= maxY )
          dir = 2;
        break;
      case 2:   // west
        X = X - 1;
        if ( X <= minX )
          dir = 3;
        break;
      case 3:   // north
        Y = Y - 1;
        if ( Y <= minY ) {
          dir = 0;
          minX = minX + 1;
          maxX = maxX - 1;
          minY = minY + 1;
          maxY = maxY - 1;
          if (maxX < 0)
            sleep(1000);
          X = minX;
          Y = minY;
        }
        break;
    } // switch

    for ( int i = 0; i < mKernelSize; i ++ ) {
      for ( int j = 0; j < mKernelSize; j ++ ) {
        x = X + i - halfKernelLenght;
        y = Y + j - halfKernelLenght;
        // check boundary conditions

        if ( ( x >= 0 ) && ( x < mNumCellsX ) && ( y >= 0 ) && ( y < mNumCellsY ) )
        {
          if ( ( mObstacleMap->mMapData[x][y] < mObstacleThreshold ) &&
               ( ( mSensorMap[x][y] - mSensorMapTimeOffset < mObstacleThreshold ) ||
                 !useSensorData ) && ( mDistanceMap[x][y] >= mObstacleGrowth ) ) {
            //if ( mMapData[x][y] == WAVEFRONT_MAP_UNDEFINED ) {
            //  newCell.x = x;
            //  newCell.y = y;
            //  cellList.push_back ( newCell );
           // }
            m = mMapData[X][Y] + mKernel[i][j];

            if ( mMapData[x][y] > m ) {
              mMapData[x][y] = m;
              mMaxCellValue = MAX ( mMaxCellValue, m );
            }
          }
        }
      } // for
    } // for
  } // while
  return 1; // success
}
*/
//----------------------------------------------------------------------------
void CWaveFrontMap::addRangeFinder ( ARangeFinder* rangeFinder )
{
  mRangeFinderList.push_back ( rangeFinder );
}
//----------------------------------------------------------------------------
void CWaveFrontMap::saveMaptoFile ( char* fileName )
{
  FILE* fp;

  fp = fopen ( fileName, "w" );
  if ( fp == NULL ) {
    PRT_ERR1 ( "Failed to open file %s", fileName );
    return;
  }

  for ( int x = 0; x < mNumCellsX; x++ ) {
    for ( int y = 0; y < mNumCellsY; y++ ) {
      fprintf ( fp, "%f ", mMapData[x][y] );
    }
    fprintf ( fp, "\n" );
  }
}
//----------------------------------------------------------------------------
float CWaveFrontMap::calculatePlanFrom ( const CPose2d localPos )
{
  CPoint2d point;

  point.mX = localPos.mX;
  point.mY = localPos.mY;

  return calculatePlanFrom ( point );
}
//----------------------------------------------------------------------------
float CWaveFrontMap::calculatePlanFrom ( const float x, const float y )
{
  CPoint2d point;

  point.mX = x;
  point.mY = y;
  return calculatePlanFrom ( point );
}
//----------------------------------------------------------------------------
float CWaveFrontMap::calculatePlanFrom ( const CPoint2d localPos )
{
  int x, y;
  int maxSteps;
  int steps = 0;
  float length = 0;
  float minValue;
  float wayPointDist;
  float maxDistValue;
  float lastHeading = TWO_PI;
  float heading = 0;
  tCellCoordinate minCell;
  CPoint2d minNeighbour;
  CPoint2d lastCell;
  CWaypoint2d waypoint;

  maxSteps = ( int ) ( mMaxPathLength / mCellSize );

  x = mCenterCellX + ( int ) ( localPos.mX / mCellSize );
  y = mCenterCellY + ( int ) ( localPos.mY / mCellSize );
  waypoint = localPos;

  minCell.x = x;                  // shut up compiler
  minCell.y = y;                  // shut up compiler
  lastCell.mX = localPos.mX;
  lastCell.mY = localPos.mY;

  // clear way point list
  mWaypointList.clear();

  if ( ( x < 0 ) || ( x >= mNumCellsX ) ||
       ( y < 0 ) || ( y > mNumCellsY ) ) {
    PRT_ERR2 ( "Coordinates out of bounds %f %f", localPos.mX, localPos.mY );
    return -1;
  }

  //while ( ( mMapData[x][y] != 0.0f ) && ( steps < maxSteps ) ) {
  while ( ( not isAboutZero(mMapData[x][y]) ) && ( steps < maxSteps ) ) {
    steps ++;
    minValue = INFINITY;
    maxDistValue = -1;

    for ( int i = x - 1; i <= x + 1; i++ ) {
      for ( int j = y - 1; j <= y + 1; j++ ) {
        if ( ( i >= 0 ) && ( i < mNumCellsX ) &&
             ( j >= 0 ) && ( j < mNumCellsY ) ) {

          if ( ( mMapData[i][j] < minValue ) ||
               ( ( mMapData[i][j] == minValue ) &&
                 ( mDistanceMap[i][j] > maxDistValue ) ) ) {
            minValue = mMapData[i][j];
            maxDistValue = mDistanceMap[i][j];
            minCell.x = i;
            minCell.y = j;
          }
        }
      } // for y
    } // for x
    // next cell to consider
    x = ( int ) minCell.x;
    y = ( int ) minCell.y;
    // convert from cells to meter
    minNeighbour.mX = ( minCell.x - mCenterCellX ) * mCellSize;
    minNeighbour.mY = ( minCell.y - mCenterCellY ) * mCellSize;
    // calculate path lenght
    length = length + sqrt ( pow2 ( minNeighbour.mX - lastCell.mX ) +
                             pow2 ( minNeighbour.mY - lastCell.mY ) );
    // distance to last waypoint
    wayPointDist = sqrt ( pow2 ( minNeighbour.mX - waypoint.getPose().mX ) +
                          pow2 ( minNeighbour.mY - waypoint.getPose().mY ) );

    // have we traveled far enough from the last waypoint ?
    if ( wayPointDist > mMinWaypointDistance ) {
      // what the new heading ?
      heading = atan2 ( minNeighbour.mY - lastCell.mY,
                        minNeighbour.mX - lastCell.mX );
      // has our heading changed ? if so this should be a new waypoint
      if ( ( !epsilonEqual ( heading, lastHeading, mAngleWaypointThreshold ) ) ||
           ( wayPointDist > mMaxWaypointDistance ) ) {

        waypoint. setPose ( minNeighbour,
                            atan2 ( minNeighbour.mY - waypoint.getPose().mY,
                                    minNeighbour.mX - waypoint.getPose().mX ) );
        mWaypointList.push_back ( waypoint );
        if ( mWaypointList.size() >= mMaxNumWaypoints ) {
          PRT_ERR1 ( "Exceeded maximum allowable number of %d waypoints",
                     mMaxNumWaypoints );
          return -1;  // error incomplete path
        }
        lastHeading = heading;
      }
    }
    lastCell = minNeighbour;
  } // while
  // add goal as last waypoint
  waypoint.setPose ( mGoalPosition, heading );
  waypoint.setLabel ( "Goal" );
  mWaypointList.push_back ( waypoint );

  // did we actually complete the planning ??
  if ( steps >= maxSteps ) {
    PRT_MSG1 (5, "Could not complete plan in %d steps", maxSteps );
    return -1; //
  }
  // return length of plan [m]
  return length;
}
//----------------------------------------------------------------------------
void CWaveFrontMap::getWaypointList ( std::list<CWaypoint2d> &destList )
{
  CWaypoint2d wp;
  std::list<CWaypoint2d>::iterator it;

  destList.clear();

  for ( it = mWaypointList.begin(); it != mWaypointList.end(); it++ ) {
    wp = *it;
    destList.push_back ( wp );
  }
}
//----------------------------------------------------------------------------



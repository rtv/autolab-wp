/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: wavefrontmap.h,v 1.11 2009-04-09 17:03:38 vaughan Exp $
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
 * $Log: wavefrontmap.h,v $
 * Revision 1.11  2009-04-09 17:03:38  vaughan
 * removed QT and SDL dependencies. Added install target for libraries
 *
 * Revision 1.10  2009-04-08 22:40:41  jwawerla
 * Hopefully ND interface issue solved
 *
 * Revision 1.9  2009-04-05 01:16:51  jwawerla
 * still debuging wavefront map
 *
 * Revision 1.8  2009-04-03 15:10:03  jwawerla
 * *** empty log message ***
 *
 * Revision 1.7  2009-03-31 23:52:59  jwawerla
 * Moved cell index from float to int math
 *
 * Revision 1.6  2009-03-31 04:27:33  jwawerla
 * Some bug fixing
 *
 * Revision 1.5  2009-03-29 00:54:27  jwawerla
 * Replan ctrl seems to work now
 *
 * Revision 1.4  2009-03-28 00:54:39  jwawerla
 * *** empty log message ***
 *
 * Revision 1.3  2009-03-23 06:24:22  jwawerla
 * RobotVis works now for data, maps still need more work
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
 * Revision 1.2  2008/01/11 02:05:17  jwawerla
 * Added local coordinate system
 *
 * Revision 1.1.1.1  2008/01/10 19:44:00  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#ifndef CWAVEFRONTMAP_H
#define CWAVEFRONTMAP_H

#include "rangefinder.h"
#include "gridmap.h"
#include <list>
#include <math.h>

//#define SPEED_OG 1


const float WAVEFRONT_MAP_UNDEFINED = 1000000;
const float WAVEFRONT_DYNAMICAL_OBSTACLE = 1.0;
/**
 * This class extends a grid map with wave front capabilities. It basically
 * is a map that takes a obstacle map and a goal location and generates a
 * gradient field towards the goal location
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 12/2007
 */

class CWaveFrontMap : public CGridMap
{

  public:
    /**
     * Default contrustor
     * @param obstacleMap map with obstacle to use for wave front
     */
    CWaveFrontMap( CGridMap* obstacleMap, const char* name=NULL );
    /** Default destructor */
    ~CWaveFrontMap();
    /**
     * Update the map, basically decaying sensor information
     * @param timestamp current time [s]
     */
    void update( float timestamp );
    /**
     * Sets the robot position
     * @param pos of robot
     */
    void setRobotPose(CPose* pos);
    /**
     * Adds a rangefinder for dynamic obstacles
     * @param rangerfinder to be added
     */
    void addRangeFinder(ARangeFinder* rangeFinder);
    /**
     * Calculate the wavefront for a give local location
     * @param goal location
     * @param useSensorData incoorporates the range finder data
     * @return 1 success, 0 otherwise
     */
     int calculateWaveFront(tPoint2d goal, bool useSensorData = false);
    /**
     * Calculate the wavefront for a give local location
     * @param x [m]
     * @param y [m]
     * @param useSensorData incoorporates the range finder data
     * @return 1 success, 0 otherwise
     */
     int calculateWaveFront(float x, float y, bool useSensorData = false);
     /**
      * Gets the direction of the gradient
      * @param localPos position to get gradient for
      * @return direction [rad]
      */
     float getGradientDirection(tPoint2d localPos);
     /**
      * NOTE: this doesn't work yet
      * Update the wavefront gradient only localy, based on the current
      * sensor data
      * @return 1 success, 0 otherwise
      */
     //int calculateLocalWaveFront ();
     /**
      * Gets the value of the gradient
      * @param localPos position to get gradient for
      * @return gradient value [rad]
      */
     float getGradient(tPoint2d localPos);
     /**
      * Calculates a plan from a given location to the goal location,
      * sets map markers for a plan and returns the length
      * to the goal location
      * @param localPos position to start at
      * @return length of plan [m], -1 in case of an error
      */
     float calculatePlanFrom(tPoint2d localPos);
     /**
      * Calculates a plan from a given location to the goal location,
      * sets map markers for a plan and returns the length
      * to the goal location
      * @param localPos position to start at
      * @return length of plan [m], -1 in case of an error
      */
     float calculatePlanFrom(CPose localPos);
     /**
      * Calculates a plan from a given location to the goal location,
      * sets map markers for a plan and returns the length
      * to the goal location
      * @param x of start
      * @param y of start
      * @return length of plan [m], -1 in case of an error
      */
     float calculatePlanFrom(float x, float y);
     /**
      * Gets the distance to the closest obstacle
      * @return distance [m]
      */
     float getDistanceToClosestObstacle(tPoint2d localPos);
     /**
      * Gets the name of the map
      * @return map name
      */
     char* getName() {return mName; };
     /**
      * Gets the way point list, note this method clears destList first
      * @param destList to fill with way points
      */
     void getWayPointList(std::list<CPose> &destList);
     /**
      * Saves the gradient map to a file
      * @param filename
      */
     void saveMaptoFile( char* fileName);
#ifdef SPEED_OG
     /** Datastructure for speed over ground data */
     typedef struct {
       /** Speed over ground [m/s] */
       float sog;
       /** Timestamp of last update [s] */
       float timestamp;
     } tSOGData;
     /** Map with speed over ground data */
     tSOGData** mSOGMap;
#endif
     /** Map with the local sensor data */
     float** mSensorMap;
     /** Sensor map time offset */
     float mSensorMapTimeOffset;
     /** List of way points */
     std::list<CPose> mWayPointList;
    /** Map that contains all obstacles */
    CGridMap* mObstacleMap;
  protected:
    /**
     * Generates the distance to closest obstacle map from a give
     * obstacle map
     */
    void generateDistanceMap();
    /** Generates an obstacle map form the range finder information */
    void generateSensorMap();

     typedef struct {
       int x;
       int y;
     } tCellCoordinate;
  private:
    /** Robot pose */
    CPose* mRobotPose;
    /** List of range finders */
    std::list<ARangeFinder*> mRangeFinderList;
    /** Name of map */
    char mName[25];
    /** Kernel */
    float** mKernel;
    /** Threshold for cell values, everything >= is considered an obstacle */
    float mObstacleThreshold;
    /** Maps the distance to the closest obstacle */
    float** mDistanceMap;
    /** Maximal path planning length, we don't consider paths that are longer [m] */
    float mMaxPathLength;
    /** Maximal distance for any cell to any obstacle [m] */
    float mMaxObstacleDistance;
    /** Maximal number of way points */
    unsigned int mMaxNumWayPoints;
    /** Goal location */
    tPoint2d mGoalPosition;
    /** Value by with to grow obstacles [m] */
    float mObstacleGrowth;
    /** Minimum allowable distance between two waypoints [m] */
    float mMinWayPointDistance;
    /** Maximum allowable distance between two waypoints [m] */
    float mMaxWayPointDistance;
    /** Threshold for direction change between two waypoints [rad] */
    float mAngleWayPointThreshold;
    /** Forget rate for the sensor map data [1/s] */
    float mSensorMapForgetRate;
    /** Time stamp of last update call [s] */
    float mTimeStampOfLastUpdate;
#ifdef SPEED_OG
    /** Flags if speed over ground measurements should be included in the map */
    bool mFgSOGEnabled;
#endif
    /** Speed over ground from last time step [m/s] */
    float mSpeedOverGround;
    /** Last robot pose information */
    CPose mLastRobotPose;
};

#endif

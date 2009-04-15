/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: gridmap.h,v 1.8 2009-04-09 17:03:37 vaughan Exp $
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
 * $Log: gridmap.h,v $
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
 * Revision 1.2  2008/01/11 02:05:16  jwawerla
 * Added local coordinate system
 *
 * Revision 1.1.1.1  2008/01/10 19:44:01  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/
#ifndef CGRIDMAP_H
#define CGRIDMAP_H

#include "math.h"
#include "utilities.h"
#include "mapmarker.h"
#include "error.h"
#include "common.h"
// #include "stage.hh"
#include <list>


/**
 * This class defines a grid style map
 * The global coordinate system is NED with the origin (0,0) in the center
 * of the map. The cell coordinate system has it's origin in the lower left corner.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 11/2007
 */
class CGridMap
{
  public:
    /**
     * Default contructor
     * @param numCellsX number of cells in X
     * @param numCellsY number of cells in Y
     * @param cellSize size of a cell [m]
     */
    CGridMap ( int numCellsX, int numCellY, float cellSize );

    /**
     * Constructs a map from an occupancy grid (0=empty, >0=occupied).
     * Map cell values are set to 0.0=empty, 1.0=occupied.
     * @param data occupancy grid, one uint8_t per cell
     * @param numCellsX number of cells in X
     * @param numCellsY number of cells in Y
     * @param cellSize size of a cell [m]
     */
    CGridMap ( uint8_t* data, int numCellsX, int numCellY, float cellSize );
    /**
     * Constructs a map from a Stage model grid (0=empty, >0=occupied).
     * Map cell values are set to 0.0=empty, 1.0=occupied.
     * @param model Stage model
     * @param cellSize size of a cell [m]
     */
    //CGridMap ( Stg::Model* model, float cellSize );

    /**
     * Loads a map from a file
     * @param filename to load
     * @param pixelPerMeter number of pixels in one meter in the image
     * @param cellSize size of a cell [m]
     * @param minCellValue minimal cell value
     * @param maxCellValue maximal cell value
     */
    CGridMap ( char* fileaname, float pixelPerMeter, float cellSize ,
               float minCellValue, float maxCellValue );

    /** Default destructor */
    ~CGridMap();
    /**
     * Pre sets every map cell with the value value
     * @param value to set
     */
    void preSetMap ( float value );
    /**
     * Pre sets every map cell with in a box around center with the value value
     * @param value to set
     * @param center
     * @param halfBoxLenght half lenght of box [m]
     */
    void preSetMap ( float value, CPose center, float halfBoxLenght );
    /**
     * Gets the name of a view for the device
     * @return name of a view to visualize the device
     */
    char* getViewName();
    /**
     * Gets the width of the map
     * @return [m]
     */
    float getMapWidth();
    /**
     * Gets the height of the map
     * @return [m]
     */
    float getMapHeight();
    /**
     * Gets the cell value at a given position
     * @param pos position to get value for
     * @return cell value
     */
    float getCellValue ( tPoint2d pos );
    /**
     * Gets the cell value at a given position
     * @param pos position to get value for
     * @return cell value
     */
    float getCellValue ( CPose pose );
    /**
     * Gets the size of a cell
     * @return cell size [m]
     */
    float getCellSize() { return mCellSize; };
    /**
     * Gets the number of cells in x direction
     * @return number of cells
     */
    int getNumCellsX() { return mNumCellsX; };
    /**
     * Gets the number of cells in y direction
     * @return number of cells
     */
    int getNumCellsY() { return mNumCellsY; };
    /**
     * Gets the maximal possibile value of a cell
     * @return max cell value
     */
    float getMaxCellValue() { return mMaxCellValue; };
    /**
     * Gets the minimal possibile value of a cell
     * @return min cell value
     */
    float getMinCellValue() { return mMinCellValue; };

    /** Actual map data */
    float** mMapData;
    /**
     * List of map markers
     */
    std::list<CMapMarker> mMapMarkerList;
    /**
     * Marks the current position of the robot
     * @param robotPos position of robot to mark
     */
    void setRobotPosition ( tPoint2d robotPos );
    /**
     * Gets the cell the robot is located at, intended for drawing purposes
     * @return cell (x,y)
     */
    tPoint2d getRobotCell() { return mRobotCell; };

  protected:
    /**
     * Transforms map center coordinates into local grid coordinate system
     * @param gPos global position
     * @return local position
     */
    tPoint2d centerToLocal ( tPoint2d gPos );
    /**
     * Iterates over the whole map and finds the larges cell value
     * @return max. cell value
     */
    float findMaxCellValue();
    /** Cell (x,y) the robot is located at */
    tPoint2d mRobotCell;
    /** Size of a quadratic cell [m] */
    float mCellSize;
    /** Size of the map in number of cells in x - north direction */
    int mNumCellsX;
    /** Size of the map in number of cells in y - west direction */
    int mNumCellsY;
    /** Center cell x */
    int mCenterCellX;
    /** Center cell y */
    int mCenterCellY;
    /** Maximal possible cell value */
    float mMaxCellValue;
    /** Minimal possible cell value */
    float mMinCellValue;
};

#endif

/***************************************************************************
 * Project: autolab-wp                                                     *
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
 * $Log: $
 *
 ***************************************************************************/
#ifndef CGRIDMAP_H
#define CGRIDMAP_H


#include "mapmarker.h"
#include "RapiCore"
#include <list>
#include <math.h>

using namespace Rapi;

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
    void preSetMap ( float value, CPose2d center, float halfBoxLenght );
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
    float getCellValue ( CPoint2d pos );
    /**
     * Gets the cell value at a given position
     * @param pos position to get value for
     * @return cell value
     */
    float getCellValue ( CPose2d pose );
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
    void setRobotPosition ( CPoint2d robotPos );
    /**
     * Gets the cell the robot is located at, intended for drawing purposes
     * @return cell (x,y)
     */
    CPoint2d getRobotCell() { return mRobotCell; };

  protected:
    /**
     * Empty constructor for derived class that construct their own version of
     * a grid map
     */
    CGridMap();
    /**
     * Transforms map center coordinates into local grid coordinate system
     * @param gPos global position
     * @return local position
     */
    CPoint2d centerToLocal ( CPoint2d gPos );
    /**
     * Iterates over the whole map and finds the larges cell value
     * @return max. cell value
     */
    float findMaxCellValue();
    /** Cell (x,y) the robot is located at */
    CPoint2d mRobotCell;
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

/*****************************************************************************
 * Project: Utilities
 * File:    logwriter.h
 * Author:  Jens Wawerla (jwawerla_at_sfu.ca)
 *
 *****************************************************************************
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ******************************************************************************
 * $Log: logwriter.h,v $
 * Revision 1.4  2009-04-03 15:10:02  jwawerla
 * *** empty log message ***
 *
 * Revision 1.3  2009-03-29 00:54:27  jwawerla
 * Replan ctrl seems to work now
 *
 * Revision 1.2  2009-03-28 21:53:57  jwawerla
 * *** empty log message ***
 *
 * Revision 1.1  2009-03-27 01:32:58  jwawerla
 * wait prob controller improved
 *
 * Revision 1.1  2008/03/17 23:58:41  jwawerla
 * Added logwrite support
 *
 * Revision 1.5  2007/02/12 22:47:23  jwawerla
 * static was missing in logwriter
 *
 * Revision 1.4  2006/12/13 22:27:43  jwawerla
 * added singleton pattern to logwriter
 *
 * Revision 1.3  2006/07/10 21:44:48  jwawerla
 * Support for player error exception added
 *
 * Revision 1.2  2005/07/04 18:59:34  jwawerla
 * added matlab competibality to logwriter
 *
 * Revision 1.1.1.1  2005/01/17 22:31:32  jwawerla
 * first time in cvs
 *
 *
 *****************************************************************************/

#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>


/**
 * Provides a log writer, register a simple data type by calling
 * register(&VARIABLE, TYPE, "NAME", "UNIT")  and call
 * print() in order to print a log line into the log file. The class
 * takes care of time stampes
 * @author Jens Wawerla (jwawerla_at_sfu.ca)
 * @version 0.1 - 09/2004
 */
class CLogWriter
{
  public:
    /** Default destructor */
    ~CLogWriter();
    typedef enum {FLOAT, DOUBLE, INT, LONG, CHAR, BOOL} tVarType;
    /**
     * Registers a variable for logging
     * @param ptr pointer to the variable to register
     * @param varType type of the variable to register
     * @param varName name of the variable to register
     * @param varUnit physical unit of the variable
     * @param scaler scaling factor to multiply this variable with
     */
    void registerVariable ( void* ptr, tVarType varType, const char* varName,
                            const char* varUnit, double scaler = 1.0 );

    /**
     * Prints a line into the logfile
     * @return 1 if successfull, 0 otherwise
     */
    int print();
    /**
     * Prints a line into the logfile, this version allows to use a user
     * generated timestamp which might be useful if the application does
     * not run in real time, e.g. faster or slower simulations
     * @param timestamp to be used
     * @return 1 if successfull, 0 otherwise
     */
    int print ( float timestamp );
    /**
     * Prints a line into the logfile, this version allows to use a user
     * generated timestamp which might be useful if the application does
     * not run in real time, e.g. faster or slower simulations. Printing
     * is only done once per timer interval
     * @param timestamp to be used [s]
     * @param interval of printing [s]
     * @return 1 if successfull, 0 otherwise
     */
    int print ( float timestamp, float interval );
    /**
     * Sets the print ratio, this is the ration between print() calls
     * and actual prints in the log file. So one you call print()
     * more frequently then having entries in the log file. ratio 1 means
     * every call causes an entry, ratio 2 means every second call results
     * in an entry. (default is 1)
     * @param ratio to be used
     */
    void setRatio ( unsigned short ratio );
    /**
     * Singleton pattern, gets the instance of the object
     * @param fileName name of the log file
     * @param fgErrorMsg flags whether to print error messages or not
     * @param fgMatlab flags whether output should be Matlab compatible or not
     * @return instance of object
     */
    static CLogWriter* getInstance ( const char* fileName = "./default.log",
                                     bool fgErrorMsg = true,
                                     bool fgMatlab = true );

  protected:
    /**
     * Default constructor
     * @param fileName name of the log file
     * @param fgErrorMsg flags whether to print error messages or not
     * @param fgMatlab flags whether output should be Matlab compatible or not
     */
    CLogWriter ( const char* fileName = "./default.log", bool fgErrorMsg = true,
                 bool fgMatlab = true );

    /** Definition of a node in the linked list */
    typedef struct tNode {
      /** Pointer to the next node in the list */
      tNode* next;
      /** Pointer to the previous node in the list */
      tNode* prev;
      /** Pointer to the variable */
      void* ptr;
      /** Type of the variable */
      tVarType varType;
      /** Name of the variable */
      char* varName;
      /** Physical unit of the varibable */
      char* varUnit;
      /** Scaling factor to multiply this variable with */
      double scaler;
    } tNode;
    /**
     * If enabled this method prints out an error message
     * @param msg message to be printed
     */
    void errorMsg ( const char* msg1 );
    /**
     * Overwritten for convenience
     */
    void errorMsg ( const char* msg1, const char* msg2 );
    /**
     * Actual implementation of printing a line to the file
     * @param timestamp
     * @return 1 if successfull, 0 otherwise
     */
    int printLine( float timestamp );

  private:
    /** Time stamp of last print */
    double m_lastPrintTimestamp;
    /** Log file name */
    char m_fileName[50];
    /** Flags whether to print error messages or not */
    bool m_fgErrorMsg;
    /** Main node of the linked list of variable to be logged */
    tNode* m_mainNode;
    /** Last node of the linked list */
    tNode* m_lastNode;
    /**
     * Flags whether this is the first time we write a line into the log
     * file, needed to determin if we need to print a header or not
     */
    bool m_fgInit;
    /** Flags if output is Matlab compatible or not */
    bool m_fgMatlab;
    /** Width of columns in character */
    unsigned short m_columnWidth;
    /** File pointer for the log file */
    FILE* m_fpLogFile;
    /** Start time */
    time_t m_startTime;
    /** Current time stamp */
    double m_timeStamp;
    /** Call to print ratio */
    unsigned short m_callPrintRatio;
    /** Call counter */
    unsigned short m_callCount;
    /** Matlab comment symbole */
    char m_matlabComment;
};

#endif // LOGWRITER_H

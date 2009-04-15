/*****************************************************************************
 * Project: Utilities
 * File:    logwriter.cpp
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
 * $Log: logwriter.cpp,v $
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

#include "logwriter.h"

/** Only instance of this file */
CLogWriter* logInstance = NULL;

const unsigned int COL_SIZE = 40;

//-----------------------------------------------------------------------------
CLogWriter::CLogWriter ( const char* fileName, bool fgErrorMsg, bool fgMatlab )
{
  strncpy ( m_fileName, fileName, 50 );
  m_fgErrorMsg = fgErrorMsg;
  m_fgMatlab = fgMatlab;

  m_fgInit = true;
  m_columnWidth = 10;
  m_fpLogFile = NULL;
  m_callCount = 0;
  m_callPrintRatio = 1;
  m_lastPrintTimestamp = 0;

  // init linked list
  m_mainNode = new tNode;
  m_mainNode->next = NULL;
  m_mainNode->prev = NULL;

  // store reference on last node in list
  m_lastNode = m_mainNode;

  // take care of time stamps
  m_timeStamp = 0.0;
  m_startTime = time ( NULL );
  registerVariable ( &m_timeStamp, DOUBLE, "dT", "sec" );

  m_matlabComment = '%';

}
//-----------------------------------------------------------------------------
CLogWriter::~CLogWriter()
{
  tNode* tmpNode = NULL;

  // clear linked list
  while ( m_mainNode != NULL ) {
    tmpNode = m_mainNode->next;
    delete m_mainNode;
    m_mainNode = tmpNode;
  }
}
//-----------------------------------------------------------------------------
CLogWriter* CLogWriter::getInstance ( const char* fileName, bool fgErrorMsg,
                                      bool fgMatlab )
{
  if ( logInstance == NULL )
    logInstance = new CLogWriter ( fileName, fgErrorMsg, fgMatlab );

  return logInstance;
}
//-----------------------------------------------------------------------------
void CLogWriter::setRatio ( unsigned short ratio )
{
  if ( ratio >= 1 )
    m_callPrintRatio = ratio;
  else
    m_callPrintRatio = 1;
}
//-----------------------------------------------------------------------------
void CLogWriter::registerVariable ( void* ptr,
                                    tVarType varType,
                                    const char* varName,
                                    const char* varUnit,
                                    double scaler )
{
  tNode* newNode = NULL;

  // generate a new node
  newNode = new tNode;
  newNode->next = NULL;
  newNode->prev = m_lastNode;
  newNode->ptr = ptr;
  newNode->varType = varType;
  newNode->scaler = scaler;
  newNode->varName = new char[strlen ( varName ) +1];
  strcpy ( newNode->varName, varName );
  newNode->varUnit = new char[strlen ( varUnit ) +1];
  strcpy ( newNode->varUnit, varUnit );

  // insert into list
  m_lastNode->next = newNode;
  // update last node pointer
  m_lastNode = newNode;
}
//-----------------------------------------------------------------------------
int CLogWriter::print()
{
  time_t nowTime;
  // check if we need to print this time
  if ( m_callCount < m_callPrintRatio ) {
    return 1;  // nothing to print this time
  }

  // get current stop watch time
  nowTime = time ( NULL );

  return printLine ( difftime ( nowTime, m_startTime ) );
}
//-----------------------------------------------------------------------------
int CLogWriter::print ( float timestamp )
{

  // check if we need to print this time
  if ( m_callCount < m_callPrintRatio ) {
    return 1;  // nothing to print this time
  }

  return printLine ( timestamp );
}
//-----------------------------------------------------------------------------
int CLogWriter::print ( float timestamp, float interval )
{
  if ( interval > (timestamp - m_lastPrintTimestamp)   ) {
    return 1; // nothing to print this time
  }
  m_lastPrintTimestamp = timestamp;
  return printLine ( timestamp );
}
//-----------------------------------------------------------------------------
int CLogWriter::printLine ( float timestamp )
{
  tNode* tmpNode = m_mainNode->next;

  m_timeStamp = timestamp;
  // enforce column width to be at least 3 characters
  if ( m_columnWidth < 2 )
    m_columnWidth = 3;

  struct tm currentTime;
  time_t today;

  double doubleData = 0;
  float floatData = 0;
  int intData = 0;
  long longData = 0;
  char charData = 0;
  char timeStr[50];
  char colData[COL_SIZE];
  char colFormData[m_columnWidth+1];
  char colName[m_columnWidth+1];
  char colUnit[m_columnWidth-1];

  // increment call counter
  m_callCount ++;

  // reset counter
  m_callCount = 0;

  // check if already initialized, if not do so
  if ( m_fgInit ) {
    m_fpLogFile = fopen ( m_fileName, "a" );

    // open file
    if ( m_fpLogFile == NULL ) {
      errorMsg ( "Could not open file", m_fileName );
      return 0; // error
    }

    // handle time stamp
    today = time ( NULL );
    currentTime = *localtime ( &today );
    strftime ( timeStr, sizeof ( timeStr ),
               "%A %Y-%m-%d  %H:%M:%S  %Z", &currentTime );
    if ( m_fgMatlab ) {
      fprintf ( m_fpLogFile, "%c%s \n", m_matlabComment, timeStr );
      fprintf ( m_fpLogFile,
                "%c------------------------------------------------\n",
                m_matlabComment );
    }
    else {
      fprintf ( m_fpLogFile, "%s \n", timeStr );
      fprintf ( m_fpLogFile, "-----------------------------------------------\n" );
    }

    // get start time
    m_startTime = time ( NULL );

    // print header - list of variable names
    if ( m_fgMatlab ) {
      fprintf ( m_fpLogFile, "%c", m_matlabComment );
    }
    while ( tmpNode != NULL ) {
      strncpy ( colName, tmpNode->varName, m_columnWidth );
      colName[m_columnWidth] = '\0';
      fprintf ( m_fpLogFile, "%s ", colName );
      for ( unsigned int i = 0; i < m_columnWidth - strlen ( colName ); i++ )
        fprintf ( m_fpLogFile, " " );

      // increment list
      tmpNode = tmpNode->next;
    } // while

    // carrige return after 1. line of header
    fprintf ( m_fpLogFile, "\n" );

    // print header - list of variable units
    if ( m_fgMatlab ) {
      fprintf ( m_fpLogFile, "%c", m_matlabComment );
    }

    tmpNode = m_mainNode->next;
    while ( tmpNode != NULL ) {
      strncpy ( colUnit, tmpNode->varUnit, m_columnWidth );
      colUnit[m_columnWidth-2] = '\0';
      for ( unsigned int i = 0; i < m_columnWidth - 2 - strlen ( colUnit ); i++ ) {
        fprintf ( m_fpLogFile, " " );
      }
      fprintf ( m_fpLogFile, "[%s] ", colUnit );

      // increment list
      tmpNode = tmpNode->next;
    } // while

    // carrige return after header
    fprintf ( m_fpLogFile, "\n" );

    // initialization completed
    m_fgInit = false;

  } // if (m_fgInit)



  // now print data
  tmpNode = m_mainNode->next;
  while ( tmpNode != NULL ) {

    switch ( tmpNode->varType ) {
      case DOUBLE:
        memcpy ( &doubleData, tmpNode->ptr, sizeof ( double ) );
        snprintf ( colData, COL_SIZE, "%lf", doubleData * tmpNode->scaler );
        break;

      case FLOAT:
        memcpy ( &floatData, tmpNode->ptr, sizeof ( float ) );
        snprintf ( colData, COL_SIZE, "%f", floatData * tmpNode->scaler );
        break;

      case INT:
        memcpy ( &intData, tmpNode->ptr, sizeof ( int ) );
        snprintf ( colData, COL_SIZE, "%i", ( int ) ( intData * tmpNode->scaler ) );
        break;

      case LONG:
        memcpy ( &longData, tmpNode->ptr, sizeof ( long ) );
        snprintf ( colData, COL_SIZE, "%li", ( long ) ( longData * tmpNode->scaler ) );
        break;

      case CHAR:
        memcpy ( &charData, tmpNode->ptr, sizeof ( char ) );
        snprintf ( colData, COL_SIZE, "%c", charData );
        break;

      case BOOL:
        memcpy ( &charData, tmpNode->ptr, sizeof ( char ) );
        snprintf ( colData, COL_SIZE, "%d", charData );
        break;
    } // switch

    // trim to the correct length
    strncpy ( colFormData, colData, m_columnWidth );

    colFormData[m_columnWidth] = '\0';
    for ( unsigned int i = 0; i < m_columnWidth - strlen ( colFormData ); i++ ) {
      fprintf ( m_fpLogFile, " " );
    }
    // write data
    fprintf ( m_fpLogFile, "%s ", colFormData );
    tmpNode = tmpNode->next;
  } // while

  // carrige return after data line
  fprintf ( m_fpLogFile, "\n" );

  // make sure everything is on disk
  fflush ( m_fpLogFile );

  return 1; // success
}
//-----------------------------------------------------------------------------
void CLogWriter::errorMsg ( const char* msg1 )
{
  // check if printing error msg is enabled
  if ( m_fgErrorMsg ) {
    printf ( "CConfigFile: %s\n", msg1 );
  }
}
//-----------------------------------------------------------------------------
void CLogWriter::errorMsg ( const char* msg1, const char* msg2 )
{
  // check if printing error msg is enabled
  if ( m_fgErrorMsg ) {
    printf ( "CConfigFile: %s %s\n", msg1, msg2 );
  }
}
//-----------------------------------------------------------------------------

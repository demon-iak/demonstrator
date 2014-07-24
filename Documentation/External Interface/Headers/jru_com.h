/*******************************************************************************

 Copyright European Rail Software Applications (c) 2014

 --------------------------------------------------------------------------------
 Declaration of functions for interface with eurocab simulator
 --------------------------------------------------------------------------------

 Project : EuroCab Simulator
 Module  : EuroCab
 File    : jru_com.h

 *******************************************************************************/

#ifndef _JRU_COM_H
#define _JRU_COM_H

//------------------------------ include ---------------------------------------
#include "etcs_types.h"

//--------------------------- class definition -----------------------------
class CSerial;

/// class used for the management of JRU interface with EVC
class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

CJru_com
{
private:
    CSerial * m_pSerial;

    // Used only when file is used to log
    char m_pszFileName[256];

    /// Init file name with date and EVC key
    void InitFileName();

public:
    /// Default constructor: a file to store JRU data is created
    CJru_com();

    /// destructor
    ~CJru_com();

    /// Get the JRU filename (created only when the default constructor is used)
    /// @return: true if JRU file exists
    bool GetFileName(   char * const pszFileName,         ///< [out]: JRU file name
                        const size_t ulFileNameLength     ///< [in]:  JRU file name length
                    );

    /// Create new JRU file (available only when the default constructor is used)
    /// @return: true if succeed
    bool ResetFile();

    /// Get the class size (for linking consistency check in use of FULL/LIGHT EVC version)
    /// @return: class size in bytes
    int32_t Get_Class_Size( void );
};

#endif

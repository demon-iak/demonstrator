/*****************************************************************
Copyright © 2014 - European Rail Software Applications (ERSA)
                   5 rue Maurice Blin
                   67500 HAGUENAU
                   FRANCE
                   http://www.ersa-france.com

Author(s): Alexis JULIN (ERSA), Didier WECKMANN (ERSA)

Licensed under the EUPL Version 1.1.

You may not use this work except in compliance with the License.
You may obtain a copy of the License at:
http://ec.europa.eu/idabc/eupl.html

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied. See the License for the specific
language governing permissions and limitations under the License.
*****************************************************************/
/*--------------------------------------------------------------------------------
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

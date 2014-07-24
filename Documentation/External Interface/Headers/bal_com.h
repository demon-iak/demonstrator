/*******************************************************************************

 Copyright European Rail Software Applications (c) 2014

 --------------------------------------------------------------------------------
 Declaration of functions for interface with eurocab simulator
 --------------------------------------------------------------------------------

 Project : EuroCab Simulator
 Module  : EuroCab
 File    : bal_com.h

*******************************************************************************/

#ifndef _BAL_COM_H
#define _BAL_COM_H

//------------------------------ include ---------------------------------------
#include "etcs_types.h"

//--------------------------- class definition -----------------------------
class CSerial;

/// class used for the management of balise/loop communication with EVC
class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

CBalise_com
{
private:
    bool m_bInit_ok;
    bool m_bUseDirectCom;

    SComParam m_Bal_MsgQ_Com;
    SComParam m_Loop_MsgQ_Com;

#ifndef WIN32
    CSerial *   m_pSerialBalise;
    CSerial *   m_pSerialLoop;
#endif

    /// initialise class members
    void Init( void );

public:
    /// Constructor to be used when no serial/ethernet communication is used (use methods Send_Balise and Send_Loop)
    CBalise_com();

    /// destructor
    ~CBalise_com( );

    /// Send a balise message to EVC
    /// @return: 0 if message is sent correctly
    int32_t Send_Balise     (   const int32_t           iMsgLength,         ///< [in]: Message length in bytes
                                const uint8_t * const   uszMsg ,            ///< [in]: Message (binary stream)
                                const double            dBaliseLocation = 0 ///< [in]: balise location (optional, used if config CFG_BAL_WITH_ODO_STAMP is set)
            );

    /// Send a loop message to EVC
    /// @return: 0 if message is sent correctly
    int32_t Send_Loop       (   const int32_t           iMsgLength,                     ///< [in]: Message length in bytes
                                const uint8_t * const   uszMsg ,                        ///< [in]: Message (binary stream)
                                const int32_t           lSSCode = IGNORE_Q_SSCODE_VALUE ///< [in]: spread spectrum code of loop = Q_SSCODE (optional)
            );

    /// Get the class size (for linking consistency check in use of FULL/LIGHT EVC version)
    /// @return: class size in bytes
    int32_t Get_Class_Size  ( );
};

#endif

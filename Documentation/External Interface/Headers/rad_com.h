/*******************************************************************************

  Copyright European Rail Software Applications (c) 2014

--------------------------------------------------------------------------------
  Declaration of functions for interface with eurocab simulator
--------------------------------------------------------------------------------

  Project : EuroCab Simulator
  Module  : EuroCab
  File    : rad_com.h

*******************************************************************************/

#ifndef _RAD_COM_H
#define _RAD_COM_H

//------------------------------ include ---------------------------------------
#include "etcs_types.h"

//--------------------------- class definition -----------------------------

/// class used for the management of radio communication with EVC
class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

CRadio_com
{

private:
    bool    m_bInit_ok;
    bool    m_bUseDirectCom;

    SComParam    m_Rad_Com;
    SComParam    m_Rad_Com2;

public:
    /// Constructor to be used when no ethernet radio communication is used (use methods Send_Radio_Msg and Receive_Radio_Msg)
    CRadio_com( );

    /// Destructor
    ~CRadio_com( );

    /// Get the class size (for linking consistency check in use of FULL/LIGHT EVC version)
    /// @return: class size in bytes
    int32_t Get_Class_Size      ();

    /// Send a radio message to EVC via the indicated radio equipment
    /// @return: 0 if message is sent correctly
    int32_t Send_Radio_Msg      ( int32_t iMsgLength,     ///< [in]: Message length in bytes
                                  uint8_t *  uszMsg,         ///< [in]: Message (binary stream)
                                  int32_t iEquipmentID   ///< [in]: radio equipement ID (0 or 1)
                                  );

    /// Send a radio message to EVC via the first radio equipment
    /// @return: 0 if message is sent correctly
    int32_t Send_Radio_Msg1     ( int32_t iMsgLength,     ///< [in]: Message length in bytes
                                  uint8_t *  uszMsg          ///< [in]: Message (binary stream)
                                  );

    /// Send a radio message to EVC via the second radio equipment
    /// @return: 0 if message is sent correctly
    int32_t Send_Radio_Msg2     ( int32_t iMsgLength,     ///< [in]: Message length in bytes
                                  uint8_t *  uszMsg          ///< [in]: Message (binary stream)
                                  );

    /// Receive a radio message from the indicated EVC radio equipment
    /// @return Size of the received message in bytes (0 if there is no message)
    int32_t Receive_Radio_Msg   ( uint8_t *         uszMsg,         ///< [out] : pointer on the buffer to store the received data
                                  int32_t           iMaxLength,     ///< [in]  : maximum size of the buffer to store the received data
                                  bool              bWait,          ///< [in]  : (DEPRECATED)qualificator indicating if the function should block until data are received
                                  int32_t           iEquipmentID   ///< [in]  : equipment ID ( 0 or 1)
                                  );

    /// Receive a radio message from the first EVC radio equipement
    /// @return Size of the received message in bytes (0 if there is no message)
    int32_t Receive_Radio_Msg1  ( uint8_t *        uszMsg,         ///< [out] : pointer on the buffer to store the received data
                                  int32_t          iMaxLength,     ///< [in]  : maximum size of the buffer to store the received data
                                  bool             bWait           ///< [in]  : (DEPRECATED)qualificator indicating if the function should block until data are received
                                  );

    /// Receive a radio message from the second EVC radio equipement
    /// @return Size of the received message in bytes (0 if there is no message)
    int32_t Receive_Radio_Msg2  ( uint8_t *        uszMsg,         ///< [out] : pointer on the buffer to store the received data
                                  int32_t          iMaxLength,     ///< [in]  : maximum size of the buffer to store the received data
                                  bool             bWait           ///< [in]  : (DEPRECATED)qualificator indicating if the function should block until data are received
                                  );
};

#endif




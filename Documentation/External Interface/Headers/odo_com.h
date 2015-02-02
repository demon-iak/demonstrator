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
  File    : odo_com.h

*******************************************************************************/

#ifndef _ODO_COM_H
#define _ODO_COM_H

//------------------------------ include ---------------------------------------
#include "etcs_types.h"


// enumeration for tiu requests
// (driver actions from TIU)
typedef enum
{
    TIU_RQST_MAINSWITCH_ON = 0,
    TIU_RQST_MAINSWITCH_OFF,
    TIU_RQST_TRAININTEGRITY_OK,
    TIU_RQST_TRAININTEGRITY_NOK,
    TIU_RQST_CABIN_A,
    TIU_RQST_CABIN_B,
    TIU_RQST_NOCABIN,
    TIU_RQST_ISOLATION_ON,
    TIU_RQST_ISOLATION_OFF,
    TIU_RQST_EVCSLEEPING_ON,
    TIU_RQST_EVCSLEEPING_OFF,
    TIU_RQST_DIRECTION_NOMINAL,
    TIU_RQST_DIRECTION_REVERSE,
    TIU_RQST_DIRECTION_UNDEF,
    TIU_RQST_DIRECTION_STANDSTILL,
    TIU_RQST_SB_ON,
    TIU_RQST_SB_OFF,
    TIU_RQST_EB_OFF,
    TIU_RQST_EB_ON,
    TIU_RQST_SAFETY_ERROR_ON,
    TIU_RQST_SAFETY_ERROR_OFF,
    TIU_RQST_EVCNONLEADING_ON,
    TIU_RQST_EVCNONLEADING_OFF,
    TIU_RQST_EVCPASSIVESH_ON,
    TIU_RQST_EVCPASSIVESH_OFF,
    TIU_RQST_COLDMVT_ON,
    TIU_RQST_COLDMVT_OFF,
    TIU_RQST_TRAIN_DATA_FIXED,
    TIU_RQST_TRAIN_DATA_FLEXIBLE,
    TIU_RQST_TRAIN_DATA_SWITCHABLE,
    TIU_RQST_MAGNETIC_BRAKE_ON,
    TIU_RQST_MAGNETIC_BRAKE_OFF,
    TIU_RQST_EDDY_BRAKE_ON,
    TIU_RQST_EDDY_BRAKE_OFF,
    TIU_RQST_REGEN_BRAKE_ON,
    TIU_RQST_REGEN_BRAKE_OFF,
    TIU_RQST_EP_BRAKE_ON,
    TIU_RQST_EP_BRAKE_OFF,
    TIU_RQST_ADD_BRAKE_ON,
    TIU_RQST_ADD_BRAKE_OFF,
    TIU_RQST_TRACTION_ON,
    TIU_RQST_TRACTION_OFF

} t_TIUREQUEST;



//--------------------------- class definition -----------------------------


/// class for transmission of odometric data and exchange of TIU data with EVC
class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

COdo_com
{
public:
    /// constructor
    COdo_com();

    /// destructor
    ~COdo_com();

    /// Get the class size (for linking consistency check in use of FULL/LIGHT EVC version)
    /// @return: class size in bytes
    int32_t Get_Class_Size( );

    /// Get Initialization status
    /// @return true is initialisation is done
    bool IsInitOk(void) const {return m_bInit_ok;}

    /// get context to save
    /// @return true on success
    bool SaveContext( SRxTiuData * pData    ///< [out] pointer on train TIU
                      );

    /// set context to load
    /// @return true on success
    bool LoadContext( SRxTiuData * pData    ///< [in] pointer on train TIU
                      );

    /// Send a odometric data to EVC
    /// @return: 0 if message is sent correctly
    int32_t Send_Odo_data       ( t_distance   dLocation_m,    ///< [in] absolute train location in meter (incremental when moving with cabin A first)
                                  t_speed      dSpeed_m_s,     ///< [in] absolute train speed in m/s (positive when moving with cabin A first)
                                  t_accel      dGamma_m_s2     ///< [in] absolute train acceleration in m/s2
                                  );

    /// Refresh TIU from EVC
    /// @return -1 on error
    /// @return  0 when there is no TIU change
    /// @return  1 when there is a TIU change
    int32_t RefreshTIUData(void);

    /// Refresh TIU to EVC
    /// @return  0 on success
    int32_t UpdateTiuData();

    /// Get the TIU from train
    /// @return TRAIN TIU
    SRxTiuData getTrainTiu() const { return m_TrainTIU;    }

    /// Get the TIU from EVC
    /// @return EVC TIU
    STxTiuData getEvcTiu() const { return m_EvcTIU;      }


    //////////// Get TIU from EVC //////////////

    /// Get TIU data from EVC
    /// @return -1 on error
    /// @return  0 when there is no TIU change
    /// @return  1 when there is a TIU change
    int32_t GetEvcTIU           ( bool &               bEBRqst,                    ///< [out] emergency brake request
                                  bool &               bSBRqst,                    ///< [out] service brake request
                                  bool &               bTractionCutOffRqst,        ///< [out] traction cut-off request
                                  bool &               bOpenCircuitBreakerRqst,    ///< [out] open MCB request
                                  bool &               bPantographLow,             ///< [out] pantograph low request
                                  bool &               bInhibitPEB,                ///< [out] inhibition of passenger emergency brake
                                  bool &               bAirTightRqst,              ///< [out] request air tightness
                                  bool &               bPermitRegenBrake,          ///< [out] permission to use regenerative brakes
                                  bool &               bPermitMagnShoeBrake,       ///< [out] permission to use magnetic shoe brakes
                                  bool &               bPermitEddyCurrBrakeForSB,  ///< [out] permission to use eddy current brakes for service brake
                                  bool &               bPermitEddyCurrBrakeForEB,  ///< [out] permission to use eddy current brakes for emergency brake
                                  bool &               bIgnoreBTMAlarm,            ///< [out] indicate the BTM alarm should be ignored (big metal masses)
                                  int32_t &               lAllowedConsumption,        ///< [out] indicate the allowed current consumption (Ampere)
                                  bool &               bIsolationStatus,           ///< [out] indicate the status of isolation (in B3)
                                  ePlatformElevation & PlatformElevation,          ///< [out] indicate the platform elevation (in B3)
                                  ePlatformPosition &  PlatformPosition            ///< [out] indicate the platform side (in B3)
                                  );


    /// Get the emergency brake command from EVC
    /// @return EB command (true = brake application requested)
    bool GetEB (void);

    /// Get the service brake command from EVC
    /// @return SB command (true = brake application requested)
    bool GetSB (void);

    /// Get the traction cut off command from EVC
    /// @return TCO command (true =TCO requested)
    bool GetCutOff (void);

    /// Get the EVC request to open the main circuit breaker
    /// @return MCB command (true =MCB open requested)
    bool GetOpenCircuitBreaker (void);

    /// Get the EVC request to low the pantograph
    /// @return pantograph command (true = pantograph low, false = pantograph up)
    bool GetPantographLow (void);

    /// Get the EVC request to inhibit the passenger emergency brake (only in B2)
    /// @return PEB inhibition(true = inhibited, false = available)
    bool GetInhibitPEB (void);

    /// Get the EVC request to use air tightness
    /// @return air tightness request
    bool GetAirTightRqst (void);

    /// Get the EVC permission to use the regenerative brakes
    /// @return true if use of regenerative brakes is permitted
    bool GetPermitRegenBrake (void);

    /// Get the EVC permission to use the magnetic shoe brakes
    /// @return true if use of magnetic shoe  brakes is permitted
    bool GetPermitMagnShoeBrake (void);

    /// Get the EVC permission to use the eddy current brakes for service brake
    /// @return true if use of eddy current brakes is permitted for service brake
    bool GetPermitEddyCurrBrakeForSB (void);

    /// Get the EVC permission to use the eddy current brakes for emergency brake
    /// @return true if use of eddy current brakes is permitted for emergency brake
    bool GetPermitEddyCurrBrakeForEB (void);

    /// Get the EVC indication to ignore the BTM alarm
    /// @return true if BTM alarm should be ignored
    bool GetIgnoreBTMAlarm (void);

    /// Get the EVC indication of allowed current consumption (only in B3)
    /// @return the allowed current consumption (Ampere)
    int32_t GetAllowedConsumption (void);

    /// Get the EVC indication of isolation (only in B3)
    /// @return the isolation status
    bool GetIsolationStatus (void);

    /// Get the EVC indication of platform elevation (only in B3)
    /// @return the platform elevation
    ePlatformElevation GetPlatformElevation (void);

    /// Get the EVC indication of platform position (only in B3)
    /// @return the platform position
    ePlatformPosition GetPlatformPosition (void);


    //////////// SEND TIU TO EVC //////////////

    /// Send the train TIU to EVC
    /// @return 0 on success
    int32_t SetTiuData(bool            bMainSwitchOn,              ///< [in] indicate if ETCS power is on
                       bool            bTrainIntegrityOK,          ///< [in] indicate if train is integer
                       eActiveCabin    ActiveCabin,                ///< [in] indicate the opened cabin
                       bool            bIsolationRqst,             ///< [in] indicate if isolation of ETCS is requested
                       bool            bSleeping,                  ///< [in] indicate if sleeping signal is set
                       eDirection      DirectionCtrl,              ///< [in] indicate the position of the direction controller
                       bool            bEBStatus,                  ///< [in] indicate if the emergency brake is applied
                       bool            bSBStatus,                  ///< [in] indicate if the service brake is applied
                       bool            bNonLeading,                ///< [in] indicate if non-leading signal is set
                       bool            bPassiveShunting,           ///< [in] indicate if passive shunting signal is set
                       bool            bColdMvtDetected,           ///< [in] indicate if a cold movement has been detected
                       eTrainDataType  TrainDataType,              ///< [in] indicate the type of train data entry
                       bool            bRegenBrakeAvailable,       ///< [in] indicate if regenerative brakes are available
                       bool            bMagnetShoeBrakeAvailable,  ///< [in] indicate if magnetic shoe brakes are available
                       bool            bEddyBrakeAvailable,        ///< [in] indicate if eddy current brakes are available
                       bool            bEpBrakeAvailable,          ///< [in] indicate if electro-pneumatic brakes are available
                       bool            bAddBrakeAvailable,         ///< [in] indicate if additional brakes are available
                       bool            bTractionOn,                ///< [in] indicate if the traction is set
                       bool            bSystFailure );             ///< [in] indicate if there is a system failure

    /// Set indication if ETCS power is on
    /// @return 0 on success
    int32_t SetMainSwitchOn            ( bool bValue );

    /// Set indication if train is integer
    /// @return 0 on success
    int32_t SetTrainIntegrityOK        ( bool bValue );

    /// Set indication of the opened cabin
    /// @return 0 on success
    int32_t SetActiveCabin             ( eActiveCabin eValue );

    /// Set indication if isolation of ETCS is requested
    /// @return 0 on success
    int32_t SetIsolation               ( bool bValue );

    /// Set indication if sleeping signal is set
    /// @return 0 on success
    int32_t SetSleeping                ( bool bValue );

    /// Set indication of the position of the direction controller
    /// @return 0 on success
    int32_t SetDirectionCtrl           ( eDirection eValue );

    /// Set indication if the emergency brake is applied
    /// @return 0 on success
    int32_t SetEBStatus                ( bool bValue );

    /// Set indication if the service brake is applied
    /// @return 0 on success
    int32_t SetSBStatus                ( bool bValue );

    /// Set indication if non-leading signal is set (only in B3)
    /// @return 0 on success
    int32_t SetNonLeading              ( bool bValue );

    /// Set indication if passive shunting signal is set (only in B3)
    /// @return 0 on success
    int32_t SetPassiveSH               ( bool bValue );

    /// Set indication if a cold movement has been detected (only in B3)
    /// @return 0 on success
    int32_t SetColdMvtDetection        ( bool bValue );

    /// Set indication of the type of train data entry (only in B3)
    /// @return 0 on success
    int32_t SetTrainDataType           ( eTrainDataType lValue );

    /// Set indication if regenerative brakes are available (only in B3)
    /// @return 0 on success
    int32_t SetRegenBrakeAvailability  ( bool bValue );

    /// Set indication if magnetic shoe brakes are available (only in B3)
    /// @return 0 on success
    int32_t SetMagneticBrakeAvailability(bool bValue );

    /// Set indication if eddy current brakes are available (only in B3)
    /// @return 0 on success
    int32_t SetEddyBrakeAvailability   ( bool bValue );

    /// Set indication if electro-pneumatic brakes are available (only in B3)
    /// @return 0 on success
    int32_t SetEpBrakeAvailability     ( bool bValue );

    /// Set indication if additional brakes are available (only in B3)
    /// @return 0 on success
    int32_t SetAddBrakeAvailability    ( bool bValue );

    /// Set indication if the traction is set (only in B3)
    /// @return 0 on success
    int32_t SetTractionOn              ( bool bValue );

    /// Set indication if there is a system failure (only in B3)
    /// @return 0 on success
    int32_t SetSystFailure             ( bool bValue );

    /// Set the pneumatic brake pressure
    /// @return 0 on success
    int32_t SetBrakePressure       ( double dBrakePressure );

    /// Set position of the traction/brake handle position in % (-100 to +100)
    /// @return 0 on success
    int32_t SetTractBrakeHandlePos ( int32_t lTractBrakeHandlePos );

    /// Send TIU drive request
    /// @return 0 on success
    int32_t Send_TIUDriver_request( t_TIUREQUEST Request );

    /// Send NTC isolation request (only in B3)
    /// @return 0 on success
    int32_t SendNtcIsolation( int32_t lNtcId,     ///< NTC identifier
                              bool bIsolated   ///< indicator of isolation
                              );

private:
    bool    m_bInit_ok;
    bool    m_bNoRefresh;

    // memorizes the parameters between TIU data setting function calls
    SRxTiuData m_TrainTIU;
    STxTiuData m_EvcTIU;

    SComParam   m_Odo_Com;
    SComParam   m_TIU_Com;

    /// Initialise TIU data
    void Init( void );
};



#endif


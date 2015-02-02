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

// *************************************************************************************************

/// @file   evc_com.h
/// @brief  Declaration of functions for interface with eurocab simulator.
/// Project     : EVC Simulator -
/// Module      : EVC -
// *************************************************************************************************

#ifndef _EVC_COM_H
#define _EVC_COM_H

/*************************************************************************************************
 *  Includes
 *************************************************************************************************/
#include <vector>
#include <string>
#include <inttypes.h>

#include "etcs_types.h"
#include "etcs_config.h"

/*************************************************************************************************
 *  Forward declarations
 *************************************************************************************************/

class CEVC_Sim;
class CBalise_com;
class CDMI_com;
class CJru_com;
class COdo_com;
class CRadio_com;

/*************************************************************************************************
 *  Typedefs and structure declarations
 *************************************************************************************************/

/// Structure containing variable and its value (used to indicate driver data entry on DMI)
typedef struct SVarData
{
    std::string VarName; ///< Name of data
    std::string VarValue; ///< value of data
} SVarData;

/// List of data entered
typedef std::vector< SVarData > SVarDataList;

/// Structure containing information about a track condition
typedef struct STCInfo
{
    t_distance   dStartLocation;    ///< start location of TC (m)
    t_distance   dEndLocation; ///< end location of TC (m)
    eTCInfo      TCInfo;  ///< type of TC
    SVarDataList VarList; ///< data associated to the TC
} STCInfo;

typedef std::vector< t_distance >    SDistanceList;
typedef std::vector< STCInfo >       STCInfoList;
typedef std::vector< SDmiActionLog > SDmiActionInfoList;
typedef std::vector< SIHMData >      SIhmMessageList;

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

typedef struct SDmiComTextMsg
{
    uint32_t     ulId;
    bool         bAck;
    SDMI_TextMsg data;
} SDmiComTextMsg;

typedef struct SDmiComIcon
{
    uint32_t ulId;
    bool     bAck;
    uint32_t lIconNameLength;
    char     szIconName[ 8 ];
    int32_t  lIconGroup;
    int32_t  lIconRank;
    uint32_t lAreaNameLength;
    char     szAreaName[ 8 ];
    int32_t  lAreaGroup;
    int32_t  lAreaRank;
} SDmiComIcon;

typedef struct SDmiComDynamic
{
    uint32_t DMI_T_CLOCK;
    uint16_t DMI_V_TRAIN : 10;
    char     DMI_X_VTRAIN_DIGITS[ 3 ];
    uint32_t DMI_O_TRAIN;
    uint32_t DMI_O_BRAKETARGET;
    char     DMI_X_OBRAKETARGET_DIGITS[ 5 ];
    uint16_t DMI_V_TARGET       : 10;
    uint16_t DMI_V_PERMITTED    : 10;
    uint16_t DMI_V_RELEASE      : 10;
    uint32_t DMI_O_BCSP;
    uint16_t DMI_V_INTERVENTION : 10;
    uint8_t  DMI_M_MODE         : 4;
    uint8_t  DMI_M_LEVEL        : 3;
    uint8_t  DMI_NID_STM;
    uint16_t DMI_NID_C          : 10;
    bool     DMI_NID_C_UNKNOWN  : 1;
    uint8_t  DMI_M_WARNING      : 4;
    uint8_t  DMI_M_SUPSTATUS    : 4;
    uint32_t DMI_O_LOA;
    uint16_t DMI_V_LOA          : 10;
    uint32_t DMI_O_KP_BALISE_TRACK_KILOMETER;
    uint32_t DMI_O_KP_DIST_TO_BALISE;
    uint8_t  DMI_M_KP_FLAG : 2;
    uint32_t DMI_O_DIST_TO_TSA;
} SDmiComDynamic;

typedef enum eDmiAction
{
    DMI_DO_TRAIN_RUNNING_NUMBER,
    DMI_DO_TRAIN_DATA,         // Default train data
    DMI_DO_TRAIN_DATA_FREIGHT, // Train data with freight (FP1) train category selected
    DMI_DO_SR_DATA,
    DMI_DO_LEVEL_0,
    DMI_DO_LEVEL_1,
    DMI_DO_LEVEL_2,
    DMI_DO_LEVEL_3,
    DMI_DO_LEVEL_STM,
    DMI_DO_START_OF_MISSION,
    DMI_DO_SLIPPERY_TRACK,
    DMI_DO_NON_SLIPPERY_TRACK,
    DMI_DO_NON_LEADING,
    DMI_DO_NON_LEADING_EXIT, // only B2
    DMI_DO_OVERRIDE_EOA,
    DMI_DO_OVERRIDE_RS, // only B2
    DMI_DO_SHUNTING,
    DMI_DO_SHUNTING_EXIT,
    DMI_DO_TAF_ACK,
    DMI_DO_ACK_MODE_OR_LEVEL,
    DMI_DO_ACK_TEXT_MSG,
    DMI_DO_ACK_BRAKE,
    DMI_DO_CONFIRM_INTEGRITY,
    DMI_DO_USE_SHORT_NUMBER,      // only B3
    DMI_DO_CONTACT_LAST_RBC,      // only B3
    DMI_DO_SELECT_RADIO_NETWORK1, // only B3
    DMI_DO_ENTER_VBC,             // only B3
    DMI_DO_REMOVE_VBC,

    // DMI_DO_ENTER_NETWORK, ?
    DMI_DO_ENTER_RBC_DATA,    // only B3
    DMI_DO_MAINTAIN_SHUNTING, // only B3
    DMI_DO_ISOLATION,         // only B3
    DMI_DO_CHANGE_TRAIN_LENGTH,
    DMI_DO_NUM
} eDmiAction;

/*************************************************************************************************
 *  Class declarations
 *************************************************************************************************/

class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

CEvc_com
{
public:

#ifndef OPEN_API // Do not expose unnedded struct
    explicit CEvc_com( bool bDMI = false );

#else
    explicit CEvc_com();
#endif

    virtual ~CEvc_com();

    /*************************************************************************************************
     *  SIM functions
     *************************************************************************************************/

    /// Modify the EVC configuration.
    /// It should be call just after constructor
    void SIM_Modify_EVC_Configuration( bool bSet, ///< [in] indicate if the config should be set or reset
                                       eConfigData eConfig ///< [in] flag to identify the configuration
                                       );

    // --------------------------------------------------------------
    // METHODS TO CONTROL EVC SIMULATION
    // --------------------------------------------------------------

    /// Initialise the EVC simulator: creation of EVC data structure, initialisation of interfaces
    /// @return  0 on success
    int32_t SIM_Init( uint32_t ulLogId ///< [in] key used as prefix for log files
                      );

    /// Start the EVC simulator modules
    /// @return 0 on success
    int32_t SIM_Start_processes( void );

    /// Start the EVC simulation
    /// @return 0 on success
    int32_t SIM_Run( void );

    /// Stop the EVC simulation
    /// @return 0 on success
    int32_t SIM_Stop( void );

    /// check if DMI is connected and version of communication protocol is compatible
    /// it should be called after Start_processes
    /// @return true is communication with DMI is working
    bool SIM_IsDMIReady( void );

    // --------------------------------------------------------------
    // METHODS TO CONFIFURE EVC SIMULATION (STARTING CONDITIONS)
    // --------------------------------------------------------------

    /// set ETCS identity
    void SIM_SetETCSID( uint32_t ulETCSId ///< [in] value of ETCS ID / NID_ENGINE (stored on 24 bits)
                        );

    /// Set the stored RBC short number
    void SIM_SetEirenePhoneNumber( const char* szShortNb ///< [in] RBC short number, up to 16 digits (ie: "05670454")
                                   );

    /// set the offsets of balise antennas
    void SIM_SetBaliseAntennaOffsets( t_distance dBalAntennaOffsetCabA, ///< [in] offset between balise antenna and train front when cabin A is used
                                      t_distance dBalAntennaOffsetCabB ///< [in] offset between balise antenna and train front when cabin B is used (for future)
                                      );

    /// Set the available train equipment
    void SIM_SetTrainEquipment( bool bBaliseComAvailable, ///< [in]    indicate if the balise communication is available
                                bool bLoopComAvailable, ///< [in]    indicate if the loop communication is available
                                int32_t lNbRadioSessionAvailable, ///< [in]    indicate number of available radio equipements (0, 1 or 2)
                                bool bIntegrityDeviceAvailable, ///< [in]    indicate if the integrity check device is available
                                bool bServiceBrakeAvailable, ///< [in]    indicate if the service brake interface is available
                                bool bTCOAvailable, ///< [in]    indicate if the traction cut off is available
                                bool bBrakeFeedBackAvailable, ///< [in]    indicate if the brake feedback information is available
                                bool bAirtighAvailable, ///< [in]    indicate if the airtight control is available
                                bool bColdMvtDetectorAvailable, ///< [in]    indicate if the cold movement detector is available
                                const char* szPhoneNb1 = NULL, ///< [in]    value of first train phone number,  up to 16 digits (ie: "05670454")
                                const char* szPhoneNb2 = NULL ///< [in]    value of second train phone number, up to 16 digits (ie: "05670454")
                                );

    /// Set one Emergency Brake deceleration model with its parameters Kdry and Kwet and corresponding brakes configuration
    void SIM_SetEBModel( SEBModelParam EBModelParam, ///< [in] : Emergency Brake deceleration model
                         bool bDefault = false ///< [in] : indicates if model must be applied to all brake configurations
                         );

    /// Set one Service Brake deceleration model with its corresponding brakes configuration
    void SIM_SetSBModel( SSBModelParam SBModelParam, ///< [in] : Service Brake deceleration model
                         bool bDefault = false ///< [in] : indicates if model must be applied to all brake configurations
                         );

    /// Set the on-board correction factors for gradient on normal service deceleration
    void SIM_SetFactorsKn( SKn Kn_p, ///< [in] Correction factor for positive gradient
                           SKn Kn_n ///< [in] Correction factor for negative gradient
                           );

    /// Set the normal service brake decceleration model according to brake position
    void SIM_SetNormSBModel( bool bBrakePosInP, ///< [in] indicate if model apply to train in P
                             t_accel dA_SB01, ///< [in] deceleration of service brake used as first pivot value
                             t_accel dA_SB12, ///< [in] deceleration of service brake used as second pivot value
                             SBrkModel * pSB_Model0, ///< [in] normal SB model applicable under SB deceleration dA_SB01
                             SBrkModel * pSB_Model1, ///< [in] normal SB model applicable between SB deceleration dA_SB01 and dA_SB12
                             SBrkModel * pSB_Model2 ///< [in] normal SB model applicable above SB deceleration dA_SB12
                             );

    /// set the traction model
    void SIM_SetTractionModel( t_accel dMaxAccel, ///< [in] maximum train acceleration (m/s²)
                               t_time dTractionCutOffTime ///< [in] time to cut off the traction (s)
                               );

    /// Set the odometry error model (to simulate inaccuracy in odometry)
    void SIM_SetOdoError( t_distance dOdoLinErrcoeff, ///< [in]    linear error (in metre)
                          t_distance dOdoFixedError, ///< [in]    fixed error (in metre)
                          double dUnderErrCoeff, ///< [in]    coefficient for the under-reading error (in %)
                          double dOverErrCoeff ///< [in]    coefficient for the over-reading error (in %)
                          );

    /// Set the stored position data (starting condition)
    void SIM_SetStoredTrainPosition( eValidity PosVal, ///< [in] position data validity
                                     uint32_t ulNID_LRBG, ///< [in] identifier of the LRBG
                                     int32_t lLRBGDistance, ///< [in] distance between train front (cab A) and LRBG
                                     eDirection LRBGDir ///< [in] orientation of LRBG
                                     );

    /// Set stored level data (starting condition)
    void SIM_SetStoredLevel( eValidity Validity, ///< [in] validity of level data
                             SLevel Level ///< [in] stored ETCS level
                             );

    /// Set stored radio network id
    void SIM_SetStoredRadioNetwork( const char* szRadNetId ///< [in] radio network id, up to 6 digits (ie: "02054" )
                                    );

    /// Set stored RBC data
    void SIM_SetStoredRbcData( eValidity Validity, ///< [in] validity of RBC data
                               uint32_t ulRBCId, ///< [in] RBC identity (value in 24 bits)
                               const char* szPhoneNb ///< [in] RBC phone nb, up to 16 digits (ie: "05670454")
                               );

    /// Set the stored national values from binay message
    /// @return 0 on success (valid binary message)
    int32_t SIM_SetStoredNationalData( const uint8_t * pcBinaryNationalValues, ///< [in] binary frame of general radio message (24) containing national values packet(s)
                                       int32_t iBinaryLength ///< [in] lengh in bytes of binary frame
                                       );

    /// Set the stored national values
    void SIM_SetStoredNationalData( uint32_t ulNbCountryId, ///< [in] nb country for the national values
                                    uint32_t * ulCountriesId, ///< [in] countries for the national values
                                    int32_t lSHSpeed, ///< [in] permitted speed in shunting (km/h)
                                    int32_t lSRSpeed, ///< [in] permitted speed in staff responsible (km/h)
                                    int32_t lOSSpeed, ///< [in] permitted speed in on sigh (km/h)
                                    int32_t lLSSpeed, ///< [in] permitted speed in limited supervision (km/h)
                                    int32_t lUNSpeed, ///< [in] permitted speed in unfitted (km/h)
                                    int32_t lReleaseSpeed, ///< [in] release speed (km/h)
                                    int32_t lRollAwayDist, ///< [in] roll away distance (m)
                                    bool bUseSBforTarget, ///< [in] indicate if the service brake can be used when braking to a target
                                    bool bReleaseEB, ///< [in] indicate if emergency brake can be release when not at standstill
                                    int32_t lEntryOVSpeed, ///< [in] maximum speed to select override (km/h)
                                    int32_t lOVSpeed, ///< [in] permitted speed in override (km/h)
                                    int32_t lMaxOVDistance, ///< [in] maximum distance in override (m)
                                    int32_t lMaxOVTime, ///< [in] maximum time in override (m)
                                    bool bDriverIdEntryInRun, ///< [in] indicate if the driver can enter the train running number when the train is running
                                    eReaction ContactReaction, ///< [in] supervision of radio link: reaction to apply
                                    int32_t lContactTime, ///< [in] supervision of radio link: maximum time between two radio message (s)
                                    int32_t lMaxPTDistance, ///< [in] maximum distance to reverse in post trip (m)
                                    int32_t lMaxSRDistance, ///< [in] maximum distance in staff responsible (m)
                                    bool bDriverAdhesion, ///< [in] indicate if the driver is allowed to to modify the adhesion
                                    bool bUseGuidanceCurve, ///< [in] Use Guidance Curve in supervision curve calculation
                                    bool bUseServiceBrakeFeedback, ///< [in] use brake feedback in supervision curve calculation
                                    t_accel dReducAcc1, ///< [in] TBD
                                    t_accel dReducAcc2, ///< [in] TBD
                                    t_accel dReducAcc3, ///< [in] TBD
                                    double dKv_int, ///< [in] TBD
                                    double dKr_int, ///< [in] TBD
                                    double dKt_int, ///< [in] TBD
                                    double dBGAccuracy, ///< [in] TBD
                                    double dEBConfidenceLevel, ///< [in] TBD
                                    double dWheelRailAdhesion, ///< [in] TBD
                                    bool bInhibSpeedInaccuracy ///< [in] TBD
                                    );

    /// calculate the B3 train category from Brake position
    /// @return B2 train category
    static int32_t SIM_GetB3TrainCategory( eBrakePosition BrakePosition ///< [in] brake position (R, P, G)
                                           );

    /// Set the default train data in B3 format, will be converted in B2 format if necessary
    void SIM_SetB3DefaultTrainData( t_distance dLength, ///< [in] length of train (in metre)
                                    t_speed dSpeedLimit, ///< [in] maximum train speed limit(m/s)
                                    int32_t lTrainCategory, ///< [in] B3 train category calculated from Brake position
                                    t_distance dCantDeficiency, ///< [in] train cant deficiency (m)
                                    int32_t lLoadingGauge, ///< [in] train loading gauge
                                    eAxleLoadCategory AxleloadCategory, ///< [in] train axle category
                                    int32_t lAxleNumber, ///< [in] train axle number
                                    int32_t lBrakePercentage ///< [in] train percentage
                                    );

    /// Set the list of available traction system
    void SIM_SetAvailableTractionSystem( int32_t lNbTractionPowers, ///< [in] number of available traction system
                                         int32_t * alTractionPowers, ///< [in] list of traction system identifier (B2 M_TRACTION)
                                         int32_t * alTractionVoltage, ///< [in] list of traction voltage (B3 M_VOLTAGE)
                                         int32_t * alTractionCountry ///< [in] list of traction country (B3 NID_C_TRACTION)
                                         );

    // --------------------------------------------------------------
    // METHODS TO SET EVENT DURING SIMULATION
    // --------------------------------------------------------------

    /// set/reset EVC isolation (only in B3)
    void SIM_SetIsolation( bool bIsolated ///< [in] new isolation status to set
                           );

    /// Set or reset BTM alarm
    void SIM_SetBTMAlarm( bool bSwitchOn ///< [in] Set BTM alarm
                          );

    // --------------------------------------------------------------
    // METHODS TO GET INTERNAL DATA DURING SIMULATION
    // --------------------------------------------------------------

    /// Get current on-board ETCS level
    eLevel SIM_GetCurrentLevel( void );

    /// Get current on-board ETCS mode
    eTrainMode SIM_GetCurrentMode( void );

    /// Return estimated front loc of the train (m)
    t_distance SIM_GetEstimatedFrontLoc( void );

    /// Get current train speed (km/h)
    double SIM_GetTrainSpeed( void );

    /// Get EOA location
    /// @return EOA location or 0 if not applicable
    t_distance SIM_GetEoaLocation( void );

    /// Get EOA speed
    /// @return EOA speed (km/h) or 0 if not applicable
    double SIM_GetEoaSpeed( void );

    /// Get release speed
    /// @return     Release speed (km/h) or 0 if not applicable
    double SIM_GetReleaseSpeed( void );

    /// Get current Emergency brake intervention speed (km/h)
    double SIM_GetEBrakeSpeed( void );

    /// Get current service brake intervention speed (km/h)
    double SIM_GetSBrakeSpeed( void );

    /// Get current permitted speed (km/h)
    double SIM_GetPermSpeed( void );

    /// Get current warning speed (km/h)
    double SIM_GetWarnSpeed( void );

    /// Get current speed monitoring status (CSM,PIM,TSM,RSM,UNKNOWN)
    eSpeedMonitoringStatus SIM_GetSpeedMonitoringStatus( void );

    /// Get next target location
    /// @return target location (m) or 0 if not applicable
    t_distance SIM_GetTargetLocation( void );

    /// Get next target speed
    /// @return target speed (km/h) or 0 if not applicable
    double SIM_GetTargetSpeed( void );

    /// Get safe connection status for indicated equipment (0 or 1)
    /// @return safe connection status (RELEASED, REQUESTED, ESTABLISHED)
    eRadSafeConnectStatus SIM_GetSafeConnStatus( int32_t lEquipmentIdx );

    /// Get session status for indicated equipment (0 or 1)
    /// @return session status (RELEASED, INITIALISED, ESTABLISHED, TERMINATED)
    eRadSessionState SIM_GetSessionStatus( int32_t lEquipmentIdx );

    /// Get current adhesion factor (70 or 100)
    int32_t SIM_GetAdhesionFactor( void );

    /// Get current most restrictive speed (from MRSP) (km/h)
    double SIM_GetMostRestrictiveSpeed( void );

    bool   SIM_GetTCProfile( STCInfoList & rTCInfoList,
                             int32_t & rlCounter );

    bool SIM_GetDriverAction( t_distance & rdLocation,
                              t_time & rdTimeStamp,
                              eDriverActionInfo & rDriverAction,
                              SVarDataList & rVarDataList );

    eBrakePosition SIM_GetBrakePosition( void );

    double         SIM_GetBaliseAntennaOffset( void );

    bool           SIM_GetIhmMessages( char* szMessage,
                                       eMSG_TYPE * type );

    void SIM_GetTrackConditions( SProfile * pGradProfile,
                                 SProfile * pSpeedProfile,
                                 STrackCondTrigger * pTrackCondTriggers,
                                 STrackCondTrigger * pBigMetalMasses,
                                 STractionChangeList * pTractionChangeList );

    bool SIM_GetTrackCondStationPlatform( t_distance * dDistance,
                                          bool* bResetProfile,
                                          int32_t * lNbStation,
                                          SCondStation * aCondStation );

    bool SIM_GetTrackCondAllowedConsumption( t_distance * dDistance,
                                             int32_t * lNewAllowedConsumption );

    // --------------------------------------------------------------
    // METHODS TO SET FIXED DATA (TO REMOVE AS FIXED BY SRS)
    // --------------------------------------------------------------

    // for test only
    void SIM_SetFixedDataSafeConnectionRepeatTimeout( t_time dSafeConnectionRepeatTimeout );
    void SIM_SetFixedDataMaxSessionMaintainTime( t_time dMaxSessionMaintainTime );

    // -------------------------------------------------------------------------------------------------
    /// Convert traction value (baseline 2) to country voltage (baseline 3)
    /// @SRS 6.6.3.2.3 [13]
    bool SIM_TractionToCountryVoltage( int32_t lTraction, STractionParam * p );

    /*************************************************************************************************
     *  BAL functions
     *************************************************************************************************/

    /// Send a balise message to EVC
    /// @return: 0 if message is sent correctly
    int32_t BAL_Send_Balise( const int32_t iMsgLength,        ///< [in]: Message length in bytes
                             const uint8_t * const uszMsg,    ///< [in]: Message (binary stream)
                             const double dBaliseLocation = 0 ///< [in]: balise location (optional, used if config CFG_BAL_WITH_ODO_STAMP is set)
                             );

    /// Send a loop message to EVC
    /// @return: 0 if message is sent correctly
    int32_t BAL_Send_Loop( const int32_t iMsgLength,                     ///< [in]: Message length in bytes
                           const uint8_t * const uszMsg,                 ///< [in]: Message (binary stream)
                           const int32_t lSSCode = IGNORE_Q_SSCODE_VALUE ///< [in]: spread spectrum code of loop = Q_SSCODE (optional)
                           );

    /*************************************************************************************************
     *  ODO functions
     *************************************************************************************************/

    /// Send a odometric data to EVC
    /// @return: 0 if message is sent correctly
    int32_t ODO_Send_Odo_data( t_distance dLocation_m, ///< [in] absolute train location in meter (incremental when moving with cabin A first)
                               t_speed dSpeed_m_s,     ///< [in] absolute train speed in m/s (positive when moving with cabin A first)
                               t_accel dGamma_m_s2     ///< [in] absolute train acceleration in m/s2
                               );

    /// Refresh TIU from EVC
    /// @return -1 on error
    /// @return  0 when there is no TIU change
    /// @return  1 when there is a TIU change
    int32_t ODO_RefreshTIUData( void );

    /// Get the TIU from train
    /// @return TRAIN TIU
    SRxTiuData ODO_getTrainTiu();

    /// Get the TIU from EVC
    /// @return EVC TIU
    STxTiuData ODO_getEvcTiu();

    /// Get TIU data from EVC
    /// @return -1 on error
    /// @return  0 when there is no TIU change
    /// @return  1 when there is a TIU change
    int32_t ODO_GetEvcTIU( bool& bEBRqst,                          ///< [out] emergency brake request
                           bool& bSBRqst,                          ///< [out] service brake request
                           bool& bTractionCutOffRqst,              ///< [out] traction cut-off request
                           bool& bOpenCircuitBreakerRqst,          ///< [out] open MCB request
                           bool& bPantographLow,                   ///< [out] pantograph low request
                           bool& bInhibitPEB,                      ///< [out] inhibition of passenger emergency brake
                           bool& bAirTightRqst,                    ///< [out] request air tightness
                           bool& bPermitRegenBrake,                ///< [out] permission to use regenerative brakes
                           bool& bPermitMagnShoeBrake,             ///< [out] permission to use magnetic shoe brakes
                           bool& bPermitEddyCurrBrakeForSB,        ///< [out] permission to use eddy current brakes for service brake
                           bool& bPermitEddyCurrBrakeForEB,        ///< [out] permission to use eddy current brakes for emergency brake
                           bool& bIgnoreBTMAlarm,                  ///< [out] indicate the BTM alarm should be ignored (big metal masses)
                           int32_t & lAllowedConsumption,          ///< [out] indicate the allowed current consumption (Ampere)
                           bool& bIsolationStatus,                 ///< [out] indicate the status of isolation (in B3)
                           ePlatformElevation & PlatformElevation, ///< [out] indicate the platform elevation (in B3)
                           ePlatformPosition & PlatformPosition    ///< [out] indicate the platform side (in B3)
                           );

    /// Get the EVC indication of allowed current consumption (only in B3)
    /// @return the allowed current consumption (Ampere)
    int32_t ODO_GetAllowedConsumption( void );

    /// Set indication if the emergency brake is applied
    /// @return 0 on success
    int32_t ODO_SetEBStatus( bool bValue );

    /// Set indication if the service brake is applied
    /// @return 0 on success
    int32_t ODO_SetSBStatus( bool bValue );

    /// Set indication of the type of train data entry (only in B3)
    /// @return 0 on success
    int32_t ODO_SetTrainDataType( eTrainDataType lValue );

    /// Set the pneumatic brake pressure
    /// @return 0 on success
    int32_t ODO_SetBrakePressure( double dBrakePressure ); ///< [in] pressure

    /// Send TIU drive request
    /// @return 0 on success
    int32_t ODO_Send_TIUDriver_request( t_TIUREQUEST Request );

    /*************************************************************************************************
     *  RAD functions
     *************************************************************************************************/

    /// Send a radio message to EVC via the first radio equipment
    /// @return: 0 if message is sent correctly
    int32_t RAD_Send_Radio_Msg1( int32_t iMsgLength, ///< [in]: Message length in bytes
                                 uint8_t * uszMsg ///< [in]: Message (binary stream)
                                 );

    /// Send a radio message to EVC via the second radio equipment
    /// @return: 0 if message is sent correctly
    int32_t RAD_Send_Radio_Msg2( int32_t iMsgLength, ///< [in]: Message length in bytes
                                 uint8_t * uszMsg ///< [in]: Message (binary stream)
                                 );

    /// Receive a radio message from the first EVC radio equipement
    /// @return Size of the received message in bytes (0 if there is no message)
    int32_t RAD_Receive_Radio_Msg1( uint8_t * uszMsg, ///< [out] : pointer on the buffer to store the received data
                                    int32_t iMaxLength, ///< [in]  : maximum size of the buffer to store the received data
                                    bool bWait ///< [in]  : (DEPRECATED)qualificator indicating if the function should block until data are received
                                    );

    /// Receive a radio message from the second EVC radio equipement
    /// @return Size of the received message in bytes (0 if there is no message)
    int32_t RAD_Receive_Radio_Msg2( uint8_t * uszMsg, ///< [out] : pointer on the buffer to store the received data
                                    int32_t iMaxLength, ///< [in]  : maximum size of the buffer to store the received data
                                    bool bWait ///< [in]  : (DEPRECATED)qualificator indicating if the function should block until data are received
                                    );

    /*************************************************************************************************
     *  DMI functions
     *************************************************************************************************/

    /// Retrieve available menu mask, and set it to 0 to get a new value for next call
    /// @return mask indicating the status of GUI update
    int32_t DMI_getAndResetUpdateMask();

    /// Send a message to EVC to simulate a driver action on the DMI
    /// @return true if this action is allowed (button available)
    bool DMI_setAction( eDmiAction action,                              ///< [in] Type of driver action on DMI
                        int32_t param = -1,                             ///< [in] optional parameter: \n
                                                                        ///< - driver id for DMI_DO_DRIVER_ID \n
                                                                        ///< - train running number for DMI_DO_TRAIN_RUNNING_NUMBER \n
                                                                        ///< - STM Id for DMI_DO_LEVEL_STM \n
                                                                        ///< - message id for DMI_DO_ACK_TEXT_MSG \n
                                                                        ///< - VBC code for DMI_DO_ENTER_VBC and DMI_DO_REMOVE_VBC
                        struct SMMISRData* const pStaffRespData = NULL, ///< [in] Used for DMI_DO_SR_DATA action (optional)
                        struct SMMIRBCData* const pRbcData = NULL,      ///< [in] Used for DMI_DO_LEVEL_2 and DMI_DO_LEVEL_3 action (optional)
                        struct SMMITrainData* const pTrainData = NULL   ///< [in] Used for DMI_DO_TRAIN_DATA action (optional)
                        );

    /// Send a message to EVC to simulate entering driver id on the DMI
    /// @return true if this action is allowed (button available)
    bool DMI_doDriverIdAction( const char* const szDriverId );

    /// Send a message to EVC to simulate entering RBC data on the DMI
    /// @return true if this action is allowed (button available)
    bool DMI_doRBCDataAction( int32_t lRBCId,
                              const char* const szPhoneNr,
                              const char* const szNetworkId );

    /// Indicate if a text message to acknowledge is displayed
    /// @return true if a message to acknowledge is displayed
    bool DMI_IsTextMessageAckDisplayed() const;

    /// Indicate if a mode or level icon to acknowledge is displayed
    /// @return true if a mode or level icon to acknowledge is displayed
    bool DMI_IsModeOrLevelAckDisplayed() const;

    /// Indicate if a brake icon to acknowledge is displayed
    /// @return true if a brake icon to acknowledge is displayed
    bool DMI_IsBrakeAckDisplayed() const;

    /// test is cabin is opened
    /// @return true if cabin is open
    bool DMI_getCabinStatus() const;

    /// test if track ahead free is displayed
    /// @return true if TAF is displayed
    bool DMI_getTafStatus() const;

    /// get the mask of available menu/button
    /// @return the mask of available menu/button
    uint32_t DMI_getAvailableMenu() const;

    /// get list of displayed icons
    void DMI_getIconList( std::vector< SDmiComIcon > &icons ) const;

    /// get list of displayed text messages
    void DMI_getTextMessageList( std::vector< SDmiComTextMsg > &texts ) const;

    /// get list of active levels
    void DMI_getLevelList( std::vector< SLevel > &levels );

    /// get dynamic packet
    const SDmiComDynamic &DMI_getDynamicPacket() const;

    /*************************************************************************************************
     *  Accessor functions
     *************************************************************************************************/
    inline CEVC_Sim*    getEVC_Sim()    { return( m_pCEVC_Sim ); }
    inline CBalise_com* getBalise_com() { return( m_pBalise_com ); }
    inline CDMI_com*    getDMI_com()    { return( m_pDMI_com ); }
    inline CJru_com*    getJru_com()    { return( m_pJru_com ); }
    inline COdo_com*    getOdo_com()    { return( m_pOdo_com ); }
    inline CRadio_com*  getRad_com()    { return( m_pRad_com ); }

private:

    CEVC_Sim*    m_pCEVC_Sim;
    CBalise_com* m_pBalise_com;
    CDMI_com*    m_pDMI_com;
    CJru_com*    m_pJru_com;
    COdo_com*    m_pOdo_com;
    CRadio_com*  m_pRad_com;
};
#endif // ifndef _EVC_COM_H

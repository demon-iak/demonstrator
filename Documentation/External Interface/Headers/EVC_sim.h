//*************************************************************************************************
/// @file   EVC_sim.h
/// @brief  Declaration of functions for interface with eurocab simulator.
/// Project     : EVC Simulator -
/// Module      : EVC -
/// Copyright   : European Rail Software Applications (c) 2014
//*************************************************************************************************

#ifndef _EVC_SIM_H
#define _EVC_SIM_H

//-------------------------------------------------------------------------------------------------
//                                include
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>

#include "etcs_types.h"

//-------------------------------------------------------------------------------------------------
//                                define & structure
//-------------------------------------------------------------------------------------------------

// time out for waiting for command ack
//#define CMD_TIME_OUT    5        // in second

/// Structure to store a point of a supervision curve
typedef struct SPoint_tag
{
    t_distance  Location    ;   ///< Location of point (m)
    t_speed     Speed       ;   ///< Speed of point (m/s)
} SPoint;

/// Structure containing the supervision curves
typedef struct SInterventionCurves_tag
{
    t_distance     RefPos              ;   ///< Reference position of curves data (m)
    std::vector<SPoint> PermittedSpeedCurve ;   ///< Permitted speed supervision curve
    std::vector<SPoint> IndicationSpeedCurve;   ///< Indication speed supervision curve
    std::vector<SPoint> WarningSpeedCurve   ;   ///< Warning speed supervision curve
    std::vector<SPoint> FLOISpeedCurve       ;   ///< SBI speed supervision curve
    std::vector<SPoint> EBISpeedCurve       ;   ///< EBI speed supervision curve
} SInterventionCurves;

/// Structure containing variable and its value (used to indicate driver data entry on DMI)
typedef struct SVarData
{
    std::string VarName  ;   ///< Name of data
    std::string VarValue ;   ///< value of data
} SVarData;

/// List of data entered
typedef std::vector<SVarData> SVarDataList;

/// Structure containing information about a track condition
typedef struct STCInfo
{
    t_distance      dStartLocation; ///< start location of TC (m)
    t_distance      dEndLocation;   ///< end location of TC (m)
    eTCInfo         TCInfo;         ///< type of TC
    SVarDataList    VarList;        ///< data associated to the TC

} STCInfo;

typedef std::vector<t_distance> SDistanceList;
typedef std::vector<STCInfo> STCInfoList;
typedef std::vector<SDmiActionLog> SDmiActionInfoList;
typedef std::vector<SIHMData> SIhmMessageList;

//--------------------------- class definition -----------------------------

/// Class for control of EuroCab simulator.
/// Methods for configuration, start, stop of simulator

class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

CEVC_Sim
{

public:

    //--------------------------------------------------------------
    //      CONSTRUCTOR / DESTRUCTOR
    //--------------------------------------------------------------

    /// Contructor
    CEVC_Sim (void);

    /// Destructor
    ~CEVC_Sim ( );

    //--------------------------------------------------------------
    //      METHODS TO CHECK CONSISTENCY AND BASIC CONFIGURATION
    //--------------------------------------------------------------


    /// Get the class size (for linking consistency check in use of FULL/LIGHT EVC version)
    /// @return: class size in bytes
    int32_t Get_Class_Size          (                                                   );

    /// Check the max size of processed MA
    /// @return true on success, otherwise false
    bool    CheckMAMaxSize          ( int32_t lMaxSize                    ///< [in] Maximum length of MA to be tested (MA_MAX_SIZE or MA_REDUCED_SIZE)
                                      );

    /// Modify the EVC configuration.
    /// It should be call just after constructor
    void    Modify_EVC_Configuration(   bool                bSet                        , ///< [in] indicate if the config should be set or reset
                                        eConfigData         Config                        ///< [in] flag to identify the configuration
                                        );

    /// Set parameters for cyclic recording of supervision data in CSV formatted file
    /// @return     0 on success, -1 on failure
    int32_t SetCSVRecordingParameters(  t_time              dDeltaTime                  , ///< [in] maximum time margin (s) to record data
                                        t_distance          dDeltaDistance              , ///< [in] maximum distance margin (m) to record data
                                        t_speed             dDeltaSpeed                 , ///< [in] maximum speed margin (m/s) to record data
                                        char                cDecimalSymbol              , ///< [in] decimal symbol to used in CSV file creation
                                        char                cListSeparator              , ///< [in] symbol to use as colum separator in CSV file
                                        char *              szFileName                    ///< [in] CSV file name
                                        );

    /// Save the current supervision curves in a CSV formatted file
    /// @return     true on success, false on failure
    bool    SaveCurvesToCSVFile     (   char *                  szCSVFileName           , ///< [in]    file name
                                        char *                  szDecimalSymbol         , ///< [in]    decimal separator to be used
                                        char *                  szListSeparator           ///< [in]    colum separator to be used
                                        );

    /// Get information about the EVC version
    /// @return     0
    int32_t GetEvcVersionInfo       (   char *              szName                      , ///< [out] name of EVC simulator
                                        char *              szVersion                   , ///< [out] version of EVC simulator
                                        char *              szCompatibility               ///< [out] SRS compatibility of EVC simulator
                                        );

    //--------------------------------------------------------------
    //      METHODS TO CONTROL EVC SIMULATION
    //--------------------------------------------------------------
    /// Initialise the EVC simulator: creation of EVC data structure, initialisation of interfaces
    /// @return  0 on success
    int32_t Init                    ( uint32_t ulLogId                     ///< [in] key used as prefix for log files
                                      );

    /// Start the EVC simulator modules
    /// @return 0 on success
    int32_t Start_processes         (   void                                            );

    /// Start the EVC simulation
    /// @return 0 on success
    int32_t Run                     (   void                                            );

    /// Pause the EVC simulation
    /// @return  0 on success
    int32_t Pause                   (   void                                            );

    /// Resume the EVC simulation
    /// @return 0 on success
    int32_t Resume                  (   void                                            );

    /// Stop the EVC simulation
    /// @return 0 on success
    int32_t Stop                    (   void                                            );


    /// Get context data
    /// @return true on success
    bool    SaveContext             (   SETCS_IO *          pETCS_IO                    , ///< [out] pointer on I/O data
                                        SConfig *           pConfig                     , ///< [out] pointer on Configuration data
                                        SSupervisionData *  pSupervisionData            , ///< [out] pointer on supervision data
                                        SEVCStaticData *    pEVCStaticData                ///< [out] pointer on EVC static data
                                        );

    /// Set context data
    /// @return true on success
    bool    LoadContext             (   SETCS_IO *          pETCS_IO                    , ///< [in] pointer on I/O data
                                        SConfig *           pConfig                     , ///< [in] pointer on Configuration data
                                        SSupervisionData *  pSupervisionData            , ///< [in] pointer on supervision data
                                        SEVCStaticData *    pEVCStaticData                ///< [in] pointer on EVC static data
                                        );

    /// check if DMI is connected and version of communication protocol is compatible
    /// it should be called after Start_processes
    /// @return true is communication with DMI is working
    bool    IsDMIReady              (   void                                            );


    //--------------------------------------------------------------
    //   METHODS TO CONFIFURE EVC SIMULATION (STARTING CONDITIONS)
    //--------------------------------------------------------------

    //   SET EVC CONFIGURATION DATA

    /// Set initial time of day
    void    SetTimeOfDay            (   t_time              dTimeOfDay                  ///< [in] initial time of day (in second)
                                        );

    /// set ETCS identity
    void    SetETCSID               ( uint32_t ulETCSId                    ///< [in] value of ETCS ID / NID_ENGINE (stored on 24 bits)
                                      );

    /// Set the stored RBC short number
    void    SetEirenePhoneNumber    (   const char *        szShortNb                   ///< [in] RBC short number, up to 16 digits (ie: "05670454")
                                        );


    /// set the offsets of balise antennas
    void    SetBaliseAntennaOffsets (   t_distance          dBalAntennaOffsetCabA      , ///< [in] offset between balise antenna and train front when cabin A is used
                                        t_distance          dBalAntennaOffsetCabB        ///< [in] offset between balise antenna and train front when cabin B is used (for future)
                                        );

    /// set length of engine (only in B3)
    void    SetEngineLength         ( t_distance            dEngineLength               ///< [in] length of engine (m)
                                      );


    /// Set the available train equipment
    void SetTrainEquipment(             bool                bBaliseComAvailable         , ///< [in]    indicate if the balise communication is available
                                        bool                bLoopComAvailable           , ///< [in]    indicate if the loop communication is available
                                        int32_t lNbRadioSessionAvailable    , ///< [in]    indicate number of available radio equipements (0, 1 or 2)
                                        bool                bIntegrityDeviceAvailable   , ///< [in]    indicate if the integrity check device is available
                                        bool                bServiceBrakeAvailable      , ///< [in]    indicate if the service brake interface is available
                                        bool                bTCOAvailable               , ///< [in]    indicate if the traction cut off is available
                                        bool                bBrakeFeedBackAvailable     , ///< [in]    indicate if the brake feedback information is available
                                        bool                bAirtighAvailable           , ///< [in]    indicate if the airtight control is available
                                        bool                bColdMvtDetectorAvailable   , ///< [in]    indicate if the cold movement detector is available
                                        const char *        szPhoneNb1 = NULL           , ///< [in]    value of first train phone number,  up to 16 digits (ie: "05670454")
                                        const char *        szPhoneNb2 = NULL             ///< [in]    value of second train phone number, up to 16 digits (ie: "05670454")
            );


    /// Transmit indication about automated or manual train commands
    void SetTrainCmdAuto    (           bool                bSwitchOffRegenBrkAuto      , ///< [in] indicate if the deactivation of regenerative brake is automatic (or manual)
                                        bool                bSwitchOffEddyCBrkAuto      , ///< [in] indicate if the deactivation of eddy current brake is automatic (or manual)
                                        bool                bSwitchOffMagneticBrkAuto   , ///< [in] indicate if the deactivation of magnetic shoe brake is automatic (or manual)
                                        bool                bAirTightCtrlAuto           , ///< [in] indicate if the control of air tightness is automatic (or manual)
                                        bool                bPantoRaiseAuto             , ///< [in] indicate if the control of pantograph raise is automatic (or manual)
                                        bool                bPantoLowAuto               , ///< [in] indicate if the control of pantograph low is automatic (or manual)
                                        bool                bCircuitBreakerCtrlAuto     , ///< [in] indicate if the control of main circuit breaker is automatic (or manual)
                                        bool                bSwitchTractionSystemAuto     ///< [in] indicate if the change of traction system is automatic (or manual)
                                        );

    /// Set the brake interface parameters
    void SetBrakeInterfaceParam (       SBrakeInterfaceParam RegenBrakeInterface          , ///< [in] indicate if regenerative brake is used for EB and/or SB
                                        SBrakeInterfaceParam EpBrakeInterface             , ///< [in] indicate if electro-pneumatic brake is used for EB and/or SB
                                        SBrakeInterfaceParam EddyBrakeInterface           , ///< [in] indicate if eddy current brake is used for EB and/or SB
                                        SBrakeInterfaceParam MShoeBrakeInterface            ///< [in] indicate if magnetic shoe brake is used for EB and/or SB
                                        );

    /// Set one Emergency Brake deceleration model with its parameters Kdry and Kwet and corresponding brakes configuration
    void SetEBModel         (           SEBModelParam        EBModelParam                , ///< [in] : Emergency Brake deceleration model
                                        bool                 bDefault = false              ///< [in] : indicates if model must be applied to all brake configurations
            );

    /// Set one Service Brake deceleration model with its corresponding brakes configuration
    void    SetSBModel              (   SSBModelParam        SBModelParam                , ///< [in] : Service Brake deceleration model
                                        bool                 bDefault = false              ///< [in] : indicates if model must be applied to all brake configurations
            );

    /// Set the on-board correction factors for gradient on normal service deceleration
    void    SetFactorsKn   (            SKn                  Kn_p                        , ///< [in] Correction factor for positive gradient
                                        SKn                  Kn_n                          ///< [in] Correction factor for negative gradient
                                        );

    /// Set the normal service brake decceleration model according to brake position
    void    SetNormSBModel     (        bool                bBrakePosInP                , ///< [in] indicate if model apply to train in P
                                        t_accel             dA_SB01                     , ///< [in] deceleration of service brake used as first pivot value
                                        t_accel             dA_SB12                     , ///< [in] deceleration of service brake used as second pivot value
                                        SBrkModel *         pSB_Model0                  , ///< [in] normal SB model applicable under SB deceleration dA_SB01
                                        SBrkModel *         pSB_Model1                  , ///< [in] normal SB model applicable between SB deceleration dA_SB01 and dA_SB12
                                        SBrkModel *         pSB_Model2                    ///< [in] normal SB model applicable above SB deceleration dA_SB12
                                        );

    /// set the traction model
    void    SetTractionModel         (  t_accel             dMaxAccel                   , ///< [in] maximum train acceleration (m/s²)
                                        t_time              dTractionCutOffTime           ///< [in] time to cut off the traction (s)
                                        );

    /// Set the odometry error model (to simulate inaccuracy in odometry)
    void SetOdoError        (           t_distance          dOdoLinErrcoeff             , ///< [in]    linear error (in metre)
                                        t_distance          dOdoFixedError              , ///< [in]    fixed error (in metre)
                                        double              dUnderErrCoeff              , ///< [in]    coefficient for the under-reading error (in %)
                                        double              dOverErrCoeff                 ///< [in]    coefficient for the over-reading error (in %)
                                        );

    /// set the parameters used for the calculation of service brake equivalent time when conversion model is used
    void SetSBEquivalentTimeParameters(    double           dA                          , ///< [in] model parameter A
                                           double           dB                          , ///< [in] model parameter B
                                           double           dC                          , ///< [in] model parameter C
                                           double           dKto                          ///< [in] model parameter kt0
                                           );

    /// Set the nominal rotating mass factor
    void SetNomRotatingMassFactor(      bool                bUsed                       , ///< [in] indicate if nominal rotating mass factor is used
                                        double              dNomRotatingMassFactor        ///< [in] nominal rotating mass factor
                                        );

    /// Set the minimum distance for announcement of order
    void SetDMIOrderDistance        (   t_distance          dDMIOrderDist               ///< [in] distance for announcement of order
                                        );

    /// Set the minimum time for application of an order
    void SetDMIOrderApplicationTime (   t_time              dOrderAppTime               ///< [in] time for application of an order
                                        );

    /// Set the maximum time to have a feedback for service brake after a brake application request before request emergency brake
    void SetBrakeFeedBackMaxTime    (   t_time              dBrakeFeedBackMaxTime       ///< [in] maximum time to have a brake feedback
                                        );




    //   STORED DATA IN MODE NO POWER (SRS 4.10)

    /// Set the stored position data (starting condition)
    void    SetStoredTrainPosition  (   eValidity           PosVal                      , ///< [in] position data validity
                                        uint32_t ulNID_LRBG                  , ///< [in] identifier of the LRBG
                                        int32_t lLRBGDistance               , ///< [in] distance between train front (cab A) and LRBG
                                        eDirection          LRBGDir                       ///< [in] orientation of LRBG
                                        );

    /// Set stored level data (starting condition)
    void    SetStoredLevel          (   eValidity           Validity            ,       ///< [in] validity of level data
                                        SLevel              Level                       ///< [in] stored ETCS level
                                        );

    /// Set stored radio network id
    void    SetStoredRadioNetwork   (   const char *        szRadNetId              ///< [in] radio network id, up to 6 digits (ie: "02054" )
                                        );

    /// Set stored RBC data
    void    SetStoredRbcData        (   eValidity           Validity                ,   ///< [in] validity of RBC data
                                        uint32_t ulRBCId                 ,   ///< [in] RBC identity (value in 24 bits)
                                        const char *        szPhoneNb                   ///< [in] RBC phone nb, up to 16 digits (ie: "05670454")
                                        );

    /// set stored level priority table
    void    SetStoredLevelTable     (   eValidity           Validity            ,       ///< [in] validity of level priority table
                                        int32_t lLevelNb            ,       ///< [in] number of level in table
                                        SLevel *            paLevelList                 ///< [in] array of level
                                        );


    /// Set the stored national values from binay message
    /// @return 0 on success (valid binary message)
    int32_t SetStoredNationalData(   const uint8_t* pcBinaryNationalValues     ,     ///< [in] binary frame of general radio message (24) containing national values packet(s)
                                     int32_t iBinaryLength                    ///< [in] lengh in bytes of binary frame
                                     );

    /// Set the stored national values
    void SetStoredNationalData      ( uint32_t ulCountryId                 , ///< [in] valid country for the national values
                                      int32_t lSHSpeed                    , ///< [in] permitted speed in shunting (km/h)
                                      int32_t lSRSpeed                    , ///< [in] permitted speed in staff responsible (km/h)
                                      int32_t lOSSpeed                    , ///< [in] permitted speed in on sigh (km/h)
                                      int32_t lLSSpeed                    , ///< [in] permitted speed in limited supervision (km/h)
                                      int32_t lUNSpeed                    , ///< [in] permitted speed in unfitted (km/h)
                                      int32_t lReleaseSpeed               , ///< [in] release speed (km/h)
                                      int32_t lRollAwayDist               , ///< [in] roll away distance (m)
                                      bool                bUseSBforTarget             , ///< [in] indicate if the service brake can be used when braking to a target
                                      bool                bReleaseEB                  , ///< [in] indicate if emergency brake can be release when not at standstill
                                      int32_t lEntryOVSpeed               , ///< [in] maximum speed to select override (km/h)
                                      int32_t lOVSpeed                    , ///< [in] permitted speed in override (km/h)
                                      int32_t lMaxOVDistance              , ///< [in] maximum distance in override (m)
                                      int32_t lMaxOVTime                  , ///< [in] maximum time in override (m)
                                      bool                bDriverIdEntryInRun         , ///< [in] indicate if the driver can enter the train running number when the train is running
                                      eReaction           ContactReaction             , ///< [in] supervision of radio link: reaction to apply
                                      int32_t lContactTime                , ///< [in] supervision of radio link: maximum time between two radio message (s)
                                      int32_t lMaxPTDistance              , ///< [in] maximum distance to reverse in post trip (m)
                                      int32_t lMaxSRDistance              , ///< [in] maximum distance in staff responsible (m)
                                      bool                bDriverAdhesion             , ///< [in] indicate if the driver is allowed to to modify the adhesion
                                      bool                bUseGuidanceCurve           , ///< [in] Use Guidance Curve in supervision curve calculation
                                      bool                bUseServiceBrakeFeedback    , ///< [in] use brake feedback in supervision curve calculation
                                      t_accel             dReducAcc1                  , ///< [in] TBD
                                      t_accel             dReducAcc2                  , ///< [in] TBD
                                      t_accel             dReducAcc3                  , ///< [in] TBD
                                      double              dKv_int                     , ///< [in] TBD
                                      double              dKr_int                     , ///< [in] TBD
                                      double              dKt_int                     , ///< [in] TBD
                                      double              dBGAccuracy                 , ///< [in] TBD
                                      double              dEBConfidenceLevel          , ///< [in] TBD
                                      double              dWheelRailAdhesion          , ///< [in] TBD
                                      bool                bInhibSpeedInaccuracy         ///< [in] TBD
                                      );


    /// set stored EOLM data (for future)
    void SetStoredEOLM              (   eValidity           Validity                    ,   ///< [in] validity of EOLM data
                                        int32_t lLoop_Id                    ,   ///< [in] ETCS loop identity (on 24bits)
                                        t_distance          dLocation                   ,   ///< [in] Location of loop (in metre)
                                        t_distance          dLength                     ,   ///< [in] Length of loop (in metre)
                                        eDirection          LoopDir                     ,   ///< [in] Direction of loop
                                        int32_t lCode                           ///< [in] Spectrum code of loop message (Q_SSCODE)
                                        );


    /// Set stored operated system version (Only in B3)
    void SetStoredOperatedSystemVersion( eOperatedSystemVersion  OperatedSystemVersion   ///< [in] operated system version
                                         );

    /// set stored EOLM data (for future)
    void SetStoredVBC                   (   eValidity           Validity                    ,   ///< [in] validity of VBC data
                                            uint32_t ulVBCcode                       ///< [in] stored VBC code
                                            );



    //   SET DEFAULT TRAIN DATA

    /// set default value of driver id
    void SetDriverId             (   const char* const   szDriverId                  ///< driver identity (up to 16 char)
                                     );


    /// Set the train running number from a string
    void SetTrainRunningNb(   const char* const   szNID_OPERATIONAL           ///< [in] BCD string of train running number
                              );

    /// Set the train running number (with no conversion)
    void SetNID_OPERATIONAL( uint32_t ulNID_OPERATIONAL           ///< [in] numerical value of train running number (cf SRS NID_OPERATIONAL)
                             );


    /// calculate the B2 train category from Brake position and cant deficiency
    /// @return B2 train category
    static int32_t GetB2TrainCategory(     eBrakePosition      BrakePosition,          ///< [in] brake position (R, P, G)
                                           t_distance          dCantDeficiency         ///< [in] train cant deficiency (m)
                                           );

    /// calculate the Brake position from B2 train category
    /// @return Brake position
    static eBrakePosition GetBrkPosFromB2TrainCategory( int32_t lB2TrainCategory       ///< [in] B2 train category
                                                        );

    /// calculate the Cant deficiency from B2 train category
    /// @return train cant deficiency (m)
    static t_distance GetCantDefFromB2TrainCategory( int32_t lB2TrainCategory       ///< [in] B2 train category
                                                     );

    /// calculate the B3 train category from Brake position
    /// @return B2 train category
    static int32_t GetB3TrainCategory(     eBrakePosition      BrakePosition ///< [in] brake position (R, P, G)
                                           );

    /// calculate the Brake position from B3 train category
    /// @return Brake position
    static eBrakePosition GetBrkPosFromB3TrainCategory( int32_t lB3TrainCategory       ///< [in] B3 train category
                                                        );

    /// calculate B3 axle load category from maximum axle load
    /// @return B3 axle load category
    static eAxleLoadCategory GetB3AxleLoadCategory(  t_mass              dAxleloadMass  ///< [in] train maximum axle load (in kg)
                                                     );

    /// calculate B2 axle load from axle load category
    /// @return train maximum axle load (in kg)
    static t_mass GetB2AxleLoad(  eAxleLoadCategory     AxleLoadCategory        ///< [in] axle load category
                                  );


    /// Set the default train data in B2 format, will be converted in B3 format if necessary
    void    SetB2DefaultTrainData  (    t_distance          dLength                     ,  ///< [in] length of train (in metre)
                                        t_speed             dSpeedLimit                 ,  ///< [in] maximum train speed limit(m/s)
                                        int32_t lTrainCategory              ,  ///< [in] B2 train category calculated from Brake position and cant deficiency ( use GetB2TrainCategory() )
                                        int32_t lLoadingGauge               ,  ///< [in] train loading gauge
                                        t_mass              dAxleloadMass               ,  ///< [in] train maximum axle load (in kg)
                                        int32_t lAxleNumber                 ,  ///< [in] train axle number
                                        int32_t lBrakePercentage               ///< [in] train percentage
                                        );

    /// Set the default train data in B3 format, will be converted in B2 format if necessary
    void    SetB3DefaultTrainData  (    t_distance          dLength                     ,  ///< [in] length of train (in metre)
                                        t_speed             dSpeedLimit                 ,  ///< [in] maximum train speed limit(m/s)
                                        int32_t lTrainCategory              ,  ///< [in] B3 train category calculated from Brake position
                                        t_distance          dCantDeficiency             ,  ///< [in] train cant deficiency (m)
                                        int32_t lLoadingGauge               ,  ///< [in] train loading gauge
                                        eAxleLoadCategory   AxleloadCategory            ,  ///< [in] train axle category
                                        int32_t lAxleNumber                 ,  ///< [in] train axle number
                                        int32_t lBrakePercentage               ///< [in] train percentage
                                        );




    /// Set the list of available traction system
    void SetAvailableTractionSystem( int32_t lNbTractionPowers           , ///< [in] number of available traction system
                                     int32_t *              alTractionPowers            , ///< [in] list of traction system identifier (B2 M_TRACTION)
                                     int32_t *              alTractionVoltage           , ///< [in] list of traction voltage (B3 M_VOLTAGE)
                                     int32_t *              alTractionCountry             ///< [in] list of traction country (B3 NID_C_TRACTION)
                                     );



    /// Transmit B2 train data from external source to EVC (during simulation), train must be at standstill
    /// @return     true on success, false on failure
    bool SetB2TrainDataExternal     (   t_distance          dLength                     ,  ///< [in] length of train (in metre)
                                        t_speed             dSpeedLimit                 ,  ///< [in] maximum train speed limit(m/s)
                                        int32_t lTrainCategory              ,  ///< [in] B2 train category calculated from Brake position and cant deficiency ( use GetB2TrainCategory() )
                                        int32_t lLoadingGauge               ,  ///< [in] train loading gauge
                                        t_mass              dAxleloadMass               ,  ///< [in] train maximum axle load (in kg)
                                        bool                bAirtighAvailability        ,  ///< [in] indicate if the airtight control is available
                                        int32_t lNbTractionPowers           ,  ///< [in] number of available traction system
                                        int32_t *              alTractionPowers               ///< [in] list of traction system identifier (B2 M_TRACTION)
                                        );


    /// Transmit B3 train data from external source to EVC (during simulation), train must be at standstill
    /// @return     true on success, false on failure
    bool SetB3TrainDataExternal     (   t_distance      dLength                         ,   ///< [in]    length of train (in metre)
                                        t_speed         dSpeedLimit                     ,   ///< [in]    technical speed limit of train
                                        int32_t lCantDeficiencyTrain_category   ,   ///< [in]    Cant Specific train category
                                        int32_t lOtherSpecificTrain_category    ,   ///< [in]    Other Specific train category
                                        int32_t lLoading_gauge                  ,   ///< [in]    train loading gauge
                                        int32_t lAxleLoadCategory               ,   ///< [in]    train axle load category
                                        bool            bAirtightAvailability           ,   ///< [in]    indicate if the airtight control is available
                                        int32_t lAxleNumber                     ,   ///< [in]    Axle number of the engine
                                        int32_t lNbTraction                     ,   ///< [in]    Nb traction parameters
                                        int32_t *          lTractionVoltage                ,   ///< [in]    Traction voltage list
                                        int32_t *          lTractionCountry                ,   ///< [in]    Traction country list
                                        int32_t lOperationalNb                  ,   ///< [in]    Operationnal number
                                        double          dTractionCutOffTime             ,   ///< [in]    Traction cut off time
                                        int32_t lBrakePercentage                ,   ///< [in]    Brake percentage
                                        bool            bRegenerativeBrakeForEB         ,   ///< [in]    Indicate if the regenerative brake affect EB model
                                        bool            bEddyCurrentBrakeForEB          ,   ///< [in]    Indicate if the eddy brake affect EB model
                                        bool            bMagneticShoeBrakeForEB         ,   ///< [in]    Indicate if the magnetic brake affect EB model
                                        bool            bElectroPneumaticBrakeForEB     ,   ///< [in]    Indicate if the electro pneumatic brake affect EB model
                                        bool            bRegenerativeBrakeForSB         ,   ///< [in]    Indicate if the regenerative brake affect SB model
                                        bool            bEddyCurrentBrakeForSB          ,   ///< [in]    Indicate if the eddy brake affect EB model
                                        bool            bMagneticShoeBrakeForSB         ,   ///< [in]    Indicate if the magnetic shoe brake affect EB model
                                        bool            bElectroPneumaticBrakeForSB     ,   ///< [in]    Indicate if the electro pneumatic brake affect EB model
                                        bool            bTractionCutOffInterface        ,   ///< [in]    Traction cut off availability
                                        bool            bServiceBrakeInterface          ,   ///< [in]    Service brake availability
                                        bool            bServiceBrakeFeedback               ///< [in]    Service brake feedback availability
                                        );



    //--------------------------------------------------------------
    //   METHODS TO SET EVENT DURING SIMULATION
    //--------------------------------------------------------------

    /// set/reset EVC isolation (only in B3)
    void    SetIsolation            (   bool               bIsolated                    ///< [in] new isolation status to set
                                        );

    /// Active or not EVC menu request to DMI
    void    SetDmiEvcMenuRequest    (   bool                bActive                     ///< [out] Activate = true, Deactivate = false
                                        );

    /// set/reset a safety error
    void    SetSafetyError          (   bool                bError = true               ///< [in] new safety error status
            );
    /// Set or reset BTM alarm
    void    SetBTMAlarm             (   bool                bSwitchOn                   ///< [in] Set BTM alarm
                                        );


    //--------------------------------------------------------------
    //   METHODS TO GET INTERNAL DATA DURING SIMULATION
    //--------------------------------------------------------------

    /// get the current time from simulation start (s)
    t_time  GetCurrentTime          (   void                                             );

    /// Get the driver identity
    uint32_t GetDriverId             (   void                                            );

    /// Get train running number
    uint32_t GetNID_OPERATIONAL      (   void                                            );

    /// Get train running number
    /// @param      pszNID_OPERATIONAL  [out]: the digits string
    /// @param      bDigitOnly
    void    GetTrainRunningNumber   (   char* const         pszNID_OPERATIONAL          , ///< [out] the digits string
                                        const bool          bDigitOnly                    ///< [in]  if we must not convert values > 9
                                        );

    /// Get current on-board ETCS level
    eLevel  GetCurrentLevel         (   void                                            );

    /// Get current on-board ETCS mode
    eTrainMode GetCurrentMode       (   void                                            );

    /// Get current internal on-board status
    eEVCStatus GetOnBoardStatus     (   void                                            );


    // get location data

    /// Return estimated front loc of the train (m)
    t_distance  GetEstimatedFrontLoc(   void                                            );

    /// Return min safe front loc of the train (m)
    t_distance  GetMinSafeFrontLoc  (   void                                            );

    /// Return max safe front loc of the train (m)
    t_distance GetMaxSafeFrontLoc   (   void                                            );

    // get supervision data

    /// Get current train speed (km/h)
    double  GetTrainSpeed           (   void                                            );

    /// Get EOA location
    /// @return EOA location or 0 if not applicable
    t_distance  GetEoaLocation      (   void                                            );

    /// Get EOA speed
    /// @return EOA speed (km/h) or 0 if not applicable
    double  GetEoaSpeed             (   void                                            );

    /// Get release speed
    /// @return     Release speed (km/h) or 0 if not applicable
    double  GetReleaseSpeed         (   void                                            );

    /// Get current Emergency brake intervention speed (km/h)
    double  GetEBrakeSpeed          (   void                                            );

    /// Get current service brake intervention speed (km/h)
    double  GetSBrakeSpeed          (   void                                            );

    /// Get current permitted speed (km/h)
    double  GetPermSpeed            (   void                                            );

    /// Get current warning speed (km/h)
    double  GetWarnSpeed            (   void                                            );

    /// Get current indication speed (km/h)
    double  GetIndSpeed             (   void                                            );

    /// Get preindication status
    /// @return true in pre-indication zone
    bool    GetPreindicationStatus  (   void                                            );

    /// Get current speed monitoring type (NONE,CEILING,CURVE)
    eSpeedMonitoringType GetSpeedMonitoringType(   void                                 );

    /// Get current speed monitoring status (CSM,PIM,TSM,RSM,UNKNOWN)
    eSpeedMonitoringStatus GetSpeedMonitoringStatus(   void                             );

    /// Get next target location
    /// @return target location (m) or 0 if not applicable
    t_distance  GetTargetLocation   (   void                                            );

    /// Get next target speed
    /// @return target speed (km/h) or 0 if not applicable
    double  GetTargetSpeed          (   void                                            );

    /// test if there is a known target
    bool    IsTargetKnown           (   void                                            );

    /// Get Staff Responsible maximum speed (km/h)
    double  GetStaffRespSpeed       (   void                                            );

    /// Get maximum distance in Staff Responsible (m)
    t_distance  GetStaffRespDistance(   void                                            );


    // get radio data

    /// Get safe connection status for indicated equipment (0 or 1)
    /// @return safe connection status (RELEASED, REQUESTED, ESTABLISHED)
    eRadSafeConnectStatus GetSafeConnStatus ( int32_t lEquipmentIdx                      );

    /// Get session status for indicated equipment (0 or 1)
    /// @return session status (RELEASED, INITIALISED, ESTABLISHED, TERMINATED)
    eRadSessionState GetSessionStatus       ( int32_t lEquipmentIdx                      );

    /// Get RBC associated to equipment
    /// @return true if equipment is consistent
    bool GetRadioData( int32_t lEquipmentIdx,
                       int32_t &   rlRBCCountry,
                       int32_t &   rlRBCId,
                       uint8_t *  pPhoneNb
                       );

    /// Get current radio network
    void    GetRadNetwork               (   char *pszRadNet     ///< [out] radio network id, up to 6 digits (ie: "02054" )
                                            );

    /// Get string info about radio network registration
    void    GetRadNetworkInfo               (   char *pszRadNetInfo   ///< [out] radio equipment registration information
                                                );

    /// Get current adhesion factor (70 or 100)
    int32_t GetAdhesionFactor       (   void                                            );

    /// Get current most restrictive speed (from MRSP) (km/h)
    double  GetMostRestrictiveSpeed (  void                                             );

    bool    GetSupervisionCurves    (   SInterventionCurves *   pNewCurves              ,
                                        int32_t &                  rlCurveCnt              );

    bool    GetGradientProfile      (   SProfile *              pGradProf               ,
                                        int32_t &                  rlCounter               );

    bool    GetIndicationMarkerList (   SDistanceList &         rDistanceList           ,
                                        int32_t &                  rlCounter               );

    bool    GetTCProfile            (   STCInfoList &           rTCInfoList             ,
                                        int32_t &                  rlCounter               );

    bool    GetMRSP                 (   SProfile *              pMRSP                   ,
                                        int32_t &                  rlCounter               );

    bool    GetDriverAction         (   t_distance &            rdLocation              ,
                                        t_time &                rdTimeStamp             ,
                                        eDriverActionInfo &     rDriverAction           ,
                                        SVarDataList &          rVarDataList            );

    void    GetLRBG                 (   int32_t * const         piID                     ,
                                        int32_t * const         piDistance               ,
                                        eDirection * const      peDirection);


    double  GetGradient             (   void                                            );

    // get train data
    /// Return train length (m)
    double  GetTrainLength          (   void                                            );

    double  GetTrainSpeedLimit      (   void                                            );

    eBrakePosition GetBrakePosition (   void                                            );

    int32_t GetBrakePercentage      (   void                                            );

    int32_t GetTrainCategory        (   void                                            );

    double  GetAxleLoad             (   void                                            );

    double  GetLoadingGauge         (   void                                            );

    bool   GetAirTightAvailability  (   void                                            );

    double  GetBaliseAntennaOffset  (   void                                            );

    int32_t GetCurrentKp            (   void                                            );

    eActiveCabin GetActiveCabin     (   void                                            );

    bool    GetIhmMessages          (   char *                  szMessage               ,
                                        eMSG_TYPE *             type                    );

    bool GetPermSpeedProfile        ( SPerSpeedData * pProfile                          );

    void GetTractionData            (   t_accel                 & rdMaxAccel             ,
                                        t_time                  & rdTractionCutOffTime   ,
                                        int32_t & rlNbTractionPowers     ,
                                        int32_t * alTractionPowers      ,
                                        int32_t * alTractionVoltage      ,
                                        int32_t * alTractionCountry     );

    bool    IsTripActive            (   void                                            );

    bool GetEmergencyStopStatus     (   void                                            );

    void GetRouteSuitabilities      ( SRouteSuitList            * pRoutesSuitabilities  );

    void GetTrackConditions         ( SProfile                  * pGradProfile          ,
                                      SProfile                  * pSpeedProfile         ,
                                      STrackCondTrigger         * pTrackCondTriggers    ,
                                      STrackCondTrigger         * pBigMetalMasses       ,
                                      STractionChangeList       * pTractionChangeList   );

    void GetTrackCondStationPlatform( t_distance                * dDistance,
                                      bool                      * bResetProfile,
                                      int32_t                   * lNbStation,
                                      SCondStation              * aCondStation );

    void GetTrackCondAllowedConsumption( t_distance             * dDistance,
                                         int32_t                * lNewAllowedConsumption );

    //--------------------------------------------------------------
    //   METHODS TO SET FIXED DATA (TO REMOVE AS FIXED BY SRS)
    //--------------------------------------------------------------

    // for test only
    void    SetFixedDataSafeConnectionRepeatTimeout ( t_time dSafeConnectionRepeatTimeout   );
    void    SetFixedDataMaxSessionMaintainTime      ( t_time dMaxSessionMaintainTime   );

    //-------------------------------------------------------------------------------------------------
    /// Convert traction value (baseline 2) to country voltage (baseline 3)
    /// @SRS 6.6.3.2.3 [13]
    bool TractionToCountryVoltage( int32_t lTraction, STractionParam * p );

    //--------------------------------------------------------------
    //      MEMBERS
    //--------------------------------------------------------------

protected:

    void    UpdateLists();
    void    CharToVarDataList( char * szList , SVarDataList & rVarDataList );

    int32_t GetFractionalPart   ( double dValue );

    char    m_szName         [150]  ;   ///< Name of EVC simulator
    char    m_szVersion      [50]   ;   ///< Version of EVC simulator
    char    m_szCompatibility[100]  ;   ///< SRS compatibility of EVC simulator

private:
    pthread_mutex_t	m_EVCControlMutex       ;   ///< Mutex protecting simultaneous calls to EVC control functions
    pthread_mutex_t	m_EVCDataListsMutex     ;   ///< Mutex protecting simultaneous calls to EVC control functions

    SIhmMessageList m_IhmMessageList;
    SDmiActionInfoList m_DmiActionInfoList;
    std::vector<STrackCondLog> m_TrackCondLogList;
};

#endif // _EVC_SIM_H


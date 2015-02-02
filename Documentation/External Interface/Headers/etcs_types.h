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

/// @mainpage
/// @section Introduction
/// It is the HTML documentation of the EVC simulator source code.\n
/// The EVC simulator has been developed according to SRS Class 1 v3.3.0.
/// @section Overview
/// It contains the description of the source code of the EVC simulator (directory eurocab).\n
/// It contains the description of the source code of the EVC simulator control library (directory evc_sim).\n
/// It contains the description of the source code of the EVC simulator interface library for linux (directory interface).\n


//*************************************************************************************************
/// @file   etcs_types.h
/// @brief  Declaration of all common typedef, define, enumeration used in EVC simulator.
/// Project     : EVC Simulator -
/// Module      : EVC -
//*************************************************************************************************

#ifndef _ETCS_TYPES_H
#define _ETCS_TYPES_H
//------------------------------------------------------------------------------
//                                include
//------------------------------------------------------------------------------

#include <time.h>
#include <pthread.h>
#include "etcs_config.h"
#include "SRSTypes.h"
//------------------------------------------------------------------------------
//                                define & structure
//------------------------------------------------------------------------------
#define _LIGHT_EVC_SHMEM_ 1

//===========================  miscellaneous definition  =======================

// Basic types
typedef double          t_distance;     ///< distance in m
typedef double          t_time;         ///< time in s
typedef double          t_speed;        ///< speed in m/s
typedef double          t_accel;        ///< acceleration in m/s^2
typedef double          t_mass;         ///< mass in kg


// Maximum length of the safety area in meters
// ie Max Nb of points in track profile and supervision curves
// Note: maximum possible SRS value = 327660m
#define MA_MAX_SIZE             328000          ///< Maximum value for MA length : 328 km
#define MA_REDUCED_SIZE         32800           ///< Reduced value for MA length : 32,8 km

/// Maximum size of location data, mainly use for the calculation of braking curves
#if (_LIGHT_EVC_SHMEM_ > 0)
#define MAX_LENGTH_METERS       MA_REDUCED_SIZE
#else //_LIGHT_EVC_SHMEM_: FULL
#define MAX_LENGTH_METERS       MA_MAX_SIZE
#endif //_LIGHT_EVC_SHMEM_

/// Macro to test if the size used for curves calculation is the maximum value
#define IS_MA_MAX_SIZE \
    (MAX_LENGTH_METERS == MA_MAX_SIZE )

/// Macro to test if the size used for curves calculation is the reduced value
#define IS_MA_REDUCED_SIZE \
    (MAX_LENGTH_METERS == MA_MAX_SIZE )

/// Maximum distance that can be processed by the train (Max track length)
/// ie Max length of (dDistLOA+dOverlap) in meters
/// 340 such that INFINITE_DISTANCE_METERS=327660 when MAX_LENGTH_METERS is initialized to 328000m
/// 340 is a safety margin for curves computation
#define INFINITE_DISTANCE_METERS ((t_distance)(MAX_LENGTH_METERS-340))

/// maximum distance (arbitrary) to calculate supervision curves in UN (when there is some TSR)
#define MAX_UN_CALC_DISTANCE        10000.0

#define INFINITE_SPEED  40000

/// Maximum number of discontinuities in profiles used
#define MAX_PROFILE_DISCONTINUITIES 256

/// Size of buffer
#define BUFFER_SIZE                 1024

/// Maximum value to store in 32 bits variable (int32_t) = (2^32)/2 - 1
#define INFINITE_LONG               2147483647

/// Maximum value for time
#define INFINITE_TIME               4294967295.0

/// Special value to indicate that national speed should be used
#define NATIONAL_SPEED              9999

/// Gravity acceleration
#define G                           (t_accel)9.81

/// Allowed error
#define SPEED_EPSILON               0.001

/// Special values for variables in packets
#define IMMEDIATE_VALIDITY_FOR_NV       32767
#define IMMEDIATE_LEVEL_TR              32767
#define ALLOWED_CONSUMPTION_RESET       1023

// Tunnel stopping area for DMI variable
#define DMI_NO_TUNNEL_STOPPING_AREA     0xFFFFFFFF

/// Special values for brakes parameters
#define NB_EB_MODELS        16
#define NB_SB_MODELS        8
#define TBS_LOCKED          0.0 //s
#define TBS2_LOCKED         2.0 //s
#define MIN_SRS300_ACCEL    0.0 // m/s²
#define MAX_SRS300_ACCEL    4.0 // m/s²

/// Special value for timestamp
#define TIMESTAMP_UNKNOWN   4294967295UL

#define NB_SECONDS_BY_DAY   86400

/// Maximum size of data transmitted to IHM
#define     MAX_IHM_DATA_SIZE           256

/// Maximum size of data transmitted via message queue
#define MAX_QUEUE_DATA_SIZE     8192

/// Macro to write an application error in log file with file name and line number
#define     APP_ERROR(Func,Msg)      Trace_AppError( __FILE__, __LINE__, Func, Msg )

/// Macro to write a system error in log file with errno, file name and line number
#define     SYS_ERROR(Func,Msg)      Trace_SysError( __FILE__, __LINE__, Func, Msg )

/// max nb of module in eurocab
#define     MAX_MODULE_NB               20

#define     UNUSED(x) (void)x;

/// value send for force considerign SSCODE valid
#define     IGNORE_Q_SSCODE_VALUE  15

/// Maximum length of an error message
#define MAX_LENGTH_ERROR_MSG    256

typedef enum
{
    ETCS_IO_KEY  = 0    ,   ///< key for the shared memory ETCS IO

    CONFIG_KEY          ,   ///< key for the shared memory config
    INTERVENTION_1_KEY  ,   ///< key for the shared memory intervention curves set 1
    INTERVENTION_2_KEY  ,   ///< key for the shared memory intervention curves set 2
    SUPERVISION_DATA_KEY,   ///< key for the shared memory supervision data
    TEMPORARY_DATA_KEY  ,   ///< key for the shared memory temporary curves calculation data
    STATIC_DATA_KEY     ,   ///< key for the shared memory containing EVC static data

    MESSAGE_KEY         ,   ///< key for the shared memory for internal communication
    MSG_KEY             ,   ///< key for the message queue for internal communication

    ODO_COM_KEY         ,   ///< key for the message queue for ODO communication
    TIU_COM_KEY         ,   ///< key for the message queue for TIU communication
    BAL_COM_KEY         ,   ///< key for the message queue for balise communication
    RAD_COM_KEY         ,   ///< key for the message queue for radio communication
    RAD_COM_KEY2        ,   ///< key for the message queue for radio communication 2
    DMI_COM_KEY         ,   ///< key for the message queue for DMI communication
    LOOP_COM_KEY        ,   ///< key for the message queue for LOOP communication
    DUMMY_COM_KEY       ,   ///< key for the message queue for DUMMY communication
    JRU_COM_KEY         ,   ///< key for the message queue for JRU communication

    MAX_KEY_VAL
} eEVC_Key;


/// Structure to store time
typedef struct STime
{
    int32_t tv_sec  ;   ///< seconds
    int32_t tv_usec ;   ///< microseconds

} STime;

/// Type for file management
typedef void t_File;

//===========================  internal communication  ============================================

/// structure used to store information about a started module
typedef struct SModuleAttribute
{
    bool        bUsed       ;   ///< indicate if it contains module data
    bool        bStop       ;   ///< require stop
    bool        bStart      ;   ///< require start
    pthread_t   thread      ;   ///< module identity
    pid_t       pid         ;   ///< module pid
    int32_t     lPriority   ;   ///< module priority
    int32_t     lPeriod     ;   ///< module period
    t_File *    pLogFile    ;   ///< pointer module log file
    bool        bPrintfLogs ;   ///< indicate whether to show logs (Traces) on screen
    STime       LastTime    ;   ///< time of last wakeup

} SModuleAttribute;


/// Enumeration for message application code definition
typedef enum
{
    MISC                            ,   ///< miscellaneous
    ALL_MSG_ACK_RESULT              ,   ///< result of message acknowledgement

    // application code for data manager
    DAT_BG_DATA                     ,   ///< balise group message (for data manager)
    DAT_LOOP_MSG                    ,   ///< loop message (for data manager)
    DAT_RADIO_MSG                   ,   ///< radio message  (for data manager)
    DAT_RADIO_MSG_BUFFER            ,   ///< radio message transition buffer for RBC/RBC transition (for data manager)
    DAT_MA_RQST                     ,   ///< MA request (for data manager)
    DAT_MA_RQST_SECTION_TIMEOUT     ,   ///< MA request because of section timeout
    DAT_DRIVER_ADHESION             ,   ///< Driver changes adhesion (for data manager)
    DAT_DELETE_STORED_RBC_MA_DATA   ,   ///< Request to delete stored RBC MA data (for data manager)
    DAT_STOP_MA_RQST                ,   ///< Stop sending of MA request
    DAT_EXTERNAL_TRAIN_DATA         ,   ///< train data received from external source

    // application code for speed controller
    CTRL_SR_DATA                    ,   ///< staff responsible MA data (for speed controller)
    CTRL_SR_BG_LIST                 ,   ///< list of BG for SR area (for speed controller)
    CTRL_EB_APP                     ,   ///< EB application required (for speed controller)
    CTRL_EB_REL                     ,   ///< release of EB required (for speed controller)
    CTRL_SB_APP                     ,   ///< SB application required (for speed controller)
    CTRL_SB_REL                     ,   ///< release of SB required (for speed controller)
    CTRL_ACK_BRAKE                  ,   ///< ack brake intervention (for speed controller)
    CTRL_SH_BG_LIST                 ,   ///< shunting area data (for speed controller)
    CTRL_NEW_BG                     ,   ///< indication of new BG (for speed controller)
    CTRL_DEL_SH_BG_LIST             ,   ///< delete shunting area data (for speed controller)
    CTRL_I1_EB_APP                 ,   ///< EB application required from internal (for speed controller)
    CTRL_I2_EB_REL                 ,   ///< release of EB required from internal (for speed controller)
    CTRL_I3_SB_APP                 ,   ///< SB application required from internal (for speed controller)
    CTRL_I4_SB_REL                 ,   ///< release of SB required from internal (for speed controller)

    // application code for mode manager
    MOD_MAFSOK                      ,   ///< speed sup says Full Mode is OK after MA check (for mode manager)
    MOD_DRIVER_ID_ENTERED           ,   ///< driver id entered (for mode manager)
    MOD_ETCS_LEVEL_0_ENTERED        ,   ///< etcs level 0 entered (for mode manager)
    MOD_ETCS_LEVEL_1_ENTERED        ,   ///< etcs level 1 entered (for mode manager)
    MOD_ETCS_LEVEL_2_ENTERED        ,   ///< etcs level 2 entered (for mode manager)
    MOD_ETCS_LEVEL_3_ENTERED        ,   ///< etcs level 3 entered (for mode manager)
    MOD_ETCS_LEVEL_MAX_ENTERED      ,   ///< etcs level MAX entered (for mode manager)
    MOD_RBC_DATA_ENTERED			,   ///< RBC data entered (for mode manager)
    MOD_SESSION_OPEN                ,   ///< radio com. session established (for mode manager)
    MOD_SESSION_CLOSED              ,   ///< radio com. session released  (for mode manager)
    MOD_RBC_CONFIRM_SOM_PR          ,   ///< RBC confirm SoM PR (for mode manager)
    MOD_RBC_ACCEPT_TRAIN            ,   ///< RBC accept train (for mode manager)
    MOD_RBC_REJECT_TRAIN            ,   ///< RBC reject train (for mode manager)
    MOD_CONTINUE_WITH_NO_SESSION    ,   ///< continue with no session (for mode manager)
    MOD_NOT_CONTINUE_WITH_NO_SESSION,   ///< do not continue with no session (for mode manager)
    MOD_NL_RQST                     ,   ///< Non leading request (for mode manager)
    MOD_NL_EXIT_RQST                ,   ///< Non leading exit request (for mode manager)
    MOD_DRIVER_SH_RQST              ,   ///< shunting request from driver (for mode manager)
    MOD_SH_RQST                     ,   ///< shunting request (internal: mode profile) (for mode manager)
    MOD_TRAIN_RUN_NB                ,   ///< entry of train running nb performed (for mode manager)
    MOD_TRAIN_DATA_ENTRY_RQST       ,   ///< Train data request from
    MOD_TRAIN_DATA_ENTERED          ,   ///< entry of train data entry performed (for mode manager)
    MOD_RBC_ACK_TRAIN_DATA          ,   ///< RBC acknowledged train data (for mode manager)
    MOD_RBC_NACK_TRAIN_DATA         ,   ///< RBC did not acknowledge train data (for mode manager)
    MOD_OVERRIDE_RQST               ,   ///< override request (for mode manager)
    MOD_START_RQST                  ,   ///< start mission request (for mode manager)
    MOD_SH_ACCEPTED                 ,   ///< shunting accepted by RBC (for mode manager)
    MOD_SH_REFUSED                  ,   ///< shunting refused by RBC (for mode manager)
    MOD_UN_ACK                      ,   ///< ack of unfitted by the driver (for mode manager)
    MOD_OS_ACK                      ,   ///< ack of on sight by the driver (for mode manager)
    MOD_SR_ACK                      ,   ///< ack of staff resp by the driver (for mode manager)
    MOD_SH_ACK                      ,   ///< ack of shunting by the driver (for mode manager)
    MOD_DM_ACK                      ,   ///< ack dummy by the driver (for mode manager)
    MOD_LS_RQST                     ,   ///< limited supervision request (for mode manager)
    MOD_OS_RQST                     ,   ///< on sight request (for mode manager)
    MOD_FS_RQST                     ,   ///< full supervision request (for mode manager)
    MOD_SR_RBC_RQST                 ,   ///< Staff resp request from RBC (for mode manager)
    MOD_SH_RBC_RQST                 ,   ///< shunting request from RBC (for mode manager)
    MOD_UN_RQST                     ,   ///< unfitted request (level tr) (for mode manager)
    MOD_DM_RQST                     ,   ///< dummy request (level tr) (for mode manager)
    MOD_FORCE_TRIP_RQST             ,   ///< force train trip request (for mode manager)
    MOD_CONTINUE_PS_RQST            ,   ///< Request to continue in passive shunting on desks closure (for mode manager)
    MOD_TRIP_RQST                   ,   ///< train trip request (for mode manager)
    MOD_LEVEL_TR_DATA               ,   ///< level transition data (for mode manager)
    MOD_LEVEL_TR_ON_MODE_CHANGE     ,   ///< Level transition data on mode change (for mode manager)
    MOD_DMI_TEST_OK                 ,   ///< test of DMI OK (for mode manager)
    MOD_EXIT_SH                     ,   ///< exit shunting (for mode manager)
    MOD_ACK_TRIP                    ,   ///< driver ack train trip (for mode manager)
    MOD_REVERSING_RQST              ,   ///< request to enter in reversing (for mode manager)
    MOD_RBC_ACK_PT                  ,   ///< RBC ack exit from trip (PT) (for mode manager)
    MOD_ISOLATION_RQST              ,   ///< isolation request (for mode manager)
    MOD_ISOLATION_RESET              ,  ///< out of isolation request (for mode manager)
    MOD_REVERSING_ACK               ,   ///< ack of reversing (for mode manager)
    MOD_REVERSE_AREA                ,   ///< reversing area data (for mode manager)
    MOD_PROFIL                      ,   ///< mode profile data (for mode manager)
    MOD_PROFIL_UPDATE               ,   ///< mode profile data : current profil update (for mode manager)
    MOD_PROFIL_DELETE				,	///< current mode profile : delete after a point or delete completely (for mode manager and speed supervision)
    MOD_DRIVER_ACK_MODE				,	///< Driver ack mode (for mode manager)
    MOD_DRIVER_ACK_LEVEL			,	///< Driver ack level (for mode manager)
    MOD_EXIT_SH_DESK_OPEN           ,   ///< Train should exit SH on desk opening
    MOD_SESSION_WAIT                ,   ///< Suspend display (hourglass) while we try to open a radio session

    // application code for supervision curves computer
    COMP_MA_DATA                    ,   ///< MA data (for supervision curves computer)
    COMP_TSR                        ,   ///< TSR data (for supervision curves computer)
    COMP_TSR_REVOC                  ,   ///< TSR revocation (for supervision curves computer)
    COMP_DEF_GRAD                   ,   ///< Default gradient for TSR (for supervision curves computer)
    COMP_SPD_PROF                   ,   ///< speed profile data (for supervision curves computer)
    COMP_GRAD_PROF                  ,   ///< gradient profile data (for supervision curves computer)
    COMP_SHORT_MA                   ,   ///< shortened MA (for supervision curves computer)
    COMP_EMER_STOP                  ,   ///< emergency stop data (for supervision curves computer)
    COMP_REDUC_MA                   ,   ///< release MA (for supervision curves computer)
    COMP_AXLELOAD_PROF              ,   ///< axle load speed profile (for supervision curves computer)
    COMP_RECALC_MA                  ,   ///< required re-calculation of MA (for supervision curves computer)
    COMP_RECALC_MA_SR               ,   ///< required re-calculation of MA in SR mode (for supervision curves computer)
    COMP_REVERSE_SUP_DAT            ,   ///< reverse supervision data is received (for supervision curves computer)
    COMP_REPOS_INFO                 ,   ///< repositioning information (for supervision curves computer)
    COMP_NEW_NATIONAL_DAT           ,   ///< new national data have been set (for supervision curves computer)
    COMP_CONVERT_MODEL              ,   ///< request calcul of train data from conversion model (for supervision curves computer)
    COMP_CALC_SUP_DATA              ,   ///< request calcul of supervision data for current mode (for supervision curves computer)
    COMP_LX_INFO                    ,   ///< Level crossing information (for supervision curves computer)
    COMP_PERM_BRAK_DIST             ,   ///< Permitted braking distance (for supervision curves computer)

    // application code for odometer interface

    // application code for recorder interface manager
    REC_DATA                        ,   ///< record attached data (for recorder)
    REC_JRU_DATA                    ,   ///< record attached data to JRU (for recorder)

    // application code for DMI interface
    DISP_TRACKSIDE_TXT_MSG          ,   ///< message for driver from trackside packet 72 and 76 (for DMI interface manager)
    DISP_SYSTEM_INFO				,	///< Information send to DMI (Driver ack, transition info) (for DMI interface manager)
    DISP_SYSTEM_INFO_REMOVE			,	///< Information send to DMI (Driver ack, transition info) (for DMI interface manager)
    DISP_SYSTEM_STATUS_MSG			,	///< Information send to DMI (Driver ack, transition info) (for DMI interface manager)
    DISP_SYSTEM_STATUS_REMOVE       ,	///< Information send to DMI (Driver ack, transition info) (for DMI interface manager)
    DISP_FREE_TXT_MSG               ,   ///< Free message for driver from EVC. Implementation dependent.
    DISP_ROUTE_INFO                 ,   ///< route information (for DMI interface manager)
    DISP_DMI_STATE_RQST             ,   ///< request DMI status (for DMI interface manager)
    DISP_ACK_LAST_MODE_LEVEL		,	///< ack last icon (for DMI interface manager)
    DISP_ACK_LAST_MSG               ,   ///< ack last text mess (for DMI interface manager)
    DISP_ACK_MSG                    ,   ///< ack text message with id in data (for DMI interface manager)
    DISP_TRACK_CONDITION            ,   ///< track condition data (for DMI interface manager)
    DISP_BIG_METAL_MASS             ,   ///< big metal masses condition (for DMI interface manager)
    DISP_TSR                        ,   ///< info about TSR (for DMI interface manager)
    DISP_TRACTION                   ,   ///< info about TSR (for DMI interface manager)
    DISP_MODEPROF                   ,   ///< Mode profile (for DMI interface manager)
    DISP_LX_INFO                    ,   ///< level crossing information (for DMI interface manager)
    DISP_DUMREQUEST                 ,   ///< Dummy display request (for DMI interface manager)
    DISP_DUMMY_DATA_ENTRY           ,   ///< Dummy
    DISP_DUMMY2_DATA_ENTRY          ,   ///< Dummy 2
    DISP_TAF_RQST                   ,   ///< Management of TAF menu
    DISP_ACK_TAF                    ,   ///< acknowledgement of TAF
    DISP_TRAIN_NUMBER               ,   ///< Send train number to DMI.
    DISP_RADIO_NETWORK_LIST_UPDATED ,   ///< notification that network list is updated

    // application code for radio
    RAD_SOM_CONNECT_RQST            ,   ///< Request radio session establishment (in SoM scope) (for radio interface)
    RAD_CONNECT_RQST                ,   ///< Request radio session establishment (for radio interface)
    RAD_DISCONNECT_RQST             ,   ///< Request radio session termination (for radio interface)
    RAD_SEND_MSG_RQST               ,   ///< Request to send a radio message (for radio interface)
    RAD_CONNECT                     ,   ///< connection order + connection ok reply from RIM (for radio interface)
    RAD_DISCONNECT                  ,   ///< disconnection order + connection impossible reply from RIM (for radio interface)
    RAD_DATA                        ,   ///< data received from RBC (for radio interface)
    RAD_HP_DATA                     ,   ///< high priority data received from RBC (for radio interface)
    RAD_RBC_TRANSITION              ,   ///< RBC/RBC handover data (for radio interface)
    RAD_SESSION_MNG                 ,   ///< order to connect, disconnect RBC (for radio interface)
    RAD_END_OF_MISSION              ,   ///< start of end of mission procedure (for radio interface)
    RAD_SEND_TRAIN_DATA             ,   ///< Request sending of train data to RBC (for radio interface)
    RAD_DELETE_STORED_RBC_MA_DATA   ,   ///< Request to delete stored RBC MA data (for radio interface)
    RAD_INFILL_DATA                 ,   ///< data for infill management
    RAD_RIU_SESSION_MNG             ,   ///< order to connect, disconnect RIU (for radio interface)
    RAD_EXIT_RBC_AREA               ,   ///< request repetition of PR due to exit of RBC area
    RAD_NETWORK_REGISTRATION_ORDER  ,   ///< network registration order from driver or track-side

    // application code for RIM interface
    RIM_CONNECT                     ,   ///< request for safe radio connection, unlimited attempts (for RIM interface)
    RIM_CONNECT_LIMITED             ,   ///< request for safe radio connection, limited attempts (for RIM interface)
    RIM_DISCONNECT                  ,   ///< request for safe radio disconnection (for RIM interface)
    RIM_DATA                        ,   ///< data to be sent to RBC by RIM (for RIM interface)
    RIM_NETWORK_REGISTER            ,   ///< request to register mobile to radio network (for RIM interface)
    RIM_NETWORK_LIST                ,   ///< request to get list of permitted network (for RIM interface)

    // application code for balise message manager
    BAL_LINK_DATA                   ,   ///< linking information (for balise message manager)
    BAL_BIG_METAL_MASSES            ,   ///< track condition data, big metal masses (for balise message manager)
    BAL_LOOP_INFO                   ,   ///< info about loop (for balise message manager)
    BAL_BTM_ALARM_ON                ,   ///< BTM alarm is set, for test (for balise message manager)
    BAL_BTM_ALARM_OFF               ,   ///< BTM alarm is reset, for test (for balise message manager)
    BAL_VBC_TRACKSIDE               ,   ///< Balise cover order by trackside
    BAL_VBC_DRIVER_ENTER            ,   ///< Balise cover order entered by driver
    BAL_VBC_DRIVER_REMOVE           ,   ///< Balise cover order removed by driver

    // application code for odometer interface
    ODO_LINKING_CORRECTION          ,   ///< Linking correction used to compute confidence interval
    ODO_NTC_ISOLATION               ,   ///< NTC isolation information
    ODO_TRACK_CONDITION_STATION = 165,  ///< Track condition station information

    // Received by multiple threads
    MISC_TRAIN_LENGTH_INCREASED = 187,  ///< Reduce storage data in rear of the train because driver has changed train length

    // Received by EVC Sim, to be used by other applications
    EXTERNAL_INFO_TEXT              ,
    EXTERNAL_INFO_TRACK_COND        ,
    EXTERNAL_INFO_DMI_ACTION        ,

    MAX_APP_CODE                        ///< Maximum number of code

} eMsgCode;


/// Macro to get the string corresponding to a message application code
#define STR_APPCODE(d) \
    (d)== MISC                              ? "MISC" : \
    (d)== ALL_MSG_ACK_RESULT                ? "ALL_MSG_ACK_RESULT" : \
    (d)== DAT_BG_DATA                       ? "DAT_BG_DATA" : \
    (d)== DAT_LOOP_MSG                      ? "DAT_LOOP_MSG" : \
    (d)== DAT_RADIO_MSG                     ? "DAT_RADIO_MSG" : \
    (d)== DAT_MA_RQST                       ? "DAT_MA_RQST" : \
    (d)== DAT_DRIVER_ADHESION               ? "DAT_DRIVER_ADHESION" : \
    (d)== CTRL_SR_DATA                      ? "CTRL_SR_DATA" : \
    (d)== CTRL_SR_BG_LIST                   ? "CTRL_SR_BG_LIST" : \
    (d)== CTRL_EB_APP                       ? "CTRL_EB_APP" : \
    (d)== CTRL_EB_REL                       ? "CTRL_EB_REL" : \
    (d)== CTRL_SB_APP                       ? "CTRL_SB_APP" : \
    (d)== CTRL_SB_REL                       ? "CTRL_SB_REL" : \
    (d)== CTRL_ACK_BRAKE                    ? "CTRL_ACK_BRAKE" : \
    (d)== CTRL_SH_BG_LIST                   ? "CTRL_SH_BG_LIST" : \
    (d)== CTRL_NEW_BG                       ? "CTRL_NEW_BG" : \
    (d)== CTRL_DEL_SH_BG_LIST               ? "CTRL_DEL_SH_BG_LIST" : \
    (d)== CTRL_I1_EB_APP                    ? "CTRL_I1_EB_APP" : \
    (d)== CTRL_I2_EB_REL                    ? "CTRL_I2_EB_REL" : \
    (d)== CTRL_I3_SB_APP                    ? "CTRL_I3_SB_APP" : \
    (d)== CTRL_I4_SB_REL                    ? "CTRL_I4_SB_REL" : \
    (d)== MOD_MAFSOK                        ? "MOD_MAFSOK" : \
    (d)== MOD_DRIVER_ID_ENTERED             ? "MOD_DRIVER_ID_ENTERED" : \
    (d)== MOD_ETCS_LEVEL_0_ENTERED          ? "MOD_ETCS_LEVEL_0_ENTERED" : \
    (d)== MOD_ETCS_LEVEL_1_ENTERED          ? "MOD_ETCS_LEVEL_1_ENTERED" : \
    (d)== MOD_ETCS_LEVEL_2_ENTERED          ? "MOD_ETCS_LEVEL_2_ENTERED" : \
    (d)== MOD_ETCS_LEVEL_3_ENTERED          ? "MOD_ETCS_LEVEL_3_ENTERED" : \
    (d)== MOD_ETCS_LEVEL_MAX_ENTERED        ? "MOD_ETCS_LEVEL_MAX_ENTERED" : \
    (d)== MOD_RBC_DATA_ENTERED              ? "MOD_RBC_DATA_ENTERED" : \
    (d)== MOD_SESSION_OPEN                  ? "MOD_SESSION_OPEN" : \
    (d)== MOD_SESSION_CLOSED                ? "MOD_SESSION_CLOSED" : \
    (d)== MOD_RBC_CONFIRM_SOM_PR            ? "MOD_RBC_CONFIRM_SOM_PR" : \
    (d)== MOD_RBC_ACCEPT_TRAIN              ? "MOD_RBC_ACCEPT_TRAIN" : \
    (d)== MOD_RBC_REJECT_TRAIN              ? "MOD_RBC_REJECT_TRAIN" : \
    (d)== MOD_CONTINUE_WITH_NO_SESSION      ? "MOD_CONTINUE_WITH_NO_SESSION" : \
    (d)== MOD_NOT_CONTINUE_WITH_NO_SESSION  ? "MOD_NOT_CONTINUE_WITH_NO_SESSION" : \
    (d)== MOD_NL_RQST                       ? "MOD_NL_RQST" : \
    (d)== MOD_NL_EXIT_RQST                  ? "MOD_NL_EXIT_RQST" : \
    (d)== MOD_DRIVER_SH_RQST                ? "MOD_DRIVER_SH_RQST" : \
    (d)== MOD_SH_RQST                       ? "MOD_SH_RQST" : \
    (d)== MOD_TRAIN_DATA_ENTRY_RQST         ? "MOD_TRAIN_DATA_ENTRY_RQST" : \
    (d)== MOD_TRAIN_RUN_NB                  ? "MOD_TRAIN_RUN_NB" : \
    (d)== MOD_TRAIN_DATA_ENTERED            ? "MOD_TRAIN_DATA_ENTERED" : \
    (d)== MOD_RBC_ACK_TRAIN_DATA            ? "MOD_RBC_ACK_TRAIN_DATA" : \
    (d)== MOD_OVERRIDE_RQST                 ? "MOD_OVERRIDE_RQST" : \
    (d)== MOD_START_RQST                    ? "MOD_START_RQST" : \
    (d)== MOD_SH_ACCEPTED                   ? "MOD_SH_ACCEPTED" : \
    (d)== MOD_SH_REFUSED                    ? "MOD_SH_REFUSED" : \
    (d)== MOD_EXIT_SH_DESK_OPEN             ? "MOD_EXIT_SH_DESK_OPEN" : \
    (d)== MOD_UN_ACK                        ? "MOD_UN_ACK" : \
    (d)== MOD_OS_ACK                        ? "MOD_OS_ACK" : \
    (d)== MOD_SR_ACK                        ? "MOD_SR_ACK" : \
    (d)== MOD_SH_ACK                        ? "MOD_SH_ACK" : \
    (d)== MOD_SN_ACK                        ? "MOD_SN_ACK" : \
    (d)== MOD_OS_RQST                       ? "MOD_OS_RQST" : \
    (d)== MOD_LS_RQST                       ? "MOD_LS_RQST" : \
    (d)== MOD_FS_RQST                       ? "MOD_FS_RQST" : \
    (d)== MOD_SR_RBC_RQST                   ? "MOD_SR_RBC_RQST" : \
    (d)== MOD_SH_RBC_RQST                   ? "MOD_SH_RBC_RQST" : \
    (d)== MOD_UN_RQST                       ? "MOD_UN_RQST" : \
    (d)== MOD_SN_RQST                       ? "MOD_SN_RQST" : \
    (d)== MOD_FORCE_TRIP_RQST               ? "MOD_FORCE_TRIP_RQST" : \
    (d)== MOD_CONTINUE_PS_RQST              ? "MOD_CONTINUE_PS_RQST" : \
    (d)== MOD_TRIP_RQST                     ? "MOD_TRIP_RQST" : \
    (d)== MOD_LEVEL_TR_DATA                 ? "MOD_LEVEL_TR_DATA" : \
    (d)== MOD_DMI_TEST_OK                   ? "MOD_DMI_TEST_OK" : \
    (d)== MOD_EXIT_SH                       ? "MOD_EXIT_SH" : \
    (d)== MOD_ACK_TRIP                      ? "MOD_ACK_TRIP" : \
    (d)== MOD_REVERSING_RQST                ? "MOD_REVERSING_RQST" : \
    (d)== MOD_RBC_ACK_PT                    ? "MOD_RBC_ACK_PT" : \
    (d)== MOD_ISOLATION_RQST                ? "MOD_ISOLATION_RQST" : \
    (d)== MOD_REVERSING_ACK                 ? "MOD_REVERSING_ACK" : \
    (d)== MOD_REVERSE_AREA                  ? "MOD_REVERSE_AREA" : \
    (d)== MOD_ISOLATION_RQST                ? "MOD_ISOLATION_RQST" : \
    (d)== MOD_PROFIL                        ? "MOD_PROFIL" : \
    (d)== COMP_MA_DATA                      ? "COMP_MA_DATA" : \
    (d)== COMP_TSR                          ? "COMP_TSR" : \
    (d)== COMP_TSR_REVOC                    ? "COMP_TSR_REVOC" : \
    (d)== COMP_DEF_GRAD                     ? "COMP_DEF_GRAD" : \
    (d)== COMP_SPD_PROF                     ? "COMP_SPD_PROF" : \
    (d)== COMP_GRAD_PROF                    ? "COMP_GRAD_PROF" : \
    (d)== COMP_SHORT_MA                     ? "COMP_SHORT_MA" : \
    (d)== COMP_EMER_STOP                    ? "COMP_EMER_STOP" : \
    (d)== COMP_REDUC_MA                     ? "COMP_REDUC_MA" : \
    (d)== COMP_AXLELOAD_PROF                ? "COMP_AXLELOAD_PROF" : \
    (d)== COMP_RECALC_MA                    ? "COMP_RECALC_MA" : \
    (d)== COMP_RECALC_MA_SR                 ? "COMP_RECALC_MA_SR" : \
    (d)== COMP_REVERSE_SUP_DAT              ? "COMP_REVERSE_SUP_DAT" : \
    (d)== COMP_REPOS_INFO                   ? "COMP_REPOS_INFO" : \
    (d)== COMP_NEW_NATIONAL_DAT             ? "COMP_NEW_NATIONAL_DAT" : \
    (d)== COMP_CONVERT_MODEL                ? "COMP_CONVERT_MODEL" : \
    (d)== COMP_PERM_BRAK_DIST               ? "COMP_PERM_BRAK_DIST" : \
    (d)== REC_DATA                          ? "REC_DATA" : \
    (d)== REC_JRU_DATA                      ? "REC_JRU_DATA" : \
    (d)== DISP_TRACKSIDE_TXT_MSG            ? "DISP_TRACKSIDE_TXT_MSG" : \
    (d)== DISP_SYSTEM_INFO					? "DISP_SYSTEM_INFO" : \
    (d)== DISP_SYSTEM_INFO_REMOVE			? "DISP_SYSTEM_INFO_REMOVE" : \
    (d)== DISP_FREE_TXT_MSG                 ? "DISP_FREE_TXT_MSG" : \
    (d)== DISP_SYSTEM_STATUS_MSG			? "DISP_SYSTEM_STATUS_MSG" : \
    (d)== DISP_ROUTE_INFO                   ? "DISP_ROUTE_INFO" : \
    (d)== DISP_DMI_STATE_RQST               ? "DISP_DMI_STATE_RQST" : \
    (d)== DISP_ACK_LAST_MSG                 ? "DISP_ACK_LAST_MSG" : \
    (d)== DISP_ACK_MSG                      ? "DISP_ACK_MSG" : \
    (d)== DISP_TRACK_CONDITION              ? "DISP_TRACK_CONDITION" : \
    (d)== DISP_BIG_METAL_MASS               ? "DISP_BIG_METAL_MASS" : \
    (d)== DISP_TSR                          ? "DISP_TSR" : \
    (d)== DISP_MODEPROF                     ? "DISP_MODEPROF" : \
    (d)== DISP_LX_INFO                      ? "DISP_LX_INFO" : \
    (d)== DISP_DUMREQUEST                   ? "DISP_DUMREQUEST" : \
    (d)== DISP_TAF_RQST						? "DISP_SHOW_TAF_RQST" : \
    (d)== DISP_ACK_TAF						? "DISP_ACK_TAF" : \
    (d)== DISP_TRACTION						? "DISP_TRACTION" : \
    (d)== DISP_RADIO_NETWORK_LIST_UPDATED	? "DISP_RADIO_NETWORK_LIST_UPDATED" : \
    (d)== RAD_SOM_CONNECT_RQST              ? "RAD_SOM_CONNECT_RQST" : \
    (d)== RAD_CONNECT_RQST                  ? "RAD_CONNECT_RQST" : \
    (d)== RAD_DISCONNECT_RQST               ? "RAD_DISCONNECT_RQST" : \
    (d)== RAD_SEND_MSG_RQST                 ? "RAD_SEND_MSG_RQST" : \
    (d)== RAD_CONNECT                       ? "RAD_CONNECT" : \
    (d)== RAD_DISCONNECT                    ? "RAD_DISCONNECT" : \
    (d)== RAD_DATA                          ? "RAD_DATA" : \
    (d)== RAD_HP_DATA                       ? "RAD_HP_DATA" : \
    (d)== RAD_RBC_TRANSITION                ? "RAD_RBC_TRANSITION" : \
    (d)== RAD_SESSION_MNG                   ? "RAD_SESSION_MNG" : \
    (d)== RAD_END_OF_MISSION                ? "RAD_END_OF_MISSION" : \
    (d)== RAD_SEND_TRAIN_DATA               ? "RAD_SEND_TRAIN_DATA" : \
    (d)== RAD_DELETE_STORED_RBC_MA_DATA     ? "RAD_DELETE_STORED_RBC_MA_DATA" : \
    (d)== RAD_EXIT_RBC_AREA                 ? "RAD_EXIT_RBC_AREA" : \
    (d)== RIM_CONNECT                       ? "RIM_CONNECT" : \
    (d)== RIM_CONNECT_LIMITED               ? "RIM_CONNECT_LIMITED" : \
    (d)== RIM_DISCONNECT                    ? "RIM_DISCONNECT" : \
    (d)== RIM_NETWORK_REGISTER              ? "RIM_NETWORK_REGISTER" : \
    (d)== RIM_NETWORK_LIST                  ? "RIM_NETWORK_LIST" : \
    (d)== RIM_DATA                          ? "RIM_DATA" : \
    (d)== BAL_LINK_DATA                     ? "BAL_LINK_DATA" : \
    (d)== BAL_BIG_METAL_MASSES              ? "BAL_BIG_METAL_MASSES" : \
    (d)== BAL_LOOP_INFO                     ? "BAL_LOOP_INFO" : \
    (d)== BAL_BTM_ALARM_ON                  ? "BAL_BTM_ALARM_ON" : \
    (d)== BAL_BTM_ALARM_OFF                 ? "BAL_BTM_ALARM_OFF" : \
    (d)== BAL_VBC_DRIVER_ENTER              ? "BAL_VBC_DRIVER_ENTER" : \
    (d)== BAL_VBC_DRIVER_REMOVE             ? "BAL_VBC_DRIVER_REMOVE" : \
    (d)== BAL_VBC_TRACKSIDE                 ? "BAL_VBC_TRACKSIDE" : \
    (d)== MISC_TRAIN_LENGTH_INCREASED		? "MISC_TRAIN_LENGTH_INCREASED"

/// Enumeration for module ID used in internal communication
typedef enum
{
    ADDR_ALL       = 0  ,   ///< all modules
    ADDR_BALISE_ID = 1  ,   ///< Balise messages manager
    ADDR_DATASERVER_ID  ,   ///< Data manager
    ADDR_SPEEDSUP_ID    ,   ///< Supervision curves computor
    ADDR_RECORDER_ID    ,   ///< Recorder interface module
    ADDR_DISPLAY_ID     ,   ///< DMI interface module
    ADDR_SPEEDCTRL_ID   ,   ///< Motion and speed controller
    ADDR_ODO_ID         ,   ///< Odometer interface module
    ADDR_MODE_ID        ,   ///< Mode manager
    ADDR_RADIO_ID       ,   ///< Radio interface manager
    ADDR_RIM_ID         ,   ///< RIM interface manager
    ADDR_DUMMYCTRL_ID   ,   ///< Dummy controller
    ADDR_EVCCTRL_ID     ,   ///< EVC controller
    ADDR_TEST_ID        ,   ///< Used for testing
    ADDR_MAX_ID             ///< Max value

} eAddressId;

/// Macro to get the string corresponding to an address id
#define STR_MODULEID(d)        \
    (d)==ADDR_ALL               ?   "ADDR_ALL"              : \
    (d)==ADDR_BALISE_ID         ?   "ADDR_BALISE_ID"        : \
    (d)==ADDR_DATASERVER_ID     ?   "ADDR_DATASERVER_ID"    : \
    (d)==ADDR_SPEEDSUP_ID       ?   "ADDR_SPEEDSUP_ID"      : \
    (d)==ADDR_RECORDER_ID       ?   "ADDR_RECORDER_ID"      : \
    (d)==ADDR_DISPLAY_ID        ?   "ADDR_DISPLAY_ID"       : \
    (d)==ADDR_SPEEDCTRL_ID      ?   "ADDR_SPEEDCTRL_ID"     : \
    (d)==ADDR_ODO_ID            ?   "ADDR_ODO_ID"           : \
    (d)==ADDR_MODE_ID           ?   "ADDR_MODE_ID"          : \
    (d)==ADDR_RADIO_ID          ?   "ADDR_RADIO_ID"         : \
    (d)==ADDR_RIM_ID            ?   "ADDR_RIM_ID"           : \
    (d)==ADDR_DUMMYCTRL_ID      ?   "ADDR_DUMMYCTRL_ID"     : \
    (d)==ADDR_EVCCTRL_ID        ?   "ADDR_EVCCTRL_ID"       : \
    (d)==ADDR_TEST_ID           ?   "ADDR_TEST_ID"          : "???"

/// structure for transmission of internal message via message queue
typedef struct SIntMessage
{
    eAddressId     DestId       ;   ///< Identity of the receiver
    eAddressId     SendId       ;   ///< Identity of the sender
    int32_t        lMessSize    ;   ///< Message size in bytes
    eMsgCode       AppCod       ;   ///< Application code
}
SIntMessage;


/// Maximum size of the data exchanged between modules (in bytes)
#define MAX_DATA_SIZE        43000


/// Structure used to exchange internal message
typedef struct SInternalMsg
{
    eAddressId  DestId                  ;   ///< Identifier of the recipient
    eAddressId  SenderId                ;   ///< Identifier of the sender
    int32_t     lSize                   ;   ///< Size of the included data (in bytes)
    eMsgCode    AppCode                 ;   ///< Code of the message
    char        szData[MAX_DATA_SIZE]   ;   ///< Data included in the message

} SInternalMsg;


/// enumeration of type of communication
typedef enum
{
    NO_COM_TYPE     ,   ///< no communication
    COM_MSG_QUEUE   ,   ///< communication via message queue

} eComType;

/// Structure used to send /receive data via message queue
typedef struct SMsgQueueStruct
{
    uint8_t   szData[MAX_QUEUE_DATA_SIZE] ;   ///< data

} SMsgQueueStruct;

/// maximum size of pipe name
#define MAX_PIPE_NAME_SIZE      256

/// definition of recipient id for EVC - external communication via message queue: EVC
#define EVC_COM                 0

/// definition of recipient id for EVC - external communication via message queue: external
#define NO_EVC_COM              1

/// structure for communication management
typedef struct SComParam
{
    eComType        Type                                ;        ///< type of communication

    int32_t         lMsgQueueKey                        ;       ///< key for the ipc message queue
    int32_t         lQueueDestId                        ;       ///< Id of the recipient
    int32_t         lQueueSenderId                      ;       ///< Id of the sender

    void *          pPipe                               ;       ///< pointer on the pipe management
    char            szTxPipeName[MAX_PIPE_NAME_SIZE]    ;       ///< Name of the pipe used to send data
    char            szRxPipeName[MAX_PIPE_NAME_SIZE]    ;       ///< Name of the pipe used to receive data
} SComParam;

/// Enumeration for direction or data orientation
typedef enum eDirection
{
    NOMINAL     ,   ///< Nominal direction or forward direction
    REVERSE     ,   ///< Reverse direction or backward direction
    DIR_UNDEF   ,   ///< Unknown direction / orientation
    STANDSTILL      ///< Neutral or standstill

} eDirection;

/// Macro to get the string corresponding to the direction/orientation
#define STR_DIRECTION(d) \
    (d)==STANDSTILL ? "STANDSTILL"  : \
    (d)==NOMINAL    ? "NOMINAL"     : \
    (d)==REVERSE    ? "REVERSE"     : \
    "???"

/// enumeration for the list of different Operated System Version
typedef enum eOperatedSystemVersion
{
    OPERATED_SYSTEM_V1 = 1,
    OPERATED_SYSTEM_V2 = 2,
    OPERATED_SYSTEM_UNKNOWN

}  eOperatedSystemVersion;

/// Macro to get the string corresponding to the Operated System Version
#define STR_OP_SYST_VERSION(d)    \
    (d)==OPERATED_SYSTEM_V1         ? "OPERATED_SYSTEM_V1"      : \
    (d)==OPERATED_SYSTEM_V2         ? "OPERATED_SYSTEM_V2"      : \
    (d)==OPERATED_SYSTEM_UNKNOWN    ? "OPERATED_SYSTEM_UNKNOWN" : \
    "???"

/// structure for transmission of odometric data
typedef struct SOdoData
{
    double dLocation_m  ;   ///< current location (m)
    double dSpeed_m_s   ;   ///< current speed (m/s)
    double dGamma_m_s2  ;   ///< current acceleration (m/s²)

} SOdoData;

/// structure for transmission of Tiu data (EVC-->TRAIN)
typedef struct STxTiuData
{
    bool bCut_Off_App                    ; ///< Traction cut off request
    bool bSB_App                         ; ///< Service Brake intervention request
    bool bEB_App                         ; ///< Emergency Brake intervention request
    bool bOpenCircuitBreaker             ; ///< Main circuit breaker  open request
    bool bPantographLow                  ; ///< Pantograph low request
    bool bAirTightRqst                   ; ///< Air tight request
    bool bPermitRegenBrake               ; ///< Regenerative brake use permission
    bool bPermitEddyCurrBrakeForSB       ; ///< Eddy current brake use permission for SB
    bool bPermitEddyCurrBrakeForEB       ; ///< Eddy current brake use permission for EB
    bool bPermitMagnShoeBrake            ; ///< Magnetic shoe brake use permission
    bool bSwitchOffBalAntenna            ; ///< Balise antenna switch off
    int32_t lAllowedConsumption             ; ///< Maximum allowed power consumption
    bool bIsolationStatus                ; ///< Isolation status
    ePlatformElevation platformElevation ; ///< Track condition platform elevation
    ePlatformPosition platformPosition   ; ///< Track condition platform elevation
} STxTiuData;


/// structure for reception of Tiu data (TRAIN-->EVC)
typedef struct SRxTiuData
{
    bool    bMainSwitchOn       ; ///< Power status
    bool    bTrainIntegrityOK   ; ///< Train integrity status
    int32_t ActiveCabin         ; ///< Cabin status
    bool    bIsolation          ; ///< Isolation request status
    bool    bSleeping           ; ///< Sleeping signal status
    int32_t DirectionCtrl       ; ///< Direction controller status
    bool    bEBStatus           ; ///< EB status
    bool    bSBStatus           ; ///< SB status
    bool    bNonLeading         ; ///< Non leading permission signal status
    bool    bPassiveSH          ; ///< Passive Shunting permission signal status
    bool    bColdMvtStatus      ; ///< Cold movement detection status
    int32_t lTrainDataType      ; ///< Train data type status
    bool    bRegenBrake         ; ///< Regenerative brake availability
    bool    bMagnetShoeBrake    ; ///< Magnetic shoe brake availability
    bool    bEddyBrake          ; ///< Eddy current brake availability
    bool    bEpBrake            ; ///< Electro magnetic brake availability
    bool    bAddBrake           ; ///< Additional brake availability
    double  dBrakePressure      ; ///< Current brake pressure
    int32_t lTractBrakeHandlePos; ///< Current traction/brake handle position
    bool    bTractionOn         ; ///< Indicate if traction is on
    bool    bSystFailure        ; ///< Indicate a system failure

} SRxTiuData;

//===========================  DMI Actions  =======================

typedef enum eDriverActionInfo
{
    DMI_ACTION_DRIVER_ID,
    DMI_ACTION_TRAIN_RUNNING_NUMBER,
    DMI_ACTION_TRAIN_DATA,
    DMI_ACTION_SR_DATA,
    DMI_ACTION_LEVEL_0,
    DMI_ACTION_LEVEL_1,
    DMI_ACTION_LEVEL_2,
    DMI_ACTION_LEVEL_3,
    DMI_ACTION_LEVEL_MAX,
    DMI_ACTION_START_OF_MISSION,
    DMI_ACTION_SLIPPERY_TRACK,
    DMI_ACTION_NON_SLIPPERY_TRACK,
    DMI_ACTION_NON_LEADING,
    DMI_ACTION_NON_LEADING_EXIT,
    DMI_ACTION_OVERRIDE_EOA,
    DMI_ACTION_OVERRIDE_RS,
    DMI_ACTION_SHUNTING,
    DMI_ACTION_SHUNTING_EXIT,
    DMI_ACTION_TAF_ACK,
    DMI_ACTION_ACK_ICON,
    DMI_ACTION_ACK_TEXT,
    DMI_ACTION_ACK_BRAKE

} eDriverActionInfo;

/// Enumerarion for type of track condition
typedef enum eTCInfo
{
    TC_NON_STOP_TUNNEL                ,
    TC_NON_STOP_BRIDGE                ,
    TC_NON_STOP_UNKNOWN               ,
    TC_POWERLESS_LOW_PANTO            ,
    TC_RADIO_HOLE                     ,
    TC_AIR_TIGHTNESS                  ,
    TC_SWITCHOFF_REGENERATIVE_BRAKE   ,
    TC_SWITCHOFF_EDDY_CURRENT_BRAKE   ,
    TC_SWITCHOFF_MAGNETIC_SHOE_BRAKE  ,
    TC_MAIN_SWITCH_OFF                ,
    TC_BIG_METAL_MASS                 ,
    TC_CHANGE_TRACTION

} eTCInfo;

/// Maximu length of free text message
#define MAX_TEXT_LENGTH        256

typedef struct STrackCondLog
{
    t_distance      dStartLocation;
    t_distance      dEndLocation;
    eTCInfo         TCInfo;
    char            VarList[MAX_TEXT_LENGTH];

} STrackCondLog;

typedef struct SDmiActionLog
{
    t_distance          dLocation;
    t_time              dTimeStamp;
    eDriverActionInfo   DriverAction;
    char                VarList[MAX_TEXT_LENGTH];

}SDmiActionLog;

//===========================  Fixed text code (Q_TEXT)  =======================

typedef enum {
    SYS_INFO_CONFIRM_REVERSE = 1    ,   ///< confirm reversing
    SYS_INFO_ACK_SWTCH_OS           ,   ///< ack for switch to on sight mode
    SYS_INFO_ACK_SWTCH_SR           ,   ///< ack for switch to staff responsible mode
    SYS_INFO_ACK_SWTCH_UN           ,   ///< ack for switch to unfitted mode
    SYS_INFO_ACK_SWTCH_SH           ,   ///< ack for switch to shunting mode
    SYS_INFO_ACK_SWTCH_LS           ,   ///< ack for switch to limited supervision
    SYS_INFO_ACK_SH = 8             ,   ///< ack for shunting mode (from SB to SH)
    SYS_INFO_ACK_OS                 ,   ///< ack for shunting mode (from SB to SH)
    SYS_INFO_ACK_TRIP               ,   ///< ack of train trip
    SYS_INFO_TRANSITION_LEVEL_0     ,   ///< transition to level 0
    SYS_INFO_TRANSITION_LEVEL_1 = 13,   ///< transition to level 1
    SYS_INFO_TRANSITION_LEVEL_2     ,   ///< transition to level 2
    SYS_INFO_TRANSITION_LEVEL_3     ,   ///< transition to level 3
    SYS_INFO_ACK_ENTRY_L0           ,   ///< ack transition to level 0
    SYS_INFO_ACK_ENTRY_L1 = 18      ,   ///< ack transition to level 1
    SYS_INFO_ACK_ENTRY_L2           ,   ///< ack transition to level 2
    SYS_INFO_ACK_ENTRY_L3           ,   ///< ack transition to level 3
    SYS_INFO_OVERRIDE               ,   ///< Switch off traction system automatically
    SYS_INFO_TRACTION_NONE_AUTO     ,   ///< Switch off traction system by driver
    SYS_INFO_TRACTION_NONE_DRIVER   ,   ///< Switch traction 25KV by driver
    SYS_INFO_TRACTION_25KV_AUTO     ,   ///< Switch traction 25KV automatically
    SYS_INFO_TRACTION_25KV_DRIVER   ,   ///< Switch traction 15KV by driver
    SYS_INFO_TRACTION_15KV_AUTO     ,   ///< Switch traction 15KV automatically
    SYS_INFO_TRACTION_15KV_DRIVER   ,   ///< Switch traction 15KV by driver
    SYS_INFO_TRACTION_3000V_DRIVER  ,   ///< Switch traction 3000V by driver
    SYS_INFO_TRACTION_3000V_AUTO    ,   ///< Switch traction 3000V automatically
    SYS_INFO_TRACTION_1500V_DRIVER  ,   ///< Switch traction 1500KV by driver
    SYS_INFO_TRACTION_1500V_AUTO    ,   ///< Switch traction 1500V automatically
    SYS_INFO_TRACTION_750V_DRIVER   ,   ///< Switch traction 750V by driver
    SYS_INFO_TRACTION_750V_AUTO     ,   ///< Switch traction 750V automatically
    SYS_INFO_BRAKE                      ///< Emergency/Service brake
} eSystemInfo;

/// Macro to get the string corresponding to a text code
#define STR_SYSTEM_INFO(d) \
    (d)==SYS_INFO_CONFIRM_REVERSE             ?        "confirm reversing"                                                   : \
    (d)==SYS_INFO_ACK_SWTCH_OS                ?        "ack for switch to on sight mode"                                     : \
    (d)==SYS_INFO_ACK_SWTCH_SR                ?        "ack for switch to staff responsible mode"                            : \
    (d)==SYS_INFO_ACK_SWTCH_UN                ?        "ack for switch to unfitted mode"                                     : \
    (d)==SYS_INFO_ACK_SWTCH_SH                ?        "ack for switch to shunting mode"                                     : \
    (d)==SYS_INFO_ACK_SWTCH_LS                ?        "ack for switch to limited supervision mode"                          : \
    (d)==SYS_INFO_ACK_SH                      ?        "ack for shunting mode"                                               : \
    (d)==SYS_INFO_ACK_OS                      ?        "ack for on sight mode"                                               : \
    (d)==SYS_INFO_ACK_TRIP                    ?        "ack of train trip"                                                   : \
    (d)==SYS_INFO_TRANSITION_LEVEL_0          ?        "transition to level 0"                                               : \
    (d)==SYS_INFO_TRANSITION_LEVEL_1          ?        "transition to level 1"                                               : \
    (d)==SYS_INFO_TRANSITION_LEVEL_2          ?        "transition to level 2"                                               : \
    (d)==SYS_INFO_TRANSITION_LEVEL_3          ?        "transition to level 3"                                               : \
    (d)==SYS_INFO_ACK_ENTRY_L0                ?        "ack transition to level 0"                                           : \
    (d)==SYS_INFO_ACK_ENTRY_L1                ?        "ack transition to level 1"                                           : \
    (d)==SYS_INFO_ACK_ENTRY_L2                ?        "ack transition to level 2"                                           : \
    (d)==SYS_INFO_ACK_ENTRY_L3                ?        "ack transition to level 3"                                           : \
    (d)==SYS_INFO_OVERRIDE                    ?        "override"                                                            : \
    "???"

//=============================    System status message   ==============================


typedef enum eSystemStatusMesg
{
    SSM_BALISE_READ_ERROR       = 256,
    SSM_COM_ERROR               = 257,
    SSM_ENTERING_FS             = 258,
    SSM_ENTERING_OS             = 259,
    // = 260 Unused
    SSM_RUNAWAY_MOVEMENT        = 261,
    SSM_SH_REFUSED              = 262,
    SSM_TKS_NOT_COMPATIBLE      = 263,
    SSM_TRAIN_DATA_CHANGED      = 264,
    SSM_TRAIN_REJECTED          = 265,
    SSM_UNAUTH_PASS_EOA_LOA     = 266,
    SSM_NO_MA_RECV_AT_LTR       = 267,
    SSM_SR_DIST_EXCEED          = 268,
    SSM_SH_STOP_ORDER           = 269,
    SSM_SR_STOP_ORDER           = 270,
    SSM_EMERGENCY_STOP          = 271,
    SSM_TKS_MALFUNCTION         = 272,
    SSM_SH_REQUEST_FAILED       = 273,
    SSM_RV_DIST_EXCEEDING       = 274,
    SSM_NO_TRACK_DESC           = 275,
    SSM_NTC_BRAKE_DEMAND        = 276,
    SSM_ROUTE_UN_AXLE_LOAD      = 277,
    SSM_ROUTE_UN_LOADING_GAUGE  = 278,
    SSM_ROUTE_UN_TRACTION_SYS   = 279,
    SSM_RADIO_REG_FAILED        = 280

} eSystemStatusMesg;


#define STR_SYS_STATUS_MESG(d) \
    (d)==SSM_BALISE_READ_ERROR      ? "System status msg: BALISE_READ_ERROR"       : \
    (d)==SSM_COM_ERROR              ? "System status msg: COM_ERROR"               : \
    (d)==SSM_ENTERING_FS            ? "System status msg: ENTERING_FS"             : \
    (d)==SSM_ENTERING_OS            ? "System status msg: ENTERING_OS"             : \
    (d)==SSM_RUNAWAY_MOVEMENT       ? "System status msg: RUNAWAY_MOVEMENT"        : \
    (d)==SSM_SH_REFUSED             ? "System status msg: SH_REFUSED"              : \
    (d)==SSM_TKS_NOT_COMPATIBLE     ? "System status msg: TKS_NOT_COMPATIBLE"      : \
    (d)==SSM_TRAIN_DATA_CHANGED     ? "System status msg: TRAIN_DATA_CHANGED"      : \
    (d)==SSM_TRAIN_REJECTED         ? "System status msg: TRAIN_DATA_REJECTED"     : \
    (d)==SSM_UNAUTH_PASS_EOA_LOA    ? "System status msg: UNAUTH_PASS_EOA_LOA"     : \
    (d)==SSM_NO_MA_RECV_AT_LTR      ? "System status msg: NO_MA_RECV_AT_LTR"       : \
    (d)==SSM_SR_DIST_EXCEED         ? "System status msg: SR_DIST_EXCEED"          : \
    (d)==SSM_SH_STOP_ORDER          ? "System status msg: SH_STOP_ORDER"           : \
    (d)==SSM_SR_STOP_ORDER          ? "System status msg: SR_STOP_ORDER"           : \
    (d)==SSM_EMERGENCY_STOP         ? "System status msg: EMERGENCY_STOP"          : \
    (d)==SSM_TKS_MALFUNCTION        ? "System status msg: TKS_MALFUNCTION"         : \
    (d)==SSM_SH_REQUEST_FAILED      ? "System status msg: SH_REQUEST_FAILED"       : \
    (d)==SSM_RV_DIST_EXCEEDING      ? "System status msg: RV_DIST_EXCEEDING"       : \
    (d)==SSM_NO_TRACK_DESC          ? "System status msg: NO_TRACK_DESC"           : \
    (d)==SSM_NTC_BRAKE_DEMAND       ? "System status msg: NTC_BRAKE_DEMAND"        : \
    (d)==SSM_ROUTE_UN_AXLE_LOAD     ? "System status msg: ROUTE_UN_AXLE_LOAD"      : \
    (d)==SSM_ROUTE_UN_LOADING_GAUGE ? "System status msg: ROUTE_UN_LOADING_GAUGE"  : \
    (d)==SSM_ROUTE_UN_TRACTION_SYS  ? "System status msg: ROUTE_UN_TRACTION_SYS"   : \
    (d)==SSM_RADIO_REG_FAILED       ? "System status msg: RADIO_REGISTRATION_FAILED" : \
    "???"


/// Generic enumeration to store validity
// Used by train running number.
typedef enum eValidity
{
    VALID,
    INVALID,
    UNKNOWN

} eValidity;


//=============================       JRU      =====================================


/// Maximum size of JRU data
#define MAX_JRU_DATA_SIZE   2048


/// Enumeration for JRU Message types (Subset-027)
typedef enum eJRUMessageTypeID
{
    JRU_GENERAL_MESSAGE_ID              = 1 ,   ///< JRU general message
    JRU_TRAIN_DATA_MESSAGE_ID           = 2 ,   ///< JRU train data message
    JRU_EB_COMMAND_STATE_ID             = 3 ,   ///< JRU emergency brake command state message
    JRU_SB_COMMAND_STATE_ID             = 4 ,   ///< JRU service brake command state message
    JRU_MSG_TO_RIU_ID                   = 5 ,   ///< JRU message to radio infill unit message
    JRU_TELEGRAM_FROM_BALISE_ID         = 6 ,   ///< JRU telegram from balise message
    JRU_MSG_FROM_EUROLOOP_ID            = 7 ,   ///< JRU message from euroloop message
    JRU_MSG_FROM_RIU_ID                 = 8 ,   ///< JRU message from radio infill unit message
    JRU_MSG_FROM_RBC_ID                 = 9 ,   ///< JRU message from radio block centre message
    JRU_MSG_TO_RBC_ID                   = 10,   ///< JRU message to radio block centre message
    JRU_DRIVERS_ACTIONS_ID              = 11,   ///< JRU driver's actions message
    JRU_BALISE_GROUP_ERROR_ID           = 12,   ///< JRU balise group error message
    JRU_RADIO_ERROR_ID                  = 13,   ///< JRU radio error message
    JRU_INFO_FROM_COLD_MVT_DETECTOR_ID  = 15,   ///< JRU information from cold movement detector message
    JRU_START_DISP_FIXED_TEXT_ID        = 16,   ///< JRU start displaying fixed text message
    JRU_STOP_DISP_FIXED_TEXT_ID         = 17,   ///< JRU stop displaying fixed text message
    JRU_START_DISP_PLAIN_TEXT_ID        = 18,   ///< JRU start displaying plain text message
    JRU_STOP_DISP_PLAIN_TEXT_ID         = 19,   ///< JRU stop displaying fixed text message
    JRU_MONITORING_INFO_ID              = 20,   ///< JRU speed and distance monitoring information message
    JRU_DMI_SYMBOL_STATUS_ID            = 21,   ///< JRU dmi symbol status message
    JRU_DMI_SOUND_STATUS_ID             = 22,   ///< JRU dmi sound status message
    JRU_DMI_SYSTEM_STATUS_MSG_ID        = 23,   ///< JRU dmi system status message message
    JRU_ADDITIONAL_DATA_ID              = 24,   ///< JRU additional data message
    JRU_SR_DATA_ID                      = 25,   ///< JRU SR speed/distance entered by the driver message
    JRU_NTC_SELECTED_ID                 = 26,   ///< JRU ntc selected message
    JRU_SAFETY_CRITICAL_FAULT_ID        = 27,   ///< JRU safety critical fault in SL, NL or PS message
    JRU_VBC_SET_ID                      = 28,   ///< JRU virtual balise cover set by the driver message
    JRU_VBC_REMOVED_ID                  = 29,   ///< JRU virtual balise cover removed by the driver message
    JRU_SLEEPING_INPUT_ID               = 30,   ///< JRU sleeping input message
    JRU_PASSIVE_SHUNTING_ID             = 31,   ///< JRU passive shunting input message
    JRU_NON_LEADING_INPUT_ID            = 32,   ///< JRU non leading input message
    JRU_REGENERATIVE_BRAKE_STATUS_ID    = 33,   ///< JRU regenerative brake status message
    JRU_MAGNETICSHOE_BRAKE_STATUS_ID    = 34,   ///< JRU magnetic shoe brake status message
    JRU_EDDYCURRENT_BRAKE_STATUS_ID     = 35,   ///< JRU eddy current brake status message
    JRU_EP_BRAKE_STATUS_ID              = 36,   ///< JRU electro pneumatic brake status message
    JRU_ADDITIONAL_BRAKE_STATUS_ID      = 37,   ///< JRU additional brake status message
    JRU_CAB_STATUS_ID                   = 38,   ///< JRU cabin status message
    JRU_DIRECTION_CONTROLLER_POS_ID     = 39,   ///< JRU direction controller position message
    JRU_TRACTION_STATUS_ID              = 40,   ///< JRU traction status message
    JRU_TRAIN_DATA_TYPE_ID              = 41,   ///< JRU type of train data message
    JRU_NATIONAL_SYSTEM_ISOLATION_ID    = 42,   ///< JRU national system isolation message
    JRU_TCO_COMMAND_STATE_ID            = 43,   ///< JRU traction cut off command state message
    JRU_PROPRIETARY_DATA_ID             = 255   ///< JRU ETCS on-board proprietary juridical data

} eJRUMessageTypeID;

#define STR_JRU_MESSAGE(d) \
    (d)==JRU_GENERAL_MESSAGE_ID             ? "JRU_GENERAL_MESSAGE_ID"             : \
    (d)==JRU_TRAIN_DATA_MESSAGE_ID          ? "JRU_TRAIN_DATA_MESSAGE_ID"          : \
    (d)==JRU_EB_COMMAND_STATE_ID            ? "JRU_EB_COMMAND_STATE_ID"            : \
    (d)==JRU_SB_COMMAND_STATE_ID            ? "JRU_SB_COMMAND_STATE_ID"            : \
    (d)==JRU_MSG_TO_RIU_ID                  ? "JRU_MSG_TO_RIU_ID"                  : \
    (d)==JRU_TELEGRAM_FROM_BALISE_ID        ? "JRU_TELEGRAM_FROM_BALISE_ID"        : \
    (d)==JRU_MSG_FROM_EUROLOOP_ID           ? "JRU_MSG_FROM_EUROLOOP_ID"           : \
    (d)==JRU_MSG_FROM_RIU_ID                ? "JRU_MSG_FROM_RIU_ID"                : \
    (d)==JRU_MSG_FROM_RBC_ID                ? "JRU_MSG_FROM_RBC_ID"                : \
    (d)==JRU_MSG_TO_RBC_ID                  ? "JRU_MSG_TO_RBC_ID"                  : \
    (d)==JRU_DRIVERS_ACTIONS_ID             ? "JRU_DRIVERS_ACTIONS_ID"             : \
    (d)==JRU_BALISE_GROUP_ERROR_ID          ? "JRU_BALISE_GROUP_ERROR_ID"          : \
    (d)==JRU_RADIO_ERROR_ID                 ? "JRU_RADIO_ERROR_ID"                 : \
    (d)==JRU_INFO_FROM_COLD_MVT_DETECTOR_ID ? "JRU_INFO_FROM_COLD_MVT_DETECTOR_ID" : \
    (d)==JRU_START_DISP_FIXED_TEXT_ID       ? "JRU_START_DISP_FIXED_TEXT_ID"       : \
    (d)==JRU_STOP_DISP_FIXED_TEXT_ID        ? "JRU_STOP_DISP_FIXED_TEXT_ID"        : \
    (d)==JRU_START_DISP_PLAIN_TEXT_ID       ? "JRU_START_DISP_PLAIN_TEXT_ID"       : \
    (d)==JRU_STOP_DISP_PLAIN_TEXT_ID        ? "JRU_STOP_DISP_PLAIN_TEXT_ID"        : \
    (d)==JRU_MONITORING_INFO_ID             ? "JRU_MONITORING_INFO_ID"             : \
    (d)==JRU_DMI_SYMBOL_STATUS_ID           ? "JRU_DMI_SYMBOL_STATUS_ID"           : \
    (d)==JRU_DMI_SOUND_STATUS_ID            ? "JRU_DMI_SOUND_STATUS_ID"            : \
    (d)==JRU_DMI_SYSTEM_STATUS_MSG_ID       ? "JRU_DMI_SYSTEM_STATUS_MSG_ID"       : \
    (d)==JRU_ADDITIONAL_DATA_ID             ? "JRU_ADDITIONAL_DATA_ID"             : \
    (d)==JRU_SR_DATA_ID                     ? "JRU_SR_DATA_ID"                     : \
    (d)==JRU_NTC_SELECTED_ID                ? "JRU_NTC_SELECTED_ID"                : \
    (d)==JRU_SAFETY_CRITICAL_FAULT_ID       ? "JRU_SAFETY_CRITICAL_FAULT_ID"       : \
    (d)==JRU_VBC_SET_ID                     ? "JRU_VBC_SET_ID"                     : \
    (d)==JRU_VBC_REMOVED_ID                 ? "JRU_VBC_REMOVED_ID"                 : \
    (d)==JRU_SLEEPING_INPUT_ID              ? "JRU_SLEEPING_INPUT_ID"              : \
    (d)==JRU_PASSIVE_SHUNTING_ID            ? "JRU_PASSIVE_SHUNTING_ID"            : \
    (d)==JRU_NON_LEADING_INPUT_ID           ? "JRU_NON_LEADING_INPUT_ID"           : \
    (d)==JRU_REGENERATIVE_BRAKE_STATUS_ID   ? "JRU_REGENERATIVE_BRAKE_STATUS_ID"   : \
    (d)==JRU_MAGNETICSHOE_BRAKE_STATUS_ID   ? "JRU_MAGNETICSHOE_BRAKE_STATUS_ID"   : \
    (d)==JRU_EDDYCURRENT_BRAKE_STATUS_ID    ? "JRU_EDDYCURRENT_BRAKE_STATUS_ID"    : \
    (d)==JRU_EP_BRAKE_STATUS_ID             ? "JRU_EP_BRAKE_STATUS_ID"             : \
    (d)==JRU_ADDITIONAL_BRAKE_STATUS_ID     ? "JRU_ADDITIONAL_BRAKE_STATUS_ID"     : \
    (d)==JRU_CAB_STATUS_ID                  ? "JRU_CAB_STATUS_ID"                  : \
    (d)==JRU_DIRECTION_CONTROLLER_POS_ID    ? "JRU_DIRECTION_CONTROLLER_POS_ID"    : \
    (d)==JRU_TRACTION_STATUS_ID             ? "JRU_TRACTION_STATUS_ID"             : \
    (d)==JRU_TRAIN_DATA_TYPE_ID             ? "JRU_TRAIN_DATA_TYPE_ID"             : \
    (d)==JRU_NATIONAL_SYSTEM_ISOLATION_ID   ? "JRU_NATIONAL_SYSTEM_ISOLATION_ID"   : \
    (d)==JRU_TCO_COMMAND_STATE_ID           ? "JRU_TCO_COMMAND_STATE_ID"           : \
    (d)==JRU_PROPRIETARY_DATA_ID            ? "JRU_PROPRIETARY_DATA_ID"            : "???"

/// Structure for time stamping
typedef struct SClearTime
{
    int32_t lYear      ;   ///< value of year   (ie 2008)
    int32_t lMonth     ;   ///< value of month  (1 - 12)
    int32_t lMDay      ;   ///< value of day    (1 - 31)

    int32_t lHour      ;   ///< value of hour   (0 - 23)
    int32_t lMinutes   ;   ///< value of minute (0 - 59)
    int32_t lSeconds   ;   ///< value of second (0 - 59)

    int32_t lMSeconds  ;   ///< value of milli-seconds (0 - 99)
}
SClearTime;

/// Structure for JRU event sending (internal communication)
typedef struct sJruEventData
{
    eJRUMessageTypeID   JruEventType;                   ///< JrRU event type
    SClearTime          JruEventTimestamp;              ///< Time stamp at which the JRU event occured
    uint8_t             uszJruData[MAX_JRU_DATA_SIZE];  ///< Data array of the JRU event
    int32_t             lSize;                          ///< Size of the data

} sJruEventData;

/// Enumeration for JRU driver action
typedef enum eJRUDriverActionType
{
    JRU_DRV_ACT_ACK_OS              = 0 ,   ///< Ack of On Sight mode
    JRU_DRV_ACT_ACK_SH              = 1 ,   ///< Ack of Shunting mode
    JRU_DRV_ACT_ACK_TR              = 2 ,   ///< Ack of Train Trip
    JRU_DRV_ACT_ACK_SR              = 3 ,   ///< Ack of Staff Responsible mode
    JRU_DRV_ACT_ACK_UN              = 4 ,   ///< Ack of Unfitted mode
    JRU_DRV_ACT_ACK_RV              = 5 ,   ///< Ack of Reversing mode
    JRU_DRV_ACT_ACK_L0              = 6 ,   ///< Ack of level 0
    JRU_DRV_ACT_ACK_L1              = 7 ,   ///< Ack of level 1
    JRU_DRV_ACT_ACK_L2              = 8 ,   ///< Ack of level 2
    JRU_DRV_ACT_ACK_L3              = 9 ,   ///< Ack of level 3
    JRU_DRV_ACT_ACK_LNTC            = 10,   ///< Ack of level NTC
    JRU_DRV_ACT_SH                  = 11,   ///< Shunting selected
    JRU_DRV_ACT_NL                  = 12,   ///< Non Leading selected
    JRU_DRV_ACT_ACK_LS              = 13,   ///< Ack of Limited Supervision mode
    JRU_DRV_ACT_OVERRIDE            = 14,   ///< Override selected
    JRU_DRV_ACT_CONTINUE_SH         = 15,   ///< "Continue Shunting on desk closure" selected
    JRU_DRV_ACT_BRK_REL             = 16,   ///< Brake release acknowledgement
    JRU_DRV_ACT_EXIT_SH             = 17,   ///< Exit of Shunting selected
    JRU_DRV_ACT_IS                  = 18,   ///< Isolation selected
    JRU_DRV_ACT_SOM                 = 19,   ///< Start selected
    JRU_DRV_ACT_DATA_ENTRY          = 20,   ///< Train Data Entry requested
    JRU_DRV_ACT_DRIVER_CONFIRM_DATA = 21,   ///< Validation of train data
    JRU_DRV_ACT_CONFIRM_TAF         = 22,   ///< Confirmation of track ahead free
    JRU_DRV_ACT_ACK_PLAIN_TXT       = 23,   ///< Ack of Plain Text information
    JRU_DRV_ACT_ACK_FIXED_TXT       = 24,   ///< Ack of Fixed Text information
    JRU_DRV_ACT_HIDE_LIMITS         = 25,   ///< Request to hide supervision limits
    JRU_DRV_ACT_CONF_INTEGRITY      = 26,   ///< Train integrity confirmation
    JRU_DRV_ACT_SHOW_LIMITS         = 27,   ///< Request to show supervision limits
    JRU_DRV_ACT_ACK_SN              = 28,   ///< Ack of SN mode
    JRU_DRV_ACT_LANGUAGE            = 29,   ///< Selection of language
    JRU_DRV_ACT_SHOW_GEO_POS        = 30,   ///< Request to show geographical position
    JRU_DRV_ACT_HIDE_GEO_POS        = 33,   ///< Request to hide geographical position
    JRU_DRV_ACT_L0                  = 34,   ///< Level 0 selected
    JRU_DRV_ACT_L1                  = 35,   ///< Level 1 selected
    JRU_DRV_ACT_L2                  = 36,   ///< Level 2 selected
    JRU_DRV_ACT_L3                  = 37,   ///< Level 3 selected
    JRU_DRV_ACT_LNTC                = 38,   ///< Level NTC selected
    JRU_DRV_ACT_SHOW_TSA_INFO       = 39,   ///< Request to show tunnel stopping area information
    JRU_DRV_ACT_HIDR_TSA_INFO       = 40,   ///< Request to hide tunnel stopping area information
    JRU_DRV_ACT_SCROLL_UP           = 41,   ///< Scroll up button activated
    JRU_DRV_ACT_SCROLL_DOWN         = 42    ///< Scroll down button activated

} eJRUDriverActionType;


// Enumeration for DMI symbol status type (in order to divide bit field on 86 bits)
typedef enum eJruDmiSymbolStatusType
{
    JRU_DMI_SYMBOL_STATUS_LEVEL,            ///< Level symbols (LE01-05) - respect bit field description of level icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_LEVEL_TRANSITION, ///< Level transition symbols (LE06-15) - respect bit field description of level icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_MODE,             ///< Mode symbols (MO01,04,06,07,09,11,12,13,14,16,18,19,21) - respect bit field description of mode icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_MODE_ACK,         ///< Mode ack symbols (MO02,03,05,08,10,15,17,20,22) - respect bit field description of mode icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_STATUS,           ///< Status symbols (ST01,02,03,04,06) - respect bit field description of status icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_STATUS_HOURGLASS, ///< hourglass symbol (ST05) - respect bit field description of status icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_TC1,              ///< Track condition symbols (TC01-32) - respect bit field description of track condition icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_TC2,              ///< Track condition symbols (TC33-35) - respect bit field description of track condition icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_TSA,              ///< Tunnel stopping area icons (DR05,TC36,37) - specific bit field: DR05 TC36 TC37
    JRU_DMI_SYMBOL_STATUS_DRIVER_TOGGLE,    ///< Driver symbols (DR01) - respect bit field description of driver icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_DRIVER_TAF,       ///< Driver symbols (DR02) - respect bit field description of driver icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_DRIVER_GEOPOS,    ///< Driver symbols (DR03) - respect bit field description of driver icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_DRIVER_ACK,       ///< Driver symbols (DR04) - respect bit field description of driver icons in DMI_SYMBOL_STATUS JRU variable
    JRU_DMI_SYMBOL_STATUS_LX                ///< Level crossing symbol (LX01)

} eJruDmiSymbolStatusType;

/// Structure DMI symbol status
typedef struct sJruDmiSymbolStatus
{
    eJruDmiSymbolStatusType DmiSymbolStatusType;    ///< DMI symbol status type
    int32_t lDmiSymbolStatusBitfield;                  ///< Associated bit field

} sJruDmiSymbolStatus;

/// Enumeration for SYSTEM_STATUS_MESSAGE masks
typedef enum eJruSystemStatusMsgMask
{
    JRU_SSM_BALISE_READ_ERROR       = 0x000001, ///< Bit 01 - Balise read error
    JRU_SSM_TKS_MALFUNCTION         = 0x000002, ///< Bit 02 - Trackside malfunction
    JRU_SSM_COM_ERROR               = 0x000004, ///< Bit 03 - Communication error
    JRU_SSM_ENTERING_FS             = 0x000008, ///< Bit 04 - Entering FS
    JRU_SSM_ENTERING_OS             = 0x000010, ///< Bit 05 - Entering OS
    JRU_SSM_RUNAWAY_MOUVEMENT       = 0x000020, ///< Bit 06 - Runaway movement
    JRU_SSM_SH_REFUSED              = 0x000040, ///< Bit 07 - SH refused
    JRU_SSM_SH_REQUEST_FAILED       = 0x000080, ///< Bit 08 - SH request failed
    JRU_SSM_TKS_NOT_COMPATIBLE      = 0x000100, ///< Bit 09 - Trackside not compatible
    JRU_SSM_TRAIN_DATA_CHANGED      = 0x000200, ///< Bit 10 - Train data changed
    JRU_SSM_TRAIN_REJECTED          = 0x000400, ///< Bit 11 - Train data rejected
    JRU_SSM_UNAUTH_PASS_EOA_LOA     = 0x000800, ///< Bit 12 - Unauthorized passing of EOA/LOA
    JRU_SSM_NO_MA_RECV_AT_LTR       = 0x001000, ///< Bit 13 - No MA received at level transition
    JRU_SSM_SR_DIST_EXCEED          = 0x002000, ///< Bit 14 - SR distance exceeded
    JRU_SSM_SH_STOP_ORDER           = 0x004000, ///< Bit 15 - SH stop order
    JRU_SSM_SR_STOP_ORDER           = 0x008000, ///< Bit 16 - SR stop order
    JRU_SSM_EMERGENCY_STOP          = 0x010000, ///< Bit 17 - Emergency stop
    JRU_SSM_RV_DIST_EXCEEDED        = 0x020000, ///< Bit 18 - RV distance exceeded
    JRU_SSM_NO_TRACK_DESC           = 0x040000, ///< Bit 19 - No track description
    JRU_SSM_ROUTE_UN_AXLE_LOAD      = 0x080000, ///< Bit 20 - Route unsuitable - axle load category
    JRU_SSM_ROUTE_UN_LOADING_GAUGE  = 0x100000, ///< Bit 21 - Route unsuitable - loading gauge
    JRU_SSM_ROUTE_UN_TRACTION_SYS   = 0x200000, ///< Bit 22 - Route unsuitable - traction system
    JRU_SSM_RADIO_REG_FAILED        = 0x400000  ///< Bit 23 - Radio network registration failed

} eJruSystemStatusMsgMask;

/// Enumeration for RBC entry type
typedef enum eJruRbcEntryType
{
    JRU_RBC_LASTRBC_ENTRY       = 0,    ///< Contact last known RBC
    JRU_RBC_SHORTNUMBER_ENTRY   = 1,    ///< Use short number
    JRU_RBC_RBCDATA_ENTRY       = 2,    ///< Enter RBC data
    JRU_RBC_NO_ENTRY                    ///< No RBC entry

} eJruRbcEntryType;
/// Enumeration for type of data message transmitted to Ihm
typedef enum eMSG_TYPE
{
    MSG_BALISE      ,   ///< Balise message
    MSG_LOOP        ,   ///< Loop message
    MSG_RADIO       ,   ///< Radio message
    MSG_MMI         ,   ///< DMI message
    MSG_INFO2           ///< Informative message

} eMSG_TYPE;

/// Structure for exchange of data with IHM
typedef struct SIHMData
{
    int32_t lCode;
    int32_t lLength;
    char szData[MAX_IHM_DATA_SIZE];

} SIHMData;


/// Macro to get the string corresponding the type of message for the IHM
#define STR_MSG_TYPE(d)\
    (d)==MSG_BALISE     ? "Balise"      : \
    (d)==MSG_LOOP       ? "Loop"        : \
    (d)==MSG_RADIO      ? "Radio"       : \
    (d)==MSG_INFO2       ? "Info"        : \
    (d)==MSG_MMI        ? "DMI"         : \
    "???"

/// Structure for record of radio error
typedef struct SBaliseError
{
    int32_t lBGId      ;   ///< BG identity
    int32_t lErrorCode ;   ///< code of error

} SBaliseError;

/// Structure for record of radio error
typedef struct SRadioError
{
    int32_t lRBCId     ;   ///< RBC identity
    int32_t lErrorCode ;   ///< code of error

} SRadioError;


//=======================       data exchanged with RIM     =======================


/// Size of phone number in digit
#define PHONE_NB_SIZE           16

/// Maximum size of radio message (in bits)
#define MAX_RADIO_MSG_SIZE        1024


/// Structure for exchange of data with rim interface
typedef struct SRIMData
{
    int32_t lRadioEquipmentIndex            ;   ///< Index of used radio equipment
    uint8_t   uszRadioMsg[MAX_RADIO_MSG_SIZE] ;   ///< Radio message (SRS binary format)
    int32_t lRadioMsgSize                   ;   ///< Length of radio message (in bytes)

} SRIMData;


//============================       train reaction     =================================


/// Enumeration for train reaction to an error (balise linking, supervision of radio link, ...)
typedef enum eReaction
{
    TRAIN_TRIP  ,   ///< Train trip required
    APPLY_SB    ,   ///< Application of service brake required
    NO_REACTION     ///< No reaction required

} eReaction;


/// Macro to get the string corresponding to a train reaction
#define STR_REACTION(r) \
    r==TRAIN_TRIP   ? "TRAIN TRIP"  : \
    r==APPLY_SB     ? "APPLY SB"    : \
    r==NO_REACTION  ? "NO REACTION" : \
    "???"


//=======================    Code for error report to RBC     =======================

///< Enumeration for error code used in report for RBC
typedef enum eM_Error
{
    BG_LINKING_ERROR            = 0,    ///< Balise group : linking consistency error (ref. 3.16.2.3)
    BG_MSG_ERROR                = 1,    ///< Linked balise group : message consistency error (ref. 3.16.2.4.1/4)
    UNLINKED_BG_MSG_ERROR       = 2,    ///< Unlinked balise group : message consistency error (ref. 3.16.2.5)
    RADIO_MSG_ERROR             = 3,    ///< Radio : message consistency error (ref. 3.16.3.1.1a,c)
    RADIO_SEQUENCE_ERROR        = 4,    ///< Radio : sequence error (ref. 3.16.3.1.1b)
    RADIO_LINK_ERROR            = 5,    ///< Radio : radio link error (ref. 3.16.3.4, to be sent when communication links re-established)
    SAFETY_CRITICAL_FAILURE     = 6,    ///< Safety Critical Failure (ref. 4.4.6.1.6, 4.4.15.1.5, 4.4.20.1.10)
    DOUBLE_LINKING_ERROR        = 7,    ///< Double linking error (ref. 3.16.2.7.1)
    DOUBLE_REPOSITIONNING_ERROR = 8,    ///< Double repositionning error (ref. 3.16.2.7.2)
    NO_ERR                              ///< No Error

} eM_Error;

/// Macro to get the string corresponding to an error code
#define STR_ERROR(err) \
    err==BG_LINKING_ERROR           ? "BG_LINKING_ERROR" : \
    err==BG_MSG_ERROR               ? "BG_MSG_ERROR    " : \
    err==UNLINKED_BG_MSG_ERROR      ? "UNLINKED_BG_MSG_ERROR " : \
    err==RADIO_MSG_ERROR            ? "RADIO_MSG_ERROR     " : \
    err==RADIO_SEQUENCE_ERROR       ? "RADIO_SEQUENCE_ERROR" : \
    err==RADIO_LINK_ERROR           ? "RADIO_LINK_ERROR    " : \
    err==SAFETY_CRITICAL_FAILURE    ? "SAFETY_CRITICAL_FAILURE" : \
    err==DOUBLE_LINKING_ERROR       ? "DOUBLE_LINKING_ERROR" : \
    err==DOUBLE_REPOSITIONNING_ERROR? "DOUBLE_REPOSITIONNING_ERROR" : \
    "NO_ERROR"



//=======================    SRS profiles     =======================


/// Structure to store gradient and speed profiles
typedef struct SProfile
{
    int32_t lNbValues                               ;   ///< Number of discontinuities
    t_distance  adDistance[MAX_PROFILE_DISCONTINUITIES] ;   ///< Array of locations
    double      adValue   [MAX_PROFILE_DISCONTINUITIES] ;   ///< Array of values (speed in m/s, gradient without an unit)
    bool        bInfinite                               ;   ///< Indicate if the profile is not distance limited

} SProfile;


//=======================    balise group data     =======================

/// Special value indicating LRBG unknown
#define LRBG_UNKNOWN            16777215

#define UNKNOWN_COUNTRY         2147483647

/// Structure used to store information about balise group
typedef struct SBG_Id
{
    uint32_t ulCountryId ;       ///< Country id (NID_C)
    uint32_t ulGroupId   ;       ///< Group id (NID_BG)
    eDirection  Orientation ;   ///< Orientation of the balise group

} SBG_Id;


//=======================    ETCS mode & level     =======================


/// Enumeration for Internal modes of the trainborne equipment
/// (values are set to represent the equivalent M_MODE SRS variable)
typedef enum eTrainMode
{
    MODE_FULL           = 0 ,   ///< Full supervision
    MODE_ON_SIGHT       = 1 ,   ///< On sight
    MODE_STAFF_RESP     = 2 ,   ///< Staff responsible
    MODE_SHUNTING       = 3 ,   ///< Shunting
    MODE_UNFITTED       = 4 ,   ///< Unfitted
    MODE_SLEEPING       = 5 ,   ///< Sleeping
    MODE_STAND_BY       = 6 ,   ///< Stand by
    MODE_TRIP           = 7 ,   ///< Trip
    MODE_POST_TRIP      = 8 ,   ///< Post trip
    MODE_SYST_FAILURE   = 9 ,   ///< System failure
    MODE_ISOLATION      = 10,   ///< Isolation
    MODE_NON_LEADING    = 11,   ///< Non leading
    MODE_LIMITED_SUP    = 12,   ///< Limited supervision
    MODE_REVERSING      = 14,   ///< Reversing
    MODE_PASSIVE_SH     = 15,   ///< Passive shunting
    MODE_UNKNOWN            ,   ///< Unknown (special value)
    MODE_NO_POWER           ,   ///< No power
    MODE_MAX_NB                 ///< maximum number of mode value

} eTrainMode;


/// Macro to get the string corresponding to an ETCS mode
#define STR_MODE(d)            \
    (d)==MODE_NO_POWER      ? "No Power"            : \
    (d)==MODE_STAND_BY      ? "Stand By"            : \
    (d)==MODE_FULL          ? "Full Supervision"    : \
    (d)==MODE_ON_SIGHT      ? "On Sight"            : \
    (d)==MODE_STAFF_RESP    ? "Staff Responsible"   : \
    (d)==MODE_SHUNTING      ? "Shunting"            : \
    (d)==MODE_UNFITTED      ? "Unfitted"            : \
    (d)==MODE_SLEEPING      ? "Sleeping"            : \
    (d)==MODE_TRIP          ? "Trip"                : \
    (d)==MODE_POST_TRIP     ? "Post Trip"           : \
    (d)==MODE_SYST_FAILURE  ? "Failure"             : \
    (d)==MODE_REVERSING     ? "Reversing"           : \
    (d)==MODE_ISOLATION     ? "Isolation"           : \
    (d)==MODE_NON_LEADING   ? "Non Leading"         : \
    (d)==MODE_LIMITED_SUP   ? "Limited Supervision" : \
    (d)==MODE_PASSIVE_SH    ? "Passive Shunting"    : \
    "???"

#define STR_MODE_SHORT(d)        \
    (d)==MODE_NO_POWER      ? "NP" : \
    (d)==MODE_STAND_BY      ? "SB" : \
    (d)==MODE_FULL          ? "FS" : \
    (d)==MODE_ON_SIGHT      ? "OS" : \
    (d)==MODE_STAFF_RESP    ? "SR" : \
    (d)==MODE_SHUNTING      ? "SH" : \
    (d)==MODE_SLEEPING      ? "SL" : \
    (d)==MODE_UNFITTED      ? "UN" : \
    (d)==MODE_TRIP          ? "TR" : \
    (d)==MODE_POST_TRIP     ? "PT" : \
    (d)==MODE_SYST_FAILURE  ? "SF" : \
    (d)==MODE_REVERSING     ? "RV" : \
    (d)==MODE_ISOLATION     ? "IS" : \
    (d)==MODE_NON_LEADING   ? "NL" : \
    (d)==MODE_LIMITED_SUP   ? "LS" : \
    (d)==MODE_PASSIVE_SH    ? "PS" : \
    "???"

/// Enumeration for ETCS level
typedef enum eLevel
{
    LEVEL_0         = 0             ,   ///< Level 0
    LEVEL_1         = 2             ,   ///< Level 1
    LEVEL_2         = 3             ,   ///< Level 2
    LEVEL_3         = 4             ,   ///< Level 3
    LEVEL_UNKNOWN   = 5             ,   ///< Level unknown (special value)
    NB_LEVEL        = LEVEL_UNKNOWN     ///< Number of different level

} eLevel;


/// Macro to get the string corresponding to ETCS level
#define STR_LEVEL( d ) \
    d == LEVEL_0    ? "L0"      : \
    d == LEVEL_1    ? "L1"      : \
    d == LEVEL_2    ? "L2"      : \
    d == LEVEL_3    ? "L3"      : "??"

/// Macro to get the string corresponding to ETCS level (level number only)
#define STR_LEVEL_NB( d ) \
    d == LEVEL_0    ? "0"      : \
    d == LEVEL_1    ? "1"      : \
    d == LEVEL_2    ? "2"      : \
    d == LEVEL_3    ? "3"      : "??"

///< Structure used to store level
typedef struct SLevel
{
    eLevel      Level;      ///< ETCS level
} SLevel;


///< Maximum number of levels (0, 1, 2 or 3)
#define MAX_LEVEL_NB    NB_LEVEL-1

///< Maximum iterations in packets
#define MAX_ITERATIONS  32


/// Enumeration for level transition
typedef enum eLevelTr
{
    LTR_NONE    ,   ///< No level transition
    LTR_L0_L1 = 2,  ///< Level transition: L0  --> L1
    LTR_L0_L2   ,   ///< Level transition: L0  --> L2
    LTR_L0_L3   ,   ///< Level transition: L0  --> L3
    LTR_L1_L0 = 10, ///< Level transition: L1  --> L0
    LTR_L1_L2 = 12, ///< Level transition: L1  --> L2
    LTR_L1_L3   ,   ///< Level transition: L1  --> L3
    LTR_L2_L0   ,   ///< Level transition: L2  --> L0
    LTR_L2_L1 = 16, ///< Level transition: L2  --> L1
    LTR_L2_L3   ,   ///< Level transition: L2  --> L3
    LTR_L3_L0   ,   ///< Level transition: L3  --> L0
    LTR_L3_L1 = 20, ///< Level transition: L3  --> L1
    LTR_L3_L2       ///< Level transition: L3  --> L2

} eLevelTr;

#define STR_LEVELTR( d ) \
    d == LTR_NONE    ? "NO LTR"     : \
    d == LTR_L0_L1   ? "L0 -> L1"   : \
    d == LTR_L0_L2   ? "L0 -> L2"   : \
    d == LTR_L0_L3   ? "L0 -> L3"   : \
    d == LTR_L1_L0   ? "L1 -> L0"   : \
    d == LTR_L1_L2   ? "L1 -> L2"   : \
    d == LTR_L1_L3   ? "L1 -> L3"   : \
    d == LTR_L2_L0   ? "L2 -> L0"   : \
    d == LTR_L2_L1   ? "L2 -> L1"   : \
    d == LTR_L2_L3   ? "L2 -> L3"   : \
    d == LTR_L3_L0   ? "L3 -> L0"   : \
    d == LTR_L3_L1   ? "L3 -> L1"   : \
    d == LTR_L3_L2   ? "L3 -> L2"   : "???"

///< Structure used to store level transition
typedef struct SLevelTransition
{
    eLevelTr    LevelTr;        ///< ETCS level transition
    eLevel      PreviousLevel;  ///< identifier of previous level
    eLevel      NextLevel;      ///< identifier of futur level

} SLevelTransition;

/// Enumeration for EVC status (internal)
typedef enum eEVCStatus
{
    OBS_NO_POWER                    ,   ///< EVC in NP
    OBS_SELF_TEST                   ,   ///< EVC in SB, self test in progress
    OBS_WAIT_CABIN                  ,   ///< EVC in SB, wait for cabin open
    OBS_WAIT_RADIO_RELEASE          ,   ///< EVC in SB, wait for pending communication session and/or radio safe connection to be released before engaging SoM
    OBS_WAIT_DRIVER_ID              ,   ///< EVC in SB, wait for driver id entry
    OBS_WAIT_TRAIN_RUN_NB           ,   ///< EVC in SB, manage the train running number
    OBS_WAIT_LEVEL                  ,   ///< EVC in SB, wait for level entry
    OBS_WAIT_SESSION_SOM_OPEN       ,   ///< EVC in SB, wait for radio session open
    OBS_WAIT_SESSION_OPEN           ,   ///< EVC in SB, wait for radio session open
    OBS_WAIT_SESSION_CHANGE         ,   ///< EVC in SB, wait for current radio session to be disconnected, and then to establish a new session with new radio data.
    OBS_MNG_SOM_PR                  ,   ///< EVC in SB, manage SoM position report
    OBS_WAIT_DRIVER_SELECTION       ,   ///< EVC in SB or PT, wait for driver selection (train data, non-leading, shunting, level entry or override)
    OBS_WAIT_DRIVER_START_SELECTION ,   ///< EVC in SB or PT, wait for driver selection (train data, non-leading, shunting, start or override)
    OBS_MNG_TRAIN_DATA              ,   ///< EVC in SB or PT, manage the train data entry
    OBS_WAIT_TR_DAT_RBC_ACK         ,   ///< EVC in SB or PT, wait for train data acknowledgement from RBC
    OBS_WAIT_RBC_AUTHORISATION      ,   ///< EVC in SB or PT, wait RBC movement authorisation
    OBS_WAIT_UN_ACK                 ,   ///< EVC in SB or PT, wait driver acknowledgement of UN
    OBS_WAIT_SR_ACK                 ,   ///< EVC in SB or PT, wait driver acknowledgement of SR
    OBS_WAIT_SH_ACK                 ,   ///< EVC in SB or PT, wait driver acknowledgement of SH
    OBS_WAIT_OS_ACK                 ,   ///< EVC in SB or PT, wait driver acknowledgement of OS
    OBS_WAIT_LS_ACK                 ,   ///< EVC in SB or PT, wait driver acknowledgement of LS
    OBS_WAIT_SN_ACK                 ,   ///< EVC in SB or PT, wait driver acknowledgement of SN
    OBS_SH                          ,   ///< EVC in SH
    OBS_PS                          ,   ///< EVC in Passive Shunting
    OBS_UN                          ,   ///< EVC in UN
    OBS_SR                          ,   ///< EVC in SR
    OBS_OS                          ,   ///< EVC in OS
    OBS_LS                          ,   ///< EVC in LS
    OBS_FS                          ,   ///< EVC in FS
    OBS_TRIP                        ,   ///< EVC in TR
    OBS_PT_WAIT_RBC_ACK             ,   ///< EVC in PT, wait for RBC acknowledgement of exit from TR
    OBS_PT_WAIT_EMER_STOP_REVOC     ,   ///< EVC in PT, wait for revocation of emergency stop
    OBS_PT                          ,   ///< EVC in PT
    OBS_SH_WAIT_RBC_TERMINATE       ,   ///< EVC in SH, wait for RBC terminate session (SRS300 5.6.2 - A095, E055, A115)
    OBS_ISOLATION                   ,   ///< EVC in IS
    OBS_FAILURE                     ,   ///< EVC in SF
    OBS_SLEEPING                    ,   ///< EVC in SL
    OBS_NON_LEADING                 ,   ///< EVC in NL
    OBS_REVERSING = 39              ,   ///< EVC in RV
    OBS_UNKNOWN                         ///< EVC status unknown

} eEVCStatus;


/// Macro to get the string corresponding to an EVC status
#define STR_STATUS(d) \
    (d)== OBS_NO_POWER                      ? "OBS_NO_POWER"                    : \
    (d)== OBS_SELF_TEST                     ? "OBS_SELF_TEST"                   : \
    (d)== OBS_WAIT_CABIN                    ? "OBS_WAIT_CABIN"                  : \
    (d)== OBS_WAIT_RADIO_RELEASE            ? "OBS_WAIT_RADIO_RELEASE"          : \
    (d)== OBS_WAIT_DRIVER_ID                ? "OBS_WAIT_DRIVER_ID"              : \
    (d)== OBS_WAIT_TRAIN_RUN_NB             ? "OBS_WAIT_TRAIN_RUN_NB"           : \
    (d)== OBS_WAIT_LEVEL                    ? "OBS_WAIT_LEVEL"                  : \
    (d)== OBS_WAIT_SESSION_OPEN             ? "OBS_WAIT_SESSION_OPEN"           : \
    (d)== OBS_WAIT_SESSION_CHANGE           ? "OBS_WAIT_SESSION_CHANGE"         : \
    (d)== OBS_MNG_SOM_PR                    ? "OBS_MNG_SOM_PR"                  : \
    (d)== OBS_WAIT_DRIVER_SELECTION         ? "OBS_WAIT_DRIVER_SELECTION"       : \
    (d)== OBS_WAIT_DRIVER_START_SELECTION   ? "OBS_WAIT_DRIVER_START_SELECTION" : \
    (d)== OBS_MNG_TRAIN_DATA                ? "OBS_MNG_TRAIN_DATA"              : \
    (d)== OBS_WAIT_TR_DAT_RBC_ACK           ? "OBS_WAIT_TR_DAT_RBC_ACK"         : \
    (d)== OBS_WAIT_RBC_AUTHORISATION        ? "OBS_WAIT_RBC_AUTHORISATION"      : \
    (d)== OBS_WAIT_UN_ACK                   ? "OBS_WAIT_UN_ACK"                 : \
    (d)== OBS_WAIT_SR_ACK                   ? "OBS_WAIT_SR_ACK"                 : \
    (d)== OBS_WAIT_SH_ACK                   ? "OBS_WAIT_SH_ACK"                 : \
    (d)== OBS_WAIT_OS_ACK                   ? "OBS_WAIT_OS_ACK"                 : \
    (d)== OBS_WAIT_LS_ACK                   ? "OBS_WAIT_LS_ACK"                 : \
    (d)== OBS_WAIT_SN_ACK                   ? "OBS_WAIT_SN_ACK"                 : \
    (d)== OBS_SH                            ? "OBS_SH"                          : \
    (d)== OBS_PS                            ? "OBS_PS"                          : \
    (d)== OBS_UN                            ? "OBS_UN"                          : \
    (d)== OBS_SR                            ? "OBS_SR"                          : \
    (d)== OBS_OS                            ? "OBS_OS"                          : \
    (d)== OBS_LS                            ? "OBS_LS"                          : \
    (d)== OBS_FS                            ? "OBS_FS"                          : \
    (d)== OBS_TRIP                          ? "OBS_TRIP"                        : \
    (d)== OBS_PT_WAIT_RBC_ACK               ? "OBS_PT_WAIT_RBC_ACK"             : \
    (d)== OBS_PT_WAIT_EMER_STOP_REVOC       ? "OBS_PT_WAIT_EMER_STOP_REVOC"     : \
    (d)== OBS_PT                            ? "OBS_PT"                          : \
    (d)== OBS_SH_WAIT_RBC_TERMINATE         ? "OBS_SH_WAIT_RBC_TERMINATE"       : \
    (d)== OBS_ISOLATION                     ? "OBS_ISOLATION"                   : \
    (d)== OBS_FAILURE                       ? "OBS_FAILURE"                     : \
    (d)== OBS_SLEEPING                      ? "OBS_SLEEPING"                    : \
    (d)== OBS_NON_LEADING                   ? "OBS_NON_LEADING"                 : \
    (d)== OBS_REVERSING                     ? "OBS_REVERSING"                   : \
    "???"


/// Structure used to stored data about the EVC status
typedef struct SOnBoardStatus
{
    eTrainMode  TrainMode               ;   ///< Current ETCS mode
    eTrainMode  PreviousMode            ;   ///< Previous ETCS mode
    eTrainMode  LastReportedModeToRBC   ;   ///< Last ETCS mode reported to RBC (with position report)
    SLevel      LastReportedLevelToRBC  ;   ///< Last ETCS level reported to RBC (with position report)

    eEVCStatus  Status                  ;   ///< Current status of the on-board
    eEVCStatus  PreviousStatus          ;   ///< Previous status of the on-board

    bool        bWaitShuntingAck        ;   ///< Indicate if on board is waiting for shunting authorisation by RBC
    bool        bWaitRegistration       ;   ///< Indicate if on board is waiting for network registration
    bool        bMission                ;   ///< Indicate if a mission is running

    SLevel      ETCSLevel               ;   ///< Current ETCS level
    eValidity   ETCSLevelValidity       ;   ///< Validity of current ETCS level
    SLevel      PreviousLevel           ;   ///< Previous ETCS level
    SLevelTransition LevelTr            ;   ///< Current level transition
    bool        bLevelTrData            ;   ///< Indicate that there is a level transition in progress (there are some stored data in level transition data)
    bool        bLevelTrRBCMAData       ;   ///< Indicate if MA and track description received from RBC is stored onboard (level 2/3 transition)

    SLevel      aLevelPriority[MAX_ITERATIONS]; ///< List of supported level on the trackside sorted by priority
    int32_t lNbLevelP               ;       ///< Number of supported levels in the list
    eValidity   LevelPriorityValidity   ;       ///< Validity of trackside supported level

} SOnBoardStatus;




//======================================================================================
/// enumeration of type of function as described in SRS 4.5.2
typedef enum eFunctionType
{
    FCT_TYPE_CHK_LINK_CONSISTENCY,                      ///< Check linking consistency
    FCT_TYPE_CHK_BG_MSG_CONSISTENCY_IF_LINKING,         ///< Check Balise Group Message Consistency if linking consistency is checked
    FCT_TYPE_CHK_BG_MSG_CONSISTENCY_IF_NO_LINKING,      ///< Check Balise Group Message Consistency if no linking consistency is checked (because no linking information is available and/or because the function “check linking consistency is not active)
    FCT_TYPE_CHK_UNLINKED_BG_MSG_CONSISTENCY,           ///< Check Unlinked Balise Group Message Consistency
    FCT_TYPE_CHK_CORRECTNESS_OF_RADIO_MSG,              ///< Check correctness of radio messages
    FCT_TYPE_CHK_RADIO_SEQUENCE,                        ///< Check radio sequence
    FCT_TYPE_CHK_RADIO_SAFE_CNX,                        ///< Check radio safe connection (only level 2/3)
    FCT_TYPE_DET_TRAIN_LOC_REF_TO_LRBG,                 ///< Determine train location  referenced to LRBG
    FCT_TYPE_DET_TRAIN_SPEED_ACCEL_STANDSTILL,          ///< Determine train speed, train acceleration, train standstill
    FCT_TYPE_DET_GEO_POS,                               ///< Determine Geographical Position
    FCT_TYPE_REP_POS_STANDSTILL,                        ///< Report train position when train reaches standstill
    FCT_TYPE_REP_POS_MODE_CHANGES,                      ///< Report train position when mode changes
    FCT_TYPE_REP_POS_INTEGRITY_SEL_BY_DRIVER,           ///< Report train position when train integrity selected by driver
    FCT_TYPE_REP_POS_LOSS_INTEGRITY_DETECTED,           ///< Report train position when loss of train integrity is detected
    FCT_TYPE_REP_POS_RBC_RBC_BORDER,                    ///< Report train position when train has passed a RBC/RBC border
    FCT_TYPE_REP_POS_CHANGE_LVL_TRACKSIDE,              ///< Report train position when change of level due to trackside order
    FCT_TYPE_REP_POS_CHANGE_LVL_DRIVER,                 ///< Report train position when change of level due to driver request
    FCT_TYPE_REP_POS_ESTABLISH_SESSION_RBC,             ///< Report train position when establishing a session with RBC
    FCT_TYPE_REP_POS_REQUESTED_BY_RBC,                  ///< Report train position as requested by RBC
    FCT_TYPE_REP_POS_BG_PASSAGE,                        ///< … or Report train position at every balise group passage
    FCT_TYPE_REQ_MA_CYCLIC,                             ///< Request MA Cyclically respect to approach of target indication point (T_MAR) or MA timer elapsing (T_TIMEOUTRQST) (only level 2/3)
    FCT_TYPE_REQ_MA_CYCLIC_START_SELECTED,              ///< Request MA Cyclically when “Start” is selected (only level 2/3)
    FCT_TYPE_REQ_MA_TAF,                                ///< Request MA on reception of "track ahead free up to the level 2/3 transition location" (only level 0 or 1)
    FCT_TYPE_REQ_MA_TRACK_DESC_DELETION,                ///< Request MA on track description deletion
    FCT_TYPE_DET_EOA_LOA_SVL_DP,                        ///< Determine EOA/LOA, SvL, Danger Point, etc
    FCT_TYPE_HANDLE_COOP_MA_REVOC,                      ///< Handle Co-operative MA revocation
    FCT_TYPE_MNG_UNCOND_EMER_STOP,                      ///< Manage Unconditional Emergency Stop
    FCT_TYPE_MNG_COND_EMER_STOP,                        ///< Manage Conditional Emergency Stop
    FCT_TYPE_SSP,                                       ///< SSP
    FCT_TYPE_ASP,                                       ///< ASP
    FCT_TYPE_TSR,                                       ///< TSR
    FCT_TYPE_SIGNALLING_SRES_AS_SLIM,                   ///< Signalling related speed restriction when evaluated as a speed limit
    FCT_TYPE_MODE_SRES,                                 ///< Mode related speed restriction
    FCT_TYPE_TRAIN_SRES,                                ///< Train related speed restriction
    FCT_TYPE_LX_SPEED = 36,                             ///< LX speed
    FCT_TYPE_SRES_ENSURE_BRAKE_DISTANCE,                ///< Speed restriction to ensure a given permitted braking distance
    FCT_TYPE_OVR_SRES,                                  ///< Override related speed restriction
    FCT_TYPE_SPD_DIST_MONITORING,                       ///< Speed and Distance Monitoring based on MRSP, MA, release speed, gradient, mode profile, non protected LX start location, and route unsuitability location
    FCT_TYPE_SPD_DIST_MONITORING_MRSP,                  ///< Speed and Distance Monitoring based on MRSP
    FCT_TYPE_SPD_DIST_MONITORING_MRSP_SR_MODE,          ///< Speed and Distance Monitoring based on MRSP, allowed distance to run in Staff Resp. mode
    FCT_TYPE_CSM_ONLY,                                  ///< Ceiling Speed Monitoring only (no braking curve) based on MRSP
    FCT_TYPE_BKWDS_DIST_MONITORING,                     ///< Backwards Distance Monitoring
    FCT_TYPE_ROLL_AWAY_PROTECTION,                      ///< Roll Away Protection
    FCT_TYPE_RV_MOV_PROTECTION,                         ///< Reverse Movement Protection
    FCT_TYPE_STANDSTILL_SUPERVISION,                    ///< Standstill Supervision
    FCT_TYPE_SUP_DANGER_FOR_SHUNTING,                   ///< Supervise “danger for shunting” information and list of expected balises for shunting
    FCT_TYPE_SUP_STOP_IF_IN_SR,                         ///< Supervise “Stop if in SR” information and list of expected balises for Staff Responsible
    FCT_TYPE_SUP_SIGNALLING,                            ///< Supervise signalling related speed restriction when evaluated as a trip order
    FCT_TYPE_CMD_EB,                                    ///< Command Emergency Brake
    FCT_TYPE_DET_ETCS_MODE,                             ///< Determine ERTMS/ETCS Mode
    FCT_TYPE_DET_ETCS_LVL,                              ///< Determine ERTMS/ETCS level
    FCT_TYPE_SYS_VER,                                   ///< System Version Management
    FCT_TYPE_MNG_COM_SESSION,                           ///< Manage Communication Session
    FCT_TYPE_DEL_REVOKED_TSR,                           ///< Delete Revoked TSR
    FCT_TYPE_OVERRIDE,                                  ///< Override (Trip inhibition)
    FCT_TYPE_MNG_TRACK_COND_EX_BMM,                     ///< Manage Track Conditions excluding Big Metal Masses
    FCT_TYPE_MNG_TRACK_COND_BMM,                        ///< Manage Track Condition Big Metal Masses
    FCT_TYPE_MNG_TRACK_COND_TSA_SH_NSA,                 ///< Manage Track Condition Tunnel Stopping Area, Sound Horn, Non stopping area
    FCT_TYPE_MNG_ROUTE_SUIT,                            ///< Manage Route Suitability
    FCT_TYPE_MNG_TXT_DISP,                              ///< Manage Text Display to the driver
    FCT_TYPE_MNG_RBC_RBC_HANDOVER,                      ///< Manage RBC/RBC Handover
    FCT_TYPE_MNG_TAF_REQ,                               ///< Manage Track Ahead Free Request
    FCT_TYPE_ALLOW_ACCESS,                              ///< Allow access to MMI, train interfaces, JRU and odometer
    FCT_TYPE_PROVIDE_FIXED_VALUES,                      ///< Provide Fixed Values, and Default/National Values
    FCT_TYPE_CAPTURE_TRAIN_DATA,                        ///< Capture Train Data
    FCT_TYPE_PROVIDE_DATE_TIME,                         ///< Provide Date and Time
    FCT_TYPE_RECORD_JURI_DATA,                          ///< Record Juridical Data
    FCT_TYPE_INHIBIT_REVOC_TSR,                         ///< Inhibition of revocable TSRs from balises(only level 2/3)
    FCT_TYPE_COLD_MOV_DETECTION,                        ///< Cold Movement Detection
    FCT_TYPE_CONTINUE_SHUNTING_DESK_CLOSURE,            ///< Continue Shunting on desk closure (Enabling transition to Passive Shunting mode)
    FCT_TYPE_VIRTUAL_BALISE_COVER,                      ///<
    FCT_TYPE_NETWORK_REGISTRATION_FAILED,               ///< Report of failed network registration

    FCT_TYPE_MAX_NB

} eFunctionType;


/// enumeration of type of data as used in SRS 4.8.3, 4.8.4, 4.9, 4.10
typedef enum eDataType
{
    DATA_TYPE_LINKING = 1,              ///< Linking
    DATA_TYPE_SIGNAL_SPEED,             ///< Signalling Related Speed Restriction
    DATA_TYPE_MA,                       ///< Movement Authority
    DATA_TYPE_REPOSITIONING,            ///< Repositioning Information
    DATA_TYPE_GRADIENT,                 ///< Gradient Profile
    DATA_TYPE_SSP,                      ///< International SSP
    DATA_TYPE_AXLE_LOAD_SP,             ///< Axle Load speed profile
    DATA_TYPE_LEVEL_TR = 10,             ///< Level transition order
    DATA_TYPE_SESSION_MNGT,             ///< Session Management
    DATA_TYPE_RAD_NET_REG,              ///< Radio Network registration
    DATA_TYPE_SH_AREA,                  ///< List of balises for SH area
    DATA_TYPE_MA_RQST_PARAM,            ///< MA Request Parameters
    DATA_TYPE_PR_PARAM,                 ///< Position Report parameters
    DATA_TYPE_SR_AUTHORITY,             ///< SR Authorisation + (optional) List of Balises in SR mode
    DATA_TYPE_SR_STOP,                  ///< Stop if in SR mode
    DATA_TYPE_SR_DISTANCE_FROM_LOOP,    ///< SR distance information from loop
    DATA_TYPE_TSR,                      ///< Temporary Speed Restriction
    DATA_TYPE_TSR_REVOC,                ///< Temporary Speed Restriction Revocation
    DATA_TYPE_INHIBIT_TSR_REVOC,        ///< Inhibition of revocable TSRs from balises in L2/3
    DATA_TYPE_TSR_GRADIENT,             ///< Default Gradient for TSR
    DATA_TYPE_ROUTE_SUITABILITY,        ///< Route Suitability Data
    DATA_TYPE_ADHESION_FROM_TRACKSIDE,  ///< Adhesion Factor sent by trackside
    DATA_TYPE_ADHESION_FROM_DRIVER,     ///< Adhesion Factor sent by driver
    DATA_TYPE_PLAIN_TEXT,               ///< Plain Text Information
    DATA_TYPE_FIXED_TEXT,               ///< Fixed Text Information
    DATA_TYPE_GEO_POS,                  ///< Geographical Position
    DATA_TYPE_MODE_PROFIL,              ///< Mode Profile
    DATA_TYPE_RBC_TR,                   ///< RBC Transition Order
    DATA_TYPE_SH_DANGER,                ///< Danger for SH information
    DATA_TYPE_STOP_SH,                  ///< Stop SH on desk opening
    DATA_TYPE_RADIO_INFILL_AREA,        ///< Radio Infill Area information
    DATA_TYPE_SESSION_MNGT_RIU,         ///< Session Management with neighbouring RIU
    DATA_TYPE_EOLM,                     ///< EOLM information
    DATA_TYPE_ASSIGN_COOR,              ///< Assignment of Co-ordinate system
    DATA_TYPE_INFILL_LOC_REF,           ///< Infill Location Reference
    DATA_TYPE_TRACK_COND,               ///< Track Conditions excluding big metal masses
    DATA_TYPE_TRACK_COND_BIG_METAL_MASS,///< Track condition big metal masses
    DATA_TYPE_TRACK_COND_TUNNEL_STOPAREA,///< Track condition tunnel stopping area
    DATA_TYPE_LOCATION_IDENTITY,        ///< Location Identity (NID_C + NID_BG transmitted in the balise telegram)
    DATA_TYPE_ACK_EXIT_TRIP,            ///< Recognition of exit from TRIP mode
    DATA_TYPE_ACK_TRAIN_DATA,           ///< Acknowledgement of Train Data
    DATA_TYPE_REDUCE_MA_RQST,           ///< Co-operative shortening of MA
    DATA_TYPE_UNCOND_EMERGENCY_STOP,    ///< Unconditional Emergency Stop
    DATA_TYPE_COND_EMERGENCY_STOP,      ///< Conditional Emergency Stop
    DATA_TYPE_TRAIN_LOCATION,           ///< Train Location
    DATA_TYPE_TRAIN_DATA,               ///< Train Data
    DATA_TYPE_ETCS_LEVEL,               ///< ERTMS/ETCS level
    DATA_TYPE_PRIORITY_TABLE,           ///< Table of priority of trackside supported levels
    DATA_TYPE_INHIBITED_LEVELS,         ///< List of Inhibited Levels
    DATA_TYPE_DRIVER_ID,                ///< Driver ID
    DATA_TYPE_RADIO_NETWORK_ID,         ///< Radio Network ID
    DATA_TYPE_RBC_ID_PHONE_NUMBER,      ///< RBC ID/Phone Number
    DATA_TYPE_REVOC_EMERGENCY_STOP,     ///< Revocation of Emergency Stop
    DATA_TYPE_SHUNT_REFUSED,            ///< SH refused
    DATA_TYPE_SHUNT_ACCEPTED,           ///< SH authorised
    DATA_TYPE_TKS_SYSTEM_VERSION,       ///< Track side constituent System Version
    DATA_TYPE_SYSTEM_VERSION_ORDER,     ///< System Version order
    DATA_TYPE_TAF_RQST,                 ///< Track Ahead Free Request
    DATA_TYPE_TRAIN_RUN_NB,             ///< Train Running Number
    DATA_TYPE_INIT_SESSION,             ///< Initiation of session
    DATA_TYPE_ACK_TERM_SESSION,         ///< Acknowledgement of session termination
    DATA_TYPE_TRAIN_REJECTED,           ///< Train Rejected
    DATA_TYPE_TRAIN_ACCEPTED,           ///< Train Accepted
    DATA_TYPE_SOM_PR_ACK,               ///< SoM Position Report Confirmed by RBC
    DATA_TYPE_REVERSING_AREA,           ///< Reversing Area Information
    DATA_TYPE_REVERSING_SUPERVISION,    ///< Reversing Supervision Information
    DATA_TYPE_CONFIRM_LOCATION_BY_DRIVER,///< Confirmation of Location by Driver Requested
    DATA_TYPE_DEFAULT_BALISE_INFO,      ///< Default Balise Information
    DATA_TYPE_TAF_UP_L2_3_TR,           ///< Track Ahead Free up to level 2/3 transition location
    DATA_TYPE_PERM_BRAKE_DIST,          ///< Permitted Braking Distance Information
    DATA_TYPE_LEVEL_CROSSING,           ///< Level Crossing information
    DATA_TYPE_RBC_SYSTEM_VERSION,       ///< RBC/RIU System Version
    DATA_TYPE_OPERATED_SYSTEM_VERSION,  ///< Operated System Version
    DATA_TYPE_VIRTUAL_BALISE_COVER,     ///< Virtual balise cover
    DATA_TYPE_EXTERNAL_DATA,     ///< Virtual balise cover

    DATA_TYPE_MAX_NB

} eDataType;


/// Macro to get the string corresponding to an EVC status
#define STR_DATATYPE(d) \
    (d)== DATA_TYPE_NATIONAL_VALUES         ? "DATA_TYPE_NATIONAL_VALUES"                   : \
    (d)== DATA_TYPE_LINKING                 ? "DATA_TYPE_LINKING"                           : \
    (d)== DATA_TYPE_SIGNAL_SPEED            ? "DATA_TYPE_SIGNAL_SPEED"                      : \
    (d)== DATA_TYPE_MA                      ? "DATA_TYPE_MA"                                : \
    (d)== DATA_TYPE_REPOSITIONING           ? "DATA_TYPE_REPOSITIONING"                     : \
    (d)== DATA_TYPE_GRADIENT                ? "DATA_TYPE_GRADIENT"                          : \
    (d)== DATA_TYPE_SSP                     ? "DATA_TYPE_SSP"                               : \
    (d)== DATA_TYPE_AXLE_LOAD_SP            ? "DATA_TYPE_AXLE_LOAD_SP"                      : \
    (d)== DATA_TYPE_LEVEL_TR                ? "DATA_TYPE_LEVEL_TR"                          : \
    (d)== DATA_TYPE_SESSION_MNGT            ? "DATA_TYPE_SESSION_MNGT"                      : \
    (d)== DATA_TYPE_RAD_NET_REG             ? "DATA_TYPE_RAD_NET_REG"                       : \
    (d)== DATA_TYPE_SH_AREA                 ? "DATA_TYPE_SH_AREA"                           : \
    (d)== DATA_TYPE_MA_RQST_PARAM           ? "DATA_TYPE_MA_RQST_PARAM"                     : \
    (d)== DATA_TYPE_PR_PARAM                ? "DATA_TYPE_PR_PARAM"                          : \
    (d)== DATA_TYPE_SR_AUTHORITY            ? "DATA_TYPE_SR_AUTHORITY"                      : \
    (d)== DATA_TYPE_SR_STOP                 ? "DATA_TYPE_SR_STOP"                           : \
    (d)== DATA_TYPE_SR_DISTANCE_FROM_LOOP   ? "DATA_TYPE_SR_DISTANCE_FROM_LOOP"             : \
    (d)== DATA_TYPE_TSR                     ? "DATA_TYPE_TSR"                               : \
    (d)== DATA_TYPE_TSR_REVOC               ? "DATA_TYPE_TSR_REVOC"                         : \
    (d)== DATA_TYPE_INHIBIT_TSR_REVOC       ? "DATA_TYPE_INHIBIT_TSR_REVOC"                 : \
    (d)== DATA_TYPE_TSR_GRADIENT            ? "DATA_TYPE_TSR_GRADIENT"                      : \
    (d)== DATA_TYPE_ROUTE_SUITABILITY       ? "DATA_TYPE_ROUTE_SUITABILITY"                 : \
    (d)== DATA_TYPE_ADHESION_FROM_TRACKSIDE ? "DATA_TYPE_ADHESION_FROM_TRACKSIDE"           : \
    (d)== DATA_TYPE_ADHESION_FROM_DRIVER    ? "DATA_TYPE_ADHESION_FROM_DRIVER"              : \
    (d)== DATA_TYPE_PLAIN_TEXT              ? "DATA_TYPE_PLAIN_TEXT"                        : \
    (d)== DATA_TYPE_FIXED_TEXT              ? "DATA_TYPE_FIXED_TEXT"                        : \
    (d)== DATA_TYPE_GEO_POS                 ? "DATA_TYPE_GEO_POS"                           : \
    (d)== DATA_TYPE_MODE_PROFIL             ? "DATA_TYPE_MODE_PROFIL"                       : \
    (d)== DATA_TYPE_RBC_TR                  ? "DATA_TYPE_RBC_TR"                            : \
    (d)== DATA_TYPE_SH_DANGER               ? "DATA_TYPE_SH_DANGER"                         : \
    (d)== DATA_TYPE_STOP_SH                 ? "DATA_TYPE_STOP_SH"                           : \
    (d)== DATA_TYPE_RADIO_INFILL_AREA       ? "DATA_TYPE_RADIO_INFILL_AREA"                 : \
    (d)== DATA_TYPE_SESSION_MNGT_RIU        ? "DATA_TYPE_SESSION_MNGT_RIU"                  : \
    (d)== DATA_TYPE_EOLM                    ? "DATA_TYPE_EOLM"                              : \
    (d)== DATA_TYPE_ASSIGN_COOR             ? "DATA_TYPE_ASSIGN_COOR"                       : \
    (d)== DATA_TYPE_INFILL_LOC_REF          ? "DATA_TYPE_INFILL_LOC_REF"                    : \
    (d)== DATA_TYPE_TRACK_COND              ? "DATA_TYPE_TRACK_COND"                        : \
    (d)== DATA_TYPE_TRACK_COND_BIG_METAL_MASS   ? "DATA_TYPE_TRACK_COND_BIG_METAL_MASS"     : \
    (d)== DATA_TYPE_LOCATION_IDENTITY       ? "DATA_TYPE_LOCATION_IDENTITY"                 : \
    (d)== DATA_TYPE_ACK_EXIT_TRIP           ? "DATA_TYPE_ACK_EXIT_TRIP"                     : \
    (d)== DATA_TYPE_ACK_TRAIN_DATA          ? "DATA_TYPE_ACK_TRAIN_DATA"                    : \
    (d)== DATA_TYPE_REDUCE_MA_RQST          ? "DATA_TYPE_REDUCE_MA_RQST"                    : \
    (d)== DATA_TYPE_UNCOND_EMERGENCY_STOP   ? "DATA_TYPE_UNCOND_EMERGENCY_STOP"             : \
    (d)== DATA_TYPE_COND_EMERGENCY_STOP     ? "DATA_TYPE_COND_EMERGENCY_STOP"               : \
    (d)== DATA_TYPE_TRAIN_LOCATION          ? "DATA_TYPE_TRAIN_LOCATION"                    : \
    (d)== DATA_TYPE_TRAIN_DATA              ? "DATA_TYPE_TRAIN_DATA"                        : \
    (d)== DATA_TYPE_ETCS_LEVEL              ? "DATA_TYPE_ETCS_LEVEL"                        : \
    (d)== DATA_TYPE_PRIORITY_TABLE          ? "DATA_TYPE_PRIORITY_TABLE"                    : \
    (d)== DATA_TYPE_INHIBITED_LEVELS        ? "DATA_TYPE_INHIBITED_LEVELS"                  : \
    (d)== DATA_TYPE_DRIVER_ID               ? "DATA_TYPE_DRIVER_ID"                         : \
    (d)== DATA_TYPE_RADIO_NETWORK_ID        ? "DATA_TYPE_RADIO_NETWORK_ID"                  : \
    (d)== DATA_TYPE_RBC_ID_PHONE_NUMBER     ? "DATA_TYPE_RBC_ID_PHONE_NUMBER"               : \
    (d)== DATA_TYPE_REVOC_EMERGENCY_STOP    ? "DATA_TYPE_REVOC_EMERGENCY_STOP"              : \
    (d)== DATA_TYPE_SHUNT_REFUSED           ? "DATA_TYPE_SHUNT_REFUSED"                     : \
    (d)== DATA_TYPE_SHUNT_ACCEPTED          ? "DATA_TYPE_SHUNT_ACCEPTED"                    : \
    (d)== DATA_TYPE_TKS_SYSTEM_VERSION      ? "DATA_TYPE_TKS_SYSTEM_VERSION"                : \
    (d)== DATA_TYPE_SYSTEM_VERSION_ORDER    ? "DATA_TYPE_SYSTEM_VERSION_ORDER"              : \
    (d)== DATA_TYPE_TAF_RQST                ? "DATA_TYPE_TAF_RQST"                          : \
    (d)== DATA_TYPE_TRAIN_RUN_NB            ? "DATA_TYPE_TRAIN_RUN_NB"                      : \
    (d)== DATA_TYPE_INIT_SESSION            ? "DATA_TYPE_INIT_SESSION"                      : \
    (d)== DATA_TYPE_ACK_TERM_SESSION        ? "DATA_TYPE_ACK_TERM_SESSION"                  : \
    (d)== DATA_TYPE_TRAIN_REJECTED          ? "DATA_TYPE_TRAIN_REJECTED"                    : \
    (d)== DATA_TYPE_TRAIN_ACCEPTED          ? "DATA_TYPE_TRAIN_ACCEPTED"                    : \
    (d)== DATA_TYPE_SOM_PR_ACK              ? "DATA_TYPE_SOM_PR_ACK"                        : \
    (d)== DATA_TYPE_REVERSING_AREA          ? "DATA_TYPE_REVERSING_AREA"                    : \
    (d)== DATA_TYPE_REVERSING_SUPERVISION   ? "DATA_TYPE_REVERSING_SUPERVISION"             : \
    (d)== DATA_TYPE_CONFIRM_LOCATION_BY_DRIVER  ? "DATA_TYPE_CONFIRM_LOCATION_BY_DRIVER"    : \
    (d)== DATA_TYPE_DEFAULT_BALISE_INFO         ? "DATA_TYPE_DEFAULT_BALISE_INFO"           : \
    (d)== DATA_TYPE_TAF_UP_L2_3_TR              ? "DATA_TYPE_TAF_UP_L2_3_TR"                : \
    (d)== DATA_TYPE_PERM_BRAKE_DIST             ? "DATA_TYPE_PERM_BRAKE_DIST"               : \
    (d)== DATA_TYPE_LEVEL_CROSSING              ? "DATA_TYPE_LEVEL_CROSSING"                : \
    (d)== DATA_TYPE_RBC_SYSTEM_VERSION          ? "DATA_TYPE_RBC_SYSTEM_VERSION"            : \
    (d)== DATA_TYPE_OPERATED_SYSTEM_VERSION     ? "DATA_TYPE_OPERATED_SYSTEM_VERSION"       : \
    (d)== DATA_TYPE_EXTERNAL_DATA               ? "DATA_TYPE_EXTERNAL_DATA"                 : \
    "???"

/// enumeration of possible change of data status
typedef enum eDataStatusChange
{
    DATA_STATUS_UNCHANGED,
    DATA_STATUS_TO_BE_REVALIDATED,
    DATA_STATUS_DELETED,
    DATA_STATUS_RESET

} eDataStatusChange;

typedef enum eDataException
{
    EX1  = 0x001,
    EX2  = 0x002,
    EX3  = 0x004,
    EX4  = 0x008,
    EX5  = 0x010,
    EX6  = 0x020,
    EX7  = 0x040,
    EX8  = 0x080,
    EX9  = 0x100,
    EX10 = 0x200,
    EX11 = 0x400,
    EX12 = 0x800,
    EX13 = 0x1000

} eDataException;



//=======================	Version data	=========================

/// Structure to store version order
typedef struct SVersionOrder
{
    bool	bValid			;   ///< Validity of data
    bool	bInfillData		;   ///< Data received via infill equipment: use infill location reference

    int32_t lVersion		;	///< Version of the ETCS system
} SVersionOrder;



//=======================    Corrections factors    =======================


#define MAX_KV_PT   5           ///< Maximum nb of point in Kv_rst model (SRS 3.13.2.2.9.1.2)

/// structure to store correction
typedef struct SCorr
{
    t_speed dSpeed ;            ///< Applicable speed for correction factor
    double  dFactor;            ///< Correction factor : Gives the correction factor if maximum emergency brake deceleration is lower than A_NVP12
    double  dFactor2;           ///< Correction factor : Gives the correction factor if maximum emergency brake deceleration is higher than A_NVP23
} SCorr;

/// structure to store Kv point
typedef struct SKvPt
{
    bool    bTypePassenger;     ///< Type of Kv_int set (0 : freight, 1 : Passenger)
    double  dLowerDeceleration; ///< Lower deceleration limit to determine the set of Kv to be used
    double  dUpperDeceleration; ///< Upper deceleration limit to determine the set of Kv to be used

    int32_t lNbCorr;
    SCorr   Corr[MAX_KV_PT];
} SKvPt;

/// Structure used to store Kv_int data (SRS 3.13.2.3.7)
typedef struct SKv
{
    int32_t lNbPt;                 ///< Number of point in model
    SKvPt Pt[MAX_KV_PT];
} SKv;


#define MAX_KR_PT   5           ///< Maximum nb of point in Kv_rst model (SRS 3.13.2.2.9.1.5)

/// structure to store Kr point
typedef struct SFactorPerLength
{
    double      dFactor;        ///< Correction factor
    t_distance  dLength ;       ///< Applicable length for correction factor
} SFactorPerLength;

/// Structure used to store Kr_int data (SRS 3.13.2.7.12)
typedef struct SKr
{
    int32_t lNbPt;                 ///< Number of point in model
    SFactorPerLength Pt[MAX_KR_PT];
} SKr;


#define MAX_KN_PT   5           ///< Maximum nb of point in Kn model (SRS 3.13.2.2.9.3.2)

/// structure to store Kn point
typedef struct SFactorPerSpeed
{
    double  dFactor;        ///< Correction factor
    t_speed dSpeed ;        ///< Applicable speed for correction factor
} SFactorPerSpeed;

/// Structure used to store Kn data (SRS 3.13.2.2.9.3)
typedef struct SKn
{
    int32_t lNbPt;                 ///< Number of point in model
    SFactorPerSpeed Pt[MAX_KN_PT];
} SKn;

/// Structure used to store SKdry_rst data (SRS 3.13.2.2.9.1)
typedef struct SKdry_rst
{
    int32_t lNbPt;                 ///< Number of point in model
    SFactorPerSpeed Pt[MAX_KR_PT];

}SKdry_rst;

/// Structure used to store SKwet_rst data (SRS 3.13.2.2.9.1)
typedef struct SKwet_rst
{
    int32_t lNbPt;                 ///< Number of point in model
    SFactorPerSpeed Pt[MAX_KR_PT];

}SKwet_rst;


//=======================    National data    =======================

/// Maximum number of country in list for national data application
#define MAX_COUNTRY_NB      10


/// Structure to store National values
typedef struct SNationalData
{
    bool        bValid                          ;   ///< Validity of data
    bool        bInfillData                     ;   ///< Data received via infill equipment: use infill location reference
    bool        bForceNow                       ;   ///< National Values are immediately applicable
    t_distance  dValidLocation                  ;   ///< Location for application of the national data
    int32_t lCountryNb                      ;   ///< Number of country id in list
    uint32_t auCountryList[MAX_COUNTRY_NB]   ;   ///< List of country identity (associated to national data)

    // class1v10
    t_speed     dShuntSpeed                     ;   ///< Shunting speed limit
    t_speed     dStaffSpeed                     ;   ///< Staff responsible speed limit
    t_speed     dOSSpeed                        ;   ///< On Sight speed limit
    t_speed     dUnfitSpeed                     ;   ///< Unfitted speed limit
    t_speed     dReleaseSpeed                   ;   ///< Release speed limit
    t_distance  dRollAwayDist                   ;   ///< Roll away distance limit
    bool        bUseSB                          ;   ///< Indicator for use of SB in target speed monitoring
    bool        bEBRelease                      ;   ///< Authorisation to release the EB when not at standstill
    t_speed     dOVSupSpeed                     ;   ///< Maximum speed limit in override
    t_speed     dOVSpeed                        ;   ///< Maximum speed limit for overriding the train trip
    t_distance  dOVDist                         ;   ///< Maximum distance for overriding the train trip
    t_time      dOVTime                         ;   ///< Maximum time for overriding the train trip
    t_distance  dPostTripDist                   ;   ///< Maximum distance for reversing in post trip mode
    eReaction   ContactReact                    ;   ///< Reaction when maximal contact time elapses
    t_time      dContactTime                    ;   ///< Maximal time without new safe message
    bool        bRun_ID_Entry                   ;   ///< Entry of driver id permitted while running
    t_distance  dMaxSRDistance                  ;   ///< Maximum distance in SR
    bool        bDriverAdhesionEntry            ;   ///< Entry of adhesion factor by driver allowed

    // class1v11, class1v20
    bool        bUseGuidanceCurve               ;   ///< Permission to use the guidance curve (Q_NVGUIPERM)
    bool        bInhibSpeedInacc                ;   ///< Permission to inhibit the speed mesurement inaccurary (Q_NVINHSMICPERM)
    bool        bUseServiceBrakeFeedback        ;   ///< Permission to use the service brake feedback (Q_NVSBFBPERM)
    t_accel     dMaxDecelUnderRAC1              ;   ///< Max deceleration under Reduced Adhesion Condition (A_NVMAXREDADH1)
    t_accel     dMaxDecelUnderRAC2              ;   ///< Max deceleration under Reduced Adhesion Condition (A_NVMAXREDADH2)
    t_accel     dMaxDecelUnderRAC3              ;   ///< Max deceleration under Reduced Adhesion Condition (A_NVMAXREDADH3)
    SKv         Kv_int                          ;   ///< Correction Factor Kv
    SKr         Kr_int                          ;   ///< Correction Factor Kr
    double      dKt_int                         ;   ///< Correction factor Kt
    double      dWheelRailAdhesion              ;   ///< Wheighting factor for available wheel/rail adhesion
    double      dEBConfidenceLevel              ;   ///< Confidence level for emergency brake saf deceleration on dry rails
    bool        bIntegratedCorrection           ;   ///< Integrated correction factor

    // class1v20 only
    t_distance  dBaliseLocAccuracy              ;   ///< Accuracy of the balise location (Q_NVLOCACC)
    t_speed     dLSSpeed                        ;   ///< Limited supervision speed (V_NVLIMSUPERV)

} SNationalData;



//=======================    text message data    =======================

/// Macro to get the string corresponding to trackside Q_TEXT
#define STR_TKS_Q_TEXT(d) \
    (d)==0 ? "Level crossing not protected" :   \
    (d)==1 ? "Acknowledgement"              :   \
    "???"

/// Enumeration for the class of text message
typedef enum eText_Class
{
    EVC_SYS_STATUS_MESG = 0,
    AUXILIARY_INFO      = 1,        ///< Default
    IMPORTANT_INFO      = 2         ///< Important information

} eText_Class ;


/// Enumeration for message acknowledgement qualifier
typedef enum eConfirmation
{
    NO_ACK          ,   ///< No acknowledgment required
    ACK             ,   ///< Acknowledgment required
    ACK_SB_AT_END   ,   ///< Acknowledgment required and service brake applied when end condition for display message reached and no ack
    ACK_EB_AT_END       ///< Acknowledgment required and emergency brake applied when end condition for display message reached and no ack

} eConfirmation;


/// Macro to get the string corresponding to a confirmation request
#define STR_CONFIRMATION(d) \
    (d)==NO_ACK         ? "NO_ACK"          : \
    (d)==ACK            ? "ACK"             : \
    (d)==ACK_SB_AT_END  ? "ACK_SB_AT_END"   : \
    (d)==ACK_EB_AT_END  ? "ACK_EB_AT_END"   : "???"

/// Structure to store data for text message condition
typedef struct SDisplayConditions
{
    bool            bAllCondRequired            ;   ///< Start/End of display : all condition must be fullfilled

    t_distance      dLocation                   ;   ///< Location of the display
    t_distance      dDistance                   ;   ///< Distance on which the text is displayed
    t_time          dTime                       ;   ///< Time on which the text is displayed

    eTrainMode      StartMode                   ;   ///< Start condition on mode
    eTrainMode      EndMode                     ;   ///< End condition on mode

    SLevel          StartLevel                  ;   ///< Start condition on level
    SLevel          EndLevel                    ;   ///< End condition on level

    eConfirmation   Confirmation                ;   ///< Indicate if a confirmation is required
    bool            bAckEndDisplay              ;   ///< Driver ack end the display (true), or is an additionnal condition (false). (Q_CONFTEXTDISPLAY)

} SDisplayConditions;

/// Structure to store data for text message display received by trackside
typedef struct STextMsgData
{
    bool                bValid                      ;   ///< Validity of data
    bool                bInfillData                 ;   ///< Data received via infill equipment: use infill location reference

    eText_Class         Text_Class                  ;   ///< Class of message to be displayed

    SDisplayConditions  conditions                  ;   ///< Conditions to display message (Mode, level, location...)

    bool            bReportRequirement          ;   ///< driver confirmation requirement (Q_TEXTREPORT)
    int32_t lTextMessageId              ;   ///< Text message identifier to report to RBC (NID_TEXTMESSAGE)
    int32_t lCountryId                  ;   ///< Identity number of the country or region (NID_C)
    int32_t lRBCId                      ;   ///< RBC ETCS identity number (NID_RBC)

    int32_t lTextCode                   ;   ///< Code of the fixed text ( -1 = plain text )
    char            szPlainText[MAX_TEXT_LENGTH];   ///< free text

} STextMsgData;


//=======================    Linking data    =======================

/// Maximum number of linked BG in list
#define MAX_LK_BAL        32

/// Structure to store BG
typedef struct SBG
{
    t_distance  dDistance   ;   ///< Distance between the balise group n-1 and n
    t_distance  dAccuracy   ;   ///< Accuracy of the position in metre
    SBG_Id      ID          ;   ///< ID of the balise group
    eReaction   Reaction    ;   ///< Reaction of EVC if linked BG not found
} SBG;

/// Structure containing linking data
typedef struct SLinkingData
{
    bool        bValid          ;   ///< Validity of data
    bool        bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dRefLoc         ;   ///< Reference location

    int32_t lLinkedBGNb     ;   ///< Number of linked balise group

    SBG         aBG[MAX_LK_BAL]         ;   ///< List of linking BG

} SLinkingData;


/// Structure transmitted from balise_interface to odometer_interface in order
/// to correct odometer.
typedef struct SLinkingCorrectionForOdo
{
    t_distance dCorrection;         /// Distance that shall be corrected
    int32_t lBGIndex;            /// Send current BG index to set LRBG correction
    int32_t bUseNationalBGAcc;   /// Send current BG index to set LRBG correction

} SLinkingCorrectionForOdo;

//================ Staff Responsible distance info from loop =================

/// structure to store a SR BG
typedef struct SSRBG
{
    SBG_Id      ID          ;   ///< ID of the balise group
    t_distance	dSRDistance	;	///< Distance that can be run in SR mode
} SSRBG;

/// Staff responsibilities distance information from loop
typedef struct SSRDistInfo
{
    bool bValid			;	///< Validity of data
    bool bInfillData	;	///< Data received via infill equipment: use infill location reference

    SBG_Id MainSignalBG	;	///< Main Signal Balise group

    int32_t lBGNb			;	///< Number of reference balise group

    SSRBG aBG[MAX_LK_BAL]           ;   ///< List of linked BG

} SSRDistInfo;


//=======================    Movement authority data    =======================

/// Maximum number of section in a movement authority
#define MAX_SECTION_NB        32

/// structure to store MA Section
typedef struct SMASection
{
    t_distance  dEndSectionLoc          ;   ///< Location of the end of the section
    bool        bSectionTimer           ;   ///< Indicate if a timer is associated to the section
    t_time      dTimerValue             ;   ///< Value of the timer associated to the section
    t_distance  dTimeOutStopLoc         ;   ///< Location to stop if expiration of timer
    bool        bTTimeOutRqstMARequested;   ///< Indicate if MA has been requested due to T_TIMEOUTRQST (if there is a timer for this section)
} SMASection;


/// Structure to store the movement authority data
typedef struct SMAData
{
    bool            bValid                  ;   ///< Validity of data
    bool            bInfillData             ;   ///< Data received via infill equipment: use infill location reference

    t_distance      dRefLoc                 ;   ///< Reference location (used in msg transfert)
    t_time          dReceptionMATime        ;   ///< Time of MA reception

    t_speed         dMainSpd                ;   ///< Level 1: signal related permitted speed
    t_speed         dLoaSpd                 ;   ///< Speed at LOA
    t_time          dLoaTime                ;   ///< Validity time of the LOA speed
    bool            bNextSignalUsed         ;   ///< indicate if there is next signal info
    t_speed         dNextSignalSpd          ;   ///< signal speed at next signal (used for IF MA)
    t_distance      dNextSignalDist         ;   ///< distance to next signal speed from ref loc (used for IF MA)

    int32_t lSectionNb              ;   ///< Number of MA section

    SMASection aSection[MAX_SECTION_NB]     ;   ///< List of MA section


    bool        bEndTimerStarted            ;   ///< Indicate that end timer started
    bool        bEndTimer                   ;   ///< Indicate if there is an additional timer for the last section
    t_time      dEndTimerValue              ;   ///< Value of the additional timer associated to the end section
    t_time      dEndTimerStartTime          ;   ///< Time at which the end timer has been started
    t_distance  dEndTimerStartLoc           ;   ///< Location to start the end timer

    bool        bDangerPtData               ;   ///< Indicator for danger point data
    t_distance  dDangerPtLoc                ;   ///< Location of the danger point
    t_speed     dDangerPtRelSpd             ;   ///< Release speed associated to the dange point


    bool        bOverlapData                ;   ///< Indicator for overlap data
    t_distance  dOverlapLoc                 ;   ///< Location of the overlap
    t_speed     dOverlapRelSpd              ;   ///< Release speed associated to the overlap
    t_distance  dOverlapTimerStartLoc       ;   ///< Location to start the overlap timer
    t_time      dOverlapTimerValue          ;   ///< Overlap timer value
    t_time      dOverlapTimerStartTime      ;   ///< Time at which the overlap timer has been started
    bool        bOverlapTimerStarted        ;   ///< Indicate that overlap timer is started

} SMAData;




//=======================    gradient data    =======================


/// Maximum number of discontinuities in gradient profile
#define MAX_GRAD_NB        32

/// Structure to store a gradient profile
typedef struct SGradient
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dRefLoc     ;   ///< Reference location (used in msg transfert)

    SProfile    Profile    ;   ///< Gradient profile

} SGradient;



/// Structure to store default gradient for temporary speed restriction
typedef struct STSRDefaultGradient
{
    bool    bValid      ;              ///< Validity of data
    bool    bInfillData ;              ///< Data received via infill equipment: use infill location reference

    int32_t lValue      ;              ///< Value of default gradient for TSR
} STSRDefaultGradient;



//=======================    speed profile data    =======================

/// Maximum number of discontinuities in speed profile
#define MAX_SPEED_NB    32

/// Structure to store static speed profile
typedef struct SSpeed
{
    bool        bValid                                          ;   ///< Validity of data
    bool        bInfillData                                     ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dRefLoc                                         ;   ///< Reference location (used in msg transfert)

    SProfile    Profile                                         ;   ///< Speed profile
    bool        bTrainLengthDelay[MAX_PROFILE_DISCONTINUITIES]  ;   ///< Indicator for train length delay

} SSpeed;


//=======    Track condition change of allowed current consumption    =======

/// Structure to store information about allowed current consumption
typedef struct SAllowedConsumption
{
    bool		bValid;         ///< Validity of data
    bool		bInfillData;    ///< Data received via infill equipment: use infill location reference
    t_distance  dRefLoc;        ///< Reference location

    t_distance	dDistance;              /// Distance where the new allowed consumption is taken in account
    int32_t lNewAllowedConsumption; /// Allowed consumption at dDistance. (Ampere)

} SAllowedConsumption;


//=======================    level transition data    =======================

/// Structure to store data about a level transition
typedef struct SLevelTr
{
    bool            bValid                  ;   ///< Validity of data
    bool            bInfillData             ;   ///< Data received via infill equipment: use infill location reference

    t_distance      dLocation               ;   ///< Location of the level transition
    int32_t lNbLevelP               ;   ///< Number of supported level
    SLevel          aLevelPriority[MAX_ITERATIONS];   ///< Priority table for supported
    t_distance      dAckLength[MAX_ITERATIONS];   ///< Area length for level transition acknowledgment

} SLevelTr;


/// Structure containing information about Conditional Level Transition Order (packet 46)
typedef struct SConditionalLTR
{
    bool    bValid                  ;   ///< Validity of data
    bool    bInfillData             ;   ///< Data received via infill equipment: use infill location reference

    int32_t lNbLevelP               ;   ///< Number of supported level
    SLevel  aLevelPriority[MAX_ITERATIONS];   ///< Priority table for supported level

} SConditionalLTR;


//=======================    radio management data    =======================

/// Enumeration for radio connection order
typedef enum eConnection
{
    CONNECT     ,   ///< Connection order
    DISCONNECT      ///< Disconnection order

} eConnection ;


/// Structure to store RBC connection order
typedef struct SRBCConnectOrder
{
    bool            bValid                              ;   ///< Validity of data
    bool            bInfillData                         ;   ///< Data received via infill equipment: use infill location reference

    eConnection     Connection                          ;   ///< Order to connect or disconnect RBC

    int32_t lRBCCountry                         ;   ///< RBC country id
    int32_t lRBCId                              ;   ///< RBC identity
    uint8_t           uszRBCPhoneNumber[PHONE_NB_SIZE]    ;   ///< Radio phone number
    bool            bForSleeping                        ;   ///< Indicate if order should be taken into account by EVC in SL mode

} SRBCConnectOrder;

//=======================    shunting area data    =======================

/// Maximum number of BG in list
#define MAX_BG_NB        32


/// Structure to store data about a shunting area
typedef struct SShuntArea
{
    bool        bValid              ;   ///< Validity of data
    bool        bInfillData         ;   ///< Data received via infill equipment: use infill location reference

    int32_t lBGNb               ;   ///< Number of BG in the shunting area
    SBG_Id      aBGList[MAX_BG_NB]  ;   ///< list of BG which the train can pass over in SH mode

} SShuntArea;


//======================= Virtual balise cover ========================

/// Maximum VBC order stored
#define MAX_VBC_ORDERS   8

#define UNKOWN_VBC_ID    2147483647

/// Structure to store data about virtual balise cover marker
typedef struct SVBaliseCoverMarker
{
    bool        bValid;
    uint32_t ulId;           ///< Cover marker id

}SVBaliseCoverMarker;

/// Structure to store data about virtual balise cover order
typedef struct SVBaliseCoverOrder
{
    bool        bValid;
    bool        bInfillData;

    bool        bSet;           ///< Set or remove order
    uint32_t ulId;           ///< Cover order id
    uint32_t ulCountry;      ///< Cover order country id
    uint32_t ulDay;           ///< Cover order timer

    t_time      dStartTime;     ///< use to know when VBC it has been received

}SVBaliseCoverOrder;

//=======================    geographical position data    =======================

/// Maximum number of point given KP coordinate
#define MAX_KP_COOR             10



///< Stucture to store a track kilometre reference data for calculation of geographical position
typedef struct SKPReference
{
    SBG_Id      Ref_BG      ;   ///< Reference BG
    t_distance  dKPOffset   ;   ///< Track kilometre offset from the reference BG
    t_distance  dRouteKP    ;   ///< Track kilometre reference value
    bool        bNominalDir ;   ///< Indicates if route kilometer direction is nominal (same than train direction)

} SKPReference;


/// Structure to store geographical position information
typedef struct SGeoPos
{
    bool            bValid                  ;   ///< Validity of data
    bool            bInfillData             ;   ///< Data received via infill equipment: use infill location reference
    t_distance      dRefLoc                 ;   ///< Reference location
    int32_t lNb                     ;   ///< Number of point in list to used for the calculation of KP
    SKPReference    aKP_Coor[MAX_KP_COOR]   ;   ///< List of point for calculation of KP

} SGeoPos;


//=======================    mode profile data    =======================

/// Maximum number of mode discontinuities
#define MAX_MODE_DISC    10

/// structure to store specific mode area
typedef struct SModeChange
{
    t_distance  dLocation       ;   ///< Location of the mode change
    eTrainMode  New_Mode        ;   ///< New mode
    t_distance  dLength         ;   ///< Length of the required mode
    t_distance  dAckLength      ;   ///< Length of ack area before the mode change
    t_speed     dSpeed          ;   ///< Speed limit for the indicated mode
    bool        bSuperviseSL    ;   ///< Indicate the supervision of the beginning of the mode profile (Q_MAMODE)
}SModeChange;

/// Structure to store mode profile data
typedef struct SModeProf
{
    bool            bValid          ;   ///< Validity of data
    bool            bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    t_distance      dRefLoc         ;   ///< Reference location (used in msg transfert)

    int32_t lModeChangeNb   ;   ///< Number of mode change in list

    SModeChange aModeChange[MAX_MODE_DISC]    ;   ///< List of specific mode area

} SModeProf;

/// Structure used for communication between dataserver and speedsup
typedef struct SModeProfileUpdate
{
    SModeProf ModeProfile;
    bool      bUpdate;

} SModeProfileUpdate;


//=======================    MA request parameters data    =======================

/// Structure to store MA request parameters
typedef struct SMARqstPara
{
    bool        bValid                      ;   ///< Validity of data
    bool        bInfillData                 ;   ///< Data received via infill equipment: use infill location reference

    bool        bEoa_Warn_Time_Relevant     ;   ///< False if no time before indication point should be taken into account for a new MA request
    t_time      dEoa_Warn_Time              ;   ///< Time before reach IP for EOA at warning speed

    bool        bTimeOut_Relevant           ;   ///< False when no time before "section timer expiration" should be taken into account for a new MA request
    t_time      dTimeOut                    ;   ///< Time before expiration of deferment timer

    bool        bMA_Rqst_Cycl_Time_Relevant ;   ///< False if no periodic repetition
    t_time      dMA_Rqst_Cycl_Time          ;   ///< MA request cyclic time

} SMARqstPara;


//=======================    Repositioning data    =======================

typedef enum eRepositioningAction
{
    ReposCheckDistance      = 0,
    ReposAnnounced          = 1,
    ReposAwaitedBGReceived  = 2,
    ReposUnexpectedReposBG  = 3
} eRepositioningAction;

/// Structure to store repositioning info
typedef struct SReposInfo
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dSection    ;   ///< Section length

} SReposInfo;


//=======================    track condition    =======================




/// Structure for traction data
typedef struct STractionParam
{
    int32_t lVoltage;
    int32_t lCountry;

} STractionParam;

/// Structure to store track condition: traction system
typedef struct SCondTraction
{
    bool            bValid;         ///< Validity of data
    bool            bInfillData;    ///< Data received via infill equipment: use infill location reference

    t_distance      dLocation;      ///< Location of the traction power change
    STractionParam  Parameters;     ///< Data associated to location

} SCondTraction;

/// structure to store specific traction change
typedef struct STractChange
{
    t_distance      dLocation;      ///< Location of the traction power change
    STractionParam  Data;           ///< Data associated to location

} STractChange;


/// Maximum number of packets that can be stored (about power changes ahead)
#define MAX_CHANGE_TRACTION_POWER   5

/// structure containing the list of traction power change
typedef struct STractionChangeList
{
    int32_t lNb;                       ///< Number of traction power change
    STractChange TractChange[MAX_CHANGE_TRACTION_POWER];

} STractionChangeList;


/// Enumeration for track condition type
/// Values of enum correspond to SRS M_TRACKCOND variable
typedef enum eTrackCond
{
    // SRS
    COND_NON_STOP                       = 0,    ///< Non stopping area – other reasons
    COND_TUNNEL_STOPPING_AREA           = 1,    ///< Non stopping tunnel area
    COND_SOUND_HORN                     = 2,    ///< Sound horn
    COND_POWERLESS_LOW_PANTO            = 3,    ///< Powerless section – lower pantograph
    COND_RADIO_HOLE                     = 4,    ///< Radio hole (stop supervising T_NVCONTACT)
    COND_AIR_TIGHTNESS                  = 5,    ///< Air tightness
    COND_SWITCHOFF_REGENERATIVE_BRAKE   = 6,    ///< Switch off regenerative brake
    COND_SWITCHOFF_EDDY_BRAKE_FOR_SB    = 7,    ///< Switch off eddy current brake for service brake
    COND_SWITCHOFF_MAGNETIC_SHOE_BRAKE  = 8,    ///< Switch off magnetic shoe brake
    COND_MAIN_SWITCH_OFF                = 9,    ///< Powerless section – switch off the main power switch
    COND_SWITCHOFF_EDDY_BRAKE_FOR_EB    = 10,   ///< Switch off eddy current brake for emergency brake

    // Non SRS
    COND_METAL_MASS                     = 11,   ///< added to manage packet 67
    COND_TSR                            = 12,   ///< added for TSR
    COND_LX                             = 13,   ///< added to manage packet 88
    COND_SHUNTING_WARNING               = 19,   ///< added to manage mode profile
    COND_ON_SIGHT_WARNING               = 20,   ///< added to manage mode profile
    COND_STAFFRESP_WARNING              = 21,   ///< added to manage mode profile
    COND_LX_NON_PROTECTED               = 31,   ///< added for non protected LX

    COND_TRACTION_VOLT_AC_25000,
    COND_TRACTION_VOLT_AC_15000,
    COND_TRACTION_VOLT_DC_3000,
    COND_TRACTION_VOLT_DC_1500,
    COND_TRACTION_VOLT_DC_600_750,
    COND_TRACTION_VOLT_DC_UNAVAILABLE,

    COND_NONE,                                  ///< Condition trigger initialization
    COND_RESET,                                 ///< Condition in case Q_TRACKINIT=1
    COND_TSR_SPD_OFFSET                 = 128   ///< Added for TSR (speed = (value -128)*5 )

} eTrackCond;


/// Macro to get the string corresponding to a track condition
#define STR_COND(d) \
    (d)==COND_NONE                          ? "NONE"                                : \
    (d)==COND_TUNNEL_STOPPING_AREA          ? "TUNNEL STOPPING AREA"                : \
    (d)==COND_SOUND_HORN                    ? "SOUND HORN"                          : \
    (d)==COND_RESET                         ? "RESET"                               : \
    (d)==COND_NON_STOP                      ? "NON STOPPING AREA"                   : \
    (d)==COND_POWERLESS_LOW_PANTO           ? "POWERLESS LOW PANTO"                 : \
    (d)==COND_RADIO_HOLE                    ? "RADIO HOLE"                          : \
    (d)==COND_AIR_TIGHTNESS                 ? "AIR TIGHTNESS"                       : \
    (d)==COND_SWITCHOFF_REGENERATIVE_BRAKE  ? "SWITCHOFF REGENERATIVE BRAKE"        : \
    (d)==COND_SWITCHOFF_EDDY_BRAKE_FOR_SB   ? "SWITCHOFF EDDY CURRENT BRAKE FOR SB" : \
    (d)==COND_SWITCHOFF_EDDY_BRAKE_FOR_EB   ? "SWITCHOFF EDDY CURRENT BRAKE FOR EB" : \
    (d)==COND_SWITCHOFF_MAGNETIC_SHOE_BRAKE ? "SWITCHOFF MAGNETIC SHOE BRAKE"       : \
    (d)==COND_MAIN_SWITCH_OFF               ? "NEUTRAL SECTION"                     : \
    (d)==COND_TSR                           ? "TEMPORARY SPEED RESTRICTION"         : \
    (d)==COND_LX                            ? "LX"                                  : \
    (d)==COND_LX_NON_PROTECTED              ? "LX NON PROTECTED"                    : \
    (d)==COND_METAL_MASS                    ? "BIG METAL MASSES"                    : \
    (d)==COND_SHUNTING_WARNING              ? "SHUNTING WARNING"                    : \
    (d)==COND_ON_SIGHT_WARNING              ? "ON SIGHT WARNING"                    : \
    (d)==COND_STAFFRESP_WARNING             ? "STAFF RESPONSIBLE"                   : \
    (d)==COND_TRACTION_VOLT_AC_25000        ? "TRACTION SYSTEM (VOLT_AC_25000)"     : \
    (d)==COND_TRACTION_VOLT_AC_15000        ? "TRACTION SYSTEM (VOLT_AC_15000)"     : \
    (d)==COND_TRACTION_VOLT_DC_3000         ? "TRACTION SYSTEM (VOLT_DC_3000)"      : \
    (d)==COND_TRACTION_VOLT_DC_1500         ? "TRACTION SYSTEM (VOLT_DC_1500)"      : \
    (d)==COND_TRACTION_VOLT_DC_600_750      ? "TRACTION SYSTEM (VOLT_DC_600_750)"   : \
    (d)==COND_TRACTION_VOLT_DC_UNAVAILABLE  ? "TRACTION SYSTEM (VOLT_DC_UNAVAILABLE)": \
    "???"

/// Enumeration for track condition type
typedef enum eTCStatus
{
    TC_STATUS_BEFORE    ,   ///< Train is before the track condition and announcement
    TC_STATUS_ANNOUNCE  ,   ///< Track condition is annouced
    TC_STATUS_INSIDE    ,   ///< Track condition applies
    TC_STATUS_AFTER     ,   ///< Track condition is passed
    TC_STATUS_END           ///< Track condition finished

} eTCStatus;

/// Enumeration for track condition type
#define STR_COND_STATUS(d) \
    (d)==TC_STATUS_BEFORE   ? "Before"    : \
    (d)==TC_STATUS_ANNOUNCE ? "Announce"  : \
    (d)==TC_STATUS_INSIDE   ? "Inside"    : \
    (d)==TC_STATUS_AFTER    ? "After"     : \
    (d)==TC_STATUS_END      ? "End"       : \
    "???"


/// Structure to store the current track condition application
/// It is a shared structure managed by dataserver, initialized at 0 at EVC simulation start
/// This is a list of counters, in case several track conditions overlap
typedef struct STrackCondCurrent
{
    int32_t lNonStop                       ;   ///< Indicate if train is in a track condition: Non stopping area – other reasons
    int32_t lPowerlessLowPanto             ;   ///< Indicate if train is in a track condition: Powerless section – lower pantograph
    int32_t lRadioHole                     ;   ///< Indicate if train is in a track condition: Radio hole (stop supervising T_NVCONTACT)
    int32_t lAirTightness                  ;   ///< Indicate if train is in a track condition: Air tightness
    int32_t lSwitchOffRegenerativeBrake    ;   ///< Indicate if train is in a track condition: Switch off regenerative brake
    int32_t lSwitchOffEddyCurBrakeForSB    ;   ///< Indicate if train is in a track condition: Switch off eddy current brake for service brake
    int32_t lSwitchOffEddyCurBrakeForEB    ;   ///< Indicate if train is in a track condition: Switch off eddy current brake for emergency brake
    int32_t lSwitchOffMagneticShoeBrake    ;   ///< Indicate if train is in a track condition: Switch off magnetic shoe brake
    int32_t lPowerlessMainSwitchOff        ;   ///< Indicate if train is in a track condition: Powerless section – switch off the main power switch
    int32_t lBigMetalMasses                ;   ///< Indicate if train is in a track condition: Big metal masses

} STrackCondCurrent;


/// Structure to store location of a track condition
/// the simplified structure used in a static array (able to store several packets 68)
/// structure used to send condition states to other interfaces at their occuring time
/// note: for COND_RESET, stop location is not relevant
typedef struct STrackCondTrigger
{
    t_distance    dLocationStart;   ///< Start location of the track condition
    t_distance    dLocationStop ;   ///< Stop location of the track condition
    eTrackCond    TrackCond     ;   ///< Track condition type (including reset)
    eTCStatus     TCStatus      ;   ///< Track condition status
    t_time        dEndTime      ;   ///< Time stamp when the train reaches the end of the TC

} STrackCondTrigger;


/// Size of the array for the management of the track condition and of the big metal masses
#define MAX_COND_TRIGGERS   50


/// Maximum number of track condition about big metal masses
#define MAX_METAL_COND    33

/// Structure to store track condition: big metal masses
typedef struct SCondBigMetal
{
    bool                bValid                      ;   ///< Validity of data
    bool                bInfillData                 ;   ///< Data received via infill equipment: use infill location reference

    t_distance          dRefLoc                     ;   ///< Reference location
    int32_t lNb                         ;   ///< Number of condition
    STrackCondTrigger   aCondition[MAX_METAL_COND]  ;   ///< List of big metal masses

} SCondBigMetal;



/// Maximum number of track conditions
#define MAX_TRACK_COND        33

/// Structure to store track condition (Packet 68)
typedef struct STrackCond
{
    bool                bValid                      ;   ///< Validity of data
    bool                bInfillData                 ;   ///< Data received via infill equipment: use infill location reference

    t_distance          dRefLoc                     ;   ///< Reference location (used in msg transfert)

    // if Q_TRACKINIT == 1
    bool                bReset                      ;   ///< Reset track condition required
    t_distance          dResetLocation              ;   ///< Location to start the reset

    // if Q_TRACKINIT == 0
    int32_t lNb                         ;   ///< Number of condition
    STrackCondTrigger   aCondition[MAX_TRACK_COND]  ;   ///< List of track condition

} STrackCond;


//=============   Track Cond. Station Platform information   ==========

/// structure to store Condition station
typedef struct SCondStation
{
    t_distance          dLocationStart    ;   ///< Start location of the track condition
    t_distance          dLocationStop     ;   ///< Stop location of the track condition
    ePlatformElevation  platformElevation ;   ///< Nominal height of platform above rail level (refer to TSI infrastructure)
    ePlatformPosition   platformPosition  ;   ///< Platform position (relative to direction of authorised movement)
} SCondStation;

/// Structure to store informations about station platform
typedef struct STrackCondStation
{
    bool        bValid          ;   ///< Validity of data
    bool        bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    int32_t     lNbStation      ;   ///< Number of station platform
    bool        bResetProfile   ;   ///< Indicate if profile shall be reseted
    t_distance  dResetLocation  ;   ///< Distance to start of empty profile
    t_distance  dRefLoc         ;   ///< Reference location
    eTrainMode  train_Mode      ;   ///< accounts for actions to be taken at train mode change

    SCondStation aCondStation[MAX_ITERATIONS];

} STrackCondStation;

//=======================    adhesion factor    =======================

/// Structure to store adhesion factor data
typedef struct SAdhesion
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dLocation   ;   ///< Location of adhesion change
    t_distance  dLength     ;   ///< Length of adhesion change
    int32_t lAdhesion   ;   ///< Value of adhesion factor: 70% or 100%

} SAdhesion;


//=======================    RBC transition    =======================

/// Structure to store RBC handover data
typedef struct SRBCTransition
{
    bool            bValid                          ;   ///< Validity of data
    bool            bInfillData                     ;   ///< Data received via infill equipment: use infill location reference

    t_distance      dRefLoc 			            ;   ///< Reference location
    t_distance      dLocation                       ;   ///< Location of RBC handover

    int32_t lRBCCountry                     ;   ///< Accepting RBC country id
    int32_t lRBCId                          ;   ///< Accepting RBC identity
    uint8_t           uszRBCPhoneNumber[PHONE_NB_SIZE];   ///< Accepting RBC phone nb

    bool            bForSleeping                    ;   ///< Indicate if it concerns EVC in SL mode

} SRBCTransition;



//=======================    End Of Loop Marker    =======================

/// Structure to store information about loop
typedef struct SEOLMData
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    int32_t lLoop_Id    ;   ///< ETCS loop identity
    t_distance  dLocation   ;   ///< Location of loop
    t_distance  dLength     ;   ///< Length of loop
    eDirection  LoopDir     ;   ///< Direction of loop
    int32_t lCode       ;   ///< Spectrum code of loop message

} SEOLMData;


//=======================  stop SH on desk opening packet    =======================
typedef struct SExitSH
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

} SExitSH ;

//=======================    Assign coordinate    =======================

/// Structure containing assignement of coordinate system
typedef struct SAssignCoor
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    SBG_Id      BaliseGroup ;   ///< Balise group id with assigned orientation

} SAssignCoor;


//=======================    train running number from RBC    =======================


/// Structure containing the train running number received from RBC
typedef struct STrainRunNbFromRBC
{
    bool            bValid          ;   ///< Validity of data
    bool            bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    uint32_t ulTrain_Run_Nb  ;   ///< Train running number received from RBC

} STrainRunNbFromRBC;



//=======================    Location report parameters    =======================


/// Enumeration for special location to report position
typedef enum ePRSpeLoc
{
    NOW                     ,   ///< Report position now
    AT_LRBG_COMPLIANT       ,   ///< Report position at each balise group
    NOT_AT_LRBG_COMPLIANT       ///< Do not report position at each balise group

} ePRSpeLoc;



/// Maximum number of other location to report position
#define MAX_OTHER_LOC_NB        10

/// structure to store other location
typedef struct SOtherLoc
{
    t_distance  dLocation           ;   // location to send a loc report
    bool        bForTrainRearLoc    ;   // qualifier for location (front or rear end)
} SOtherLoc;

/// Structure containing the location report parameters
typedef struct SLocReportPara
{
    bool            bValid              ;   ///< Validity of data
    bool            bInfillData         ;   ///< Data received via infill equipment: use infill location reference

    t_distance      dRefLoc             ;   ///< Reference location

    bool            bTimePeriodic       ;   ///< Indicate if time periodic position report is required
    t_time          dTimePeriod         ;   ///< Maximum time between two report
    bool            bDistPeriodic       ;   ///< Indicate if distance periodic position report is required
    t_distance      dDistPeriod         ;   ///< Maximum distance between two report
    ePRSpeLoc       PRSpecialLoc        ;   ///< Special location to send report

    int32_t lOtherLocNb         ;   /// Number of other location to report position

    SOtherLoc aOtherLoc[MAX_OTHER_LOC_NB]       ;   ///< List of other location to report position

} SLocReportPara;



//=======================    Staff responsible data    =======================


/// Structure containing information about stop request for train in SR
typedef struct SSRStop
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference
    bool        bStop       ;   ///< Indicate if train in SR must stop

} SSRStop;


/// Structure containing a SR authority
typedef struct SSRAuthority
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dDistance   ;   ///< Distance that can be run in SR mode

} SSRAuthority;


/// Structure containing the list of BG in the SR area
typedef struct SSRArea
{
    bool        bValid              ;   ///< Validity of data
    bool        bInfillData         ;   ///< Data received via infill equipment: use infill location reference

    int32_t lBGNb               ;   ///< Number of BG in SR area
    SBG_Id      aBGList[MAX_BG_NB]  ;   ///< List of BG id which the train can pass over in SR mode

} SSRArea;


//=======================    TSR data    =======================


/// Maximum number of TSR in a message
#define MAX_TSR_NB      10


/// Structure containing TSR data
typedef struct STSRData
{
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference
    int32_t lId         ;   ///< TSR id
    t_distance  dLocation   ;   ///< Location of the speed restriction
    bool        bStartAt0   ;   ///< Indicate that TSR start at BG
    t_distance  dLength     ;   ///< Length of the speed restriction
    t_speed     dSpeed      ;   ///< Speed restriction

} STSRData;


/// Structure containing the list of the received TSR
typedef struct STSRList
{
    bool        bValid              ;   ///< Validity of data

    int32_t lTSRnb              ;   ///< Number of TSR
    STSRData    aTSRList[MAX_TSR_NB];   ///< List of TSR

} STSRList;


/// Structure containing TSR revocation data
typedef struct STSRRevocation
{
    bool    bInfillData ;   ///< Data received via infill equipment: use infill location reference
    int32_t lTSRid      ;   ///< Identifier of TSR to revoque

} STSRRevocation;


/// Maximum number of TSR revocation in a message
#define MAX_TSR_REVOCATION_NB   10


/// Structure containing the list of received TSR revocation  (Packet 66)
typedef struct STSRrevocationList
{
    bool            bValid                                      ;   ///< Validity of data

    int32_t lTSRRevocationNb                            ;   ///< Number of TSR revocations
    STSRRevocation  aTSRRevocationList[MAX_TSR_REVOCATION_NB]   ;   ///< List of TSR revocation

} STSRrevocationList;

/// Structure to store inhibition of revocable TSRs from balises in L2/3 (Packet 64)
typedef struct STSRInhibitRevocable
{
    bool        bValid              ;   ///< Validity of data
    bool        bInfillData         ;   ///< Data received via infill equipment: use infill location reference

} STSRInhibitRevocable;



/// Structure containing an axle load speed restriction
typedef struct SAxleLoadSpeedRestriction
{
    int32_t lNb                             ;   ///< Number of axle load speed restrictions
    t_distance  dLocation   [MAX_AXLELOAD_PROF] ;   ///< List of axle load speed restriction start locations (in meters)
    t_distance  dLength     [MAX_AXLELOAD_PROF] ;   ///< List of axle load speed restriction lengths (in meters)
    t_speed     dSpeedLimit [MAX_AXLELOAD_PROF] ;   ///< List of axle load speed restriction values (in m/s)

} SAxleLoadSpeedRestriction;

/// structure to store speed limit
typedef struct SLimitation
{
    eAxleLoadCategory   axleLoadCategory        ;   ///< Axle load
    t_speed             dSpeedLimit             ;   ///< Speed limitation
} SLimitation;

/// structure to store discontinuities
typedef struct SDiscont
{
    t_distance  dDistance               ;   ///< Location of axle load restriction
    t_distance  dLength                 ;   ///< Length of the axle load restriction
    int32_t lNbLimit                ;   ///< Number of speed limitation

    SLimitation aLimitation[MAX_AXLELOAD_LIMIT]   ;   ///< List of speed limit
} SDiscont;


/// Structure containing the axle load speed profile
typedef struct SAxleLoadSpeedProf
{
    bool            bValid                  ;   ///< Validity of data
    bool            bInfillData             ;   ///< Data received via infill equipment: use infill location reference

    bool            bResetProf              ;   ///< Request to reset axle load profile
    t_distance      dResetLoc               ;   ///< Location to reset axle load profile
    t_distance      dRefLoc                 ;   ///< Reference location
    int32_t lNb                     ;   ///< Number of discontinuities in profile

    SDiscont aProfile[MAX_AXLELOAD_PROF]           ;   ///< List of discontinuities

} SAxleLoadSpeedProf;



//=================  Permitted Braking Distance Information  =====================

#define MAX_PERM_BRAKE_DIST       32

/// structure to store restrictions
typedef struct SRestriction
{
    t_distance  dBrakingDistance            ;   ///< Permitted Braking Distance
    int32_t lGradient                   ;   ///< Default gradient for PBD Speed restriction
    bool        bInterventionType           ;   ///< Qualifier for Permitted Braking Distance (0 = EB intervention requested ; 1 = SB intervention requested)
    t_distance  dSpeedRestrictionStart      ;   ///< Incremental distance to the start of the next speed restriction to ensure permitted braking distance
    t_distance  dSpeedRestrictionLength     ;   ///< Length of speed restriction to ensure permitted braking distance
} SRestriction;

typedef struct SPermBrakingDist
{

    bool    bValid              ;   ///< Validity of data
    bool    bInfillData         ;   ///< Data received via infill equipment: use infill location reference

    bool        bReset              ;   ///< Request calculation of in-board speed restriction
    int32_t lNbRestriction      ;   ///< Number of restrictions
    t_distance  dResetLocation      ;   ///< Distance to start of empty profile (bReset = true)
    t_distance  dRefLoc             ;   ///< Reference location

    SRestriction aRestriction[MAX_ITERATIONS]              ;   ///< List of restrictions

} SPermBrakingDist;


//=======================    route suitability data    =======================

/// Enumeration for type of route suitability
typedef enum eSuitability
{
    SUIT_UNDEF          = 0 ,   ///< No route suitability
    SUIT_NO_RESTRICTION     ,   ///< Reset route suitability
    SUIT_LOADING_GAUGE         ,   ///< Route suitability about loading gauge
    SUIT_AXLE_LOAD          ,   ///< Route suitability about maximum axle load
    SUIT_TRACTION           ,   ///< Route suitability about traction system
    MAX_SUITABILITY_TYPE

} eSuitability ;


/// Macro to get a string corresponding to a route suitability
#define STR_RS(d) \
    (d)==SUIT_UNDEF             ?   "SUIT_UNDEF"            : \
    (d)==SUIT_NO_RESTRICTION    ?   "SUIT_NO_RESTRICTION"   : \
    (d)==SUIT_LOADING_GAUGE     ?   "SUIT_LOADING_GAUGE"    : \
    (d)==SUIT_AXLE_LOAD         ?   "SUIT_AXLE_LOAD"        : \
    (d)==SUIT_TRACTION          ?   "SUIT_TRACTION"         : \
    "???"


/// Structure for route suitability data
typedef struct SSuitValue
{
    int32_t lLineGauge         ;  ///< Loading gauge profile
    eAxleLoadCategory axleLoadCategory   ;  ///< Maximum axle load
    STractionParam    TractionData       ;  ///< Traction voltage & country

} SSuitValue;


/// Maximum number of route suitability
#define MAX_SUITABILITY        33


/// structure to store route unsuitability
typedef struct SSuitability
{
    t_distance      dLocation       ;   ///< Location of the route suitability
    eSuitability    SuitabilityType ;   ///< Type of route suitability
    SSuitValue      SuitValue       ;   ///< Value of the route unsuitability

} SSuitability;


/// Structure containing the received route suitability
typedef struct SRouteSuitability
{
    bool                bValid          ;   ///< Validity of data
    bool                bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    t_distance          dRefLoc         ;   ///< Reference location
    int32_t lNbSuitability  ;   ///< Number of valid entries in suitability array

    SSuitability aSuitability[MAX_SUITABILITY]     ;   ///< List of route unsuitability

} SRouteSuitability;



/// Maximum number of managed route suitability
#define MAX_SUITABILITY_ENTRIES    MAX_SUITABILITY_TYPE


/// Structure containing route suitability information
typedef struct SRouteSuitabilityEntry
{
    t_distance      dLocationStart  ;   ///< Start location

    eSuitability    SuitabilityType ;   ///< Type of route suitability
    SSuitValue      SuitValue       ;   ///< Value of the route unsuitability

    // whether or not
    bool            bDriverInformed ;   ///< Indicate if driver has been informed, in case of incompatibility

} SRouteSuitabilityEntry;


/// structure containing the curret list of RS
typedef struct SRouteSuitList
{
    int32_t lNb                 ;  ///< Number of RS

    SSuitability RS[MAX_SUITABILITY_ENTRIES];

} SRouteSuitList;



//=======================    danger for shunting    =======================

/// Structure containing the received information about danger for SH
typedef struct SSHDanger
{
    bool        bValid          ;   ///< Validity of data
    bool        bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    bool        bStop           ;   ///< Indicate if train in SH must stop

} SSHDanger;



//=======================    Radio infill area    =======================

/// Structure containing information about radio infill area
typedef struct SRIUArea
{
    bool            bValid                          ;   ///< Validity of data
    bool            bInfillData                     ;   ///< Data received via infill equipment: use infill location reference

    t_distance      dRefLoc                         ;   ///< Reference location
    eConnection     Connection                      ;   ///< Connection / disconnection order
    int32_t lCountryId                      ;   ///< Country id of RIU
    int32_t lRIUId                          ;   ///< Radio infill unit identity
    uint8_t   uszRIUPhoneNumber[PHONE_NB_SIZE];   ///< Radio subscriber number
    t_distance      dLocation                       ;   ///< Location of connection, disconnection
    SBG_Id          MainBG                          ;   ///< Identity of the main BG associated to infill info

} SRIUArea;


//=======================    assignement of reference location    =======================

/// Structure containing information about assignement of reference location
typedef struct SRefLocAssign
{
    bool        bValid  ;   ///< Validity of data
    SBG_Id      RefBG   ;   ///< Reference BG

} SRefLocAssign;


//=======================    reverse data    =======================


/// Structure containing information about reversing area
typedef struct SReversingArea
{
    bool        bValid          ;   ///< Validity of data
    bool        bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dStartLocation  ;   ///< Start location of the reversing area
    t_distance  dLength         ;   ///< Length of the reversing area

} SReversingArea;


/// Structure containing information about reverse supervision data
typedef struct SReverseSupervision
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    t_speed     dSpeed      ;   ///< Supervision speed
    t_distance  dDistance   ;   ///< Supervision distance

} SReverseSupervision;


//=======================    track ahead free request    =======================

/// Structure containing information about track ahead free request
typedef struct STrackFreeRqst
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    t_distance  dLocation   ;   ///< Location for display of TAF
    t_distance  dLength     ;   ///< Length of display of TAF

} STrackFreeRqst;


/// Structure containing information about Track Ahead Free up to level 2/3 transition location
typedef struct STrackAheadFreeInfo
{
    bool        bValid      ;   ///< Validity of data
    bool        bInfillData ;   ///< Data received via infill equipment: use infill location reference

    SBG_Id      RefBG       ;   ///< Reference BG

} STrackAheadFreeInfo;

//=======================    Radio Network registration    =======================

/// Size of radio network id (in bytes)
#define RADIO_NETWORK_ID_SIZE       6                   ///< NID_MN : See Subset 026-7
#define RADIO_NETWORK_NAME_SIZE_MAX     16                  ///< See 3GPP TS 27.007 version 8.5.0 Release 8

/// Max of radio network id
#define RADIO_NETWORK_MAX_LIST_SIZE 50                   ///< See Subset 37 : EuroRadio FIS G.1.1.1, refer to SIM FFFIS

/// Radio Network Registration order repeat timeout (in s)
#define RADIO_NETWORK_REG_REQUEST_TIMEOUT   5

/// Structure containing information about radio network registration
typedef struct SRadioNetworkRegis
{
    bool        bValid                                  ;   ///< Validity of data
    bool        bInfillData                             ;   ///< Data received via infill equipment: use infill location reference

    uint8_t       uszRadioNetworkID[RADIO_NETWORK_ID_SIZE];   ///< Radio network identifier

} SRadioNetworkRegis;


//=======================    External Data    =======================


/// Maximum size of packet (in byte)
#define MAX_PACKET_SIZE     1024

/// Structure containing information about External Data (packet 44)
typedef struct SExternalData
{
    bool            bValid                  ;   ///< Validity of data
    bool            bInfillData             ;   ///< Data received via infill equipment: use infill location reference
    t_distance      dRefLoc                 ;   ///< Reference location

    int32_t lUserSystem             ;   ///< Identity of user system (NID_XUSER)
    uint8_t           uszData[MAX_PACKET_SIZE];   ///< Binary data
    int32_t lDataLength             ;   ///< Length of data (in bytes)

} SExternalData;



//================    Level Crossing information    ====================

/// Structure containing Level crossing information
typedef struct SLevelCrossInfo
{
    bool            bValid          ;   ///< Validity of data
    bool            bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    int32_t lLCId           ;   ///< Level corssing identifier
    t_distance      dLocation       ;   ///< Location of level crossing start (m)
    t_distance      dLength         ;   ///< Length of level crossing area (m)
    bool            bProtected      ;   ///< Indicate if level crossing is protected or not
    t_speed         dSpeedLimit     ;   ///< Permitted speed if LX is not protected (m/s)
    bool            bStopReq        ;   ///< Indicate if stop is required before no protected LX
    t_distance      dStopAreaLength ;   ///< Length of the stopping area before LX start (m)

} SLevelCrossInfo;

/// maximum number of level crossing information in a message
#define MAX_LX_INFO_NB      10


///Structure containing a list of level crossing information
typedef struct SLXList
{
    int32_t lNb                 ;   ///< Number of level crossing info

    SLevelCrossInfo aLX[MAX_LX_INFO_NB] ;   ///< array of LX info

} SLXList;


//=========    Session management with neighbouring RIU     ==========

/// Enumeration for Session management with neighbouring Radio In-fill Unit
typedef struct SSessionRIU
{
    bool        bValid          ;   ///< Validity of data
    bool        bInfillData     ;   ///< Data received via infill equipment: use infill location reference

    eConnection     Connection      ;   /// Communication session order : 0 = Terminate , 1 = Establish

    uint32_t ulCountry       ;   ///< Country id (NID_C)
    uint32_t ulRIU           ;   ///< Country id (NID_RIU)

    uint8_t   uszRIUPhoneNumber[PHONE_NB_SIZE]    ;   ///< Radio phone number
} SSessionRIU;

//=======================    balise message    =======================

/// Enumeration for indication of duplicate balise
typedef enum eDuplicate
{
    NOTDUP  ,   ///< Balise is not duplicate
    DUPNEXT ,   ///< Duplicate next balise
    DUPPREV ,   ///< Duplicate previous balise
    DUPSPARE    ///< Not used

} eDuplicate;


/// Structure containing information about balise message parameters
typedef struct SBalisePara
{
    t_distance  dBGOdoLoc       ;   ///< Location of the balise in odometer coordinates
    t_distance  dOdoErrorLoc    ;   ///< Odometer error in the location of the balise
    SBG_Id      Id              ;   ///< Id of the BG
    int32_t lPig            ;   ///< Position in group (SRS_N_PIG + 1)
    int32_t lTotal          ;   ///< Total number of balises in group (SRS_N_TOTAL + 1)
    int32_t lM_Count        ;   ///< Value of message counter for detection of message change
    bool        bLinkedBalise   ;   ///< Indicate if balise is linked
    eDuplicate  M_Dup           ;   ///< Indicate if balise is duplicated

} SBalisePara;


//=======================    radio message    =======================


/// Enumeration for type of radio message to send
typedef enum eRadMsgToSendId
{
    RAD_MSG_VALID_TRAIN_DATA    =   129,    ///< validated train data
    RAD_MSG_SH_RQST             =   130,    ///< shunting request
    RAD_MSG_MA_RQST             =   132,    ///< MA request
    RAD_MSG_POS_REPORT          =   136,    ///< train position report
    RAD_MSG_MA_REVOC_ACK        =   137,    ///< request to stop earlier is granted
    RAD_MSG_MA_REVOC_NACK       =   138,    ///< request to stop earlier is rejected
    RAD_MSG_ACK                 =   146,    ///< acknowledgment
    RAD_MSG_EMER_STOP_ACK       =   147,    ///< acknowledgment of emergency message
    RAD_MSG_TRACK_FREE          =   149,    ///< track free granted
    RAD_MSG_END_MISSION         =   150,    ///< end of mission
    RAD_MSG_INFILL_RQST         =   153,    ///< Radio infill request
    RAD_MSG_NO_COMPATIBLE_VER   =   154,    ///< no compatible version supported
    RAD_MSG_INIT_SESSION        =   155,    ///< initiation of a communication session
    RAD_MSG_TERM_SESSION        =   156,    ///< terminate a communication session
    RAD_MSG_SOM_POS_REPORT      =   157,    ///< SoM position report
    RAD_MSG_TXT_MSG_CONFIRMED   =   158,    ///< text message confirmed by driver
    RAD_MSG_SESSION_ESTABLISH   =   159     ///< session established

} eRadMsgToSendId;

/// Macro to get the string corresponding to the type of radio message to send
#define STR_TX_RADIO(d) \
    (d)==RAD_MSG_VALID_TRAIN_DATA   ?   "Validated train data"                  : \
    (d)==RAD_MSG_SH_RQST            ?   "Shunting request"                      : \
    (d)==RAD_MSG_MA_RQST            ?   "MA request"                            : \
    (d)==RAD_MSG_POS_REPORT         ?   "Train position report"                 : \
    (d)==RAD_MSG_MA_REVOC_ACK       ?   "Request to stop earlier is granted"    : \
    (d)==RAD_MSG_MA_REVOC_NACK      ?   "Request to stop earlier is rejected"   : \
    (d)==RAD_MSG_ACK                ?   "Acknowledgment"                        : \
    (d)==RAD_MSG_EMER_STOP_ACK      ?   "Acknowledgment of emergency message"   : \
    (d)==RAD_MSG_TXT_MSG_CONFIRMED  ?   "Text message confirmed by driver"      : \
    (d)==RAD_MSG_TRACK_FREE         ?   "Track ahead free granted"              : \
    (d)==RAD_MSG_END_MISSION        ?   "End of mission"                        : \
    (d)==RAD_MSG_INFILL_RQST        ?   "Radio infill request"                  : \
    (d)==RAD_MSG_NO_COMPATIBLE_VER  ?   "No compatible version"                 : \
    (d)==RAD_MSG_INIT_SESSION       ?   "Initiation of a communication session" : \
    (d)==RAD_MSG_TERM_SESSION       ?   "Terminate a communication session"     : \
    (d)==RAD_MSG_SOM_POS_REPORT     ?   "SoM position report"                   : \
    (d)==RAD_MSG_SESSION_ESTABLISH  ?   "Session established"                   : \
    "???"


/// Structure containing data for radio infill request
typedef struct SInfillData
{
    SBG_Id  BG              ;   ///< Target BG
    bool    bEnterIFArea    ;   ///< Indicate if entry or exit of infill area

}  SInfillData;

typedef enum eMARqstReason
{
    MARqstReason_StartSelected      = 1,
    MARqstReason_TimeEOAReached     = 2,
    MARqstReason_TimeSectionReached = 4,
    MARqstReason_TrackDescDeleted   = 8,
    MARqstReason_TrackAheadFree     = 16
} eMARqstReason;


/// Enumeration for qualifier for emergency stop acknowledgement
typedef enum eESAckQualifier
{
    ES_COND_ACK_EOA_CHANGED     = 0,    ///< Conditional Emergency Stop accepted, with update of EoA
    ES_COND_ACK_EOA_NOT_CHANGED = 1,    ///< Conditional Emergency Stop accepted, with no update of EoA
    ES_UNCOND_ACK               = 2,    ///< Unconditional Emergency Stop accepted
    ES_NACK                     = 3,    ///< Emergency stop (Conditional or Unconditional) rejected, whatever the reason

} eESAckQualifier;




/// Structure containing data about the radio message to send
typedef struct SRadMsgToSend
{
    eRadMsgToSendId     MsgId           ;   ///< Identity of the message to send
    eMARqstReason       MARqstReason    ;   ///< MA Request reason if MsgId is MA Request
    double              dTimeStampRef   ;   ///< Time stamp of the reference message (ie for acknowledgement, ...)

    SInfillData         Infill_Data     ;   ///< Infill data for IF MA request

    int32_t lEmerId         ;   ///< Emergency stop id for ack
    eESAckQualifier     EmerQualifier   ;   ///< Qualifier for emergency stop ack

    bool                bError          ;   ///< Indicate if error report is inluded in message
    eM_Error            ErrorType       ;   ///< Error code to use in error report

    bool                bLTRInfo        ;   ///< Indicate if level transition info should be included in Ma request
    uint32_t ulLTRBGId       ;   ///< Target BG of level transition

    int32_t lNIdTextMessage ;   ///< Identity of the message that the driver has confirmed
    int32_t lCountryId      ;   ///< Identity number of the country or region (NID_C)
    int32_t lRBCId          ;   ///< Identity number of the RBC (NID_RBC)

} SRadMsgToSend;


/// Enumeration for the type of received message
typedef enum eRxRadioMsgType
{
    M_UNKNOWN           = 0 ,   ///< Unknown message
    M_MOVA              = 1 ,   ///< Movement Authority
    M_GEN               = 2 ,   ///< General telegram
    M_SR                = 3 ,   ///< Staff responsible authorisation
    M_TR_DAT_ACK        = 4 ,   ///< Acknowledgement of traindata
    M_MA_SHORT_RQST     = 5 ,   ///< Request to stop earlier
    M_EMER_STP          = 6 ,   ///< Emergency stop
    M_R_EMER_STP        = 7 ,   ///< Revocation Emergency stop
    M_SH_NACK           = 8 ,   ///< Shunting refused
    M_SH_ACK            = 9 ,   ///< Shunting authorised
    M_IF_MA             = 10,   ///< Infill MA
    M_CONFIG            = 11,   ///< Configuration determination
    M_ACK_TERM          = 12,   ///< Ack communication termination
    M_TRAIN_ACCEPTED    = 13,   ///< Train accepted
    M_TRAIN_REJECTED    = 14,   ///< Train rejected
    M_SOM_CONFIRMED     = 16,   ///< SoM position report confirmed by RBC
    M_INIT_SESSION      = 17,   ///< Init communication session
    M_ACK_EXIT_TRIP     = 18,   ///< Recognition of exit from trip mode
    M_SHIFTED_MA        = 19,   ///< MA with shifted location reference
    M_ASSIGN_COORDINATE = 20    ///< Assignment of coordinate system

} eRxRadioMsgType;

/// Macro to get the string corresponding to the type of received message
#define STR_RX_RADIO(d) \
    (d)==M_MOVA                 ?   "Movement Authority"                    : \
    (d)==M_TR_DAT_ACK           ?   "Ack of train data"                     : \
    (d)==M_GEN                  ?   "General telegram"                      : \
    (d)==M_SR                   ?   "Staff responsible authorisation"       : \
    (d)==M_MA_SHORT_RQST        ?   "Request to stop earlier"               : \
    (d)==M_EMER_STP             ?   "Emergency stop"                        : \
    (d)==M_R_EMER_STP           ?   "Revocation Emergency stop"             : \
    (d)==M_SH_NACK              ?   "Shunting refused"                      : \
    (d)==M_SH_ACK               ?   "Shunting authorised"                   : \
    (d)==M_IF_MA                ?   "Infill MA"                             : \
    (d)==M_CONFIG               ?   "Configuration determination"           : \
    (d)==M_ACK_TERM             ?   "Ack communication termination"         : \
    (d)==M_TRAIN_ACCEPTED       ?   "Train accepted"                        : \
    (d)==M_TRAIN_REJECTED       ?   "Train rejected"                        : \
    (d)==M_SOM_CONFIRMED        ?   "SoM position report confirmed by RBC"  : \
    (d)==M_INIT_SESSION         ?   "Initiation of communication session"   : \
    (d)==M_ACK_EXIT_TRIP        ?   "Recognition of exit from trip mode"    : \
    (d)==M_SHIFTED_MA           ?   "MA with shifted location reference"    : \
    (d)==M_ASSIGN_COORDINATE    ?   "Assignment of coordinate system"       : \
    "???"


/// Structure containing data about the received radio message
typedef struct SRadioPara
{
    eRxRadioMsgType TypMess         ;   ///< Type of message
    uint32_t ulTelegramId    ;   ///< Identifier of telegram (used for display purpose)
    t_time          dClock          ;   ///< Time stamp of message from RBC
    bool            bRIU            ;   ///< Data come from an RIU
    bool            bAckRequired    ;   ///< Indicate if an acknowledgement is required
    uint32_t ulRqstClock     ;   ///< Time stamp of a request or a reference message ( SH accepted, ack of train data,  ...)
    SBG_Id          IdRefBG         ;   ///< Identity of reference BG (LRBG) + orientation (for Assignment of system coordinate)
    uint32_t ulVersionId     ;   ///< System version used in configuration determination message
    t_distance      dShiftedDistance;   ///< Value of distance used in shifted MA

} SRadioPara;



//=======================    emergency stop    =======================


/// Enumeration for type of emergency stop data
typedef enum eEmerStopType
{
    EMER_STOP_REVOCATION,   ///< Emergency stop revocation
    EMER_STOP_COND      ,   ///< Conditional emergency stop
    EMER_STOP_UNCOND        ///< Unconditional emergency stop

} eEmerStopType;


/// Macro to get string corresponding to an emergency stop type
#define STR_EMERGENCYSTOP_TYPE( d ) \
    (d)==EMER_STOP_REVOCATION   ?   "EMER_STOP_REVOCATION"  : \
    (d)==EMER_STOP_COND         ?   "EMER_STOP_COND"        : \
    (d)==EMER_STOP_UNCOND       ?   "EMER_STOP_UNCOND"      : \
    "???"

/// Structure containing emergency stop information
typedef struct SEmerStop
{
    bool            bValid          ;   ///< Validity of data
    eEmerStopType   EmerStopType    ;   ///< Emergency stop type
    int32_t lMessageId      ;   ///< Identity of the emergency stop message
    eDirection      Direction       ;   ///< Valid direction for conditional emergency stop
    t_distance      dStopDistance   ;   ///< distance to stop for conditional emergency stop
    t_distance      dMinSafeFrontEnd;   ///< Min safe front loc when ES is received

} SEmerStop;



//=======================    received trackside message    =======================


/// Enumeration to identify source of trackside message
typedef enum eMedium
{
    BALISE  ,   ///< Message from balise
    RADIO   ,   ///< Message from RBC or RIU
    LOOP        ///< Message from loop

} eMedium ;


/// Structure to store a received trackside message
typedef struct STksData
{
    SEmerStop               EmergencyStop       ;   ///< emergency stop message
    SMAData                 ShortMA             ;   ///< reduce MA request
    SNationalData           National            ;   ///< national values
    SNationalData           National203         ;   ///< national values from paquet 203
    SLinkingData            Linking             ;   ///< linking
    SMAData                 MA                  ;   ///< Movement authority
    SReposInfo              ReposInfo           ;   ///< repositioning info
    SGradient               Gradient            ;   ///< gradient profile
    STSRDefaultGradient     DefaultGradient     ;   ///< default gradient value for TSR
    SSpeed                  Speed               ;   ///< speed profile
    SCondTraction           CondTraction        ;   ///< track condition: traction
    SLevelTr                LevelTr             ;   ///< Level transition
    SRBCConnectOrder        RBCConnect          ;   ///< radio management
    SShuntArea              SHArea              ;   ///< Shunting Area
    SMARqstPara             MARqstPara          ;   ///< MA request parameters
    SLocReportPara          LocReportPara       ;   ///< Location report parameters
    SSRAuthority            SRAuth              ;   ///< Staff responsible authority
    SSRArea                 SRBGList            ;   ///< list of BG in SR
    STSRList                TSRlist             ;   ///< list of TSR
    STSRrevocationList      TSRrevocationList   ;   ///< list of TSR revocation
    SCondBigMetal           CondMetal           ;   ///< track condition: big metal masses
    STrackCond              TrackCond           ;   ///< track condition
    SAdhesion               Adhesion            ;   ///< adhesion factor
    SGeoPos                 GeoPos              ;   ///< geographical position info
    SModeProf               ModeProf            ;   ///< mode profile
    STextMsgData            FixedTextMess       ;   ///< fixed text message
    STextMsgData            PlainTextMess       ;   ///< plain text message
    SRBCTransition          RBCTransition       ;   ///< RBC transition order
    SEOLMData               EOLMPacket          ;   ///< EOLM packet
    SExitSH                 ExitSH              ;   ///< stop SH on desk opening packet
    SAssignCoor             AssignCoor          ;   ///< assign coordinate system
    STrainRunNbFromRBC      TrainRunNb          ;   ///< train running nb from RBC
    SSRStop                 SRStop              ;   ///< stop in SR
    SSHDanger               SHDanger            ;   ///< danger for shunting
    SRIUArea                RadioIFArea         ;   ///< Radio infill area
    SRefLocAssign           RefLocAssign        ;   ///< assignement of location reference
    SRouteSuitability       RouteSuitability    ;   ///< route suitability data
    SReversingArea          ReversingArea       ;   ///< reversing area
    SReverseSupervision     ReverseSupervision  ;   ///< reverse Supervision
    SAxleLoadSpeedProf      AxleLoadSpeedProf   ;   ///< axle load speed profile
    STrackFreeRqst          TrackFree           ;   ///< track ahead free rqst
    SRadioNetworkRegis      RadioNetworkRegis   ;   ///< Radio network registration
    SConditionalLTR         ConditionalLTROrder ;   ///< conditional level transition order
    STrackAheadFreeInfo     TrackAheadFreeInfo  ;   ///< Track ahead free information
    SExternalData           ExternalData        ;   ///< External data
    SVersionOrder			VersionOrder        ;	///< ETCS system version
    SSRDistInfo				SRDistInfo          ;   ///< Staff responsible distance from loop
    SAllowedConsumption     AllowedConsumption  ;   ///< Track condition: allowed current consumption
    SPermBrakingDist        PermBrakingDist     ;   ///< Permitted braking distance information
    STSRInhibitRevocable    TSRInhibitRevocable ;   ///< Inhibition of revocable balises in L2/L3
    STrackCondStation       TrackCondStation    ;   ///< Informations about station platform
    SSessionRIU             SessionRIU          ;   ///< Session management with radio in-fill unit
    SLXList                 LevelCrossList      ;   ///< Level crossing information list
    SVBaliseCoverOrder      VirtualBalCoverOrder   ;   ///< order for virtual balise cover
    SVBaliseCoverMarker     VBaliseCoverMarker  ;   ///< marker for virtual balise cover
    bool                    bDefaultInformation ;   ///< indicates if trackside data are default information due to a fault of the trackside equipment
    bool                    bInhibBGReaction    ;   ///< Inhibition of balise group message consistency reaction
    bool                    bPacket206Received  ;   ///< Indicated if paquet 68 (track condition) has been received. See SRS 6.6.3.2.3 [9]
    bool                    bPacket207Received  ;   ///< Indicated if paquet 68 (track condition) has been received. See SRS 6.6.3.2.3 [11]
    bool                    bPacket239Received  ;   ///< Indicated if paquet 68 (track condition) has been received. See SRS 6.6.3.2.3 [13]

    int32_t alPacketList[64]    ;   ///< Packet list to get display on GUI
    int32_t lPacketListSize     ;   ///< Size of packet list to get display on GUI

} STksData;


/// Structure to store the basic medium data
typedef struct STksMediumData
{
    t_time          dTimeStamp  ;   ///< Time of reception
    t_distance      dRefPosition;   ///< Reference position
    uint32_t ulVersionId ;   ///< SRS language version id
    eMedium         Medium      ;   ///< Source of data
    SBalisePara     Balise_Para ;   ///< Balise message parameters
    SRadioPara      Radio_Para  ;   ///< Radio message parameters
    int32_t lLoopId     ;   ///< Identity of loop

} STksMediumData;



/// Structure to store trackside message
typedef struct STracksideMessage
{
    STksMediumData          TksMediumData;      ///< Data about reception media
    STksData                TksData      ;      ///< Received data
    bool                    bValid       ;      ///< Indicates if message is valid (used for transition buffer)

} STracksideMessage;



/// Structure used to store decoded message
typedef struct SDecodedTksMsg
{
    STksMediumData         TksMediumData    ;   ///< Data about reception media
    eDirection             ValidDir         ;   ///< Valid direction for the received data

    STksData               TksDataNominal   ;   ///< Data valid for the nominal direction
    STksData               TksDataReverse   ;   ///< Data valid for the reverse direction

} SDecodedTksMsg;

/// Maximum number of set of information to store in transition buffer
/// @SRS 4.8.5.1
#define MAX_TRANSITION_BUFFER_SIZE  3


//=======================    track profile    =======================

/// Structure used to store the track profile (speed, gradient, electric)
typedef struct STrackDesc
{
    t_speed     aMRS            [MAX_LENGTH_METERS] ;   ///< Most Restrictive Speed
    double      aGradient       [MAX_LENGTH_METERS] ;   ///< Gradient value (o/oo)
    int32_t lTrackEnd                           ;   ///< Last valid track point (aMRS[lTrackEnd] and aGradient[lTrackEnd] are still valid)

} STrackDesc;


//=======================    supervision curves    =======================


/// Array for speed curve
typedef t_speed       t_curve[MAX_LENGTH_METERS];

/// Array for distance curve
typedef t_distance    t_curved[MAX_LENGTH_METERS];

/// Structure containing the image of a curve: contains both X and Y points
typedef struct SCurvePoint
{
    t_distance  x   ;   ///< distance coordinate
    t_speed     y   ;   ///< speed coordinate

} SCurvePoint;

/// Type for pointer to deceleration model function
typedef t_accel (*t_decelmodel)(t_speed);

/// Type for pointer to speed model function
typedef t_speed (*t_speedmodel)(t_speed);

/// Structure containing the supervision curves
typedef struct SIntervCurves
{

    t_curve   EBD                             ;   ///< Emergency brake intervention speed
    t_curve   SBD                             ;   ///< Emergency brake intervention speed
    t_curve   GUI                             ;   ///< Emergency brake intervention speed

    t_curve   EBI                             ;   ///< Emergency brake intervention speed
    t_curve   SBI1                            ;   ///< Service brake intervention speed
    t_curve   SBI2                            ;   ///< Service brake intervention speed
    t_curve   FLOI                            ;   ///< Service brake intervention speed

    t_curve   Permitted                       ;   ///< Permitted speed
    t_curve   Indication                      ;   ///< Indication curve
    t_curve   Warning                         ;   ///< Warning speed

    t_distance      dRefLocation                    ;   ///< Reference location for curves

    // this point is unique and located both on ovEBI and SBI curves if SBI exists
    // it is valid only if the associated release speed is not null
    t_distance      dReleaseSpeedAreaStartingPoint  ; ///< Start location of release monitoring

} SIntervCurves;


//=======================    conversion model    =======================

/// Enumeration for brake position
typedef enum eBrakePosition
{
    BRK_POS_R,  ///< R: Passenger Train In P
    BRK_POS_P,  ///< P: freight Train In P
    BRK_POS_G   ///< G: freight Train In G

} eBrakePosition;

/// Macro to get the string corresponding to a brake position
#define STR_BRAKE_POSITION(d) \
    (d)==BRK_POS_R  ?   "Passenger train in P"      : \
    (d)==BRK_POS_P  ?   "Freight train in P"        : \
    (d)==BRK_POS_G  ?   "Freight train in G"        : "???"



//=======================    train data    =======================

/// Enumeration for train data status
typedef enum eDataStatus
{
    DATA_DEFAULT        ,
    DATA_ENTERED        ,
    DATA_TO_VALIDATED   ,
    DATA_SAFE

} eDataStatus ;

/// Enumeration for train data internationale category (cf SRS330)
typedef enum eB3NCTrain
{
    B3NC_TRAIN_NONE       = 0x0000,
    B3NC_TRAIN_FREIGHT_P  = 0x0001,
    B3NC_TRAIN_FREIGHT_G  = 0x0002,
    B3NC_TRAIN_PASSENGER  = 0x0004

} eB3NCTrain;

/// Enumeration for train data internationale category (cf ERA_ERTMS_040001)
typedef enum eB2NCTrain
{
    B2NC_TRAIN_275        = 0x0001,
    B2NC_TRAIN_80         = 0x0002,
    B2NC_TRAIN_100        = 0x0004,
    B2NC_TRAIN_130        = 0x0008,
    B2NC_TRAIN_150        = 0x0010,
    B2NC_TRAIN_165        = 0x0020,
    B2NC_TRAIN_180        = 0x0040,
    B2NC_TRAIN_225        = 0x0080,
    B2NC_TRAIN_300        = 0x0100,
    B2NC_TRAIN_FREIGHT_P  = 0x0200,
    B2NC_TRAIN_FREIGHT_G  = 0x0400,
    B2NC_TRAIN_PASSENGER  = 0x0800,
    B2NC_TRAIN_245        = 0x1000,
    B2NC_TRAIN_210        = 0x2000,
    B2NC_TRAIN_RESERVED   = 0x4000

} eB2NCTrain;

/// Maximum number of point in brake decceleration model
#define MAX_BRK_PT          20

/// Maximum number of traction system available in train
#define MAX_TRACTION_POWER  10

/// structure to store model
typedef struct SModel
{
    t_speed dSpeed          ;   ///< speed
    t_accel dDeceleration   ;   ///< brake decceleration
} SModel;


/// Structure for brake decceleration model
typedef struct SBrkModel
{
    int32_t lNbPt           ;   ///< Number of point in decceleration model
    SModel  aModel[MAX_BRK_PT];

} SBrkModel;


typedef struct SNormalSBParam
{
    t_accel          A_SB01;
    t_accel          A_SB12;
    SBrkModel        SBModel0;
    SBrkModel        SBModel1;
    SBrkModel        SBModel2;

}SNormalSBParam;


typedef struct SBrakeInterfaceParam
{
    bool bAffectEBModel;
    bool bAffectSBModel;

}SBrakeInterfaceParam;


typedef struct SEBModelParam
{
    SBrkModel Model;            ///< Emergency brake model
    SKdry_rst Kdry[NB_KDRY];    ///< Correction factor for emergency brake deceleration. There is 10 possibilities of EBCL.
    SKwet_rst Kwet;             ///< Correction factor for emergency brake deceleration
    t_time    buildUpTime;      ///< EB Build Up Time according to 4 brakes (SRS 3.13.2.2.3.2.4)

    bool    bRegenBrakeUsed;    ///< Parameters for regenerative brake interface
    bool    bEddyBrakeUsed;     ///< Parameters for eddy brake interface
    bool    bMShoeBrakeUsed;    ///< Parameters for magnetic shoe brake interface
    bool    bEpBrakeUsed;       ///< Parameters for electro pneumatic brake interface

}SEBModelParam;

typedef struct SSBModelParam
{
    SBrkModel Model;            ///< Emergency brake model
    t_time    buildUpTime;      ///< SB Build Up Time according to 3 brakes (SRS 3.13.2.2.3.2.4)

    bool    bRegenBrakeUsed;    ///< Parameters for regenerative brake interface
    bool    bEddyBrakeUsed;     ///< Parameters for eddy brake interface
    bool    bEpBrakeUsed;       ///< Parameters for electro pneumatic brake interface

}SSBModelParam;

typedef struct SBrakesEquivalentTimeParams
{
    bool bValid;
    double dA, dB, dC;
    double dKto;
} SBrakesEquivalentTimeParams;

/// Enumeration for loading gauge
/// @SRS 7.5.1.68
typedef enum eLoadingGauge
{
    LOADING_GAUGE_UNAVAILABLE = 0,
    LOADING_GAUGE_G1 = 1,
    LOADING_GAUGE_GA = 2,
    LOADING_GAUGE_GB = 3,
    LOADING_GAUGE_GC = 4

} eLoadingGauge;

#define STR_LOADING_GAUGE(d) \
    (d)==LOADING_GAUGE_UNAVAILABLE   ?   "Unavailable"     : \
    (d)==LOADING_GAUGE_G1            ?   "G1"    : \
    (d)==LOADING_GAUGE_GA            ?   "GA"    : \
    (d)==LOADING_GAUGE_GB            ?   "GB"    : \
    (d)==LOADING_GAUGE_GC            ?   "GC"    : "???"



/// Enumeration for train cant deficiency category
/// @SRS 7.5.1.82.2
typedef enum eCantDeficiencyCategory
{
    CANT_DEFICIENCY_80  = 0,
    CANT_DEFICIENCY_100 = 1,
    CANT_DEFICIENCY_130 = 2,
    CANT_DEFICIENCY_150 = 3,
    CANT_DEFICIENCY_165 = 4,
    CANT_DEFICIENCY_180 = 5,
    CANT_DEFICIENCY_210 = 6,
    CANT_DEFICIENCY_225 = 7,
    CANT_DEFICIENCY_245 = 8,
    CANT_DEFICIENCY_275 = 9,
    CANT_DEFICIENCY_300 = 10

} eCantDeficiencyCategory;

#define STR_CANT_DEFICIENCY_CATEGORY(d) \
    (d)==CANT_DEFICIENCY_80  ? "80mm"  : \
    (d)==CANT_DEFICIENCY_100 ? "100mm" : \
    (d)==CANT_DEFICIENCY_130 ? "130mm" : \
    (d)==CANT_DEFICIENCY_150 ? "150mm" : \
    (d)==CANT_DEFICIENCY_165 ? "165mm" : \
    (d)==CANT_DEFICIENCY_180 ? "180mm" : \
    (d)==CANT_DEFICIENCY_210 ? "210mm" : \
    (d)==CANT_DEFICIENCY_225 ? "225mm" : \
    (d)==CANT_DEFICIENCY_245 ? "245mm" : \
    (d)==CANT_DEFICIENCY_275 ? "275mm" : \
    (d)==CANT_DEFICIENCY_300 ? "300mm" : "???"

/// Conversion of cant deficiency category to distance
#define CANT_CAT_TO_DIST(d) \
    (d)==CANT_DEFICIENCY_80    ?  0.080    : \
    (d)==CANT_DEFICIENCY_100   ?  0.100    : \
    (d)==CANT_DEFICIENCY_130   ?  0.130    : \
    (d)==CANT_DEFICIENCY_150   ?  0.150    : \
    (d)==CANT_DEFICIENCY_165   ?  0.165    : \
    (d)==CANT_DEFICIENCY_180   ?  0.180    : \
    (d)==CANT_DEFICIENCY_210   ?  0.210    : \
    (d)==CANT_DEFICIENCY_225   ?  0.225    : \
    (d)==CANT_DEFICIENCY_245   ?  0.245    : \
    (d)==CANT_DEFICIENCY_275   ?  0.275    : \
    (d)==CANT_DEFICIENCY_300   ?  0.300    : 0

/// Conversion of cant deficiency distance to category
#define CANT_DIST_TO_CAT(d) \
    (d)<=0.080 ?   CANT_DEFICIENCY_80   : \
    (d)<=0.100 ?   CANT_DEFICIENCY_100  : \
    (d)<=0.130 ?   CANT_DEFICIENCY_130  : \
    (d)<=0.150 ?   CANT_DEFICIENCY_150  : \
    (d)<=0.165 ?   CANT_DEFICIENCY_165  : \
    (d)<=0.180 ?   CANT_DEFICIENCY_180  : \
    (d)<=0.210 ?   CANT_DEFICIENCY_210  : \
    (d)<=0.225 ?   CANT_DEFICIENCY_225  : \
    (d)<=0.245 ?   CANT_DEFICIENCY_245  : \
    (d)<=0.275 ?   CANT_DEFICIENCY_275  : CANT_DEFICIENCY_300

/// Structure containing train data exchanged with DMI
typedef struct SETCSTrainData
{
    eDataStatus             State                       ;   ///< Train data status

    t_distance              dLength                     ;   ///< length of train
    t_speed                 dSpeedLimit                 ;   ///< technical speed limit of train
    int32_t lDMITrainCategory           ;   ///< DMI train categories (combination of Brake position and cant defficiency)
    eLoadingGauge           loadingGauge                ;   ///< train loading gauge
    eAxleLoadCategory       axleLoadCategory            ;   ///< train axle load category
    bool                    bAirtighAvailable           ;   ///< indicate if the airtight control is available
    int32_t lBrakePercentage            ;   ///< Brake percentage

} SETCSTrainData;

/// Structure containing external train
typedef struct SExternalTrainData
{
    SETCSTrainData          TrainData;
    int32_t lAxleNumber;                        ///< Axle number of the engine
    eCantDeficiencyCategory cantDeficiencyTrainCategory ;       ///< train cant deficiency category
    int32_t lOtherSpecificTrainCategory;        ///< Brake position
    // Tractions
    int32_t lNbTractionPower;                   ///< Number of power supply types accepted by the train
    STractionParam          TractionData[MAX_TRACTION_POWER];   ///< List of power supply types accepted by the train
    // Various
    int32_t OperationalNb;
    double                  dTractionCutOffTime;
    bool                    bTractionCutOffInterface;
    // int32_t lBrakePercentage;
    // Brake interface
    SBrakeInterfaceParam    RegenerativeBrake;
    SBrakeInterfaceParam    EddyCurrentBrake;
    SBrakeInterfaceParam    MagneticShoeBrake;
    SBrakeInterfaceParam    EpBrakeInterface;
    bool                    bServiceBrakeInterface;
    bool                    bServiceBrakeFeedback;

} SExternalTrainData;

/// Structure containing the train data
typedef struct STrainCharact
{
    SETCSTrainData  ETCSTrainData                       ;   ///< ETCS train data (exchanged with DMI)

    eCantDeficiencyCategory cantDeficiencyTrainCategory ;   ///< train cant deficiency category
    int32_t lOtherSpecificTrainCategory ;   ///< Brake position



    int32_t lETCSId                             ;   ///< Train ETCS identity

    t_distance      dEngineLength                       ;   ///< Length of the engine (For SL/NL, see SRS 4.4.15.1.3 - 4.4.6.1.4)
    t_distance      dBalAntennaOffset                   ;   ///< Distance between balise antenna and train front end cab A or B
    t_distance      dCabABalAntennaOffset               ;   ///< Distance between balise antenna and train front end cab A

    t_time          dCutOffTime                         ;   ///< Traction cut off response time

    ///< @SRS 3.13.2.2.3.1.9
    ///< @SRS 3.13.2.2.3.1.10
    SNormalSBParam           NormSBModelParam_G         ;   ///< Normal service brake model parameters for brake position in G
    SNormalSBParam           NormSBModelParam_P         ;   ///< Normal service brake model parameters for brake position in P
    ///< @SRS  3.13.2.2.6
    SBrakeInterfaceParam    RegenBrakeInterface         ;   ///< Parameters for regenerative brake interface
    SBrakeInterfaceParam    EddyBrakeInterface          ;   ///< Parameters for eddy brake interface
    SBrakeInterfaceParam    MShoeBrakeInterface         ;   ///< Parameters for magnetic shoe brake interface
    SBrakeInterfaceParam    EpBrakeInterface            ;   ///< Parameters for Ep shoe brake interface
    SBrakeInterfaceParam    AdditionalBrakeInterface    ;   ///< Parameters for additional brake interface
    SEBModelParam           aEBModelParam[NB_EB_MODELS] ;   ///< EB model parameters. There is 12 combinations of use of regenerative brake, eddy current brake and magnetic shoe brake.(3.13.2.2.3.1)
    SSBModelParam           aSBModelParam[NB_SB_MODELS] ;   ///< SB models. There is 4 combinations of use of regenerative brake, eddy current brake and magnetic shoe brake.(3.13.2.2.3.1)
    t_time                  dTbs1                       ;   ///< SB build up time
    t_time                  dTbs2                       ;   ///< EB build up time
    // Conversion model
    SEBModelParam           ConvEBModelParam            ;   ///< EB model parameters for conversion model
    SSBModelParam           ConvSBModelParam            ;   ///< SB model parameters for conversion model
    // Model for current brakes
    SEBModelParam *         CurrentEBModelParam         ;   ///< Current EB models param
    SSBModelParam *         CurrentSBModelParam         ;   ///< Current EB models param

    // Services brakes equivalent times
    SBrakesEquivalentTimeParams SBEquivalentTimeParams  ;   ///< SB equivalent times A3.9.6

    int32_t lAxleNumber                         ;   ///< Axle number of the engine

    int32_t lNbTractionPower                    ;   ///< Number of power supply types accepted by the train
    STractionParam  TractionData[MAX_TRACTION_POWER]    ;   ///< List of power supply types accepted by the train

    /// Index of power supply type currently used by the train (updated when asked by packet 38)
    // -> it is going to be 0 at the beginning, i.e. first entry of traction power table above
    // Note: this variable is significant only if train supports several traction power types
    int32_t lCurrentTractionIndex               ;
    bool            bConversionModelUsed                ;   ///< indicate if the conversion model is used

} STrainCharact;



//=======================    radio data    =======================

/// Default radio network identifier
#define DEFAULT_RAD_NET_ID      "\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF"
#define DEFAULT_RAD_NET_NAME    "Default Network"

/// Special value for unknown RBC ID
#define UNKNOWN_RBC_ID          16383

/// Number of radio equipments
#define MAX_RADIO_EQUIPMENT     2

/// Enumeration for radio link status
typedef enum eRadioLinkState
{
    STATE_START_SESSION_ESTABLISHMENT,  // No session, no connection
    STATE_WAIT_FOR_SAFE_CONNECTION,
    STATE_WAIT_FOR_SYSTEM_VERSION,
    STATE_TERMINATE_SESSION,
    STATE_WAIT_FOR_TERMINATE_ACK,
    STATE_WAIT_FOR_SAFE_CONNECTION_RELEASE,
    STATE_SESSION_ESTABLISHED,
    STATE_WAIT_FOR_RBC_INIT_SESSION_MSG,
    STATE_RESETTING_SAFE_CONNECTION_MAINTAINING_SESSION

} eRadioLinkState;

/// Macro to get string for radio link state
#define STR_RADIOLINKSTATE(r) \
    (r)==STATE_START_SESSION_ESTABLISHMENT                      ?   "STATE_START_SESSION_ESTABLISHMENT"                     : \
    (r)==STATE_WAIT_FOR_SAFE_CONNECTION                         ?   "STATE_WAIT_FOR_SAFE_CONNECTION"                        : \
    (r)==STATE_WAIT_FOR_SYSTEM_VERSION                          ?   "STATE_WAIT_FOR_SYSTEM_VERSION"                         : \
    (r)==STATE_TERMINATE_SESSION                                ?   "STATE_TERMINATE_SESSION"                               : \
    (r)==STATE_WAIT_FOR_TERMINATE_ACK                           ?   "STATE_WAIT_FOR_TERMINATE_ACK"                          : \
    (r)==STATE_WAIT_FOR_SAFE_CONNECTION_RELEASE                 ?   "STATE_WAIT_FOR_SAFE_CONNECTION_RELEASE"                : \
    (r)==STATE_SESSION_ESTABLISHED                              ?   "STATE_SESSION_ESTABLISHED"                             : \
    (r)==STATE_WAIT_FOR_RBC_INIT_SESSION_MSG                    ?   "STATE_WAIT_FOR_RBC_INIT_SESSION_MSG"                   : \
    (r)==STATE_RESETTING_SAFE_CONNECTION_MAINTAINING_SESSION    ?   "STATE_RESETTING_SAFE_CONNECTION_MAINTAINING_SESSION"   : "???"

/// Enumeration for radio safe connection status
typedef enum eRadSafeConnectStatus
{
    CONNECTION_RELEASED,                ///< Radio safe connection is released
    CONNECTION_REQUESTED,               ///< Radio safe connection is requested
    CONNECTION_ESTABLISHED              ///< Radio safe connection is established

} eRadSafeConnectStatus;

#define STR_SAFE_CONN_STATUS(d)    \
    (d)==CONNECTION_RELEASED    ? "CONNECTION_RELEASED"     : \
    (d)==CONNECTION_REQUESTED   ? "CONNECTION_REQUESTED"    : \
    (d)==CONNECTION_ESTABLISHED ? "CONNECTION_ESTABLISHED"  : \
    "???"

/// enumeration for the state of the safe radio connection indication
/// @SRS 3.5.7.1
typedef enum eRadSafeConnectionInd
{
    CONNECTION_NONE,
    CONNECTION_FAILED,
    CONNECTION_UP

} eRadSafeConnectionInd;

#define STR_RAD_CONN_IND(d)    \
    (d)==CONNECTION_NONE    ? "CONNECTION_NONE"     : \
    (d)==CONNECTION_FAILED  ? "CONNECTION_FAILED"   : \
    (d)==CONNECTION_UP      ? "CONNECTION_UP"       : \
    "???"

/// Enumeration for status of radio communication session
typedef enum eRadSessionState
{
    SESSION_RELEASED,       ///< Session is released
    SESSION_INIT,           ///< Try to establish a session
    SESSION_ESTABLISHED,    ///< Session is established
    SESSION_TERMINATED      ///< Session is terminated (wait for ack termination)

} eRadSessionState;

/// Enumeration for status of the radio network registration
typedef enum eRadNetRegState
{
    RADNET_NOT_REGISTERED,          ///< Not registered to radio network
    RADNET_REGISTERING,             ///< Registering to radio network
    RADNET_REGISTERED,              ///< Registered to radio network
    RADNET_FAILED_REGISTER          ///< Registration  to radio network has failed

} eRadNetRegState;

/// Enumeration for status of the radio network registration error report
typedef enum eRadNetRegReportError
{
    RADNET_NO_ERROR,          ///< No error
    RADNET_ERROR,             ///< Error has to be reported
} eRadNetRegReportError;

/// Macro to get the string corresponding to the radio network registration status
#define STR_RAD_NET_STATE(d)    \
    (d)==RADNET_NOT_REGISTERED  ? "NOT REGISTERED"  : \
    (d)==RADNET_REGISTERING     ? "REGISTERING"     : \
    (d)==RADNET_REGISTERED      ? "REGISTERED"      : \
    "???"

typedef struct SRadioNetwork
{
    uint8_t   uszId[RADIO_NETWORK_ID_SIZE];           ///< Radio network identifier
    char    uszName [RADIO_NETWORK_NAME_SIZE_MAX +1];       ///< Radio network name (+1 for '\0' char)

}SRadioNetwork;

/// Structure containing data for radio communication related to a radio equipment
typedef struct SRadioEquipmentData
{
    uint8_t                       uszETCSPhoneNb[PHONE_NB_SIZE]           ;   ///< Phone Number of first  radio equipment (if equipped)

    eRadNetRegState             RadioNetworkRegistrationStatus          ;   ///< State of the radio network registration
    uint8_t                       RadioNetwork[RADIO_NETWORK_ID_SIZE]     ;   ///< Radio network identifier of radio equipment (if equipped)
    uint32_t lNetworkRegistrationAttempts            ;   ///< Radio network registration attemps counter
    eRadSafeConnectionInd       RadSafeConnectionInd                    ;   ///< State of the safe radio connection indication (SRS 3.5.7)
    bool                        bConnectionTimerStarted                 ;   ///< indicate that the connection status timer has been started
    t_time                      dConnectionStartTime                    ;   ///< time at which the connection status timer has been started
    t_time                      dConFailIndTime                         ;   ///< time at which the connection fail indication starts
    eRadSafeConnectStatus       SafeConnectionStatus                    ;   ///< State of radio safe connection
    eRadSessionState            CommunicationSessionStatus              ;   ///< State of radio communication session
    t_time                      dSafeConnectionRequestTime              ;   ///< Time spent in the safe connection request (used to timeout)
    bool                        bRIU                                    ;   ///< Indicates if connected to a radio infill unit or a radio block centre
    int32_t lRBCCountry                             ;   ///< Country identifier of connected radio block centre or radio infill unit
    int32_t lRBCIdentity                            ;   ///< Identifier of connected radio block centre or radio infill unit
    uint8_t                       uszRBCPhoneNb[PHONE_NB_SIZE]            ;   ///< Phone number of connected radio block centre or radio infill unit
    uint32_t uConnectionId                           ;   ///< Connection identifier (SaCEPID)
    bool                        bConnectNbLimited                       ;   ///< Indicate if the connection attempts are limited
    int32_t lConnectionAttemptNb                    ;   ///< Number of connection attempt
    bool                        bUseEirenePhoneNumber                   ;   ///< Indicate if we try to contact a RBC using an EIRENE phone number

    uint32_t ulVersionId                             ;   ///< SRS language version (used in communication with RBC)

    bool                        bAcceptUnknownBGFromRBC                 ;   ///< Indicates if radio message with unknown NID_LRBG are accepted
    bool                        bDisconnectionConfirmed                 ;   ///< Indicates if disconnection has been confirmed
    eRadioLinkState             RadioLinkState                          ;   ///< State of radio link
    eRadioLinkState             LastRadioLinkState                      ;   ///< Last recorded state of radio link
    t_time                      dRadioLinkStateChangeTime               ;   ///< Time of last radio link status change
    t_time                      dLastReceivedMsgTimeStamp               ;   ///< Time stamp of the last received message (used for radio link supervision)
    t_time                      dSafeConnectionTime                     ;   ///< Time of the last safe connection (used for radio link supervision)
    t_time                      dLastEOMSendingTime                     ;   ///< Time of last attempt send End of Mission message
    int32_t lNbEOMSendingAttempt                    ;   ///< Number of attempts sending End of Mission message

    bool                        bTrainDataSent                          ;   ///< Indication that train data have been sent
    bool                        bTrainDataAck                           ;   ///< Indication of ack of train data by RBC
    bool                        bWaitRelease                            ;   ///< Wait equipment release
} SRadioEquipmentData;

/// Structure containing data for radio communication
typedef struct SRadioData
{
    SRadioEquipmentData aRadioEquipmentData[2]                      ;   ///< Radio equipment data
    int32_t lPrimaryRadioEquipmentIndex                 ;   ///< Index of the primary radio equipment (session with supervising RBC)
    int32_t lSecondaryRadioEquipmentIndex               ;   ///< Index of the secondary radio equipment (session with second RBC)

    SRadioNetwork       aNetworkList[RADIO_NETWORK_MAX_LIST_SIZE]   ;   ///< List of permitted radio network
    int32_t lNbNetworkAvailable                         ;   ///< Number of permitted network
    uint8_t               DefaultNetwork[RADIO_NETWORK_ID_SIZE]       ;   ///< Default and current radio network id to be used (set by driver or received from trackside)

    eValidity           RBCDataStatus                               ;   ///< Status of RBC data (RBC ID and phone number)

    bool                bRBCTrMAData                                ;   ///< Indicates if MA and track description received from MA are stored onboard (RBC transition)

    uint32_t auLastSHRqstTimeStamp[2]                    ;   ///< Time stamp of the last 2 sent SH request msg
    uint32_t auLastTrDataTimeStamp[2]                    ;   ///< Time stamp of the last 2 sent train data msg
    uint32_t uLastShortMATimeStamp                       ;   ///< Time stamp of the last received short MA msg

} SRadioData;

//=======================    fixed data    =======================


/// Structure containing the fixed data (SRS A3.1)
typedef struct SFixedData
{
    int32_t lRadioMsgRepeatNb               ;   ///< Number of repetition of radio msg
    t_time      dRadioMsgRepeatTimeOut          ;   ///< Time out for repetition (in seconds)
    // this value is independent of lRadioMsgRepeatNb and also defined in SRS
    t_time      dSafeConnectionRepeatTimeout    ;   ///< Time between each safe connection try
    int32_t lSafeConnRepeatNb               ;   ///< Number of tries to setup safe connection
    t_time      dMaxSessionMaintainTime         ;   ///< Maximum time to maintain a communication session in case of failed re-connection attempts (in seconds)
    t_time      dDelayBeforeDisc                ;   ///< Additional delay time to disconnection on supervision of safe radio connection
    t_time      dConnectionStatusTimeOut        ;   ///< “Connection status” timer for safe radio connection indication

    t_speed     dDV_EBI_MIN                     ;   ///< Minimum value of DV_EBI
    t_speed     dDV_EBI_MAX                     ;   ///< Maximum value of DV_EBI
    t_speed     dV_EBI_MIN                      ;   ///< Value of the MRSP below which DV_EBI_MIN is valid
    t_speed     dV_EBI_MAX                      ;   ///< Value of the MRSP above which DV_EBI_MAX is valid

    t_speed     dDV_SBI_MIN                     ;   ///< Minimum value of DV_SBI
    t_speed     dDV_SBI_MAX                     ;   ///< Maximum value of DV_SBI
    t_speed     dV_SBI_MIN                      ;   ///< Value of the MRSP below which DV_SBI_MIN is valid
    t_speed     dV_SBI_MAX                      ;   ///< Value of the MRSP above which DV_SBI_MAX is valid

    t_speed     dDV_WARN_MIN                    ;   ///< Minimum value of DV_WARNING
    t_speed     dDV_WARN_MAX                    ;   ///< Maximum value of DV_WARNING
    t_speed     dV_WARN_MIN                     ;   ///< Value of the MRSP below which DV_WARNING_MIN is valid
    t_speed     dV_WARN_MAX                     ;   ///< Value of the MRSP above which DV_WARNING_MAX is valid

    t_time      dT_DRIVER                       ;   ///< driver reaction time

    double      dRotatingMassMin                ;   ///< Minimum rotating mass of the train as a percentage of the total train weight
    double      dRotatingMassMax                ;   ///< Maximum rotating mass of the train as a percentage of the total train weight
    t_speed     dScirsob                        ;   ///< Assumed value of speed difference between V_estimated and V_safe (V_scirsob)

    t_time      dAckModeTrTime                  ;   ///< Time to ack mode transition
    t_time      dCyclicMARqst                   ;   ///< MA request repetition cycle, default value for variable T_CYCRQST
    t_time      dT_WARNING                      ;   ///< Warning time before SBI
    t_time      dPreIndicationTime              ;   ///< Time for indication point

    t_distance  dD_Metal                        ;   ///< Distance of metal immunity in Level 0

    t_distance  dStorageDistance                ;   ///< Distance to keep onboard information in rear of the min safe rear end of the train

    t_time      dSB2EBTimeout                   ;   ///< After SB request, timeout before requesting EB if SB not applicated (default: 1s)(not defined in A3.1)


    t_distance  dDMIOrderDist                   ;   ///< announcement distance before reach order (not defined in A3.1)
    t_time      dOrderAppTime                   ;   ///< Time for the driver to apply the order (s) (not defined in A3.1)
    t_time      dHornAppTime                    ;   ///< Time for the driver to apply the horn  (s)
    t_time      dDMIOrderExitIndTime            ;   ///< Time between minimum safe rear end of the train leaving a track condition area and on-board deleting the applicable indication (s)

    uint8_t uszEirenePhoneNb[PHONE_NB_SIZE]       ;   ///< EIRENE phone number (SRS300 5.4.4 S3)


} SFixedData;


//=======================    Adhesion data    =======================

/// Adhesion percentage value for slippery rail
#define ADHES_SLIPPERY_RAIL         70
/// Adhesion percentage value for non slippery rail
#define ADHES_NONSLIPPERY_RAIL      100

/// Structure containing adhesion data
typedef struct SAdhesionData
{
    int32_t lAdhesionFactor         ;   ///< Current adhesion factor (in %)
    int32_t lDriverValue            ;   ///< Adhesion factor from driver (in %)
    int32_t lTrackValue               ;   ///< Adhesion factor from trackside (in %)

} SAdhesionData;

//=======================    Corrections factors    =======================

/// Structure containing correction factors
typedef struct SCorrectionFactors
{
    SKn     Kn_p  ;     ///< Correction factor for positive gradient
    SKn     Kn_n  ;     ///< Correction factor for negative gradient

} SCorrectionFactors;


//=======================    CSV recording parameters    =======================

/// Maximum size of file name
#define MAX_FILENAME_SIZE   128

///Structure containing parameters for CSV recording
typedef struct SCSVConfig
{
    t_time      dDeltaTime                  ;   ///< Delta time for recording data to csv file
    t_distance  dDeltaDistance              ;   ///< Delta distance for recording data to csv file
    t_speed     dDeltaSpeed                 ;   ///< Delta speed for recording data to csv file

    char    cDecimalSymbol                  ;   ///< Decimal symbol used in csv file (for international compatibility)
    char    cListSeparator                  ;   ///< List separator used in csv file (for international compatibility)
    char    szFileName[MAX_FILENAME_SIZE]   ;   ///< Name of the csv file to record data to

} SCSVConfig;

//======================= Train running number =======================

/// Structure to store train running number info
typedef struct SOperationnalNb
{
    uint32_t ulValue;        ///< Value of the train running number
    eValidity       Validity;       ///< Indicate if the number is valid/invalid/unknown
    bool            bSent;          ///< Indicate if there is a newer valid train running number which need to be sent

}SOperationnalNb;


/// structure to store configuration of odometry error
typedef struct SOdoCfg
{
    // model for odometric error: A.x + b.rand()
    // error on estimated loc: err = linear error * D_LRBG /1000
    // Uerr (Under reading error) = fixed error + underreading error coeff * Err
    // Oerr (Over reading error)  = fixed error + overerreading error coeff * Err

    t_distance      dOdoLinErrcoeff                     ;   ///< Linear error for 1000m
    t_distance      dOdoOverFixedError                  ;   ///< Fixed error for over-reading
    t_distance      dOdoUnderFixedError                 ;   ///< Fixed error for under-reading
    double          dOdoUnderErrCoeff                   ;   ///< Under reading error coefficient in %
    double          dOdoOverErrCoeff                    ;   ///< Over reading error coefficient in %
    double          dOdoFixedError                      ;   ///< Reading error coefficient in %

} SOdoCfg;


/// Information about the train equipement
typedef struct STrainEquipement
{
    bool    bBaliseComAvailable             ;   ///< Indicates if train is equipped for balise communication
    bool    bLoopComAvailable               ;   ///< Indicates if train is equipped for loop communication
    int32_t lNbRadioComAvailable            ;   ///< Number of radio equipments equipped
    bool    bIntegrityDeviceAvailable       ;   ///< Indicates if train is equipped with an integrity device
    bool    bServiceBrakeAvailable          ;   ///< Indicates if train is equipped with service brake
    bool    bBrakeFeedBackAvailable         ;   ///< Indicates if train has brake feedback information
    bool    bTCOAvailable                   ;   ///< Indicates if train is equipped with traction cut off
    bool    bDMIAvailable                   ;   ///< Indicates if DMI is connected and ready
    bool    bColdMvtDetectorAvailable       ;   ///< Indicates if train is equipped with cold mouvement detector

    bool    bSwitchOffRegenBrkAuto          ;   ///< indicate if switch off of regenarative brake is automatic (or driver action required)
    bool    bSwitchOffEddyCBrkAuto          ;   ///< indicate if switch off of eddy current brake is automatic (or driver action required)
    bool    bSwitchOffMagneticBrkAuto       ;   ///< indicate if switch off of magnetic shoe brake is automatic (or driver action required)
    bool    bAirTightCtrlAuto               ;   ///< indicate if control of air tightness is automatic (or driver action required)
    bool    bPantoRaiseAuto                 ;   ///< indicate if raising control of pantograph is automatic (or driver action required)
    bool    bPantoLowAuto                   ;   ///< indicate if lowering control of pantograph is automatic (or driver action required)
    bool    bCircuitBreakerCtrlAuto         ;   ///< indicate if control of circuit breaker is automatic (or driver action required)
    bool    bSwitchTractionSystemAuto       ;   ///< indicate if switch of traction system is auto (or driver action required)

}STrainEquipement;


//=======================    configuration data    =======================

/// Size of driver identifier SUBSET-27 4.2.3.7
#define DRIVER_ID_SIZE  10

/// Maximum number of message to be acknowledged
#define MAX_MESSAGE_ACK 10

/// Structure containing a message to be acknowledged
typedef struct SMessageAck
{
    bool            bValid      ;   ///< indicates validity of message acknowledgement
    int32_t lMessageID  ;   ///< Message identifier
    int32_t lTxtCode    ;   ///< Text code of the message (see Q_TEXT)
    eConfirmation   AckType     ;   ///< Message acknowledgement type

} SMessageAck;

/// structure for NTC isolation information
typedef struct SNtcIsolation
{
    int32_t lNtcId;
    bool bIsolated;

} SNtcIsolation;

/// Structure containing onboard configuration
typedef struct SConfig
{
    t_time              dRefTime_s                      ;   ///< Offset for absolute time

    t_time              dTimeOfDayOffset                ;   ///< Offset to get time of day
    char                szName         [150]            ;   ///< EVC name
    char                szVersion      [50]             ;   ///< EVC version
    char                szCompatibility[100]            ;   ///< EVC compatibility (SRS)

    STrainEquipement    TrainEquipement                 ;   ///< Train equipment level

    bool                bUseNomRotMassFactor            ;   ///< indicate if nominal rotating mass factor is available for use
    double              dNomRotMassFactor               ;   ///< nominal rotating mass factor

    SRadioData          RadioData                       ;   ///< Data about radio communication

    eOperatedSystemVersion  OperatedSystemVersion       ;   ///< current Operated System Version
    eOperatedSystemVersion  L1TksSystemVersion          ;   ///< L1 trackside (balise/loop) system version
    bool                    bL1TksSystemVersionUsed     ;   ///< Indicate that L1 trackside (balise/loop) system version has been used

    char                szDriver_Id[DRIVER_ID_SIZE]     ;   ///< Driver identifier (SUBSET-27: 4.2.3.7)

    SOperationnalNb     OperationalNb                   ;   ///< Train running number infos

    SNationalData       National                        ;   ///< Current national data

    SFixedData          FixedData                       ;   ///< Fixed data

    STrainCharact       TrainData                       ;   ///< Train data

    SCorrectionFactors  CorrectionFactors               ;   ///< Correction factors

    bool                bMA_Rqst_TimeOut_Relevant       ;   ///< Indicates if MA request timeOut has to be taken into account
    t_time              dMA_Rqst_TimeOut                ;   ///< must request MA before timeout of MA timer

    SAdhesionData       AdhesionData                    ;   ///< Adhsesion data (from Tks, driver, ...)

    bool                bBTMFailure                     ;   ///< indicates if a BTM failure occured

    bool                bReversingAvailable             ;   ///< Indicates if reversing is available

    uint32_t ulDMIActiveOption               ;   ///< Current active option on DMI
    uint32_t ulDMIMsgConfirm                 ;   ///< Current status of acknowledgement for text messages (availability of ACK / NACK)
    SMessageAck         DMIMsgAckList[MAX_MESSAGE_ACK]  ;   ///< List of messages to be acknowledged

    SOdoCfg             OdoCfg                          ;   ///< Configuration of odometry error

    bool                bInactiveDmiEvcMenuRequest      ;   ///< Indicates if Tbs1 has to be forced to 0

    SCSVConfig          CSVRecordingParameters          ;   ///< CSV recording parameters

    SVBaliseCoverOrder  BaliseCoverMarker[MAX_VBC_ORDERS];  ///< Order for balise cover
    int32_t lNbCoverMarker                  ;   /// Just indicate number of VBC. Do not use it for loop into VBC data!

} SConfig;

//=======================    location data    =======================

/// Structure containing location data
typedef struct SLocationData
{
    t_distance      dEstimatedBalAntennaLoc ;   ///< Odometer Distance in m (antenna loc)
    t_distance      dOdoRelativLoc          ;   ///< Position Principal (front) End of the Train in m without taking movement error in account
    t_distance      dEstimatedFrontLoc      ;   ///< Position Principal (front) End of the Train in m
    t_distance      dEstimatedRearLoc       ;   ///< Position Secondary (rear) End of the Train in m
    t_distance      dLRBGDistance           ;   ///< Distance between front end and ref balise in m
    t_distance      dLRBGTravelledDistance  ;   ///< Distance travelled by the train since we encounter the LRBG

    t_distance      dMinSafeRearEnd         ;   ///< Train's rear end is for sure *after* this location
    t_distance      dMaxSafeFrontEnd        ;   ///< Train's front end is for sure *before* this location
    t_distance      dMinSafeFrontEnd        ;   ///< Train's front end is for sure *after* this location
    t_distance      dMaxSafeRearEnd         ;   ///< Train's rear end is for sure *before* this location

    eDirection      Train_direction         ;   ///< Train direction or standstill indication

    t_speed         dOdoSpeed               ;   ///< Odometer Speed
    t_accel         dOdoGamma               ;   ///< Odometer Gamma

    t_distance      dLocOverErr             ;   ///< Upper bound width of the confidence interval of the odometer distance
    t_distance      dLocUnderErr            ;   ///< Lower bound width of the confidence interval of the odometer distance
    t_speed         dSpdOverErr             ;   ///< Upper bound width of the confidence interval of the odometer speed
    t_speed         dSpdUnderErr            ;   ///< Lower bound width of the confidence interval of the odometer speed

    t_distance      dRefLocation            ;   ///< Received location without correction (due to active cabin, …)

    t_distance      dLocationOffset         ;   ///< Offset for location processing according to active cab

    // Data for calculation of geographical position
    bool            bKPDataValid            ;   ///< Indicates if track kilometre reference data is applicable
    int32_t lKP_Dir                 ;   ///< Direction -1 or +1 (0 for unknown)
    t_distance      dKP_RefBalise           ;   ///< Position KP reference balise
    t_distance      dKP_RefBaliseLocation   ;   ///< Position of reference balise (train referential)
    int32_t lKp                     ;   ///< Value of Kp

    t_time          dCurrentTime            ;   ///< Current time from start

}   SLocationData;


//=======================    TIU data    =======================

/// Enumeration for status of active cabin
typedef enum eActiveCabin
{
    NO_CABIN,       ///< No active cabin
    CABIN_A,        ///< Cabin A activated
    CABIN_B,        ///< Cabin B activated
    CABIN_A_B       ///< Cabin A and B activated

} eActiveCabin;

#define STR_CABIN(d)              \
    (d)==CABIN_A    ? "CABIN_A"     : \
    (d)==CABIN_B    ? "CABIN_B"     : \
    (d)==NO_CABIN   ? "NO_CABIN"    : \
    (d)==CABIN_A_B  ? "CABIN_A_B"   : "???"

typedef enum eTrainDataType
{
    TRAIN_DATA_FLEXIBLE,
    TRAIN_DATA_FIXED,
    TRAIN_DATA_SWITCHABLE

}eTrainDataType;


#define STR_TRAIN_DATA_TYPE(d)                      \
    (d)==TRAIN_DATA_FLEXIBLE    ?   "Flexible"      :   \
    (d)==TRAIN_DATA_FIXED       ?   "Fixed"         :   \
    (d)==TRAIN_DATA_SWITCHABLE  ?   "Switchable"    : "UNKNOWN TRAIN DATA TYPE"


/// Structure containing TIU Input/Output
typedef struct STiuData
{
    // OUTPUTS
    bool               bEBApp                  ;   ///< Indicates if EB is requested
    bool               bSBApp                  ;   ///< Indicates if SB is requested
    bool               bCutOffApp              ;   ///< Indicates if cut off traction is requested
    bool               bOpenCircuitBreaker     ;   ///< Indicates if main circuit breaker opening is requested
    bool               bPantographLow          ;   ///< Indicates if pantograph lowering is requested
    bool               bAirTightRqst           ;   ///< Indicates if air tightness is requested
    bool               bPermitRegenBrake       ;   ///< Indicates if regenerative brake is permitted
    bool               bPermitEddyBrakeForSB   ;   ///< Indicates if eddy current brake is permitted for SB
    bool               bPermitEddyBrakeForEB   ;   ///< Indicates if eddy current brake is permitted for EB
    bool               bPermitMagnetShoeBrake  ;   ///< Indicates if magnetic shoe brake is permitted
    int32_t lAllowedConsumption     ;   ///< Indicates the allowed consumption (Ampere)

    // INPUTS
    bool               bMainSwitchOn           ;   ///< Indicates if power is on
    bool               bTrainIntegrityOK       ;   ///< Indicates if integrity is OK
    eActiveCabin       ActiveCabin             ;   ///< Current active cabin status
    eActiveCabin       LastActiveCabin         ;   ///< Last active cabin status
    bool               bIsolation              ;   ///< Indicates if isolation is requested (no more used)
    bool               bSleeping               ;   ///< Indicates if sleeping is requested
    eDirection         DirectionCtrl           ;   ///< Current direction controller status
    bool               bEBStatus               ;   ///< Indicates if EB is applied
    bool               bSBStatus               ;   ///< Indicates if SB is applied
    bool               bRegenBrakeStatus       ;   ///< Indicates if regenerative brake is available
    bool               bMagnShoeBrakeStatus    ;   ///< Indicates if magnetic shoe brake is available
    bool               bEddyCurrBrakeStatus    ;   ///< Indicates if eddy current brake is available
    bool               bEPBrakeStatus          ;   ///< Indicates if electro pneumatic brake is available
    bool               bAdditionalBrakeStatus  ;   ///< Indicates if additional brake is available
    bool               bTractionStatus         ;   ///< Indicates if traction is applied

    bool               bNonLeading             ;   ///< Indicates if non leading is requested
    bool               bPassiveSH              ;   ///< signal indicating that PS is authorised
    bool               bColdMvtStatus          ;   ///< Indicates if cold mouvement have been detected
    eTrainDataType     TrainDataType           ;   ///< Indicates the types of train data used on DMI (Fixed, flexible, Switchable)

    double             dBrakePressure          ;   ///< Current pneumatic brake pressure
    int32_t lTractBrakeHandlePos    ;   ///< Current position of the traction/brake handle position in % (-100 to +100)
    ePlatformPosition  platformPosition       ;   ///< current track condition platform position
    ePlatformElevation platformElevation      ;   ///< current track condition platform elevation
    bool               bSystemFailure          ;   ///< Indication of system failure

}   STiuData;


//=======================    Permitted speed curve    =======================

/// Maximum number of speed points
#define     MAX_PS_POINT        100

/// Structure containing a speed curve ( Speed = f(Location) )
typedef struct SPerSpeedData
{
    int32_t lCount                      ;   ///< Counter for indication of new data
    int32_t lNbPoint                    ;   ///< Number of points of curve

    t_speed     dSpeed[MAX_PS_POINT]    ;   ///< Speed value
    t_distance  dLocation[MAX_PS_POINT] ;   ///< Location value

} SPerSpeedData;


//=======================    ETCS IO    =======================

/// Maximum number of last relevant balise groups
#define     MAX_LRBG_NB         50

/// Maximum number of balises in a group
#define     MAX_BAL_IN_GROUP    8

/// Enumeration for intervention types
typedef enum eInterventionType
{
    NO_INTERVENTION,        ///< No intervention
    INDIC_INTERVENTION,     ///< Indication intervention
    WARNING_INTERVENTION,   ///< Warning intervention
    BRAKE_INTERVENTION      ///< Brake intervention

} eInterventionType;

/// Enumeration for speed monitoring type
typedef enum eSpeedMonitoringStatus
{
    SPEEDMS_CSM = 0,
    SPEEDMS_PIM = 1,
    SPEEDMS_TSM = 2,
    SPEEDMS_RSM = 3,
    SPEEDMS_UNKNOWN

} eSpeedMonitoringStatus ;

#define STR_SPEED_MONIT_STATUS(d) \
    (d)==SPEEDMS_CSM  ?   "CSM"  : \
    (d)==SPEEDMS_PIM  ?   "PIM"  : \
    (d)==SPEEDMS_TSM  ?   "TSM"  : \
    (d)==SPEEDMS_RSM  ?   "RSM"  : "UNKNOWN"

/// Enumeration for speed monitoring type
typedef enum eTargetType
{
    TARGET_INTERMEDIATE,
    TARGET_EOA,
    TARGET_SVL

} eTargetType ;

#define STR_TARGET_TYPE(d) \
    (d)==TARGET_INTERMEDIATE  ? "Intermediate"   : \
    (d)==TARGET_EOA  ?   "EOA"  : \
    (d)==TARGET_SVL  ?   "SVL"  : "Unknown type"

/// Structure containing LRBG data
typedef struct SLRBG
{
    bool            bValid          ;   ///< Validity of the stored data
    SBG_Id          Id              ;   ///< BG identifier & direction
    t_distance      dPosBal         ;   ///< BG location in m
    t_time          dTime           ;   ///< Time stamp of the BG
    eDirection      RunningDirection;   ///< Running direction of the train when passing BG
    bool            bNewLRBG        ;   ///< Indicates if it is the first time LRBG is passed

} SLRBG;

/// Structure containing the main EVC data (status, location, ...)
/// It corresponds to a shared memory of the EVC simulator
typedef struct SETCS_IO
{
    SOnBoardStatus      OBStatus                ;   ///< Onboard status

    bool                bSuperviseSVL           ;   ///< Indicate if SvL = EoA or not (See Q_MAMODE)
    t_distance          dNextModeChangeLoc      ;   ///< Location for the next mode transition (on-sight or shunting)
    bool                bEmergencyStop          ;   ///< Indicates if an emergency stop is required

    STiuData            TiuData                 ;   ///< TIU inputs/outputs

    SLocationData       LocationData            ;   ///< Train location data

    STrackCondCurrent   TrackConditionData      ;   ///< Track condition data

    eValidity           Position_Validity       ;   ///< State of the train location data

    // Balises
    SBG_Id              LastBGId                ;   ///< Last passed balise group
    SLRBG               LRBGList[MAX_LRBG_NB]   ;   ///< List of passed balise group (the first is the last passed)
    SLinkingData        LinkingData             ;   ///< Linking data
    uint32_t uMissedBG               ;   ///< Identifier of the missed or ignored BG due to linking

    int32_t lGradientValue          ;   ///< Current gradient value according to track description

    bool                bTripActive             ;   ///< Indicates if transition to trip mode is available
    bool                bMA_Data                ;   ///< Indicates if some MA data are available
    bool                bModeProfileSH          ;   ///< Indicates if mode profile for SH is available
    bool                bTMAR_MARequested       ;   ///< Indicates if TMAR triggered MA has been requested
    bool                bTrackDeleted           ;   ///< Indicates if the track description has been deleted
    bool                bTksMsgInProcessing     ;   ///< Indicates if a trackside msg is in processing

    //    eSupervisionStatus      SpeedMonitStatus        ;   ///< Status of current speed monitoring (Overspeed, Indication...)
    eSpeedMonitoringStatus  SpeedMonitStatus        ;   ///< Status of current speed monitoring (CSM, PIM...)
    t_distance              dIP_Location            ;   ///< IP location for target indication
    t_distance              dPreIPLocation          ;   ///< Pre-IP location for target indication
    t_distance              BrakeTargetType         ;   ///< Type of the next target
    t_distance              dBrakeTargetLoc         ;   ///< Next brake target location
    t_speed                 dBrakeTargetSpeed       ;   ///< Next brake target speed
    bool                    bInTargetSpeedMonitoring;   ///< Indicates if currently in target speed monitoring
    bool                    bFeedbackStarted        ;   ///< boolean stating whereas the feedback function has started to reduce Tbs

    t_speed             dPredicted_Speed        ;   ///< Predicted speed at target location
    t_distance          dPredicted_Dist         ;   ///< Predicted distance where train reaches standstill

    eInterventionType   Int_Type                ;   ///< Type of intervention
    t_distance          dInterventionDist       ;   ///< Distance for intervention
    t_time              dInterventionTime       ;   ///< Time for intervention

    bool                bIntegrityFromDriver    ;   ///< Indicates if driver confirmed train integrity
    bool                bRouteUnsuitability     ;   ///< Indicates if route unsuitability is applicable
    t_distance          dUnSuitabilityLoc       ;   ///< Location of route unsuitability

    bool                bRunning                ;   ///< Indicates if simulation is running
    bool                bPaused                 ;   ///< Indicates if simulation is currently paused
    t_time              dStartTime              ;   ///< Simulation start time
    t_time              dElapsedTime            ;   ///< Elapsed simulation time (used when saving and loading context)
    t_time              dPauseStartTime         ;   ///< Current pause start time
    t_time              dPauseTotalTime         ;   ///< Total pause time

    SPerSpeedData       PerSpeedData            ;   ///< Data for permitted speed curves

} SETCS_IO;

/// Maximum number of targets
#define     MAX_TARGET_NB   256

/// structure to store target
typedef struct STarget
{
    t_distance  dTargetLocation ;   ///< Target location
    t_speed     dTargetSpeed    ;   ///< Target speed
    t_distance  dBrakingStart   ;   ///< Braking curve starting point
    t_distance  dIndLocation    ;   ///< Indication point
    t_distance  dPreIndLocation ;   ///< Pre-Indication point
    eTargetType Type            ;   ///< Type of target
} STarget;

/// Structure containing the list of targets
typedef struct STargetList
{
    int32_t lNb                     ;   ///< Number of targets in array below
    STarget Target[MAX_TARGET_NB]   ;   ///< List of targets
}
STargetList;


//=======================    reference track data    =======================

/// Structure containing MA data
typedef struct SMA_Para
{
    t_speed     dMainSpeed      ;   ///< Level 1: signal related permitted speed
    t_speed     dLOASpeed       ;   ///< Speed at LOA
    t_distance  dLOADistance    ;   ///< Distance to the LOA
    t_speed     dReleaseSpeed   ;   ///< Release speed
    t_distance  dOverlapDistance;   ///< Overlap (or danger point) distance
    t_distance  dRefLocation    ;   ///< Reference location for the MA data
    bool        bNextSignalUsed ;   ///< indicate if there is next signal info
    t_speed     dNextSignalSpd  ;   ///< signal speed at next signal (used for IF MA)
    t_distance  dNextSignalDist ;   ///< distance to next signal speed from ref loc (used for IF MA)

} SMA_Para;

/// Structure containing profile data for DMI
typedef struct SProfileDataForDMI
{
    SProfile    GradProfile ;   ///< List of gradient discontinuities
    SProfile    SpeedProfile;   ///< List of speed discontinuities

} SProfileDataForDMI;

/// structure containing result of message acknowledgements for processing
typedef struct SAck_Msg_Result
{
    bool    bAck                        ;   ///< Indicates if the message has been acknowledged in the specified window
    int32_t lMsgCode                    ;   ///< Identifier of the message
    char    szPlainText[MAX_TEXT_LENGTH];   ///< Free text


} SAck_Msg_Result;

/// Structure to store all temporary data, to avoid killing the kernel
/// Note: avoid doing operations like memset on those, because they may
///       be very time-consuming in case the host system does not have
///       enough memory (in this case, it is writing to a file: the swap)
typedef struct STemporaryDataBig
{
    STrackDesc      TrackDesc1  ;   ///< First  data set for MRSP and gradient track description (can be used while second set is being computed)
    STrackDesc      TrackDesc2  ;   ///< Second data set for MRSP and gradient track description (can be used while first  set is being computed)

    t_curve   TempCurve1  ;   ///< First  data set for temporary data used for curve calculation (can be used while second set is being computed)
    t_curve   TempCurve2  ;   ///< Second data set for temporary data used for curve calculation (can be used while first  set is being computed)
} STemporaryDataBig;

/// Enumeration for speed monitoring type
typedef enum eSpeedMonitoringType
{
    MONITORING_NONE,        ///< Speed is not monitored at all
    MONITORING_CEILING,     ///< Speed is simply monitored against a maximum value
    MONITORING_CURVE        ///< Speed is monitored using curves

} eSpeedMonitoringType;


#define STR_MONITORING_TYPE(d)    \
    (d)==MONITORING_NONE         ? "MONITORING_NONE"      : \
    (d)==MONITORING_CEILING      ? "MONITORING_CEILING"   : \
    (d)==MONITORING_CURVE        ? "MONITORING_CURVE"     : \
    "???"

/// Enumeration for reaction type
typedef enum eReactionType
{
    REACTION_NONE,      ///< No reaction
    REACTION_SB,        ///< Service brake is requested
    REACTION_EB,        ///< Emergency brake is requested
    REACTION_TRIP       ///< Transition to TRIP mode is requested

} eReactionType;

/// Structure containing movement bounds
typedef struct SMovementBound
{
    t_distance      dAbsDistance;   ///< Absolute location on the track
    eReactionType   eReaction   ;   ///< Reaction requested

} SMovementBound;


//=======================    Supervision data    =======================

/// Enumeration for state of curve data validity
typedef enum eCurveSetValidity
{
    CURVEDATA_INVALID       = 0,    ///< No current curve set is defined or computed data is invalid
    CURVEDATA_VALID_SET1    = 1,    ///< Curve set 1 is the current one
    CURVEDATA_VALID_SET2    = 2     ///< Curve set 2 is the current one

} eCurveSetValidity;

/// Structure containning supervision data
typedef struct SSupervisionData
{
    eSpeedMonitoringType    SpeedMonType            ;   ///< Current speed monitoring type
    int32_t lCurveCnt               ;   ///< Curve counter
    bool                    bCurvesInProcessing     ;   ///< Indicates if curves are currently in processing

    eCurveSetValidity       CurveSetStatus          ;   ///< Indicates which curve set is in use and if it contains valid data

    SMovementBound          MinBound                ;   ///< Minimum movement bound
    SMovementBound          MaxBound                ;   ///< Maximum movement bound
    t_speed                 dMaxModeSpeed           ;   ///< Maximum permitted speed for current mode
    t_speed                 dWModeSpeed             ;   ///< Warning speed for current mode
    t_speed                 dSBIModeSpeed           ;   ///< SBI speed for current mode
    t_speed                 dEBIModeSpeed           ;   ///< EBI speed for current mode
    eTrainMode              EnterMode                ;

    t_distance              dOverrideFormerEOA      ;   ///< Former EOA stored when override selected

    t_distance              dMaxSRDistance          ;   ///< Maximum distance in SR mode
    t_speed                 dMaxSRSpeed             ;   ///< Maximum speed in SR mode
    t_distance              dSRRefLocation          ;   ///< Reference location for maximum distance in SR mode
    bool                    bSRRefLocLocked         ;   ///< indication of locked reference SR location (cannot be modified for the current calculation)
    bool                    bNationalValueSRDistance;   ///< Indicates if national value is currently used for maximum distance in SR mode
    bool                    bNationalValueSRSpeed   ;   ///< Indicates if national value is currently used for maximum speed in SR mode

    t_speed                 dMaxShuntingSpeed       ;   ///< Maximum speed in SH mode, defaulting to national value but that can be modified by mode profile
    bool                    bNationalValueSHSpeed   ;   ///< Indicates if national value is currently used for maximum speed in SH mode

    t_distance              dMaxReversingDistance   ;   ///< Maximum distance in RV mode
    t_speed                 dMaxReversingSpeed      ;   ///< Maximum speed in RV mode
    t_distance              dReversingRefLocation   ;   ///< Reference location for maximum distance in RV mode

    t_distance              dPTRefLocation          ;   ///< Reference location for maximum distance in PT mode

    STargetList             TargetList              ;   ///< List of targets when in curve monitoring

    t_distance              dReleaseAreaStart       ;   ///< Release start location for release speed monitoring
    t_speed                 dReleaseSpeed           ;   ///< Release speed for release speed monitoring

    bool                    bWarningIndication      ;   ///< Indicates if indication of warning on MMI is required (warning speed is passed)
    bool                    bOverSpeedInt           ;   ///< Indicates if intervention is due to over speed
    bool                    bIndicationSpeed        ;   ///< indicates if indication speed is passed

    bool                    bPreIndication          ;   ///< Indicates if train is after pre-indication point

    t_speed                 dMRSP                   ;   ///< Most restrictive speed

    t_speed                 dIndic_Speed            ;   ///< Indication speed
    t_speed                 dPermitted_Speed        ;   ///< Permitted speed
    t_speed                 dWarn_Speed             ;   ///< Warning speed
    t_speed                 dSBI_Speed              ;   ///< SBI speed
    t_speed                 dEBI_Speed              ;   ///< EBI speed

    t_distance              dEOALocation            ;   ///< EOA not taking in account LX
    int32_t lNewStopLocCnt          ;   ///< Counter of stop location (reduction) to apply A3.4
    t_distance              dStopLocation           ;   ///< SL not taking in account mode change or route unsuitability

    bool                    bReverseMvt             ;   ///< Indicates if the train performs a reverse movement

    int32_t lSBAckRqst              ;   ///< Counter for request of ack to release SB
    int32_t lEBAckRqst              ;   ///< Counter for request of ack to release EB

    bool                    bInNonProtectedLXArea   ;   ///< indicate that the train is in the BCA of a non protected LX
    uint32_t ulTunnelStopAreaDist    ;   ///< Indicates distance to tunnel stopping area ( 0 = inside )

} SSupervisionData;


/// Macro to get the last LOA distance (position of last target in the target list array)
#define SUPDATA_LASTLOADISTANCE \
    (pShared->ShSupervisionData.TargetList.Target[pShared->ShSupervisionData.TargetList.lNb-1].dTargetLocation)
/// Macro to get the last LOA speed (speed of last target in the target list array)
#define SUPDATA_LASTLOASPEED \
    (pShared->ShSupervisionData.TargetList.Target[pShared->ShSupervisionData.TargetList.lNb-1].dTargetSpeed)
/// Macro to get the last LOA indication point (indication point of last target in the target list array)
#define SUPDATA_LASTLOABRAKEIP \
    (pShared->ShSupervisionData.TargetList.Target[pShared->ShSupervisionData.TargetList.lNb-1].dIndLocation)
/// Macro to get the last LOA pre-indication point (pre-indication point of last target in the target list array)
#define SUPDATA_LASTLOABRAKEPREIP \
    (pShared->ShSupervisionData.TargetList.Target[pShared->ShSupervisionData.TargetList.lNb-1].dPreIndLocation)
/// Macro to get the last LOA brake start position (brake start position of last target in the target list array)
#define SUPDATA_LASTLOABRAKESTART \
    (pShared->ShSupervisionData.TargetList.Target[pShared->ShSupervisionData.TargetList.lNb-1].dBrakingStart)



//===============================    Static data    =========================================================

//------- static data used by Balise/loop manager -----

/// Enumeration for balise group reading status
typedef enum eGroupReadingStatus
{
    GRP_READ_NO            ,                               ///< No balise group reading
    GRP_READ_IN_PROGRESS   ,                               ///< Balise group reading in progress
    GRP_READ_END           ,                               ///< Balise group reading end
    GRP_READ_FAIL                                          ///< Balise group reading failed

} eGroupReadingStatus;

/// Enumeration for balise group direction validity
typedef enum eGroupReadingOrder
{
    GRP_ORDER_INC      ,                                   ///< Reading balise group in incremental direction
    GRP_ORDER_DEC      ,                                   ///< Reading balise group in decremental direction
    GRP_ORDER_UNKNOWN                                      ///< Reading direction of balise group is unknown

} eGroupReadingOrder;


typedef struct SBAL_CheckRepositioning
{
    bool         bActive         ;
    t_distance   dBGRefLocation  ;
    t_distance   dBGAccuracy     ;

} SBAL_CheckRepositioning;


typedef struct SBAL_ManageBaliseLinking
{
    t_distance   LastLinkedBGLoc        ; // location of the last linked BG (m)
    int32_t iNextIdx               ; // indice of the next BG in linking data table
    eTrainMode   PreviousMode           ;
    int32_t lNewStopLocCnt         ;
    uint32_t bPreviousNoReactErr    ;

} SBAL_ManageBaliseLinking;


typedef struct SBAL_Manage_balise_msg
{
    eGroupReadingOrder   GroupOrder                 ;
    int32_t lGroupPig                  ;
    int32_t GroupCount                 ;
    bool                 InvalidTelegramCounter     ;
    bool                 bInvalidPacketDuplication  ;   ///< Some paquets are dupplicated, but it's not authorised for these data
    SDecodedTksMsg       LastValidBalise            ;
    t_distance           LastValidBaliseLocation    ;
    bool                 InvalidBalReceived         ;

} SBAL_Manage_balise_msg;


typedef struct SBAL_Manage_Track_Condition_Big_Metal_Masses
{
    STrackCondTrigger   arTrackCondMetals[MAX_COND_TRIGGERS];   ///< array to store metal condition events
    eTrainMode          Train_Mode                          ;   ///< accounts for actions to be taken at train mode change
    int32_t lNewStopLocCnt                      ;
    bool                bDataValid                          ;   ///< indicate that valid data are stored

} SBAL_Manage_Track_Condition_Big_Metal_Masses;


typedef struct SBAL_ManageBrkForBGinconsistency
{
    bool bInterventionDone ;
    bool ReductionRequested;

} SBAL_ManageBrkForBGinconsistency;


typedef struct SBAL_ManageLoopInfo
{
    eTrainMode   LastMode           ;
    SEOLMData    LoopData           ;
    eActiveCabin ActiveCabin        ;
    SBG_Id       LoopMainBG         ;   ///< Globam variable containing current loop main BG identity (reference location)
    bool         bLoopMainBGKnown   ;   ///< Globam variable indicating if current loop main BG identity is already known


} SBAL_ManageLoopInfo;

#define MAX_BALISE_MSG_SIZE             104            ///< Maximum balise (and loop) message size in bytes (830 bits)

typedef struct SBAL_ManageLoopMsg
{
    uint8_t   puszLastLoopMsg[MAX_BALISE_MSG_SIZE];   ///< Global variable containing last evaluated loop message
    int32_t lLastLoopMsgSize                    ;   ///< Global variable containing size of last evaluated loop message

} SBAL_ManageLoopMsg;


typedef struct SBAL_ManageBTMAlarm
{
    bool         bBTMAlarmON  ;
    t_distance   BTMAlarmONLoc;

} SBAL_ManageBTMAlarm;


typedef struct SBalStatic
{
    eGroupReadingStatus GroupReading                        ;   ///< Global variable containing current balise group reading status
    t_distance          dLastBaliseLocation                 ;   ///< Global variable containing the location of the last read balise
    bool                bLinkingInfoUsed                    ;   ///< Global variable indication if linking information are currently used
    bool                bRefBaliseDuplicated                ;   ///< Global variable indicating if reference balise is duplicated
    t_distance          dDupRefBaliseLocation               ;   ///< Global variable containing the duplicated reference balise group
    SDecodedTksMsg      BGMsg                               ;   ///< structure used to store data from a balise group

    SBAL_CheckRepositioning                         Static_CheckRepositioning;
    SBAL_ManageBaliseLinking                        Static_ManageBaliseLinking;
    SBAL_Manage_balise_msg                          Static_Manage_balise_msg;
    SBAL_Manage_Track_Condition_Big_Metal_Masses    Static_Manage_Track_Condition_Big_Metal_Masses;
    SBAL_ManageBrkForBGinconsistency                Static_ManageBrkForBGinconsistency;
    SBAL_ManageLoopInfo                             Static_ManageLoopInfo;
    SBAL_ManageLoopMsg                              Static_ManageLoopMsg;
    SBAL_ManageBTMAlarm                             Static_ManageBTMAlarm;

} SBalStatic;



//------- static data used by curve computer -----

#define TSR_NB  32  ///< Maximum size of temporary speed restriction table

typedef struct SCOMP_SuspendThread
{
    int32_t lCount_ms;

} SCOMP_SuspendThread;


typedef struct SCOMP_ManageModeSpeedProfile
{
    SModeProf   ModeSpeedProf;

} SCOMP_ManageModeSpeedProfile;


typedef struct SCOMP_PrepareTrackDataAndCalculateMAcurves
{
    int32_t lNewStopLocCnt;

} SCOMP_PrepareTrackDataAndCalculateMAcurves;

typedef struct SCOMP_GetSRUNRefLoc
{
    double dLastRefLoc;

} SCOMP_GetSRUNRefLoc;


/// Structure used by SCOMP_TransmitMRSPToDMI() to store the previous status of internal data
typedef struct SCOMP_TransmitMRSPToDMI
{
    SProfileDataForDMI   PrevDataForDMI; ///< Previous track profile data transmitted to DMI

} SCOMP_TransmitMRSPToDMI;


typedef struct SCOMP_ManageCurrentMA
{
    SMAData     Current_MA_data;
    SMA_Para    MA_Para;
    SMA_Para    TempMA_Para;
    bool        bLastOS;
    bool        bLastLS;
    bool        bProfilesAreSufficient;
    eTrainMode  TrainMode;
    bool        bMsgDisplayed;

} SCOMP_ManageCurrentMA;


typedef struct SCOMP_ReleaseMAAtStandstill
{
    bool    bReleaseAtStandstillRequested;

} SCOMP_ReleaseMAAtStandstill;


typedef struct SCOMP_ManageAxleLoadSpeedProfile
{
    SAxleLoadSpeedRestriction   Axle_Profile;
    eTrainMode                  TrainMode;
    int32_t lNewStopLocCnt;

} SCOMP_ManageAxleLoadSpeedProfile;


typedef struct SCOMP_UpdateSupervisionDataDependingOnMode
{
    eTrainMode  LastMode;
    bool        bLastTripActive;
    t_speed     dLastTrainSpeedLimit;
    eDirection  LastDirection;

} SCOMP_UpdateSupervisionDataDependingOnMode;


typedef struct SCOMP_TransmitTSRToDMI
{
    STrackCondTrigger PrevTSRList[MAX_COND_TRIGGERS];

} SCOMP_TransmitTSRToDMI;


typedef struct SCOMP_UpdateTSRList
{
    eDirection  TSRDir;
    bool        bTSRReceivedWaitDir;
    eTrainMode  TrainMode;

} SCOMP_UpdateTSRList;


typedef struct SCOMP_UpdateDefaultGradient
{
    eTrainMode TrainMode;

} SCOMP_UpdateDefaultGradient;


typedef struct SCOMP_ManageSpeedProfileData
{
    SProfile    SpeedProfile;
    t_distance  adSSPorgLoc[MAX_PROFILE_DISCONTINUITIES]; ///< store SSP position without effect of train length delay

    eTrainMode  TrainMode;

} SCOMP_ManageSpeedProfileData;


typedef struct SCOMP_ManageGradientProfileData
{
    SProfile     GradientProfile;
    eTrainMode   TrainMode;
    int32_t lCount;             ///< Counter of gradient profile change

} SCOMP_ManageGradientProfileData;

typedef struct SCOMP_ManageLevelCrossing
{
    eTrainMode PreviousMode;
    bool       bLxTxtDisplayed;

} SCOMP_ManageLevelCrossing;


typedef struct SCOMP_UpdateCurvesCnt
{
    eSpeedMonitoringType    PreviouseSpeedMonType;
    t_speed                 dPreviousCeilingSpeed;

} SCOMP_UpdateCurvesCnt;



typedef struct SCOMP_ManageDataChangeForCurveCalc_6L
{
    bool    bFirstCall   ;
    t_time  LastCalcTime ;
    t_time  LastTbs      ;
    t_accel LastTrainAcc ;

} SCOMP_ManageDataChangeForCurveCalc_6L;


typedef struct SCOMP_IsNewTarget
{
    t_speed      dLastTargetSpeed;
    t_time       dLastTbs;

} SCOMP_IsNewTarget;


typedef struct SCOMP_ManagePermittedBrakingDistance
{
    eTrainMode  TrainMode;
    int32_t lNewStopLocCnt;

    t_distance  dRestrictionStart   [MAX_PERM_BRAKE_DIST]   ;   ///< List of speed restriction start locations (in meters)
    t_distance  dRestrictionLength  [MAX_PERM_BRAKE_DIST]   ;   ///< List of speed restriction lengths (in meters)
    t_speed     dRestrictionSpeed   [MAX_PERM_BRAKE_DIST]   ;   ///< List of speed restriction values (in m/s)
    int32_t lNbRestrictions                             ;   ///< Number of speed restrictions

}  SCOMP_ManagePermittedBrakingDistance;


typedef struct SCOMP_ManageBrakeFeedback
{
    bool    bTbs_locked             ;   ///< boolean stating whereas Tbs has been locked  to a shorter time during the release of the brakes
    double  p0                      ;   ///< kPa, reference pressure when not braking (nominal 500 kPa)
    t_time  dLastPressureUpdateTime ;   ///< time of the last pressure update

} SCOMP_ManageBrakeFeedback;


typedef struct SCompStatic
{
    bool        bKeepLast;              ///< Global variable indicating if last reference location has to be retained instead of current location (see GetSR_UN_RefLoc() function)
    STSRData    aTmp_Speed[TSR_NB];     ///< Global variable containing list of temporary speed restrictions
    int32_t lTmp_Speed_nb;          ///< Global variable containing number of stored temporary speed restrictions
    int32_t lDefaultGradient;       ///< Global variable containing default gradient value
    SLXList     CurrentLX;              ///< Current list of level crossing information
    bool        bMAWithoutModeProfileReceived;
    t_time      dTbsUsedForCalc;
    t_distance  dLastTargetdistance;
    t_time      T_bs1;
    t_time      T_bs2;

    SCOMP_SuspendThread                         Static_SuspendThread;
    SCOMP_ManageModeSpeedProfile                Static_ManageModeSpeedProfile;
    SCOMP_PrepareTrackDataAndCalculateMAcurves  Static_PrepareTrackDataAndCalculateMAcurves;
    SCOMP_GetSRUNRefLoc                         Static_GetSRUNRefLoc;
    SCOMP_TransmitMRSPToDMI                     Static_TransmitMRSPToMMI;
    SCOMP_ManageCurrentMA                       Static_ManageCurrentMA;
    SCOMP_ReleaseMAAtStandstill                 Static_ReleaseMAAtStandstill;
    SCOMP_ManageAxleLoadSpeedProfile            Static_ManageAxleLoadSpeedProfile;
    SCOMP_UpdateSupervisionDataDependingOnMode  Static_UpdateSupervisionDataDependingOnMode;
    SCOMP_TransmitTSRToDMI                      Static_TransmitTSRToDMI;
    SCOMP_UpdateTSRList                         Static_UpdateTSRList;
    SCOMP_UpdateDefaultGradient                 Static_UpdateDefaultGradient;
    SCOMP_ManageSpeedProfileData                Static_ManageSpeedProfileData;
    SCOMP_ManageGradientProfileData             Static_ManageGradientProfileData;
    SCOMP_ManageLevelCrossing                   Static_ManageLevelCrossing;
    SCOMP_UpdateCurvesCnt                       Static_UpdateCurvesCnt;
    SCOMP_ManageDataChangeForCurveCalc_6L       Static_ManageDataChangeForCurveCalc_6L;
    SCOMP_IsNewTarget                           Static_IsNewTarget;
    SCOMP_ManageBrakeFeedback                   Static_ManageBrakeFeedback;
    SCOMP_ManagePermittedBrakingDistance        Static_ManagePermittedBrakingDistance;

} SCompStatic;


//--------------------- static data used by Data manager ----------------------------

#define MAX_ADHESION_DATA   8


typedef struct SDAT_ManageOperatedSystemVersion
{
    eLevel  PreviousLevel;

} SDAT_ManageOperatedSystemVersion;


typedef struct SDAT_Manage_Traction_Power_Change_Order
{
    SCondTraction    Change_TractionSystem;
    eTrainMode       TrainMode;

} SDAT_Manage_Traction_Power_Change_Order;


typedef struct SDAT_Manage_Route_Suitability
{
    SRouteSuitabilityEntry   pSuitabilities[MAX_SUITABILITY_ENTRIES];
    eTrainMode               PreviousMode;
    bool                     bActivated;
    int32_t lNewStopLocCnt;

} SDAT_Manage_Route_Suitability;


typedef struct SDAT_Manage_Geo_Pos
{
    SGeoPos      GeoPosData;
    bool         bKPApplicable;
    SKPReference KPReferenceData;
    t_distance   dKPBGLocation;
    eDirection   KPBGOrientation;
    eTrainMode   TrainMode;
    eActiveCabin ActiveCAB;

} SDAT_Manage_Geo_Pos;


typedef struct SDAT_ManageInhibRevocableTSR
{
    STSRInhibitRevocable    TSRInhibitRevocable ;
    eTrainMode              TrainMode           ;
    int32_t lRBCcountryId       ;   ///< Store information about RBC : country id (SRS 3.0.0 - 3.11.5.14)
    int32_t lRBCid              ;   ///< Store RBC id (SRS 3.0.0 - 3.11.5.14)

} SDAT_ManageInhibRevocableTSR;


typedef struct SDAT_Manage_Track_Condition
{
    bool                 bDataValid;                           ///< indicate if valid data are stored
    STrackCondTrigger    TrackCondTriggers[MAX_COND_TRIGGERS]; ///< array to store track condition events
    eTrainMode           Train_Mode;                           ///< indicate previous mode
    int32_t lNewStopLocCnt;                       ///< previous value of stop location

} SDAT_Manage_Track_Condition;


typedef struct SDAT_ManageNationalData
{
    SNationalData   NationalClass1v1;          // Store last paquet 3 class1v1.x
    SNationalData   NationalClass1v2;          // Store last paquet 3 class1v2.0
    bool            b1v1Applied;
    bool            b1v2Applied;
    uint32_t ulLastVersion;

    t_distance      dLastLocation;
    SBG_Id          CurrentBalise;
    bool            bCurrentBaliseValid;
    eTrainMode      LastMode;
    SNationalData   LastNationalData;

} SDAT_ManageNationalData;


typedef struct SDAT_ManageSRDistanceFromLoop
{
    eTrainMode  PreviousMode;
    SSRDistInfo SRDistInfo;

} SDAT_ManageSRDistanceFromLoop;


typedef struct SDAT_ManageAdhesion
{
    SProfile    TksAdhesionProfile;
    eTrainMode  TrainMode;

} SDAT_ManageAdhesion;


#define MAX_EMERSTOP_NB 20      ///< Maximum number of emergency stops

typedef enum eEmergencyStopStatus
{
    EMERSTOP_STATUS_UNKNOWN = 0,
    EMERSTOP_STATUS_ACTIVE = 1,
    EMERSTOP_STATUS_TOREMOVE = 2

}eEmergencyStopStatus;

typedef struct SDAT_ManageEmergencyStop
{
    int32_t lEmerStopId[MAX_EMERSTOP_NB];
    //bool         bEmerStop  [MAX_EMERSTOP_NB];
    eEmergencyStopStatus eEmerStopStatus [MAX_EMERSTOP_NB];
    bool         bValidEmer                  ;
    t_distance   dEmerStopMinDistance        ;
    eTrainMode   LastMode                    ;

} SDAT_ManageEmergencyStop;


typedef struct SDAT_ManagePositionReport
{
    SLocReportPara   Loc_Rep_Para;
    t_time           last_PR_Time;       ///< time of last position report sending
    int32_t last_PR_Pos;        ///< position of the last position report sending
    int32_t OtherLocId;
    eDirection       LastTrainDir        ;
    eTrainMode       Last_Train_Mode     ;
    eLevel           LastLevel           ;
    eLevelTr         LastLevelTr         ;
    bool             LastIntegrity       ;
    bool             LastDriverIntegrity ;
    bool             bPRParamFromRBC     ;

} SDAT_ManagePositionReport;


typedef struct SDAT_Manage_MA_Requests
{
    SMARqstPara  currentMA_Rqst_Para;
    // when this variable is > 0.0 it means that a first MA has been requested and that
    // periodic requests are being sent (and this will be stopped when a new MA is received)
    t_time       dLastMARequestSendingTimeSec ;
    bool         bStopCyclicRequests;
    eLevel       LastLevel;
    eTrainMode   LastMode;
    bool         bPreviousTrackDeleted ;

} SDAT_Manage_MA_Requests;


typedef struct SDAT_Manage_Radio_IF_Area
{
    bool             DataValid          ;
    bool             WaitConnectionState;
    t_distance       Connection_Location;
    SInfillData      Infill_Data;
    eTrainMode       TrainMode;
    bool             PassedBG;
    int32_t RIUId;
    uint8_t            RIUPhoneNb[PHONE_NB_SIZE];
    int32_t lNewStopLocCnt;
    bool             bDisconnectPrimaryRadio;
    bool             bConnectPrimaryRadio;
    bool             bDisconnectSecondaryRadio;

} SDAT_Manage_Radio_IF_Area;


typedef struct SDAT_ManageExternalTrainData
{
    bool                bCriticalDataImpacted;
    bool                bWaitForStandstill;
    bool                bWaitForDriverAcknowledgement;
    bool                bNewExternalTrainData;
    bool                bExternalTrainDataInProgress;
    SExternalTrainData  NewExternalTrainData;
    SExternalTrainData  ExternalTrainDataInProgress;

} SDAT_ManageExternalTrainData;

typedef struct SDAT_ManageAllowedConsumption
{
    int32_t lCurrentAllowedConsumption;  ///< Current allowed consumption (Ampere)

    bool        bLocationValid;             ///< Indicate if the new data received are valid (dLocation and lNewAllowedConsumption)
    t_distance	dLocation;                  ///< Distance where the new allowed consumption will be set
    int32_t lNewAllowedConsumption;     ///< Value of the new allowed consumption (Ampere)
} SDAT_ManageAllowedConsumption;


typedef struct SDatStatic
{

    SLevelTransition    CurrentLevelTr                              ;   ///< Global variable containing current level transition for message buffering
    STracksideMessage   TransitionBuffer[MAX_TRANSITION_BUFFER_SIZE];   ///< Global variable containing trackside message buffer in level transition/RBC transition

    SDAT_ManageOperatedSystemVersion                Static_ManageOperatedSystemVersion;
    SDAT_Manage_Traction_Power_Change_Order         Static_Manage_Traction_Power_Change_Order;
    SDAT_Manage_Route_Suitability                   Static_Manage_Route_Suitability;
    SDAT_Manage_Geo_Pos                             Static_Manage_Geo_Pos;
    SDAT_Manage_Track_Condition                     Static_Manage_Track_Condition;
    SDAT_ManageNationalData                         Static_ManageNationalData;
    SDAT_ManageSRDistanceFromLoop                   Static_ManageSRDistanceFromLoop;
    SDAT_ManageAdhesion                             Static_ManageAdhesion;
    SDAT_ManageEmergencyStop                        Static_ManageEmergencyStop;
    SDAT_ManagePositionReport                       Static_ManagePositionReport;
    SDAT_Manage_MA_Requests                         Static_Manage_MA_Requests;
    SDAT_Manage_Radio_IF_Area                       Static_Manage_Radio_IF_Area;
    SDAT_ManageExternalTrainData                    Static_ManageExternalTrainData;
    SDAT_ManageInhibRevocableTSR                    Static_ManageInhibRevocableTSR;
    SAllowedConsumption                             Static_ManageAllowedConsumption;

} SDatStatic;


//------------- static data used by DMI interface manager ----------------

typedef enum eDmiObjectType
{
    DMI_OBJ_ICON,
    DMI_OBJ_TXT

}eDmiObjectType;


typedef struct SDMI_Icon {

    int32_t lId;
    int32_t lArea;

} SDMI_Icon;


typedef struct SDMI_TextMsg {

    int32_t         lId                         ;   ///< Identifier used to inform RBC
    int32_t         lCountryId                  ;   ///< Identity number of the country or region (NID_C)
    int32_t         lRBCId                      ;   ///< RBC ETCS identity number (NID_RBC)
    bool            bReportRequirement          ;   ///< driver confirmation requirement (Q_TEXTREPORT)

    eText_Class     TextClass                   ;   ///< Define type of message (Trackside fixed text or plain text or system message )
    int32_t         lTextCode                   ;   ///< Code of the fixed text ( -1 = plain text )
    int32_t         TextMesgId                  ;    ///< Text message ID (e.g Emergency stop status message id)
    char            szPlainText[MAX_TEXT_LENGTH];   ///< free text

} SDMI_TextMsg;

/// Structure used to store message properties in DMI thread
typedef struct SDMI_DisplayObject
{
    int32_t             lId;                ///< A primary key associated to identify an object (when it is ack)
    eAddressId          SenderId;           ///< Module which has sent the data

    SDisplayConditions  DisplayCond;        ///< Start / end Conditions associated to the object
    bool                bIsDisplay;         ///< Indicate if the object is display

    eDmiObjectType      Type;               ///< Type of object (Txt, icon)
    SDMI_Icon           Icon;               ///< Icon data
    SDMI_TextMsg        TextMesg;           ///< Text message data

    t_time              dStartTime;         ///< Time when the object has been displayed
    bool                bAckReceived;       ///< Indicate if we have received the driver ack (optionnal)
    bool                bBrakeApplied;      ///< SB applied due to non-acknowledgement of driver (optionnal)

    // Optionnally store SystemInfo if object concerns an eSystemInfo
    // This info will be re-transmit to the concerned module when driver ack.
    int32_t SystemInfo;

}SDMI_DisplayObject;


/// Structure used to transmit message data to DMI thread
typedef struct SSystemMessageData
{
    int32_t             Info;
    SDisplayConditions  Conditions;
    int32_t             MsgID;

} SSystemMessageData;

/// Structure used to transmit remove status message to DMI thread
typedef struct SRemoveSystemMessage
{
    eSystemStatusMesg   statusCode;
    int32_t             MsgID;
} SRemoveSystemMessage;

///////////////////////////////////////////////////////////////////////////

#define MAX_OBJECT_NB                   40          ///< Maximum number of icon in table used for icon management

typedef struct SDMI_ManageTAFRequest
{
    STrackFreeRqst   TrackFree;
    eLevel           LastLevel;
    eTrainMode       TrainMode;
    bool             bMsgDisplayed;

}SDMI_ManageTAFRequest;

typedef struct SDMI_IsLXListModified
{
    SLXList TmpLX;

} SDMI_IsLXListModified;

typedef struct SDMI_ManageTunnelStoppingArea
{
    eTrainMode  TrainMode;
    t_curve     SBI;
    t_curve     Permitted;
    int32_t lCurrentStoppingDistance;
    int32_t lCurrentStartingDistance;

}SDMI_ManageTunnelStoppingArea;

typedef struct SDMI_ManageNonStoppinArea
{
    t_curve     SBI1;               // SBI_D represented in 5.18.4 Fig.15
    t_curve     SBI2;               // SBI_G represented in 5.18.4 Fig.15
    int32_t lCurrentAreaStart;
    int32_t lCurrentAreaStop;

}SDMI_ManageNonStoppinArea;

typedef struct SDMI_ManageTextMessageDisplay
{
    t_distance   LastStopLoc;
    eTrainMode   TrainMode;
    eActiveCabin CurrentCabin;

} SDMI_ManageTextMessageDisplay;


typedef struct SDMI_NTCInput
{
    uint32_t ulSizeBits;
    uint8_t*       pcMessage;
} SDMI_NTCInput;

typedef struct SDMI_NTCOutput
{
    uint8_t           cNIDPacket;
    uint8_t           ucLength[2];
    uint8_t*          pcMessage;
} SDMI_NTCOutput;

/// Maximum number of simultaneous (current) order displayed on DMI
#define MAX_DMI_ORDER       3


typedef struct SDMI_ManageOrder
{
    bool                bFirstCall;
    eTrainMode          PreviousMode;

    STrackCondTrigger   aTCList[MAX_COND_TRIGGERS];
    STrackCondTrigger   aTSRList[MAX_COND_TRIGGERS];
    STrackCondTrigger   aBigMetalList[MAX_COND_TRIGGERS];
    STrackCondTrigger   aLXList[MAX_COND_TRIGGERS];
    STrackCondTrigger   aTractionList[MAX_COND_TRIGGERS];

} SDMI_ManageOrder;

#define MAX_DMIMSG_NB               20                                      ///< Maximum number of DMI messages

typedef struct SDmiStatic
{
    SDMI_DisplayObject              aObject[MAX_OBJECT_NB];                 ///< Global variable containing number of object to display
    int32_t lNbObject;                              ///< Global variable containing number of object to display
    int32_t lMessId;                                ///< Global variable containing message identifier counter

    SLXList                         CurrentLX;                              ///< Current list of level crossing information

    bool                            bStaticEmerSynbol;

    SProfileDataForDMI              RouteInfo;
    int32_t lNbTC;
    int32_t lNbTSR;
    int32_t lNbBigMetal;
    int32_t lNbTraction;
    STrackCondTrigger               TrackCondition[MAX_COND_TRIGGERS];
    STrackCondTrigger               TSRList[MAX_COND_TRIGGERS];
    STrackCondTrigger               BigMetalMasses[MAX_COND_TRIGGERS];
    STrackCondTrigger               ModeProfile[MAX_COND_TRIGGERS];
    STrackCondTrigger               Traction[MAX_COND_TRIGGERS];

    SETCSTrainData                  NewTrainData;                           ///< Global variable containing new train data entered by driver
    bool                            bWaitTrainDataValidation;               ///< Global variable indicating if EVC is waiting for train data validation
    bool                            bRestoreContext;                        ///< Global variable indicating if context has to be restored (resend text messages, track description, ...)
    SVBaliseCoverMarker             NewVBCSet;
    SVBaliseCoverMarker             NewVBCRemove;

    SDMI_IsLXListModified           Static_IsLXListModified;
    SDMI_ManageTextMessageDisplay   Static_ManageTextMessageDisplay;
    SDMI_ManageOrder                Static_ManageOrder;
    SDMI_ManageTAFRequest           Static_ManageTAF;
    SDMI_ManageTunnelStoppingArea   Static_ManageTunnelStoppingArea;
    SDMI_ManageNonStoppinArea       Static_ManageNonStoppinArea;





    eEVCStatus      savedStatus;
    eTrainMode      savedMode;
    eLevel          savedLevel;
    SLevel          currentLevelPriority[MAX_ITERATIONS];
    int32_t lNbLevelPriority;
    uint32_t ulAvailableMenus;

} SDmiStatic;


//------------- static data used by mode manager ----------------------

typedef struct SMOD_EntryInNewStatus
{
    eEVCStatus LastStatus;

} SMOD_EntryInNewStatus;


typedef struct SMOD_ManageTrainDataStatus
{
    eTrainMode PreviousMode;

} SMOD_ManageTrainDataStatus;


typedef struct SMOD_ManageShuntingRequest
{
    bool     bRequestDone       ;   ///< indicate that the driver has request SH
    int32_t lSendingAttempt    ;   ///< number of SH request sent
    t_time   dSendingTime       ;   ///< time of last sending request

} SMOD_ManageShuntingRequest;


typedef struct SMOD_ManageStatusSHWaitRBCTerminate
{
    int32_t lSendingAttempt;
    t_time   dSendingTime;

} SMOD_ManageStatusSHWaitRBCTerminate;


typedef struct SMOD_ManagePassiveShuntRequest
{
    bool bShuntingOnDesksClosure;

} SMOD_ManagePassiveShuntRequest;


typedef struct SMOD_ManageStatusTrip
{
    bool bMsgSent;

} SMOD_ManageStatusTrip;

typedef struct SMOD_ManageStatusWaitRBCAckPT
{
    int32_t lSendingAttempt;
    t_time   dSendingTime;

} SMOD_ManageStatusWaitRBCAckPT;


typedef struct SMOD_ManageStatusPTWaitEmerStopRevoc
{
    bool bEmergency;

} SMOD_ManageStatusPTWaitEmerStopRevoc;


typedef struct SMOD_CheckCommonStatusTransition
{
    bool bSafetyError;

} SMOD_CheckCommonStatusTransition;


typedef struct SMOD_ManageTripActivation
{
    t_distance  dReset_Loc;
    t_distance  dReset_EOA;
    eTrainMode  Reset_Mode;
    t_time      dReset_Time;
    bool        bRequestEndOfOverride;
    t_time      dEndOfOverrideRqstTime;
    eTrainMode  LastMode;

} SMOD_ManageTripActivation;


typedef struct SMOD_ManageModeChangeBrake
{
    bool    bBrkRqst;
    t_time  dBrkTime;
    bool    bInterventionDone;

} SMOD_ManageModeChangeBrake;


typedef struct SMOD_ModeProfAck
{
    eTrainMode Mode;
    t_distance dLocation;

}SMOD_ModeProfAck;

typedef struct SMOD_ManageModeProf
{
    SModeProf       ModeProfile;        ///< Current applicable mode profile
    eTrainMode      AckRqst_mode;       ///< indicate if ack request for next mode has been sent
    int32_t lNextModeIdx;       ///< Index of next mode change
    SMOD_ModeProfAck Ack;

    eTrainMode  LastMode;
    int32_t lNewStopLocCnt;

} SMOD_ManageModeProf;


typedef struct SMOD_ManageLevelTransition
{
    SLevelTr    LevelTransitionData;
    t_distance  dLevelTrLoc;
    t_distance  dAckLength;
    t_time      dTransitionTime;
    bool        bAckRequested;
    bool        bAckReceived;
    bool        bSBrequired;
    t_distance  dL2BorderLocation;
    eTrainMode  LastMode;
    bool        bCheckBrakeForAck;

} SMOD_ManageLevelTransition;

typedef struct SMOD_LevelTrOnModeChange
{
    bool        bApply;
    SLevelTr    LevelTr;

} SMOD_LevelTrOnModeChange;


typedef struct SMOD_ManageReversingArea
{
    bool            bSend;
    SReversingArea  ReversingArea;
    eTrainMode      TrainMode;
    int32_t lNewStopLocCnt;
    bool            bAckRequested;

} SMOD_ManageReversingArea;

typedef struct SMOD_ManageLevelInhibitionSB
{
    bool    bAck;
    bool    bSBactive;
} SMOD_ManageLevelInhibitionSB;

typedef struct SMOD_ManageReconnectionRqst
{
    SRadioData  RadioData;
} SMOD_ManageReconnectionRqst;


typedef struct SModeStatic
{
    SMOD_EntryInNewStatus                   Static_EntryInNewStatus;
    SMOD_ManageTrainDataStatus              Static_ManageTrainDataStatus;
    SMOD_ManageShuntingRequest              Static_ManageShuntingRequest;
    SMOD_ManageStatusSHWaitRBCTerminate     Static_ManageStatusSHWaitRBCTerminate;
    SMOD_ManagePassiveShuntRequest          Static_ManagePassiveShuntRequest;
    SMOD_ManageStatusTrip                   Static_ManageStatusTrip;
    SMOD_ManageStatusWaitRBCAckPT           Static_ManageStatusWaitRBCAckPT;
    SMOD_ManageStatusPTWaitEmerStopRevoc    Static_ManageStatusPTWaitEmerStopRevoc;
    SMOD_CheckCommonStatusTransition        Static_CheckCommonStatusTransition;
    SMOD_ManageTripActivation               Static_ManageTripActivation;
    SMOD_ManageModeChangeBrake              Static_ManageModeChangeBrake;
    SMOD_ManageModeProf                     Static_ManageModeProf;
    SMOD_ManageLevelTransition              Static_ManageLevelTransition;
    SMOD_ManageReversingArea                Static_ManageReversingArea;
    SMOD_LevelTrOnModeChange                Static_LevelTrOnModeChange;             ///< Level transition that will be applied on mode change (SRS300 4.4.8.1.5)
    SMOD_ManageLevelInhibitionSB            Static_ManageLevelInhibitionSB;         ///< Manage SB intervention when no level transition can be done (SRS300 5.10.2.7.2)
    SMOD_ManageReconnectionRqst             Static_ManageReconnectionRqst;
    bool                                    bStopSHOnDeskOpen;

} SModeStatic;

//------------- static data used by odometer interface ----------------------

typedef struct SODO_UpdateLocation
{
    bool            bFirstCall;
    eActiveCabin    ActiveCabin;

    double          dShiftTrain;        /// In case train is late thanks to odometer, we shift positively the train position.
    double          dWaitUntilPos;      /// In case train is in advance thanks to odometer, we wait for odo.
    bool            bWaitUntilPos;      /// Indicate if we wait for odo.

    double          dLastLRBGDist;      /// Used to compute dLRBGTravelledDistance in location data.
    SBG_Id          dCurrentBGId;      /// Used to compute dLRBGTravelledDistance in location data.

} SODO_UpdateLocation;

typedef struct SOdoStatic
{
    SODO_UpdateLocation     Static_UpdateLocation;
    STrackCondStation       Static_STrackCondStation;

} SOdoStatic;


//------------- static data used by radio interface ----------------------

/// Enumeration for radio block centre transition status
typedef enum eRBCTransitionState
{
    RBCTRANS_NOTRANSITION = 0,
    RBCTRANS_WAITING_TERMINATE_SESSION_RBC1_SENDING_POSITION_REPORTS,
    RBCTRANS_WAITING_ESTABLISH_SESSION_RBC2

} eRBCTransitionState;

typedef struct SRAD_ManageSendingTerminationMsg
{
    int32_t lSessionId;
    t_time   LastSendingTime;
    int32_t lSendingNb;

} SRAD_ManageSendingTerminationMsg;


typedef struct SRAD_SuperviseRadioLink
{
    bool        bSuperviseRadioLink;
    eReaction   SupervisingRadioLinkTimeoutReaction;
    bool        bSupervisingRadioLinkTimeout;
    bool        bMAReleased;
    bool        bReconnectionRequested;
    bool        bWasInRadioHole;
    eLevel      PreviousLevel;
    t_time      dRadioLinkTimeOutTime;

} SRAD_SuperviseRadioLink;

typedef struct SRAD_RepeatPosRepAtExitRBCArea
{
    bool    bRepeatPR;
    int32_t lSessionId;
    t_time  dLastPositionReportSentTime;
    int32_t lCountNumberOfPositionReportsSent;

} SRAD_RepeatPosRepAtExitRBCArea;

typedef struct SRAD_ManageRBCTrWith1Session
{
    eRBCTransitionState RBCTransState;
    bool                bFirstPositionReportSent;
    bool                bSecondPositionReportSent;
    t_time              dEstablishRBC2SessionTimer;

} SRAD_ManageRBCTrWith1Session;


typedef struct SRAD_ManageRBCTrWith2Session
{
    bool    bConnectionRequested;
    bool    bFirstPositionReportSent;
    bool    bSecondPositionReportSent;

} SRAD_ManageRBCTrWith2Session;


typedef struct SRAD_ManageRBCTransition
{
    eTrainMode      TrainMode;
    SRBCTransition  RBCTransitionInfo;
    int32_t lNewStopLocCnt;

} SRAD_ManageRBCTransition;


typedef struct SRAD_ManageSessionOrder
{
    SRBCConnectOrder    RBCSessionOrder;
    eTrainMode          PreviousTrainMode;
    eLevel              PreviousLevel;

} SRAD_ManageSessionOrder;


typedef struct SRAD_ManageErrorReportInSleeping
{
    eM_Error    CurrentError;

} SRAD_ManageErrorReportInSleeping;


typedef struct SRAD_Create_Radio_Mess_SRS_class1
{
    uint32_t ulLastTimeStamp;

} SRAD_Create_Radio_Mess_SRS_class1;

typedef struct SRAD_RadioNetworkRegistration
{
    eTrainMode      TrainMode;
    bool bRegistrationFromPowerOff;

} SRAD_RadioNetworkRegistration;


typedef struct SRAD_ManageTrainDataSending
{
    uint32_t ulNumTrainDataSent;         ///< Indication about the number of tries to send train data (because of absence of an ack)
    t_time          dTrainDataSendingTime;      ///< Time when last train data have been sent
    bool            bTrainDataResend;           ///< Indication that train data need to be resent (due to loss of radio safe connection)
} SRAD_ManageTrainDataSending;

typedef struct SRadioStatic
{
    bool                                bRadioHole;                                     ///< Global variable indicating if currently in radio hole
    STracksideMessage                   TransitionBuffer[MAX_TRANSITION_BUFFER_SIZE];   ///< Global variable containing buffer for radio messages (used during RBC transition)

    SRAD_SuperviseRadioLink             Static_SuperviseRadioLink;
    SRAD_RepeatPosRepAtExitRBCArea      Static_RepeatPosRepAtExitRBCArea;
    SRAD_ManageRBCTrWith1Session        Static_ManageRBCTrWith1Session;
    SRAD_ManageRBCTrWith2Session        Static_ManageRBCTrWith2Session;
    SRAD_ManageRBCTransition            Static_ManageRBCTransition;
    SRAD_ManageSessionOrder             Static_ManageSessionOrder;
    SRAD_ManageErrorReportInSleeping    Static_ManageErrorReportInSleeping;
    SRAD_ManageSendingTerminationMsg    Static_ManageSendingTerminationMsg;
    SRAD_Create_Radio_Mess_SRS_class1   Static_Create_Radio_Mess_SRS_class1;
    SRAD_RadioNetworkRegistration       Static_RadioNetworkRegistration;
    SRAD_ManageTrainDataSending         Static_ManageTrainDataSending[MAX_RADIO_EQUIPMENT];
} SRadioStatic;


//------------- static data used by RIM interface ----------------------

typedef struct SRIM_ConvertPrim
{
    eRadSafeConnectStatus    ConnectStatus;          ///< Global variable containing radio safe connection status (train side)
    eRadSafeConnectStatus    RBCConnectStatus;       ///< Global variable containing radio safe connection status (RBC side)
    uint8_t                       uszSaCEPID[4];          ///< Global variable containing SaCEPID value
    uint8_t                       uszNetworkAddress[18];  ///< Global variable containing network address (connection parameter)
    uint8_t                       uszCalledETCSId[4];     ///< Global variable containing called ETCS identifier (connection parameter)
    uint8_t                       uszCallingETCSId[4];    ///< Global variable containing calling ETCS identifier (connection parameter)
    uint8_t                       uszQOS[2];              ///< Global variable containing Quality of service value
    uint32_t uDefSaCEPID;            ///< Global variable containing SaCEPID value for connection request

} SRIM_ConvertPrim;


typedef struct SRimStatic
{
    SRIM_ConvertPrim                    Static_ConvertPrim;

} SRimStatic;

//------------- static data used by speed controller ----------------------

/// Structure containing intervention request
typedef struct SInterventionRequest
{
    bool         EB;        ///< Emergency brake intervention
    bool         SB;        ///< Service brake intervention
    bool         CutOff;    ///< Traction cut off intervention
    bool         Warn;      ///< Warning indication intervention
    bool         Indic;     ///< Indication intervention

} SInterventionRequest;


typedef struct SCTRL_ApplyBrakeChange
{
    SInterventionRequest LastBrakeStatus;
    t_time               dLastSBRequestTime;

} SCTRL_ApplyBrakeChange;


typedef struct SCTRL_ManageSRArea
{
    SSRArea SRAreaInfo;

} SCTRL_ManageSRArea;


typedef struct SCTRL_ManageSHArea
{
    int32_t lNewStopLocCnt;
    SShuntArea  SHAreaInfo;

} SCTRL_ManageSHArea;


typedef struct SCTRL_ReverseMvtProtection
{
    t_distance  dStandStillLocation;
    bool        bIntervention;
    bool        bAckReceived;

} SCTRL_ReverseMvtProtection;


typedef struct SCTRL_RollAwayProtection
{
    t_distance  dStandStillLocation;
    bool        bInterventionDone;
    bool        bAckReceived;

} SCTRL_RollAwayProtection;


typedef struct SCTRL_StandstillSupervision
{
    t_distance      dStandStillLocation;
    bool            bInterventionDone;
    bool            bAckReceived;
    eActiveCabin    LastCabinStatus;

} SCTRL_StandstillSupervision;


typedef struct SCTRL_ControlSpeed
{
    bool    bTripRequestSent;
    bool    bEB;
    bool    bSB;
    bool    bWarn;
    bool    bTCO;
    bool    bIndic;
    bool    bMsgSentToDMI;
    bool    bRVdistanceExceeded;

} SCTRL_ControlSpeed;


typedef struct SCTRL_ManageBrakingRequest
{
    int32_t lEB_Required;       ///< Requirement of EB , release at standstill & acknowledgement
    int32_t lSB_Required;       ///< Requirement of SB , release if acknowledgement

} SCTRL_ManageBrakingRequest;


typedef struct SCTRL_GetPermSpeedData
{
    eSpeedMonitoringType    LastSpeedMonitoringType;
    int32_t lLastValue;
    int32_t lLastValue2;

} SCTRL_GetPermSpeedData;


typedef struct SCtrlStatic
{
    SInterventionRequest            InterventionReq;            ///< Global variable containing containing the next intervention request
    eTrainMode                      PreviousMode;
    eActiveCabin                    LastCabinStatus;

    SCTRL_ApplyBrakeChange          Static_ApplyBrakeChange;
    SCTRL_ManageSRArea              Static_ManageSRArea;
    SCTRL_ManageSHArea              Static_ManageSHArea;
    SCTRL_ReverseMvtProtection      Static_ReverseMvtProtection;
    SCTRL_RollAwayProtection        Static_RollAwayProtection;
    SCTRL_StandstillSupervision     Static_StandstillSupervision;
    SCTRL_ControlSpeed              Static_ControlSpeed;
    SCTRL_ManageBrakingRequest      Static_ManageBrakingRequest;
    SCTRL_GetPermSpeedData          Static_GetPermSpeedData;

} SCtrlStatic;

/// structure containing all static data used in EVC
typedef struct SEVCStaticData
{
    /// static data used by Data manager
    SBalStatic BalStatic;

    ///static data used by curve computer
    SCompStatic CompStatic;

    ///static data used by Data manager
    SDatStatic DatStatic;

    ///static data used by DMI interface manager
    SDmiStatic DmiStatic;

    ///static data used by mode manager
    SModeStatic ModeStatic;

    ///static data used by odometer interface
    SOdoStatic OdoStatic;

    ///static data used by radio interface
    SRadioStatic RadioStatic;

    ///static data used by RIM interface
    SRimStatic RimStatic;

    ///static data used by speed controller
    SCtrlStatic CtrlStatic;

} SEVCStaticData;


//===============================    Shared data    =========================================================

/// Macro to get pointer on current curve data set (null pointer if no data are available)
#define CURVEDATA_CURRENT \
    ((pShared->ShSupervisionData.CurveSetStatus==CURVEDATA_INVALID)?NULL:((pShared->ShSupervisionData.CurveSetStatus==CURVEDATA_VALID_SET1)?&pShared->ShCurveSet1:&pShared->ShCurveSet2) )

/// Macro to get string of current curve data set ("none" if no data are available)
#define CURVEDATA_CURRENT_STR \
    ((pShared->ShSupervisionData.CurveSetStatus==CURVEDATA_INVALID)?"None":((pShared->ShSupervisionData.CurveSetStatus==CURVEDATA_VALID_SET1)?"Curve Set 1":"Curve Set 2") )

/// Macro to get pointer on current track description data set (null pointer if no data are available)
#define TRACKDATA_CURRENT \
    ((pShared->ShSupervisionData.CurveSetStatus==CURVEDATA_INVALID)?NULL:((pShared->ShSupervisionData.CurveSetStatus==CURVEDATA_VALID_SET1)?&pShared->ShTemporaryDataBig.TrackDesc1:&pShared->ShTemporaryDataBig.TrackDesc2) )

/// Structure containing pointer to structures stored in shared memory
typedef struct SShared_data
{
    SETCS_IO            ETCS_IO             ;   ///< Pointer to structure containing the main EVC data
    SConfig             Config              ;   ///< Pointer to structure containing the EVC configuration

    SIntervCurves       ShCurveSet1         ;   ///< Pointer to first  supervision curve set (can be used while second set is being computed)
    SIntervCurves       ShCurveSet2         ;   ///< Pointer to second supervision curve set (can be used while first  set is being computed)

    SSupervisionData    ShSupervisionData   ;   ///< Pointer to structure containing supervision data

    STemporaryDataBig   ShTemporaryDataBig  ;   ///< Pointer to structure containing all temporary data

    SEVCStaticData      EVCStaticData       ;   ///< Pointer to structure containing all static data used in EVC

} SShared_data;

/// Macro to get absolute distance according to given reference location and relative distance
#define ABSOLUTE_DISTANCE(_refloc,_rloc)    (_rloc + _refloc)

/// Macro to get relative distance according to given reference location and absolute distance
#define RELATIVE_DISTANCE(_refloc,_aloc)    (_aloc - _refloc)


//=======================    global variable    =======================

#ifdef ETCS_TYPE_MAIN

SShared_data    SharedData;                         ///< Shared memory data
SShared_data*   pShared = &SharedData;              ///< Pointer to shared memory

#else

extern	SShared_data    SharedData;                 ///< Shared memory data
extern	SShared_data*   pShared;                    ///< Pointer to shared memory

#endif


#endif //_ETCS_TYPES_H

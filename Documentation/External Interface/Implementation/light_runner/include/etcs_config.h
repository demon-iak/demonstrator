// *************************************************************************************************

/// @file   etcs_config.h
/// @brief  Declaration of macro and types for management of EuroCab configuration.
/// Project     : EVC Simulator -
/// Module      : EVC -
/// Copyright   : European Rail Software Applications (c) 2008
// *************************************************************************************************

#ifndef _ETCS_CONFIG_H
#define _ETCS_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#else
 #include <stdbool.h>
#endif

// -------------------------------------------------------------------------------------------------
// define
// -------------------------------------------------------------------------------------------------

#define EVC_NAME            "EuroCab Simulator"                                         ///< EVC name string
#define EVC_COMPATIBILITY   "for SRS Class 1 v3.3.0"                                    ///< EVC SRS compatibility string
#define EVC_COPYRIGHT       "(C) Copyright 2013 ERSA - All Rights Reserved."            ///< EVC copyright string

#define EVC_REFERENCE       "ERTMS/ETCS - Class 1, SRS: v3.3.0\n"                       ///< EVC reference string
#define EVC_SRS_MAJOR_CODE  3
#define EVC_SRS_MINOR_CODE  3
#define EVC_SRS_UPDATE_CODE 0

// version for DMI communication
#define DMI_PROTOCOL_VERSION_MAJOR_CODE 4 ///< major version of supported DMI communication protocol
#define DMI_PROTOCOL_VERSION_MINOR_CODE 3 ///< minor version of supported DMI communication protocol

#define DMI_IMPL_MAJOR_CODE             0                                               ///< DMI communication major version number
#define DMI_IMPL_MINOR_CODE             0                                               ///< DMI communication minor version number

#ifdef __linux__
 #define LOG_DIR "./data/log"                                                           ///< Directory to store log files
#elif WIN32
 #define LOG_DIR "data\\log"                                                            ///< Directory to store log files
#endif

/// name of pipe for EVC --> DMI communication
#define DMI_OUT_PIPE_NAME "opsimu1"

/// name of pipe for DMI --> EVC communication
#define DMI_IN_PIPE_NAME "opsimu2"

// -------------------------------------------------------------------------------------------------
// type definition
// -------------------------------------------------------------------------------------------------

// Enumeration for list of configurations
typedef enum eConfigData
{
    CFG_RADIO_INTERNAL_TIME_STAMP, ///< Internal time stamping (not use T_TRAIN)

    CFG_USE_JRU,            ///< Use JRU (generates JRU data file)
    CFG_RECORD_TO_CSV_FILE, ///< Record supervision data to CSV file

    CFG_LOOP_WITH_SSCODE,   ///< Loop are received with spread spectrum code
    CFG_BAL_WITH_ODO_STAMP, ///< Balise are received with odo stamp

    CFG_LOCAL_TIME_STAMP,                 ///< Request local time stamp otherwise GMT time in log & JRU record
    CFG_RECORDER_LOG_ADD_FULL_TIME_STAMP, ///< add time stamp in EuroCabLog.dat like 2009-05-29/08:15:21.29

    CONFIG_SIZE
} eConfigData;

/// Macro to trace EVC configuration to log file
#define TRACE_EUROCAB_CONFIG \
    { \
        Trace( "\n---- EUROCAB CONFIGURATION ----\n" ); \
        Trace( "CFG_RADIO_INTERNAL_TIME_STAMP          = %x\n", IsConfigSet( CFG_RADIO_INTERNAL_TIME_STAMP ) ); \
        Trace( "CFG_USE_JRU                            = %x\n", IsConfigSet( CFG_USE_JRU ) ); \
        Trace( "CFG_RECORD_TO_CSV_FILE                 = %x\n", IsConfigSet( CFG_RECORD_TO_CSV_FILE ) ); \
        Trace( "CFG_LOOP_WITH_SSCODE                   = %x\n", IsConfigSet( CFG_LOOP_WITH_SSCODE ) ); \
        Trace( "CFG_BAL_WITH_ODO_STAMP                 = %x\n", IsConfigSet( CFG_BAL_WITH_ODO_STAMP ) ); \
        Trace( "CFG_LOCAL_TIME_STAMP                   = %x\n", IsConfigSet( CFG_LOCAL_TIME_STAMP ) ); \
        Trace( "CFG_RECORDER_LOG_ADD_FULL_TIME_STAMP   = %x\n", IsConfigSet( CFG_RECORDER_LOG_ADD_FULL_TIME_STAMP ) ); \
    }

// -------------------------------------------------------------------------------------------------
// function prototype
// -------------------------------------------------------------------------------------------------
void SetConfig( eConfigData Config );

bool IsConfigSet( eConfigData Config );

void ResetConfig( eConfigData Config );

#ifdef __cplusplus
}
#endif
#endif // _ETCS_CONFIG_H

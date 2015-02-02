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
 File    : dmi_com.h

*******************************************************************************/

#ifndef _DMI_COM_H
#define _DMI_COM_H

//------------------------------ include ---------------------------------------
#include <stdint.h>
#include <vector>

#include "dmi_protocol.h"

//-- types
class CSerial;

#define DMI_BUFFER_SIZE                 2048    ///< DMI message buffer size
#define DMI_COM_PROTOCOL_MAJOR_CODE     4       ///< DMI communication major version number
#define DMI_COM_PROTOCOL_MINOR_CODE     3       ///< DMI communication minor version number

typedef struct SDmiComTextMsg
{
    uint32_t ulId;
    bool bAck;
    SDMI_TextMsg data;
} SDmiComTextMsg;

typedef struct SDmiComIcon
{
    uint32_t ulId;
    bool bAck;
    uint32_t lIconNameLength;
    char szIconName[8];
    int32_t lIconGroup;
    int32_t lIconRank;
    uint32_t lAreaNameLength;
    char szAreaName[8];
    int32_t lAreaGroup;
    int32_t lAreaRank;

}SDmiComIcon;

typedef struct SDmiComDynamic
{
    uint32_t DMI_T_CLOCK;
    uint16_t DMI_V_TRAIN:10;
    char DMI_X_VTRAIN_DIGITS[3];
    uint32_t DMI_O_TRAIN;
    uint32_t DMI_O_BRAKETARGET;
    char DMI_X_OBRAKETARGET_DIGITS[5];
    uint16_t DMI_V_TARGET:10;
    uint16_t DMI_V_PERMITTED:10;
    uint16_t DMI_V_RELEASE:10;
    uint32_t DMI_O_BCSP;
    uint16_t DMI_V_INTERVENTION:10;
    uint8_t DMI_M_MODE:4;
    uint8_t DMI_M_LEVEL:3;
    uint16_t DMI_NID_C:10;
    bool DMI_NID_C_UNKNOWN:1;
    uint8_t DMI_M_WARNING:4;
    uint8_t DMI_M_SUPSTATUS:4;
    uint32_t DMI_O_LOA;
    uint16_t DMI_V_LOA:10;
    uint32_t DMI_O_KP_BALISE_TRACK_KILOMETER;
    uint32_t DMI_O_KP_DIST_TO_BALISE;
    uint8_t DMI_M_KP_FLAG:2;
    uint32_t DMI_O_DIST_TO_TSA;
} SDmiComDynamic;

typedef enum eDmiAction
{
    DMI_DO_TRAIN_RUNNING_NUMBER,
    DMI_DO_TRAIN_DATA,              // Default train data
    DMI_DO_TRAIN_DATA_FREIGHT,      // Train data with freight (FP1) train category selected
    DMI_DO_SR_DATA,
    DMI_DO_LEVEL_0,
    DMI_DO_LEVEL_1,
    DMI_DO_LEVEL_2,
    DMI_DO_LEVEL_3,
    DMI_DO_START_OF_MISSION = 9,
    DMI_DO_SLIPPERY_TRACK,
    DMI_DO_NON_SLIPPERY_TRACK,
    DMI_DO_NON_LEADING,
    DMI_DO_NON_LEADING_EXIT,    // only B2
    DMI_DO_OVERRIDE_EOA,
    DMI_DO_OVERRIDE_RS,         // only B2
    DMI_DO_SHUNTING,
    DMI_DO_SHUNTING_EXIT,
    DMI_DO_TAF_ACK,
    DMI_DO_ACK_MODE_OR_LEVEL,
    DMI_DO_ACK_TEXT_MSG,
    DMI_DO_ACK_BRAKE,
    DMI_DO_CONFIRM_INTEGRITY,
    DMI_DO_USE_SHORT_NUMBER,        // only B3
    DMI_DO_CONTACT_LAST_RBC,        // only B3
    DMI_DO_SELECT_RADIO_NETWORK1,   // only B3
    DMI_DO_ENTER_VBC,               // only B3
    DMI_DO_REMOVE_VBC,
    //DMI_DO_ENTER_NETWORK, ?
    DMI_DO_ENTER_RBC_DATA,          // only B3
    DMI_DO_MAINTAIN_SHUNTING,       // only B3
    DMI_DO_ISOLATION,               // only B3
    DMI_DO_CHANGE_TRAIN_LENGTH,
    DMI_DO_NUM

} eDmiAction;

typedef enum eDmiUpdateData
{
    DMI_UPDATE_DISPLAY_CONTROL      = 0x0001,
    DMI_UPDATE_ICON                 = 0x0002,
    DMI_UPDATE_TEXT_MESSAGE         = 0x0004,
    DMI_UPDATE_MENU                 = 0x0008,
    DMI_UPDATE_TAF_DISPLAY          = 0x0010,
    DMI_UPDATE_TRACK_DESCRIPTION    = 0x0020,
    DMI_UPDATE_LEVEL_DATA           = 0x0040,

    DMI_UPDATE_MASK                 = 0xFFFF

}eDmiUpdateData;


//--------------------------- class definition -----------------------------
/// class for exchange of DMI data with EVC
class

#ifdef DLL_EXPORT
DLL_EXPORT
#endif

CDMI_com : public CBridge
{
    // Simulate a device with no functions
    class CDeviceUnused;

public:

    /// Constructor to be used when no DMI communication is used (or simplified DMI)
    CDMI_com();

    /// Constructor to be used when DMI named pipe communication is used
    CDMI_com(   const char* szInPipe,   ///< [in]: Pipe in (DMI --> EVC)
                const char* szOutPipe   ///< [in]: Pipe out(DMI <-- EVC)
                );

    /// Constructor to be used when DMI serial communication is used
    CDMI_com(   const char *szDevice,   ///< [in] : TTY device
                int32_t iBaudRate,  ///< [in] : baud rate
                int32_t iDataBits,  ///< [in] : data bits number
                char        cParity,    ///< [in] : parity
                int32_t iStopBits   ///< [in] : stop bits number
                );


    /// Destructor
    virtual ~CDMI_com( );


    /// Get the class size (for linking consistency check in use of FULL/LIGHT EVC version)
    /// @return: class size in bytes
    int32_t Get_Class_Size();


    /// Retrieve available menu mask, and set it to 0 to get a new value for next call
    /// @return mask indicating the status of GUI update
    int32_t getAndResetUpdateMask();

    // Actions by driver

    /// Send a message to EVC to simulate a driver action on the DMI
    /// @return true if this action is allowed (button available)
    bool setAction( eDmiAction      action,                 ///< [in] Type of driver action on DMI
                    int32_t param = -1,             ///< [in] optional parameter: \n
                    ///< - driver id for DMI_DO_DRIVER_ID \n
                    ///< - train running number for DMI_DO_TRAIN_RUNNING_NUMBER \n
                    ///< - Level 1 Id for DMI_DO_LEVEL_1 \n
                    ///< - message id for DMI_DO_ACK_TEXT_MSG \n
                    ///< - VBC code for DMI_DO_ENTER_VBC and DMI_DO_REMOVE_VBC
                    SMMISRData *    pStaffRespData = NULL,  ///< [in] Used for DMI_DO_SR_DATA action (optional)
                    SMMIRBCData *   pRbcData = NULL ,       ///< [in] Used for DMI_DO_LEVEL_2 and DMI_DO_LEVEL_3 action (optional)
                    SMMITrainData * pTrainData = NULL       ///< [in] Used for DMI_DO_TRAIN_DATA action (optional)
            );

    /// Send a message to EVC to simulate entering driver id on the DMI
    /// @return true if this action is allowed (button available)
    bool doDriverIdAction(const char * const szDriverId );

    /// Send a message to EVC to simulate entering RBC data on the DMI
    /// @return true if this action is allowed (button available)
    bool doRBCDataAction( int32_t lRBCId, const char* const szPhoneNr, const char* const szNetworkId );

    /// Indicate if a text message to acknowledge is displayed
    /// @return true if a message to acknowledge is displayed
    bool IsTextMessageAckDisplayed() const;

    /// Indicate if a mode or level icon to acknowledge is displayed
    /// @return true if a mode or level icon to acknowledge is displayed
    bool IsModeOrLevelAckDisplayed() const;

    /// Indicate if a brake icon to acknowledge is displayed
    /// @return true if a brake icon to acknowledge is displayed
    bool IsBrakeAckDisplayed() const;

    /// Test if an icon is displayed
    /// @return true if icon is displayed
    bool IsIconDisplayed( eDMI_Icon icon    ///< [in] : icon identifier
                          );
    /// Test if an icon to acknowledge is displayed
    /// @return true if displayed
    bool AckIconDisplayed( eDMI_Icon icon   ///< [in] : icon identifier
                           );

    /// test is cabin is opened
    /// @return true if cabin is open
    bool getCabinStatus() const { return m_bCabinOpen; }

    /// test if track ahead free is displayed
    /// @return true if TAF is displayed
    bool getTafStatus() const { return m_bTAFOpen; }

    /// get the mask of available menu/button
    /// @return the mask of available menu/button
    uint32_t getAvailableMenu() const { return m_ulMenuMask; }

    /// get the current RBC data: RBC ID, phone nb, network id
    /// @return the RBC data
    SMMIRBCData getRbcData()const { return m_rbcData; }

    /// get list of displayed icons
    void getIconList ( std::vector<SDmiComIcon> & icons ) const;

    /// get list of displayed text messages
    void getTextMessageList( std::vector<SDmiComTextMsg> & texts );

    /// get list of active levels
    void getLevelList( std::vector<SLevel> & levels );

    /// get dynamic packet
    const SDmiComDynamic& getDynamicPacket() const { return m_Dynamic; }

protected:

    ///  initialisation of members and start of bridge
    void initCommonDataAndStart();

    /// Decod a variable of a given length in the message
    /// @return Variable value
    uint32_t decod( SDecodMsg * data, ///< [in] binary message and offset
                    int32_t lLength      ///< [in] varibale size in bits
                    );
    int32_t readDeviceA(void * data , int32_t iSize );

    /// Decod an EVC message send to DMI
    void decodPaquet( SDecodMsg * pDecodMsg ///< [in] : message from EVC
                      );

    /// Send a message to DMI
    /// @return true on success
    bool sendDMIMessage(int32_t            iMsgLengthBits, ///< [in] : Length of message
                        uint8_t * uszMsg          ///< [in] : Message data
                        );
    /// Add new value to the update mask
    void setUpdateMask( eDmiUpdateData dataToUpdate ///< [in] Type of menu that has been updated
                        );
    /// Acknowledge a specific text message
    /// @return true if a text message is acknowledged
    bool ackTextMessage( uint32_t lId  ///< [in] : text identifier
                         );
    /// Acknowledge the last text message
    /// @return true if there is a text message to ack
    bool ackLastTextMessage();
    /// Do a mode or level ack
    /// @return true if a mode or level icon is acknowledged
    bool ackModeOrLevel();
    /// Do a brake ack
    /// @return true if a brake icon is acknowledged
    bool ackBrake();

    /// Decod packet dynamic
    void decod_dynamic();
    /// Decod packet EVC version
    void decod_EvcVersion();
    /// Decod packet menu avaibility
    void decod_menuAvailability();
    /// Decod packet EVC request
    void decod_EvcRequest();
    /// Decod packet track description
    void decod_trackDescription();
    /// Decod packet text message
    void decod_textMessage();
    /// Decod packet default train data
    void decod_defaultTrainData();
    /// Decod packet coded train data
    void decod_codedTrainData();
    /// Decod packet SR data
    void decod_staffRespData();
    /// Decod packet driver Id
    void decod_driverId();
    /// Decod packet train running number
    void decod_trainRunningNumber();
    /// Decod packet adhesion
    void decod_adhesion();
    /// Decod packet operated system version
    void decod_operatedSystemVersion();
    /// Decod packet icons
    void decod_icons();
    /// Decod packet RBC data
    void decod_RbcData();
    /// Decod packet default level
    void decod_defaultLevel();
    /// Decod packet level list
    void decod_levelList();
    /// Decod packet display control
    void decod_displayControl();

    /// Encod packet driver request
    void encod_driverRequest( int32_t request );
    /// Encod packet level selected
    void encod_levelSelected( eLevel level, 0 );
    /// Encod packet RBC data
    void encod_RbcData( int32_t lRBCId, uint8_t * uszRadioNumber, uint8_t * uszNetworkId );
    /// Encod packet driver id
    void encod_driverId( char *szDriverId );
    /// Encod packet train running number
    void encod_trainRunningNumber( uint32_t ulOperationalNb );
    /// Encod packet adhesion factor
    void encod_adhesionFactor( int32_t lAdhesionFactor );
    /// Encod packet SR data
    void encod_StaffRespData( int32_t lSRDistance, int32_t lSRSpeed );
    /// Encod packet text message ack
    void encod_textMessageAck( uint32_t lMessageId );
    /// Encod packet train data
    void encod_trainData( SMMITrainData * ptrainData );
    /// Encod packet train data ack
    void encod_trainDataAck( bool bTrainDataAck );
    /// Encod packet DMI identifier
    void encod_DmiIdentifier();
    /// Encod packet icon ack
    void encod_iconAck( uint32_t ulIconId ///< [in] Icon identifier
                        );
    /// Encode VBC data (only B3)
    void encod_Vbc( uint32_t ulVbcId,    ///< [in] VBC id
                    bool            bRemove     ///< [in] Set or remove VBC
                    );

    /// Acknowlege VBC data (only B3)
    void encod_VbcAck( bool bRemove ///< [in] Set or remove VBC
                       );

private:
    SDecodMsg *         m_pDecodMsg;
    bool                m_bAutoDmiResponse;      // Response to EVC message. Used when no other DMI is plugged in.
    uint8_t       m_data[DMI_BUFFER_SIZE];

    SMMIDriverId        m_driverId;
    SMMIOperationalNb   m_operationnalNb;
    SMMILevel           m_levelSelected;
    SMMIRBCData         m_rbcData;
    SMMITrainData       m_trainData;
    SMMISRData          m_staffRespData;
    SMMIAdhesion        m_adhesion;

    // Flag for data request to EVC
    bool m_bWaitTrainDataFromEvc;
    bool m_bWaitSRDataFromEvc;
    bool m_bWaitNetworkListFromEvc;

    // Special case: when user enter RBC data, ignore those from EVC
    bool m_bIgnoreRBCDataFromEvc;

    uint32_t m_ulMenuMask;
    uint32_t m_ulEvcRequest;
    std::vector<SDmiComTextMsg> m_messages;
    std::vector<SDmiComIcon> m_icons;
    std::vector<SLevel> m_levels;
    SDmiComDynamic m_Dynamic;
    bool m_bCabinOpen;
    bool m_bTAFOpen;

    uint32_t m_ulUpdateMask;               // Contains which menu need to be updated

    CSerial * m_pSerial;                // For MsgQueue to serial

    // Thread locker
    pthread_mutex_t m_mutex;
};

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file IpmiNetFnAppDefinitions.h
    Definitions required for Application commands.

**/

//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2009 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  IpmiNetFnAppDefinitions.h
  
Abstract:
  The file has all the definitions required for App commands
--*/

#ifndef _IPMINETFNAPPDEFINITIONS_H_
#define _IPMINETFNAPPDEFINITIONS_H_


//
// LUN definition
//
#ifndef BMC_LUN 
    #define BMC_LUN     0x00
#endif 

//
// Net function definition for App command
//
#define EFI_SM_NETFN_APP  0x06

/*----------------------------------------------------------------------------------------
    Definitions for Get Device ID command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_DEVICE_ID 0x1

//
//  Constants and Structure definitions for "Get Device ID" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8   DeviceId;
  UINT8   DeviceRevision : 4;
  UINT8   Reserved : 3;
  UINT8   DeviceSdr : 1;
  UINT8   MajorFirmwareRev : 7;
  UINT8   UpdateMode : 1;
  UINT8   MinorFirmwareRev;
  UINT8   SpecificationVersion;
  UINT8   SensorDeviceSupport : 1;
  UINT8   SdrRepositorySupport : 1;
  UINT8   SelDeviceSupport : 1;
  UINT8   FruInventorySupport : 1;
  UINT8   IPMBMessageReceiver : 1;
  UINT8   IPMBMessageGenerator : 1;
  UINT8   BridgeSupport : 1;
  UINT8   ChassisSupport : 1;
  UINT8   ManufacturerId[3];
  UINT16  ProductId;
  UINT32  AuxFirmwareRevInfo;
} EFI_SM_CTRL_INFO;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Cold Reset command
----------------------------------------------------------------------------------------*/
#define EFI_APP_COLD_RESET  0x2

//
//  Constants and Structure definitions for "Cold Reset" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Warm Reset command
----------------------------------------------------------------------------------------*/
#define EFI_APP_WARM_RESET  0x3

//
//  Constants and Structure definitions for "Warm Reset" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Self Results command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_SELFTEST_RESULTS  0x4

//
//  Constants and Structure definitions for "Get Self Results" command to follow here
//
#define EFI_APP_SELFTEST_RESERVED             0xFF
#define EFI_APP_SELFTEST_NO_ERROR             0x55
#define EFI_APP_SELFTEST_NOT_IMPLEMENTED      0x56
#define EFI_APP_SELFTEST_ERROR                0x57
#define EFI_APP_SELFTEST_FATAL_HW_ERROR       0x58
#define EFI_APP_SELFTEST_INACCESSIBLE_SEL     0x80
#define EFI_APP_SELFTEST_INACCESSIBLE_SDR     0x40
#define EFI_APP_SELFTEST_INACCESSIBLE_FRU     0x20
#define EFI_APP_SELFTEST_IPMB_SIGNAL_FAIL     0x10
#define EFI_APP_SELFTEST_SDR_REPOSITORY_EMPTY 0x08
#define EFI_APP_SELFTEST_FRU_CORRUPT          0x04
#define EFI_APP_SELFTEST_FW_BOOTBLOCK_CORRUPT 0x02
#define EFI_APP_SELFTEST_FW_CORRUPT           0x01

typedef struct {
  UINT8   Status;
  UINT8   ErrorBitSet;
} EFI_GET_SELF_TEST_STATUS;

/*----------------------------------------------------------------------------------------
    Definitions for Manufacturing Test ON command
----------------------------------------------------------------------------------------*/
#define EFI_APP_MANUFACTURING_TEST_ON 0x5

//
//  Constants and Structure definitions for "Manufacturing Test ON" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set ACPI Power State command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_ACPI_POWERSTATE 0x6

//
//  Constants and Structure definitions for "Set ACPI Power State" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 SystemPowerState : 7;
  UINT8 SystemStateChange : 1;
  UINT8 DevicePowerState : 7;
  UINT8 DeviceStateChange : 1;
} EFI_ACPI_POWER_STATE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get ACPI Power State command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_ACPI_POWERSTATE 0x7

//
//  Constants and Structure definitions for "Get ACPI Power State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Device GUID command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_DEVICE_GUID 0x8

//
//  Constants and Structure definitions for "Get Device GUID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Reset WatchDog Timer command
----------------------------------------------------------------------------------------*/
#define EFI_APP_RESET_WATCHDOG_TIMER  0x22

//
//  Constants and Structure definitions for "Reset WatchDog Timer" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 TimerUse : 3;
  UINT8 Reserved : 3;
  UINT8 TimerRunning : 1;
  UINT8 TimerUseExpirationFlagLog : 1;
} EFI_BMC_IPMI_WATCHDOG_TIMER_USE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set WatchDog Timer command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_WATCHDOG_TIMER  0x24

//
//  Constants and Structure definitions for Timer use expiration flags
//
#pragma pack(1)
typedef struct {
  UINT8   ExpirationFlagReserved1 : 1;
  UINT8   Frb2ExpirationFlag : 1;
  UINT8   PostExpirationFlag : 1;
  UINT8   OsLoadExpirationFlag : 1;
  UINT8   SmsOsExpirationFlag : 1;
  UINT8   OemExpirationFlag : 1;
  UINT8   ExpirationFlagReserved2 : 1;
  UINT8   ExpirationFlagReserved3 : 1;
} EFI_BMC_IPMI_WATCHDOG_TIMER_EXPIRATION_FLAGS;
#pragma pack()

//
//  Constants and Structure definitions for Timer Actions
//
#pragma pack(1)
typedef struct {
  UINT8 TimeoutAction : 3;
  UINT8 Reserved1 : 1;
  UINT8 PreTimeoutIntr : 3;
  UINT8 Reserved2 : 1;
} EFI_BMC_IPMI_WATCHDOG_TIMER_ACTIONS;
#pragma pack()

//
//  Constants and Structure definitions for "Set WatchDog Timer" command to follow here
//
#pragma pack(1)
typedef struct {
  EFI_BMC_IPMI_WATCHDOG_TIMER_USE                   TimerUse;
  EFI_BMC_IPMI_WATCHDOG_TIMER_ACTIONS               TimerActions;
  UINT8                                             PretimeoutInterval;
  EFI_BMC_IPMI_WATCHDOG_TIMER_EXPIRATION_FLAGS      TimerUseExpirationFlagsClear;
  UINT16                                            InitialCountdownValue;
} EFI_BMC_SET_WATCHDOG_TIMER_STRUCTURE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get WatchDog Timer command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_WATCHDOG_TIMER  0x25

//
//  Constants and Structure definitions for "Get WatchDog Timer" command to follow here
//
#pragma pack(1)
typedef struct {
  EFI_BMC_IPMI_WATCHDOG_TIMER_USE                   TimerUse;
  EFI_BMC_IPMI_WATCHDOG_TIMER_ACTIONS               TimerActions;
  UINT8                                             PretimeoutInterval;
  EFI_BMC_IPMI_WATCHDOG_TIMER_EXPIRATION_FLAGS      TimerUseExpirationFlagsClear;
  UINT16                                            InitialCountdownValue;
  UINT16                                            PresentCountdownValue;
} EFI_BMC_GET_WATCHDOG_TIMER_STRUCTURE;
#pragma pack()
//
// The following srucure is temporary, need to be removed later
//
#pragma pack(1)
typedef struct {
  UINT8 TimerUse : 3;
  UINT8 Reserved1 : 3;
  UINT8 StartTimer : 1;
  UINT8 DontLog : 1;
  union {
    struct {
      UINT8 TimeoutAction : 3;
      UINT8 Reserved2 : 1;
      UINT8 PreTimeoutIntr : 3;
      UINT8 Reserved3 : 1;
    } Byte2Bmp;
    UINT8 Data8;
  } uByte2;

  UINT8   PreTimeoutInterval;
  UINT8   ExpirationReserved1 : 1;
  UINT8   BiosFrb2TimerExpirationFlag : 1;
  UINT8   BiosPostTimerExpirationFlag : 1;
  UINT8   OsLoadTimerExpirationFlag : 1;
  UINT8   ExpirationReserved2 : 4;
  UINT16  Timeout;
  UINT16  CurrCountdown;
  UINT16  Reserved4;
} EFI_FRB_TIMER;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set BMC Global Enables command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_BMC_GLOBAL_ENABLES  0x2E

//
//  Constants and Structure definitions for "Set BMC Global Enables " command to follow here
//

#pragma pack(1)
typedef struct {
  UINT8 ReceiveMsgQueueInterrupt : 1;
  UINT8 EventMsgBufferFullInterrupt : 1;
  UINT8 EventMsgBuffer : 1;
  UINT8 SystemEventLogging : 1;
  UINT8 Reserved : 1;
  UINT8 OEM0 : 1;
  UINT8 OEM1 : 1;
  UINT8 OEM2 : 1;
} EFI_BMC_GLOBAL_ENABLES;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get BMC Global Enables command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_BMC_GLOBAL_ENABLES  0x2F

//
//  Constants and Structure definitions for "Get BMC Global Enables " command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Clear Message Flags command
----------------------------------------------------------------------------------------*/
#define EFI_APP_CLEAR_MESSAGE_FLAGS 0x30

//
//  Constants and Structure definitions for "Clear Message Flags" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Message Flags command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_MESSAGE_FLAGS 0x31

//
//  Constants and Structure definitions for "Get Message Flags" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Enable Message Channel Receive command
----------------------------------------------------------------------------------------*/
#define EFI_APP_ENABLE_MESSAGE_CHANNEL_RECEIVE  0x32

//
//  Constants and Structure definitions for "Enable Message Channel Receive" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Message command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_MESSAGE 0x33

//
//  Constants and Structure definitions for "Get Message" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Send Message command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SEND_MESSAGE  0x34

//
//  Constants and Structure definitions for "Send Message" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Read Event Message Buffer command
----------------------------------------------------------------------------------------*/
#define EFI_APP_READ_EVENT_MSG_BUFFER 0x35

//
//  Constants and Structure definitions for "Read Event Message Buffer" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get BT Interface Capabilities command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_BT_INTERFACE_CAPABILITY 0x36

//
//  Constants and Structure definitions for "Get BT Interface Capabilities" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get System GUID command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_SYSTEM_GUID 0x37

//
//  Constants and Structure definitions for "Get System GUID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel Authentication Capabilities command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_AUTHENTICATION_CAPABILITIES 0x38

//
//  Constants and Structure definitions for "Get Channel Authentication Capabilities" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Session Challenge command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_SESSION_CHALLENGE 0x39

//
//  Constants and Structure definitions for "Get Session Challenge" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Activate Session command
----------------------------------------------------------------------------------------*/
#define EFI_APP_ACTIVATE_SESSION  0x3A

//
//  Constants and Structure definitions for "Activate Session" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Session Privelege Level command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_SESSION_PRIVELEGE_LEVEL 0x3B

//
//  Constants and Structure definitions for "Set Session Privelege Level" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Close Session command
----------------------------------------------------------------------------------------*/
#define EFI_APP_CLOSE_SESSION 0x3C

//
//  Constants and Structure definitions for "Close Session" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Session Info command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_SESSION_INFO  0x3D

//
//  Constants and Structure definitions for "Get Session Info" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Auth Code command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_AUTHCODE  0x3F

//
//  Constants and Structure definitions for "Get AuthCode" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Channel Access command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_CHANNEL_ACCESS  0x40

//
//  Constants and Structure definitions for "Set Channel Access" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel Access command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_ACCESS  0x41

//
//  Constants and Structure definitions for "Get Channel Access" command to follow here
//

typedef enum {
  MemoryTypeReserved0,
  NonVolatile,
  PresentVolatileSetting,
  MemoryTypeReserved1
} CHANNEL_ACCESS_MEMORY_TYPE;

typedef enum {
  ChannelAccesDisabled,
  ChannelAccesPreBootOnly,
  ChannelAccesAlwaysAvailable,
  ChannelAccesShared
} CHANNEL_ACCESS_MODES;

typedef struct {
  UINT8 ChannelNo : 4;
  UINT8 Reserve1 : 4;
  UINT8 Reserve2 : 6;
  UINT8 MemoryType : 2;
} EFI_GET_CHANNEL_ACCESS_COMMAND;

typedef struct {
  UINT8 AccessMode : 3;
  UINT8 UserLevelAuthEnabled : 1;
  UINT8 MessageAuthEnable : 1;
  UINT8 Alert : 1;
  UINT8 Reserve1 : 2;
  UINT8 ChannelPriviledgeLimit : 4;
  UINT8 Reserve2 : 4;
} EFI_GET_CHANNEL_ACCESS_RESPONSE;

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel Info command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_INFO  0x42

//
//  Constants and Structure definitions for "Get Channel Info" command to follow here
//
typedef struct {
  UINT8   ChannelNo : 4;
  UINT8   Reserve1 : 4;
  UINT8   ChannelMediumType : 7;
  UINT8   Reserve2 : 1;
  UINT8   ChannelProtocolType : 5;
  UINT8   Reserve3 : 3;
  UINT8   ActiveSessionCount : 6;
  UINT8   SessionSupport : 2;
  UINT32  VendorId : 24;
  UINT16  AuxChannelInfo : 16;
  UINT8   Dummy;
} EFI_CHANNEL_INFO;

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel Info command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_INFO  0x42

//
//  Constants and Structure definitions for "Get Channel Info" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set User Access command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_USER_ACCESS 0x43

//
//  Constants and Structure definitions for "Set User Access" command to follow here
//
typedef struct {
  //
  // Byte1
  //
  UINT8   ChannelNo:4;
  UINT8   EnableIpmiMessaging:1;
  UINT8   EnableUserLinkAuthetication:1;
  UINT8   UserRestrictedToCallback:1;
  UINT8   EnableChangeBitsInByte1: 1;
  //
  // Byte2
  //
  UINT8   UserID:6;
  UINT8   Reserve1:2;
  //
  // Byte3
  //
  UINT8   UserPrivilegeLimit:4;
  UINT8   Reserve2:4;
  //
  // Byte4
  //
  UINT8   UserSimultaneousSessionLimit:4;
  UINT8   Reserve3:4;
} EFI_SET_USER_ACCESS_COMMAND;
/*----------------------------------------------------------------------------------------
    Definitions for Get User Access command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_USER_ACCESS 0x44

//
//  Constants and Structure definitions for "Get User Access" command to follow here
//
typedef struct {
  //
  // Byte1
  //
  UINT8   ChannelNo:4;
  UINT8   Reserve1:4;
  //
  // Byte2
  //
  UINT8   UserId:6;
  UINT8   Reserve2:2;
} EFI_GET_USER_ACCESS_COMMAND;

typedef struct {
  //
  // Byte1
  //
  UINT8   MaxUserId:6;
  UINT8   Reserve1:2;
  //
  // Byte2
  //
  UINT8   CurrentUserId:6;
  UINT8   Reserve2:2;
  //
  // Byte3
  //
  UINT8   FixedUserId:6;
  UINT8   Reserve3:2;
  //
  // Byte4
  //
  UINT8   UserPrivilegeLimit:4;
  UINT8   EnableIpmiMessaging:1;
  UINT8   EnableUserLinkAuthetication:1;
  UINT8   UserAccessAvailableConn:1;
  UINT8   Reserve4:1;
} EFI_GET_USER_ACCESS_RESPONSE;
/*----------------------------------------------------------------------------------------
    Definitions for Set User Name command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_USER_NAME 0x45

//
//  Constants and Structure definitions for "Set User Name" command to follow here
//
typedef struct {
  UINT8   UserId : 6;
  UINT8   Reserved : 2;
  UINT8   UserName[16];
} EFI_SET_USER_NAME_COMMAND;
/*----------------------------------------------------------------------------------------
    Definitions for Get User Name command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_USER_NAME 0x46

//
//  Constants and Structure definitions for "Get User Name" command to follow here
//
typedef struct {
  UINT8   UserId : 6;
  UINT8   Reserved : 2;
} EFI_GET_USER_NAME_COMMAND;

typedef struct {
  UINT8   UserName[16];
} EFI_GET_USER_NAME_RESPONSE;
/*----------------------------------------------------------------------------------------
    Definitions for Set User Password command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_USER_PASSWORD 0x47

//
//  Constants and Structure definitions for "Set User Password" command to follow here
//
typedef enum {
  DisableUser,
  EnableUser,
  SetPassword,
  TestPassword
} SET_USER_PASSWORD_OPERATION_TYPE;

typedef enum {
  SixteenByte,
  TwentyByte
} SET_USER_PASSWORD_SIZE;

typedef struct {
  UINT8   UserId : 6;
  UINT8   Reserved1 : 1;
  UINT8   PasswordSize : 1;
  UINT8   Operation : 2;
  UINT8   Reserved2 : 6;
  UINT8   PasswordData[20];
} EFI_SET_USER_PASSWORD_COMMAND_OF_SIZE_TWENTY;

typedef struct {
  UINT8   UserId : 6;
  UINT8   Reserved1 : 1;
  UINT8   PasswordSize : 1;
  UINT8   Operation : 2;
  UINT8   Reserved2 : 6;
  UINT8   PasswordData[16];
} EFI_SET_USER_PASSWORD_COMMAND_OF_SIZE_SIXTEEN;

/*----------------------------------------------------------------------------------------
    Definitions for Activate Payload command
----------------------------------------------------------------------------------------*/
#define EFI_APP_ACTIVATE_PAYLOAD  0x48

//
//  Constants and Structure definitions for "Activate Payload" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for De-Activate Payload command
----------------------------------------------------------------------------------------*/
#define EFI_APP_DEACTIVATE_PAYLOAD  0x49

//
//  Constants and Structure definitions for "DeActivate Payload" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Payload activation Status command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_PAYLOAD_ACTIVATION_STATUS 0x4a

//
//  Constants and Structure definitions for "Get Payload activation Status" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Payload Instance Info command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_PAYLOAD_INSTANCE_INFO 0x4b

//
//  Constants and Structure definitions for "Get Payload Instance Info" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set User Payload Access command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_USER_PAYLOAD_ACCESS 0x4C

//
//  Constants and Structure definitions for "Set User Payload Access" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get User Payload Access command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_USER_PAYLOAD_ACCESS 0x4D

//
//  Constants and Structure definitions for "Get User Payload Access" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel Payload Support command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_PAYLOAD_SUPPORT 0x4E

//
//  Constants and Structure definitions for "Get Channel Payload Support" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel Payload Version command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_PAYLOAD_VERSION 0x4F

//
//  Constants and Structure definitions for "Get Channel Payload Version" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Channel OEM Payload Info command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_OEM_PAYLOAD_INFO  0x50

//
//  Constants and Structure definitions for "Get Channel OEM Payload Info" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for  Master Write-Read command
----------------------------------------------------------------------------------------*/
#define EFI_APP_MASTER_WRITE_READ 0x52

//
//  Constants and Structure definitions for "Master Write Read" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for  Get Channel Cipher Suites command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_CHANNEL_CIPHER_SUITES 0x54

//
//  Constants and Structure definitions for "Get Channel Cipher Suites" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for  Suspend-Resume Payload Encryption command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SUSPEND_RESUME_PAYLOAD_ENCRYPTION 0x55

//
//  Constants and Structure definitions for "Suspend-Resume Payload Encryption" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for  Set Channel Security Keys command
----------------------------------------------------------------------------------------*/
#define EFI_APP_SET_CHANNEL_SECURITY_KEYS 0x56

//
//  Constants and Structure definitions for "Set Channel Security Keys" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for  Get System Interface Capabilities command
----------------------------------------------------------------------------------------*/
#define EFI_APP_GET_SYSTEM_INTERFACE_CAPABILITIES 0x57

//
//  Constants and Structure definitions for "Get System Interface Capabilities" command to follow here
//

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

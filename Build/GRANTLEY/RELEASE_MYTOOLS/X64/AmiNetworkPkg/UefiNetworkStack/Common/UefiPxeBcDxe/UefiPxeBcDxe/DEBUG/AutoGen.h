/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_B95E9FDA_26DE_48d2_8807_1F9107AC5E3A
#define _AUTOGENH_B95E9FDA_26DE_48d2_8807_1F9107AC5E3A

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>
#include <Uefi.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern CHAR8 *gEfiCallerBaseName;

#define EFI_CALLER_ID_GUID \
  {0xB95E9FDA, 0x26DE, 0x48d2, {0x88, 0x07, 0x1F, 0x91, 0x07, 0xAC, 0x5E, 0x3A}}

// Guids
extern EFI_GUID gEfiNetworkStackSetupGuid;
extern EFI_GUID gEfiMdePkgTokenSpaceGuid;
extern EFI_GUID gEfiVTUTF8Guid;
extern EFI_GUID gEfiVT100Guid;
extern EFI_GUID gEfiVT100PlusGuid;
extern EFI_GUID gEfiPcAnsiGuid;
extern EFI_GUID gEfiUartDevicePathGuid;
extern EFI_GUID gEfiSasDevicePathGuid;
extern EFI_GUID gEfiEventReadyToBootGuid;
extern EFI_GUID gEfiEventLegacyBootGuid;
extern EFI_GUID gEfiGlobalVariableGuid;
extern EFI_GUID gEfiNicIp4ConfigVariableGuid;
extern EFI_GUID gEfiSmbiosTableGuid;

// Protocols
extern EFI_GUID gEfiDevicePathProtocolGuid;
extern EFI_GUID gEfiNetworkInterfaceIdentifierProtocolGuid_31;
extern EFI_GUID gEfiArpServiceBindingProtocolGuid;
extern EFI_GUID gEfiArpProtocolGuid;
extern EFI_GUID gEfiIp4ServiceBindingProtocolGuid;
extern EFI_GUID gEfiIp4ProtocolGuid;
extern EFI_GUID gEfiIp6ServiceBindingProtocolGuid;
extern EFI_GUID gEfiIp6ProtocolGuid;
extern EFI_GUID gEfiIp6ConfigProtocolGuid;
extern EFI_GUID gEfiUdp4ServiceBindingProtocolGuid;
extern EFI_GUID gEfiUdp4ProtocolGuid;
extern EFI_GUID gEfiMtftp4ServiceBindingProtocolGuid;
extern EFI_GUID gEfiMtftp4ProtocolGuid;
extern EFI_GUID gEfiDhcp4ServiceBindingProtocolGuid;
extern EFI_GUID gEfiDhcp4ProtocolGuid;
extern EFI_GUID gEfiUdp6ServiceBindingProtocolGuid;
extern EFI_GUID gEfiUdp6ProtocolGuid;
extern EFI_GUID gEfiMtftp6ServiceBindingProtocolGuid;
extern EFI_GUID gEfiMtftp6ProtocolGuid;
extern EFI_GUID gEfiDhcp6ServiceBindingProtocolGuid;
extern EFI_GUID gEfiDhcp6ProtocolGuid;
extern EFI_GUID gEfiPxeBaseCodeCallbackProtocolGuid;
extern EFI_GUID gEfiPxeBaseCodeProtocolGuid;
extern EFI_GUID gEfiLoadFileProtocolGuid;
extern EFI_GUID gPcdProtocolGuid;
extern EFI_GUID gEfiPcdProtocolGuid;
extern EFI_GUID gGetPcdInfoProtocolGuid;
extern EFI_GUID gEfiGetPcdInfoProtocolGuid;
extern EFI_GUID gAmiDebugServiceProtocolGuid;
extern EFI_GUID gEfiDebugPortProtocolGuid;
extern EFI_GUID gEfiHiiFontProtocolGuid;
extern EFI_GUID gEfiHiiStringProtocolGuid;
extern EFI_GUID gEfiHiiImageProtocolGuid;
extern EFI_GUID gEfiHiiDatabaseProtocolGuid;
extern EFI_GUID gEfiHiiConfigRoutingProtocolGuid;
extern EFI_GUID gEfiDriverBindingProtocolGuid;
extern EFI_GUID gEfiSimpleTextOutProtocolGuid;
extern EFI_GUID gEfiGraphicsOutputProtocolGuid;
extern EFI_GUID gEfiUgaDrawProtocolGuid;
extern EFI_GUID gEfiComponentNameProtocolGuid;
extern EFI_GUID gEfiComponentName2ProtocolGuid;
extern EFI_GUID gEfiDriverConfigurationProtocolGuid;
extern EFI_GUID gEfiDriverConfiguration2ProtocolGuid;
extern EFI_GUID gEfiDriverDiagnosticsProtocolGuid;
extern EFI_GUID gEfiDriverDiagnostics2ProtocolGuid;
extern EFI_GUID gEfiFormBrowser2ProtocolGuid;
extern EFI_GUID gEfiLoadedImageProtocolGuid;
extern EFI_GUID gEfiSimpleNetworkProtocolGuid;
extern EFI_GUID gEfiManagedNetworkProtocolGuid;
extern EFI_GUID gEfiManagedNetworkServiceBindingProtocolGuid;
extern EFI_GUID gEfiHiiConfigAccessProtocolGuid;
extern EFI_GUID gEfiDpcProtocolGuid;

// Definition of PCDs used in libraries is in AutoGen.c


EFI_STATUS
EFIAPI
PxeBcDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );



EFI_STATUS
EFIAPI
NetLibDefaultUnload (
  IN EFI_HANDLE        ImageHandle
  );



#ifdef __cplusplus
}
#endif

#endif

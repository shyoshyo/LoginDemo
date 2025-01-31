//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
  Runtime instance of the AmiRomLayoutLib library class.
**/
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugPrintErrorLevelLib.h>  //AptioV server override
#include <Library/BaseLib.h>
#include <Protocol/AmiDebugService.h>  //AptioV server override

static AMI_DEBUG_SERVICE_PROTOCOL *AmiDebugServiceProtocol = NULL;
static EFI_BOOT_SERVICES  *gRuntimeAmiDebugLibBS = NULL;
static EFI_RUNTIME_SERVICES  *gRuntimeAmiDebugLibRT = NULL;
// AptioV Server Override Start: Changes to include Library Destructor
static EFI_EVENT   RuntimeAmiDebugLibExitBootServicesEvent = NULL;
static EFI_EVENT   RuntimeAmiDebugLibVirtualAddressChangeEvent = NULL;
// AptioV Server Override End: Changes to include Library Destructor
/**
  The function returns AmiDebugService instance
  @retval AmiDebugService instance or NULL if service is not available
 */
AMI_DEBUG_SERVICE_PROTOCOL *GetAmiDebugServicesProtocolInstance (VOID)
{
    EFI_STATUS Status;
    EFI_TPL CurrentTpl;
    
    if (AmiDebugServiceProtocol != NULL) return AmiDebugServiceProtocol;
    // gRuntimeAmiDebugLibBS is reset to NULL in the exit-boot-services callback
    if (gRuntimeAmiDebugLibBS == NULL) return NULL;
    // It's illegal to call LocateProtocol at TPL higher than TPL_NOTIFY
    CurrentTpl = gRuntimeAmiDebugLibBS->RaiseTPL(TPL_HIGH_LEVEL);
    gRuntimeAmiDebugLibBS->RestoreTPL(CurrentTpl);
    if ( CurrentTpl > TPL_NOTIFY ) return NULL;
    Status = gRuntimeAmiDebugLibBS->LocateProtocol(&gAmiDebugServiceProtocolGuid, NULL, &AmiDebugServiceProtocol);
    if (EFI_ERROR(Status)) AmiDebugServiceProtocol = NULL;

    return AmiDebugServiceProtocol;
}

/**
  Prints a debug message to the debug output device if the specified error level is enabled.

  If any bit in ErrorLevel is also set in DebugPrintErrorLevelLib function 
  GetDebugPrintErrorLevel (), then print the message specified by Format and the 
  associated variable argument list to the debug output device.

  If Format is NULL, then ASSERT().

  If the length of the message string specified by Format is larger than the maximum allowable
  record length, then directly return and not print it.

  @param  ErrorLevel  The error level of the debug message.
  @param  Format      Format string for the debug message to print.
  @param  ...         Variable argument list whose contents are accessed 
                      based on the format string specified by Format.
**/
VOID
EFIAPI
DebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CONST CHAR8  *Format,
  ...
  )
{
    VA_LIST   ArgList;
    AMI_DEBUG_SERVICE_PROTOCOL *AmiDebugServiceProtocol = GetAmiDebugServicesProtocolInstance();

    if(AmiDebugServiceProtocol == NULL || (ErrorLevel & GetDebugPrintErrorLevel ()) == 0) return;  //AptioV server override

    VA_START(ArgList,Format);
    AmiDebugServiceProtocol->Debug(ErrorLevel, Format, ArgList);
    VA_END(ArgList);
}

/**
  Prints an assert message containing a filename, line number, and description.  
  This may be followed by a breakpoint or a dead loop.

  Print a message of the form "ASSERT <FileName>(<LineNumber>): <Description>\n"
  to the debug output device.  If DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED bit of 
  PcdDebugProperyMask is set then CpuBreakpoint() is called. Otherwise, if 
  DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED bit of PcdDebugProperyMask is set then 
  CpuDeadLoop() is called.  If neither of these bits are set, then this function 
  returns immediately after the message is printed to the debug output device.
  DebugAssert() must actively prevent recursion.  If DebugAssert() is called while
  processing another DebugAssert(), then DebugAssert() must return immediately.

  If FileName is NULL, then a <FileName> string of "(NULL) Filename" is printed.
  If Description is NULL, then a <Description> string of "(NULL) Description" is printed.

  @param  FileName     Pointer to the name of the source file that generated the assert condition.
  @param  LineNumber   The line number in the source file that generated the assert condition
  @param  Description  Pointer to the description of the assert condition.

**/
VOID
EFIAPI
DebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description
  )
{
    AMI_DEBUG_SERVICE_PROTOCOL *AmiDebugServiceProtocol = GetAmiDebugServicesProtocolInstance();

    if(AmiDebugServiceProtocol == NULL) return;
    AmiDebugServiceProtocol->DebugAssert(FileName, LineNumber, Description);
    if ((PcdGet8(PcdDebugPropertyMask) & DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED) != 0) {
        CpuBreakpoint ();
    } else if ((PcdGet8(PcdDebugPropertyMask) & DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED) != 0) {
        CpuDeadLoop ();
    }
}


/**
  Fills a target buffer with PcdDebugClearMemoryValue, and returns the target buffer.

  This function fills Length bytes of Buffer with the value specified by 
  PcdDebugClearMemoryValue, and returns Buffer.

  If Buffer is NULL, then ASSERT().
  If Length is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT(). 

  @param   Buffer  Pointer to the target buffer to be filled with PcdDebugClearMemoryValue.
  @param   Length  Number of bytes in Buffer to fill with zeros PcdDebugClearMemoryValue. 

  @return  Buffer  Pointer to the target buffer filled with PcdDebugClearMemoryValue.

**/
VOID *
EFIAPI
DebugClearMemory (
  OUT VOID  *Buffer,
  IN UINTN  Length
  )
{
    ASSERT (Buffer != NULL);

    return SetMem (Buffer, Length, PcdGet8 (PcdDebugClearMemoryValue));
}


/**
  Returns TRUE if ASSERT() macros are enabled.

  This function returns TRUE if the DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED bit of 
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugAssertEnabled (
  VOID
  )
{
    return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED) != 0);
}


/**  
  Returns TRUE if DEBUG() macros are enabled.

  This function returns TRUE if the DEBUG_PROPERTY_DEBUG_PRINT_ENABLED bit of 
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_DEBUG_PRINT_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_DEBUG_PRINT_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugPrintEnabled (
  VOID
  )
{
    return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_PRINT_ENABLED) != 0);
}


/**  
  Returns TRUE if DEBUG_CODE() macros are enabled.

  This function returns TRUE if the DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of 
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugCodeEnabled (
  VOID
  )
{
    return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_CODE_ENABLED) != 0);
}


/**  
  Returns TRUE if DEBUG_CLEAR_MEMORY() macro is enabled.

  This function returns TRUE if the DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED bit of 
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugClearMemoryEnabled (
  VOID
  )
{
    return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED) != 0);
}

VOID EFIAPI AmiDebugLibExitBsNotifyEvent(IN EFI_EVENT Event, IN VOID *Context)
{
    gRuntimeAmiDebugLibBS = NULL;
}

VOID EFIAPI AmiDebugLibVirtualNotifyEvent(IN EFI_EVENT Event, IN VOID *Context)
{
    if (AmiDebugServiceProtocol != NULL)
        gRuntimeAmiDebugLibRT->ConvertPointer(0, (VOID **)&AmiDebugServiceProtocol);
}

EFI_STATUS
EFIAPI
RuntimeAmiDebugLibConstructor (
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    gRuntimeAmiDebugLibBS = SystemTable->BootServices;
    gRuntimeAmiDebugLibRT = SystemTable->RuntimeServices;
// AptioV Server Override Start: Changes to include Library Destructor
    gRuntimeAmiDebugLibBS->CreateEvent(
        EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_CALLBACK, AmiDebugLibExitBsNotifyEvent, NULL,
        &RuntimeAmiDebugLibExitBootServicesEvent
    );
    gRuntimeAmiDebugLibBS->CreateEvent(
        EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE, TPL_CALLBACK, AmiDebugLibVirtualNotifyEvent, NULL,
        &RuntimeAmiDebugLibVirtualAddressChangeEvent
    );
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RuntimeAmiDebugLibDestructor (
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    gRuntimeAmiDebugLibBS->CloseEvent(RuntimeAmiDebugLibExitBootServicesEvent);
    gRuntimeAmiDebugLibBS->CloseEvent(RuntimeAmiDebugLibVirtualAddressChangeEvent);
    
    return EFI_SUCCESS;
}
// AptioV Server Override End: Changes to include Library Destructor
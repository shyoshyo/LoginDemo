//**********************************************************************
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
//*****************************************************************//
// $Archive: /Alaska/SOURCE/Modules/AMITSE2_0/AMITSE/Uefi2.1/FormBrowser2.c $
//
// $Author: Arunsb $
//
// $Revision: 27 $
//
// $Date: 9/20/12 11:15a $
//
//*****************************************************************//
//<AMI_FHDR_START>
//
// Name:          FormBrowser2.C
//
// Description:
//
//<AMI_FHDR_END>
//*************************************************************************

//----------------------------------------------------------------------------
#include "minisetup.h"
#include "FormBrowser2.h"
#include "TseUefiHii.h"
#include "Token.h"
//----------------------------------------------------------------------------
extern BOOLEAN gPackUpdatePending;
extern BOOLEAN gEnableDrvNotification; //TRUE if allow notification function to process action, FALSE to ignore the notification
extern UINTN HpkFileCount;
//---------------------------------------------------------------------------
// Variables
//---------------------------------------------------------------------------
BOOLEAN gRequireSysReboot = FALSE; //EIP 75567 :If TRUE, ACTION_REQUEST_RESET will be returned from SendForm, else ignore reboot
VOID    **gSfHandles;
UINTN   gSfHandleCount;
EFI_GUID *gFSetGuid = NULL;
UINT8 *gSfNvMap;
extern BOOLEAN gBrowserCallbackEnabled; //Allow external drivers to change ASL cache only if it's TRUE; ignore browser callback otherwise
 extern BOOLEAN gEnterSetup;
extern EFI_EVENT gKeyTimer;
EFI_BROWSER_ACTION gBrowserCallbackAction = 0 ;//Contains the BrowserCallback action when a callback is in progress.
//static CONTROL_INFO *gCallbackControl = NULL; unused
static EFI_HANDLE gCurrVarHandle = (EFI_HANDLE)NULL;
static UINT32 gCurrVarIndex = 0;
#define BROWSERCALLBACK_MAX_ENTRIES 10
static EFI_HANDLE  gCallBackHandleStack[BROWSERCALLBACK_MAX_ENTRIES]; //EIP-147816
static UINT32 gCallBackVarIndexStack[BROWSERCALLBACK_MAX_ENTRIES];//EIP-147816
BOOLEAN gHiiFormNotFound = FALSE;
//EFI_GUID gEfiFormBrowser2ProtocolGuid = EFI_FORM_BROWSER2_PROTOCOL_GUID;

//---------------------------------------------------------------------------
// Structure
//---------------------------------------------------------------------------
EFI_FORM_BROWSER2_PROTOCOL FormBrowser2 =
{
  SendForm,
  BrowserCallback
};

//---------------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------------
EFI_STATUS _StorageToConfigResp(VARIABLE_INFO *VariableInfo, VOID *Buffer, CHAR16 *ConfigHdr, CHAR16 **ConfigResp);
EFI_STATUS _ConfigRespToStorage(VARIABLE_INFO *varInfo, UINT8 **buffer, UINTN *Size, EFI_STRING configResp);
BOOLEAN EDKVersion_1_05_RetrieveData (VOID);
EFI_STATUS CleanNotificationQueue (VOID);
//---------------------------------------------------------------------------
// Function Implementation
//---------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:	SetCallBackControlInfo
//
// Description:	Set the variable inforamtion
//
// Parameter:	EFI_HANDLE VarHandle - Variable handle to process
//              UINT32 VarIndex - Variable index in the gVariables->VariableList
//
// Return value:VOID
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SetCallBackControlInfo(EFI_HANDLE VarHandle, UINT32 VarIndex)
{
    static int ContextVar=0;
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Entering SetCallBackControlInfo, HIIHandle =%x \n\n", VarHandle ); 
    if(VarHandle != NULL){ //If the varaible handle is valid, set handle and variable index
        if(ContextVar >= BROWSERCALLBACK_MAX_ENTRIES)
        {
            SETUP_DEBUG_TSE ("\n[TSE] Too many nested Browser Callbacks!\n\n");
            ASSERT (0);    
        }
        gCurrVarHandle = VarHandle;
        gCurrVarIndex = VarIndex;
        //EIP-147816
        //Use Stack to manage gCurrVarHandle and gCurrVarIndex
        gCallBackHandleStack[ContextVar] = gCurrVarHandle;
        gCallBackVarIndexStack[ContextVar] = gCurrVarIndex;
        ContextVar++;
        
    } else{ //Else reset global the varaible handle and variable index
	
        if(ContextVar == 0)
            return;
                    
        ContextVar--;
        
        if(ContextVar == 0)
        {
            gCurrVarHandle = (EFI_HANDLE)NULL;
            gCurrVarIndex = 0;
        }else{
            //EIP-147816
            //Reset gCurrVarHandle and gCurrVarIndex to previous stack entry
            gCurrVarHandle = gCallBackHandleStack[ContextVar-1] ;
            gCurrVarIndex = gCallBackVarIndexStack[ContextVar-1] ;
        }
            
    }
    
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Exiting SetCallBackControlInfo HIIHandle =%x \n\n", VarHandle ); 
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:	InstallFormBrowserProtocol
//
// Description:	Install FormBrowser Protocol
//
// Parameter:	EFI_HANDLE Handle
//
// Return value:EFI_STATUS
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS InstallFormBrowserProtocol(EFI_HANDLE Handle)
{
  EFI_STATUS status = EFI_SUCCESS;

  UnInstallFormBrowserProtocol(Handle);
  status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiFormBrowser2ProtocolGuid,
                  &FormBrowser2,
                  NULL
                  );

  return status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:	UnInstallFormBrowserProtocol
//
// Description:	UnInstall FormBrowser Protocol
//
// Parameter:	EFI_HANDLE Handle
//
// Return value:VOID
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID UnInstallFormBrowserProtocol(EFI_HANDLE Handle)
{
  EFI_STATUS status = EFI_SUCCESS;
  EFI_HANDLE handle;
  VOID *iface;
  UINTN size = 0;
  
  pBS = gBS;
  status = gBS->LocateHandle (
                    ByProtocol,
                    &gEfiFormBrowser2ProtocolGuid,
                    NULL,
                    &size,
                    &handle
                    );
  if(status == EFI_BUFFER_TOO_SMALL)
  {
    handle = EfiLibAllocateZeroPool(size);
    if (handle == NULL)
    {
      status = EFI_OUT_OF_RESOURCES;
      goto DONE;
    }

    status = gBS->LocateHandle (
                      ByProtocol,
                      &gEfiFormBrowser2ProtocolGuid,
                      NULL,
                      &size,
                      &handle
                      );
  }

  if(EFI_ERROR(status))
  {
    goto DONE;
  }
  status = gBS->LocateProtocol (
                    &gEfiFormBrowser2ProtocolGuid,
                    NULL,
                    &iface
                    );
  if(EFI_ERROR(status))
  {
    goto DONE;
  }
  status = gBS->UninstallMultipleProtocolInterfaces (
                  handle,
                  &gEfiFormBrowser2ProtocolGuid,
                  iface,
                  NULL
                  );

  if(EFI_ERROR(status))
  {
    goto DONE;
  }

DONE:
  return;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   SendForm
//
// Description: This is the routine which an external caller uses to direct
//				the browser where to obtain it's information.
//
// Input:	EFI_FORM_BROWSER2_PROTOCOL *This - The Form Browser protocol instanse.
//          EFI_HII_HANDLE *Handles - A pointer to an array of Handles. If
//                  HandleCount > 1 we display a list of the formsets for the
//                  handles specified.
//          UINTN HandleCount - The number of Handles specified in Handle.
//          EFI_GUID *FormSetGuid [OPTIONAL] - This field points to the EFI_GUID
//                  which must match the Guid field in the EFI_IFR_FORM_SET
//                  op-code for the specified forms-based package. If
//                  FormSetGuid is NULL, then this function will display the
//                  first found forms package.
//          UINT16 FormId [OPTIONAL] - This field specifies which EFI_IFR_FORM
//                  to render as the first displayable page. If this field has
//                  a value of 0x0000, then the forms browser will render the
//                  specified forms in their encoded order.
//          EFI_SCREEN_DESCRIPTOR *ScreenDimenions [OPTIONAL] - Points to
//                  recommended form dimensions, including any non-content
//                  area, in characters. This allows the browser to be called
//                  so that it occupies a portion of the physical screen
//                  instead of dynamically determining the screen dimensions.
//          EFI_BROWSER_ACTION_REQUEST *ActionRequest [OPTIONAL] - Points to
//                  the action recommended by the form.
//
// Output:	EFI_STATUS status - EFI_SUCCESS, The function completed successfully.
//			EFI_INVALID_PARAMETER - One of the parameters has an invalid value.
//			EFI_NOT_FOUND - No valid forms could be found to display.
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EFIAPI
SendForm (
  IN  CONST EFI_FORM_BROWSER2_PROTOCOL *This,
  IN  EFI_HII_HANDLE                   *Handles,
  IN  UINTN                            HandleCount,
  IN  EFI_GUID                         *FormSetGuid, OPTIONAL
  IN  UINT16                           FormId, OPTIONAL
  IN  CONST EFI_SCREEN_DESCRIPTOR      *ScreenDimensions, OPTIONAL
  OUT EFI_BROWSER_ACTION_REQUEST       *ActionRequest  OPTIONAL
  )
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN i;
	BOOLEAN BackUpgEnterSetup = FALSE;

#if !SETUP_FORM_BROWSER_SUPPORT
	SETUP_DEBUG_TSE ("[TSE] SendForm not supported with SETUP_FORM_BROWSER_SUPPORT token disabled\n");
	return EFI_UNSUPPORTED;
#endif

	if(gApp != NULL || gSetupContextActive)
	{
		// Inside Setup. Setupdata contexts valid.
		UINT8 Sel=0;
		
		AMI_POST_MANAGER_PROTOCOL 	*PostMgr = NULL;
		    	
		Status = gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL,(void**) &PostMgr);
		if(Status == EFI_SUCCESS)
		{
			//PostManagerDisplayMsgBox (L"SendForm Unsupported",L"SendForm Not suppored Nestedly or from Valid Setup Context!",MSGBOX_TYPE_OK,&Sel);
			PostMgr->DisplayMsgBox(  L"SendForm Unsupported",  L"SendForm Not suppored Nestedly or from Valid Setup Context!", MSGBOX_TYPE_OK,NULL);
		}
		return EFI_UNSUPPORTED;
	}
	if( Handles == NULL || HandleCount == 0 )
	{
		Status = EFI_INVALID_PARAMETER;
		goto DONE;
	}

#ifndef STANDALONE_APPLICATION
#if !TSE_FOR_EDKII_SUPPORT
    if ( ! gConsoleControl )
    {
        //All necessary protocols are not available yet.
        //We can still proceed if Uga Draw is the only protocol not available yet.
        ActivateApplication();

			if (!IsDelayLogoTillInputSupported())	//EIP-75236
				ActivateInput();
    }
#endif    
#endif

	//Initialize send form global variables
	if(HandleCount)
	{
		gSfHandles = EfiLibAllocateZeroPool( HandleCount * sizeof(VOID*));
		gSfHandleCount = 0;
        gFSetGuid = FormSetGuid;
		for(i=0;i<HandleCount;i++)
		{
			UINT8 *FormSet = NULL;
			UINTN Length = 0;

			// Check if the Handle Has Forms to Display
			FormSet = HiiGetForm( Handles[i], 0, &Length);
			if( FormSet != NULL )
			{
				gSfHandles[gSfHandleCount] = (VOID*)(UINTN)Handles[i];
				MemFreePointer((void**) &FormSet );
				gSfHandleCount++;
			}
		}
		if( gSfHandleCount == 0 )
		{
			MemFreePointer( (VOID**)&gSfHandles );
			Status = EFI_NOT_FOUND;
			goto DONE;
		}
	}
	gSfNvMap = NULL;//NvMapOverride;

    if(!gVariableList)
	{
	    CleanTempNvramVariableList();//EIP 76381 : Reset to read the variable from NVRAM
        VarLoadVariables( (VOID **)&gVariableList, NULL );
	}
	// this *MUST* be run a EFI_TPL_APPLICATION
	gBS->RaiseTPL( EFI_TPL_HIGH_LEVEL );	// guarantees that RestoreTPL won't ASSERT
	gBS->RestoreTPL( EFI_TPL_APPLICATION );

#if TSE_FOR_EDKII_SUPPORT
	if (NULL == gLanguages)
	{
		BootGetLanguages();
	}
#endif
	StyleUpdateVersionString();

	//Show forms
	Status = gST->ConIn->Reset( gST->ConIn, FALSE );
	BackUpgEnterSetup = gEnterSetup;
	if (gKeyTimer)
	{
		TimerStopTimer( &gKeyTimer );
	}
	gHiiFormNotFound = FALSE;
	Status = MainSetupLoopHook();//EIP74591 : Modified MainSetupLoop as board module hook
	gEnterSetup = BackUpgEnterSetup;

	//Reset send form global variables
	//gSfHandles = (VOID**)NULL;
	MemFreePointer( (VOID**)&gSfHandles );
	gSfHandles = NULL;

	gSfHandleCount = 0;
	gSfNvMap = NULL;

DONE:

	if(gRequireSysReboot){ //EIP 75567 : If system reboot was set, return ACTION_REQUEST_RESET
		if(ActionRequest)
			*ActionRequest = EFI_BROWSER_ACTION_REQUEST_RESET;
		gRequireSysReboot = FALSE; //EIP 75567  :Reset system reboot variable
	}
	if(gPackUpdatePending) //If IFR notifications are pending clear the queue before exiting SendForm
	{
		CleanNotificationQueue();
	}
	gPackUpdatePending = FALSE;
	CleanTempNvramVariableList();//EIP 76381 : Reset to read the variable from NVRAM
	if (TRUE == gHiiFormNotFound)			//If form is not found then return EFI_NOT_FOUND
	{
		Status = EFI_NOT_FOUND;
	}
	return Status;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   BrowserCallback
//
// Description: This function is called by a callback handler to retrieve
//              uncommitted state data from the browser.
//
// Input:   
//          IN CONST EFI_FORM_BROWSER2_PROTOCOL *This - A pointer to the
//                  EFI_FORM_BROWSER2_PROTOCOL instance.
//          IN OUT UINTN *ResultsDataSize - A pointer to the size of the
//                  buffer associated with ResultsData.
//          IN OUT EFI_STRING ResultsData - A string returned from an IFR
//                  browser or equivalent. The results string will have no
//                  routing information in them.
//          IN BOOLEAN RetrieveData - A BOOLEAN field which allows an agent
//                  to retrieve (if RetrieveData = TRUE) data from the
//                  uncommitted browser state information or set 
//                  (if RetrieveData = FALSE) data in the uncommitted browser
//                  state information.
//          IN CONST EFI_GUID *VariableGuid, OPTIONAL - An optional field to
//                  indicate the target variable GUID name to use.
//          IN CONST CHAR16 *VariableName  OPTIONAL - An optional field to
//                  indicate the target human-readable variable name.
//
// Output:  EFI_STATUS status - EFI_SUCCESS, The results have been distributed
//                          or are awaiting distribution.
//                              EFI_BUFFER_TOO_SMALL, The ResultsDataSize
//                          specified was too small to contain the results data.
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EFIAPI
BrowserCallback (
  IN CONST EFI_FORM_BROWSER2_PROTOCOL  *This,
  IN OUT UINTN                         *ResultsDataSize,
  IN OUT EFI_STRING                    ResultsData,
  IN BOOLEAN                           RetrieveData,
  IN CONST EFI_GUID                    *VariableGuid, OPTIONAL
  IN CONST CHAR16                      *VariableName  OPTIONAL
  )
{
  EFI_STATUS    status = EFI_SUCCESS;
//  CONTROL_INFO  *ctrlInfo = NULL; Unused
  VARIABLE_INFO *varInfo = NULL;
  BOOLEAN       found;
  CHAR16        *configResp = NULL;
  CHAR16        *strPtr = NULL;
  CHAR16        *configHdr = NULL;
  UINT8         *buffer = NULL;
  UINTN         bufferSize;
  UINTN         size = 0;
  UINTN         length = 0;
  UINT32         varIndex = 0;
  UINT32         index = 0;
  UINT16		CurrentBootOption = 0;

	if((gBrowserCallbackEnabled == FALSE) && (gEnableDrvNotification == FALSE)) { 	//Allow Browser callback for AMI ExecuteCallback/AccessConfig/RouteConfig calls
		status = EFI_UNSUPPORTED;
		goto DONE;
	}

	if(ResultsDataSize == NULL || (*ResultsDataSize && ResultsData == NULL))
	{
		status = EFI_INVALID_PARAMETER;
		goto DONE;
	}
	
	SETUP_DEBUG_UEFI_CALLBACK ( "[TSE] Entering BrowserCallback()\n\n" );

  //
  // Find target storage
  //
  if(VariableGuid != NULL)
  {
    //
    // Try to find target storage
    //
    found = FALSE;
    for(index = 0; index < gVariables->VariableCount; index++)
    {
		varInfo = (VARIABLE_INFO *)VarGetVariableInfoIndex(index);
		//EIP-75352 Suppress the warnings from static code analyzer
		if(NULL == varInfo){
			continue;
		}
      if(EfiCompareGuid(&varInfo->VariableGuid, (EFI_GUID *)VariableGuid))
      {
        if(((varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_EFI_VARSTORE) != VARIABLE_ATTRIBUTE_EFI_VARSTORE) &&
          ((varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE) != VARIABLE_ATTRIBUTE_NAMEVALUE))
        {
          //
          // Buffer storage require both GUID and Name
          //
          if (VariableName == NULL)
          {
            status = EFI_NOT_FOUND;
            goto DONE;
          }

          if (EfiStrCmp (varInfo->VariableName, (CHAR16 *)VariableName) == 0)
          {
				found = TRUE;
				varIndex = index;
				SETUP_DEBUG_UEFI_CALLBACK ( "[TSE] Comparing gCurrVarHandle = %x to variableHandle %x\n\n", gCurrVarHandle, varInfo->VariableHandle  );
				if (varInfo->VariableHandle == gCurrVarHandle)					//Checking for same variable names for two different handles, if match not found continue with default
				{
	            break;
				}
          }
        }
      }
    }

    if (!found)
    {
      status = EFI_NOT_FOUND;
      goto DONE;
    }
	else						//If gCurrVarHandle is not matched then varInfo will have last index data so again filling VarInfo
	{
		//Updating the varinfo with the varIndex
		varInfo = (VARIABLE_INFO *)VarGetVariableInfoIndex(varIndex);
	}	
  } else
  {
    // GUID/Name is not specified, store variable index
    varIndex = gCurrVarIndex;
    varInfo = (VARIABLE_INFO *)VarGetVariableInfoIndex(varIndex);
  }
	//EIP-75352 Suppress the warnings from static code analyzer
	if (NULL == varInfo){
		goto DONE;
	}


  if((varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_EFI_VARSTORE) == VARIABLE_ATTRIBUTE_EFI_VARSTORE)
  {
    status = EFI_INVALID_PARAMETER;
    goto DONE;
  }

  //
  //  Retrive NVRam Buffer
  //
  buffer = (UINT8 *)VarGetVariable(varIndex, &size);
  if(buffer == NULL)
  {
    goto DONE;
  }

  //
  // Generate <ConfigHdr>
  //
  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] GetConfigHeader()\n" );  
  status = GetConfigHeader(varInfo, &configHdr, &length);
  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] GetConfigHeader retuned,  status = 0x%x \n\n" , status );    
	if(EFI_ERROR(status) || (NULL == configHdr))//EIP-75384 Static code
	{
		goto DONE;
	}

  if(RetrieveData)
  {
    //
    // Generate <ConfigResp>
    //
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] _StorageToConfigResp()\n" );  
    status = _StorageToConfigResp(varInfo, buffer, configHdr, &configResp);
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] _StorageToConfigResp returned,  status = 0x%x \n\n" , status ); 
    if (EFI_ERROR(status))
    {
       goto DONE;
    }

    //
    // Skip <ConfigHdr> and '&' to point to <ConfigBody>
    //
    if (EDKVersion_1_05_RetrieveData ())
    {
        strPtr = configResp + EfiStrLen (configHdr) + 1;
    }
    else
    {
        strPtr = configResp;
    }
    bufferSize = EfiStrSize (strPtr);
    if (*ResultsDataSize < bufferSize)
    {
      *ResultsDataSize = bufferSize;

     // gBS->FreePool (configResp);
      status = EFI_BUFFER_TOO_SMALL;
      goto DONE;
    }

    *ResultsDataSize = bufferSize;
    EfiCopyMem (ResultsData, strPtr, bufferSize);

    SETUP_DEBUG_UEFI_CALLBACK ( "[TSE] Displaying Retrieve Data Buffer, VariableName: %s, VariableHandle: 0x%x\n\n", varInfo->VariableName, varInfo->VariableHandle  );

#if TSE_DEBUG_MESSAGES
    DebugShowBufferContent(bufferSize, ResultsData);
#endif

    //gBS->FreePool(configResp);
  } else
  {
    //
    // Prepare <configResp>
    //

#if TSE_DEBUG_MESSAGES
	{
		CHAR16 BrowserCallbackFilename[50];		
		CHAR16 BrowserCallbackContent[200];
		CHAR16 *BrowserCallbackBuffer = NULL ;
		UINTN Len=0;
	
  		
		SPrint(BrowserCallbackFilename,50,L"BROWSERCALLBACK_%s_%03d.txt", varInfo->VariableName, HpkFileCount);
		SPrint(BrowserCallbackContent, 200, L"VariableName: %s\nVariableGuid: %g\nResultsDataSize: 0x%x\n", VariableName, &VariableGuid, *ResultsDataSize);
		Len = (EfiStrLen (BrowserCallbackContent) + EfiStrLen (ResultsData) + 1) * sizeof (CHAR16);
		BrowserCallbackBuffer = EfiLibAllocateZeroPool (Len);

		if (BrowserCallbackBuffer != NULL)
		{
			EfiStrCpy (BrowserCallbackBuffer, BrowserCallbackContent);
			EfiStrCat (BrowserCallbackBuffer, ResultsData);

    		status = WriteDataToFile(BrowserCallbackFilename, BrowserCallbackBuffer, Len,0) ;//EIP-123312 resolve build issue
			if (!EFI_ERROR (status))
			{
				HpkFileCount++ ;
			}

			MemFreePointer((void**)&BrowserCallbackBuffer);
		}
	}
#endif

	
    bufferSize = (EfiStrLen (ResultsData) + EfiStrLen (configHdr) + 2) * sizeof (CHAR16);
    configResp = EfiLibAllocateZeroPool (bufferSize);
    ASSERT (configResp != NULL);

    EfiStrCpy (configResp, configHdr);
    EfiStrCat (configResp, L"&");
    EfiStrCat (configResp, ResultsData);

    //
    // Update Browser uncommited data
    //
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] _ConfigRespToStorage()\n" );  
    status = _ConfigRespToStorage (varInfo, &buffer, &size, configResp);
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] _ConfigRespToStorage returned,   status = 0x%x \n\n" , status );
    
    if (EFI_ERROR (status))
    {
		goto DONE;
    }

    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Displaying Browser Uncommited Data Buffer, VariableName: %s, VariableHandle: 0x%x\n\n", varInfo->VariableName, varInfo->VariableHandle  );

#if TSE_DEBUG_MESSAGES
    DebugShowBufferContent(size, buffer);
#endif

    SETUP_DEBUG_UEFI_CALLBACK (  "\n[TSE] Updating Browser Uncommited Data\n" );
    if (varIndex == VARIABLE_ID_BBS_ORDER)
    {
		if(gCurrLegacyBootData)
		{
			UINT16 CurLegBootDataValid = 0;
			UINT32 i;
			for (i = 0; i < gBootOptionCount; i++ )
			{
				if ( &gBootData[i] == gCurrLegacyBootData )
					CurLegBootDataValid = 1;
			}
			if(CurLegBootDataValid)
				CurrentBootOption = gCurrLegacyBootData->Option;
			else
				gCurrLegacyBootData = NULL;
		}
		gCurrLegacyBootData = BootGetBootData(CurrentBootOption);
		if(gCurrLegacyBootData)
			status = VarSetValue(varIndex, 0, size, buffer);
    }
    else
    status = VarSetValue(varIndex, 0, size, buffer);
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Updating Browser Uncommited Data returned,  status = 0x%x \n\n" , status );
  }
DONE:
//MEMFREE
  if(configResp)
  {
    MemFreePointer((void**)&configResp);
  }

  if(buffer)
  {
    MemFreePointer((void**)&buffer);
  }

  if(configHdr)
  {
    MemFreePointer((void**)&configHdr);
  }
  
  SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] Exiting BrowserCallback(),  status = 0x%x \n\n" , status );  
      
  return status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	_StorageToConfigResp
//
// Description:
//
// Parameter:	VARIABLE_INFO *VariableInfo
//				VOID *Buffer
//				CHAR16 *ConfigHdr
//				CHAR16 **ConfigResp
//
// Return value:EFI_STATUS
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS _StorageToConfigResp(VARIABLE_INFO *VariableInfo, VOID *Buffer, CHAR16 *ConfigHdr, CHAR16 **ConfigResp)
{
  EFI_STATUS status = EFI_SUCCESS;
  EFI_STRING  Progress;
  CHAR16  *configHdr = NULL;
  UINTN   length = 0;

  length  = EfiStrLen(ConfigHdr) * sizeof(CHAR16);
  configHdr = (CHAR16*)EfiLibAllocateZeroPool(length + 2);
  if(configHdr == NULL)
  {
    status = EFI_OUT_OF_RESOURCES;
    goto DONE;
  }

  MemCopy(configHdr, ConfigHdr, length);

  //
  // Generate <BlockName>
  //
  status = GetBlockName(&configHdr, &length, VAR_ZERO_OFFSET, VariableInfo->VariableSize);
  if(EFI_ERROR(status))
  {
    goto DONE;
  }

  if((VariableInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE) == VARIABLE_ATTRIBUTE_NAMEVALUE)
  {
    status = EFI_UNSUPPORTED;
    goto DONE;
  }else
  {
    //
    //  VARIABLE_ATTRIBUTE_VARSTORE
    //
    status = HiiInitializeProtocol();
    if(EFI_ERROR(status))
    {
      goto DONE;
    }

    status = gHiiConfigRouting->BlockToConfig(gHiiConfigRouting, configHdr, Buffer,
      VariableInfo->VariableSize, ConfigResp, &Progress);
  }

DONE:
  if(configHdr)			//EIP81617
  {
    MemFreePointer((void**)&configHdr);
  }
  return status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	_ConfigRespToStorage
//
// Description:
//
// Parameter:	VARIABLE_INFO *VariableInfo
//				VOID **Buffer
//				UINTN *Size
//				EFI_STRING ConfigResp
//
// Return value:EFI_STATUS
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS _ConfigRespToStorage(VARIABLE_INFO *VariableInfo, UINT8 **Buffer, UINTN *Size, EFI_STRING ConfigResp)
{
  EFI_STATUS status = EFI_SUCCESS;

  if((VariableInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE) == VARIABLE_ATTRIBUTE_NAMEVALUE)
  {
    status = EFI_UNSUPPORTED;
    goto DONE;
  }else
  {
    EFI_STRING stringPtr = NULL;
    EFI_STRING tempPtr = NULL;
    UINTN length = 0;
    UINT16 *sizeBuf = NULL;

    stringPtr = ConfigResp;
    //
    // Get Buffer data Size
    //
    for(stringPtr = ConfigResp; *stringPtr != 0 && EfiStrnCmp(stringPtr, L"&WIDTH=", EfiStrLen(L"&WIDTH=")) != 0; stringPtr++);
    stringPtr += EfiStrLen(L"&WIDTH=");
    tempPtr = stringPtr;
    for(; *stringPtr != 0 && EfiStrnCmp(stringPtr, L"&VALUE=", EfiStrLen(L"&VALUE=")) != 0; length++, stringPtr++);
    sizeBuf = (CHAR16*)EfiLibAllocateZeroPool(length + 1);
    if(sizeBuf == NULL)
	{
      status = EFI_OUT_OF_RESOURCES;
	  goto DONE;
	}
    EfiCopyMem(sizeBuf, tempPtr, length);
	status = GetHexStringAsBuffer(Buffer, sizeBuf);
    MemFreePointer((void**)&sizeBuf);
    if(EFI_ERROR(status))
    {
      *Size = 0;
      goto DONE;
    }
    //
    //  Get Buffer Data
    //
    for(; *stringPtr != 0 && EfiStrnCmp(stringPtr, L"&VALUE=", EfiStrLen(L"&VALUE=")) != 0; stringPtr++);
    stringPtr += EfiStrLen(L"&VALUE=");
    status = GetHexStringAsBuffer(Buffer, stringPtr);
  }

DONE:
  return status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure	:	FormBrowserHandleValid
//
// Description	:	return True Browser is showing forms from SendForm interface.
//
// Input		:	none
//
// Output		:	BOOLEAN
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN FormBrowserHandleValid(VOID)
{
#if SETUP_FORM_BROWSER_SUPPORT
	if(gSfHandles)
		return TRUE;
#endif
	return FALSE;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure	:	FormBrowserLocateSetupHandles
//
// Description	:	return Handles and count that is passed to SendForm interface.
//
// Input		:	OUT handleBuffer and count
//
// Output		:	Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS FormBrowserLocateSetupHandles(VOID*	**handleBuffer,UINT16 *count)
{
  EFI_STATUS status = EFI_SUCCESS;

#if SETUP_FORM_BROWSER_SUPPORT
  if(gSfHandles)
  {
    *handleBuffer = (VOID**)gSfHandles;
    *count = (UINT16)gSfHandleCount;
  }
  else
  {
    status =  EFI_NOT_FOUND;
  }
#else
  status = EFI_UNSUPPORTED;
#endif
  return status;
}
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

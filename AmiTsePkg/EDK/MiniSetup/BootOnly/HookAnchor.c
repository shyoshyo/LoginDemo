//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2011, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/SOURCE/Modules/AMITSE2_0/AMITSE/BootOnly/HookAnchor.c $
//
// $Author: Rajashakerg $
//
// $Revision: 9 $
//
// $Date: 11/20/11 7:05a $
//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//


//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		HOOKANCHOR.C
//
// Description:	file to handle the hook functions
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#include "minisetup.h"

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	SearchLastHook
//
// Description:	Returns the lasthookfn number
//
// Input:		firsthookfn
//
// Output:		lastHookfn
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINTN SearchLastHook(UINTN firsthookfn)
{
	UINTN ii;

	for ( ii = firsthookfn; (UINTN)AMITSE_HookList[ii] ; ii++ )
		if( HOOKID0 == ((UINTN)AMITSE_HookList[ii]& HOOKID0) ) // Check till next HOOK
			return (UINTN)ii-1;
	return (UINTN)ii-1; // Last HOOKID may not be followed by another HOOKID but only by end of table
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	SearchHook
//
// Description:	Finds the Firsthookfn and Lasthookfn of HookNum
//
// Input:		HookNum
//
// Output:		firsthookfn , lastHookfn
//				returns zero if hook not found. Non Zero on success.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINTN SearchHook(UINTN HookNum,UINTN *firsthookfn,UINTN *lasthookfn)
{
	int ii;

	*firsthookfn = 0;
	for ( ii = 0; (UINTN)AMITSE_HookList[ii] ; ii++ )
	{
		if ( HookNum == (UINTN)AMITSE_HookList[ii] )
		{
			*firsthookfn = (UINTN)ii+2; // Skip the HOOKx and funHook
			*lasthookfn = SearchLastHook(*firsthookfn);
			return *firsthookfn;
		}
						
	}
	return 0;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHook
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
//Input:		HookNum , default function.
//
// Output:		none
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID GenericHook( UINTN HookNum, TSE_HOOK * defaultfn )
{
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			AMITSE_HookList[ii]();
	}
	else
		defaultfn();
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType1
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function.
//
// Output:		Boolean
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN GenericHookType1( UINTN HookNum, TSE_HOOK * defaultfn)
{
	BOOLEAN ret = FALSE;
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			ret |= ((TSE_HOOK_T1 *)AMITSE_HookList[ii])();
	}
	else
		ret = ((TSE_HOOK_T1 *)defaultfn)();
	return ret;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType2
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function and Key.
//
// Output:		CONTROL_ACTION
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
CONTROL_ACTION GenericHookType2( UINTN HookNum, TSE_HOOK * defaultfn, AMI_EFI_KEY_DATA key)
{
	CONTROL_ACTION ret = 0;
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			ret = ((TSE_HOOK_T2 *)AMITSE_HookList[ii])(key);
	}
	else
		ret = ((TSE_HOOK_T2 *)defaultfn)(key);
	return ret;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType3
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function, Event and Context.
//
// Output:		none
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID GenericHookType3( UINTN HookNum, TSE_HOOK * defaultfn, EFI_EVENT Event, VOID *Context)
{
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			((TSE_HOOK_T3 *)AMITSE_HookList[ii])(Event, Context);
	}
	else
		((TSE_HOOK_T3 *)defaultfn)(Event, Context);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType4
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function, Event and Context.
//
// Output:		none
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS GenericHookType4( UINTN HookNum, TSE_HOOK * defaultfn, BOOT_FLOW *bootFlowPtr)
{
	EFI_STATUS ret = EFI_SUCCESS;
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			ret = ((TSE_HOOK_T4 *)AMITSE_HookList[ii])(bootFlowPtr);
	}
	else
		ret = ((TSE_HOOK_T4 *)defaultfn)(bootFlowPtr);
	return ret;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType5
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function, Password and MaxSize.
//
// Output:		none
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID GenericHookType5( UINTN HookNum, TSE_HOOK * defaultfn, CHAR16 *Password, UINTN MaxSize)
{
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			((TSE_HOOK_T5 *)AMITSE_HookList[ii])(Password, MaxSize);
	}
	else
		((TSE_HOOK_T5 *)defaultfn)(Password, MaxSize);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType6
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function and Variable pointers to hold colors.
//
// Output:		none
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS GenericHookType6( UINTN HookNum, TSE_HOOK * defaultfn,
                        UINT8 *BGColor, UINT8 *FGColor, UINT8 *SecBGColor, UINT8 *SecFGColor, 
                        UINT8 *SelBGColor, UINT8 *SelFGColor, UINT8 *NSelBGColor, UINT8 *NSelFGColor,
                        UINT8 *LabelBGColor, UINT8 *LabelFGColor,UINT8 *NSelLabelFGColor,
                        UINT8 *EditBGColor, UINT8 *EditFGColor, UINT8 *PopupFGColor, UINT8 *PopupBGColor)
{
	EFI_STATUS ret = EFI_SUCCESS;
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			ret = ((TSE_HOOK_T6 *)AMITSE_HookList[ii])(BGColor, FGColor, SecBGColor, SecFGColor, SelBGColor,
                            SelFGColor, NSelBGColor, NSelFGColor, LabelBGColor,
                            LabelFGColor, NSelLabelFGColor, EditBGColor, EditFGColor,
                            PopupFGColor, PopupBGColor);
	}
	else
		ret = ((TSE_HOOK_T6 *)defaultfn)(BGColor, FGColor, SecBGColor, SecFGColor, SelBGColor,
                            SelFGColor, NSelBGColor, NSelFGColor, LabelBGColor,
                            LabelFGColor, NSelLabelFGColor, EditBGColor, EditFGColor,
                            PopupFGColor, PopupBGColor);

    return ret;
}

//#pragma optimize ("",off)

//Hook #1
VOID DrawQuietBootLogoHook(VOID)
{
	GenericHook( HOOKID1, DrawQuietBootLogo );
}

//Hook #2
BOOLEAN ProcessConInAvailabilityHook(VOID)
{
    return GenericHookType1( HOOKID2, (TSE_HOOK *)ProcessConInAvailability);
}

//Hook #3
VOID ProcessEnterSetupHook(VOID)
{
    GenericHook( HOOKID3, ProcessEnterSetup );
}

//Hook #4
VOID ProcessProceedToBootHook(VOID)
{
    GenericHook( HOOKID4, ProcessProceedToBoot );
}

//Hook #5
VOID MinisetupDriverEntryHookHook(VOID)
{
    GenericHook( HOOKID5, MinisetupDriverEntryHook );
}

//Hook #6
VOID PostManagerHandShakeHookHook(VOID)
{
    GenericHook( HOOKID6, PostManagerHandShakeHook );
}

//Hook #7
CONTROL_ACTION MapControlKeysHook(AMI_EFI_KEY_DATA key)
{
    return GenericHookType2( HOOKID7, (TSE_HOOK *) MapControlKeys, key);
}

//Hook #8
VOID CheckForKeyHook( EFI_EVENT Event, VOID *Context )
{
    GenericHookType3( HOOKID8, (TSE_HOOK *) CheckForKey, Event, Context);
}

//Hook #9
VOID BeforeLegacyBootLaunchHook(VOID)
{
    GenericHook( HOOKID9, BeforeLegacyBootLaunch );
}

//Hook #10
VOID AfterLegacyBootLaunchHook(VOID)
{
    GenericHook( HOOKID10, AfterLegacyBootLaunch );
}

//Hook #11
VOID BeforeEfiBootLaunchHook(VOID)
{
    GenericHook( HOOKID11, BeforeEfiBootLaunch );
}

//Hook #12
VOID AfterEfiBootLaunchHook(VOID)
{
    GenericHook( HOOKID12, AfterEfiBootLaunch );
}

//Hook #13
VOID SavedConfigChangesHook(VOID)
{
    GenericHook( HOOKID13, SavedConfigChanges );
}

//Hook #14
VOID LoadedConfigDefaultsHook(VOID)
{
    GenericHook( HOOKID14, LoadedConfigDefaults );
}

//Hook #15
VOID TimeOutLoopHookHook(VOID)
{
    GenericHook( HOOKID15, TimeOutLoopHook );
}

//Hook #16
EFI_STATUS	OemKey1HookHook ( BOOT_FLOW *bootFlowPtr )
{
    return GenericHookType4( HOOKID16, (TSE_HOOK *) OemKey1Hook, bootFlowPtr);
}

//Hook #17
EFI_STATUS	OemKey2HookHook ( BOOT_FLOW *bootFlowPtr )
{
    return GenericHookType4( HOOKID17, (TSE_HOOK *) OemKey2Hook, bootFlowPtr);
}

//Hook #18
EFI_STATUS	OemKey3HookHook ( BOOT_FLOW *bootFlowPtr )
{
    return GenericHookType4( HOOKID18, (TSE_HOOK *) OemKey3Hook, bootFlowPtr);
}

//Hook #19
EFI_STATUS	OemKey4HookHook ( BOOT_FLOW *bootFlowPtr )
{
    return GenericHookType4( HOOKID19, (TSE_HOOK *) OemKey4Hook, bootFlowPtr);
}

//Hook #20
VOID PasswordEncodeHook( CHAR16 *Password, UINTN MaxSize)
{
	GenericHookType5( HOOKID20, (TSE_HOOK *)PasswordEncode, Password, MaxSize );
}

//Hook #21
EFI_STATUS SetControlColorsHook(UINT8 *BGColor, UINT8 *FGColor, UINT8 *SecBGColor, UINT8 *SecFGColor, 
								 UINT8 *SelBGColor, UINT8 *SelFGColor, UINT8 *NSelBGColor, UINT8 *NSelFGColor,
								 UINT8 *LabelBGColor, UINT8 *LabelFGColor,UINT8 *NSelLabelFGColor, UINT8 *EditBGColor, UINT8 *EditFGColor,
								 UINT8 *PopupFGColor, UINT8 *PopupBGColor)
{
	return GenericHookType6( HOOKID21, (TSE_HOOK *)SetControlColors,
                            BGColor, FGColor, SecBGColor, SecFGColor, SelBGColor,
                            SelFGColor, NSelBGColor, NSelFGColor, LabelBGColor,
                            LabelFGColor, NSelLabelFGColor, EditBGColor, EditFGColor,
                            PopupFGColor, PopupBGColor );
}


//Hook #22
VOID InvalidActionHookHook(VOID)
{
	GenericHook( HOOKID22, InvalidActionHook );
}

//Hook #23
VOID LoadedUserDefaultsHook(VOID)
{
    GenericHook( HOOKID23, LoadedUserDefaults );
}

//Hook #24
VOID LoadedBuildDefaultsHook(VOID)
{
    GenericHook( HOOKID24, LoadedBuildDefaults );
}

//Hook #25
VOID LoadedPreviousValuesHook(VOID)
{
	GenericHook( HOOKID25, LoadedPreviousValues );
}

//Hook #26
VOID SetupConfigModifiedHook(VOID)
{
	GenericHook( HOOKID26, SetupConfigModified);
}

VOID DrawPopupMenuHook()
{
	GenericHook( HOOKID27, DrawBootOnlyBbsPopupMenu );
}

VOID BbsBootHook()
{
	GenericHook( HOOKID28, BbsBoot );
}

///Hook for Load deafaults fucntions.
VOID LoadSetupDefaultsHook( NVRAM_VARIABLE *defaults, UINTN data )
{
	GenericHookType5( HOOKID29, (TSE_HOOK *)LoadSetupDefaults, (CHAR16*)defaults, data );
}

/// EIP:26409 - Hook function to update before resetting the system
VOID PreSystemResetHookHook(VOID)
{
	GenericHook( HOOKID30, PreSystemResetHook );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GenericHookType7
//
// Description:	Locates the hook based on hooknum and calls them
//				If hook not found it calls the default function.
//
// Input:		HookNum , default function and Key.
//
// Output:		CONTROL_ACTION
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
CONTROL_ACTION GenericHookType7( UINTN HookNum, TSE_HOOK * defaultfn, VOID * pMouseInfo)
{
	CONTROL_ACTION ret = 0;
	UINTN ii;
	UINTN firsthookfn = 0;
	UINTN lasthookfn = 0;

	if ( SearchHook(HookNum,&firsthookfn, &lasthookfn ) )
	{
		for ( ii = firsthookfn; ii <= lasthookfn; ii++ )
			ret = ((TSE_HOOK_T7 *)AMITSE_HookList[ii])(pMouseInfo);
	}
	else
		ret = ((TSE_HOOK_T7 *)defaultfn)(pMouseInfo);
	return ret;
}

//Hook #31
CONTROL_ACTION MapControlMouseActionHook(VOID *pMouseInfo)
{
    return GenericHookType7( HOOKID31, (TSE_HOOK *) MapControlMouseAction, pMouseInfo);
}

//Hook #32
VOID ProcessProceedToBootNowHook(VOID)
{
    GenericHook( HOOKID32, ProcessProceedToBootNow );
}

//Hook #33
VOID ProcessConOutAvailableHook(VOID)
{
    GenericHook( HOOKID33, ProcessConOutAvailable );
}

VOID ProcessUIInitHookHook(VOID)
{
	GenericHook( HOOKID34, ProcessUIInitHook );
}

VOID AfterInitPostScreenHook(VOID)
{
	GenericHook( HOOKID35, AfterInitPostScreen );
}

//Hook #36
VOID CheckForClickHook( EFI_EVENT Event, VOID *Context )
{
    GenericHookType3( HOOKID36, (TSE_HOOK *) CheckForClick, Event, Context);
}
//HOOK #37 EIP-111415
VOID SwitchToPostScreenHook(VOID)
{
	GenericHook( HOOKID37, SwitchToPostScreen );
}

//Last Hook
VOID LastTSEHook(VOID)
{
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

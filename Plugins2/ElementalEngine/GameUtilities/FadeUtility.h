///============================================================================
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#ifndef _FADEUTILITY_HPP
#define _FADEUTILITY_HPP

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class FADEDATA
{
public:
	CHashString hszObjectName;
	CHashString hszCallbackEvent;
	float fStartFadeAmount;
	float fEndFadeAmount;
	float fElapsedFadeTime;
	float fFadeTime;
	bool bClearOverride;
	bool bCallback;
	FADEDATA()
	{
		fStartFadeAmount = 1.0f;
		fEndFadeAmount = 1.0f;
		fElapsedFadeTime = 0.0f;
		fFadeTime = 1.0f;
		bClearOverride = false;
		bCallback = false;
	}
	FADEDATA(IHashString * ObjectName, IHashString *CallbackEvent, float StartFadeAmount, float EndFadeAmount, float Time, bool ClearOverride)
	{
		if (ObjectName != NULL)
            hszObjectName.Init( ObjectName->GetString() );
		else
			hszObjectName.Init( _T("") );
		if (CallbackEvent != NULL)
		{
			bCallback = true;
			hszCallbackEvent.Init( CallbackEvent->GetString() );
		}
		else
		{
			bCallback = false;
			hszCallbackEvent.Init( _T("") );
		}
		fStartFadeAmount = StartFadeAmount;
		fEndFadeAmount = EndFadeAmount;
		fElapsedFadeTime = 0.0f;
		fFadeTime = Time;
		bClearOverride = ClearOverride;
	}
	float GetFadeAmount (float fDeltaT)
	{
		fElapsedFadeTime += fDeltaT;
		float FadeInterval = fEndFadeAmount - fStartFadeAmount;
		float CurrentFadeAmount = FadeInterval * (fElapsedFadeTime / fFadeTime ) +  fStartFadeAmount;
//		if ( FadeInterval < 0)
//			CurrentFadeAmount += fStartFadeAmount;
//		else
//			CurrentFadeAmount += fEndFadeAmount;
		return CurrentFadeAmount;
	}
	bool bStillAlive ()
	{
		if (fElapsedFadeTime < fFadeTime)
			return true;
		else
			return false;
	}
};

class CFadeUtility : public IComponent
{
	SINGLETONCONSTRUCTOROVERRIDE( CFadeUtility );
private:
	list<FADEDATA>	m_FadeList;
	IToolBox *		m_ToolBox;
	ITimer*			m_Timer;
public:
	CFadeUtility();

	// IComponent Abstract Classes.
	static IComponent * Create(int nArgs, va_list argptr);
	void Serialize(IArchive &ar);
	IHashString * GetComponentType();
	bool IsKindOf(IHashString *compType);

	// Messages.
	DWORD OnGlobalUpdateTick	( DWORD size, void * params );
	DWORD OnProcessFade			( DWORD size, void * params );
};

#endif /* #ifndef _FADEUTILITY_HPP */
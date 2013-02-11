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

#include "StdAfx.h"
#include "FadeUtility.h"
#include "FadeUtilityDefine.h"

REGISTER_COMPONENT_SINGLETON(CFadeUtility);

REGISTER_MESSAGE_HANDLER(ProcessFade, OnProcessFade, CFadeUtility );
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CFadeUtility);

TOLUA_API int tolua_FadeUtility_open (lua_State* tolua_S);

CFadeUtility::CFadeUtility()
{
	m_ToolBox = EngineGetToolBox();
	m_Timer = m_ToolBox->GetTimer();
	lua_State * LuaState;
	// hook into LUA
	LuaState = NULL;
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State *), &LuaState) == MSG_HANDLED)
	{
		// register our additional structures/handlers with LUA master
		tolua_FadeUtility_open(LuaState);
	}
	else
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
}
IComponent * CFadeUtility::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CFadeUtility);
}
void CFadeUtility::Serialize(IArchive &ar)
{
}
IHashString * CFadeUtility::GetComponentType()
{
	static CHashString compName(_T("CFadeUtility"));
	return &compName;
}

bool CFadeUtility::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

DWORD CFadeUtility::OnGlobalUpdateTick( DWORD size, void * params )
{
    VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
	list<FADEDATA>::iterator objIter;
	static CHashString hsGameTime(GAMEDELTATIMER);
	float fTimeDelta = m_Timer->GetTimeDelta( &hsGameTime );
	for ( objIter = m_FadeList.begin(); objIter != m_FadeList.end(); )
	{
		DWORD result = 0;
		Vec4 v4Fade(1.0f, 1.0f, 1.0f, (*objIter).GetFadeAmount(fTimeDelta));
		CHashString hszPixelConstant(_T("pixelconstant"));
		CHashString hszColorMultiplier(_T("ColorMultiplier"));
		MATERIALOVERRIDEPARAMS mop;
		mop.hsType = &hszPixelConstant;
		mop.hsName = &hszColorMultiplier;
		mop.vecValue = &v4Fade;
		static DWORD msgHash_SetMaterialOverride = CHashString(_T("SetMaterialOverride")).GetUniqueID();
		result = m_ToolBox->SendMessage( msgHash_SetMaterialOverride, sizeof(mop), &mop, &(*objIter).hszObjectName, NULL );

		if (!(*objIter).bStillAlive() || (result == MSG_NOT_HANDLED) )
		{
			if ((*objIter).bClearOverride)
			{
				CLEARMATOVERRIDEPARAMS cmop;
				static DWORD msgHash_ClearMaterialOverride = CHashString(_T("SetMaterialOverride")).GetUniqueID();
				m_ToolBox->SendMessage( msgHash_ClearMaterialOverride, sizeof(cmop), &cmop, &(*objIter).hszObjectName, NULL );
			}
			if ((*objIter).bCallback)
			{
				TRIGGEREVENTPARAMS tep;
				tep.EventName = &(*objIter).hszCallbackEvent;
				tep.EventParamsArchive = NULL;
				static DWORD msgHash_TriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
				static CHashString hszCQHStateObject = _T("CQHStateObject");
				m_ToolBox->SendMessage( msgHash_TriggerEvent, sizeof(tep), &tep, &(*objIter).hszObjectName, &hszCQHStateObject );
			}
			objIter = m_FadeList.erase(objIter);
		}
		else
			++objIter;
	}
	return MSG_HANDLED_PROCEED;
}
DWORD CFadeUtility::OnProcessFade( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FADEUTILITYPARAMS));
	FADEUTILITYPARAMS *fup = (FADEUTILITYPARAMS*)params;
	if (!fup->ObjectName)
	{
		m_ToolBox->Log( LOGERROR, _T("Failed to generate correct number of vertices for height node top/bottom faces.") );
	}
	FADEDATA FadeData(fup->ObjectName, fup->CallbackEvent, fup->StartFadeAmount, fup->EndFadeAmount, fup->FadeTime, fup->bClearOverride);
	m_FadeList.push_front(FadeData);
	
	static CHashString hszRenderState(_T("renderstate"));
	static CHashString hszAlphaBlend(_T("alphablend"));
	static CHashString hszCullMode(_T("cullmode"));
	static CHashString hszDepthWrite(_T("depthwrite"));
	static CHashString hszTrue(_T("true"));
	static CHashString hszFalse(_T("false"));
	static CHashString hszCullRegular(_T("cullregular"));

	static DWORD msgHash_SetMaterialOverride = CHashString(_T("SetMaterialOverride")).GetUniqueID();
	MATERIALOVERRIDEPARAMS mop;
	mop.hsType = &hszRenderState;

	mop.hsName = &hszAlphaBlend;
	mop.hsValue = &hszTrue;
	m_ToolBox->SendMessage( msgHash_SetMaterialOverride, sizeof(mop), &mop, fup->ObjectName, NULL );

	mop.hsName = &hszCullMode;
	mop.hsValue = &hszCullRegular;
	m_ToolBox->SendMessage( msgHash_SetMaterialOverride, sizeof(mop), &mop, fup->ObjectName, NULL );

	mop.hsName = &hszDepthWrite;
	mop.hsValue = &hszFalse;
	m_ToolBox->SendMessage( msgHash_SetMaterialOverride, sizeof(mop), &mop, fup->ObjectName, NULL );

	return MSG_HANDLED_STOP;
}
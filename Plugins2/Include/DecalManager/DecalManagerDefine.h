///============================================================================
/// \file		_DECALMANAGER_H_.h
/// \brief		Header file for DecalManager plugin defines
/// \date		12-12-2006
/// \author		Marvin Gouw
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

#ifndef _DECALMANAGER_H_
#define _DECALMANAGER_H_

// tolua_begin
typedef struct ADDDECALSPRITEMESSAGE
{

	float					m_Size;
	Vec3*					m_Position;
	Vec3*					m_Normal;
	float					m_TimeLinger;
	float					m_FadeTime;
	DWORD					m_Color;
	IBaseTextureObject  *	m_Texture;
	IEffect				*	m_Effect;
	IHashString*			m_hsTextureName;	

	ADDDECALSPRITEMESSAGE( )
	{	
		m_Size = 0;
		m_Effect = NULL;
		m_Texture = NULL;
		m_Position = NULL;
		m_Normal = NULL;
		m_Color = 0xFFFFFFFF;
		m_TimeLinger = 0.f; //time when alpha == 1.f
		m_FadeTime = 1.f; //time interval after time linger when alpha fades to 0.f
		m_hsTextureName = NULL;		
	}
} ADDDECALSPRITEMESSAGE;
// tolua_end

#endif // #ifndef REGISTERCAL3DANIMATIONCALLBACK

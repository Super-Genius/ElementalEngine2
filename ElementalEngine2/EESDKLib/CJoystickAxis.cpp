///============================================================================
/// \file		CJoystickAxis.cpp
/// \brief		Definition file for JoystickAxis class
/// \date		03-31-2008
/// \author		Richard Nguyen
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

#include "stdafx.h"
#include "CJoystickAxis.h"

CJoystickAxis::CJoystickAxis()
{
	m_bChanged = false;
	m_fAxisLastChanged = 0;
	m_fRawValue = 0;
	m_fValue = 0;
	m_iMin = m_iMax = 0;
	m_fSmoothAmount = 0.0f;
	m_bNoChangeFault = false;
	m_bFaultFlag = false;
	m_bCheckFault = false;
}

void CJoystickAxis::ResetFaults( float fTime )
{
	m_fAxisLastChanged = fTime;
	m_bNoChangeFault = false;

	m_bFaultFlag = false;
}

/// reset calibration data
void CJoystickAxis::ResetAxisCalibration( int iMinValue, int iMaxValue )
{
	m_iMin = iMinValue;
	m_iMax = iMaxValue;
	m_AxisHistory.clear();
}

/// Takes in raw (uncalibrated, unfiltered) data and stores normalized filtered data in the range [0,1]
void CJoystickAxis::ProcessRawInput( int iValue, float fTime, bool bUseSmoothing )
{
	float f = 0;
	if( (m_iMax - m_iMin) != 0 )
	{
		f = (float)(iValue - m_iMin) / (float)(m_iMax - m_iMin);
	}

	m_bChanged = m_fRawValue != f;
	m_fRawValue = f;

	if (m_bChanged)
	{
		m_fAxisLastChanged = fTime;
	}

	// use history to smooth input over time
	if (bUseSmoothing && m_fSmoothAmount > 0.0f)
	{
		AXISHISTORYENTRY entry = { f, fTime };
		m_AxisHistory.push_back( entry );

		f = 0; // reset f to zero to use as an accumulator
		std::vector<AXISHISTORYENTRY>::iterator itr = m_AxisHistory.begin();
		float fThresholdTime = fTime - m_fSmoothAmount; // find how far back in time we influence the input
		float fUsedSamples = 0.0f; // used to average the input
		while (itr != m_AxisHistory.end())
		{
			if (itr->fTime >= fThresholdTime)
			{
				f += itr->fValue;
				fUsedSamples += 1.0f;
			}
		}
		f /= fUsedSamples;
	}

	m_fValue = f;
}
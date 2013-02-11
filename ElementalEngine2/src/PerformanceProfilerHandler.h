///============================================================================
/// \file		PerformanceProfilerHandler.h
/// \brief		Header file for performance profiler handler component
/// \date		11-16-2006
/// \author		Brian Bazyk
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

#ifndef PERFORMANCEPROFILERHANDLER_H
#define PERFORMANCEPROFILERHANDLER_H

class CPerformanceProfilerHandler : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CPerformanceProfilerHandler );

	/// Private ctor
	CPerformanceProfilerHandler();
public:
	static IComponent *Create(int nArgs, va_list argptr);

	virtual ~CPerformanceProfilerHandler();

	/// \brief Update a performance profiler entry with a new time
	/// \param size - sizeof a PERFORMANCEPROFILERUPDATE struct
	/// \param *params - pointer to a PERFORMANCEPROFILERUPDATE struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, MSG_ERROR
	DWORD OnUpdatePerformanceProfiler(DWORD size, void *params);

	/// \brief Copies the map of performance profilers into a provided map for output.
	/// \param size - sizeof a GETPERFORMANCEPROFILERS struct
	/// \param *params - pointer to a GETPERFORMANCEPROFILERS struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, MSG_ERROR
	DWORD OnGetPerformanceProfilers(DWORD size, void *params);

	/// Gets the mem usage used by the process
	/// \param size = sizeof(PROCESSMEMINFO)
	/// \param param = PROCESSMEMINFO struct with mem data
	/// \return MSG_HANDLED_STOP
	DWORD OnGetProcessMemoryUsage(DWORD size, void *param);

	/// Clears the specified submap
	/// \param size = sizeof(IHashString)
	/// \param param = IHashString * of name of submap to clear
	DWORD OnClearProfilerSubMap(DWORD size, void *param);

	virtual void Serialize(IArchive &ar)
	{
	}

	virtual IHashString *GetComponentType()
	{
		static CHashString hszComponentName(_T("CPerformanceProfilerHandler"));
		return &hszComponentName;
	}

	virtual bool IsKindOf(IHashString *compType)
	{
		return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );
	}

private:
	/// Used to map between a profiler name and its last updated profile time
	PERFORMANCEPROFILERTYPEMAP m_PerformanceProfilers;

	/// Updates the memory tab in the performance profiler
	/// \param pmi = new memory to insert into map	
	void UpdatePerfMapMemory(PROCESSMEMINFO pmi);

	// vars that were static that now crash on exit
	DWORD m_dwDefault;
};

#endif // PERFORMANCEPROFILER_H
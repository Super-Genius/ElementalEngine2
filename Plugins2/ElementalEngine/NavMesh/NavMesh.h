///============================================================================
/// \file		NavMesh.h
/// \brief		Declaration of CNavMeshObject Class
/// \date		09-07-2005
/// \author		Brian Bazyk
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

#ifndef __NAVMESHOBJECT_H__
#define __NAVMESHOBJECT_H__

#include "CObjectTemplate.hpp"
#include "Vec3.h"

typedef unsigned short NAVINDEX;
const unsigned int MAX_NAVINDEX = ((1<<16)-1);
const unsigned int INVALID_NAVINDEX = MAX_NAVINDEX;

const unsigned int NAVFLAG_EDGE_SECTOR = (1<<0);
const unsigned int NAVFLAG_EDGE_PERIMETER = (1<<1);

class CNavVertex
{
public:
	/// position of vertex
	Vec3 m_Position;
};

class CNavEdge
{
public:
	CNavEdge()
	{
		m_vVertices[0] = m_vVertices[1] = INVALID_NAVINDEX;
		m_vAdjacentSubsectors[0] = m_vAdjacentSubsectors[1] = INVALID_NAVINDEX;
		m_Flags = 0;
	}

	/// \return length of the edge
	float GetLength() const;

	void ChangeAdjacentSubsector( NAVINDEX iOldSubsector, NAVINDEX iNewSubsector )
	{
		if (m_vAdjacentSubsectors[0] == iOldSubsector)
			m_vAdjacentSubsectors[0] = iNewSubsector;
		if (m_vAdjacentSubsectors[1] == iOldSubsector)
			m_vAdjacentSubsectors[1] = iNewSubsector;
	}

	/// indices to vertices
	/// NOTE: Winding not preserved.  Use vertex indices from the subsector to guarantee winding.
	NAVINDEX m_vVertices[2];
	/// indices to adjacent subsectors
	NAVINDEX m_vAdjacentSubsectors[2];
	/// edge flags
	unsigned int m_Flags;
};

class CNavSubsector
{
public:
	CNavSubsector()
	{
		m_iParentSector = INVALID_NAVINDEX;
		m_vVertices[0] = m_vVertices[1] = m_vVertices[2] = INVALID_NAVINDEX;
		m_AdjacentSubsectors[0] = m_AdjacentSubsectors[1] = m_AdjacentSubsectors[2] = INVALID_NAVINDEX;
	}

	/// \return area of the subsector
	float GetArea() const;
	/// \return center point of the subsector
	Vec3 GetCenter() const;

	inline NAVINDEX GetNextVertexCCW( NAVINDEX iVertex )
	{
		for (int i=0; i < 3; i++)
		{
			if (m_vVertices[i] == iVertex)
				return m_vVertices[(i+1) % 3];
		}
		return INVALID_NAVINDEX;
	}
	inline void SetNextVertexCCW( NAVINDEX iStartVertex, NAVINDEX iNewVertex )
	{
		for (int i=0; i < 3; i++)
		{
			if (m_vVertices[i] == iStartVertex)
			{
				m_vVertices[(i+1)%3] = iNewVertex;
				return;
			}
		}
	}

	inline void OrderVerticesCCW( const NAVINDEX *vInVertices, NAVINDEX *vOutVertices )
	{
		for (int i=0; i < 3; i++)
		{
			if (m_vVertices[i] == vInVertices[0])
			{
				if (m_vVertices[(i+1)%3] == vInVertices[1])
				{
					// edge vertices are in same winding
					vOutVertices[0] = vInVertices[0];
					vOutVertices[1] = vInVertices[1];
				}
				else
				{
					// edge vertices are in opposite winding
					vOutVertices[0] = vInVertices[1];
					vOutVertices[1] = vInVertices[0];
				}
				return;
			}
		}

		// vertices not found
		vOutVertices[0] = INVALID_NAVINDEX;
		vOutVertices[1] = INVALID_NAVINDEX;
	}

	void GetEdgeVertices( int iEdge, NAVINDEX &out_v0, NAVINDEX &out_v1 )
	{
		out_v0 = m_vVertices[iEdge];
		out_v1 = m_vVertices[(iEdge+1)%3];
	}

	int GetEdgeFromVertices( NAVINDEX v0, NAVINDEX v1 )
	{
		for (int i=0; i < 3; i++)
		{
			if ((m_vVertices[i] == v0 && m_vVertices[(i+1)%3] == v1) || // CCW
				(m_vVertices[i] == v1 && m_vVertices[(i+1)%3] == v0)) // CW
			{
				return i;
			}
		}
		assert( false );
		return 0;
	}

	/// index to parent sector
	NAVINDEX m_iParentSector;
	/// indices to vertices, CCW winding
	NAVINDEX m_vVertices[3];
	/// indices to adjacent subsectors
	NAVINDEX m_AdjacentSubsectors[3];
};

class CNavSector
{
public:
	CNavSector()
	{
	}
	float GetArea() const;

	std::vector<NAVINDEX> m_Subsectors;
};

class CNavObjectPool
{
/*
	while editing, allow objects to be created and destroyed without moving around other objects
	pool grows automatically but only shrinks on command
	when shrinking a map of indices is returned by the shrink function
	so we can say
	
	void RemoveUnused( std::map<NAVINDEX> &out_map );
	then if the size of out_map is > 0 then go thru every object for that pool and remap it's index
*/
};

class CNavMeshObject : public CObjectTemplate<IObject>
{
private:
	/// Default Constructor
	CNavMeshObject(IHashString *parentName, IHashString *name, bool bAddToHier);
	/// Default Destructor
	~CNavMeshObject();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Init
	virtual void Init();
	/// DeInit
	virtual void DeInit();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	// --- Messages ---
	DWORD OnGenerateNavMesh( DWORD size, void *params );
	DWORD OnVisualizeNavMesh( DWORD size, void *params );

	/// \return sector by index
	inline CNavSector* GetSector(NAVINDEX index) { return (index == INVALID_NAVINDEX) ? NULL : &m_Sectors[index]; }
	/// \return subsector by index
	inline CNavSubsector* GetSubsector(NAVINDEX index) { return (index == INVALID_NAVINDEX) ? NULL : &m_Subsectors[index]; }
	/// \return edge by index
	inline CNavEdge* GetEdge(NAVINDEX index) { return (index == INVALID_NAVINDEX) ? NULL : &m_Edges[index]; }
	/// \return vertex by index
	inline CNavVertex* GetVertex(NAVINDEX index) { return (index == INVALID_NAVINDEX) ? NULL : &m_Vertices[index]; }
	/// \return sector count
	inline int GetSectorCount() const { return m_Sectors.size(); }
	/// \return subsector count
	inline int GetSubsectorCount() const { return m_Subsectors.size(); }
	/// \return edge count
	inline int GetEdgeCount() const { return m_Edges.size(); }
	/// \return vertex count
	inline int GetVertexCount() const { return m_Vertices.size(); }

private:
	void VisualizeNavMesh( bool bVisible );
	void DebugDumpNavMesh();

	CNavVertex& NewVertex( NAVINDEX *pNewIndex=NULL );
	CNavEdge& NewEdge( NAVINDEX *pNewIndex=NULL );
	CNavSubsector& NewSubsector( NAVINDEX *pNewIndex=NULL );
	CNavSector& NewSector( NAVINDEX *pNewIndex=NULL );

	/// Inserts a point along an edge which will subsequently split sectors adjacent to the edge
	void InsertPointOnSubsectorEdge( NAVINDEX iSubsector, int iEdge, float fEdgeDistance );

	/// EE Tool Box
	IToolBox *m_ToolBox;
	/// Class Type
	CHashString m_hszNameType;
	/// NavMesh Render Object Name
	CHashString m_hszNavMeshRenderObject;

	/// list of all sectors
	std::vector<CNavSector> m_Sectors;
	/// list of all subsectors
	std::vector<CNavSubsector> m_Subsectors;
	/// list of all edges
	std::vector<CNavEdge> m_Edges;
	/// list of all vertices
	std::vector<CNavVertex> m_Vertices;

	bool m_bVisible;

	bool m_bAddToHierarchy;
};

/*
enum { NAV_EDGE_RIGHT, NAV_EDGE_LEFT, NAV_EDGE_UP, NAV_EDGE_DOWN };
static int g_vOppositeEdgeTable[] = { NAV_EDGE_LEFT, NAV_EDGE_RIGHT, NAV_EDGE_DOWN, NAV_EDGE_UP };
#define OPPOSITE_EDGE(e) (g_vOppositeEdgeTable[e])

struct NAVNODE
{
	Vec3 m_Position;
	int m_vEdges[4];
	int m_iSubNode;
	short m_iGridX, m_iGridY;
};

struct NAVEDGE
{
	int m_vNodes[2];
	float m_fDistance;
};

struct NAVDATAPARAMS
{
	NAVNODE *pNodes;
	int nNodes;
	NAVEDGE *pEdges;
	int nEdges;
};

class CNavMeshObject : public CObjectTemplate<IObject>
{
private:
	/// Default Constructor
	CNavMeshObject(IHashString *parentName, IHashString *name);
	/// Default Destructor
	~CNavMeshObject();

public:
	static IComponent *Create(va_list argptr);

	/// Init
	virtual void Init();
	/// DeInit
	virtual void DeInit();

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	// --- Messages ---
	DWORD OnGenerateNavMesh( DWORD size, void *params );
	DWORD OnGetNavMeshData( DWORD size, void *params );
	DWORD OnSetVisualizeNavMeshMode( DWORD size, void *params );

	/// Generate the nav mesh 
	void GenerateNavMesh();

	///
	void GenerateNavMeshZ_Raindrop();
	///
	void GenerateNavMeshZ_Groundup();
	///
	void GenerateNavMeshZ_Bouncy();

	/// Write out the nav mesh to a .nav file
	/// \return true if file successfully written, false otherwise
	bool WriteNavMeshFile();

	/// Read in a nav mesh from a .nav file
	bool ReadNavMeshFile();

	/// Returns whether a nav file exists
	/// \return true if nav file exists, false otherwise
	bool NavFileExists( const TCHAR *szFile );

	/// Generates a new unique nav file name
	/// \return a pointer to internal string containing the relative path to the nav file
	const TCHAR* NewNavFile();

	///
	void RemoveEdge( int iEdge );

	///
	void Reindex();

	/// 
	/// \param fSlope - max slope in degrees an edge can have without being removed
	void RemoveEdgesBySlope( float fSlope );

	/// 
	void RemoveEdgesByMonofilament();

private:
	///
	void AddSubNode( int iNode, float z );
	///
	void GenerateEdges();
	///
	void GenerateEdges( int iGridNodeA, int iGridNodeB, std::vector<NAVEDGE> &vecEdges, int iEdgeAB );
	///
	void RebuildNodeGrid();

	/// Find the root node adjacent to the given node in the direction of an edge.
	inline int AdjacentNode( int iGridX, int iGridY, int iEdge );

	/// 
	void Validate();

	/// Pointed to the EE Tool Box
	IToolBox *m_ToolBox;

	/// Class Type
	CHashString m_hszNameType;

	/// Nav Data File
	StdString m_szNavFile;

	bool m_bNavMeshVis;

	NAVNODE *m_pNodes;
	int m_nNodes;

	NAVEDGE *m_pEdges;
	int m_nEdges;

	CHashString m_hszNavMeshRenderObject;

	float m_fSampleDistance;

	std::vector<NAVNODE> m_SubNodes;

	// for fast node lookups based on position
	int m_iGridSizeX, m_iGridSizeY;
	NAVNODE **m_pNodeGrid;
};
*/
#endif //__NAVMESHOBJECT_H__
///============================================================================
/// \file	CTextureCompositer.h
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

#ifndef CTEXTURECOMPOSITE_H
#define CTEXTURECOMPOSITE_H

#include "CManager.h"
#include <set>
#include <map>
using namespace std;

struct CompositeTexEntry
{
	float x1, y1, x2, y2;
	//not always good to put all textures in one tex..when switching mipmap levels
	//we can get a lot of stalls with large textures?

};

struct CompositeTexInfo
{
	struct Region
	{
		float x1, y1, x2, y2;
		IBaseTextureObject * texture;
		Region()
		{
			texture = NULL;
		}
		bool IsZeroSize()
		{
			return ( x2 == x1 ||
				y2 == y1 );
		}
	};
	IBaseTextureObject * tex;
	list< Region > m_Regions;
	list< Region > m_EmptyRegions;

	inline Region GetOverlap( Region * a, Region * b )
	{
		Region reg;
		if( a && b )
		{
			reg.x1 = ( a->x1 > b->x1 ) ? a->x1 : b->x1;
			reg.x2 = ( a->x2 < b->x2 ) ? a->x2 : b->x2;
			reg.y1 = ( a->y1 > b->y1 ) ? a->y1 : b->y1;
			reg.y2 = ( a->y2 < b->y2 ) ? a->y2 : b->y2;
		}
		return reg;
	}

	inline bool Overlaps( Region * a, Region * b )
	{
		if( a && b )
		{
			return !( a->y2 < b->y1 || 
				a->y1 > b->y2 ||
				a->x2 < b->x1 ||
				a->x1 > b->x2 );
		}
		return false;
	}

	inline void RemoveFromEmptyRegions( Region* region )
	{
		//crops the empty regions to the region in question
		size_t curSize = m_EmptyRegions.size();//requires we save of the size since we grow it later as we loop
		list< Region >::iterator iter = m_EmptyRegions.begin();
		for( size_t i = 0; i < curSize ; i++, iter++ )
		{
			Region &curReg = m_EmptyRegions[ i ];
			//check for bounds overlap, if so crop
			if( Overlaps( &curReg, region ) )
			{
				Region overlap = GetOverlap( &curReg, region );
				Region a1, a2;
				if( curReg.x1 == overlap.x1 )
				{
					if( curReg.y1 == overlap.y1 ) //overlap on top left corner
					{
						a1.x1 = overlap.x2;
						a1.x2 = curReg.x2;
						a1.y1 = curReg.y1;
						a1.y2 = overlap.x2;
						a2.x1 = curReg.x1;
						a2.x2 = curReg.x2;
						a2.y1 = overlap.y2;
						a2.y2 = curReg.y2;
					}else //overlap on bottom left
					{
						a1.x1 = curReg.x1;
						a1.x2 = curReg.x2;
						a1.y1 = curReg.y1;
						a1.y2 = overlap.y1;
						a2.x1 = overlap.x2;
						a2.x2 = curReg.x2;
						a2.y1 = overlap.y1;
						a2.y2 = curReg.y2;
					}
				}
				else
				{
					if( curReg.y1 == overlap.y1 ) //overlap on top right
					{
						a1.x1 = curReg.x1;
						a1.x2 = overlap.x1;
						a1.y1 = curReg.y1;
						a1.y2 = overlap.y2;
						a2.x1 = curReg.x1;
						a2.x2 = curReg.x2;
						a2.y1 = overlap.y2;
						a2.y2 = curReg.y2;
					}else //overlap on bottom right
					{
						a1.x1 = curReg.x1;
						a1.x2 = curReg.x2;
						a1.y1 = curReg.y1;
						a1.y2 = overlap.y1;
						a2.x1 = curReg.x1;
						a2.x2 = overlap.x1;
						a2.y1 = overlap.y1;
						a2.y2 = curReg.y2;
					}
				}
				//A1 and A2 are now constructed, add them to the list, overwriting the current one
				bool usedCurrent = false;
				if( !a1.IsZeroSize() )
				{
					useCurrent = true;
					curReg = a1;
				}
				if( !a2.IsZeroSize() )
				{
					if( useCurrent == false)
					{
                        curReg = a2;
						useCurrent = true;
					}
					else
					{
						m_Regions.push_back( a2 );
					}
				}
				//nothing added?
				if( !useCurrent )
				{
					//delete the element, this region fits exactly
					m_EmptyRegions.erase( iter );
					break;
				}
			}
		}
	}
	bool GetEmptyRegion( Region* region, int width, int height )
	{
		//Goes through empty region list to find the best fit
		m_EmptyRegions


	}
	bool RemoveTexture( IBaseTextureObject * texture )
	{
        		
	}

	bool AddTexture( IBaseTextureObject * texture )
	{
		UINT width = texture->GetWidth();
		UINT height = texture->GetHeight();
		//find region using some algorithm
		Region reg;
		if( GetEmptyRegion( &reg, width, height ) )
		{
			RemoveFromEmptyRegions( &reg ); //dedicate the space 
			reg.texture = texture;
			m_Regions.push_back( reg );
		}
		else
		{
			//no space
		}
	}
};

typedef IBaseTextureObject * CompositeTexture;
class CTextureCompositeManager : public CManager
{
public:
	~CTextureCompositeManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );

private:
	SINGLETONCONSTRUCTOROVERRIDE( CTextureCompositeManager );
	CTextureCompositeManager();

	map< CompositeTexture, CompositeInfo > m_CompositeInfos;
	vector< CompositeTexture > m_Textures;
	map< IBaseTextureObject *, CompositeTexEntry > m_TextureMapping;
	
};

#endif
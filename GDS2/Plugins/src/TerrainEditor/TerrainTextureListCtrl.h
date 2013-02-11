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

#ifndef TERRAINTEXTURELISTCTRL_H
#define TERRAINTEXTURELISTCTRL_H

class CTerrainTextureListCtrl : public CResourceListCtrlEx
{
public:
	CTerrainTextureListCtrl();
	~CTerrainTextureListCtrl();
	/// Insert a layer into the list control
	/// \param iPositionIndex - index of the position in which to insert the new item
	/// \param szFilePath - relative path of the layer texture to insert into the list
	/// \return bool - Returns whether item was successfully inserted
	bool InsertLayer( int iPositionIndex, const TCHAR *szFilePath );
	/// Remove a layer from the list control 
	/// \param iLayerIndex - index of the layer to remove
	void RemoveLayer( int iLayerIndex );

protected:
	/// Create specific thumbnail for this resource type.
	/// \param szFilePath - relative path of the resource to create thumbnail.
	/// \return CBitmap pinter with thumbnail, NULL if thumbnail not be created.
	virtual CBitmap * CreateThumbnail(const TCHAR *szFilePath);
	
	LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnDragMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	IToolBox *m_ToolBox;
};

#endif		//#ifndef TERRAINTEXTURELISTCTRL_H

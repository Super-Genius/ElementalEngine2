///==========================================================================
/// \file	ProxyObject.h
/// \brief	Declaration for proxy objects
/// \date	03/20/2007
/// \author	Nick Tourte
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
///==========================================================================

#ifndef _C_PROXYOBJECT_H_
#define _C_PROXYOBJECT_H_

class CProxyObject : public IObject
{
public:
	/// ctor, dtor
	CProxyObject();
	virtual ~CProxyObject();

	/// Object fns, piped to proxied object
	virtual void Serialize(IArchive &ar);
	virtual void Init();
	virtual void DeInit();
	virtual void SetParentName(IHashString *inName);
	virtual bool Update();
	virtual IHashString *GetComponentType();
	virtual bool IsKindOf(IHashString *compType);
	virtual IHashString *GetName();	
	virtual void SetName(IHashString *inName);
	virtual IHashString *GetParentName();

	/// Creates a proxied object
	/// \param parentName = same parent name as the component inherited off of this
	/// \param name = same name as the component etc. etc. 
	/// \param className = name of class type to make proxied object of
	/// \return true if successful
	bool CreateProxiedObject(IHashString *parentName, IHashString *name, IHashString *className);

private:
	/// the proxied object
	IObject *m_ProxiedObject;
	
	/// toolbox cached
	IToolBox *m_ToolBox;
	
	/// proxy manager
	IManager *m_ProxyManager;

	/// proxy manager typename 
	CHashString m_hsProxyManagerType;
};

#endif // _C_PROXYOBJECT_H_
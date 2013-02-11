#ifndef HIERARCHICALMODELLOADER_H
#define HIERARCHICALMODELLOADER_H

#include <IComponent.hpp>
#include <LoadSaveMessages.hpp>

class CDX9ShaderLoader : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CDX9ShaderLoader);
	CDX9ShaderLoader();
public:

	/// We are the serializer, so we don't do anything with this.
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// standard static component create function
	static IComponent *Create(va_list argptr);

	// ------------------ Messages ------------------ //
	/// message to load a  file
	DWORD OnLoadFile(DWORD size, void *params);
	/// message to save a  file
	DWORD OnSaveFile(DWORD size, void *params);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;
};


#endif /* PARTICLELOADER_H */
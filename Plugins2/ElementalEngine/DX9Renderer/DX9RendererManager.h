#ifndef CHModelManager_H
#define CHModelManager_H

#include "CManager.h"

class CHModelManager : public CManager
{

public:

	/// \brief The Default Destructor for CHModelManager
	/// \return void
	~CHModelManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( va_list argptr );

	/// \brief	Initializes the CHModelManager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the CHModelManager
	/// \return	void
	virtual void Update( void );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
private:
	SINGLETONCONSTRUCTOROVERRIDE( CHModelManager );
	/// \brief The Default Constructor for CHModelManager
	/// \return void
	CHModelManager();
	
};

#endif
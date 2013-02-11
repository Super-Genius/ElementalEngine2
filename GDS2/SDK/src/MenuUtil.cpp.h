//---------------------------------------------------------------------
// File:		MenuUtil.h
// Description:	Menu utility functions
// Created:		02/04/04
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

// some helper functions
// FindMenuItem() will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified 
// popup menu. It returns -1 if no such menu item string is found.
static int FindMenuItem(HMENU hMenu, LPCTSTR testString)
{
	TCHAR MenuString[256];
	ASSERT(hMenu);
	ASSERT(IsMenu(hMenu));

	int count = GetMenuItemCount(hMenu);
	for (int i = 0; i < count; i++)
	{
		if (GetMenuString(hMenu, i, MenuString, 255, MF_BYPOSITION) &&
			 (_tcscmp(testString, MenuString) == 0))
			return i;
	}

	return -1;
}

//=====================================================================
// Function:	MergeMenus
// Description:	merge in a menu into another one
// Parameters:	dMenu = destination menu
//				sMenu = source menu
//				foreStr = String in menu to insert before
// Returns:		0 if error, 1 if worked.
//=====================================================================
static int MergeMenus(HMENU *dMenu, HMENU sMenu, LPCTSTR foreStr)
{
	int i;
	int index;
	int subMenuCount;
	MENUITEMINFO tInfo;
	TCHAR menuName[256];
	int destMenuCount;

	index = FindMenuItem(sMenu, foreStr);
	VERIFY(index > -1);
	subMenuCount = GetMenuItemCount(sMenu);

	destMenuCount = GetMenuItemCount(*dMenu);

	for (i=0; i<index; i++)
	{
		tInfo.cbSize = sizeof(MENUITEMINFO);
		tInfo.fMask = MIIM_SUBMENU | MFT_STRING | MIIM_TYPE | MIIM_STATE | MIIM_CHECKMARKS |
			MFT_BITMAP | MIIM_ID | MIIM_DATA;
		tInfo.cch = sizeof(menuName)-1;
		tInfo.dwTypeData = menuName;
		if (!GetMenuItemInfo(sMenu, i, TRUE, &tInfo))
			return 0;
    	if (!InsertMenuItem(*dMenu, i, TRUE, &tInfo))
			return 0;
   	}

	for (i=index; i<subMenuCount; i++)
	{
		tInfo.cbSize = sizeof(MENUITEMINFO);
		tInfo.fMask = MIIM_SUBMENU | MFT_STRING | MIIM_TYPE | MIIM_STATE | MIIM_CHECKMARKS |
			MFT_BITMAP | MIIM_ID;
		tInfo.cch = sizeof(menuName)-1;
		tInfo.dwTypeData = menuName;
		if (!GetMenuItemInfo(sMenu, i, TRUE, &tInfo))
			return 0;
    	if (!InsertMenuItem(*dMenu, i+destMenuCount, TRUE, &tInfo))
			return 0;
	}

	return TRUE;
}

//=====================================================================
// Function:	MergeSubPopupMenu
// Description:	merge a sub popup menu into another menu 
// Parameters:	dMenu = destination menu
//				sMenu = source menu
// Returns:		0 if error, 1 if worked.
//=====================================================================
static int MergeSubPopupMenu(HMENU destMenu, HMENU srcMenu)
{
	int i, index;
	int destMenuCount;
	int srcMenuCount;
	MENUITEMINFO sMenuInfo;
	MENUITEMINFO dMenuInfo;
	TCHAR srcMenuName[256];
	TCHAR destMenuName[256];

	destMenuCount = GetMenuItemCount(destMenu);
	srcMenuCount = GetMenuItemCount(srcMenu);

	index = 0;
	dMenuInfo.cbSize = sizeof(MENUITEMINFO);
	dMenuInfo.fMask = MIIM_SUBMENU | MFT_STRING | MIIM_TYPE | MIIM_STATE | MIIM_CHECKMARKS |
		MFT_BITMAP | MIIM_ID | MIIM_DATA;
	dMenuInfo.cch = sizeof(destMenuName)-1;
	dMenuInfo.dwTypeData = destMenuName;
	destMenuName[0] ='\0';
	if (!GetMenuItemInfo(destMenu, index, TRUE, &dMenuInfo))
	{
		TRACE("Failed to get menu item info at line %d, file %s", __LINE__, __FILE__);
		return 0;
	}

	for (i=0; i<srcMenuCount; i++)
	{
		sMenuInfo.cbSize = sizeof(MENUITEMINFO);
		sMenuInfo.fMask = MIIM_SUBMENU | MFT_STRING | MIIM_TYPE | MIIM_STATE | MIIM_CHECKMARKS |
			MFT_BITMAP | MIIM_ID | MIIM_DATA;
		sMenuInfo.cch = sizeof(srcMenuName)-1;
		sMenuInfo.dwTypeData = srcMenuName;
		srcMenuName[0] ='\0';
		if (!GetMenuItemInfo(srcMenu, i, TRUE, &sMenuInfo))
		{
			TRACE("Failed to get menu item info at line %d, file %s", __LINE__, __FILE__);
			return 0;
		}


		// no match?
		if (strcmp(srcMenuName, destMenuName))
		{
	    	if (!InsertMenuItem(destMenu, index, TRUE, &sMenuInfo))
			{
				TRACE("Failed to insert menu item info at line %d, file %s", __LINE__, __FILE__);
				return 0;
			}

			index++;
		}
		else
		{
			index += destMenuCount;
		}
	}

	return TRUE;
}

//=====================================================================
// Function:	MergeSubMenus
// Description:	merge a sub menu into another menu into another one
// Parameters:	dMenu = destination menu
//				sMenu = source menu
//				aftStr = String in menu to insert after
// Returns:		0 if error, 1 if worked.
//=====================================================================
static int MergeSubMenus(HMENU *dMenu, HMENU sMenu)
{
	int i;
	int index;
	int srcMenuCount;
	int destMenuCount;
	MENUITEMINFO sMenuInfo;
	MENUITEMINFO dMenuInfo;
	TCHAR srcMenuName[256];
	TCHAR destMenuName[256];

	destMenuCount = GetMenuItemCount(*dMenu);
	srcMenuCount = GetMenuItemCount(sMenu);

	// destination index we are inserting before
	index = 0;

	for (i=0; i<srcMenuCount; i++)
	{
		sMenuInfo.cbSize = sizeof(MENUITEMINFO);
		sMenuInfo.fMask = MIIM_SUBMENU | MFT_STRING | MIIM_TYPE | MIIM_STATE | MIIM_CHECKMARKS |
			MFT_BITMAP | MIIM_ID | MIIM_DATA;
		sMenuInfo.cch = sizeof(srcMenuName)-1;
		sMenuInfo.dwTypeData = srcMenuName;
		srcMenuName[0] ='\0';
		if (!GetMenuItemInfo(sMenu, i, TRUE, &sMenuInfo))
		{
			TRACE("Failed to get menu item info at line %d, file %s", __LINE__, __FILE__);
			return 0;
		}

		dMenuInfo.cbSize = sizeof(MENUITEMINFO);
		dMenuInfo.fMask = MIIM_SUBMENU | MFT_STRING | MIIM_TYPE | MIIM_STATE | MIIM_CHECKMARKS |
			MFT_BITMAP | MIIM_ID | MIIM_DATA;
		dMenuInfo.cch = sizeof(destMenuName)-1;
		dMenuInfo.dwTypeData = destMenuName;
		destMenuName[0] ='\0';
		if ((index < destMenuCount) && !GetMenuItemInfo(*dMenu, index, TRUE, &dMenuInfo))
		{
				TRACE("Failed to get menu item info at line %d, file %s", __LINE__, __FILE__);
				return 0;
		}

		// no match?
		if (strcmp(srcMenuName, destMenuName))
		{
	    	if (!InsertMenuItem(*dMenu, index, TRUE, &sMenuInfo))
			{
				TRACE("Failed to insert menu item info at line %d, file %s", __LINE__, __FILE__);
				return 0;
			}
			destMenuCount++;
		}
		else
		{
			if ((sMenuInfo.hSubMenu == NULL) || (dMenuInfo.hSubMenu == NULL))
			{
				TRACE("Unable to find sub menus in in file %s\n", __FILE__);
				return 0;
			}

			if (!MergeSubPopupMenu(dMenuInfo.hSubMenu, sMenuInfo.hSubMenu))
			{
				return 0;
			}
		}

	index++;

   	}

	return TRUE;
}

//=====================================================================
// Function:	MergeAccelerators
// Description:	merge in a accelerators into another one
// Parameters:	dAccel = pointer to handle of destination accelerators
//				sAccel = handle of source accelerators to copy from
//				Overwrites dAccel handle with newly merged handle
// Returns:		0 if error, 1 if worked.
//=====================================================================
static int MergeAccelerators(HACCEL *dAccel, HACCEL sAccel)
{
	int nAccels1, nAccels2;
	HACCEL tAccel;
	LPACCEL mergedAccel;

	if ((*dAccel == NULL) && (sAccel == NULL))
	{
		return TRUE;
	}

	// count total accelerators
	nAccels1 = CopyAcceleratorTable(*dAccel, NULL, 0);
	nAccels2 = CopyAcceleratorTable(sAccel, NULL, 0);

	// temporarily create accelerator table
	mergedAccel = (LPACCEL)malloc((nAccels1+nAccels2) * sizeof(ACCEL));
	if (mergedAccel == NULL)
	{
		return FALSE;
	}

	CopyAcceleratorTable(*dAccel, mergedAccel , nAccels1);
	CopyAcceleratorTable(sAccel, &mergedAccel[nAccels1], nAccels2);
	tAccel = CreateAcceleratorTable(mergedAccel, nAccels1+nAccels2);
	if (tAccel == NULL)
	{
		return FALSE;
	}

	if (*dAccel && !DestroyAcceleratorTable(*dAccel))
	{
		return FALSE;
	}

	*dAccel = tAccel;

	free(mergedAccel);

	return TRUE;
}


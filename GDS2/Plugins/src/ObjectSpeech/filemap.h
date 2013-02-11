#ifndef CFileMap_classes_H
#define CFileMap_classes_H

/********************************************************************
    created:    30:10:2001   15:27
    filename:   filemap.h
    author:      Vitali Brusentsev
    e-mail:       retalik@mail.ru
    version:     1.1
    purpose:    memory-mapped files stuff

*********************************************************************/

#define CFILE_INIT hFile(INVALID_HANDLE_VALUE), hMap(0), lpMapView(0), dwSize(0)

class CFileMap
{
public:
    CFileMap(): CFILE_INIT {}
    CFileMap(LPCTSTR path, bool write=false): CFILE_INIT
    {
	Open(path, write); 
    }
    CFileMap(LPCTSTR path, DWORD size): CFILE_INIT
    { 
        if(size) Create(path, size);
    }
    ~CFileMap() { CloseMap(); }
   // operator bool(){ return (hFile!=INVALID_HANDLE_VALUE); }
    void    CloseMap();
    bool    Open(LPCTSTR path, bool write);
    bool    Create(LPCTSTR path, DWORD size);

    BYTE*   Base(){ return reinterpret_cast<BYTE*>(lpMapView); }
    DWORD   Size(){ return dwSize; }
protected:
    DWORD   dwSize;
private:
    bool    OpenInternal(LPCTSTR path, DWORD dwAccess, DWORD dwCreation, DWORD flProtect, 
                        DWORD dwPageAccess, DWORD size=0);
    HANDLE  hFile, hMap;
    LPVOID  lpMapView;
};
  
inline void CFileMap::CloseMap()
{
    if(hMap) CloseHandle(hMap);
    hMap=0;
    if(lpMapView) UnmapViewOfFile(lpMapView);
    lpMapView=0;
    if(hFile!=INVALID_HANDLE_VALUE) CloseHandle(hFile);
    hFile=INVALID_HANDLE_VALUE;
    dwSize=0;
}


inline bool CFileMap::OpenInternal(LPCTSTR path, DWORD dwAccess, DWORD dwCreation, 
                                   DWORD flProtect, DWORD dwPageAccess, DWORD size)
{
    if(hFile!=INVALID_HANDLE_VALUE) CloseMap();
    hFile=CreateFile(path, dwAccess, FILE_SHARE_READ, 0, dwCreation, 
                       FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile==INVALID_HANDLE_VALUE)
    {
        return false;
    }
    
    if(!size) size=GetFileSize(hFile, 0);
    hMap=CreateFileMapping(hFile, 0, flProtect, 0, size, 0);
    if(!hMap)
    {
        CloseMap();
        return false;
    }

    lpMapView=MapViewOfFile(hMap, dwPageAccess, 0, 0, size);
    if(!lpMapView)
    {
        CloseMap();
        return false;
    }
    dwSize=size;
    return true;
}

inline bool CFileMap::Create(LPCTSTR path, DWORD size)
{
    return OpenInternal(path, GENERIC_READ|GENERIC_WRITE, CREATE_ALWAYS, 
                        PAGE_READWRITE, FILE_MAP_WRITE, size);
}

inline bool CFileMap::Open(LPCTSTR path, bool write)
{
    if(write) return OpenInternal(path, GENERIC_READ|GENERIC_WRITE, OPEN_EXISTING, 
                                  PAGE_READWRITE, FILE_MAP_READ|FILE_MAP_WRITE);
    else return OpenInternal(path, GENERIC_READ, OPEN_EXISTING, PAGE_READONLY, FILE_MAP_READ);
}

#endif //CFileMap_classes_H

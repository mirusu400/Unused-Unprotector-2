#pragma once

#include <windows.h>
#include <malloc.h>
#include <assert.h>
#include "sfmpq_static.h"

#define DEFAULTSECTION 128

#define ORIGINAL ((CHAR)0x3b)
#define HYBRID ((CHAR)0x3f)
#define BROODWAR ((CHAR)0xCD)

#define nTYPE (0x45505954)
#define nVER  (0x20524556)
#define nIVER (0x52455649)
#define nIVE2 (0x32455649)
#define nVCOD (0x444F4356)
#define nIOWN (0x4E574F49)
#define nOWNR (0x524E574F)
#define nERA  (0x20415245)
#define nDIM  (0x204D4944)
#define nSIDE (0x45444953)
#define nMTXM (0x4D58544D)
#define nPUNI (0x494E5550)
#define nUPGR (0x52475055)
#define nPTEC (0x43455450)
#define nUNIT (0x54494E55)
#define nISOM (0x4D4F5349)
#define nTILE (0x454C4954)
#define nDD2  (0x20324444)
#define nTHG2 (0x32474854)
#define nMASK (0x4B53414D)
#define nSTR  (0x20525453)
#define nUPRP (0x50525055)
#define nUPUS (0x53555055)
#define nMRGN (0x4E47524D)
#define nTRIG (0x47495254)
#define nMBRF (0x4652424D)
#define nSPRP (0x50525053)
#define nFORC (0x43524F46)
#define nWAV  (0x20564157)
#define nUNIS (0x53494E55)
#define nUPGS (0x53475055)
#define nTECS (0x53434554)
#define nSWNM (0x4D4E5753)
#define nCOLR (0x524C4F43)
#define nPUPx (0x78505550)
#define nPTEx (0x78455450)
#define nUNIx (0x78494E55)
#define nUPGx (0x78475055)
#define nTECx (0x78434554)

class Section
{
protected:
	DWORD m_dwName;
	DWORD m_dwSize;
	PVOID m_pMain;
public:
	bool operator ==(const Section &t) const ;
	bool operator <(const Section &t) const ;
	bool operator <=(const Section &t) const {return *this<t || *this==t; }
	bool operator >=(const Section &t) const {return !(*this<t);}
	bool operator >(const Section &t) const {return !(*this<=t);}
	bool operator !=(const Section &t) const {return !(*this==t);}
	Section& operator=(const Section &t)
	{
		m_dwName=t.m_dwName;
		m_pMain=realloc(m_pMain,m_dwSize=t.m_dwSize);
		CopyMemory(m_pMain,t.m_pMain,m_dwSize);
		return *this;
	}
	Section() { m_pMain = NULL; m_dwName = 0, m_dwSize = 0;}
	Section(const Section &t)
	{
		m_dwName=t.m_dwName;
		m_pMain=malloc(m_dwSize=t.m_dwSize);
		CopyMemory(m_pMain,t.m_pMain,m_dwSize);
	}
	Section(DWORD dwName, DWORD dwSize, PVOID pMain)
	{
		m_dwName = dwName;
		m_dwSize = dwSize;
		m_pMain = malloc(dwSize);
		CopyMemory(m_pMain, pMain, dwSize);
	}
	~Section() { free(m_pMain); }
	DWORD GetName() const { return m_dwName; }
	DWORD GetSize() const { return m_dwSize; }
	LPCVOID GetMain() const { return m_pMain; }
	DWORD SetName(DWORD dwNewName)
	{
		DWORD dwRet = m_dwName;
		return (m_dwName = dwNewName, dwRet);
	};
	VOID SetMain(LPCVOID pMain, size_t Length, UINT Offset, bool bOverwrite)
	{
		if (bOverwrite || Offset+Length > m_dwSize)
		{
			m_pMain = realloc(m_pMain, m_dwSize = Offset+Length);
		}
		CopyMemory((LPBYTE)m_pMain+Offset, pMain, Length);
	}
	VOID Release()
	{
		free(m_pMain);
		m_pMain = 0;
		m_dwName = m_dwSize = 0;
	}
};

typedef BOOL (__stdcall *openarchive)(LPCSTR,DWORD,DWORD,PHANDLE);
typedef BOOL (__stdcall *closearchive)(HANDLE);
typedef BOOL (__stdcall *openfileex)(HANDLE,LPCSTR,DWORD,PHANDLE);
typedef BOOL (__stdcall *closefile)(HANDLE);
typedef DWORD (__stdcall *getfilesize)(HANDLE,PDWORD);
typedef BOOL (__stdcall *readfile)(HANDLE,LPVOID,DWORD,PDWORD,LPOVERLAPPED);

class CUnprotectMapW;

class CUnprotectMapA
{
	friend class CUnprotectMapW;
protected:
	HANDLE m_hFile;
	CHAR m_szPath[MAX_PATH];
	Section *m_Sec;
	CHAR m_chkVer;
	BYTE vcod[1040];
	DWORD dwSectionCount;
	bool m_bChk;
	int  m_bRewriteLocationName;
	int  m_bDeleteComment;
	int  m_bDeleteNeverTrigger;
	int  m_bDeleteDisabledTrigger;
	int  m_nWavCompress;
	int  m_bUseStorm;
	int  m_bAllowOverwrite;
	int  FindIndex(DWORD dwSectionName, int nStart = 0);
	void RepairTerrain();
	void RepairLocation();
	void RepairTrigger();
	void RepairUNISection();
	void RepairUnusedSection();
	void RepairStr();
	void RepairWAV();
	void RepairVCOD();
	void DeleteOverlappedSection();

	static openarchive SFileOpenArchive;
	static closearchive SFileCloseArchive;
	static openfileex SFileOpenFileEx;
	static closefile SFileCloseFile;
	static getfilesize SFileGetFileSize;
	static readfile SFileReadFile;
public:
	CUnprotectMapA() : m_hFile(0), m_chkVer(0), m_bRewriteLocationName(true),
	                   m_bDeleteComment(true), m_bDeleteNeverTrigger(true),
					   m_bDeleteDisabledTrigger(true),
					   m_nWavCompress(MAWA_QUALITY_HIGH),
					   m_bAllowOverwrite(false),
					   dwSectionCount(DEFAULTSECTION),
					   m_Sec(new Section[DEFAULTSECTION]) {}
	CUnprotectMapA(const CUnprotectMapA &t) { assert(0); }
	~CUnprotectMapA() {delete[] m_Sec; CloseHandle(m_hFile); }
	void SetVCOD(PVOID pVCOD) { CopyMemory(vcod, pVCOD, 1040); }
	bool OpenMap(LPCSTR lpMapFile);
	void CloseMap();
	bool Unprotect();
	bool SaveMap(LPCSTR lpFileName, bool bSaveAsChk = false, bool bAddListFile = true, const char *ProgName=NULL);
	char GetVersion() {return m_chkVer; }
	void ResizeSectionCount()
	{
		Section *tmp=new Section[dwSectionCount<<=1];
		int i;
		for (i=0; i<(int)(dwSectionCount>>1); i++)
		{
			tmp[i]=m_Sec[i];
		}
		delete[] m_Sec;
		m_Sec=tmp;
	}
	void GetMapName(LPSTR lpString,int cbSize)
	{
		if (!m_chkVer) return;
		DWORD dwStrMapName=*(const WORD *)m_Sec[FindIndex(0x50525053)].GetMain();
		// 0x50525053: SPRP
		LPWORD lpStr=(LPWORD)m_Sec[FindIndex(0x20525453)].GetMain();
		// 0x20525453: STR
		lstrcpynA(lpString, (LPCSTR)lpStr + lpStr[dwStrMapName], cbSize);
	}
	int GetStringCount()
	{
		if (!m_chkVer) return -1;
		else return *(LPWORD)m_Sec[FindIndex(nSTR)].GetMain();
	}
	LPCSTR GetFileName() { return m_szPath; }
	// 0:·ÎÄÉÀÌ¼Ç 1:ÄÚ¸àÆ® 2:³×¹ö 3:Æ®¸®°Å 4:½ºÅè 5:µ¤¾î¾º¿ì±â 6:¾ÐÃà
	int  &GetOptionRef(int nOption) {switch(nOption){
		case 0:return m_bRewriteLocationName;
		case 1:return m_bDeleteComment;
		case 2:return m_bDeleteNeverTrigger;
		case 3:return m_bDeleteDisabledTrigger;
		case 4:return m_bUseStorm;
		case 5:return m_bAllowOverwrite;
		default:return m_nWavCompress;}
	}
	static bool Initialize(HMODULE hStorm)
	{
		BOOL (__stdcall *ptr)()=(BOOL (__stdcall *)())GetProcAddress(hStorm, (LPCSTR)0x106);
		if (!ptr) goto fail;
		if (!(SFileOpenArchive=(openarchive)GetProcAddress(hStorm,(LPCSTR)0x10A))) goto fail;
		if (!(SFileCloseArchive=(closearchive)GetProcAddress(hStorm,(LPCSTR)0xFC))) goto fail;
		if (!(SFileOpenFileEx=(openfileex)GetProcAddress(hStorm,(LPCSTR)0x10C))) goto fail;
		if (!(SFileCloseFile=(closefile)GetProcAddress(hStorm,(LPCSTR)0xFD))) goto fail;
		if (!(SFileGetFileSize=(getfilesize)GetProcAddress(hStorm,(LPCSTR)0x109))) goto fail;
		if (!(SFileReadFile=(readfile)GetProcAddress(hStorm,(LPCSTR)0x10D))) goto fail;
		ptr();
		return true;
fail:
		SFileOpenArchive=::SFileOpenArchive;
		SFileCloseArchive=::SFileCloseArchive;
		SFileOpenFileEx=::SFileOpenFileEx;
		SFileCloseFile=::SFileCloseFile;
		SFileGetFileSize=::SFileGetFileSize;
		SFileReadFile=::SFileReadFile;
		return false;
	}
	bool ChangeFunctionSet(bool storm,HMODULE hStorm=NULL)
	{
		if (storm)
		{
			if (!(SFileOpenArchive=(openarchive)GetProcAddress(hStorm,(LPCSTR)0x10A))) goto fail;
			if (!(SFileCloseArchive=(closearchive)GetProcAddress(hStorm,(LPCSTR)0xFC))) goto fail;
			if (!(SFileOpenFileEx=(openfileex)GetProcAddress(hStorm,(LPCSTR)0x10C))) goto fail;
			if (!(SFileCloseFile=(closefile)GetProcAddress(hStorm,(LPCSTR)0xFD))) goto fail;
			if (!(SFileGetFileSize=(getfilesize)GetProcAddress(hStorm,(LPCSTR)0x109))) goto fail;
			if (!(SFileReadFile=(readfile)GetProcAddress(hStorm,(LPCSTR)0x10D))) goto fail;
fail:return false;
		}
		else
		{
			SFileOpenArchive=::SFileOpenArchive;
			SFileCloseArchive=::SFileCloseArchive;
			SFileOpenFileEx=::SFileOpenFileEx;
			SFileCloseFile=::SFileCloseFile;
			SFileGetFileSize=::SFileGetFileSize;
			SFileReadFile=::SFileReadFile;
			return true;
		}
	}
};

class CUnprotectMapW
{
protected:
	CUnprotectMapA m_map;
	WCHAR m_szPath[MAX_PATH];
public:
	CUnprotectMapW() {}
	CUnprotectMapW(const CUnprotectMapW &t) {assert(0); }
	void SetVCOD(PVOID pVCOD) { m_map.SetVCOD(pVCOD); }
	bool OpenMap(LPCWSTR lpMapFile)
	{
		CHAR path[MAX_PATH]={0};
		WideCharToMultiByte(CP_ACP,0,lpMapFile,lstrlenW(lpMapFile),path,MAX_PATH,NULL,NULL);
		return m_map.OpenMap(path);
	}
	void CloseMap()
	{
		m_map.CloseMap();
	}
	bool Unprotect()
	{
		return m_map.Unprotect();
	}
	bool SaveMap(LPCWSTR lpFileName, bool bSaveAsChk = false, bool bAddListFile = true, const wchar_t *ProgName=NULL)
	{
		CHAR path[MAX_PATH]={0};
		CHAR progname[256]={0};
		WideCharToMultiByte(CP_ACP,0,lpFileName,lstrlenW(lpFileName),path,MAX_PATH,NULL,NULL);
		if (ProgName) WideCharToMultiByte(CP_ACP,0,ProgName,lstrlenW(ProgName),progname,256,0,0);
		return m_map.SaveMap(path,bSaveAsChk,bAddListFile,ProgName?progname:NULL);
	}
	char GetVersion()
	{
		return m_map.GetVersion();
	}
	void GetMapName(LPWSTR lpString,int cbSize)
	{
		if (!m_map.m_chkVer) return;
		LPSTR lpStringAnsi=new CHAR[cbSize<<1];
		m_map.GetMapName(lpStringAnsi, cbSize<<1);
		WideCharToMultiByte(CP_ACP,0,lpString,cbSize,lpStringAnsi,lstrlenA(lpStringAnsi),NULL,NULL);
	}
	int GetStringCount() { return m_map.GetStringCount(); }
	LPCWSTR GetFileName() {WideCharToMultiByte(CP_ACP,0,m_szPath,MAX_PATH,
		m_map.m_szPath,lstrlenA(m_map.m_szPath),NULL,NULL); return m_szPath; }
	// 0:·ÎÄÉÀÌ¼Ç 1:ÄÚ¸àÆ® 2:³×¹ö 3:Æ®¸®°Å 4:½ºÅè 5:µ¤¾î¾º¿ì±â 6:¾ÐÃà
	int &GetOptionRef(int nOption) {return m_map.GetOptionRef(nOption);}
	static bool Initialize(HMODULE hStorm)
	{
		return CUnprotectMapA::Initialize(hStorm);
	}
	bool ChangeFunctionSet(bool storm,HMODULE hStorm=NULL)
	{
		return m_map.ChangeFunctionSet(storm,hStorm);
	}
};

#pragma pack(push, 1)

typedef struct tagFORCE {
	BYTE cPlayerInForce[8];
	WORD wForceString[4];
	BYTE cForceSetting[4];
} FORCE;

typedef struct tagTRIGGERCONDITION {
	DWORD dwLocation;
	DWORD dwPlayer;
	DWORD dwResource;
	WORD wUnitID;
	BYTE bSetting;
	BYTE bCondition;
	BYTE bResourceSetting;
	BYTE bProperty;
	WORD wUnused;
} TRIGGERCONDITION;

typedef struct tagTRIGGERACTION {
	DWORD dwLocation;
	DWORD dwString;
	DWORD dwSoundString;
	DWORD dwTime;
	DWORD dwPlayer;
	DWORD dwSecondPlayer;
	WORD wSetting;
	BYTE bAction;
	BYTE bNum;
	BYTE bProperty;
	BYTE bUnused[3];
} TRIGGERACTION;

typedef struct tagTRIGGERPLAYER {
	DWORD dwUnused;
	BYTE bPlayer[28];
} TRIGGERPLAYER;

typedef struct tagTRIGGER {
	TRIGGERCONDITION Condition[16];
	TRIGGERACTION Action[64];
	TRIGGERPLAYER Player;
} TRIGGER;

typedef struct tagBRIEFINGACTION {
	DWORD dwUnused;
	DWORD dwString;
	DWORD dwSoundString;
	DWORD dwTime;
	DWORD dwSlot;
	DWORD dwUnused2;
	WORD wUnitID;
	BYTE bAction;
	BYTE bSetting;
	BYTE bProperty;
	BYTE bUnused[3];
} BRIEFINGACTION;

typedef struct tagBRIEFING {
	BYTE bUnused[320];
	BRIEFINGACTION Action[64];
	TRIGGERPLAYER Player;
} BRIEFING;

typedef struct tagLOCATION {
	RECT rt;
	WORD wString;
	WORD wProperty;
} LOCATION;

#pragma pack(pop)

#define ISTRIGGERDISABLED(trigger) ((trigger).bProperty & 2)
#define ENABLEACTION(triggeraction,bool_enable) (void)((triggeraction).bProperty |= (!!bool_enable) * 2)

#ifdef UNICODE
typedef CUnprotectMapW CUnprotectMap;
#else
typedef CUnprotectMapA CUnprotectMap;
#endif
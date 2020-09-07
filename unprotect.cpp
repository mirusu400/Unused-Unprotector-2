#include "stdafx.h"

#define ARSIZE(ar) (sizeof(ar)/sizeof((ar)[0]))

const DWORD dwSectionList[] = {nTYPE,nVER ,nIVER,nIVE2,nVCOD,nIOWN,nOWNR,nERA ,nDIM ,nSIDE,nMTXM,nPUNI,
							   nUPGR,nPTEC,nUNIT,nISOM,nTILE,nDD2 ,nTHG2,nMASK,nSTR ,nUPRP,nUPUS,nMRGN,
							   nTRIG,nMBRF,nSPRP,nFORC,nWAV ,nUNIS,nUPGS,nTECS,nSWNM,nCOLR,nPUPx,nPTEx,
							   nUNIx,nUPGx,nTECx};

template <typename T>
void DeleteItemInArray(T *array, size_t arraysize, int item)
{
	array[item].~T();
	MoveMemory(array+item,array+item+1,sizeof(T)*(arraysize-item-1));
	ZeroMemory(array+arraysize-1,sizeof(T));
}

bool Section::operator ==(const Section &t) const
{
	return std::find(&dwSectionList[0],&dwSectionList[ARSIZE(dwSectionList)],m_dwName) ==
		std::find(&dwSectionList[0],&dwSectionList[ARSIZE(dwSectionList)],t.m_dwName);
}

bool Section::operator <(const Section &t) const
{
	return std::find(&dwSectionList[0],&dwSectionList[ARSIZE(dwSectionList)],m_dwName) <
		std::find(&dwSectionList[0],&dwSectionList[ARSIZE(dwSectionList)],t.m_dwName);
}

static bool IsValidLocation(LPCRECT rt)
{
	if (!rt->left && !rt->top) return rt->left < rt->right && rt->top < rt->bottom;
	else return rt->left <= rt->right && rt->top <= rt->bottom;
}

openarchive CUnprotectMapA::SFileOpenArchive=0;
closearchive CUnprotectMapA::SFileCloseArchive=0;
openfileex CUnprotectMapA::SFileOpenFileEx=0;
closefile CUnprotectMapA::SFileCloseFile=0;
getfilesize CUnprotectMapA::SFileGetFileSize=0;
readfile CUnprotectMapA::SFileReadFile=0;

int CUnprotectMapA::FindIndex(DWORD dwSectionName, int nStart)
{
	int i;
	for (i = nStart; i < dwSectionCount; i++)
	{
		if (m_Sec[i].GetName() == dwSectionName) break;
	}
	return i;
}

bool CUnprotectMapA::OpenMap(LPCSTR lpMapFile)
{
	const DWORD dwNeedOriginalSection[] = {nVER, nVCOD, nOWNR, nERA, nDIM, nSIDE, nMTXM, nPUNI, nUPGR, nPTEC,
										nUNIT, nTHG2, nMASK, nSTR, nUPRP, nMRGN, nTRIG, nSPRP,
										nFORC, nUNIS, nUPGS, nTECS, 0};

	const DWORD dwNeedHybridSection[] = {nVER, nVCOD, nOWNR, nERA, nDIM, nSIDE, nMTXM, nPUNI, nUNIT, nTHG2,
									nMASK, nSTR, nUPRP, nMRGN, nTRIG, nSPRP, nFORC, nPUPx, nPTEx,
									nUNIx, nUPGx, nTECx, 0};

	const DWORD dwNeedBroodwarSection[] = {nVER, nVCOD, nOWNR, nERA, nDIM, nSIDE, nMTXM, nPUNI, nUNIT, nTHG2,
									nMASK, nSTR, nUPRP, nMRGN, nTRIG, nSPRP, nFORC, nCOLR, nPUPx,
									nPTEx, nUNIx, nUPGx, nTECx, 0};
	
	const DWORD dwDummySection[] = {nTYPE, nIVER, nIVE2, nIOWN, nISOM, nTILE, nUPUS, nWAV, nSWNM};
	const DWORD dwDummyInHybrid[] = {nUPGR, nPTEC, nUNIS, nUPGS, nTECS};

	HANDLE hMPQ, hChk;
	DWORD dwFileSize = 0xFFFFFFFF,dummy;
	PVOID pBuffer,pBufTmp;
	DWORD dwTmpname, dwTmpsize;
	int ptr=0;
	int i;
	const DWORD *pArray;
	m_hFile = CreateFileA(lpMapFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

	// 파일 존재하면
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		// MPQ인지 검사
		if (SFileOpenArchive(lpMapFile,0,0,&hMPQ))
		{
			// MPQ이고 올바른 파일인지 검사
			if (SFileOpenFileEx(hMPQ,"staredit\\scenario.chk",0,&hChk))
			{
				dwFileSize = SFileGetFileSize(hChk,NULL);
				pBuffer = malloc(dwFileSize);
				SFileReadFile(hChk,pBuffer,dwFileSize,&dummy,NULL);
				SFileCloseFile(hChk);
				m_bChk = false;
			}
			SFileCloseArchive(hMPQ);
		}
		else // CHK
		{
			dwFileSize = GetFileSize(m_hFile, NULL);
			pBuffer = malloc(dwFileSize);
			ReadFile(m_hFile, pBuffer, dwFileSize, &dummy, NULL);
			m_bChk = true;
		}
	}
	else return false;

	pBufTmp = pBuffer;

	if (dwFileSize == -1)
	{
		CloseHandle(m_hFile);
		return false;
	}

	while (dwFileSize >= 8)
	{
		dwTmpname = *(PDWORD)pBuffer;
		dwTmpsize = *((PDWORD)pBuffer+1);
		if (dwTmpsize > dwFileSize - 8) break;
		if (std::find(&dwSectionList[0],&dwSectionList[ARSIZE(dwSectionList)],dwTmpname) !=
			&dwSectionList[ARSIZE(dwSectionList)])
		{
			m_Sec[ptr].SetName(dwTmpname);
			m_Sec[ptr++].SetMain((PBYTE)pBuffer+8, dwTmpsize, 0, true);
		}
		if (ptr == dwSectionCount) ResizeSectionCount();
		dwFileSize = dwFileSize - 8 - dwTmpsize;
		pBuffer = (PBYTE)pBuffer + 8 + dwTmpsize;
	}

	free(pBufTmp);

	dummy = FindIndex(nVER);
	if (dummy == dwSectionCount) goto fail;
	m_chkVer = *(CHAR *)m_Sec[dummy].GetMain();

	if (m_chkVer == ORIGINAL) pArray=dwNeedOriginalSection;
	else if(m_chkVer == HYBRID) pArray=dwNeedHybridSection;
	else if(m_chkVer == BROODWAR) pArray=dwNeedBroodwarSection;
	else goto fail;

	for (i = 0; pArray[i]; i++)
	{
		if (FindIndex(pArray[i]) == dwSectionCount) goto fail;
	}

	for (i = 0; i < ARSIZE(dwDummySection); i++)
	{
		while ((ptr=FindIndex(dwDummySection[i])) < dwSectionCount) m_Sec[ptr].Release();
	}

	if (m_chkVer == HYBRID)
	{
		for (i = 0; i < ARSIZE(dwDummyInHybrid); i++)
		{
			while ((ptr=FindIndex(dwDummyInHybrid[i])) < dwSectionCount) m_Sec[ptr].Release();
		}
	}

	lstrcpyA(m_szPath, lpMapFile);
	return true;
fail:
	CloseMap();
	return false;
}

void CUnprotectMapA::CloseMap()
{
	CloseHandle(m_hFile);
	delete[] m_Sec;
	m_Sec=new Section[dwSectionCount=DEFAULTSECTION];
	m_chkVer = 0;
}

void CUnprotectMapA::RepairTerrain()
{
	int *idxMTXM, numMTXM, i = -1, j = -1, idxERA = FindIndex(nERA);

	for (numMTXM = 0; (i = FindIndex(nMTXM, i+1)) < dwSectionCount; numMTXM++);
	idxMTXM = new int[numMTXM];

	for (i = 0; i < numMTXM; i++)
	{
		j = idxMTXM[i] = FindIndex(nMTXM, j+1);
	}

	for (i = numMTXM - 1; i >= 1; i--)
	{
		m_Sec[idxMTXM[i-1]].SetMain(m_Sec[idxMTXM[i]].GetMain(), m_Sec[idxMTXM[i]].GetSize(), 0, false);
		m_Sec[idxMTXM[i]].Release();
	}

	(*(WORD *)m_Sec[idxERA].GetMain()) %= 8;
	
	delete[] idxMTXM;
}

void CUnprotectMapA::RepairTrigger()
{
	int *idxTRIG, numTRIG, i = -1, j = -1;
	TRIGGER *pTRIG;

	for (numTRIG = 0; (i = FindIndex(nTRIG, i+1)) < dwSectionCount; numTRIG++);
	idxTRIG = new int[numTRIG];

	for (i = 0; i < numTRIG; i++)
	{
		j = idxTRIG[i] = FindIndex(nTRIG, j+1);
	}

	for (i = numTRIG - 1; i >= 1; i--)
	{
		m_Sec[idxTRIG[i-1]].SetMain(m_Sec[idxTRIG[i]].GetMain(), m_Sec[idxTRIG[i]].GetSize(),
			m_Sec[idxTRIG[i-1]].GetSize(), false);
		m_Sec[idxTRIG[i]].Release();
	}

	numTRIG = m_Sec[idxTRIG[0]].GetSize()/2400;
	pTRIG = new TRIGGER[numTRIG];
	CopyMemory(pTRIG, m_Sec[idxTRIG[0]].GetMain(), m_Sec[idxTRIG[0]].GetSize());

	for (i = 0; i < numTRIG; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (pTRIG[i].Condition[j].bCondition > 23)
			{
del:
				DeleteItemInArray(pTRIG, numTRIG--, i--);
				goto outroop;
			}
			else if (m_bDeleteNeverTrigger && pTRIG[i].Condition[j].bCondition == 23 && !ISTRIGGERDISABLED(pTRIG[i].Condition[j]))
				goto del;
			else if (m_bDeleteDisabledTrigger && ISTRIGGERDISABLED(pTRIG[i].Condition[j]))
				DeleteItemInArray(pTRIG[i].Condition, ARSIZE(pTRIG[i].Condition),j--);
		}
		if (pTRIG[i].Condition[0].bCondition == 0)
			pTRIG[i].Condition[0].bCondition = 22;
		for (j = 0; j < 64; j++)
		{
			if (m_bDeleteComment && pTRIG[i].Action[j].bAction == 47 && !ISTRIGGERDISABLED(pTRIG[i].Action[j]))
			{
				DeleteItemInArray(pTRIG[i].Action,64,j--);
			}
			else if (m_bDeleteDisabledTrigger && ISTRIGGERDISABLED(pTRIG[i].Action[j]))
				DeleteItemInArray(pTRIG[i].Action, ARSIZE(pTRIG[i].Action), j--);
		}
outroop:;
	}

	m_Sec[idxTRIG[0]].SetMain(pTRIG, numTRIG * sizeof(TRIGGER), 0, true);
	delete[] idxTRIG;
	delete[] pTRIG;
}

void CUnprotectMapA::RepairLocation()
{
	int idxMRGN = FindIndex(nMRGN)/*, idxTRIG = FindIndex(nTRIG)*/;
	int i;
	LOCATION *pMrgn = (LOCATION *)m_Sec[idxMRGN].GetMain();

	for (i = 0; i < (int)(m_Sec[idxMRGN].GetSize() / sizeof(LOCATION)); i++)
	{
		if (!IsValidLocation(&pMrgn[i].rt)) ZeroMemory(&pMrgn[i],sizeof(LOCATION));
		if (m_bRewriteLocationName) pMrgn[i].wString = 0;
	}
}

void CUnprotectMapA::RepairUnusedSection()
{
#define CHECKPTR if(ptr==dwSectionCount)ResizeSectionCount()
	int idx, ptr = -1, i;

	DWORD dwVerMain, dwTemp, dwTemp2, dwSize;
	PVOID pBuffer;

	// 1. TYPE, IVER, IVE2
	while (m_Sec[++ptr].GetName());
	CHECKPTR;

	if (m_chkVer == ORIGINAL) // need IVER Section
	{
		dwVerMain = 0x0A;
		m_Sec[ptr].SetName(nIVER);
		m_Sec[ptr].SetMain(&dwVerMain,2,0,true);
	}
	else // need IVE2 Section
	{
		dwVerMain = 0x0B;
		m_Sec[ptr].SetName(nIVE2);
		m_Sec[ptr].SetMain(&dwVerMain,2,0,true);

		while (m_Sec[++ptr].GetName());
		CHECKPTR;

		dwVerMain = (m_chkVer == HYBRID) ? ('R' + 'A'*0x100 +'W'*0x10000 +'S'*0x1000000) :
			('R' + 'A'*0x100 +'W'*0x10000 +'B'*0x1000000);
		m_Sec[ptr].SetName(nTYPE);
		m_Sec[ptr].SetMain(&dwVerMain,4,0,true);
	}

	// 2. IOWN
	while (m_Sec[++ptr].GetName());
	CHECKPTR;

	idx=FindIndex(nOWNR);
	m_Sec[ptr].SetName(nIOWN);
	m_Sec[ptr].SetMain(m_Sec[idx].GetMain(),m_Sec[idx].GetSize(),0,true);

	// 3. ISOM/TILE/DD2
	while (m_Sec[++ptr].GetName());
	CHECKPTR;

	idx=FindIndex(nDIM);
	dwVerMain = *(DWORD *)m_Sec[idx].GetMain();

	dwTemp = LOWORD(dwVerMain), dwTemp2 = HIWORD(dwVerMain);

	pBuffer = malloc(dwSize = ((dwTemp % 2)? 4*(dwTemp+1)*(dwTemp2+1):4*(dwTemp+2)*(dwTemp2+1)));

	for (i = 0; i < (int)(dwSize / 2); i++)
	{
		((WORD *)pBuffer)[i] = 0x10;
	}

	m_Sec[ptr].SetName(nISOM);
	m_Sec[ptr].SetMain(pBuffer, dwSize, 0, true);

	while (m_Sec[++ptr].GetName());
	CHECKPTR;

	idx=FindIndex(nMTXM);
	m_Sec[ptr].SetName(nTILE);
	m_Sec[ptr].SetMain(m_Sec[idx].GetMain(),m_Sec[idx].GetSize(),0,true);

	if (FindIndex(nDD2) == dwSectionCount)
	{
		while (m_Sec[++ptr].GetName());
		CHECKPTR;
		m_Sec[ptr].SetName(nDD2);
		m_Sec[ptr].SetMain(0,0,0,true);
	}

	// 4. UPUS

	while (m_Sec[++ptr].GetName());
	CHECKPTR;
	m_Sec[ptr].SetName(nUPUS);
	pBuffer = realloc(pBuffer, 64);
	ZeroMemory(pBuffer, 64);
	m_Sec[ptr].SetMain(pBuffer, 64, 0, true);

	// 5. MBRF

	if (FindIndex(nMBRF) == dwSectionCount)
	{
		while (m_Sec[++ptr].GetName());
		CHECKPTR;
		m_Sec[ptr].SetName(nMBRF);
		m_Sec[ptr].SetMain(0,0,0,true);
	}

	// 6. SWNM

	while (m_Sec[++ptr].GetName());
	CHECKPTR;
	m_Sec[ptr].SetName(nSWNM);
	pBuffer = realloc(pBuffer,0x400);
	ZeroMemory(pBuffer, 0x400);
	m_Sec[ptr].SetMain(pBuffer, 0x400, 0, true);

	// 7. x시리즈

	if (m_chkVer == HYBRID)
	{
		int idxPUPx = FindIndex(nPUPx), idxPTEx = FindIndex(nPTEx),
			idxUNIx = FindIndex(nUNIx), idxUPGx = FindIndex(nUPGx),
			idxTECx = FindIndex(nTECx);
		void *pmain;

		// 1. UPGR
		while (m_Sec[++ptr].GetName());
		CHECKPTR;

		pBuffer = realloc(pBuffer, 1748);

		CopyMemory(pBuffer, pmain = (void *)m_Sec[idxPUPx].GetMain(), 12 * 45);
		CopyMemory((char *)pBuffer+12*45, (char *)pmain + 12 * 61, 12*45);
		CopyMemory((char *)pBuffer+24*45, (char *)pmain + 24 * 61, 45);
		CopyMemory((char *)pBuffer+25*45, (char *)pmain + 25 * 61, 45);
		CopyMemory((char *)pBuffer+26*45, (char *)pmain + 26 * 61, 45*12);
		m_Sec[ptr].SetMain(pBuffer, 1748, 0, true);
		m_Sec[ptr].SetName(nUPGR);

		// 2. PTEC
		while (m_Sec[++ptr].GetName());
		CHECKPTR;

		pBuffer = realloc(pBuffer, 0x390);

		CopyMemory(pBuffer, pmain = (void *)m_Sec[idxPTEx].GetMain(), 12 * 24);
		CopyMemory((char *)pBuffer+12*24, (char *)pmain + 12 * 44, 12*24);
		CopyMemory((char *)pBuffer+24*24, (char *)pmain + 24 * 44, 24);
		CopyMemory((char *)pBuffer+25*24, (char *)pmain + 25 * 44, 24);
		CopyMemory((char *)pBuffer+26*24, (char *)pmain + 26 * 44, 24*12);
		m_Sec[ptr].SetMain(pBuffer, 0x390, 0, true);
		m_Sec[ptr].SetName(nPTEC);

		// 3. UNIS (제일 길다 시바놈 근데 코드는 제일짧아)

		while (m_Sec[++ptr].GetName());
		CHECKPTR;

		pBuffer = realloc(pBuffer, 0xFD0);

		CopyMemory(pBuffer, pmain = (void *)m_Sec[idxUNIx].GetMain(), 3848);
		CopyMemory((char *)pBuffer+3848, (char *)pmain + 3908, 200);
		m_Sec[ptr].SetMain(pBuffer, 0xFD0, 0, true);
		m_Sec[ptr].SetName(nUNIS);

		// 4. UPGS (아까 어떤 허접새끼가 UPGx라고 써놨지?)

		while (m_Sec[++ptr].GetName());
		CHECKPTR;

		pBuffer = realloc(pBuffer, 0x256);

		CopyMemory(pBuffer, pmain = (void *)m_Sec[idxUPGx].GetMain(), 46);
		CopyMemory((char *)pBuffer+46, (char *)pmain + 61, 46*2);
		CopyMemory((char *)pBuffer+46*3, (char *)pmain + 61*3, 46*2);
		CopyMemory((char *)pBuffer+46*5, (char *)pmain + 61*5, 46*2);
		CopyMemory((char *)pBuffer+46*7, (char *)pmain + 61*7, 46*2);
		CopyMemory((char *)pBuffer+46*9, (char *)pmain + 61*9, 46*2);
		CopyMemory((char *)pBuffer+46*11, (char *)pmain + 61*11, 46*2);
		m_Sec[ptr].SetMain(pBuffer, 0x256, 0, true);
		m_Sec[ptr].SetName(nUPGS);

		// 5. TECS

		while (m_Sec[++ptr].GetName());
		CHECKPTR;

		pBuffer = realloc(pBuffer, 0xD8);

		CopyMemory(pBuffer, pmain = (void *)m_Sec[idxTECx].GetMain(), 24);
		CopyMemory((char *)pBuffer+24, (char *)pmain + 44, 2*24);
		CopyMemory((char *)pBuffer+3*24, (char *)pmain + 3 * 44, 2*24);
		CopyMemory((char *)pBuffer+5*24, (char *)pmain + 5 * 44, 2*24);
		CopyMemory((char *)pBuffer+7*24, (char *)pmain + 7 * 44, 2*24);
		m_Sec[ptr].SetMain(pBuffer, 0xD8, 0, true);
		m_Sec[ptr].SetName(nTECS);
	}

	// WAV는 지금 당장 작성하지는 않고 자리만 잡아놓는다.
	while (m_Sec[++ptr].GetName());
	CHECKPTR;

	pBuffer = realloc(pBuffer, 0x800);
	ZeroMemory(pBuffer, 0x800);
	m_Sec[ptr].SetMain(pBuffer, 0x800, 0, true);
	m_Sec[ptr].SetName(nWAV);
	free(pBuffer);
#undef CHECKPTR
}

void CUnprotectMapA::RepairVCOD()
{
	m_Sec[FindIndex(nVCOD)].SetMain(vcod, 1040, 0, true);
}

// RepairStrHelperFunc1은 적혀있는 문자열 번호가 올바른지 검사한다.
template <typename T>
static inline void RepairStrHelperFunc1(T &t, DWORD dwMaxString, BYTE *bStrUsed, WORD *str, DWORD size)
{
	if (t <= dwMaxString && bStrUsed[t] && str[t] < size && *((char *)str+str[t])) bStrUsed[t] = 2;
	else t = 0;
}

// RepairStrHelperFunc2는 문자열을 새 번호로 바꾸어 준다.
template <typename T>
static inline void RepairStrHelperFunc2(T &t, WORD *lpNewStrNum)
{
	if (t) t=lpNewStrNum[t];
}

void CUnprotectMapA::RepairStr()
{
	// idx변수들: 인덱스.
	int idxTRIG = FindIndex(nTRIG), idxSTR = FindIndex(nSTR), idxMBRF = FindIndex(nMBRF),
		idxFORC = FindIndex(nFORC), idxSPRP = FindIndex(nSPRP), idxUNI,
		idxMRGN = FindIndex(nMRGN);
	int i,j,k; // 자잘한 계산용변수
	char locationtemp[13]; // 로케이션이름을 저장하는 변수
	BYTE *bStrUsed = new BYTE[65536]; // 문자열이 사용된 여부를 저장하는 배열.
	TRIGGER *pTrig = (TRIGGER *)m_Sec[idxTRIG].GetMain(); // 트리거의 인덱스.
	BRIEFING *pBrif = 0; // 브리핑의 인덱스.
	FORCE *pForc = (FORCE *)m_Sec[idxFORC].GetMain(); // 포스의 인덱스.
	LOCATION *pLoc = (LOCATION *)m_Sec[idxMRGN].GetMain(); // 로케이션의 인덱스.
	BYTE *pUni; // UNI단락의 포인터.
	PWORD pNewStr; // 새 STR단락.
	DWORD dwMaxString = m_Sec[idxSTR].GetSize() >> 1; // 문자열의 가능한 최대 개수.
	WORD wStringNum=0; // 새 문자열의 개수.
	WORD *wNewStrNum = new WORD[65536]; // 새 문자열을 정하는 배열.
	WORD wMaxLocation=(m_chkVer==ORIGINAL)?64:255; // 로케이션 개수.
	unsigned char bLocationUsed[255]={0}; // 로케이션이 쓰였는지 여부.
	RECT rtNull = {0}; // 빈 렉트.
	std::string *str = new std::string[65536]; // 새 문자열 리스트.
	WORD *wStrOffset = new WORD[65536]; // 문자열 오프셋.
	WORD *pStr = (WORD *)m_Sec[idxSTR].GetMain();
	DWORD size=dwMaxString << 1;

	// 문자열의 한계치는 65535 혹은 STR단락의 크기에 2를 나눈것 혹은 STR단락의 첫 번째 WORD이다.
	dwMaxString = min(dwMaxString, 65535);
	dwMaxString = min(dwMaxString, *(WORD *)m_Sec[idxSTR].GetMain());

	if (idxMBRF < dwSectionCount) pBrif = (BRIEFING *)m_Sec[idxMBRF].GetMain();
	FillMemory(bStrUsed,65536,1);
	FillMemory(wNewStrNum,65536*sizeof(WORD),0);

	j = m_Sec[idxSTR].GetSize();

	for (i = 1; i <= (int)dwMaxString; i++)
	{
		if (pStr[i] > (unsigned)j) bStrUsed[i] = 0;
	}

	for (i = 0; i < (int)(m_Sec[idxTRIG].GetSize() / sizeof(TRIGGER)); i++)
	{
		for (j = 0; j < 64; j++)
		{
			RepairStrHelperFunc1(pTrig[i].Action[j].dwString,dwMaxString, bStrUsed, pStr, size);
			RepairStrHelperFunc1(pTrig[i].Action[j].dwSoundString,dwMaxString, bStrUsed, pStr, size);
		}
	}

	for (i = 0; i < 4; i++)
	{
		RepairStrHelperFunc1(pForc->wForceString[i],dwMaxString,bStrUsed, pStr, size);
	}

	if (pBrif)
	{
		for (i = 0; i < (int)(m_Sec[idxMBRF].GetSize() / sizeof(TRIGGER)); i++)
		{
			for (j = 0; j < 64; j++)
			{
				RepairStrHelperFunc1(pBrif[i].Action[j].dwString,dwMaxString, bStrUsed, pStr, size);
				RepairStrHelperFunc1(pBrif[i].Action[j].dwSoundString,dwMaxString, bStrUsed, pStr, size);
			}
		}
	}

	j=(int)m_Sec[idxSPRP].GetMain();
	RepairStrHelperFunc1(*(WORD *)j,dwMaxString,bStrUsed, pStr, size);
	RepairStrHelperFunc1(*((WORD *)j+1),dwMaxString,bStrUsed, pStr, size);

	idxUNI = FindIndex(m_chkVer == ORIGINAL ? nUNIS : nUNIx);

#define UNIOFFSET 1*228+4*228+2*228+1*228+2*228+2*228+2*228
	pUni=(BYTE *)m_Sec[idxUNI].GetMain();
	for(j=0; j<228; j++)
	{
		RepairStrHelperFunc1(*(WORD *)(pUni+UNIOFFSET+2*j),dwMaxString,bStrUsed, pStr, size);
	}
#undef UNIOFFSET

	if (!m_bRewriteLocationName)
	{
		for (i=0;i<wMaxLocation;i++)
		{
			RepairStrHelperFunc1(pLoc[i].wString,dwMaxString,bStrUsed, pStr, size);
		}
	}

	j = 1;

	for (i=1; i <= (int)dwMaxString; i++)
	{
		wStringNum += (2 == bStrUsed[i]);
		if (2 == bStrUsed[i]) wNewStrNum[i] = j++;
	}

	for (i=0; i < wMaxLocation; i++)
	{
		if (pLoc[i].rt.left || pLoc[i].rt.right || pLoc[i].rt.bottom || pLoc[i].rt.top)
		{
			if (m_bRewriteLocationName || pLoc[i].wString == 0 || (pLoc[i].wString < dwMaxString && *((char *)pStr+pStr[pLoc[i].wString]) == 0))
			{
				bLocationUsed[i] = 1;
				wStringNum++;
			}
		}
	}

	for (i=1; i<=(int)dwMaxString; i++)
	{
		if (wNewStrNum[i])
		{
			str[j=wNewStrNum[i]] = (char *)pStr + pStr[i];
		}
	}

	for (i=0; i<wMaxLocation; i++)
	{
		if (bLocationUsed[i] == 1)
		{
			if (i == 63) str[++j]="Anywhere";
			else wsprintfA(locationtemp,"%03d",i),str[++j]=locationtemp;
			pLoc[i].wString=j;
		}
		else
		{
			RepairStrHelperFunc2(pLoc[i].wString,wNewStrNum);
		}
	}

	k=j;

	wStringNum = max(wStringNum, 1024);

	wStrOffset[1] = (((DWORD)wStringNum)<<1) + 3;

	for (i=2; i<=j; i++)
		wStrOffset[i] = wStrOffset[i-1] + str[i-1].length() + 1;
	for (; i<=wStringNum; i++) wStrOffset[i] = (((DWORD)wStringNum)<<1)+2;

	for (i = 0; i < (int)(m_Sec[idxTRIG].GetSize() / sizeof(TRIGGER)); i++)
	{
		for (j = 0; j < 64; j++)
		{
			RepairStrHelperFunc2(pTrig[i].Action[j].dwString,wNewStrNum);
			RepairStrHelperFunc2(pTrig[i].Action[j].dwSoundString,wNewStrNum);
		}
	}

	for (i = 0; i < 4; i++)
	{
		RepairStrHelperFunc2(pForc->wForceString[i],wNewStrNum);
	}

	if (pBrif)
	{
		for (i = 0; i < (int)(m_Sec[idxMBRF].GetSize() / sizeof(TRIGGER)); i++)
		{
			for (j = 0; j < 64; j++)
			{
				RepairStrHelperFunc2(pBrif[i].Action[j].dwString,wNewStrNum);
				RepairStrHelperFunc2(pBrif[i].Action[j].dwSoundString,wNewStrNum);
			}
		}
	}

	j=(int)m_Sec[idxSPRP].GetMain();
	RepairStrHelperFunc2(*(WORD *)j,wNewStrNum);
	RepairStrHelperFunc2(*((WORD *)j+1),wNewStrNum);

	idxUNI = FindIndex(m_chkVer == ORIGINAL ? nUNIS : nUNIx);

#define UNIOFFSET 1*228+4*228+2*228+1*228+2*228+2*228+2*228
	pUni=(BYTE *)m_Sec[idxUNI].GetMain();
	for(i=0; i<228; i++)
	{
		RepairStrHelperFunc2(*(WORD *)(pUni+UNIOFFSET+2*i),wNewStrNum);
	}
#undef UNIOFFSET

	pNewStr = (PWORD)malloc(m_Sec[idxSTR].GetSize()+65535);
	pNewStr[0] = wStringNum;
	for (i = 1; i <= k; i++)
	{
		pNewStr[i] = wStrOffset[i];
		lstrcpyA((char *)pNewStr + wStrOffset[i], str[i].c_str());
	}
	for (; i<=wStringNum; i++) pNewStr[i] = (((DWORD)wStringNum)<<1)+2;
	((char *)pNewStr)[(((DWORD)wStringNum)<<1)+2] = 0;
	m_Sec[idxSTR].SetMain(pNewStr,wStrOffset[k]+str[k].length()+1,0,true);
	free(pNewStr);
	delete[] str;
	delete[] wStrOffset;
	delete[] wNewStrNum;
	delete[] bStrUsed;
}

void CUnprotectMapA::DeleteOverlappedSection()
{
	DWORD dwSecName[] = {nVER, nDIM, nSTR, nSPRP};
	int i,j,k;

	for (i = 0; i < ARSIZE(dwSecName); i++)
	{
		j = FindIndex(dwSecName[i]);
		while ((k = FindIndex(dwSecName[i], j + 1)) < dwSectionCount)
		{
			m_Sec[j].Release();
			j = k;
		}
	}
}

bool CUnprotectMapA::Unprotect()
{
	int i;
	if (m_chkVer == 0) return false;

	try
	{
		DeleteOverlappedSection();
		RepairTerrain();
		RepairTrigger();
		RepairLocation();
		RepairUnusedSection();
		RepairStr();
		RepairVCOD();

		std::stable_sort(&m_Sec[0],&m_Sec[dwSectionCount]);

		i = FindIndex(nTECx);
		if (i == dwSectionCount) i = FindIndex(nTECS);

		for (i++; i < dwSectionCount; i++)
		{
			m_Sec[i].Release();
		}

		RepairWAV();
	}
	catch (...)
	{
		CloseMap();
		return false;
	}

	return true;
}

void CUnprotectMapA::RepairWAV()
{
	HANDLE hMPQ, hFile;
	int i,j,k,ptr = 0,ptr2 = 0;
	int idxWAV = FindIndex(nWAV), idxTRIG = FindIndex(nTRIG), idxMBRF = FindIndex(nMBRF);
	std::string strSound;
	std::string strAddedSound[512];

	PDWORD pWAV = (PDWORD)m_Sec[idxWAV].GetMain();
	TRIGGER *pTRIG = (TRIGGER *)m_Sec[idxTRIG].GetMain();
	BRIEFING *pBRIF = (BRIEFING *)m_Sec[idxMBRF].GetMain();
	PWORD pSTR = (PWORD)m_Sec[FindIndex(nSTR)].GetMain();

	if (m_bChk) return;

	SFileOpenArchive(m_szPath, 0, 0, &hMPQ);
	
	for (i = 0; i < m_Sec[idxTRIG].GetSize()/2400; i++)
	{
		for (j = 0; j < 64; j++)
		{
			if (pTRIG[i].Action[j].bAction == 0x07 || pTRIG[i].Action[j].bAction == 0x08)
			{
				if (pTRIG[i].Action[j].dwSoundString)
				{
					strSound = (char *)pSTR+pSTR[pTRIG[i].Action[j].dwSoundString];
					for (k = 0; k < ptr; k++)
					{
						if (!strAddedSound[k].compare(strSound)) goto already;
					}
					try
					{
						if (SFileOpenFileEx(hMPQ, strSound.c_str(), 0, &hFile))
						{
							SFileCloseFile(hFile);
							strAddedSound[ptr++] = strSound;
							pWAV[ptr2++] = pTRIG[i].Action[j].dwSoundString;
						}
					}
					catch(...)
					{
					}
				}
already:;
			}
		}
	}

	for (i = 0; i < m_Sec[idxMBRF].GetSize()/2400; i++)
	{
		for (j = 0; j < 64; j++)
		{
			if (pBRIF[i].Action[j].bAction == 2 || pBRIF[i].Action[j].bAction == 8)
			{
				if (pBRIF[i].Action[j].dwSoundString)
				{
					strSound = (char *)pSTR+pSTR[pBRIF[i].Action[j].dwSoundString];
					for (k = 0; k < ptr; k++)
					{
						if (!strAddedSound[k].compare(strSound)) goto already2;
					}
					try
					{
						if (SFileOpenFileEx(hMPQ, strSound.c_str(), 0, &hFile))
						{
							SFileCloseFile(hFile);
							strAddedSound[ptr++] = strSound;
							pWAV[ptr2++] = pBRIF[i].Action[j].dwSoundString;
						}
					}
					catch(...)
					{
					}
				}
already2:;
			}
		}
	}

	SFileCloseArchive(hMPQ);
}

bool CUnprotectMapA::SaveMap(LPCSTR lpFileName, bool bSaveAsChk, bool bAddListFile, const char * /*ProgName*/)
{
	char chktmp[TMP_MAX];
	char chktmp2[TMP_MAX];
	int i;
	HANDLE hFile, hMpq;
	HANDLE hWav, hMpqWav;
	DWORD dummy, dwTmp;
	std::string strSound;
	BOOL flag=0;

	PVOID pWavBuffer;
	DWORD dwWavSize;
	PDWORD pWAV = (PDWORD)m_Sec[FindIndex(nWAV)].GetMain();
	PWORD pSTR = (PWORD)m_Sec[FindIndex(nSTR)].GetMain();

	if (!m_chkVer) return false;
	if (!m_bAllowOverwrite && !lstrcmpA(lpFileName, m_szPath)) return false;
	tmpnam(chktmp);
	tmpnam(chktmp2);

	hFile = CreateFileA(chktmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	for (i = 0; i < dwSectionCount; i++)
	{
		if (m_Sec[i].GetName())
		{
			dwTmp = m_Sec[i].GetName();
			WriteFile(hFile, &dwTmp, 4, &dummy, 0);
			dwTmp = m_Sec[i].GetSize();
			WriteFile(hFile, &dwTmp, 4, &dummy, 0);
			WriteFile(hFile, m_Sec[i].GetMain(), dwTmp, &dummy, 0);
		}
	}/*
	if (ProgName)
	{
		char info[58]={0,0,0,0,50,0,0,0,'R','e','p','a','i','r','e','d',' ','b','y',' '};
		lstrcpyA(info+20,ProgName);
		WriteFile(hFile,info,58,&dummy,0);
	}
	*/
	CloseHandle(hFile);

	if (bSaveAsChk)
	{
		CopyFileA(chktmp,lpFileName, FALSE);
	}
	else
	{
		hMpq = MpqOpenArchiveForUpdate(chktmp2, MOAU_CREATE_ALWAYS, 1024);
		if (hMpq == INVALID_HANDLE_VALUE) {DeleteFileA(chktmp); return false;}
		MpqAddFileToArchive(hMpq, chktmp, "staredit\\scenario.chk",MAFA_COMPRESS | MAFA_ENCRYPT);

		// 리스트파일작성하기
		hFile = CreateFileA(chktmp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hFile, "staredit\\scenario.chk\r\n", sizeof("staredit\\scenario.chk\r\n")-1, &dummy, 0);

		// 웨이브 추가
		SFileOpenArchive(m_szPath, 0, 0, &hMpqWav);

		for (i = 0; i < 512 && pWAV[i]; i++)
		{
			strSound = (char *)pSTR + pSTR[pWAV[i]];
			SFileOpenFileEx(hMpqWav, strSound.c_str(), 0, &hWav);
			dwWavSize = SFileGetFileSize(hWav, 0);
			pWavBuffer = malloc(dwWavSize);
			SFileReadFile(hWav, pWavBuffer, dwWavSize, &dummy, 0);
			SFileCloseFile(hWav);

			if (m_nWavCompress == -1)
				MpqAddFileFromBuffer(hMpq, pWavBuffer, dwWavSize, strSound.c_str(), MAFA_ENCRYPT);
			else
				MpqAddWaveFromBuffer(hMpq, pWavBuffer, dwWavSize, strSound.c_str(), MAFA_COMPRESS | MAFA_ENCRYPT,
					m_nWavCompress);
			free(pWavBuffer);

			strSound += "\r\n";
			WriteFile(hFile, strSound.c_str(), strSound.length(), &dummy, NULL);
		}

		SFileCloseArchive(hMpqWav);

		CloseHandle(hFile);
		MpqAddFileToArchive(hMpq, chktmp, "(listfile)", MAFA_COMPRESS | MAFA_ENCRYPT);

		MpqCloseUpdatedArchive(hMpq, 0);

		if (lstrcmpA(lpFileName,m_szPath)==0)
		{
			CloseHandle(m_hFile);
			DeleteFileA(lpFileName);
			MoveFileA(chktmp2,lpFileName);
			m_hFile=CreateFileA(lpFileName,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
		}
		else
		{
			MoveFileA(chktmp2,lpFileName);
		}
	}

	DeleteFileA(chktmp);

	return true;
}
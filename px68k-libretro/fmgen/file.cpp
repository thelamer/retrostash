//	$fmgen-Id: file.cpp,v 1.6 1999/12/28 11:14:05 cisc Exp $

#include "common.h"

#include "headers.h"
#include "file.h"

// ---------------------------------------------------------------------------
//	構築/消滅
// ---------------------------------------------------------------------------

FileIO::FileIO()
{
	flags = 0;
}

FileIO::FileIO(const char* filename, uint32_t flg)
{
	flags = 0;
	Open(filename, flg);
}

FileIO::~FileIO()
{
	Close();
}

// ---------------------------------------------------------------------------
//	ファイルを開く
// ---------------------------------------------------------------------------

bool FileIO::Open(const char* filename, uint32_t flg)
{
	Close();

	DWORD access = (flg & readonly ? 0 : GENERIC_WRITE) | GENERIC_READ;
	DWORD share = (flg & readonly) ? FILE_SHARE_READ : 0;
	DWORD creation = flg & create ? CREATE_ALWAYS : OPEN_EXISTING;

	hfile = CreateFile(filename, access, share, 0, creation, 0, 0);
	
	flags = (flg & readonly) | (hfile == INVALID_HANDLE_VALUE ? 0 : open);
	return !!(flags & open);
}

// ---------------------------------------------------------------------------
//	ファイルを閉じる
// ---------------------------------------------------------------------------

void FileIO::Close()
{
	uint32_t flags = GetFlags();
	if (flags & open)
	{
		FAKE_CloseHandle(hfile);
		flags = 0;
	}
}

// ---------------------------------------------------------------------------
//	ファイル殻の読み出し
// ---------------------------------------------------------------------------

int32_t FileIO::Read(void* dest, int32_t size)
{
	uint32_t flags = GetFlags();
	if (!(flags & open))
		return -1;
	
	DWORD readsize;
	if (!ReadFile(hfile, dest, size, &readsize, 0))
		return -1;
	return readsize;
}

// ---------------------------------------------------------------------------
//	ファイルへの書き出し
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//	ファイルをシーク
// ---------------------------------------------------------------------------

bool FileIO::Seek(int32_t pos, SeekMethod method)
{
        uint32_t flags = GetFlags();
	if (!(flags & open))
		return false;
	
	DWORD wmethod;
	switch (method)
	{
	case begin:	
		wmethod = FILE_BEGIN; 
		break;
	case current:	
		wmethod = FILE_CURRENT; 
		break;
	case end:		
		wmethod = FILE_END; 
		break;
	default:
		return false;
	}

	return 0xffffffff != SetFilePointer(hfile, pos, 0, wmethod);
}

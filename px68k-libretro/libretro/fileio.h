#ifndef winx68k_fileio_h
#define winx68k_fileio_h

#include <stdint.h>
#include "common.h"
#include "dosio.h"

#define	FILEH		HANDLE

#define	FSEEK_SET	0
#define	FSEEK_CUR	1
#define	FSEEK_END	2

LPSTR getFileName(LPSTR filename);

FILEH	File_Open(uint8_t *filename);
FILEH	File_Create(uint8_t *filename);
DWORD	File_Seek(FILEH handle, long pointer, int16_t mode);
DWORD	File_Read(FILEH handle, void *data, DWORD length);
DWORD	File_Write(FILEH handle, void *data, DWORD length);
int16_t	File_Close(FILEH handle);
#define	File_Open	file_open
#define	File_Create	file_create
#define	File_Seek	file_seek
#define	File_Read	file_lread
#define	File_Write	file_lwrite
#define	File_Close	file_close

FILEH	File_OpenCurDir(uint8_t *filename);
FILEH	File_CreateCurDir(uint8_t *filename);
#define	File_OpenCurDir		file_open_c
#define	File_CreateCurDir	file_create_c

#endif

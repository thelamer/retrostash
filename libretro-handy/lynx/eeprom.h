//////////////////////////////////////////////////////////////////////////////
// Lynx 3wire EEPROM class header file                                      //
//////////////////////////////////////////////////////////////////////////////


#ifndef EEPROM_H
#define EEPROM_H

#include <string.h>
#include <string/stdstring.h>

#ifndef PATH_MAX_LENGTH
#if defined(_XBOX1) || defined(_3DS) || defined(PSP) || defined(PS2) || defined(GEKKO)|| defined(WIIU) || defined(ORBIS) || defined(__PSL1GHT__) || defined(__PS3__)
#define PATH_MAX_LENGTH 512
#else
#define PATH_MAX_LENGTH 4096
#endif
#endif

#ifndef __min
#define __min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })
#endif

enum {EE_NONE=0, EE_START, EE_DATA, EE_BUSY, EE_WAIT};

class CEEPROM : public CLynxBase
{
   // Function members

public:
   CEEPROM();
   ~CEEPROM();

   bool ContextSave(LSS_FILE *fp);
   bool ContextLoad(LSS_FILE *fp);
   void Reset(void);

   bool Available(void){ return ((type != 0) && !string_is_empty(filename)); };
   void ProcessEepromIO(UBYTE iodir,UBYTE iodat);
   void ProcessEepromCounter(UWORD cnt);
   void ProcessEepromBusy(void);
   bool OutputBit(void)
   {
      return mAUDIN_ext;
   };
   void SetEEPROMType(UBYTE b);
   int Size(void);
   void InitFrom(char *data,int count){ memcpy(romdata,data,__min(count,Size()));};

   void Poke(ULONG addr,UBYTE data) { };
   UBYTE Peek(ULONG addr)
   {
      return(0);
   };

   void SetFilename(const char *f)
   {
      if (!string_is_empty(f))
         strlcpy(filename, f, PATH_MAX_LENGTH);
      else
         *filename='\0';
   };
   char* GetFilename(void){ return filename;};
   
   void Load(void);
   void Save(void);

private:
   char filename[PATH_MAX_LENGTH];
    
   void UpdateEeprom(UWORD cnt);
   UBYTE type; // 0 ... no eeprom

   UWORD ADDR_MASK;
   UBYTE CMD_BITS;
   UBYTE ADDR_BITS;
   ULONG DONE_MASK;

   UBYTE iodir, iodat;
   UWORD counter;
   int busy_count;
   int state;
   UWORD readdata;

   ULONG data;
   UWORD romdata[1024];// 128, 256, 512, 1024 WORDS bzw 128 bytes fuer byte zugriff
   UWORD addr;
   int sendbits;
   bool readonly;

   bool mAUDIN_ext;// OUTPUT

public:
};

#endif

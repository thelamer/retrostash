#ifndef _GIT_H
#define _GIT_H

#include <string.h>
#include <boolean.h>

#include "state.h"
#include "video.h"

typedef struct
{
	const char *SettingName;	// No spaces, shouldbe all a-z0-9 and _. Definitely no ~!
	const char *Name;
        const int ConfigOrder;          // Configuration order during in-game config process, -1 for no config.
	const char *ExcludeName;	// SettingName of a button that can't be pressed at the same time as this button
					// due to physical limitations.
} InputDeviceInputInfoStruct;

typedef struct
{
 const char *ShortName;
 const char *FullName;

 int NumInputs; // Usually just the number of buttons....OR if PortExpanderDeviceInfo is non-NULL, it's the number of input
		// ports this port expander device provides.
 const InputDeviceInputInfoStruct *IDII;
} InputDeviceInfoStruct;

typedef struct
{
 const char *ShortName;
 const char *FullName;
 int NumTypes; // Number of unique input devices available for this input port
 InputDeviceInfoStruct *DeviceInfo;
 const char *DefaultDevice;	// Default device for this port.
} InputPortInfoStruct;

typedef struct
{
 int InputPorts;
 const InputPortInfoStruct *Types;
} InputInfoStruct;

struct MemoryPatch;

struct CheatFormatStruct
{
 const char *FullName;		//"Game Genie", "GameShark", "Pro Action Catplay", etc.
 const char *Description;	// Whatever?
};

struct CheatFormatInfoStruct
{
 unsigned NumFormats;

 struct CheatFormatStruct *Formats;
};

// Miscellaneous system/simple commands(power, reset, dip switch toggles, coin insert, etc.)
// (for DoSimpleCommand() )
enum
{
 MDFN_MSC_RESET = 0x01,
 MDFN_MSC_POWER = 0x02,
 MDFN_MSC__LAST = 0x3F	// WARNING: Increasing(or having the enum'd value of a command greater than this :b) this will necessitate a change to the netplay protocol.
};

typedef struct
{
	// Pitch(32-bit) must be equal to width and >= the "fb_width" specified in the MDFNGI struct for the emulated system.
	// Height must be >= to the "fb_height" specified in the MDFNGI struct for the emulated system.
	// The framebuffer pointed to by surface->pixels is written to by the system emulation code.
	MDFN_Surface *surface;

	// Will be set to TRUE if the video pixel format has changed since the last call to Emulate(), FALSE otherwise.
	// Will be set to TRUE on the first call to the Emulate() function/method
	bool VideoFormatChanged;

	// Set by the system emulation code every frame, to denote the horizontal and vertical offsets of the image, and the size
	// of the image.  If the emulated system sets the elements of LineWidths, then the horizontal offset(x) and width(w) of this structure
	// are ignored while drawing the image.
	MDFN_Rect DisplayRect;

	// Pointer to an array of MDFN_Rect, number of elements = fb_height, set by the driver code.  Individual MDFN_Rect structs written
	// to by system emulation code.  If the emulated system doesn't support multiple screen widths per frame, or if you handle
	// such a situation by outputting at a constant width-per-frame that is the least-common-multiple of the screen widths, then
	// you can ignore this.  If you do wish to use this, you must set all elements every frame.
	MDFN_Rect *LineWidths;

	// Skip rendering this frame if true.  Set by the driver code.
	int skip;

	//
	// If sound is disabled, the driver code must set SoundRate to false, SoundBuf to NULL, SoundBufMaxSize to 0.

        // Will be set to TRUE if the sound format(only rate for now, at least) has changed since the last call to Emulate(), FALSE otherwise.
        // Will be set to TRUE on the first call to the Emulate() function/method
	bool SoundFormatChanged;

	// Sound rate.  Set by driver side.
	double SoundRate;

	// Maximum size of the sound buffer, in frames.  Set by the driver code.
	int32 SoundBufMaxSize;

	// Number of frames currently in internal sound buffer.  Set by the system emulation code, to be read by the driver code.
	int32 SoundBufSize;
	int32 SoundBufSizeALMS;	// SoundBufSize value at last MidSync(), 0
				// if mid sync isn't implemented for the emulation module in use.

	// Number of cycles that this frame consumed, using MDFNGI::MasterClock as a time base.
	// Set by emulation code.
	int64 MasterCycles;

} EmulateSpecStruct;

#define MDFN_MASTERCLOCK_FIXED(n)	((int64)((double)(n) * (1LL << 32)))

typedef struct
{
 // Time base for EmulateSpecStruct::MasterCycles
 int64 MasterClock;

 uint32 fps; // frames per second * 65536 * 256, truncated

 int lcm_width;
 int lcm_height;

 void *dummy_separator;	//

 int nominal_width;
 int nominal_height;

 int fb_width;		// Width of the framebuffer(not necessarily width of the image).  MDFN_Surface width should be >= this.
 int fb_height;		// Height of the framebuffer passed to the Emulate() function(not necessarily height of the image)

 int soundchan; 	// Number of output sound channels.
} MDFNGI;

#ifdef __cplusplus
extern "C" {
#endif
int StateAction(StateMem *sm, int load, int data_only);
#ifdef __cplusplus
}
#endif

#endif

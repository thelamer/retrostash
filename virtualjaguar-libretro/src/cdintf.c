//
// OS agnostic CDROM interface functions
//
// by James Hammons
// (C) 2010 Underground Software
//
// JLH = James Hammons <jlhamm@acm.org>
//
// Who  When        What
// ---  ----------  -------------------------------------------------------------
// JLH  01/16/2010  Created this log ;-)
//

//
// This now uses the supposedly cross-platform libcdio to do the necessary
// low-level CD twiddling we need that libsdl can't do currently. Jury is
// still out on whether or not to make this a conditional compilation or not.
//

// Comment this out if you don't have libcdio installed
// (Actually, this is defined in the Makefile to prevent having to edit
//  things too damn much. Jury is still out whether or not to make this
//  change permanent.)

#include <boolean.h>
#include "cdintf.h"								// Every OS has to implement these

// *** OK, here's where we're going to attempt to put the platform agnostic CD interface ***

bool CDIntfInit(void)
{
   /* No suitable CDROM driver found */
   return false;
}

void CDIntfDone(void)
{
   /* Shutting down CDROM subsystem */
}

bool CDIntfReadBlock(uint32_t sector, uint8_t * buffer)
{
//#warning "!!! FIX !!! CDIntfReadBlock not implemented!"
   // !!! FIX !!!
   return false;
}

uint32_t CDIntfGetNumSessions(void)
{
//#warning "!!! FIX !!! CDIntfGetNumSessions not implemented!"
	// Still need relevant code here... !!! FIX !!!
	return 2;
}

void CDIntfSelectDrive(uint32_t driveNum)
{
//#warning "!!! FIX !!! CDIntfSelectDrive not implemented!"
	// !!! FIX !!!
}

uint32_t CDIntfGetCurrentDrive(void)
{
//#warning "!!! FIX !!! CDIntfGetCurrentDrive not implemented!"
	return 0;
}

const uint8_t * CDIntfGetDriveName(uint32_t driveNum)
{
//#warning "!!! FIX !!! CDIntfGetDriveName driveNum is currently ignored!"
	// driveNum is currently ignored... !!! FIX !!!

	return (uint8_t *)"NONE";
}

uint8_t CDIntfGetSessionInfo(uint32_t session, uint32_t offset)
{
//#warning "!!! FIX !!! CDIntfGetSessionInfo not implemented!"
	return 0xFF;
}

uint8_t CDIntfGetTrackInfo(uint32_t track, uint32_t offset)
{
//#warning "!!! FIX !!! CDIntfTrackInfo not implemented!"
	return 0xFF;
}

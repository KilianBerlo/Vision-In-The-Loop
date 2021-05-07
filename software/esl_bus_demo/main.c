//-----------------------------------------------------------------------------
// 
// ESL demo
// Version: 1.0
// Creator: Rene Moll
// Date: 10th April 2012
//
//-----------------------------------------------------------------------------
//
// Demo application which sets and reads form a register in the demo IP.
// 
// IOWR/IORD are part of the HAL, see the Nios 2 Software developer’s handbook
// for more information.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <io.h>
#include "system.h"

int main()
{
	// Say hello through the debug interface
	printf("Hello from Nios II!\n");

	// Put 0x08 in the memory of the IP and enable the count down
	IOWR(ESL_BUS_DEMO_0_BASE, 0x00, 1 << 31 | 0x08);

	// Verify that it is there
	int nReadOut = IORD(ESL_BUS_DEMO_0_BASE, 0x00);
	printf("From the IP: %u \n\r", nReadOut);

	// Now loop forever ...
	while(1){}

	return 0;
}

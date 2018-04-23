// Defines the C-code kernel entry point, calls initialisation routines

#include "src/monitor.h"

int helloWorld ()
{
	return 0xDEADBEEF;
}

int testScreen ()
{
	monitor_clear();
	monitor_write( "Bonjour!" );

	return 0;
}

int main ( struct multiboot *mboot_ptr )
{

	// return helloWorld();
	return testScreen();
}

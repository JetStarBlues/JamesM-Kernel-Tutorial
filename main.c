// Defines the C-code kernel entry point, calls initialisation routines

// #include "src/monitor.h"

int main ( struct multiboot *mboot_ptr )
{

	/*monitor_clear();
	monitor_write( "Bonjour!" );

	return 0;*/

	int x;

	x = 9000;  // 0x2328

	return 0xDEADBEEF;
}

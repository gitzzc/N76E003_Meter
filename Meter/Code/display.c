#include "bike.h"

#if ( defined LCD9040 ) || ( defined LCD5535 )
	#include "display_9040.c"
#elif ( defined LCD8794GCT )
	#include "display_8794GCT.c"
#elif ( defined LCD6040 )
	#include "display_6040.c"
#elif ( defined TM1640 )
	#include "display_TM1640.c"
#elif ( defined TM1624 )
	#include "display_TM1624.c"
#else
	#error "Please select a display type!!!"
#endif





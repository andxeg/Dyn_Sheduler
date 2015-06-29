#include <stdio.h>

#include "algorithm.h"
#include "taskfactory.h"
#include "snapshot.h"
#include "defs.h"


int main ( int argc, char ** argv )
{	
	if ( argc != 3 )
	{
		printf("Usage: %s <input file> <output file>\n", *argv);
		return 1;
	}
	
	Snapshot snapshot;
	
	TaskFactory * inputTasks = snapshot.read( argv[1] );
	
	if ( inputTasks == 0 ) return 1;
	Algorithm algorithm ( inputTasks );
	Shedule result = algorithm.shedule();
	
	if ( !snapshot.write( argv[2], result, inputTasks ) ) return 1;
	
	printf("Success\n");
	return 0;
}
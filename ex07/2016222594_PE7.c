
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define oops(m,x) {perror(m), exit(x);}

int main(int argc, char* argv[])
{
	int apipe[2], bpipe[2], pid1, pid2;
	if( argc != 4 )
		oops("not match argc\n",1);
	if( pipe(apipe) == -1 )
		oops("cannot get a pipe",1);
	if( (pid1=fork()) == -1 )
		oops("fail to fork",1);

	if( pid1 > 0)
	{
		close( apipe[1] );
		dup2( apipe[0], 0 );
		close( apipe[0] );
		execlp( argv[3], argv[3], NULL );
	}
	else
	{
		if( pipe(bpipe) == -1)
			oops("cannot get a pipe2", 1);
		if( (pid2=fork()) == -1 )
			oops("fail to fork2", 1)
		if( pid2 > 0 )
		{
			close( apipe[0] );
			dup2( apipe[1], 1 );
			close( apipe[1] );

			close( bpipe[1] );
			dup2( bpipe[0], 0 );
			close( bpipe[0] );
			execlp( argv[2], argv[2], NULL );
		}
		else
		{
		 	close( bpipe[0] );
			dup2( bpipe[1], 1 );
			close( bpipe[1] );
			execlp( argv[1], argv[1], NULL );
		}
	}
	return 0;
}


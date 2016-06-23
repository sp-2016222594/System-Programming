#include <stdio.h>
#include <signal.h>

int main()
{
	void f(int);
	int i;
	signal(SIGINT, f);
	for(i=0; i<5; ++i)
	{
		printf("hello\n");
		sleep(1);
	}
}

void f(int signum)
{
	printf("Interrupted! OK to quit (y/n)?");
	while(1)
	{
		switch( getchar() )
		{
			case 'y':
				exit(1);
				return;
			case 'n':
				return;
		}
	}
}

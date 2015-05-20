#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <csignal>

using namespace std;
int c,quit,z=0;

void handler(int signum)
{
	if(signum==SIGQUIT)
	{
		cout << "\\";
		quit++;
	}
	else if(signum==SIGINT)
	{
		cout << "c";
		c++;
	}
	else if(signum==SIGTSTP)
	{
		cout << "s";
		z++;
		raise(SIGSTOP);

	}
}	

int main()
{
	struct sigaction sa;
	sa.sa_handler = handler;

	while(1)
	{
		cout << "x" <<flush;
		sleep(1);
		if(sigaction(SIGQUIT,&sa,NULL)==-1)
			perror("sigaction");
		else if(sigaction(SIGINT,&sa,NULL)==-1)
			perror("sigaction");
		else if (sigaction(SIGTSTP,&sa,NULL)==-1)
			perror("sigaction");
		if(z==3)
			break;
	}
	cout << endl;
	cout << "The number of times ctrl-C: " << c << endl;
	cout << "The number of times ctrl-\\: " << quit << endl;
	cout << "The number of times ctrl-Z: " << z << endl;

	return 0;
}

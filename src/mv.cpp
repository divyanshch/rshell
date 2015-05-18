#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv)
{

	struct stat buffer;
	if(argc < 3)
	{
		if(argc < 2)
		{
			cerr << "mv: missing file operand\nTry mv--help for"<< " information" << endl;
		}
		else
		{
			cerr << "mv: missing destination folder after '" << argv[1]	<< "'\nTry mv--help for mor information" << endl;
		}
		exit(1);
	}

	string param1 = argv[1];
	if(stat(param1.c_str(), &buffer) == -1)
	{
		cerr << "File doesn not exist\n";
		exit(1);
	}

	string param2 = argv[2];
	if(stat(param2.c_str(), &buffer) == 0)
	{
		if(S_ISDIR(buffer.st_mode))
		{
			param2.append("/");
			param2.append(param1);
			if(link(param1.c_str(), param2.c_str()) != -1)
			{
				if(unlink(param1.c_str()) == -1)
				{
					perror("unlink");
					exit(1);
				}
				return 0;
			}
			else
			{
				perror("link");
				exit(1);
			}
		}

		else 
		{
			cerr << "Directory doesn't exist\n";
			exit(1);
		}
	}

	if(link(param1.c_str(), param2.c_str()) != -1)
	{
		if(unlink(param1.c_str()) == -1)
		{
			perror("unlink");
			exit(1);
		}
		return 0;
	}
	else
	{
		perror("link");
		exit(1);
	}


	return 0;
}


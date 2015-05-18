#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/stat.h>

using namespace std;
bool recursive;


void d_check(const char *folder,string value)
{
	struct stat buff;

	if(lstat((folder),&buff)==-1)
	{
		perror("lstat");
		exit(1);
	}
	
	if(S_ISDIR(buff.st_mode)&& recursive)
	{
		DIR *dirp;
		if(NULL==(dirp=opendir(folder)))
		{
			perror("opendir");
			exit(1);
		}
		struct dirent *filespecs;
		errno =0;
		while(NULL!=(filespecs=readdir(dirp)))
		{
			string word = filespecs->d_name;
			string word1 = folder;
			if(word!="." && word !="..")
			{
				word = word1 + "/" +word;
				d_check(word.c_str(),value);
			} 
		}
		
		if(errno!=0)
		{
			perror("readdir");
			exit(1);
		}


		if(-1==closedir(dirp))
		{
			perror("close");
			exit(1);
		}

		if(-1==rmdir(folder))
		{
			perror("rmdir");
			printf("%s", folder);
			exit(1);
		}
		return;
	}
	else if (S_ISDIR(buff.st_mode) && !recursive)
	{
			cerr << "rm: cannot remove '" << value << "': Is a directory" << endl;
			exit(1);
		
	}
	else 
	{
		if(-1== unlink((folder)))
		{
			perror("unlink");
			exit(1);
		}
		return;

	}

}

int main(int argc, char **argv)
{
	recursive = false;
	vector <string> value;
	string word;

	if (argc==1)
	{
		cerr<<"No arguments" << endl;
		exit(1);
	}

	for (int i=1;i<argc;i++)
	{
		word = argv[i];
		if(word=="-r")
			recursive = true;
		else
			value.push_back(word);
	}

	char folder[200];

	for (unsigned int i=0; i<value.size();i++)
	{
		word = "./" + value[i] ;
		strcpy (folder,word.c_str());
		d_check(folder,value[i]);
		if(i>=(value.size()-1))
			break;

	}

}

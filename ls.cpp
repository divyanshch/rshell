#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

using namespace std;

bool no_case_sensitivity(const string &one, const string &two)
{
	for(string::const_iterator lit = one.begin(), rit = two.begin(); lit!=one.end() && rit != two.end(); ++lit,++rit)
	{
		if(tolower(*lit) <tolower(*rit))
			return true;
		else if (tolower(*lit)>tolower(*rit))
			return false;
	}
	if(one.size() < two.size())
		return true;
	return false;
}


char blue[] = {"\033[1;34m"};
char white[] = {"\033[0;00m"};
char green[] = {"\033[1;32m"};

void lflag(char *folder)
{
	vector <string> temp;
	DIR *dirp;
	if(NULL==(dirp=opendir(folder)))
	{
		perror("opendir");
		exit(1);
	}
	struct dirent *filespecs;
	errno =0;
	while(NULL!=(filespecs = readdir(dirp)))
	{
		if((filespecs->d_name)[0] !='.')
		temp.push_back(filespecs->d_name);
	}
	if (errno!=0)
	{
		perror("readdir");
		exit(1);
	}
	if (-1==closedir(dirp))
	{
		perror("closedir");
		exit(1);
	}
	sort (temp.begin(),temp.end(),no_case_sensitivity);

	int total=0;
	for(int i=0;i<temp.size();i++)
	{
		struct stat fakebuff;
		if(lstat((temp[i]).c_str(), &fakebuff) == -1)
		{
			perror("lstat");
			exit(1);
		}
		total += fakebuff.st_size;
	}
	cout << "total " << total << endl;
	for (int i=0; i<temp.size();i++)
	{
		struct stat buff;
		if(lstat((temp[i]).c_str(), &buff) == -1)
		{
			perror("lstat");
			exit(1);
		}

		if(S_ISREG(buff.st_mode))
			cout << "-";
		else if(S_ISDIR(buff.st_mode))
			cout << "d";
		else if(S_ISCHR(buff.st_mode))
			cout << "c";
		else if(S_ISBLK(buff.st_mode))
			cout << "b";
		else if(S_ISFIFO(buff.st_mode))
			cout << "p";
		else if(S_ISLNK(buff.st_mode))
			cout << "l";
		else if(S_ISSOCK(buff.st_mode))
			cout << "s";
		else 
			cout << "?";
				


		if(S_IRUSR & buff.st_mode)
			cout << "r";
		else
			cout << "-";
		if(S_IWUSR & buff.st_mode)
			cout << "w";
		else 
			cout << "-";
		if(S_IXUSR & buff.st_mode)
			cout << "x";
		else
			cout << "-";
		if(S_IRGRP & buff.st_mode)
			cout << "r";
		else
			cout << "-";
		if(S_IWGRP & buff.st_mode)
			cout << "w";
		else 
			cout << "-";
		if(S_IXGRP & buff.st_mode)
			cout << "x";
		else
			cout << "-";
		if(S_IROTH & buff.st_mode)
			cout << "r";
		else
			cout << "-";
		if(S_IWOTH & buff.st_mode)
			cout << "w";
		else 
			cout << "-";
		if(S_IXOTH & buff.st_mode)
			cout << "x";
		else
			cout << "-";

		string time = ctime(&buff.st_mtime);
		time = time.substr(3,time.size()-12)+ " ";
		struct passwd *user = getpwuid(buff.st_uid);
		if(user==NULL)
		{
			perror("getpwuid");
			exit(1);
		}
		struct group *grp = getgrgid(buff.st_gid);
		if (grp==NULL)
		{
			perror("getgrgid");
			exit(1);
		}

		cout << " ";
		cout <<  buff.st_nlink << " ";
		cout <<  user->pw_name << " ";
		cout <<  grp->gr_name  << " ";
		cout << setw(7) << right<< buff.st_size << " ";
		cout << setw(13) << time << temp[i]; 
		cout << endl;

	}
}

void aflag(char *folder,bool flag)
{
	vector <string> temp;
	DIR *dirp;
	if(NULL==(dirp=opendir(folder)))
	{
		perror("opendir");
		exit(1);
	}
	struct dirent *filespecs;
	errno =0;
	while(NULL!=(filespecs = readdir(dirp)))
	{
		if ((!flag) &&((filespecs->d_name)[0]!= '.'))
		{
			temp.push_back(filespecs->d_name);
		}
		else if (flag)
			temp.push_back(filespecs->d_name);
	}
	if (errno!=0)
	{
		perror("readdir");
		exit(1);
	}
	if (-1==closedir(dirp))
	{
		perror("closedir");
		exit(1);
	}
	sort (temp.begin(),temp.end(),no_case_sensitivity);

	for (int i=0; i<temp.size();i++)
	{
		cout << temp[i] << "  ";
	}
	cout << endl;
}


void rflag()
{
}



int main(int argc, char *argv[])
{
bool flag_a, flag_l, flag_R = false;
int i =1;
while (argv[i]!=NULL) //sets which flags to mark
{
	string input = argv[i];
	if (input == "-a")
	{
		flag_a=true;
	}
	else if (input == "-l")
	{
		flag_l = true;
	}
	else if (input == "-R")
	{
		flag_R = true;
	}
	//else 
	//{
	//	cout << "Invalid flag! -a -l -R only" << endl;
	//	exit(1);
	//}
	i++;
}
char *folder = new char('.');
if (!(flag_a||flag_l||flag_R))
{
	aflag(folder,flag_a);
}
else if (flag_a && flag_l && flag_R)
{
}
else if(flag_a && flag_l)
{
}
else if(flag_a && flag_R)
{
}
else if(flag_l && flag_R)
{
}
else if(flag_a)
{
	aflag(folder,flag_a);
}
else if(flag_l)
{
	lflag(folder);
}
else if(flag_R)
{
}

delete folder;

return 0;
}

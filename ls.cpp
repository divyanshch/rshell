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
#include <sys/ioctl.h>
using namespace std;

bool flag_a, flag_l, flag_R = false;
char blue[] = {"\033[1;34m"};
char white[] = {"\033[100m"};
char green[] = {"\033[1;32m"};
char RESET[] = {"\033[0m"};
void rflag(char *folder);
void printer(vector<string> temp,char *folder)
{
	int check=0;
	int max_size;
	for(int i=0;i<temp.size();i++)
	{
		if(max_size<temp[i].size())
			max_size = temp[i].size();
	}
//	cout << folder << "works" << endl;
	for (int i=0;i<temp.size();i++)
	{
//		string fake = temp[i];
//		fake = "./src/"+fake;
//		temp[i] = fake;
		struct stat buff;
		if(lstat((folder+(temp[i])).c_str(),&buff) ==-1)
		{
			perror("lstat");
			exit(1);
		}
		if(S_ISDIR(buff.st_mode))
		{
			if (temp[i][0]=='.')
				cout << white << blue << setw(max_size) << left << temp[i] <<RESET << " ";
			else
				cout << blue << setw(max_size) << left << temp[i] << RESET << " ";
		}
		else if(buff.st_mode & S_IXUSR)
			cout << green << setw(max_size) << left << temp[i] << RESET << " ";
			
		else 
			cout << setw(max_size) << left << temp[i]<< " ";
			
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		
		
		
	
	}

		cout << endl;
}

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




void lflag(char *folder,bool flag)
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

	int total=0;
	
	//cout << folder << "works" << endl;
	for(int i=0;i<temp.size();i++)
	{
		struct stat fakebuff;
		if(lstat(((folder+temp[i])).c_str(), &fakebuff) == -1)
		{
			perror("lstat");
			exit(1);
		}
		total += fakebuff.st_blocks;
	}
	cout << "total " << total/2 << endl;
	for (int i=0; i<temp.size();i++)
	{
		struct stat buff;
		if(lstat(((folder+temp[i])).c_str(), &buff) == -1)
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
		cout << setw(13) << time; 
	
		if(S_ISDIR(buff.st_mode))
		{
			if (temp[i][0]=='.')
				cout << white << blue << right << temp[i] << RESET;
			else
				cout << blue << right << temp[i] << RESET ;
		}
		else if(buff.st_mode & S_IXUSR)
			cout << green << right<< temp[i] << RESET ;
			
		else 
			cout << right << temp[i];

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
		if ((!flag) &&((filespecs->d_name)[0]!='.')&&((filespecs->d_name)[1]!='.'))
		{
			temp.push_back(filespecs->d_name);
		}
		else if (flag&&(filespecs->d_name)[1]!='.')
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
//	for (int i=0; i<temp.size();i++)
//	{
//		cout << temp[i] << "  ";
//	}
//	cout << endl;
//	printer(temp,folder);
	for(int i=0;i<temp.size();i++)
	{
		struct stat buff;
//		cout << folder << endl;

		if(lstat((folder+temp[i]).c_str(), &buff) == -1)
		{
			perror("lstat");
			exit(1);
		}
		if(S_ISDIR(buff.st_mode))
		{
			string test3=folder;
			test3[test3.size()-1]=':';
			cout <<test3<< " " << folder <<endl;
		//	printer(temp,folder);
			cout << endl;
			string test1=folder+temp[i]+"/";
			strcpy(folder,test1.c_str());
			aflag(folder,flag_a);
		}
		else
		{
			string test3=folder;
			test3[test3.size()-1]=':';
			cout <<test3<< " " << folder <<endl;
//			printer(temp,folder);
		}
	}
//	string test3 = folder;
//	test3[test3.size()-1]=':';
//	cout << test3 << endl;
//	printer(temp,folder);
//	cout << endl;
	string test2= folder;
	int string_index=0;
	for (int j=0;j<test2.size();j++)
	{	
		if(test2[j]=='/'&& j!=test2.size()-1)
			string_index = j;
		
	}
	test2.resize(string_index+1);
	strcpy(folder,test2.c_str());



}


void rflag(char *folder)
{
	vector <string> temp,temp1;

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

	for(int i=0;i<temp.size();i++)
	{
		struct stat buff;
//		cout << temp[i] << endl;
		if(lstat((temp[i]).c_str(), &buff) == -1)
		{
			perror("lstat");
			exit(1);
		}
		if(S_ISDIR(buff.st_mode))
		{
			temp1.push_back("./"+temp[i]+"/");
			string test;
//			cout << "this is the other folder  " << folder <<endl;
			if(temp[i] == ".")
			{
				test=temp[i]+":";
			}
			else if(flag_a)
			{
				
				
			}
			else
				test = "./" + temp[i] + ":";
				strcpy(folder,test.c_str());
			
//			cout << folder << endl;
			
			test = "./" +temp[i]+"/";
			strcpy(folder,test.c_str());
//			cout << folder << endl;

			aflag(folder,flag_a);
//			cout << endl;
		
		}
//			cout << "this is folder  " << folder << endl << endl;
		

	}





}



int main(int argc, char *argv[])
{
int i =1;
string test1,test; 
char *folder = new char();
test = "./";
strcpy(folder,test.c_str());
while (argv[i]!=NULL) //sets which flags to mark
{
	string input = argv[i];
	if(input.at(0) == '-')
	{
		if (input .find("a") != string::npos)
		{
			flag_a=true;
		}
		if (input.find("l") != string::npos)
		{
			flag_l=true;
		}
		if (input.find("R") != string::npos)
		{
			flag_R=true;
		}
		if(!flag_a&&!flag_l&&!flag_R)
		{
			cout << "Error: argument "<< input << " not supported " << endl;
			exit(1);
		}


	}
	else 
	{
		test1 = argv[i] ;
		if(test1.at(test1.size()-1) != '/')
		{
			test = test1 + "/";
			strcpy(folder,test.c_str());
		}
		if(test1[0]!='.'  && test1[1]!='/')
		{
			
			test = "./" + test1;
			test1 = test;
			if(test1.at(test1.size()-1)!='/')
			{
				test=test1 + "/";
			}

			strcpy(folder,test.c_str());
		}
		else
			strcpy(folder,test1.c_str());
		//cout << folder << " works" << endl;
		}
	i++;
}
if (!(flag_a||flag_l||flag_R))
{
	aflag(folder,flag_a);
}
else if (flag_a && flag_l && flag_R)
{
}
else if(flag_a && flag_l)
{
	lflag(folder,flag_a);
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
	lflag(folder,flag_a);
}
else if(flag_R)
{
	rflag(folder);
}

delete folder;

return 0;
}

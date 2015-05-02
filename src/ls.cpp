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
	int width=0;
	int max_size;
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	for(int i=0;i<temp.size();i++)
	{
		if(max_size<temp[i].size())
			max_size = temp[i].size();
	}
	
	int term_width =  w.ws_col;
	int less = (term_width/max_size) -1;
	int check=0;
//	cout << folder << "works" << endl;
	for (int i=0;i<temp.size();i++)
	{

	//		string fake = temp[i];
	//		fake = "./src/"+fake;
	//		temp[i] = fake;
			struct stat buff;
			string fake = folder;
			int counter=0;
			for (int m=0;m<fake.size();m++)
			{
				if(fake.at(m)=='/')
					counter++;
			}
			if (counter<3)
			{
				fake = fake+temp[i];
			}

			if(lstat((fake).c_str(),&buff) ==-1)
			{
				perror("lstat");
				exit(1);
			}
			if(S_ISDIR(buff.st_mode))
			{
				if (temp[i][0]=='.')
					cout << left  <<white << blue << setw(max_size)<<  temp[i] <<RESET << " ";
				else
					cout <<  left <<blue << setw(max_size) << temp[i] << RESET << " ";
			}
			else if(buff.st_mode & S_IXUSR)
				cout <<   left <<green  <<setw(max_size) << temp[i] << RESET << " ";
				
			else 
				cout << left<< setw(max_size)  << temp[i]<< " ";
				
			check++;
			if(check>less)
			{
				cout <<endl;
				check=0;
			}
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
//	cout << folder <<endl;
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
		else if (flag&&flag_R&&(filespecs->d_name)[1]!='.')
			temp.push_back(filespecs->d_name);
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
//	for (int i=0; i<temp.size();i++)
//	{
//		cout << temp[i] << "  ";
//	}
//	cout << endl;
	string test3 = folder;
	test3[test3.size()-1]=':';
	if(flag_R)
		cout << test3 << endl;
	printer(temp,folder);
	if(flag_R)
		cout << endl;
	if(flag_R)
	{
	for(int i=0;i<temp.size();i++)
	{
		struct stat buff;

		if(lstat((folder+temp[i]).c_str(), &buff) == -1)
		{
			perror("lstat");
			exit(1);
		}
		if(S_ISDIR(buff.st_mode))
		{
//			string test3=folder;
//			test3[test3.size()-1]=':';
	//		cout <<test3<< " " << folder <<endl;
		//	printer(temp,folder);
	//		cout << endl;
			string test1=folder+temp[i]+"/";
			strcpy(folder,test1.c_str());
			aflag(folder,flag_a);
		}
//		else
//		{
//			string test3=folder;
//			test3[test3.size()-1]=':';
//			cout <<test3<< " " << folder <<endl;
//			printer(temp,folder);
//		}
//	cout << temp[i]<< " this is the folder  " << folder << endl;
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


}


void rflag(char *folder,bool flag)
{
	vector <string> temp;
	DIR *dirp;
	//	cout << folder <<endl;
	string ch = folder;

	if(NULL==(dirp=opendir(ch.c_str())))
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
		else if (flag&&flag_R&&(filespecs->d_name)[1]!='.')
			temp.push_back(filespecs->d_name);
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
	//	for (int i=0; i<temp.size();i++)
	//	{
	//		cout << temp[i] << "  ";
	//	}
	//	cout << endl;
	string test3 = folder;
	test3[test3.size()-1]=':';
	if(flag_R)
		cout << test3 << endl;
	if(flag_l)
		lflag(folder,flag_a);
	else
	printer(temp,folder);

	if(flag_R)
		cout << endl;
	if(flag_R)
	{
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
				//			string test3=folder;
				//			test3[test3.size()-1]=':';
				//		cout <<test3<< " " << folder <<endl;
				//	printer(temp,folder);
				//		cout << endl;
				if(temp[i]!="." && temp[i]!=".."){	
					string test1=folder+temp[i]+"/";
					strcpy(folder,test1.c_str());
					rflag(folder,flag_a);
				}
			}
			//		else
			//		{
			//			string test3=folder;
			//			test3[test3.size()-1]=':';
			//			cout <<test3<< " " << folder <<endl;
			//			printer(temp,folder);
			//		}
			//	cout << temp[i]<< " this is the folder  " << folder << endl;
		}
		//	string test3 = folder;
		//	test3[test3.size()-1]=':';
		//	cout << test3 << endl;
		//	printer(temp,folder);
		//	cout << endl;
		if(folder!=NULL){
			string test2= folder;
			int string_index=0;
			for (int j=0;j<test2.size();j++)
			{	
				if(test2[j]=='/'&& j!=test2.size()-1)
					string_index = j;

			}
			test2.resize(string_index);
			test2=test2+"/";
			strcpy(folder,test2.c_str());
		}
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
		if (test1=="..")
		{
			test1=argv[1];
			test1="./"+test1+"/";
			strcpy(folder,test1.c_str());
		}
		else
		{
			if(test1.at(test1.size()-1) != '/')
			{
				test = test1 + "/";
				int counter=0;
				for (int i=0;i<test.size();i++)
				{
					if(test.at(i)=='/')
						counter++;
				}
				if (counter>3)
				{
					test.resize(test.size()-1);
				}

				strcpy(folder,test.c_str());
			}
			else if(test1[0]!='.'  && test1[1]!='/')
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
			{
				strcpy(folder,test1.c_str());
				
			}
		}
			
			test = folder;
	}
	i++;
}
if (!(flag_a||flag_l||flag_R))
{
	aflag(folder,flag_a);
	exit(0);
}
else if (flag_a && flag_l && flag_R)
{
	rflag(folder,flag_a);
}
else if(flag_a && flag_l)
{
	lflag(folder,flag_a);
}
else if(flag_a && flag_R)
{
	rflag(folder,flag_a);
	exit(0);
}
else if(flag_l && flag_R)
{
	rflag(folder,flag_a);
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
	aflag(folder,flag_a);
}

delete folder;

return 0;
}

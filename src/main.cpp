#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <string>
#include <fstream>

using namespace std;
void io_pipe(string input);
void executer(char **argv,int& status)
{
	int pid=fork();
	if (pid == -1)//there was an error with the forking
	{
		perror("fork");//if error does the proper error output
		exit(1);
	}
	else if (pid ==0)//if the child process is running
	{
		
		if(-1==execvp(argv[0],argv))
		{
			status = -1;
			perror("execvp");
		}
		exit (1);
	}
	else if (pid >0)//parent process is running
	{
		if (-1 == wait(&status))// if the wait fails it displays error and exits
		{
			perror("wait");
			exit(1);
		}


	}
}
void checker(char** argvIN,char **argvOUT,char* ops,int& sz)//this is where the actual strtok takes place
{
	int j=0;
	char *word= strtok(argvIN[0],ops);//finds whatever operation is put in and breaks it apart
	
	while (word!=NULL)
	{
		argvOUT[j] = word;

		word = strtok(NULL, ops);
		j++;
	}
	argvOUT[j]=0;
	sz=j;							//also outputs the size of how many argv's were made
	


	delete[] word;					//deallocates memory
}


void initial(char *inputchar,  char** argv)//this is needed to initallize the first token to make things easier
{

	char *word = strtok(inputchar,"");
	int i =0;
	while (word != NULL)
	{
		argv[i] = word;
		
		word = strtok(NULL, "");
		i++;
	}
	argv[i]=0;//this is where the string end
	delete[] word;
}//the output on this should just be argv[0] which would have all the words in the inputchar


void stringtoken(string input)
{
	//put the input into characters rather than string
	char *inputchar = new char [input.length()+1];//makes a char* from string
	strcpy(inputchar,input.c_str());

	bool semi		 = false;
	bool andd		 = false;
	bool orr		 = false;

	bool spac		 = true;

	char *se		 = new char[3];
	char *an		 = new char[3];
	char *orrr		 = new char[3];
	char *spa		 = new char[3];
	char *exitC		 = new char[5];


	//just defining character pointers
	strcpy(exitC,"exit");
	strcpy(se,";");
	strcpy(an,"&&");
	strcpy(orrr,"||");
	strcpy(spa," \n");

	
	
	//these check if the operations are in the string and set the bool value
	if (input.find(";") != string::npos)
		semi = true;
	if (input.find("&&")!= string::npos)
		andd = true;
	if (input.find("||")!= string::npos)
		orr = true;
	//giving the size to each of the char ** 
	char **argvSEMI   = new char*[strlen(inputchar)];
	char **argvANDD   = new char*[strlen(inputchar)];
	char **argvORR    = new char*[strlen(inputchar)];
	char **argvSPACE  = new char*[strlen(inputchar)];
	char **argvIN     = new char*[strlen(inputchar)];
	
	initial(inputchar,argvIN); //this puts the whole input into a char** for further breakdown

	int status = 0;
	int status1 =0;
	
	if(semi||andd||orr||spac)
	{
		int sz=0;
		checker(argvIN,argvSEMI,se,sz);
		for (int i=0;i<sz;i++)//for loop for semi
		{
			if(andd||orr||spac)//this one is for &&
				{
					int sz1=0;
					checker(argvSEMI,argvANDD,an,sz1);
					for(int l=0;l<sz1;l++)//for loop for and
					{
						if(orr||spac)// this one is for ||
						{
							int sz2=0;
							checker(argvANDD,argvORR,orrr,sz2);
							for(int p=0;p<sz2;p++)//for loop for or
							{
								if (spac)//this is just for spaces 
								{
									int sz3=0;
									checker(argvORR,argvSPACE,spa,sz3);
								
									if(argvSPACE[0]==NULL)// this prevents the segfaults that would be caused if NULL was passed
									{

									}
									else if(strcmp(argvSPACE[0],exitC)==0)//compares if the value is exit and if it is it exits
									{
										exit(0);
									}
									else
										executer(argvSPACE,status);

								}
								if(status==0)//if status is zero that means that the program worked
								{

									p=sz2;
									status1=-1;//if it works that means the following && should work too
								}
								else//otherwise it moves to the next ORR
								{
									for (int k=0;k<sz2;k++)
										argvORR[k]=argvORR[k+1];
								}
							}
						}
						if (status ==0 ||(orr&&status1==-1))//if it is true then it moves to the next &&
						{
							for(int k=0;k<sz1;k++)
								argvANDD[k]=argvANDD[k+1];
								status =200;
						}
						else //if false then doesnt do the next &&
						{
							char *fa=new char[1];
							strcpy(fa,"");
							argvANDD[0]=fa;
							delete []fa;
						}
					}
				}//moves to the next ;
				
				for(int k=0;k<sz;k++)
					argvSEMI[k]= argvSEMI[k+1];
		}
	}
	//deallocates memory	
	delete[] inputchar;
	delete[] se;
	delete[] an;
	delete[] orrr;
	delete[] spa;
	delete[] exitC;
	delete[] argvSEMI;
	delete[] argvANDD;
	delete[] argvORR;
	delete[] argvSPACE;
	delete[] argvIN;
}
int number_of_io_redirections =0;
void io_pipe(string input)
{	
	
	char *inputchar = new char [input.length()+1];//makes a char* from string
	char *fakechar	= new char [input.length()+1];
	strcpy(inputchar,input.c_str());
	
	
	char **argvIN		= new char*[strlen(inputchar)];
	char **argvSPACE	= new char*[strlen(inputchar)];
	char **argvOPEN		= new char*[strlen(inputchar)];
	
	char *spa = new char[3];
	char *open1 = new char[3];
	strcpy(spa," \n");
	strcpy(open1,"<> ");
	
	int sz;

	bool out,dout,in,cerrr = false;
	bool sign = false;	



	//take OPEN [1] tokenize the space out of it and then execute it and the OPEN [2] should be passed into open
	
	unsigned int first =1;
	char *holder;
	int x=0;
while (first!=0)
{
	if(x==3)
		sign = true;
	first =0;
	if(input.find(">")!=string::npos)
	{
		unsigned int temp = input.find(">");
		first = temp;
		if(temp-1>0)
		{
			if(input.at(temp-1)=='2')
				cerrr=true;
		}
		temp = temp +1;
		if(temp<input.size())
		{
			if(input.at(temp)!='>')
				out=true;
			else if(input.at(temp)=='>')
			{
				out =false;
				dout = true;
			}
		}
	}
	if(input.find("<")!=string::npos)
	{
		if(first>input.find("<")||first ==0)
		{
			first=input.find("<");
			out = false;
			cerrr=false;
			dout = false;
			in = true;
		}
	}
	string fake = input;
	if(cerrr)
		fake.resize(first-1);

	else
		fake.resize(first);

	if(first+1<input.size()&&!dout&&first!=0)
		input =input.substr(first+1);

	else if(first+2<input.size()&&dout&&first!=0)
		input =input.substr(first+2);

	if(number_of_io_redirections==0)
	{
		strcpy(inputchar,fake.c_str());

		initial(inputchar,argvIN); //this puts the whole input into a char** for further breakdown

		checker(argvIN,argvSPACE,spa,sz);
	}

	number_of_io_redirections++;
	fake =input;
	strcpy(fakechar,fake.c_str());
	initial(fakechar,argvIN);
	checker(argvIN,argvOPEN,open1,sz);
	holder = argvOPEN[0]; // holds the file name to output or input
	if(out)
	{
		if(-1==open(holder,O_WRONLY|O_CREAT|O_TRUNC,0644))
			perror("open");
			x=1;
	}
	else if(dout)
	{
		if(-1==open(holder,O_WRONLY|O_CREAT|O_APPEND,0644))
			perror("open");
			x=2;
	}
	else if(in)
	{
		if(-1==open(holder,O_RDONLY))
			perror("open");
			x=3;
	}
	if(sign && x!=3)
	{
		cout << "Error: case not handled (cannot combine < and >)" << endl;
		return;
	}
	sign = false;
}

	int status =0;
	int pid=fork();
	if (pid == -1)//there was an error with the forking
	{
		perror("fork");//if error does the proper error output
		exit(1);
	}
	else if (pid ==0)//if the child process is running
	{
		int cl =1;
		if(out)
		{
			if (cerrr)
				cl=2;
			if(-1==close(cl))
				perror("close");
			if(-1==open(holder,O_WRONLY|O_CREAT|O_TRUNC,0644))
				perror("open");
		}
		else if(dout)
		{
			if(cerrr)
				cl =2;
			if(-1==close(cl))
				perror("close");
			if(-1==open(holder,O_WRONLY|O_CREAT|O_APPEND,0644))
				perror("open");
		}
		else if(in)
		{
			if(-1==close(0))
				perror("close");
			if(-1==open(holder,O_RDONLY))
				perror("open");
		}
		if(-1==execvp(argvSPACE[0],argvSPACE))
		{
			status = -1;
			perror("execvp");
		}
			
		exit (1);
	}
	else if (pid >0)//parent process is running
	{
		if (-1 == wait(&status))// if the wait fails it displays error and exits
		{
			perror("wait");
			exit(1);
		}
	}

	out=false;
	dout=false;
	in=false;
	
	delete inputchar;
	delete fakechar;
	
	delete[] argvIN;
	delete[] argvSPACE;
	delete[] argvOPEN;

	delete spa;
	delete open1;
}




int main()
{
	string prompt; // make a string for the prompt
	char host[333];
	char *log = getlogin();	
	prompt = log;

	if (!log) // get login info 
		perror("getlogin"); //if its not there error

	if (gethostname(host,300) !=0) //get host info
		perror("gethostname");  // error otherwise

	if (log && host!=NULL)
	{
		for (int i=0;i<50;i++)
		{
			if (host[i]=='.')
				host[i]='\0'; //this just makes it so the host ends at EX @hammer.cs.ucr.edu gets shortened to @hammer
		}
		prompt = prompt+"@"+host+" $ "; //puts dchou002@hammer $ together
	}
	else // if host or login failed 
	{
		prompt = "$ ";
	}
	string input;
	while (1)//infinite loop
	{
		number_of_io_redirections=0;
		cout << prompt;
		
		getline(cin,input);//gets input
		
		if (input.find("#") != string::npos)//resizes if the comment is found
		{
			input.resize(input.find("#"));
		}
		if(input.find(">")!=string::npos || input.find(">>")!=string::npos||input.find("<")!=string::npos)
			io_pipe(input);
		else
			stringtoken(input);

	}
	


	return 0;
}

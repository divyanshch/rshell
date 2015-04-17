#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

using namespace std;

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

	bool semi  = false;
	bool andd  = false;
	bool orr   = false;
	bool spac  = true;
	char *se   = new char[3];
	char *an   = new char[3];
	char *orrr = new char[3];
	char *spa  = new char[3];
	char *exitC = new char[5];
	//just defining character pointers
	strcpy(exitC,"exit");
	strcpy(se,";");
	strcpy(an,"&&");
	strcpy(orrr,"||");
	strcpy(spa," \n");
	//these check if the operations are in the string and set the bool value
	if (input.find(";")<input.size())
		semi = true;
	if (input.find("&&")<input.size())
		andd = true;
	if (input.find("||")<input.size())
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
									cout << argvSPACE[0] << endl;
									if(strcmp(argvSPACE[0],exitC)==0)//compares if the value is exit and if it is it exits
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

int main()
{
	string prompt; // make a string for the prompt
	char host[333];

	if (getlogin()==NULL) // get login info 
		perror("getlogin()"); //if its not there error

	if (gethostname(host,300) !=0) //get host info
		perror("gethostname()");  // error otherwise

	if (getlogin()!=NULL && gethostname(host,300)== 0)
	{
		for (int i=0;i<50;i++)
		{
			if (host[i]=='.')
				host[i]='\0'; //this just makes it so the host ends at EX @hammer.cs.ucr.edu gets shortened to @hammer
		}
		prompt = getlogin();
		prompt = prompt+"@"+host+" $ "; //puts dchou002@hammer $ together
	}
	else // if host or login failed 
	{
		prompt = "$ ";
	}

	string input;
	while (1)//infinite loop
	{
	
		cout << prompt;
		
		getline(cin,input);//gets input
		
		if (input.find("#") != string::npos)//resizes if the comment is found
		{
			input.resize(input.find("#"));
		}
		stringtoken(input);//starts the program

	}
	


	return 0;
}

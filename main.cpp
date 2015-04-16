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
	perror("fork");
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
void checker(char** argvIN,char **argvOUT,char* ops,int& sz)
{
	

	int j=0;
		char *word= strtok(argvIN[0],ops);
		
		while (word!=NULL)
		{
			argvOUT[j] = word;

//			cout << argvOUT[j] << endl;
			word = strtok(NULL, ops);
			j++;

		}
		argvOUT[j]=0;
		sz=j;
	
}
void checker1(char** argvIN,char **argvOUT)
{
	

	int j=0;
		char *word= strtok(argvIN[0]," \n");
		
		while (word!=NULL)
		{
			argvOUT[j] = word;

		//	cout << argvOUT[j] << endl;
			word = strtok(NULL, " \n");
			j++;

		}
		argvOUT[j]=0;
	
	
}


void initial(char *inputchar,  char** argv)
{

	char *word = strtok(inputchar,"");
	int i =0;
	while (word != NULL)
	{
		argv[i] = word;
		

	//	cout << argv[i] << endl;
		word = strtok(NULL, "");
		i++;
	}
	argv[i]=0;//this is where the string end
}






void stringtoken(string input)
{
//put the input into characters rather than string
	char *inputchar = new char [input.length()+1];
	strcpy(inputchar,input.c_str());

	bool semi,andd,orr,spac = false;
	char *se   = new char[3];
	char *an   = new char[3];
	char *orrr = new char[3];
	char *spa  = new char[3];
	char *exitC = new char[5];
	strcpy(exitC,"exit");
	strcpy(se,";");
	strcpy(an,"&&");
	strcpy(orrr,"||");
	strcpy(spa," \n");
		if (input.find(";")<input.size())
		{
			semi = true;
		}
		if (input.find("&&")<input.size())
		{
			andd = true;
		}
		if (input.find("||")<input.size())
		{
			orr = true;
		}
			spac = true;

		

	char **argvSEMI   = new char*[strlen(inputchar)];
	char **argvANDD   = new char*[strlen(inputchar)];
	char **argvORR    = new char*[strlen(inputchar)];
	char **argvSPACE  = new char*[strlen(inputchar)];
	char **argvIN     = new char*[strlen(inputchar)];
	int status1 =0;
	initial(inputchar,argvIN);
	int status = 0;
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
									if(strcmp(argvSPACE[0],exitC)==0)
									{
										//cout << "they are same" << endl;
										exit(0);
									}

//									cout << "executing" << endl;
									executer(argvSPACE,status);
								}
									if(status==0)//if status is zero that means that the program worked
									{
										p=sz2;
										status1=-1;
									}
									else
									{
										for (int k=0;k<sz2;k++)
											argvORR[k]=argvORR[k+1];
									}
							}
						}
						if (status ==0 ||(orr&&status1==0))
						{
							for(int k=0;k<sz1;k++)
								argvANDD[k]=argvANDD[k+1];
						}
						else 
						{
							char *fa=new char[1];
							strcpy(fa,"");
							argvANDD[0]=fa;
						}
						
					}
				}
				
				for(int k=0;k<sz;k++)
					argvSEMI[k]= argvSEMI[k+1];
		}
	}
		
		
}





//	else if(andd)
//	{
//		initial(inputchar, an,argvANDD);
//		if(orr||spac)// this one is for ||
//			{
//				checker(argvANDD,argvORR,orrr);
//
//				if (spac)//this is just for spaces 
//				{
//					checker1(argvORR,argvSPACE);
//					//cout << "executing" << endl;
//					//executer(argvSPACE);
//				}
//				
//			}
//	
//	}
//	else if(orr)
//	{
//		initial(inputchar, orrr,argvORR);
//		if (spac)//this is just for spaces 
//									cout << argvSPACE[0] << endl;
//				{
//					checker1(argvORR,argvSPACE);
//					//cout << "executing" << endl;
//					//executer(argvSPACE);
//				}
//			
//	}
//	else if(spac)
//	{
//		initial(inputchar, spa,argvSPACE);
//		executer(argvSPACE);
//	}

//		if(orr)
//		{
//			checker(argvSEMI,argvANDD,"&&");
//
//		}
//	}
//	else if(andd)
//	{
//		checker(argvSPACE,argvANDD,"&&");
//
//
//		if (orr)
//		{
//			checker(argvANDD,argvORR,"||");
//
//		}
//	}
//	else if (orr)
//	{
//		checker(argvSPACE,argvORR,"||");
//	}
//
//

	

//void checker(char** argv,char **argvORR,string ops)






//separate the word up
//the AND checker	
//	while (argvsemi[i]!=0){
//	char *loc=strchr(argvsemi[i],'&');
//	if (loc==NULL)
//		statusANDD = false;
//	else if (loc[1]=='&')
//		statusANDD = true;
//	if(statusANDD){
//	char *ANDD = strtok(argvsemi[0],"&&");
//	int j=0;
//	while (ANDD!=NULL)
//	{
//		argvANDD[j] = ANDD;
//		
//
//		cout << ANDD<< endl;
//		ANDD = strtok(NULL, "&&");
//		j++;
//
//
//	}
//	argvANDD[j]=0;
//	}
//	i++;
//	}
//	int j=0;	


//return stringtoken(argv[0],"&&");

	
	
	
	//executer(argv);
	


int main()
{
	string prompt;
	char host[333];

	if (getlogin()==NULL)
		perror("getlogin()");

	if (gethostname(host,300) !=0)
		perror("gethostname()");

	if (getlogin()!=NULL && gethostname(host,300)== 0)
	{
		for (int i=0;i<50;i++)
		{
			if (host[i]=='.')
				host[i]='\0';
		}
		prompt = getlogin();
		prompt = prompt+"@"+host+" $ fake ";
	}
	else
	{
		prompt = "$ ";
	}

	string input;
	while (1)
	{
	
		cout << prompt;
		
		getline(cin,input);
		
		if (input.find("#") != string::npos)
		{
			input.resize(input.find("#"));
		}
//		if(input.find("exit") < input.size())
//		{
//			input.resize(input.find("exit"));
			stringtoken(input);
//			exit(0);
//
//		}

	}
	



	return 0;
}

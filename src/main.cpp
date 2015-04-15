#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>

using namespace std;

void display_name()
{
	/*char* login = getlogin();
	if(!getlogin()) //displays the username
		perror("getlogin");
	*/
	char log[BUFSIZ];
	getlogin_r(log, BUFSIZ);
	char host_name[BUFSIZ];
	if(gethostname(host_name, BUFSIZ)== -1) //displays the host name
		perror("gethostname");

	char directory_name[BUFSIZ];
	if(getcwd(directory_name, BUFSIZ) == NULL) //displays the directory
		perror("getcwd");

	cout << log  << "@" << host_name  << ":" << directory_name << "$ ";
	
}

bool contain_exit(char** terminate)
{
	string exit_p = *terminate;
	if(exit_p == "exit")
		return true;
	else
		return false;
}
void tokenizer(string& conditional, char**& lol, char*& the_token)
{
	string user_input;
	string space = " ";
	string or_c = "||";
	string and_c = "&&";
	string semi_c = ";";
	int count = 0;
	size_t neg_f = -1;
	char** new_lol = (char**)malloc(BUFSIZ);
	bool token_flag = false;
	while(!token_flag && the_token != NULL)
	{
		string b = the_token;
		if((b.find(or_c) != neg_f) || (b.find(and_c) != neg_f) || (b.find(semi_c) != neg_f))
		{
			token_flag = true;
		}
		if(!token_flag)
		{
			new_lol[count] = the_token;
		}
		else
		{
			new_lol[count] = NULL;
			conditional = b;
		}
		count++;
		the_token = strtok(NULL, " ");
	}
	if(count == 1)
	{
		new_lol[1] = NULL;
	}
	lol = new_lol;
	//return lol;
}

bool run_execvp(char** arg)
{
	if(*arg == NULL)
		return false;
	int pid = fork();
	if(pid == -1)			//if couldn't fork, returns perror
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		char** argv = arg;
		if(contain_exit(argv));
		else
		{
			if(-1 != execvp(argv[0], argv))
				return true;
			else
			{
				perror("execvp");
				return false;
			}
			
		}
		//cout << "This is the child"<< endl;
		//return 0;
	}
	else if(pid > 0)
	{
		if(wait(0) == -1)
			perror("There was an error waiting"); //somehow an error during waiting
	}
	return true;
}

int main(int argc, char* argv[])
{
	while(1)		//whenever there is a system call remember to have perror
	{
		display_name();
		string user_input;
		string space = " ";
		string or_c = "||";
		string and_c = "&&";
		string semi_c = ";";
		string num_s = "#";

		getline(cin, user_input);
		int num_i = user_input.find(num_s, 0);
		if(num_i >= 0)
		{
			user_input = user_input.substr(0,num_i);
		}
		int index = 0;
		string::iterator it;
		for(it = user_input.begin(); it < user_input.end(); it++, index++)
		{
			int wtf = user_input.find(semi_c, index);
			int lamp = user_input.find(or_c, index);
			int melon = user_input.find(and_c, index);
			if(wtf >= 0 && (wtf < lamp || lamp == -1) && (wtf < melon || melon == -1))
			{
				user_input.insert(user_input.find(semi_c, index), " ");

				user_input.insert(user_input.find(semi_c, index)+1, " ");
				it = user_input.begin();
				it += user_input.find(semi_c, index) +1;
				index = user_input.find(semi_c, index) +1;
			}
			else if(lamp >= 0 && (lamp < wtf || wtf == -1) && (lamp < melon || melon == -1))
			{
				user_input.insert(user_input.find(or_c, index), " ");
			
				user_input.insert(user_input.find(or_c, index)+2, " ");
				it = user_input.begin();
				it += user_input.find(or_c, index) + 2;
				index = user_input.find(or_c, index) +2;
			}
			else if(melon >= 0 && (melon < wtf || wtf == -1) && (melon < lamp || lamp == -1))
			{
				user_input.insert(user_input.find(and_c, index), " ");
			
				user_input.insert(user_input.find(and_c, index)+2, " ");
				it = user_input.begin();
				it += user_input.find(and_c, index) + 2;
				index = user_input.find(and_c, index) +2;
			}
		}
		char* womp = (char*)user_input.c_str();
		char* token;
		token = strtok(womp, " 	");
		while(token != NULL)
		{
			char** lol;
			string condition;
			// bool func only returns false at the bottom
			//run_execvp(lol);
			//cout << condition << endl;

			while(token != NULL)
			{
				
				tokenizer(condition, lol, token);
				if(*lol != NULL)
					if(contain_exit(lol)) exit(1);
				if(condition == or_c)
				{
					if(run_execvp(lol) == true)
					{
						if(token != NULL)
						{
							tokenizer(condition,lol,token);
						}
						else
						{}
					}
				}
				else if(condition == and_c)
				{
					if(run_execvp(lol) == false)
					{
						if(token != NULL)
						{
							tokenizer(condition,lol,token);
						}
						else
						{}
					}

				}
				else if(condition == semi_c)
				{
					if(run_execvp(lol));
				}
				else
				{
					if(run_execvp(lol));
				}

			}
			
			break;
		}
	}
	return 0;
}

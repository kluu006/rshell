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
	char* login = getlogin();
	if(!getlogin()) //displays the username
		perror("getlogin");
	//char log[BUFSIZ];
	//getlogin_r(log, BUFSIZ);
	//	perror("getlogin_r");
	char host_name[BUFSIZ];
	if(gethostname(host_name, BUFSIZ)== -1) //displays the host name
		perror("gethostname");

	/*char directory_name[BUFSIZ];
	if(getcwd(directory_name, BUFSIZ) == NULL) //displays the directory
		perror("getcwd");*/

	cout << login  << "@" << host_name  << ":" /*<< directory_name*/ << "$ ";
	
}

bool contain_exit(char** terminate)	//check if exit
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
	string in_re = "<";
	string out_re = ">";
	string out_re2 = ">>";
	string pipe = "|";
	int count = 0;
	size_t neg_f = -1;
	char** new_lol = (char**)malloc(BUFSIZ);
	bool token_flag = false;			// the parsing
	while(!token_flag && the_token != NULL)
	{
		string b = the_token;
		if((b.find(or_c) != neg_f) || (b.find(and_c) != neg_f) || (b.find(semi_c) != neg_f) || (b.find(in_re) != neg_f) || (b.find(out_re) != neg_f) || (b.find(out_re2) != neg_f) || (b.find(pipe) != neg_f))
		{
			token_flag = true;	//if found connector, end parse
		}
		if(!token_flag)
		{
			new_lol[count] = the_token;	//pushes everything before end of parse into char**
		}
		else
		{
			new_lol[count] = NULL;		//remember to have NULL at the end
			conditional = b;
		}
		count++;
		the_token = strtok(NULL, " ");
	}
	if(!token_flag)
	{
		new_lol[count] = NULL;		// if no connector still add NULL
	}
	lol = new_lol;
}

bool run_execvp(char** arg)
{
	int status = 0;
	if(*arg == NULL)
		return false;
	int pid = fork();
	if(pid == -1)			//if couldn't fork, returns perror
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)	//this be child
	{
		status = execvp(arg[0], arg);
		if(status == -1)
		{
			perror("execvp");
			exit(1);		//exit if error
		}
			
	}
	else if(pid > 0)
	{
		int r = waitpid(pid, &status,0);
		if(r == -1)
			perror("wait"); //somehow an error during waiting
		string init = arg[0];
		if(contain_exit(arg));	//does it contain exit
		else if(init == "true") return true;	//check if it was true
		else if(init == "false") return false;	//check if it was false
		if(status > 0)
		{
			//child ran execvp and it was an error
			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	while(cin.good())		//whenever there is a system call remember to have perror
	{
		display_name();
		string user_input;
		string space = " ";
		string or_c = "||";
		string and_c = "&&";
		string semi_c = ";";
		string num_s = "#";	
		getline(cin, user_input);			//user input
		int num_i = user_input.find(num_s, 0);
		if(num_i >= 0)
		{
			user_input = user_input.substr(0,num_i);	//look for a comment
		}
		int index = 0;
		string::iterator it;					//this is a helper function
		for(it = user_input.begin(); it < user_input.end(); it++, index++) //love my variables
		{
			int wtf = user_input.find(semi_c, index);
			int lamp = user_input.find(or_c, index);
			int melon = user_input.find(and_c, index);
			if(wtf >= 0 && (wtf < lamp || lamp == -1) && (wtf < melon || melon == -1))//adds spaces if semi colon found
			{
				user_input.insert(user_input.find(semi_c, index), " ");

				user_input.insert(user_input.find(semi_c, index)+1, " ");
				it = user_input.begin();
				it += user_input.find(semi_c, index) +1;
				index = user_input.find(semi_c, index) +1;
			}
			else if(lamp >= 0 && (lamp < wtf || wtf == -1) && (lamp < melon || melon == -1))//adds spaces if or connector found
			{
				user_input.insert(user_input.find(or_c, index), " ");
			
				user_input.insert(user_input.find(or_c, index)+2, " ");
				it = user_input.begin();
				it += user_input.find(or_c, index) + 2;
				index = user_input.find(or_c, index) +2;
			}
			else if(melon >= 0 && (melon < wtf || wtf == -1) && (melon < lamp || lamp == -1))//adds spaces if and connector found
			{
				user_input.insert(user_input.find(and_c, index), " ");
			
				user_input.insert(user_input.find(and_c, index)+2, " ");
				it = user_input.begin();
				it += user_input.find(and_c, index) + 2;
				index = user_input.find(and_c, index) +2;
			}
		}
		string in_re = "<";
		string out_re = ">";
		string out_re2 = ">>";
		string pipe = "|";
		index = 0;
		for(it = user_input.begin(); it < user_input.end(); it++, index++) //love my variables
		{
			int wtf = user_input.find(in_re, index);
			int lamp = user_input.find(out_re, index);
			int melon = user_input.find(out_re2, index);
			int wombo = user_input.find(pipe, index);
			if(wtf >= 0 && (wtf < lamp || lamp == -1) && (wtf < melon || melon == -1) && (wtf < wombo || wombo ==-1))//adds spaces if semi colon found
			{
				user_input.insert(user_input.find(in_re, index), " ");

				user_input.insert(user_input.find(in_re, index)+1, " ");
				it = user_input.begin();
				it += user_input.find(in_re, index) +1;
				index = user_input.find(in_re, index) +1;
			}
			else if(lamp >= 0 && (lamp < wtf || wtf == -1) && (lamp < melon || melon == -1) && (lamp < wombo || wombo == -1))//adds spaces if or connector found
			{
				user_input.insert(user_input.find(out_re, index), " ");
			
				user_input.insert(user_input.find(out_re, index)+1, " ");
				it = user_input.begin();
				it += user_input.find(out_re, index) + 1;
				index = user_input.find(out_re, index) +1;
			}
			else if(melon >= 0 && (melon < wtf || wtf == -1) && (melon < lamp || lamp == -1) && (melon < wombo || wombo == -1))//adds spaces if and connector found
			{
				user_input.insert(user_input.find(out_re2, index), " ");
			
				user_input.insert(user_input.find(out_re2, index)+2, " ");
				it = user_input.begin();
				it += user_input.find(out_re2, index) + 2;
				index = user_input.find(out_re2, index) +2;
			}
			else if(wombo >= 0 && (wombo < wtf || wtf == -1) && (wombo < lamp || lamp == -1) && (wombo < melon || melon == -1))//adds spaces if and connector found
			{
				user_input.insert(user_input.find(pipe, index), " ");
			
				user_input.insert(user_input.find(pipe, index)+1, " ");
				it = user_input.begin();
				it += user_input.find(pipe, index) + 1;
				index = user_input.find(pipe, index) +1;
			}
		}
		char* womp = (char*)user_input.c_str();
		char* token;
		token = strtok(womp, " \t\n"); //token by spaces and tabs
		while(token != NULL)
		{
			char** lol;		//love my variables
			string condition;
			bool prev_or = false; //conditions so the next value always run unless otherwise
			bool prev_and = true;
			while(token != NULL)
			{
				tokenizer(condition, lol, token); //parses char** as commands, lol is the command, condition is the connector
				if(*lol != NULL && !prev_or && prev_and)
					if(contain_exit(lol)) exit(1);	//exits program
				if(condition == or_c)
				{
					if(prev_or);			//if previous condition true, does not run exec
					else if(!prev_and)
					{
						prev_and = true;	//case where previous condition was AND and that it failed the previous condition
						prev_or = false;
					}
					else if(run_execvp(lol) == true)	//current statement is true
					{
						if(token != NULL)
							prev_or = true;		//sets to know that the previous condition under OR is true
					}
				}
				else if(condition == and_c)
				{
					if(!prev_and);			//if previous condition was false; does not run exec
					else if(prev_or)
					{
						prev_and = true;
						prev_or = false;
					}
					else if(run_execvp(lol) == false)	//current statement is false
					{
						if(token != NULL)
							prev_and = false;
					}

				}
				else if(condition == semi_c)	
				{
					if(!prev_or && prev_and)
					{
						if(run_execvp(lol));	//runs if previous or was false and if previous and was true
					}
					prev_or = false;	//resets conditions for passing exec
					prev_and = true;
				}
				else
					if(run_execvp(lol));
				delete lol;	//remember to delete char** at end of command to prevent memory leaks

			}
			break;
		}
	}
	return 0;
}

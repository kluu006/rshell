#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <errno.h>
#include <fcntl.h>

using namespace std;
const string in_re = "<";
const string out_re = ">";
const string out_re2 = ">>";
const string pipes = "|";
const string space = " ";
const string or_c = "||";
const string and_c = "&&";
const string semi_c = ";";
const string num_s = "#";
int descrip_in;
int descrip_out;
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

void add_spaces(string& user_input)
{
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
	index = 0;
	for(it = user_input.begin(); it < user_input.end(); it++, index++) //love my variables
	{
		int wtf = user_input.find(in_re, index);
		int lamp = user_input.find(out_re2, index);
		int melon = user_input.find(out_re, index);
		int wombo = user_input.find(pipes, index);
		if(wtf >= 0 && (wtf < lamp || lamp == -1) && (wtf < melon || melon == -1) && (wtf < wombo || wombo ==-1))//adds spaces if semi colon found
		{
			user_input.insert(user_input.find(in_re, index), " ");

			user_input.insert(user_input.find(in_re, index)+1, " ");
			it = user_input.begin();
			it += user_input.find(in_re, index) +1;
			index = user_input.find(in_re, index) +1;
		}
		else if(lamp >= 0 && (lamp < wtf || wtf == -1) && (lamp <= melon || melon == -1) && (lamp < wombo || wombo == -1))//adds spaces if or connector found
		{
			user_input.insert(user_input.find(out_re2, index), " ");
		
			user_input.insert(user_input.find(out_re2, index)+2, " ");
			it = user_input.begin();
			it += user_input.find(out_re2, index) + 2;
			index = user_input.find(out_re2, index) +2;
		}
		else if(melon >= 0 && (melon < wtf || wtf == -1) && (melon < lamp || lamp == -1) && (melon < wombo || wombo == -1))//adds spaces if and connector found
		{
			user_input.insert(user_input.find(out_re, index), " ");
		
			user_input.insert(user_input.find(out_re, index)+1, " ");
			it = user_input.begin();
			it += user_input.find(out_re, index) + 1;
			index = user_input.find(out_re, index) +1;
		}
		else if(wombo >= 0 && (wombo < wtf || wtf == -1) && (wombo < lamp || lamp == -1) && (wombo < melon || melon == -1))//adds spaces if and connector found
		{
			user_input.insert(user_input.find(pipes, index), " ");
		
			user_input.insert(user_input.find(pipes, index)+1, " ");
			it = user_input.begin();
			it += user_input.find(pipes, index) + 1;
			index = user_input.find(pipes, index) +1;
		}
	}
}

bool contain_exit(char** terminate)	//check if exit
{
	string exit_p = *terminate;
	if(exit_p == "exit")
		return true;
	else
		return false;
}
void tokenizer(string& conditional, char**& lol, char*& the_token, bool& redirection)
{
	string user_input;
	int count = 0;
	size_t neg_f = -1;
	char** new_lol = (char**)malloc(BUFSIZ);
	bool token_flag = false;			// the parsing
	while(!token_flag && the_token != NULL)
	{
		string b = the_token;
		if((b.find(or_c) != neg_f) || (b.find(and_c) != neg_f) || (b.find(semi_c) != neg_f))
		{
			token_flag = true;	//if found connector, end parse
		}
		if((b.find(in_re) != neg_f) || (b.find(out_re) != neg_f) || (b.find(out_re2) != neg_f) || (b.find(pipes) != neg_f))
		{
			redirection = true;
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
void trip_wire(string& trip, char**lol, size_t index_re)
{
	bool redir_flag = false;
	size_t i;
	for(i = index_re; lol[i] != NULL && !redir_flag; i++, index_re++)
	{
		string hello = lol[i];
		if(hello == out_re) trip = out_re, redir_flag = true;
		else if(hello == out_re2) trip = out_re2, redir_flag = true;
		else if(hello == in_re) trip = in_re, redir_flag = true;
		else if(hello == pipes) trip = pipes, redir_flag = true;
	}
}

void redirectioner(string& conditional_re, char**& wolol, char** lol, bool& redirection, size_t& index_re, size_t& exec_pos)
{
	bool redir_flag = false;
	//char** wololo = (char**)malloc(BUFSIZ);
	size_t i = index_re;
	size_t j;
	if(i > 0) j = 0;
	else j = 1;
	for( ; lol[i] != NULL && !redir_flag; i++, j++, index_re++)
	{
		string hello = lol[i];
		cout << hello << endl;
		if(j == 0);
		else if(hello == out_re) conditional_re = out_re, wolol[exec_pos] = NULL, redir_flag = true;
		else if(hello == out_re2) conditional_re = out_re2, wolol[exec_pos] = NULL, redir_flag = true;
		else wolol[exec_pos] = lol[i], exec_pos++;
	}
	//wolol = wololo;
	if(redir_flag == false) redirection = false, wolol[exec_pos - 1] = NULL;
}

vector<string> grab_files_out(char** lol, size_t index_re)
{
	vector<string> the_files;
	bool redir_flag = false;
	for(size_t i = index_re; lol[i] != NULL && !redir_flag; i++)
	{
		if((string)lol[i] == in_re || (string)lol[i] == out_re || (string)lol[i] == out_re2 || (string)lol[i] == pipes) redir_flag = true;
		else
		{
			the_files.push_back(lol[i]);
			break;
		}
	}
	return the_files;
}

bool run_redir_out(string conditional, char** wolol, vector<string> files)
{
	int fd[2];
	if(-1 == pipe(fd)) perror("pipe"), exit(1);
	for(size_t u = 0; u < files.size(); u++)
	{
		int id = -1;
		if(conditional == ">")
		{
			id = open(files.at(u).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 00664);
			cout << id << endl;
		}
		else if(conditional == ">>")
		{
			id = open(files.at(u).c_str(), O_WRONLY | O_CREAT | O_APPEND, 00664);
			cout << id << endl;
		}
		int pid = fork();
		if (pid == 0){
			close(1);
			dup(id);// was closed, but now assign fd[1] to the file descriptor that was closed
			if(-1 == execvp(wolol[0], wolol))
			{
				perror("execvp");
				exit(1);
			}
		}
		else if(pid > 0){
			wait(0);
			close(id);
			close(0);
			dup(fd[0]);
			close(fd[0]);
		}
	}
	return true;
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
	descrip_in = dup(0);
	if(descrip_in == -1) perror("dup"), exit(1);
	descrip_out = dup(1);
	if(descrip_out == -1) perror("dup"), exit(1);
	while(cin.good())		//whenever there is a system call remember to have perror
	{
		display_name();
		string user_input;
		getline(cin, user_input);			//user input
		add_spaces(user_input);
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
				bool redirection = false;
				tokenizer(condition, lol, token,redirection); //parses char** as commands, lol is the command, condition is the connector
				string condition_re;
				size_t index_re = 0;
				//if(redirection) lol_1 = lol;//check if leak
				if(!redirection)
				{
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
					free(lol);	//remember to delete char** at end of command to prevent memory leaks
				}
				while(redirection)
				{
					string trip;
					trip_wire(trip, lol, index_re);
					char** wolol = (char**)malloc(BUFSIZ);
					size_t exec_pos = index_re;
					while(redirection)
					{
						size_t index_re_copy = index_re;
						redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos);
						vector<string> files;
						if(trip == out_re || trip == out_re2)
						{
							if(condition_re == out_re || condition_re == out_re2)
							{
								files = grab_files_out(lol, index_re_copy);
								run_redir_out(condition_re, wolol, files);
								index_re = index_re_copy;
							}
							else break;
						}
						else if(trip == in_re)
						{
							if(conditon_re == in_re)
							{
								files = grab_files_in(lol, index_re_copy);
								run_redir_in(wolol, files);
								index_re = index_re_copy;
							}
							else break;
						}
						else if(condition == in_re);
						
					}
					free(wolol);
					dup2(descrip_in, 0);
					dup2(descrip_out, 1);
				}
			}
			break;
		}
	}
	return 0;
}

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
	if(login == NULL) //displays the username
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
	int down = 0;
	bool dont_do_pipe;
	string::iterator it;					//this is a helper function
	for(it = user_input.begin(); it < user_input.end(); it++, index++) //love my variables
	{
		int wtf = user_input.find(semi_c, index);
		int lamp = user_input.find(or_c, index);
		int melon = user_input.find(and_c, index);
		int wombo = user_input.find(pipes, index);
		if(down !=0) down--;
		else if(down == 0) dont_do_pipe = false;
		if(dont_do_pipe) down = 2;
		if(wtf >= 0 && (wtf < lamp || lamp == -1) && (wtf < melon || melon == -1) && (wtf < wombo || wombo == -1))//adds spaces if semi colon found
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
			dont_do_pipe = true;
		}
		else if(melon >= 0 && (melon < wtf || wtf == -1) && (melon < lamp || lamp == -1) && (melon < wombo || wombo == -1))//adds spaces if and connector found
		{
			user_input.insert(user_input.find(and_c, index), " ");
		
			user_input.insert(user_input.find(and_c, index)+2, " ");
			it = user_input.begin();
			it += user_input.find(and_c, index) + 2;
			index = user_input.find(and_c, index) +2;
		}
		else if((wombo >= 0 && !dont_do_pipe) && (wombo < wtf || wtf == -1) && (wombo < lamp || lamp == -1) && (wombo < melon || melon == -1))//adds spaces if and connector found
		{
			user_input.insert(user_input.find(pipes, index), " ");
		
			user_input.insert(user_input.find(pipes, index)+1, " ");
			it = user_input.begin();
			it += user_input.find(pipes, index) + 1;
			index = user_input.find(pipes, index) +1;
		}
	}
	index = 0;
	for(it = user_input.begin(); it < user_input.end(); it++, index++) //love my variables
	{
		int wtf = user_input.find(in_re, index);
		int lamp = user_input.find(out_re2, index);
		int melon = user_input.find(out_re, index);
		if(wtf >= 0 && (wtf < lamp || lamp == -1) && (wtf < melon || melon == -1))//adds spaces if semi colon found
		{
			user_input.insert(user_input.find(in_re, index), " ");

			user_input.insert(user_input.find(in_re, index)+1, " ");
			it = user_input.begin();
			it += user_input.find(in_re, index) +1;
			index = user_input.find(in_re, index) +1;
		}
		else if(lamp >= 0 && (lamp < wtf || wtf == -1) && (lamp <= melon || melon == -1))//adds spaces if or connector found
		{
			user_input.insert(user_input.find(out_re2, index), " ");
		
			user_input.insert(user_input.find(out_re2, index)+2, " ");
			it = user_input.begin();
			it += user_input.find(out_re2, index) + 2;
			index = user_input.find(out_re2, index) +2;
		}
		else if(melon >= 0 && (melon < wtf || wtf == -1) && (melon < lamp || lamp == -1))//adds spaces if and connector found
		{
			user_input.insert(user_input.find(out_re, index), " ");
		
			user_input.insert(user_input.find(out_re, index)+1, " ");
			it = user_input.begin();
			it += user_input.find(out_re, index) + 1;
			index = user_input.find(out_re, index) +1;
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
		else if((b.find(in_re) != neg_f) || (b.find(out_re) != neg_f) || (b.find(out_re2) != neg_f) || ((b.find(pipes) != neg_f) && (b.find(or_c) == neg_f)))
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
	else if(count == 1) new_lol[0] = NULL;
	lol = new_lol;
}

vector<string> trip_wire(string& trip, char**lol, size_t index_re, size_t& cat_size, bool& redirection)
{
	if(lol[index_re] != NULL && (lol[index_re] == out_re || lol[index_re] == out_re2 || lol[index_re] == in_re || lol[index_re] == pipes)) redirection = false, cerr << "Not valid" << endl;
	bool redir_flag = false;
	vector<string> redir;
	size_t i;
	cat_size = 0;
	for(i = index_re; lol[i] != NULL; i++, index_re++)
	{
		string hello = lol[i];
		if(hello == out_re) redir_flag = true, redir.push_back(out_re);
		else if(hello == out_re2) trip = out_re2, redir_flag = true, redir.push_back(out_re2);
		else if(hello == in_re) redir_flag = true, redir.push_back(in_re);
		else if(hello == pipes) redir_flag = true, redir.push_back(pipes);
		if(!redir_flag)cat_size++;
	}
	if(lol[i] == NULL && redir.size() != 0) redir.push_back(redir.at(redir.size()-1));
	return redir;
}

void redirectioner(string& conditional_re, char**& wolol, char** lol, bool& redirection, size_t& index_re, size_t& exec_pos, size_t p)
{
	if(lol[index_re] != NULL && (lol[index_re] == out_re || lol[index_re] == out_re2 || lol[index_re] == in_re || lol[index_re] == pipes)) redirection = false, cerr << "Not valid" << endl;
	bool redir_flag = false;
	size_t i = index_re;
	size_t j;
	if(i > 0) j = 0;
	else j = 1;
	string hello;
	//size_t op = exec_pos;
	size_t pipe_pos = exec_pos;
	for( ; lol[i] != NULL && !redir_flag; i++, j++, index_re++)
	{
		hello = lol[i];
		//cout << hello << endl;
		if(j == 0);
		else if(hello == out_re) conditional_re = out_re, wolol[exec_pos] = NULL, redir_flag = true;
		else if(hello == out_re2) conditional_re = out_re2, wolol[exec_pos] = NULL, redir_flag = true;
		else if(hello == in_re) conditional_re = in_re, wolol[exec_pos] = NULL,redir_flag = true;//wrote
		else if(hello == pipes) conditional_re = pipes, wolol[pipe_pos] = NULL, redir_flag = true;
		else wolol[exec_pos] = lol[i], exec_pos++;
		pipe_pos++;
		//if((lol[op] == out_re || lol[op] == out_re2 || lol[op] == in_re || lol[op] == pipes)) break;
	}
	//if(op == 1) cerr << "Syntax error: no inputs." << endl;
	//wolol = wololo;
	if(!redir_flag && conditional_re == in_re)
	{
		size_t k = 0;
		for( ; k < p; k++) wolol[k] = lol[k];
			wolol[k] = NULL;
		redirection = false;
	}
	if(redir_flag == false)
	{
		redirection = false;
		if(exec_pos > 0) wolol[exec_pos - 1] = NULL;
		else wolol[0] = NULL;
	}
	
	if(redir_flag && conditional_re == in_re)
	{
		size_t k = 0;
		for( ; k < p; k++) wolol[k] = lol[k];
			wolol[k] = NULL;
	}
	if(!redir_flag && conditional_re == pipes)
	{
		wolol[pipe_pos] = NULL;
	}
	if(conditional_re == pipes)
	{
		exec_pos = 0;
	}
}

void pipe_er(string& conditional_re, char**& wolol, char** lol, bool& redirection, size_t& index_re)
{
	if(lol[index_re] != NULL && (lol[index_re] == out_re || lol[index_re] == out_re2 || lol[index_re] == in_re || lol[index_re] == pipes)) redirection = false, cerr << "Not valid" << endl;
	bool redir_flag = false;
	//char** wololo = (char**)malloc(BUFSIZ);
	size_t i = index_re;
	size_t pipe_pos = 0;
	string hello;
	for( ; lol[i] != NULL && !redir_flag; i++, pipe_pos++, index_re++)
	{
		hello = lol[i];
		//cout << hello << endl;
		if(hello == out_re) conditional_re = out_re, wolol[pipe_pos] = NULL, redir_flag = true;
		else if(hello == out_re2) conditional_re = out_re2, wolol[pipe_pos] = NULL, redir_flag = true;
		else if(hello == in_re) conditional_re = in_re, wolol[pipe_pos] = NULL, redir_flag = true;
		else if(hello == pipes) conditional_re = pipes, wolol[pipe_pos] = NULL, redir_flag = true;
		else wolol[pipe_pos] = lol[i];
	}
	//wolol = wololo;
	if(redir_flag == false)
	{
		redirection = false;
		if(pipe_pos > 0) wolol[pipe_pos] = NULL;
		else wolol[0] = NULL;
	}
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

vector<string> grab_files_in(char** lol, size_t index_re)
{
	vector<string> the_files;
	bool redir_flag = false;
	for(size_t i = index_re; lol[i] != NULL && !redir_flag; i++)
	{
		if((string)lol[i] == in_re || (string)lol[i] == out_re || (string)lol[i] == out_re2 || (string)lol[i] == pipes) redir_flag = true;
		else 
		{
			//cout << lol[i] << endl;
			the_files.push_back(lol[i]);
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
			if(id == -1){
				perror ("Cannot open file");
				return false;
			}
		}
		else if(conditional == ">>")
		{
			id = open(files.at(u).c_str(), O_WRONLY | O_CREAT | O_APPEND, 00664);
			if(id == -1){
				perror ("Cannot open file");
				return false;
			}
		}
		int pid = fork();
		if(pid == -1){
			perror("fork");
			exit(1);
		}
		if (pid == 0){
			if(-1 == close(1)) perror("close"), exit(1);
			if(-1 == dup(id)) perror("dup"), exit(1);
			if(-1 == execvp(wolol[0], wolol))
			{
				perror("execvp");
				exit(1);
			}
		}
		else if(pid > 0){
			if(-1 == wait(0)) perror("wait"), exit(1);
			if(-1 == close(id)) perror("close"), exit(1);
			if(-1 == close(0)) perror("close"), exit(1);
			if(-1 == dup(fd[0])) perror("dup"), exit(1);
			if(-1 == close(fd[0])) perror("close"), exit(1);
			if(-1 == close(fd[1])) perror("close"), exit(1);
		}
	}
	return true;
}
bool run_redir_in_out(string dolol, char** polol, string conditional, string file)
{
	int kid;
	int ID = open(dolol.c_str(), O_RDONLY);
	if(-1 == ID){
		perror("Not an input file");
		return false;
	}
	if(conditional == ">")
	{
		kid = open(file.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00664);
		if(-1 == kid){
			perror("open");
			exit(1);
		}
	}
	else if(conditional == ">>")
	{
		kid = open(file.c_str(), O_RDWR | O_CREAT | O_APPEND, 00664);
		if(-1 == kid){
			perror ("open");
			exit(1);
		}
	}
	if(-1 == dup2(ID, 0)) perror("dup"), exit(1);
	if(-1 == dup2(kid, 1)) perror("dup"), exit(1);
	int pid = fork();
	if(pid == -1 )
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		if(-1 == execvp(polol[0], polol)){
			perror("execvp");
			exit(1);
		}
	}
	else if(pid > 0)
	{
		if(-1 == close(ID)) perror("close"), exit(1);
		if(-1 == close(kid)) perror("close"), exit(1);
		if(-1 == wait(0)) perror("wait"), exit(1);
		if(-1 == dup2(descrip_in, 0)) perror("dup"), exit(1);
		if(-1 == dup2(descrip_out, 1)) perror("dup"), exit(1);
	}
	return true;
}
bool run_redir_pipe_out(string conditional, char** polol, vector<string> files, int* fd)
{
	string file = files.at(0);
	//cout << files.at(0) << endl << files.at(0) << endl;
	int kid;
	if(conditional == ">")
	{
		kid = open(file.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00664);
		if(-1 == kid){
			perror("open");
			exit(1);
		}
	}
	else if(conditional == ">>")
	{
		kid = open(file.c_str(), O_RDWR | O_CREAT | O_APPEND, 00664);
		if(-1 == kid){
			perror ("open");
			exit(1);
		}
	}
	//dup2(fd[0], 0);
	if(-1 == dup2(kid, 1)) perror("dup"), exit(1);
	int pid = fork();
	if( pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		if(-1 == execvp(polol[0], polol)){
			perror("execvp");
			exit(1);
		}
	}
	else if(pid > 0)
	{
		if(-1 == close(fd[1])) perror("close"), exit(1);
		if(-1 == wait(0)) perror("wait"), exit(1);
		//if(-1 == close(kid)) perror("close"), exit(1);
		if(-1 == close(fd[0])) perror("close"), exit(1);
		if(-1 == dup2(descrip_in, 0)) perror("dup"), exit(1);
		if(-1 == dup2(descrip_out, 1)) perror("dup"), exit(1);
	}
	return true;

}
bool run_redir_in(char** wolol, string files)
{
	int ID;
	ID = open(files.c_str(), O_RDONLY);
	if(ID == -1){
		perror("Not an input file");
		return false;
	}
	int pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		if(-1 == close(0)) perror("close"), exit(1);
		if(-1 == dup(ID)) perror("dup"), exit(1);
		if(-1 == execvp(wolol[0], wolol))
		{
			perror("execvp");
			exit(1);
		}
	}
	else if(pid > 0){
		if(-1 == wait(0)) perror("wait"), exit(1);
		if(-1 == close(ID)) perror("close"), exit(1);
	}
	return true;

}

bool run_pipe_after(char** wolol, int* fd)
{
	int status;
	int pid = fork();
	if(pid == -1){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		if(-1 == close(1)) perror("close"), exit(1);
		if(-1 == dup(fd[1])) perror("dup"), exit(1);
		if(-1 == close(fd[0])) perror("close"), exit(1);
		if(-1 == execvp(wolol[0], wolol)){
			perror("execvp");
			exit(1);
		}
	}
	else if(pid > 0){
		if(-1 == wait(&status)) perror("wait"), exit(1);
		if(-1 == close(0)) perror("close"), exit(1);
		if(-1 == dup2(fd[0], 0)) perror("dup"), exit(1);
		if(-1 == close(fd[1])) perror("close"), exit(1);
	}
	return true;
}
void run_pipe_end(char** wolol)
{
	int statuz = 0;
	int pid_pipe = fork();
	if(pid_pipe == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid_pipe == 0)
	{
		if(-1 == execvp(wolol[0], wolol))
		{
			perror("execvp");
			exit(1);
		}
	}
	else if(pid_pipe > 0)
	{
		if(-1 == wait(&statuz)) perror("wait"), exit(1);
		if(WEXITSTATUS(statuz) == 0)
		{
			if(-1 == dup2(descrip_in, 0)) perror("dup"), exit(1);
			if(-1 == dup2(descrip_out, 1)) perror("dup"), exit(1);
		}
	}
}
bool run_in_pipe(char**polol, string file, int* fd)
{
	//cout << *polol << endl;
	int ID = open(file.c_str(), O_RDONLY);
	if(-1 == ID){
		perror("Not an input file");
		return false;
	}
	int status;
	int pid = fork();
	if(pid == -1){
		perror("fork");
		exit(1);
	}
	else if (pid == 0){
		if(-1 == close(0)) perror("close"), exit(1);
		if(-1 == dup(ID)) perror("dup"), exit(1);
		if(-1 == close(1)) perror("close"), exit(1);
		if(-1 == dup(fd[1])) perror("dup"), exit(1);
		if(-1 == close(fd[0])) perror("close"), exit(1);
		if(-1 == execvp(polol[0], polol))
		{
			perror("execvp");
			exit(1);
		}
	}
	else if(pid > 0){
		if(-1 == wait(&status)) perror("wait"), exit(1);
		if(-1 == close(ID)) perror("close"), exit(1);
		//close(0);
		if(-1 == dup2(fd[0], 0)) perror("dup"), exit(1);
		if(-1 == close(fd[1])) perror("close"), exit(1);
	}
	return true;
}
bool run_lonely_pipe(char** wolol)
{
	int pid = fork();
	if(pid == -1){
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		if(-1 == execvp(wolol[0], wolol))
		{
			perror("execvp");
			exit(1);
		}

	}
	else if(pid > 0)
	{
		if(-1 == wait(0)) perror("wait"), exit(1);
	}
	return true;
}
bool run_execvp(char** arg, bool& koo)
{
	int status = 0;
	if(*arg == NULL)
	{
		koo = true;
		//free(arg);
		return false;
	}
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
		else if(init == "true") 
		{
			free(arg);
			return true;	//check if it was true
		}
		else if(init == "false") 
		{
			free(arg);
			return false;	//check if it was false
		}
		if(status > 0)
		{
			//child ran execvp and it was an error
			free(arg);
			return false;
		}
	}
	free(arg);
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
				bool koo = false;
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
						if(prev_or) free(lol);			//if previous condition true, does not run exec
						else if(!prev_and)
						{
							free(lol);
							prev_and = true;	//case where previous condition was AND and that it failed the previous condition
							prev_or = false;
						}
						else if(run_execvp(lol, koo) == true)	//current statement is true
						{
							if(token != NULL)
								prev_or = true;		//sets to know that the previous condition under OR is true
						}
					}
					else if(condition == and_c)
					{
						if(!prev_and) free(lol);			//if previous condition was false; does not run exec
						else if(prev_or)
						{
							free(lol);
							prev_and = true;
							prev_or = false;
						}
						else if(run_execvp(lol, koo) == false)	//current statement is false
						{
							if(token != NULL)
								prev_and = false;
						}

					}
					else if(condition == semi_c)	
					{
						if(!prev_or && prev_and)
						{
							if(run_execvp(lol, koo));	//runs if previous or was false and if previous and was true
						}
						else free(lol);
						prev_or = false;	//resets conditions for passing exec
						prev_and = true;
					}
					else
						if(run_execvp(lol, koo));
					//free(lol);	//remember to delete char** at end of command to prevent memory leaks
					if(koo) free(lol);
				}
				string trip;
				size_t cat_size = 0;
				vector<string> redir_parse;
				bool delete_wolol = false;
				if(redirection){
					redir_parse = trip_wire(trip, lol, index_re, cat_size, redirection);
					if(!redirection) delete_wolol = true;
				}
				//int file_d[2];
				//if(-1 == pipe(file_d)) perror("pipe"), exit(1);
				while(redirection)
				{
					delete_wolol = true;
					//trip_wire(trip, lol, index_re, cat_size);
					redir_parse = trip_wire(trip, lol, index_re, cat_size, redirection);
					if(redir_parse.size() != 0 )trip = redir_parse.at(0);
					char** wolol = (char**)malloc(BUFSIZ);
					size_t exec_pos = index_re;
					//int counter = 0;
					int counters = 0;
					while(redirection)
					{
						size_t index_re_copy = index_re;
						string same;
						if(redir_parse.size() != 0)
							same = redir_parse.at(0);
						bool in_out = false;
						bool out_in = false;
						bool out_pipe = false;
						bool not_all_pipes = false;
						bool in_pipes = false;
						bool one_way = true;
						bool final_out = false;
						size_t k;
						vector<string> files;
						size_t num_pipes = 1;
						for(k = 1; k < redir_parse.size(); k++)
						{
							if(same != redir_parse.at(k))
							{
								if((same == in_re) && (out_re2 == redir_parse.at(k) || out_re == redir_parse.at(k)))
								{
									one_way = false;
									in_out = true;
									break;
								}
								else if((same == out_re || same == out_re2) && in_re == redir_parse.at(k))
								{
									one_way = false;
									out_in = true;
									break;
								}
								else if((same == out_re || same == out_re2) && pipes == redir_parse.at(k))
								{
									one_way = false;
									out_pipe = true;
									break;
								}
								else if(same == pipes)
								{
									one_way = false;
									not_all_pipes = true;
									break;
								}
								else if(same == in_re && pipes == redir_parse.at(k))
								{
									one_way = false;
									in_pipes = true;
									size_t ko;
									for( ko = k; redir_parse.at(ko) == pipes &&  ko < redir_parse.size() -1 ; ko++)
									{
										num_pipes++;
									}
									for(size_t bo = ko; (redir_parse.at(bo) == out_re || redir_parse.at(bo) == out_re2 || redir_parse.at(bo) == in_re) && bo < redir_parse.size() - 1; bo++)
									{
										final_out = true;
									}
									break;
								}
								else if(same == pipes && in_re == redir_parse.at(k))
								{
									break;
								}

								//out redirection into pipe, probably just an error, or dont do anything with output file
							}
							else if(same == pipes) num_pipes++;
						}
						if(in_pipes)
						{
							vector<string> oo;
							vector<string> boo;
							size_t v = 0;
							char** polol;
							size_t copy_copy;
							while((condition_re != pipes) && redirection)
							{
								//exec_pos = 0;
								copy_copy = index_re_copy;
								redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
								if(condition_re != pipes)
								{
									boo = grab_files_in(lol, index_re_copy);
									for(size_t pl = 0; pl < boo.size(); pl++)
									{
										//cout << boo.at(pl) << endl << endl;
										oo.push_back(boo.at(pl));
									}
								}
								else index_re_copy = copy_copy;
								//index_re = exec_pos;
								//cout << v << endl << *wolol << endl;
								if(v == 0) polol = wolol;
								//if(v == 1) dolol = wolol;
								v++;
							}
							int do_only_once = 1;
							int wait_once = 1;
							int also_once = 1;
							if(boo.size() == 0)
							{
								cerr << "No input file" << endl;
								redirection = false;
								break;
							}
							while((num_pipes > 1 || wait_once == 0) && boo.size() != 0)
							{
								int file_d [2];
								if(num_pipes == 0);
								else
								{
									if(-1 == pipe(file_d)) perror("pipe"), exit(1);
								}
								if(do_only_once != 1 && num_pipes != 1) pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
								//pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
								//run_pipe_before(wolol, file_d), counter++;
								if(do_only_once == 1)
								{
									run_in_pipe(polol, boo.at(boo.size()- 1), file_d);
									num_pipes--;
								}
								if(do_only_once != 1 && num_pipes != 1){
									if(also_once == 1) pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
									run_pipe_after(wolol, file_d);
									num_pipes--;
									also_once++;
								}
								if(num_pipes == 1)
								{
									//pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
									if(-1 == dup2(descrip_out, 1)) perror("dup"), exit(1); 
									if(!final_out)pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
									if(redirection == false)
										cerr << "Not valid pipe" << endl;
									
									if(!final_out)
									{
										if(also_once == 1) pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
										run_pipe_end(wolol);
									}
									if(wait_once == 0)
									{
										string condition_re_copy = condition_re;
										while(condition_re != out_re && condition_re != out_re2 && condition_re != in_re) 
										{
											pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
											if(condition_re != in_re) condition_re_copy = condition_re;
											else break;
										}
										//run_pipe_after(wolol, file_d),
										//pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
										polol = wolol;
										if(lol[index_re_copy -1] != NULL) files = grab_files_out(lol, index_re_copy);
									//	cerr << condition_re << endl;
										//if(files.size() == 0) cerr << "bird" << endl;
										if(condition_re == in_re) cerr << "Invalid input" << endl, redirection = false;
										else if((condition_re_copy == out_re || condition_re_copy == out_re2) && files.size() != 0) run_redir_pipe_out(condition_re_copy, polol, files, file_d);
										if(files.size() == 0){
											redirection = false;
											cerr << "No outfile" << endl;
											break;
										}
										if(condition_re == out_re || condition_re == out_re2)
										{

											if(lol[index_re_copy-1] != NULL) pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
											if(condition_re == in_re)
											{
												cerr << "Was put into output file, but invalid input after" << endl;
												redirection = false;
											}
										}
										index_re = index_re_copy;
										break;

									}
									if(final_out) wait_once = 0;
								}
								//else break;
								do_only_once = 0;
								index_re = index_re_copy;
							}
							//cout << "out ehere" << endl;
							break;
							//cout << redirection << endl;
							//redirection = false;

							
						}
						if(not_all_pipes)
						{
							char** polol;
							//cout << num_pipes << endl;
							while(redirection && num_pipes > 0)
							{
								int file_d [2];
								if(num_pipes == 0);
								else
								{
									if(-1 == pipe(file_d)) perror("pipe"), exit(1);
								}
								pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
								//run_pipe_before(wolol, file_d), counter++;
								if(num_pipes != 0)
								{
									run_pipe_after(wolol, file_d);
									num_pipes = num_pipes -1;
									//cout << num_pipes << endl;
								}
								if(num_pipes == 0)
								{
									//dup2(descrip_out, 1);
									pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
									polol = wolol;
									files = grab_files_out(lol, index_re_copy);
									if(files.size() == 0) cerr << "No outfile" << endl;
									else if(condition_re == out_re || condition_re == out_re2) run_redir_pipe_out(condition_re, polol, files, file_d);
								}
								index_re = index_re_copy;
							}
							index_re = index_re_copy;
							bool was_input = false;
							bool was_pipes = false;
							for(size_t r = k; r < redir_parse.size(); r++)
							{
								if(redir_parse.at(r) == in_re)
								{
									was_input = true;
									break;
								}
								if(redir_parse.at(r) == pipes)
								{
									was_pipes = true;
									break;
								}
							}
							if(was_input)
							{
								cerr << "Does not accept input after output" << endl;
								redirection = false;
							}
							else if(was_pipes)
							{
								cerr << "Does not pipe after output" << endl;
								redirection = false;
							}
							break;

						}
						else if(out_in || out_pipe)
						{
							vector<string> oo;
							vector<string> boo;
							while((condition_re != in_re) && redirection)
							{
								redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
								if(condition_re != in_re)
								{
									files = grab_files_in(lol, index_re_copy);
									run_redir_out(condition_re, wolol, files);
									index_re = index_re_copy;
								}
							}
							redirection = false;
							if(out_in) cerr << "Invalid input stream" << endl;
							if(out_pipe) cerr << "Invalid pipe" << endl;
							break;
						}
						else if(in_out)
						{
							vector<string> oo;
							vector<string> boo;
							size_t v = 0;
							char** polol;
							while((condition_re != out_re && condition_re != out_re2) && redirection)
							{
								//exec_pos = 0;
								redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
								if(condition_re != out_re && condition_re != out_re2)
								{
									boo = grab_files_in(lol, index_re_copy);
									for(size_t pl = 0; pl < boo.size(); pl++)
									{
										//cout << boo.at(pl) << endl << endl;
										oo.push_back(boo.at(pl));
									}
								}
								//index_re = exec_pos;
								//cout << v << endl << *wolol << endl;
								if(v == 0) polol = wolol;
								//if(v == 1) dolol = wolol;
								v++;
							}
							while((condition_re == out_re || condition_re == out_re2) && redirection)
							{
								files = grab_files_out(lol, index_re_copy);
								//oo.push_back(files.at(0));
								if(files.size() != 0 )
								{
									for(size_t kir = 0; kir < oo.size(); kir++)
									{
										//cout << oo.at(kir) << endl << endl;
										if(oo.at(kir) == "") cerr << "herp" << endl;
										else run_redir_in_out(oo.at(kir), polol, condition_re, files.at(0));
									}
								}
								redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
							}

						}
						else if(one_way)
						{

							//if(trip != pipes) redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
							//else if(trip == pipes) pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
							if(trip == out_re || trip == out_re2)
							{
								redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
								if(condition_re == out_re || condition_re == out_re2)
								{
									files = grab_files_out(lol, index_re_copy);
									if(files.size() == 0) cerr << "No output file" << endl, redirection = false;
									else run_redir_out(condition_re, wolol, files);
								//	if(files.size() == 0 && counters != 0) cerr << "No input" << endl;
									index_re = index_re_copy;
								}
								else break;
								counters++;
							}
							else if(trip == in_re)
							{
								redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
								
								if(condition_re == in_re && redirection)
								{
									files = grab_files_in(lol, index_re_copy);
									if(files.size() == 0)
									{
										cerr << "No input file" << endl;
										redirection = false;
										break;
									}
									else {
										for(size_t u = 0; u < files.size(); u++)
										run_redir_in(wolol, files.at(u));
									}
									//if(files.size() == 0 && counters != 0) cerr << "No input" << endl;
									index_re = index_re_copy;
								}
								//else break;
								else if(condition_re == out_re || condition_re == out_re2)
								{
									//close(1);
									files = grab_files_out(lol, index_re_copy);
									for(size_t u = 0; u < files.size(); u++);
										//run_redir_out_in(condition_re, files.at(u), ID);
									index_re = index_re_copy;
									//break;
								}
								else break;
								counters++;
								//close(id);
							}
							else if(trip == pipes)
							{
								if(redir_parse.size() == 0)
								{
									redirectioner(condition_re, wolol, lol, redirection, index_re_copy, exec_pos, cat_size);
									if(redirection) run_lonely_pipe(wolol);

								}
								while(redirection && num_pipes > 1)
								{
									int file_d [2];
									if(num_pipes == 1);
									else
									{
										if(-1 == pipe(file_d)) perror("pipe"), exit(1);
									}
									pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
									if(*wolol == NULL){
										redirection = false;
										cerr << "No valid pipe" << endl;
										break;
									}
									//run_pipe_before(wolol, file_d), counter++;
									if(num_pipes != 1) run_pipe_after(wolol, file_d); num_pipes--;
									if(num_pipes == 1){
										if(-1 == dup2(descrip_out, 1)) perror("dup"), exit(1);
										pipe_er(condition_re, wolol, lol, redirection, index_re_copy);
										if(*wolol == NULL) cerr << "No other end" << endl;
										else run_pipe_end(wolol);
									}
									//else break;
									index_re = index_re_copy;
								}
							}
							//else if(condition == in_re);
						}
						
					}
					//cout << "111" << endl;

					free(wolol);
					if(-1 == dup2(descrip_in, 0)) perror("dup"), exit(1);
					if(-1 == dup2(descrip_out, 1)) perror("dup"), exit(1);
				}
				if(delete_wolol) free(lol);
			}
			break;
		}
	}
	return 0;
}

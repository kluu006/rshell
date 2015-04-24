#include <iostream>
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

enum what_flags{nothing, flag_a, flag_l, flag_R} flag_check;

char** give_path(int pos_parameter, size_t argc, char** argv)
{
	char** lol = (char**)malloc(BUFSIZ);
	size_t i, j;
	//cout << "hello" << endl;
	for(i = pos_parameter, j = 0; i < argc; i++, j++)
		lol[j] = argv[i];
	lol[j] = NULL;
	return lol;
}

int binary_flag(int& param, size_t num_param, char** user_input)
{
	char flag_marker = '-';
	int flags = 0;	//will be returned, number of flags
	size_t i;
	bool is_a = false, is_l = false, is_R = false;
	for(i = 1; i < num_param; i++)
	{
		if(user_input[i][0] != flag_marker)
		{
			param = i;	//if there was no flag marker, program will only preform before this break
			break;		//this is also to see if there is any file after this point
		}
		else
		{	for(size_t b = 1; user_input[i][b] != '\0'; b++)
			{
				char this_flag = user_input[i][b];
				switch(flag_check)
				{
					case nothing:
						if(this_flag == 'a' && !is_a)
							flag_check = flag_a;
						else if(this_flag == 'l' && !is_l)
							flag_check = flag_l;
						else if(this_flag == 'R' && !is_R)
							flag_check = flag_R;
						break;
					case flag_a:
						if(this_flag == 'a')
							flag_check = nothing;
						else if(this_flag == 'l' && !is_l)
							flag_check = flag_l;
						else if(this_flag == 'R' && !is_R)
							flag_check = flag_R;
						else
							flag_check = nothing;
						break;
					case flag_l:
						if(this_flag == 'a' && !is_a)
							flag_check = flag_a;
						else if(this_flag == 'l')
							flag_check = nothing;
						else if(this_flag == 'R' && !is_R)
							flag_check = flag_R;
						else
							flag_check = nothing;
						break;
					case flag_R:
						if(this_flag == 'a' && !is_a)
							flag_check = flag_a;
						else if(this_flag == 'l' && !is_l)
							flag_check = flag_l;
						else if(this_flag == 'R')
							flag_check = nothing;
						else
							flag_check = nothing;
						break;
					default:
						flag_check = nothing;
						break;
				}
				switch(flag_check)
				{
					case flag_a:
						is_a = true;
						flags++;
						break;
					case flag_l:
						is_l = true;
						flags += 5;
						break;
					case flag_R:
						is_R = true;
						flags += 10;
						break;
					default:
						break;
				}
			}
		}
		param++;//says how many valid parameters there are in argv 
	}
	return flags;
}

bool case_insensitive(const char* left, const char* right)
{
	string leftside = left;
	string rightside = right;
	string::const_iterator l_start = leftside.begin(),
	l_end = leftside.end(),
	r_start = rightside.begin(),
	r_end = rightside.end();
	for( ;r_end != r_start && l_end != l_start; r_start++, l_start++)
	{
		const char right_char = tolower(*r_start);
		const char left_char = tolower(*l_start);
		if (left_char < right_char) return true;
		if (left_char > right_char) return false;
	}
	if(l_start != l_end)
		return false;
	if(r_start != r_end)
		return true;
	return true;
}
vector<char* > files_inside(const char* directory)
{
	vector<char* > my_files;
	DIR *my_directory = opendir(directory);
	dirent *this_dir;
	if(my_directory != NULL)
	{
		while((this_dir = readdir(my_directory)))
		{
			if(this_dir == NULL)
			{
				perror("read directory");
				exit(1);
			}
			char* bar = (char*)malloc(strlen(this_dir -> d_name) +1);
			bar = this_dir->d_name;
			my_files.push_back(bar);
		}
	}
	else
	{
		perror("open directory");
		exit(1);
	}
	if(my_files.size() != 0)
		sort(my_files.begin(), my_files.end(),case_insensitive);
	return my_files;
}

void run_a(bool show_p, const char* directory)
{
	vector<char*> hello = files_inside(directory);
	size_t u;
	for(u = 0; u < hello.size(); u++)
		cout << hello.at(u) << endl;
}

int main(int argc, char* argv[])
{
	int parameter = 1; //start at first flag
	int flags_bin;
	flags_bin = binary_flag(parameter, argc, argv);	//gives what combo to do
	cout << flags_bin << endl << parameter << endl;
	cout << argc << endl;
	char** womp;
	womp = give_path(parameter, argc, argv);
	//for(int i = 0; *womp !=NULL && parameter < argc && i < 3; i++)
	//	cout << *womp << endl;
	int number_files = argc - parameter;
	int stop = 0;
	const char* file_path;
	do
	{
		if(womp[0] == NULL)
		{
			file_path = ".";
		}
		if(flags_bin == 0)//run ls
			run_a(false, file_path);
		else if(flags_bin == 1);// run ls -a
		else if(flags_bin == 5);// run ls -l
		else if(flags_bin == 10);// run ls -R
		else if(flags_bin == 6); // run ls -al or -la
		else if(flags_bin == 11);// run ls -aR or -Ra
		else if(flags_bin == 15);// run ls -lR or -Rl
		else if(flags_bin == 16);// run ls -alR or -lRa or -aRl or -laR or -Ral


		stop++;
	}while(number_files > stop);
	delete womp;
	return 0;














}

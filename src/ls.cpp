#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

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



int main(int argc, char* argv[])
{
	int parameter = 1; //start at first flag
	int flags_bin;
	flags_bin = binary_flag(parameter, argc, argv);	//gives what combo to do
	cout << flags_bin << endl << parameter << endl;
	cout << argc << endl;
	char** womp;
	womp = give_path(parameter, argc, argv);
	for(int i = 0; *womp !=NULL && parameter < argc && i < 3; i++)
		cout << *womp << endl;
	delete womp;
	return 0;














}

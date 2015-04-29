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

using namespace std;

char blue[] = {"\033[1;34m"};
char white[] = {"\033[0;00m"};
char green[] = {"\033[1;32m"};
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

bool case_insensitive(const string left, const string right)
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
vector<string > files_inside(const char* directory)
{
	vector<string> my_files;
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
			string bar = this_dir -> d_name;
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
	if(closedir(my_directory) == -1)
	{
		perror("close directory");
		exit(1);
	}
	return my_files;
}
bool check_if_dot(const char* file)
{
	if(file[0] == '.')
		return true;
	return false;
}

bool dont_do(string file)
{
	string wut = file;
	if(wut == "." || wut == "..")
		return true;
	return false;
}
int set_width(const vector<string> my_files)
{
	size_t s;
	size_t width = 0;
	for(s = 0; s < my_files.size(); s++)
	{
		if(my_files.at(s).size() > width)
			width = my_files.at(s).size();
	}
	return width;
}
void run_a(bool show_p, const char* directory)
{
	vector<string> hello = files_inside(directory);
	size_t u;
	struct stat s;
	//int width = set_width(hello);
	cout << directory << ":" << endl;
	for(u = 0; u < hello.size(); u++)
	{
		//char* the_path = (char*)malloc(BUFSIZ);
		bool checker = check_if_dot(hello.at(u).c_str());
		string the_path = (string)directory + '/' + hello.at(u);
		//strcpy(the_path, directory);
		//strcat(the_path, "/");
		//strcat(the_path, hello.at(u));
		if(stat(the_path.c_str(), &s) == -1)
		{
			perror("stat");
			exit(1);
		}
		if(S_ISDIR(s.st_mode))
		{
			if(!show_p && checker);
			else
				cout << blue << basename(the_path.c_str()) << white << "  ";
		}
		else if(S_IXUSR & s.st_mode)
		{
			cout << green << basename(the_path.c_str()) << white << "  ";
		}
		else
			if(!show_p && checker);
			else
				cout << white << basename(the_path.c_str()) << "  ";
	}
	cout << "\n";
	
}

void output_l(const struct stat s)
{
	if(S_ISLNK(s.st_mode)) cout << "s";
	else if(S_ISDIR(s.st_mode)) cout << "d";
	else cout << "-";
	cout << ((S_IRUSR & s.st_mode) ? "r" : "-");
	cout << ((S_IWUSR & s.st_mode) ? "w" : "-");
	cout << ((S_IXUSR & s.st_mode) ? "x" : "-");
	cout << ((S_IRGRP & s.st_mode) ? "r" : "-");
	cout << ((S_IWGRP & s.st_mode) ? "w" : "-");
	cout << ((S_IXGRP & s.st_mode) ? "x" : "-");
	cout << ((S_IROTH & s.st_mode) ? "r" : "-");
	cout << ((S_IWOTH & s.st_mode) ? "w" : "-");
	cout << ((S_IXOTH & s.st_mode) ? "x" : "-");
}

void run_l(bool show_p, const char* directory)
{
	vector<string> hello = files_inside(directory);
	size_t u;
	struct stat s;
	struct passwd* local;
	struct group* group;
	char* owner_name;
	char* group_name;
	string mod;
	cout << directory << ":" << endl;
	for(u = 0; u < hello.size(); u++)
	{
		//char* the_path = (char*)malloc(BUFSIZ);
		bool checker = check_if_dot(hello.at(u).c_str());
		string the_path = (string)directory + '/' + hello.at(u);
		//strcpy(the_path, directory);
		//strcat(the_path, "/");
		//strcat(the_path, hello.at(u));
		if(stat(the_path.c_str(), &s) == -1)
		{
			perror("stat");
			exit(1);
		}
		local = getpwuid(s.st_uid);
		if(local == NULL)
		{
			perror("getpwuid");
			exit(1);
		}
		group = getgrgid(s.st_gid);
		if(group == NULL)
		{
			perror("getgrgid");
			exit(1);
		}
		owner_name = local->pw_name;
		group_name = group->gr_name;
		mod = ctime(&s.st_mtime);
		mod = mod.substr(4,12);
		if(!show_p && checker);
		else
		{
			output_l(s);
			cout << " " << setw(2) << right << s.st_nlink << " " << owner_name << " " << group_name << " " << setw(6) << right << s.st_size << " " << mod << " ";
			if(S_ISDIR(s.st_mode))
				cout << blue << basename(the_path.c_str()) << white << "  ";
			else if(S_IXUSR & s.st_mode)
				cout << green << basename(the_path.c_str()) << white << "  ";
			else
				cout << white << basename(the_path.c_str()) << "  ";
			cout << endl;
		}
	}
}

void run_R(bool show_p, const char* directory, bool show_l)
{
	vector<string> recur_dir;
	vector<string> hello = files_inside(directory);
	size_t u;
	struct stat s;
	struct passwd* local;
	struct group* group;
	char* owner_name;
	char* group_name;
	string mod;
	cout << directory << ":" << endl;
	for(u = 0; u < hello.size(); u++)
	{
		//char* the_path = (char*)malloc(BUFSIZ);
		bool checker = check_if_dot(hello.at(u).c_str());
		bool dont = dont_do(hello.at(u));
		string the_path = (string)directory + '/' + hello.at(u);
		//strcpy(the_path, directory);
		//strcat(the_path, "/");
		//strcat(the_path, hello.at(u));
		if(stat(the_path.c_str(), &s) == -1)
		{
			perror("stat");
			exit(1);
		}
		local = getpwuid(s.st_uid);
		if(local == NULL)
		{
			perror("getpwuid");
			exit(1);
		}
		group = getgrgid(s.st_gid);
		if(group == NULL)
		{
			perror("getgrgid");
			exit(1);
		}
		owner_name = local->pw_name;
		group_name = group->gr_name;
		mod = ctime(&s.st_mtime);
		mod = mod.substr(4,12);
		if(!show_p && checker);
		else
		{
			if(show_l)
			{
				output_l(s);
				cout << " " << setw(2) << right << s.st_nlink << " " << owner_name << " " << group_name << " " << setw(6) << right << s.st_size << " " << mod << " ";
			}
			if(S_ISDIR(s.st_mode))
			{
				if(dont);
				else
				{
					recur_dir.push_back(the_path);
				}
				cout << blue << basename(the_path.c_str()) << white << "  ";
			}
			else if(S_IXUSR & s.st_mode)
				cout << green << basename(the_path.c_str()) << white << "  ";
			else
				cout << white << basename(the_path.c_str()) << "  ";
			if(show_l)
				cout << endl;
		}
	}
	size_t q;
	cout << "\n\n";
	for(q = 0; q < recur_dir.size(); q++)
	{
		run_R(show_p, recur_dir.at(q).c_str(),show_l);

	}
}

int main(int argc, char* argv[])
{
	int parameter = 1; //start at first flag
	int flags_bin;
	flags_bin = binary_flag(parameter, argc, argv);	//gives what combo to do
	//cout << flags_bin << endl << parameter << endl;
	//cout << argc << endl;
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
			file_path = ".";
		else
			file_path = womp[stop];
		if(flags_bin == 0)//run ls
			run_a(false, file_path);
		else if(flags_bin == 1)// run ls -a
			run_a(true, file_path);
		else if(flags_bin == 5)// run ls -l
			run_l(false, file_path);
		else if(flags_bin == 10)// run ls -R
			run_R(false, file_path, false);
		else if(flags_bin == 6) // run ls -al or -la
			run_l(true, file_path);
		else if(flags_bin == 11)// run ls -aR or -Ra
			run_R(true, file_path, false);
		else if(flags_bin == 15)// run ls -lR or -Rl
			run_R(false, file_path, true);
		else if(flags_bin == 16)// run ls -alR or -lRa or -aRl or -laR or -Ral
			run_R(true, file_path, true);
		stop++;
	}while(number_files > stop);
	free(womp);
	return 0;
}

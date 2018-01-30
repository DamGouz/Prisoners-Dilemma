#include "main.h"
#include <experimental/filesystem>
#include <sstream>
#include "Gang.h"
#include "Interpreter.h"


namespace fs = std::experimental::filesystem;
int read_files(string * files);
void generator(int times, int type);
int spy_test_tournament(int num_strategies, string * files, string * combos, int spy_poss);



//Generates the different gang combinations
void gang_combos(string * combos, int num_combos) {
	cout << endl;
	bool once[10] = { false };
	int t;
	for (int i = 0; i < num_combos; i++) {
		int f_rem = std::count(once, once+10, false);
		if ((f_rem > 5 && i == num_combos - 2)|| (f_rem <= 5 && i == num_combos - 1)) {
			if (f_rem > 5) {
				f_rem = f_rem - 5;
			}
			for (int j = 0; j < f_rem; j++) {
				t = rand() % 10;
				while (once[t]==true)
					t = rand() % 10;
				once[t] = true;
				combos[i] = combos[i] + to_string(t);
			}
			for (int j = f_rem; j < 5; j++) {
				t = rand() % 10;
				once[t] = true;
				combos[i] = combos[i] + to_string(t);
			}
		}
		else {
			for (int j = 0; j < 5; j++) {
				t = rand() % 10;
				once[t] = true;
				combos[i] = combos[i] + to_string(t);
			}
		}
	}
}


// Sorts the files by win first, then by ties, and then by years
void sort_wins(int num_strategies, int * times_fought,  int * wins, float * total_years, int * ties, string * files) {

	for (int c = 0; c < (num_strategies - 1); c++)
	{
		for (int d = 0; d < num_strategies - c - 1; d++)
		{
			if (wins[d] < wins[d + 1] || (wins[d] == wins[d + 1] && ties[d]<ties[d + 1])|| (wins[d] == wins[d + 1] && ties[d]==ties[d + 1])&&total_years[d]>total_years[d+1])
			{
				swap(wins[d], wins[d + 1]);
				swap(files[d], files[d + 1]);
				swap(times_fought[d], times_fought[d + 1]);
				swap(ties[d], ties[d + 1]);
				swap(total_years[d], total_years[d + 1]);
			}
		}
	}
}




/* 
Calculates the amount of strategies we need for a thread in order for them to be equal.
It is needed beacuse for n strategies, the amount of loops is Sum(1+2+3+...+(n-1)+n) = (n+1)*(n/2).
So I calculated the following mathematical type.
*/
int calc_proc(int max, int cur_thread, int num_threads) { 
	double n = -0.5 + sqrt(1 + 4 * (max*max + max)*cur_thread / num_threads) / 2;
	return max - int(n);
}




// Checks if there are less files in the folder than those selected
void files_alloc(int num_strategies, string * files, int type) { 
	int size = read_files(files);
	if (size<num_strategies) {
		cout << "There are only " << size << " files available. Generating the rest." << endl;
		generator(num_strategies - size, type);
	}
	size = read_files(files);
}

//Reads all files in directory and adds them to the files array
int read_files(string * files) { 
	string path = "Strategies/";
	int i = 0;
	try {
		for (auto & p : fs::directory_iterator(path)) {
			std::stringstream filename;
			filename << p << std::endl;
			string input = filename.str();
			input.erase(input.end() - 1, input.end());
			if (input.find("Results.txt") == std::string::npos) {
				files[i] = input;
				i++;
			}


		}
	}
	catch (...) {
		cout << "Error opening files" << endl;
	}
	return i;
}

// Deletes everything from the Strategies directory
void deleteall() { 
	try {
		fs::path dir = fs::current_path();
		std::uintmax_t n = fs::remove_all(dir / "Strategies");
		fs::create_directories(dir / "Strategies/");
	}
	catch(...){
		cout << "Exception caught: Folder Strategies, or a file in it is already open in the explorer" << endl;
	}
}

// Prints to the screen and in the result file the statistics of the gang strategy tournament
void print_results_gang(int num_strategies, int * times_fought, int * wins, float * total_years, int * ties, string * files, string * combos) {
	sort_wins(num_strategies, times_fought, wins, total_years, ties, combos);
	int result_compl = -1;
	while (result_compl < 0 || result_compl>2) {
		cout << "Choose which statistics you want to show: "<<endl;
		cout << "0. Wins" << endl;
		cout << "1. Wins, Ties" << endl;
		cout << "2. Wins, Ties, Average Years" << endl;
		cin >> result_compl;
	}
	std::ofstream outfile("Strategies/Results.txt", std::ios_base::app);
	cout << "The best gang combination is " << (combos)[0] << endl << endl;
	outfile << "The best gang combination is " << (combos)[0] << endl << endl;
	for (int i = 0; i < num_strategies; i++) {
		ostringstream output;
		if (!times_fought[i]) {
			output << "Combination " << combos[i] << " could not be run." << endl;
		}
		else {
			if (result_compl==0)
				output << "Combination " << combos[i] << " won " << wins[i] << "/" << times_fought[i] << " times." << endl;
			else if (result_compl == 1)
				output << "Combination " << combos[i] << " won " << wins[i] << "/" << times_fought[i] << " times and tied " << ties[i] << "/" << times_fought[i] << " times." << endl;
			else
				output << "Combination " << combos[i] << " won " << wins[i] << "/" << times_fought[i] << " times and tied " << ties[i] << "/" << times_fought[i] << " times, with an average of " << total_years[i] / times_fought[i] << " years spent in prison." << endl;
		}
		cout << output.str();
		outfile << output.str();
	}
	cout << endl << "The results are saved at Strategies/Results.txt" << endl << endl;
	cout << endl << endl << "Now follows a test of spies for the winning combination: " << combos[0] << "." << endl << endl;
	outfile << endl << endl << "Now follows a test of spies for the winning combination: " << combos[0] << "." << endl << endl;
	int better_choice = 0;
	int spy_poss = 0;
	outfile.close();
	for (int i = 0; i < 25; i = i + 5) {
		better_choice += spy_test_tournament(num_strategies, files, combos, i);
	}
	std::ofstream outfile_n("Strategies/Results.txt", std::ios_base::app);
	if (better_choice > 2) {
		cout << endl << "The above results show that it is more beneficial for the leader to change his choice." << endl;
		outfile_n << endl << "The above results show that it is more beneficial for the leader to change his choice." << endl;
	}
	else if (better_choice < 2) {
		cout << endl << "The above results show that it is more beneficial for the leader not to change his choice." << endl;
		outfile_n << endl << "The above results show that it is more beneficial for the leader not to change his choice." << endl;
	}
	else {
		cout << endl << "The above results are inconclusive" << endl;
		outfile_n << endl << "The above results are inconclusive" << endl;
	}
	outfile_n.close(); 
}

// Prints to the screen and in the result file the statistics of the single strategy tournament
void print_results_single(int num_strategies, int * times_fought, int * wins, float * total_years, int * ties, string * files) {
	sort_wins(num_strategies, times_fought, wins, total_years, ties, files);
	std::ofstream outfile("Strategies/Results.txt", std::ios_base::app);
	cout << "The best Strategy is " << (files)[0] << endl;
	outfile << "The best Strategy is " << (files)[0] << endl;
	for (int i = 0; i < num_strategies; i++) {
		ostringstream output;
		(files)[i].erase(0, 11);
		(files)[i].erase((files)[i].end() - 4, (files)[i].end());
		if (!times_fought[i]) {
			output << "Strategy " << files[i] << " could not be run." << endl;
		}
		else {
			output << "Strategy " << files[i] << " won " << wins[i] << "/" << times_fought[i] << " times and tied " << ties[i] << "/" << times_fought[i] << " times, with an average of " << total_years[i] / times_fought[i] << " years spent in prison." << endl;
		}
		cout << output.str();
		outfile << output.str();
	}
	cout << endl << "The results are saved at Strategies/Results.txt" << endl << endl;

}

// Generates files based on how many files we want, and whether we want to use single strategies or gangs!
void generator(int times, int type) { 
	int size_num;
	int size_char;
	string * vars_num;
	string * vars_char;
	if (type == 0) {
		size_num = 7;
		size_char = 4;
		vars_num = new string[6];
		vars_num[0] = "ALLOUTCOMES_W";
		vars_num[1] = "ALLOUTCOMES_X";
		vars_num[2] = "ALLOUTCOMES_Y";
		vars_num[3] = "ALLOUTCOMES_Z";
		vars_num[4] = "ITERATIONS";
		vars_num[5] = "MYSCORE";
		vars_char = new string[4];
		vars_char[0] = "W";
		vars_char[1] = "X";
		vars_char[2] = "Y";
		vars_char[3] = "Z";
	}
	else {
		size_num = 10;
		size_char = 7;
		vars_num = new string[9];
		vars_num[0] = "ALLOUTCOMES_A";
		vars_num[1] = "ALLOUTCOMES_B";
		vars_num[2] = "ALLOUTCOMES_C";
		vars_num[3] = "ALLOUTCOMES_W";
		vars_num[4] = "ALLOUTCOMES_X";
		vars_num[5] = "ALLOUTCOMES_Y";
		vars_num[6] = "ALLOUTCOMES_Z";
		vars_num[7] = "ITERATIONS";
		vars_num[8] = "MYSCORE";
		vars_char = new string[7];
		vars_char[0] = "A";
		vars_char[1] = "B";
		vars_char[2] = "C";
		vars_char[3] = "W";
		vars_char[4] = "X";
		vars_char[5] = "Y";
		vars_char[6] = "Z";
	}
	for (int k = 0; k < times; k++) {

		string file_loc;
		file_loc = "Strategies/" + to_string(k) + ".txt";
		std::fstream  testfile(file_loc);
		while (testfile.good()==1) {
			testfile.close();
			file_loc = "Strategies/" + to_string(rand() % 5000) + ".txt";
			fstream testfile(file_loc);
		}
		testfile.close();
		ofstream newfile(file_loc);
		
		
		
		int rand_lines = rand() % 21 + 1;
		for (int i = 0; i < rand_lines; i++) {
			stringstream line;
			line << i;
			int poss;
			if (i == rand_lines-1 || rand_lines==1) {
				poss = rand() % 3 + 1;
				switch (poss) {
				case 1:
					line << " SILENCE";
					break;
				case 2:
					line << " BETRAY";
					break;
				case 3:
					line << " RANDOM";
					break;
				}
			}
			else {
				if (i == 0) {
					poss = rand() % 5;
					if (poss == 1) {
						poss = rand() % 3 + 1;
					}
					else {
						poss = 0;
					}
				}
				else {
					poss = rand() % 4;
				}
				if (rand_lines == 1) {
					poss = rand() % 3 + 1;
				}
				switch (poss) {
				case 1:
					line << " SILENCE";
					break;
				case 2:
					line << " BETRAY";
					break;
				case 3:
					line << " RANDOM";
					break;
				case 0:
					line << " IF ";
					int rand_var = rand() % 2;
					if (rand_var == 0) {
						int rand_multi = rand() % size_num;
						if (rand_multi != size_num - 1) {
							line << vars_num[rand_multi];
						}
						else {
							line << rand() % 200;
						}
						int j = 2;
						while ((rand() % j) == 0) {
							int rand_oper = rand() % 2;
							if (rand_oper)
								line << '+';
							else
								line << '-';
							int rand_multi = rand() % size_num;
							if (rand_multi != size_num - 1) {
								line << vars_num[rand_multi];
							}
							else {
								line << rand() % 200;
							}
							j++;
						}
						int rand_comp = rand() % 3;
						switch (rand_comp) {
						case 0:
							line << '<';
							break;
						case 1:
							line << '>';
							break;
						case 2:
							line << '=';
							break;
						}
						rand_multi = rand() % size_num;
						if (rand_multi != size_num - 1) {
							line << vars_num[rand_multi];
						}
						else {
							line << rand() % 200;
						}
						j = 2;

						while ((rand() % j) == 0) {
							int rand_oper = rand() % 2;
							if (rand_oper)
								line << '+';
							else
								line << '-';
							int rand_multi = rand() % size_num;
							if (rand_multi != size_num - 1) {
								line << vars_num[rand_multi];
							}
							else {
								line << rand() % 200;
							}
							j++;
						}
						line << " GOTO ";
						int rand_goto = rand() % (rand_lines - i) + i;
						while (rand_goto == i)
							rand_goto = rand() % (rand_lines - i) + i;
						line << rand_goto;
						break;
					}
					else {

						int rand_multi = rand() % 2;
						switch (rand_multi) {
						case 0:
							line << "LASTOUTCOME";
							break;
						case 1:
							line << vars_char[rand() % size_char];
							break;
						}
						line << '=';
						rand_multi = rand() % 2;
						switch (rand_multi) {
						case 0:
							line << "LASTOUTCOME";
							break;
						case 1:
							line << vars_char[rand() % size_char];
							break;
						}
						line << " GOTO ";

						int rand_goto = rand() % (rand_lines - i) + i;
						while (rand_goto == i)
							rand_goto = rand() % (rand_lines - i) + i;
						line << rand_goto;
						break;

					}
					break;
				}

			}
			newfile << line.str() << endl;
			
		}
		newfile.close();
	}
	delete [] vars_num, vars_char;
}

// Similar to the Tournament class, it is a simplified test of 1 Gang strategy vs All Gang strategies, given a spy possiblity.
int spy_test_tournament(int num_strategies, string * files, string * combos, int spy_poss) {
	int result = 0;
	Interpreter * intr = new Interpreter();
	string file_names[5];
	for (int k = 0; k < 5; k++) {
		file_names[k] = files[combos[0][k] - 48];
	}
	for (int i = 0; i < 2; i++) {
		int times_fought = 0;
		int wins = 0;
		float total_years = 0;
		int ties = 0;
		Gang * g1;
		if (i==0)
			g1 = new Gang(file_names, spy_poss, false);
		else
			g1 = new Gang(file_names, spy_poss, true);
		if (g1->check_success()) {
			for (int j = 0; j < num_strategies; j++) {
				string file_names2[5];
				for (int k = 0; k < 5; k++) {
					file_names2[k] = files[combos[j][k] - 48];
				}
				Gang * g2 = new Gang(file_names2, 0, false);
				if (g2->check_success()) {
					if (intr->against(g1, g2, 200)) {
						times_fought ++;
						if (i != j) {
						}
						total_years+= g1->myscore;
						if (g1->myscore < g2->myscore) {
							wins++;
						}
						else if (g2->myscore < g1->myscore) {

						}
						else if (g2->myscore == g1->myscore && i != j) {
							ties++;
						}
						else {
							ties++;
						}
					}
				}
				g1->init();
				delete g2;

			}
		}
		std::ofstream outfile("Strategies/Results.txt", std::ios_base::app);
		if (spy_poss != 0 || i == 0) {
			cout << "Combination " << combos[0] << " won " << wins << "/" << times_fought << " times and tied " << ties << "/" << times_fought << " times, with an average of " << total_years / times_fought << " years spent in prison, with a spy possibility of " << spy_poss << "%";
			outfile << "Combination " << combos[0] << " won " << wins << "/" << times_fought << " times and tied " << ties << "/" << times_fought << " times, with an average of " << total_years / times_fought << " years spent in prison, with a spy possibility of " << spy_poss << "%";
		}
		if (spy_poss != 0) {
			if (i == 0) {
				cout << ", when the leader didn't change choices." << endl;
				outfile << ", when the leader didn't change choices." << endl;
			}
			else {
				cout << ", when the leader changed choices." << endl;
				outfile << ", when the leader changed choices." << endl;
				result++;
			}
		}
		else {
			if (i == 0) {
				cout << "." << endl;
				outfile << "." << endl;
			}
		}
		outfile.close();
		delete g1;
	}
	return result;
	delete intr;
}
#include "Tournament.h"
#include <thread>


void files_alloc(int num_strategies, string * files, int type);
void deleteall();
void generator(int times, int type);
void print_results_single(int num_strategies, int * times_fought, int * wins, float * total_years, int * ties, string * files);
void print_results_gang(int num_strategies, int * times_fought, int * wins, float * total_years, int * ties, string * files, string * combos);
int spy_test_tournament(int num_strategies, string * files, string * combos, int spy_poss);
int calc_proc(int max, int cur_thread, int num_threads);
int read_files(string * files);
void gang_combos(string * combos, int num_combos);

void prompt_complexity(int & complexity) {
	cout << "Hello and welcome to the Prisoner's Dilemma simulation!" << endl << endl;
	while (complexity != 0 && complexity != 1) {
		cout << "Please choose complexity:" << endl << "0. Single Strategy" << endl << "1. Gang Strategy (10 Strategies)" << endl;
		cin >> complexity;
	}
}



void write_to_file(int times, int complexity, bool change_choice, int num_threads, int spy_poss, string * files) {
	std::ofstream outfile("Strategies/Results.txt");
	outfile << "The following tournament has " << times;
	if (complexity == 0)
		outfile << " strategies. " << endl << "It will be run on " << num_threads << " threads." << endl << endl;
	else {
		outfile << " strategy combinations, with a spy possibility of " << spy_poss << "%, ";
		if (change_choice == true)
			outfile << "while the leader will always change his choice.";
		else
			outfile << "while the leader will never change his choice.";
		outfile << endl << "It will be run on " << num_threads << " threads." << endl << endl;
		cout << endl;

		for (int i = 0; i < 10; i++) {
			cout << "File " << i << " is " << files[i] << endl;
			outfile << "File " << i << " is " << files[i] << endl;
		}
		cout << endl;
		outfile << endl;
	}


	outfile.close();
}




int main() {
	srand((unsigned)time(NULL));
	int complexity = 2;
	int num_combos = 0;
	int spy_poss = -1;
	bool change_choice = true;
	string * combos;
	prompt_complexity(complexity);
	int times = -1;
	int decide_gen = 2;
	if (complexity == 0) {
		while (times <= 0) {
			cout << "How many files do you want to have in the tournament?" << endl;
			cin >> times;
		}
		combos = new string[0];
	}
	else {
		times = 10;
		cout << "How many different gang strategy combinations do you want to have in the tournament?" << endl;
		cin >> num_combos;
		combos = new string[num_combos];
		for (int i = 0; i < num_combos; i++) {
			combos[i] = "";
		}
		gang_combos(combos, num_combos);
		string cc = "o";
		while (spy_poss != 0 && spy_poss != 5 && spy_poss != 10 && spy_poss != 15 && spy_poss != 20) {
			cout << "Please enter the possibility of having a spy (0,5,10,15,20)%" << endl;
			cin >> spy_poss;
		}
		if (spy_poss != 0) {
			while (cc != "y" && cc != "n") {
				cout << "Shall the leader change his choice given the option during the Monty Hall scenario? (y/n)" << endl;
				cin >> cc;
			}
		}
		if (cc == "y")
			change_choice = true;
		else
			change_choice = false;

	}
	string * files = new string[times];
	while (decide_gen != 0 && decide_gen != 1) {
		cout << "Please choose generation method:" << endl << "0. Generate files" << endl << "1. Use existing files" << endl;
		cin >> decide_gen;
	}
	if (decide_gen == 0) {
		deleteall();
		generator(times, complexity);
		times = read_files(files);
	}
	else {
		files_alloc(times, files, complexity);
	}
	
	
	int num_threads = 0;
	while (num_threads <= 0) {
		cout << "How many threads would you like to utilise?" << endl;
		cin >> num_threads;
	}
	if (complexity == 1)
		times = num_combos;
	Tournament * tourney = new Tournament(times);

	int * times_fought = new int[times];
	int * wins = new int[times];
	float * total_years = new float[times];
	int * ties = new int[times];
	for (int i = 0; i < times; i++) {
		times_fought[i] = 0;
		wins[i] = 0;
		total_years[i] = 0;
		ties[i] = 0;
	}

	write_to_file(times, complexity, change_choice, num_threads, spy_poss, files);

	//clock_t start = clock();
	std::thread * myThreads = new std::thread[num_threads];
	if (num_threads > 1) {
		int prev = 0;
		for (int i = 1; i <= num_threads; i++) {
			int next = calc_proc(times, num_threads - i, num_threads);
			if (complexity == 1)
				myThreads[i - 1] = std::thread(&Tournament::begin_gangs, Tournament(times), prev, next, times_fought, wins, total_years, ties, files, combos, spy_poss, change_choice);
			else
				myThreads[i - 1] = std::thread(&Tournament::begin_single, Tournament(times), prev, next, times_fought, wins, total_years, ties, files);
			prev = next;
		}
		for (int i = 1; i < num_threads; i++) {
			myThreads[i - 1].join();
		}
		
		
	}
	else {

		int startx = 0;
		if (complexity == 0)
			tourney->begin_single(startx, times, times_fought, wins, total_years, ties, files);
		else
			tourney->begin_gangs(startx, times, times_fought, wins, total_years, ties, files, combos, spy_poss, change_choice);
	}
	
	if (complexity == 0)
		print_results_single(times, times_fought, wins, total_years, ties, files);
	else {
		print_results_gang(times, times_fought, wins, total_years, ties, files, combos);
		
	}
	//cout << ((clock() - start) / (double)CLOCKS_PER_SEC) << endl;
	
	
	delete[]  times_fought, wins, total_years, ties, files, combos;
	delete tourney;

	system("pause");
	return 0;
}
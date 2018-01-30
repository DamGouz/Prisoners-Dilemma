#include "Tournament.h"
#include "Interpreter.h"
#include "Strategy.h"
#include "Gang.h"
std::mutex t_update_mutex;

// Constructor. Initializes the temporary arrays along with the interpreter
Tournament::Tournament(int times): num_strategies(times) {
	times_fought = new int[times];
	wins = new int[times];
	total_years = new float[times];
	ties = new int[times];
	for (int i = 0; i < times; i++) {
		times_fought[i] = 0;
		wins[i] = 0;
		total_years[i] = 0;
		ties[i] = 0;
	}
	intr = new Interpreter();
}


Tournament::~Tournament() {

}

// When finishing a tournament it updates the universal statistic arrays with the temp ones
void Tournament::update(int * times_fought_g, int * wins_g, float * total_years_g, int * ties_g) {
	for (int i = 0; i < num_strategies; i++) {
		times_fought_g[i] += times_fought[i];
		wins_g[i] += wins[i];
		total_years_g[i] += total_years[i];
		ties_g[i] += ties[i];
	}
}

// Tournament implementation for gangs
void Tournament::begin_gangs(int start, int end, int * times_fought_g, int * wins_g, float * total_years_g, int * ties_g, string * files, string * combos, int spy_chance, bool change_choice) {
	
	for (int i = start; i < end; i++) {
		string file_names[5];
		for (int k = 0; k < 5; k++) {
			file_names[k] = files[combos[i][k] - 48];
		}
		Gang * g1 = new Gang(file_names, spy_chance, change_choice);
		if (g1->check_success()) {
			for (int j = i; j < num_strategies; j++) {
				string file_names2[5];
				for (int k = 0; k < 5; k++) {
					file_names2[k] = files[combos[j][k] - 48];
				}
				Gang * g2 = new Gang(file_names2, spy_chance, change_choice);
				if (g2->check_success()) {
					if (intr->against(g1, g2, 200)) {
						times_fought[i] ++;
						if (i != j) {
							times_fought[j] ++;
							total_years[j] += g2->myscore;
						}
						total_years[i] += g1->myscore;
						if (g1->myscore < g2->myscore) {
							wins[i]++;
						}
						else if (g2->myscore < g1->myscore) {
							wins[j]++;
						}
						else if (g2->myscore == g1->myscore && i != j) {
							ties[i]++;
							ties[j]++;
						}
						else {
							ties[i]++;
						}
					}
				}
				g1->init();
				delete g2;
				
			}
		}
		delete g1;
		
	}
	
	std::lock_guard<std::mutex> guard(t_update_mutex);
	update(times_fought_g, wins_g, total_years_g, ties_g);
}

// Tournament implementation for single strategies
void Tournament::begin_single (int start, int end, int * times_fought_g, int * wins_g, float * total_years_g, int * ties_g, string * files) {
	for (int i = start; i < end; i++) {
		string loc = files[i];
		Strategy * strat1 = new Strategy(loc);
		if (strat1->check_success()) {
			for (int j = i; j < num_strategies; j++) {
				string loc = files[j];
				Strategy * strat2 = new Strategy(loc);
				if (strat2->check_success()) {
					if (intr->against(strat1, strat2, 200)) {
						times_fought[i] ++;
						if (i != j) {
							times_fought[j] ++;
							total_years[j] += strat2->num_vars["MYSCORE"];
						}
						total_years[i] += strat1->num_vars["MYSCORE"];
						if (strat1->num_vars["MYSCORE"] < strat2->num_vars["MYSCORE"]) {
							wins[i]++;
						}
						else if (strat2->num_vars["MYSCORE"] < strat1->num_vars["MYSCORE"]) {
							wins[j]++;
						}
						else if (strat2->num_vars["MYSCORE"] == strat1->num_vars["MYSCORE"] && i!=j){
							ties[i]++;
							ties[j]++;
						}
						else {
							ties[i]++;
						}
					}
				}
				strat1->init();
				delete strat2;
			}
		}
		delete strat1;
	}
	std::lock_guard<std::mutex> guard(t_update_mutex);
	update(times_fought_g, wins_g, total_years_g, ties_g);
}




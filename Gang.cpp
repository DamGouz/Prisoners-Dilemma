#include "Gang.h"
#include "Gang_Strategy.h"
#include "Interpreter.h"


/*
Constructor
It creates the files array, and checks for errors in their compilation.
*/
Gang::Gang(string * files, int spy, bool c_c):spy_p(spy), change_choice(c_c){
	success = true;
	for (int i = 0; i < 5; i++) {
		strat[i] = new Gang_Strategy(files[i]);
		success = success && strat[i]->check_success();
	}
}

Gang::~Gang() {
	for (int i=0;i<5; i++)
		delete strat[i];
}

// Checks if the leader has found the spy
int Gang::spy_found(int spy) {
	int leader = rand() % 5;
	int choice = rand() % 5;
	while (choice == leader)	choice = rand() % 5;
	int first = choice;
	int revealed = rand() % 5;
	while (revealed == spy || revealed == leader || revealed == choice) revealed = rand() % 5;
	if (change_choice == true){
		choice = rand() % 5;
		while (choice==first || choice == revealed) choice = rand() % 5;
		if (choice == spy) return 0;
	}
	else {
		if (choice == spy) return 2;
	}
	return 1;
}


// Calculates the outcome of the gang, given the individual outcomes. Also takes into account the spy
int Gang::operator >(Interpreter * intr) {
	int result = 0;
	int spy = rand() % 5;
	int spy_result;
	bool suc = true;
	for (int i = 0; i < 5; i++) {
		int r_temp = intr->run(strat[i]);
		if (r_temp == 100)
			suc = false;
		result += r_temp;
		if (i == spy) {
			spy_result = r_temp;
		}
	}
	if (rand() % 100 < spy_p) {
		int ret = spy_found(spy);
		result = result - spy_result;
		if (result == 0 || result == 1) result++;
		else if (result == 2) result = result + spy_result;
		if (ret!=1) {
			if (ret == 0) result = result + 6;
			else result = result*(-1) - 1;
			/*
			If result = 0-5 then the spy has not been found
			If result = 6-10 then the spy has been found without changing choices
			If result = (-6) - (-1) then the spy has been found after changing choices
			*/
		}
	}
	if (suc == false)
		return 100;
	else
		return result;
}


// It calls the Gang_Strategy init() method, that adds the A, B and C possibilities in the appropriate array.
void Gang::init() { 
	for (int i = 0; i < 5; i++) {
		strat[i]->init();
	}
	myscore = 0;
}


// Calculates the imprisonment years for the gang g1 and updates the appropriate values.
void Gang::update(int g1, int g2) {
	string outcome;
	float spy1 = 1;
	float spy2 = 1;
	if (g1 > 5) {
		g1 -= 6;
		spy1--;
	}
	else if (g1 < 0) {
		g1 = (-1)*g1 - 1;
		spy1++;
	}
	if (g2 > 5) {
		g2 -= 6;
		spy2--;
	}
	else if (g2 < 0) {
		g2 = (-1)*g2 - 1;
		spy2++;
	}
	float years;
	if (g1 == 0 && g2 == 0) {
		outcome = "W";
		years = 2;
	}
	else if (g1 == 5 && g2 == 5) {
		outcome = "Z";
		years = 4;
	}
	else if (g1 == 0 && g2 == 5) {
		outcome = "X";
		years = 5;
	}
	else if (g1 == 5 && g2 == 0) {
		outcome = "Y";
		years = 0;
	}
	else if (g1==g2) {
		outcome = "C";
		years = 2;
	}
	else if (g1 < g2) {
		outcome = "B";
		years = 3;
	}
	else if (g2 < g1) {
		outcome = "A";
		years = 2.5;
	}
	if (spy1 != 1 && spy2 == 1) {
		years = spy1;
	}
	else if (spy2 != 1 && spy1 == 1) {
		years = 5;
	}
	else if (spy2 != 1 && spy1 != 1) {
		years = 6;
	}
	for (int i = 0; i < 5; i++) {
		strat[i]->num_vars["ALLOUTCOMES_"+outcome]++;
		strat[i]->num_vars["MYSCORE"] += years;
		strat[i]->char_vars["LASTOUTCOME"] = (int)outcome[0];
	}
	myscore += years;
}
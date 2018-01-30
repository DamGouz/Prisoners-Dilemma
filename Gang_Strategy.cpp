#include "Gang_Strategy.h"

//Similar to the Strategy Constructor, it calls its own virtual init() method
Gang_Strategy::Gang_Strategy(string file) { 
	init();
	file_loc = file;
	run(file);
}


/*
Extension of the Strategy init() method. It also adds the A, B and C possbilities.
*/
void Gang_Strategy::init() {
	num_vars["ALLOUTCOMES_A"] = 0;
	num_vars["ALLOUTCOMES_B"] = 0;
	num_vars["ALLOUTCOMES_C"] = 0;
	num_vars["ALLOUTCOMES_W"] = 0;
	num_vars["ALLOUTCOMES_X"] = 0;
	num_vars["ALLOUTCOMES_Y"] = 0;
	num_vars["ALLOUTCOMES_Z"] = 0;
	num_vars["MYSCORE"] = 0;
	num_vars["ITERATIONS"] = 0;
	char_vars["LASTOUTCOME"] = 0;
	char_vars["A"] = 65;
	char_vars["B"] = 66;
	char_vars["C"] = 67;
	char_vars["W"] = 87;
	char_vars["X"] = 88;
	char_vars["Y"] = 89;
	char_vars["Z"] = 90;
}
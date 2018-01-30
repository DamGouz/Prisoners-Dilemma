
#include "main.h"
class Gang;
class Interpreter;
class Strategy;
class Tournament {
public:
	Tournament() {};
	Tournament(int times);
	~Tournament();
	void begin_single (int start, int end, int * times_fought, int * wins, float * total_years, int * ties, string * files);
	void begin_gangs(int start, int end, int * times_fought_g, int * wins_g, float * total_years_g, int * ties_g, string * files, string * combos, int spy_chance, bool change_choice);
private:
	int num_strategies;
	Interpreter * intr;
	int * times_fought;
	int * wins;
	float * total_years;
	int * ties;
	void update(int * times_fought_g, int * wins_g, float * total_years_g, int * ties_g);
};
#include "main.h"
class Gang_Strategy;
class Interpreter;
class Gang {
public:
	Gang() {};
	~Gang();
	Gang (string * files, int spy, bool c_c);
	int operator >(Interpreter * intr);
	void update(int g1, int g2);
	bool check_success() { return success; };
	float myscore = 0;
	void init();
private:
	Gang_Strategy * strat[5];
	bool success;
	int spy_p;
	int spy_found(int spy);
	bool change_choice;
};
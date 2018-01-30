#include "main.h"

class Strategy {
public:
	Strategy() {};
	Strategy(string file);
	~Strategy() {}
	bool check_success();
	map<int, vector<string> > exec_lines;
	void update(int s1_r, int s2_r);
	string file_loc;
	virtual void init();
	map<string, float > num_vars;
	map<string, int > char_vars;
protected:
	void run(string file);
	bool Strategy::digit_dot(char v1, char v2, bool isfirst);
	bool check_line(vector<string> &line);
	bool normalise(string &line);
	bool ischar(char c);
	bool isspecial(char c);
	bool compare(char v1, char v2, bool isfirst);
	int num_lines = 0;
	bool success;
};

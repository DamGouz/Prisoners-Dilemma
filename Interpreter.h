#include "main.h"
class Gang;
class Strategy;
class Interpreter {
public:
	Interpreter() {}
	~Interpreter() {}
	int run(Strategy *s);
	int run(Gang *g);
	template <typename T>
	// Templated function that allows both gangs and strategies to be passed
	bool against(T *s1, T *s2, int times) {
		int s1_r = 1, s2_r = 1;
		int i = 0;
		while (i<times&&s1_r!=100&&s2_r!=100) {
			s1_r = run(s1);
			s2_r = run(s2);
			if (s1_r!=100&&s2_r!=100) {
				(*s1).update(s1_r, s2_r);
				(*s2).update(s2_r, s1_r);
			}
			i++;
		}
		if (s1_r != 100 && s2_r != 100) {
			return true;
		}
		else {
			return false;
		}

	}
private:
	int run_line(Strategy *s, vector<string> *line, int cur_line);
	float numeric(Strategy *s, string & line);
	bool compare(float x, float y, char oper);
	map<int, bool > pass_once;
};
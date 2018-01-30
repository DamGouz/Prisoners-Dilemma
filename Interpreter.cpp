#include "Interpreter.h"
#include "Strategy.h"
#include "Gang.h"


 
// Given a strategy s, it runs its first line
int Interpreter::run(Strategy *s) {
	pass_once.clear();
	int result = run_line(s, &s->exec_lines.begin()->second, s->exec_lines.begin()->first);
	return result;
}

// Given a gang g, it calls its calculate method
int Interpreter::run(Gang *g) {
	return (*g)>this;
}


// Returns the value of a numeric variable or number
float Interpreter::numeric(Strategy *s, string & line) {
	float result = 0;

	if (isdigit(line[0])) result = std::stof(line);
	else result = s->num_vars[line];
	return result;
}

// Compares 2 floats according to a comparison operator char
bool Interpreter::compare(float x, float y, char oper) {
	bool result = false;
	if (oper == '<') result = (x < y);
	else if (oper == '>') result = (x > y);
	else if (oper == '=') result = (x == y);
	return result;
}

// Recursively runs a strategy line by line
int Interpreter::run_line(Strategy *s, vector<string> *line, int cur_line) {
	int result = 100;
	int next_line;
	if (pass_once.count(cur_line) == 0) {
		pass_once[cur_line] = false;
		char oper = ' ';
		int p_m = 1;
		int i = 0;
		float lhs = 0;
		float rhs = 0;
		switch ((*line).size()) {
		case 1:
			if ((*line)[i] == "SILENCE") {
				result = 0;
			}
			else if ((*line)[i] == "BETRAY") {
				result = 1;
			}
			else if ((*line)[i] == "RANDOM") {
				result = rand() % 2;
			}
			break;
		case 2:
			i++;
			next_line = stoi((*line)[i]);
			if (s->exec_lines.find(next_line) != s->exec_lines.end()) {
				result = run_line(s, &s->exec_lines[next_line], next_line);
			}
			break;
		default:
			i++;
			if (((*line)[i] == "+" || (*line)[i] == "-" || s->num_vars.find((*line)[i])!= s->num_vars.end() || isdigit(((*line)[i])[0]))) {
				while ((*line)[i] != "<" && (*line)[i] != "=" && (*line)[i] != ">") {
					if ((*line)[i] == "-") {
						i++;
						p_m = -1;
					}
					else if ((*line)[i] == "+") {
						i++;
					}
					else {
						lhs += p_m*numeric(s, (*line)[i]);
						i++;
					}
				}
				oper = (*line)[i][0];
				i++;
				p_m = 1;
				while ((*line)[i] != "GOTO") {
					if ((*line)[i] == "-") {
						i++;
						p_m = -1;
					}
					else if ((*line)[i] == "+") {
						i++;
					}
					else {
						rhs += p_m*numeric(s, (*line)[i]);
						i++;
					}
				}
				i++;
				if (compare(lhs, rhs, oper) == true) {
					next_line = stoi((*line)[i]);
					if (s->exec_lines.find(next_line) != s->exec_lines.end()) {
						result = run_line(s, &s->exec_lines[next_line], next_line);
					}
				}
				else {
					if (next((s->exec_lines.find(cur_line)), 1) != s->exec_lines.end()) {
						next_line = next((s->exec_lines.find(cur_line)), 1)->first;
						if (s->exec_lines.find(next_line) != s->exec_lines.end()) {
							result = run_line(s, &s->exec_lines[next_line], next_line);
						}
					}
				}

			}
			else {
				lhs = (float)s->char_vars[(*line)[i]];
				i = i + 2;
				rhs = (float)s->char_vars[(*line)[i]];
				i = i + 2;
				if (lhs == rhs) {
					next_line = stoi((*line)[i]);
					if (s->exec_lines.find(next_line) != s->exec_lines.end()) {
						result = run_line(s, &s->exec_lines[next_line], next_line);
					}
				}
				else {
					if (next((s->exec_lines.find(cur_line)), 1) != s->exec_lines.end()) {
						next_line = next((s->exec_lines.find(cur_line)), 1)->first;
						if (s->exec_lines.find(next_line) != s->exec_lines.end()) {
							result = run_line(s, &s->exec_lines[next_line], next_line);
						}
					}

				}
			}
			break;
		}
	}
	return result;
}


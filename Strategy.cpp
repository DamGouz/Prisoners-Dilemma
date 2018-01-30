#include "Strategy.h"

// Constructor. It initializes the variable names arrays and then runs the file through a compiler to check for syntax errors.
Strategy::Strategy(string file) {
	init();
	file_loc = file;
	run(file);
}


// Calls the normalise for each line of the file
void Strategy::run(string file) {
	int cur_line = 0;
	string* line = new string;
	ifstream filehandler;
	success = true;
	filehandler.open(file);
	while (std::getline(filehandler, *line) && success == true) {
		cur_line++;
		success = normalise(*line);
	}

	if (exec_lines.empty()) {
		success = false;
	}
	delete line;
	filehandler.close();
}

// Puts every individual word of a line into a string of the vector, and then calls check_line for the whole line
bool Strategy::normalise(string &line) {
	vector<string> temp_line;
	string id_num = "";
	string *word = new string;
	int id = 0;
	bool first_num = true;
	bool flag = true;
	int i = 0;
	char *prev = new char;
	*prev = '1';
	for (std::string::iterator it = (line).begin(); it != (line).end(); ++it) {
		*it = toupper(*it);
		if (!iswspace(*it) && flag == true) {
			if (compare(*prev, *it, first_num)) {
				if (first_num) {
					id_num += *it;
				}
				else
					*word = *word + *it;
			}
			else {
				if (first_num) {
					if (id_num == "") {
						flag = false;
					}
					else {
						first_num = false;
						id = std::stoi(id_num);
						*word = *it;
					}

				}

				else {
					temp_line.push_back(*word);
					*word = *it;
				}
			}
		}
		if (first_num == false)
			*prev = *it;
	}
	if (*word != "") {
		temp_line.push_back(*word);
		if (!exec_lines.empty()) {
			if ((exec_lines.rbegin()->first < id)) {
				exec_lines[id] = temp_line;
				flag = check_line(exec_lines[id]);
			}
			else {
				flag = false;
			}
		}
		else {
			exec_lines[id] = temp_line;
			flag = check_line(exec_lines[id]);
		}

	}
	delete word, prev;
	return flag;
}

// Checks if a char is alpha or _
bool Strategy::ischar(char c) {
	if (isalpha(c) || c == '_')
		return true;
	else
		return false;
}

// Special occasion when a . is present
bool Strategy::digit_dot(char v1, char v2, bool isfirst) {
	if (!isfirst && ((isdigit(v1) && v2=='.') || (isdigit(v2) && v1 == '.')))
		return true;
	else
		return false;
}

// Checks for a comparison operator
bool Strategy::isspecial(char c) {
	if (c == '+' || c == '-' || c == '<' || c == '>' || c == '=')
		return true;
	else
		return false;
}

// Checks 2 characters, whether or not they are of the same type
bool Strategy::compare(char v1, char v2, bool isfirst) {
	if ((ischar(v1) && ischar(v2)) || (isdigit(v1) && isdigit(v2)) || (isspecial(v1) && isspecial(v2) || digit_dot (v1,v2,isfirst)))
		return true;
	else
		return false;
}

// Checks if a line is syntactically correct.
bool Strategy::check_line(vector<string> &line) {
	int i = 0;
	bool result = false;
	switch ((line).size()) {
	case 0:
		break;
	case 1:
		if ((line)[i] == "SILENCE" || (line)[i] == "BETRAY" || (line)[i] == "RANDOM")
			result = true;
		break;
	case 2:
		if ((line)[i] == "GOTO") {
			i++;
			if (isdigit(((line)[i])[0])) {
				result = true;
			}
		}
		break;
	default:
		if ((line)[i] == "IF") {
			bool cont = true;
			i++;
			if ((line)[i] == "+" || (line)[i] == "-")
				i++;
			if ((num_vars.find(line[i]) != num_vars.end() || isdigit(((line)[i])[0]))) {
				i++;
				while (i < line.size() && line[i] != "<"&&line[i] != "="&&line[i] != ">"&&cont == true) {
					cont = false;
					if ((line)[i] == "+" || (line)[i] == "-") {
						i++;
						if ((num_vars.find(line[i]) != num_vars.end() || isdigit(((line)[i])[0]))) {
							i++;
							cont = true;
						}
					}
				}
				if (i < line.size() && (line[i] == "<" || line[i] == "=" || line[i] == ">") && cont == true) {
					i++;
					if ((line)[i] == "+" || (line)[i] == "-")
						i++;
					if ((num_vars.find(line[i]) != num_vars.end() || isdigit(((line)[i])[0]))) {
						i++;
						while (i < line.size() && line[i] != "GOTO"&&cont == true) {
							cont = false;
							if ((line)[i] == "+" || (line)[i] == "-") {
								i++;
								if ((num_vars.find(line[i]) != num_vars.end() || isdigit(((line)[i])[0]))) {
									i++;
									cont = true;
								}
							}
						}
						if (i == line.size() - 2 && line[i] == "GOTO"  && cont == true) {
							i++;
							if (isdigit(((line)[i])[0])) {
								result = true;
							}
						}

					}
				}
			}
			if ((char_vars.find(line[i]) != char_vars.end()) && i == 1) {
				i++;
				if (line[i] == "=") {
					i++;
					if (char_vars.find(line[i]) != char_vars.end()) {
						i++;
						if (i == line.size() - 2 && line[i] == "GOTO") {
							i++;
							if (isdigit(((line)[i])[0])) {
								result = true;
							}
						}
					}
				}
			}
		}
		break;
	}
	return result;
}

// Returns success
bool Strategy::check_success() {
	return success;
}

// Updates the variables in the object, given the result of itself and the opposing strategy's
void Strategy::update(int s1_r, int s2_r) {
	int score;
	int last_out;
	if (s1_r == 0 && s2_r == 0) {
		score = 2;
		last_out = 87;
		num_vars["ALLOUTCOMES_W"]++;
	}
	else if (s1_r == 0 && s2_r == 1) {
		score = 5;
		last_out = 88;
		num_vars["ALLOUTCOMES_X"]++;
	}
	else if (s1_r == 1 && s2_r == 0) {
		score = 0;
		last_out = 89;
		num_vars["ALLOUTCOMES_Y"]++;
	}
	else {
		score = 4;
		last_out = 90;
		num_vars["ALLOUTCOMES_Z"]++;
	}
	char_vars["LASTOUTCOME"] = last_out;
	num_vars["MYSCORE"] += score;
	num_vars["ITERATIONS"]++;
}

// Initialises 2 maps of strings, using the variable names
void Strategy::init() {
	num_vars["ALLOUTCOMES_W"] = 0;
	num_vars["ALLOUTCOMES_X"] = 0;
	num_vars["ALLOUTCOMES_Y"] = 0;
	num_vars["ALLOUTCOMES_Z"] = 0;
	num_vars["MYSCORE"] = 0;
	num_vars["ITERATIONS"] = 0;
	char_vars["LASTOUTCOME"] = 0;
	char_vars["W"] = 87;
	char_vars["X"] = 88;
	char_vars["Y"] = 89;
	char_vars["Z"] = 90;
}

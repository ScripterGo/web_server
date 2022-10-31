

#include"json.h"
#include<stack>
#include<vector>

using namespace std;

/*
There should be a comma between every entry

*/

json::json() {
	
}

bool json::validate_brackets() const{ //validating the brackets
	stack<char> bracing;
	for (int i = 0; i < this->json_string.size(); i++) {
		char c = this->json_string[i];
		if (c == this->array_bracing[0] || c == this->object_bracing[0]) {
			bracing.push(c);
		}else if(c == this->array_bracing[1]) {
			if (bracing.top() != this->array_bracing[0]) return false;
			bracing.pop();
		}
		else if (c == this->object_bracing[1]) {
			if (bracing.top() != this->object_bracing[0]) return false;
			bracing.pop();
		}
	}

	if (bracing.size() > 0) return false;
	return true;
}

/*
There has to be a : or a , after every pair of qoutation or a closing bracket as next character signs
*/

bool json::validate_entries() const {
	if (this->json_string[this->find_next_real_character(0)] != this->object_bracing[0]) return false;
	vector<pair<int, int>> A;

	return this->validate_entries_object(0, (int) this->json_string.size() - 1, A);
}

//Assuming the global scope is an object {}
bool json::validate_entries_object(int i, int j, vector<pair<int,int>>& A) const {
	i++; //Because we assume that the first char is an opening bracket
	bool expecting_end = false;

	while (i < j) {
		int start_idx = i;
		
		//Checking the key
		i = this->find_next_char(i, this->marker);
		if (i == -1) break; //meaning that there were no more entries
		i = this->find_next_char(i+1, this->marker);
		if (i == -1) return false;
		
		if (expecting_end == true) return false; //There must have been something wrong with the ,

		//checking if the separator is present
		i = this->find_next_real_character(i + 1);
		if (i == -1) return false;
		if (this->json_string[i] != this->value_key_separator) return false;

		//looking at the first value-char
		i = this->find_next_real_character(i + 1);
		if (i == -1) return false;

		bool res = this->entry_type_handler(i, A); //going to increase i
		if (res == false) return false;
		
		A.push_back({ start_idx, i - 1});
		expecting_end = !this->process_entry_separator(i, A); //sets i to be the next real character
	}

	if (expecting_end == false) return false;
	return true;
}

//Determines what the characters should be identified as
DATA_TYPE json::identify_data_type(int i) const{
	char value_type = this->json_string[i];
	
	const int MAP_SZ = 3;
	pair<char, DATA_TYPE> char_to_idx_map[MAP_SZ] = {
		{'\"', DATA_TYPE::STRING},
		{'{', DATA_TYPE::OBJECT},
		{'[', DATA_TYPE::ARRAY}
	};

	for (int i = 0; i < MAP_SZ; i++) {
		if (value_type == char_to_idx_map[i].first) {
			return char_to_idx_map[i].second;
		}
	}
	if (is_digit(value_type) || value_type == this->minus_sign) return DATA_TYPE::NUMBER;
	if (value_type == 't' || value_type == 'f') return DATA_TYPE::BOOLEAN;

	return DATA_TYPE::INVALID;
}

// i is the index of the first value-character
bool json::entry_type_handler(int& i, vector<pair<int,int>>& A) const{
	DATA_TYPE expected_data_type = this->identify_data_type(i);

	switch (expected_data_type)
	{

	case DATA_TYPE::OBJECT:


	default:
		break;
	}

	char value_type = this->json_string[i];

	if (value_type == this->object_bracing[0]) {
		int closing_bracket_idx = this->find_next_close_bracket(i + 1);
		if (closing_bracket_idx == -1) return false;
		if (this->validate_entries_object(i, closing_bracket_idx, A) == false) return false;

		i = closing_bracket_idx+1; //skipping to the next element
	}
	else if (value_type == this->array_bracing[0]) {
		int closing_bracket_idx = this->find_next_close_bracket(i + 1);
		if (closing_bracket_idx == -1) return false;
		if(this->validate_entries_array(i, closing_bracket_idx, A) == false) return false;

		i = closing_bracket_idx+1; //skipping to the next element
	}
	else if (is_digit(value_type)) {
		pair<number_info, int> res = this->process_number(i);
		i = res.second;
	}
	else if (value_type == this->marker) {
		int ending_marker_idx = find_next_char(i+1, this->marker);
		if (ending_marker_idx == -1) return false;
		i = ending_marker_idx + 1;
	}
	else {
		printf("WARNING!");
		return false;
	}
	//i becomes the first index after the value

	return true;
}

//i is the first bracket index, j is the closing bracket index
bool json::validate_entries_array(int i, int j, vector<pair<int,int>>& A) const{
	i++; //Because we assume that the first character is a bracket
	bool expecting_end = false;

	while (i < j) {
		int start_idx = i;
		i = this->find_next_real_character(i);
		if (this->json_string[i] == this->array_bracing[1]) break; //meaning we reached the end
		if (expecting_end == true) return false;

		bool res = this->entry_type_handler(i, A);
		if (res == false) return false;

		A.push_back({start_idx, i - 1});
		expecting_end = !this->process_entry_separator(i, A); //sets i to be the next real character
		if (expecting_end == false) i++;
	}

	if (expecting_end == false) return false; //If there was a comma at the last element
	return true;
}

//Checking if there is an entry-separator after or on i, and validating it in that case.
bool json::process_entry_separator(int& i, vector<pair<int,int>>& A) const{
	i = this->find_next_real_character(i); //We assume that there will be a closing bracket at the very least
	if (i == -1) return false;

	if (this->json_string[i] == this->entry_separator) {
		A.push_back({i,i});
		return true;
	}
	else {
		return false;
	}
}

//returns information about the processed number and returns the index where it first ends
pair<number_info, int> json::process_number(int i) const {
	int decimalpoint_idx = -1;
	int multiplier = 1;
	number_info num;

	int int_part = 0;
	double decimal_part = 0;
	if (this->json_string[i] == '-') {
		multiplier = -1; i++;
	}

	for (; i < this->json_string.size(); i++) {
		char c = this->json_string[i];
		if (this->is_digit(c) == true) {
			if (decimalpoint_idx == -1) {
				int_part = int_part * 10;
				int_part += (c - '0');
			}
			else {
				int power = i - decimalpoint_idx;
				decimal_part += double(c - '0') / pow(10, power);
			}
		}
		else if (c == this->decimalpoint && decimalpoint_idx == -1) {
			decimalpoint_idx = i;
		}
		else {
			break;
		}
	}

	if (decimalpoint_idx != -1) {
		num.decimal = true;
		num.number = double(multiplier) * (int_part + decimal_part);
	}
	else {
		num.decimal = false;
		num.integer = multiplier * int_part;
	}
	return make_pair(num, i);
}

pair<bool, int> json::process_boolean(int i) const {
	const string bool_to_pattern[2] = { "false", "true" };
	bool expected_bool = this->json_string[i] == 'f' ? false : true;
	string p = bool_to_pattern[expected_bool];

	int j = 0;
	for (; j + i < this->json_string.size() && j < p.size(); j++) {
		if (this->json_string[i + j] != p[j]) return { false, -1 };
	}

	if (j != p.size()) return { false, -1 };
	return { expected_bool, j };
}

std::pair<string, int> json::process_string(int i) const {
	string resulting_string = "";

	int j = i + 1;
	while (this->json_string[j] != '"' && j < this->json_string.size()) {
		resulting_string.push_back(this->json_string[j]);
		j++;
	}

	if (j == this->json_string.size()) return { "", -1 };
	return { resulting_string, j };
}

std::pair<ContainerNode<std::string>*, int> json::process_object(int idx) const {
	int closing_bracket_idx = this->find_next_close_bracket(idx + 1);
	if (closing_bracket_idx == -1) return {nullptr, -1};
}


void json::load_json_string(const string& s) {
	this->json_string = s;
	//if (validate_json_string(this->json_string) == false) throw "Json formatting is incorrect";
}

bool json::is_digit(const char& c) const{
	return c >= 48 && c <= 57;
}
/*
int json::invert_number(int a) {
	int new_int = 0;
}
*/

bool json::is_real_character(char c) const{
	return 33 <= c && c <= 126;
}

int json::find_next_real_character(int idx) const {
	for (int i = idx; i < this->json_string.size(); i++) {
		if (this->is_real_character(this->json_string[i])) return i;
	}
	return -1;
}

//finding the next closing bracket at the same nestling depth
int json::find_next_close_bracket(int idx) const{
	int depth = 0;
	for (int i = idx; i < this->json_string.size(); i++) {
		if (this->json_string[i] == this->array_bracing[1] || this->json_string[i] == this->object_bracing[1]) {
			if (depth == 0) return i;
			depth--;
		}
		else if (this->json_string[i] == this->array_bracing[0] || this->json_string[i] == this->object_bracing[0]) {
			depth++;
		}
	}
	return -1;
}

int json::find_next_char( int idx, char c) const{
	for (int i = idx; i < this->json_string.size(); i++) {
		if (this->json_string[i] == c) return i;
	}
	return -1;
}

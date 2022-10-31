
#pragma once
#include<string>
#include<vector>
#include"leaf_node.h"
#include"container_node.h"

/*
Create a tree whereby every node is an object
Then we label the nodes with their corresponding name

*/

//containing information about a number
struct number_info {
	bool decimal;
	union {
		int integer;
		double number;
	};
};

enum class DATA_TYPE {
	NUMBER = 0,
	BOOLEAN = 1,
	STRING = 2,
	OBJECT = 3,
	ARRAY = 4,
	INVALID = 5
};

class json {
	const char* array_bracing = "[]";
	const char* object_bracing = "{}";
	const char marker = '"';
	const char value_key_separator = ':';
	const char entry_separator = ',';
	const char decimalpoint = '.';
	const char minus_sign = '-';

	std::string json_string;

public:

	json();
	void load_json_string(const std::string& s);
	bool validate_entries() const;
	bool validate_brackets() const;

private:
	bool is_digit(const char& c) const;
	
	DATA_TYPE identify_data_type(int i) const;
	std::pair<number_info, int> process_number(int idx) const;
	std::pair<std::string, int> process_string(int idx) const;
	std::pair<bool, int> process_boolean(int idx) const;
	std::pair<ContainerNode<std::string>*, int> process_object(int idx) const;
	std::pair<ContainerNode<int>*, int> process_array(int idx) const;


	bool validate_entries_object(int i, int j, std::vector<std::pair<int,int>>& A) const;
	bool validate_entries_array(int i, int j, std::vector<std::pair<int, int>>& A) const;
	bool entry_type_handler(int& i, std::vector<std::pair<int, int>>& A) const;
	bool process_entry_separator(int& i, std::vector<std::pair<int, int>>& A) const;

	int find_next_real_character(int idx) const; //Real character is defined to be a character 33-126
	bool is_real_character(char c) const;
	int find_next_close_bracket(int idx) const;
	int find_next_char(int idx, char c) const;
	

};

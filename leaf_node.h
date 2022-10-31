#pragma once

#include"node.h"
#include<variant>

class LeafNode : virtual public Node {
	node_type type = node_type::LEAF_NODE;
	
#define VARIANT_STRING 0
#define VARIANT_BOOL 1
#define VARIANT_INT 2
#define VARIANT_DOUBLE 3	
	std::variant<std::string, bool, int, double> _data;

public:
	LeafNode() {};
	node_type get_type() const;

	bool is_string_value() const;
	bool is_bool_value() const;
	bool is_int_value() const;
	bool is_double_value() const;
	
	//we assume that the datatypes of the entries should be static
	template<typename T>
	T get_value() const {
		return this->_data;
	}
};


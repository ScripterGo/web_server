
#include"leaf_node.h"

node_type LeafNode::get_type() const{
	return this->type;
}

inline bool LeafNode::is_string_value() const {
	return this->_data.index() == VARIANT_STRING;
}

inline bool LeafNode::is_bool_value() const {
	return this->_data.index() == VARIANT_BOOL;
}

inline bool LeafNode::is_double_value() const {
	return this->_data.index() == VARIANT_DOUBLE;
}

inline bool LeafNode::is_int_value() const {
	return this->_data.index() == VARIANT_INT;
}

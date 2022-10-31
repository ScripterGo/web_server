#pragma once

#include<vector>
#include<string>

enum class node_type {
	LEAF_NODE = 0,
	CONTAINER_NODE = 1,
	UNSPECIFIED = 2
};

class Node {
public:
	virtual ~Node() = default;
	virtual node_type get_type() = 0;
};




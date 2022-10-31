#pragma once

#include<vector>
#include"node.h"

template<typename T>
class ContainerNode : virtual public Node {
	node_type type = node_type::CONTAINER_NODE;
	std::vector<std::pair<T, Node*>> children = {};

public:
	ContainerNode() {};
	
	node_type get_type() const { return this->type; };
	
	void add_child(std::pair<T, Node*> entry) {
		this->children.push_back(entry);
	}
	
	const Node* operator[](T arg) const {
		for (int i = 0; i < children.size(); i++) {
			if (this->children[i].first == arg) return this->children[i].second;
		}
		return nullptr;
	}



};



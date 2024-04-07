#ifndef CONFLICTTREE_H
#define CONFLICTTREE_H

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <utility>
#include "definitions.h"
//TODO: add ConflictTree.h and ConflictTree.cpp to CMakeLists.txt
typedef struct {
    std::list<std::pair<int, Constraint_t>> constraints;
    std::vector<std::list<point>> solutions;
    int cost;
    CTree_Node* parent;
    std::list<CTree_Node*> children_list;
} CTree_Node;
bool operator<(const CTree_Node*, const CTree_Node*);

class ConflictTree
{
private:
    CTree_Node root;
    int n_agents;
    std::priority_queue<CTree_Node> pq;
public:
    ConflictTree(int, CTree_Node);
    ~ConflictTree();
    void add_node(CTree_Node*, CTree_Node);
    bool is_empty();
    CTree_Node* best_node();
    std::pair<bool, Conflict_t> validate(CTree_Node*);
};

#endif //CONFLICTTREE_H

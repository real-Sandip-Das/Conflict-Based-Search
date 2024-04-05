#include "../include/ConflictTree.h"

bool operator<(const CTree_Node* n1, const CTree_Node* n2) {
    return n1->cost > n2->cost; //since, top() in priority_queue returns the greatest element by default
}

/*********************************
 *  Conflict Tree Implementation *
 *********************************/

ConflictTree::ConflictTree(int number_of_agents, CTree_Node given_root)
{
    n_agents = number_of_agents;
    root = given_root;
    pq.push(&self.root);
}

ConflictTree::~ConflictTree()
{
    stack<CTree_Node*> stk;
    stk.push(&self.root);
    while !stk.empty() {
        CTree_Node* p_top = stk.top();
        if !p_top->children_list.empty() {
            for (CTree_Node* node_p: p_top->children_list) {
                stk.push(node_p);
                continue;
            }
        }
        delete p_top;
        stk.pop();
    }
}

void ConflictTree::add_node(CTree_Node* p_parent, CTree_Node new_node) {
    new_node.parent = p_parent;
    CTree_Node* new_node_ptr = new CTree_Node;
    *new_node_ptr = new_node;
    new_node.parent.children_list.push_back(new_node_ptr);
    pq.push(new_node_ptr);
}

bool ConflictTree::is_empty() {
    return pq.empty();
}

CTree_Node* ConflictTree::best_node(){
    CTree_Node* best_node_p = pq.top();
    pq.pop();
    return best_node_p;
}
//TODO:Convert custom operators to comparator functions for defining sets and priority_queues
std::pair<bool, Conflict_t> validate(CTree_Node* node) {
    std::vector<list<point>>& solutions = node->solutions;
    std::vector<std::list<point>::iterator> l_it_vec;
    for(auto it = solutions.begin(); it != solutions.end(); it++) {
        l_it_vec.push_back((*it).begin());
    }
    bool flag = true;
    bool conflict_found = false;
    while flag and !conflict_found {
        flag = false;
        set<point> point_set;
        for(unsigned int i=0; i<l_it_vec.size(); i++) {
            l_it_vec[i]++;
            if (l_it_vec[i] == solutions[i].end()) {
                l_it_vec[i]--;
            } else flag = true;
            int count = point_set.count(*l_it_vec[i]);
            if(count) {
                conflict_found = true;
            }
        }
    }
}
//TODO:Complete this implementation
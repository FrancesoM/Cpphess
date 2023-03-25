//
//  chess_tree.h
//  chess_engine
//
//  Created by Francesco Maio on 18/03/23.
//

#ifndef chess_tree_h
#define chess_tree_h

#include <vector>
#include <stack>
#include <list>

typedef std::vector<int> children_idxs_t;

template<class T>
class node_t{
public:
    node_t(T data) : data(data),visited(0) {
        children_idxs = children_idxs_t();
    };
    T data;
    int data_idx;
    bool visited;
    int parent_idx;
    children_idxs_t children_idxs;
};

template<typename T>
class Tree{
public:
    std::vector<node_t<T> > nodes;
    
    Tree(T root_data) {
        node_t<T> root_node(root_data);
        root_node.parent_idx = -1;
        nodes.push_back(root_node);
    };
    
    void add_child_at(T data,int idx) {
        node_t<T> new_node(data);
        nodes.push_back(new_node);
        
        int last_idx = nodes.size() -1 ;
        
        nodes[last_idx].data       = data;
        nodes[last_idx].data_idx   = last_idx;
        nodes[last_idx].parent_idx = idx;
        
        nodes[idx].children_idxs.push_back(last_idx);
    }
    
    std::list<int> find_leaves() {
        
        auto leaves = std::list<int>();
        
        if(nodes.size() == 1 ) {
            leaves.push_back(0);
            return leaves;
        }
        
        // Init the nodes at non-visited
        for(auto n = nodes.begin(); n < nodes.end() ; n++) {
            n->visited = 0;
        }
        
        // Perform the actual algorithm - stack has references because we don't want duplicates.
        std::stack<node_t<T>*> S;
        nodes[0].visited = 1;
        S.push(&nodes[0]);
        
        while( S.size() > 0) {
            auto v = *S.top();
            S.pop();
            
            for( auto w_idx : v.children_idxs ) {
                if (  !nodes[w_idx].visited ) {
                    nodes[w_idx].visited = 1;
                    S.push( &nodes[w_idx] );
                }
            }
            
            if( v.children_idxs.size() == 0 )
            {
                leaves.push_front(v.data_idx);
            }
        }
        
        return leaves;
    }
    
    // Data is an unique identifier for this to work.
    int get_idx(T target_data) {
        
        // Init the nodes at non-visited
        for(auto n = nodes.begin(); n < nodes.end() ; n++) {
            n->visited = 0;
        }
        
        // Perform the actual algorithm - stack has pointers because we don't want duplicates.
        std::stack<node_t<T>*> S;
        nodes[0].visited = 1;
        S.push(&nodes[0]);
        
        while( S.size() > 0) {
            auto v = *S.top();
            S.pop();
            
            for( auto w_idx : v.children_idxs ) {
                if (  !nodes[w_idx].visited ) {
                    nodes[w_idx].visited = 1;
                    S.push( &nodes[w_idx] );
                }
            }
            
            if( v.data == target_data )
            {
                return v.data_idx;
            }
        }
        
        return -1;
        
    }
    
    void find_ancestors(std::vector<int> & sequence, int idx) {
        sequence.push_back(idx);
        
        if (this->nodes[idx].parent_idx != -1) {
            idx = nodes[idx].parent_idx;
            find_ancestors(sequence, idx);
        }
    }
    
    
};


#endif /* chess_tree_h */

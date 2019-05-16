#include "graph.h"
#include <QDebug>

inline void Graph::add_vertex(QPoint coord)
{
    Node * t = new Node;
    t->m_point = coord;
    nodes.push_back(t);
}

void Graph::add_edge(Node *from, Node *to)
{
    if(from->m_nexts.empty())
    {
        from->m_nexts.push_back(to);
    }
    else{
    auto ite = std::find(from->m_nexts.begin(), from->m_nexts.end(), to);
    if (ite == from->m_nexts.end())
    {
        from->m_nexts.push_back(to);
    }
    }
    if(to->m_nexts.empty())
    {
       to->m_nexts.push_back(from);
    }
    else{

    auto ite1 = std::find(to->m_nexts.begin(), to->m_nexts.end(), from);
    if (ite1 == to->m_nexts.end())
    {
        to->m_nexts.push_back(from);
    }
    }
}


//---------------------------------------DFS's-------------------------
inline void Graph::DFS(Node* a)
{
    visited_vertexes.clear();
    just_DFS(a);
}

inline void Graph::just_DFS(Node * a)
{
    visited_vertexes.push_back(a);
    for (auto it = a->m_nexts.begin(); it != a->m_nexts.end(); ++it) {
        auto ite = std::find(visited_vertexes.begin(), visited_vertexes.end(), *it);
        if (ite == visited_vertexes.end()) {
            just_DFS(*it);
        }
    }
}

inline void Graph::DFS_with_ignore(Node* start_vertex, Node* ignoring_vertex)
{
    visited_vertexes.clear();
    DFS_w_i(start_vertex, ignoring_vertex);
}

inline void Graph::DFS_w_i(Node* start_vertex, Node* ignoring_vertex)
{
    visited_vertexes.push_back(start_vertex);
    for (auto it = start_vertex->m_nexts.begin(); it != start_vertex->m_nexts.end(); ++it) {
        auto ite = std::find(visited_vertexes.begin(), visited_vertexes.end(), *it);
        if (ite == visited_vertexes.end() && *it != ignoring_vertex) {
            DFS_w_i(*it, ignoring_vertex);
        }
    }
}

inline void Graph::DFS_for_blocks(Node* start_vertex,Node*ignoring_vertex)
{
     auto itk = find(visited_vertexes.begin(), visited_vertexes.end(), start_vertex);
     if (itk == visited_vertexes.end()) {
         visited_vertexes.push_back(start_vertex);
         current_block.push_back(start_vertex);
         for (auto it = start_vertex->m_nexts.begin(); it != start_vertex->m_nexts.end(); ++it) {
             auto ite = find(current_block.begin(), current_block.end(), *it);

             if (ite == current_block.end() && *it != ignoring_vertex) {
                 if (!(is_cutpoint(*it))) {
                     DFS_for_blocks(*it, ignoring_vertex);
                 }
                 else {
                     current_block.push_back(*it);
                 }
             }
         }
     }
}

//---------------------------------cutpoints----------------------------

inline void Graph::get_cutpoints()
{
    if (cutpoints.empty())
            {
                calc_cutpoints();
            }
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            if (cutpoints[i] == nodes[k])
            {
                qDebug() << k;
            }
        }
    }
    qDebug() << "";
}

void Graph::calc_cutpoints()
{
    cutpoints.clear();
    for (size_t i = 0; i < nodes.size(); ++i) {
            DFS(nodes[i]);
            std::vector<Node*> tmp = visited_vertexes;
            DFS_with_ignore(nodes[i]->m_nexts[0], nodes[i]);
            if (tmp.size() - visited_vertexes.size() > 1) {
                cutpoints.push_back(nodes[i]);
        }
    }
}

inline bool Graph::is_cutpoint(Node* a)
{
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        if (a == cutpoints[i])
        {
            return true;
        }
    }
    return false;
}

//---------------------------------blocks-&-bridges----------------------

void Graph::calc_blocks()
{

    calc_cutpoints();
    blocks.clear();
    visited_vertexes.clear();
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        for (size_t j = 0; j < cutpoints[i]->m_nexts.size(); ++j) {
            if (!is_cutpoint(cutpoints[i]->m_nexts[j])) { //The next vertex isn't cutpoint
                current_block.clear();
                DFS_for_blocks(cutpoints[i]->m_nexts[j], cutpoints[i]);
                auto it1 = std::find(current_block.begin(), current_block.end(), cutpoints[i]);
                if (!current_block.empty()) {
                    if (it1 == current_block.end()) { current_block.push_back(cutpoints[i]); }
                    blocks.push_back(current_block);
                }
            }
            else {
                if (!search_items_in_blocks(cutpoints[i], cutpoints[i]->m_nexts[j])) {
                    current_block.clear();
                    //Denying repeat of the bridges
                    bool a = false;
                    if (!bridges.empty()) {
                        for (size_t t = 0; t < bridges.size(); ++t) {
                            if (bridges[t][0] == cutpoints[i] && bridges[t][1] == cutpoints[i]->m_nexts[j]) {
                                a = true;
                            }
                        }
                        if (a == false) {
                            current_block.push_back(cutpoints[i]->m_nexts[j]);
                            current_block.push_back(cutpoints[i]);
                            blocks.push_back(current_block);
                            bridges.push_back(current_block);
                        }
                    }
                    else {
                        current_block.push_back(cutpoints[i]->m_nexts[j]);
                        current_block.push_back(cutpoints[i]);
                        blocks.push_back(current_block);
                        bridges.push_back(current_block);
                    }
                }
            }
        }
    }
}

bool Graph::search_items_in_blocks(Node* a, Node* b) {
    bool first = true;
    bool second = false;
    if (!blocks.empty()) {
        for (size_t i = 0; i < blocks.size(); ++i) {
            for (size_t j = 0; j < blocks[i].size(); ++j) {
                if (blocks[i][j] == a) {
                    first = true;
                }
                else if (blocks[i][j] == b) {
                    second = true;
                }
            }

        }
        if (first && second) {
            return true;
        }
    }
    return false;
}

void Graph::add_edge_to_node_slot(Node* from,Node* to)
{
    add_edge(from,to);
}

void Graph::show_blocks_slot()
{
    calc_blocks();
    if(index < blocks.size())
    {
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            nodes[k]->a.setRgb(250,250,0,250);
        }
        for (size_t j = 0; j < blocks[index].size(); ++j)
        {
            blocks[index][j]->a.setRgb(100,10,10,150);
        }
        ++index;
    }else{
        index = 0;
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            nodes[k]->a.setRgb(250,250,0,250);
        }
        for (size_t j = 0; j < blocks[index].size(); ++j)
        {
        blocks[index][j]->a.setRgb(100,10,10,150);
        }
        ++index;
    }
}

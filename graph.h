#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <vector>
#include <assert.h>

class Graph: public QObject
{
    Q_OBJECT
public:
    Graph() = default;
    ~Graph() = default;

private:
    std::vector<std::vector<size_t>> vertexes_indexing;
    std::vector<std::vector<size_t>> blocks;
    std::vector < std::vector<size_t>> bridges;
    std::vector<size_t> visited_vertexes;
    std::vector<size_t> cutpoints;
    std::vector<size_t> current_block;
    void DFS_w_i(size_t start_vertex, size_t ignoring_vertex);
    void DFS_with_ignore(size_t start_vertex, size_t ignoring_vertex);
    void DFS_self_ignore(size_t start_vertex, size_t ignoring_vertex);
    void just_DFS(size_t a);
    void calc_cutpoints();
    void calc_blocks();
    bool is_cutpoint(size_t a);
    bool search_items_in_blocks(size_t a, size_t b);

public:
    void DFS(size_t a);
    void add_vertex(std::vector<size_t> t);
    void get_last_DFS();
    void reverse_connections();
    std::vector<size_t> get_cutpoints();
    void get_blocks();
};


inline void Graph::DFS(size_t a)
{
    visited_vertexes.clear();
    just_DFS(a);
}

inline void Graph::just_DFS(size_t a)
{
    assert(a >= 0 && a < vertexes_indexing.size());
    visited_vertexes.push_back(a);
    for (auto it = vertexes_indexing[a].begin(); it != vertexes_indexing[a].end(); ++it) {
        auto ite = find(visited_vertexes.begin(), visited_vertexes.end(), *it);
        if (ite == visited_vertexes.end()) {
            just_DFS(*it);
        }
    }
}

inline void Graph::DFS_with_ignore(size_t start_vertex, size_t ignoring_vertex)
{
    visited_vertexes.clear();
    DFS_w_i(start_vertex, ignoring_vertex);
}

inline void Graph::DFS_w_i(size_t start_vertex, size_t ignoring_vertex)
{
    visited_vertexes.push_back(start_vertex);
    for (auto it = vertexes_indexing[start_vertex].begin(); it != vertexes_indexing[start_vertex].end(); ++it) {
        auto ite = find(visited_vertexes.begin(), visited_vertexes.end(), *it);
        if (ite == visited_vertexes.end() && *it != ignoring_vertex) {
            DFS_w_i(*it, ignoring_vertex);
        }
    }
}

inline void Graph::DFS_self_ignore(size_t start_vertex, size_t ignoring_vertex)
{
    auto itk = find(visited_vertexes.begin(), visited_vertexes.end(), start_vertex);
    if (itk == visited_vertexes.end()) {
        visited_vertexes.push_back(start_vertex);
        current_block.push_back(start_vertex);
        for (auto it = vertexes_indexing[start_vertex].begin(); it != vertexes_indexing[start_vertex].end(); ++it) {
            auto ite = find(current_block.begin(), current_block.end(), *it);

            if (ite == current_block.end() && *it != ignoring_vertex) {
                if (!(is_cutpoint(*it))) {
                    DFS_self_ignore(*it, ignoring_vertex);
                }
                else {
                    current_block.push_back(*it);
                }
            }
        }
    }
}

inline void Graph::add_vertex(std::vector<size_t> t)
{
    vertexes_indexing.push_back(t);
}

inline void Graph::reverse_connections()
{
    for (size_t i = 1; i < vertexes_indexing.size() - 1; ++i) {
        for (size_t j = 0; j < i; ++j) {
            for (size_t t = 0; t < vertexes_indexing[j].size() - 1; ++t) {
                if (i == vertexes_indexing[j][t]) {
                    vertexes_indexing[i].push_back(j);
                }
            }
        }
    }
}

inline std::vector<size_t> Graph::get_cutpoints()
{
    if (cutpoints.empty()) {
        calc_cutpoints();
    }
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        //std::cout << cutpoints[i];
    }
    //std::cout << std::endl;
    return cutpoints;
}

void Graph::calc_cutpoints()
{
    cutpoints.clear();
    for (size_t i = 0; i < vertexes_indexing.size(); ++i) {
        if (1 == vertexes_indexing[i].size() && 1 != vertexes_indexing[vertexes_indexing[i][0]].size()) {
            if (!is_cutpoint(vertexes_indexing[i][0])) {
                cutpoints.push_back(vertexes_indexing[i][0]);
            }
        }
        else {
            DFS(i);
            std::vector<size_t> tmp = visited_vertexes;
            DFS_with_ignore(vertexes_indexing[i][0], i);
            if (tmp.size() - visited_vertexes.size() > 1) {
                cutpoints.push_back(i);
            }
        }
    }
}

void Graph::calc_blocks()
{
    if (cutpoints.empty()) {
        calc_cutpoints();
    }

    blocks.clear();
    visited_vertexes.clear();
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        for (size_t j = 0; j < vertexes_indexing[cutpoints[i]].size(); ++j) {
            if (!is_cutpoint(vertexes_indexing[cutpoints[i]][j])) { //The next vertex isn't cutpoint
                current_block.clear();
                DFS_self_ignore(vertexes_indexing[cutpoints[i]][j], cutpoints[i]);
                auto it1 = find(current_block.begin(), current_block.end(), cutpoints[i]);
                if (!current_block.empty()) {
                    if (it1 == current_block.end()) { current_block.push_back(cutpoints[i]); }
                    blocks.push_back(current_block);
                }
            }
            else {
                if (!search_items_in_blocks(cutpoints[i], vertexes_indexing[cutpoints[i]][j])) {
                    current_block.clear();
                    //Denying repeat of the bridges
                    bool a = false;
                    if (!bridges.empty()) {
                        for (size_t t = 0; t < bridges.size(); ++t) {
                            if (bridges[t][0] == cutpoints[i] && bridges[t][1] == vertexes_indexing[cutpoints[i]][j]) {
                                a = true;
                            }
                        }
                        if (a == false) {
                            current_block.push_back(vertexes_indexing[cutpoints[i]][j]);
                            current_block.push_back(cutpoints[i]);
                            blocks.push_back(current_block);
                            bridges.push_back(current_block);
                        }
                    }
                    else {
                        current_block.push_back(vertexes_indexing[cutpoints[i]][j]);
                        current_block.push_back(cutpoints[i]);
                        blocks.push_back(current_block);
                        bridges.push_back(current_block);
                    }
                }
            }
        }

    }
}

inline bool Graph::is_cutpoint(size_t a)
{
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        if (a == cutpoints[i])
        {
            return true;
        }
    }
    return false;
}

void Graph::get_blocks()
{
    if (blocks.empty()) {
        calc_blocks();
    }
    for (size_t i = 0; i < blocks.size(); ++i) {
        //std::cout << "Block no " << i << std::endl;
        for (size_t j = 0; j < blocks[i].size(); ++j) {
            //std::cout << blocks[i][j];
        }
        //std::cout << std::endl;

    }
    //std::cout << std::endl;

}

inline void Graph::get_last_DFS()
{
    for (size_t i = 0; i < visited_vertexes.size(); ++i) {
        //std::cout << visited_vertexes[i];
    }
    //std::cout << std::endl;
}

bool Graph::search_items_in_blocks(size_t a,size_t b) {
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

#endif // GRAPH_H

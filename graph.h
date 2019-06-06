#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

class Graph: public QObject
{
    Q_OBJECT
public:
    Graph();
    Graph(std::vector<Node*> a):nodes(a){}
    ~Graph() = default;

private:
    std::vector<std::vector<Node *>> subgraphs;
    std::vector<Node *> nodes;
    size_t index = 0;
    size_t ind1 = 0;

    std::vector<std::vector<Node*>> blocks;
    std::vector<std::vector<Node*>> bridges;
    std::vector<Node *> visited_vertexes;
    std::vector<Node *> visited_vertexes_for_disconnected_graph;
    std::vector<Node *> cutpoints;
    std::vector<Node*> current_block;

    void add_vertex(QPoint coord);
    void add_edge(Node *from, Node *to);

    void DFS(Node* a);
    void just_DFS(Node * a);
    void DFS_with_ignore(Node* start_vertex, Node* ignoring_vertex);
    void DFS_w_i(Node* start_vertex, Node* ignoring_vertex);
    void DFS_for_blocks(Node* start_vertex,Node* ignoring_vertex);
    void DFS_for_circle_search(Node*a);

    void calc_cutpoints();
    void calc_cutpoints(Node* forx);
    bool is_cutpoint(Node* a);
    int circle_search_dfs(Node * a);
    void get_cutpoints();

    void calc_blocks();
    bool search_items_in_blocks(Node* a,Node* b);

    bool is_connected();
    void seperate_to_subgraphs();

    bool has_more_than1(Node* a);

public:
    friend class mywidget;
public slots:
    void add_edge_to_node_slot(Node* from,Node* to);
    void show_blocks_slot();
    void show_bridges_slot();
};

#endif // GRAPH_H

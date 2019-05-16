#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include "node.h"
#include "graph.h"



class mywidget : public QWidget
{
    Q_OBJECT
public:
    explicit mywidget(QWidget *parent = nullptr);

signals:
    void objects_total_signal(size_t,size_t);
    void add_edge_signal_for_graph(Node*,Node*);
    void show_blocks();
public slots:
    void Receiving_button(int t);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

protected:

private:
    bool clicked_on_node = false;
    std::vector<std::vector<Node *>> edges;
    size_t clicked_node_index;
    bool add_vertex_received = false;
    bool add_edge_received = false;
    bool delete_received = false;
    std::vector<Node *> tmp;
    Graph graph;

};


#endif // MYWIDGET_H

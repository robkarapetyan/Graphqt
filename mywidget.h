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
    void objects_total_signal(int,int);
public slots:
   // void mouseclickedxy(int a,int b);
    void Receiving_button(int t);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

protected:

private:
    bool clicked_on_node = false;
    //QPoint m_mousePos;
     QVector<Node *> nodes;
     QVector<QVector<Node *>> edges;
     int clicked_node_index;
    bool add_vertex_received = false;
    bool add_edge_received = false;
    bool delete_received = false;
    QVector<Node *> tmp;


};

#endif // MYWIDGET_H

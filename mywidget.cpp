#include "mywidget.h"
#include <QDebug>
#include <QVector>
#include "node.h"

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{
    QObject::connect(this,SIGNAL(add_edge_signal_for_graph(Node*,Node*)),&graph,SLOT(add_edge_to_node_slot(Node*,Node*)));
    QObject::connect(this,SIGNAL(show_blocks()),&graph,SLOT(show_blocks_slot()));

    QPalette Pal(palette());

    // background color

    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    setMouseTracking(true);
}

void mywidget::Receiving_button(int t)
{
 switch(t){
    case 1:
     add_vertex_received = !add_vertex_received;
     add_edge_received = false;

    break;
    case 2:
     add_edge_received = !add_edge_received;
      add_vertex_received = false;
    break;
    case 3:
    emit show_blocks();
    break;
    case 4:
    delete_received = !delete_received;
    break;
    default:
    break;
 }

}

void mywidget::mouseMoveEvent(QMouseEvent *event)
{
    if (clicked_on_node )
    {
        graph.nodes[clicked_node_index]->m_point = event->pos();
        update();
    }
 QWidget::mouseMoveEvent( event);
}

void mywidget::mousePressEvent(QMouseEvent *event){
    QWidget::mousePressEvent( event);
    for(size_t i = 0; i < graph.nodes.size();++i)
    {
        if(graph.nodes[i]->m_point.x() +20 > event->x() && event->x() > graph.nodes[i]->m_point.x() - 20)
        {
            if(graph.nodes[i]->m_point.y() +20 > event->y() && event->y() > graph.nodes[i]->m_point.y() - 20)
            {
               clicked_on_node = true;
               clicked_node_index = i;
              // qDebug() << clicked_node_index;
               break;
             }
            else{
             clicked_on_node = false;
            }
        }
    }

if (event->button() == Qt::LeftButton)
{
    if(add_vertex_received == true )
    {
      if(clicked_on_node == false)
      {
        Node * b = new Node();
        b->m_point.setX(event->x());
        b->m_point.setY(event->y());
        graph.nodes.push_back(b);

      }
      else if(delete_received && clicked_on_node)
        {
              for(size_t j = 0; j < edges.size(); ++j)
                       {
                       for(size_t i = 0; i < edges.size(); ++i)
                       {
                               if(edges[i][0] == graph.nodes[clicked_node_index] || edges[i][1] == graph.nodes[clicked_node_index])
                               {
                                 edges.erase(edges.begin() + i);
                                 tmp.clear();
                               }
                       }
                       }
                graph.nodes.erase((std::remove(graph.nodes.begin(), graph.nodes.end(), graph.nodes[clicked_node_index])),graph.nodes.end());
          qDebug() << "delete arrived xd";
        }
    }
    else if(add_edge_received == true && clicked_on_node == true)
    {

       // check if node is selected and don't let release to work,if selected items are above 2
        if(delete_received)
        {
            qDebug() << "delete edge arrived";
            qDebug() << tmp.size();

            if(tmp.empty())
            {
                tmp.push_back(graph.nodes[clicked_node_index]);
            }
            else if(tmp.size() == 1){


                if(!edges.empty())
                {

                    for(size_t i = 0; i < edges.size(); ++i)
                    {
                        if((edges[i][0] == tmp[0] && edges[i][1] == tmp[1]) || (edges[i][0] == tmp[1] && edges[i][1] == tmp[0]))
                        {
                        std::vector<Node*> temporary;
                        temporary = *(edges.begin()+i);
                       // edges.erase(temporary);
                        edges.erase((std::remove(edges.begin(), edges.end(), temporary)),edges.end());

                        qDebug() << "fffffffffffffffffffffd";
                        qDebug() << tmp.size();
                        tmp.clear();
                        break;
                        }
                    }
                }
            }

            if(tmp.size() >= 2)
            {
                 tmp.clear();
            }
        }
        else
        {
            if(tmp.empty())
            {
                tmp.push_back(graph.nodes[clicked_node_index]);
            }
            else if(tmp.size() == 1)
            {
                tmp.push_back(graph.nodes[clicked_node_index]);
                if(!edges.empty())
                {
                    std::vector<Node *> tmp1;
                    tmp1.push_back(tmp[1]);
                    tmp1.push_back(tmp[0]);

                    auto ite = std::find(edges.begin(), edges.end(), tmp);
                    if (ite == edges.end())
                    {
                        auto ite1 = std::find(edges.begin(), edges.end(), tmp1);
                        if (ite1 == edges.end())
                        {
                            edges.push_back(tmp);
                            emit add_edge_signal_for_graph(tmp[0], tmp[1]);
                        }
                    }
                }
                else
                {
                    edges.push_back(tmp); 
                    emit add_edge_signal_for_graph(tmp[0], tmp[1]);
                    tmp.clear();
                }
            }
            if(tmp.size() >= 2)
            {
                tmp.clear();
            }
        }
    }
}
}

void mywidget::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setRenderHints(QPainter::HighQualityAntialiasing );

    if(!edges.empty())
    {
       for(size_t i = 0; i < edges.size();++i)
        {
           // QPen linepen(Qt::red);
           // linepen.setWidth(5);
           // p.setPen(linepen);
            if(edges[i][0] != edges[i][1])
            {
                QLine a;
                a.setP1(edges[i][0]->m_point);
                a.setP2(edges[i][1]->m_point);

                p.drawLine(a);
            }
            else{
                p.drawEllipse(edges[i][0]->m_point.rx() ,edges[i][0]->m_point.ry() ,45,45);
            }
        }
    }
    //QRect r(0,0,m_mousePos.x(),m_mousePos.y());
   // p.moveCenter(QPoint(width()/2,height()/2));

   // p.drawEllipse(r);
   // update();

    if(!graph.nodes.empty())
    {
        for(size_t i = 0; i < graph.nodes.size();++i)
        {
            if(clicked_on_node == true && i == clicked_node_index){
                QBrush brushx(Qt::green);
                p.setBrush(brushx);
                p.drawEllipse(graph.nodes[i]->m_point.x() -20,graph.nodes[i]->m_point.y() -20,35,35);
                p.drawText(graph.nodes[i]->m_point.x() -20,graph.nodes[i]->m_point.y() -20,QString::number(i));
            }
            else{

                QBrush brushx(Qt::yellow);
                brushx.setColor(graph.nodes[i]->a);

                p.setBrush(brushx);
                p.drawEllipse(graph.nodes[i]->m_point.x() -20,graph.nodes[i]->m_point.y() -20,40,40);
                p.drawText(graph.nodes[i]->m_point.x() -20,graph.nodes[i]->m_point.y() -20,QString::number(i));
            }
         }
    }
    QWidget::update();
    emit objects_total_signal(graph.nodes.size(),edges.size());
    QWidget::paintEvent(event);
}

void mywidget::mouseReleaseEvent(QMouseEvent *event){
    QWidget::mouseReleaseEvent( event);
    if((tmp.size() == 1 && add_edge_received == true) || (tmp.size() == 1 && add_edge_received == true )){

    }
    else{
    clicked_on_node = false;
    }
}

/*
 *
 *   QVector<Node *> tmp1;
                if(tmp[0]->m_point != tmp[1]->m_point)
                {
                    tmp1.push_back(tmp[1]);
                    tmp1.push_back(tmp[0]);
                }

                if(edges.contains(tmp))
                 {
                    edges.erase(&edges[edges.indexOf(tmp)]);
                    tmp.clear();
                 }
                else if(edges.contains(tmp1))
                {
                    edges.erase(&edges[edges.indexOf(tmp1)]);
                    tmp.clear();
                }

*/









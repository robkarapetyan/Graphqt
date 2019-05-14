#include "mywidget.h"
#include <QDebug>
#include <QVector>
#include "node.h"

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{
   // this->setAutoFillBackground(true);
   // setStyleSheet("background-color: white;");
    // connect(ui->btnCreateNode, &QPushButton::clicked, this, &FormGraph::onBtnCreateNodeClicked);
    QPalette Pal(palette());


    // foni guyne

    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    setMouseTracking(true);
}

void mywidget::Receiving_button(int t)
{
    //this->received = !this->received;
    //vertex_centers = k;
    //qDebug()<< vertex_centers.size();
   //  qDebug()<< "receiving time";
 //qDebug() << this->received;

 switch(t){
    case 1:
     add_vertex_received = !add_vertex_received;
     add_edge_received = false;

     break;
    case 2:
     add_edge_received = !add_edge_received;
      add_vertex_received = false;
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
        nodes[clicked_node_index]->m_point = event->pos();
        update();
    }
 QWidget::mouseMoveEvent( event);
}

void mywidget::mousePressEvent(QMouseEvent *event){

    for(int i = 0; i < nodes.size();++i)
    {
        if(nodes[i]->m_point.x() +20 > event->x() && event->x() > nodes[i]->m_point.x() - 20)
        {
            if(nodes[i]->m_point.y() +20 > event->y() && event->y() > nodes[i]->m_point.y() - 20)
            {
               clicked_on_node = true;
               clicked_node_index = i;
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
        nodes.push_back(b);

      }
       else if(delete_received && clicked_on_node)
        {
          if(nodes.contains(nodes[clicked_node_index]))
           {
              nodes.erase(&nodes[clicked_node_index]);
           }
        /*if(!edges.empty())
        {
          for(int i = 0; i < edges.size(); ++i)
          {
              for(int j = 0; j < edges[i].size(); ++j)
              {
                if(nodes[clicked_node_index]->m_point == edges[i][j]->m_point)
                {
                     abc.push_back(clicked_node_index);
                }
              }
           }
          if(!abc.empty())
            qDebug() << "abc size" << abc.size();
        }
        if(!abc.empty())
        {
          for(int i = 0; i < abc.size(); ++i)
          {
              QVector<QVector<Node *>>::iterator it = &edges[abc[i]];
              edges.erase(it);
          }
        }*/
          //tmp2.erase(&nodes[clicked_node_index]);
           //edges.clear();
         // nodes = tmp2;
          qDebug() << "nodes size" << nodes.size();
          //qDebug() << "nodes size" << tmp2.size();
          qDebug() << "delete arrived xd";
        }
                 //QPainter p(this); p.drawEllipse(m_mousePos.x() - 10,m_mousePos.y() - 10,20,20);
    }
    else if(add_edge_received == true && clicked_on_node == true)
    {
            // check if node is selected and don't let release to work,if selected items are above 2

        /*if(!edges.empty() && edges[edges.size()-1].size() < 2)
        {
            edges[edges.size()-1].push_back(&nodes[clicked_node_index]->m_point);

        }
        else{
            QVector<QPoint *> tmp;
            tmp.push_back(&nodes[clicked_node_index]->m_point);
            edges.push_back(tmp);
        }
        emit objects_total_signal(nodes.size(),edges.size());*/


        qDebug() << "tmp" << tmp.size();
        if(delete_received)
        {
            if(tmp.empty())
            {
                qDebug() << "delete edge arrived";
                tmp.push_back(nodes[clicked_node_index]);
            }
            else if (tmp.size() == 1){

                tmp.push_back(nodes[clicked_node_index]);
                QVector<Node *> tmp1;
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

            }

            if(tmp.size() >= 2){
                        tmp.clear();
                    }
        }
        else if(!delete_received && tmp.size() < 2)
        {
            if(tmp.empty())
            {
                qDebug() << "add edge arrived";
                tmp.push_back(nodes[clicked_node_index]);
            }
            else if (tmp.size() == 1){

                tmp.push_back(nodes[clicked_node_index]);
                QVector<Node *> tmp1;
                if(tmp[0]->m_point != tmp[1]->m_point)
                {
                    tmp1.push_back(tmp[1]);
                    tmp1.push_back(tmp[0]);
                }
                 if(!edges.contains(tmp) && !edges.contains(tmp1))
                  {
                        edges.push_back(tmp);
                        tmp.clear();
                  }

            }

            if(tmp.size() >= 2){
                        tmp.clear();
                    }
        }

        //emit objects_total_signal(nodes.size(),edges.size());

    }
   /* else if(delete_received)
    {
        if(clicked_on_node == true)
        {
            auto it = edges[clicked_node_index];
            edges.erase(&it);
        }
        else if(add_edge_received == true && clicked_on_node == true)
        {
            tmp = edges[clicked_node_index];
        }
    }*/
              // painter.drawEllipse(QPointF(event->x(),event->y()),2,2); emit mouseclickedxy(event->x(),event->y());
}
else if(event->button() == Qt::RightButton)
{


}
else if(event->button() == Qt::MidButton)
{

}

QWidget::mousePressEvent( event);
//qDebug() << edges.size();
}

void mywidget::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setRenderHints(QPainter::HighQualityAntialiasing );

    if(!edges.empty())
    {
       for(int i = 0; i < edges.size();++i)
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

    if(!nodes.empty())
    {
        for(int i = 0; i < nodes.size();++i)
        {
            if(clicked_on_node == true && i == clicked_node_index){
                QBrush brushx(Qt::green);
                p.setBrush(brushx);
                p.drawEllipse(nodes[i]->m_point.x() -20,nodes[i]->m_point.y() -20,35,35);
            }
            else{
                QBrush brushx(Qt::yellow);
                p.setBrush(brushx);
                p.drawEllipse(nodes[i]->m_point.x() -20,nodes[i]->m_point.y() -20,40,40);
            }
         }
    }
    QWidget::update();
    emit objects_total_signal(nodes.size(),edges.size());
    QWidget::paintEvent(event);
}

void mywidget::mouseReleaseEvent(QMouseEvent *event){
    QWidget::mouseReleaseEvent( event);
    if((tmp.size() == 1 && add_edge_received == true) || (tmp.size() == 1 && add_edge_received == true && delete_received)){

    }
    else{
    clicked_on_node = false;
    }
}

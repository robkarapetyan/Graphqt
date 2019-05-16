#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QColor>


class Node : public QObject
{
    Q_OBJECT
public:

    explicit Node(QObject *parent = nullptr);
    QPoint m_point;
    std::vector<Node *> m_nexts;
    QColor a;

    bool selected;
signals:

public slots:

private:
};

#endif // NODE_H

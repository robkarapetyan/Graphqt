#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H
#include <QWidget>

class GraphEditor : public QWidget
{
    Q_OBJECT
public:
    explicit GraphEditor(QWidget *parent = nullptr);

signals:

public slots:
    void addvertex();
    void showsize();

private:
    std::vector<size_t> fabc;
    int a = 0;

};
#endif // GRAPHEDITOR_H

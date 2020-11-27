#ifndef PLOT_NODE_H
#define PLOT_NODE_H



#include "../table.hpp"
#include "node.hpp"
#include <Q3DScatter>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QFile>
#include <QColor>
#include <QGraphicsSimpleTextItem>
#include <ctime>
#include <random>
#include <algorithm>
#include <QtCharts>
#include <Q3DScatter>

class plot_node : public node
{
    Q_OBJECT
private:
    QComboBox combo_box;
    QListWidget list_widget;
    QLabel label;
    QLabel label2;
    QPushButton previewBtn;
public:
    plot_node(int width, int height);
    void plot_function(std::vector<std::string> atributes, std::string label);
    void run() override;
    void preview() override;
    void on_input_changed() override;
public slots:
    void preview_b();
};


#endif // PLOT_NODE_H

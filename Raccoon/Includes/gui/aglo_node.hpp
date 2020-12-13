#ifndef AGLO_NODE_HPP
#define AGLO_NODE_HPP

#include "node.hpp"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QGraphicsView>
#include <QGraphicsScene>


#include "../agglo_cluster.hpp"
class aglo_node : public node
{
    Q_OBJECT
public:
    aglo_node(int width,int height);
    void on_input_changed() override;
    bool run() override;
    void preview() override;
    packet get_msg() override;
    void make_dendogram(QGraphicsScene &scene);

    void serialize(std::ofstream& os) override;
private:
    QComboBox combo_func;
    QLabel label_func;
    QComboBox combo_cfunc;
    QLabel label_cfunc;
    QDoubleSpinBox spin_dist;
    QLabel label_dist;
    QSpinBox spin_num;
    QLabel label_num;
    QPushButton preview_button;

    //dendogram axis
    QGraphicsLineItem x_axis;
    QGraphicsLineItem y_axis;

    //dendogram texts
    std::vector<QGraphicsSimpleTextItem*> x_texts;
    std::vector<QGraphicsSimpleTextItem*> y_texts;

    //dendogram rects
    std::vector<QGraphicsPathItem*> dend_rects;


    QGraphicsView tab_gview;
    QGraphicsScene tab_gscene;

    std::unordered_map<std::string,QColor> cluster_colors;

    agglo_cluster model;
public slots:
    void preview_b();
    void combo_changed(QString str);
    void combo_clust_changed(QString str);
    void dist_changed(double v);
    void num_changed(int v);

    void scene_changed(const QList<QRectF> &region);
};

#endif

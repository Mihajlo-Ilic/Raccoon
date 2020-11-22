#ifndef AGLO_NODE_HPP
#define AGLO_NODE_HPP

#include "node.hpp"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "../agglo_cluster.hpp"
class aglo_node : public node
{
    Q_OBJECT
public:
    aglo_node(int width,int height);
    void on_input_changed() override;
    void run() override;
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

    agglo_cluster model;
public slots:
    void preview_b();
    void combo_changed(QString str);
    void combo_clust_changed(QString str);
    void dist_changed(double v);
    void num_changed(int v);
};

#endif

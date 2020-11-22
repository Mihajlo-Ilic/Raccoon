#ifndef DBSCAN_NODE_HPP
#define DBSCAN_NODE_HPP
#include "node.hpp"
#include "../dbscan.hpp"
#include<QSpinBox>
#include<QComboBox>
class dbscan_node : public node
{
    Q_OBJECT
private:
    QPushButton preview_btn;
    QLabel min_neighbor_lbl;
    QSpinBox min_neighbor_sb;

    QLabel eps_lbl;
    QDoubleSpinBox eps_sb;
    QComboBox metric_cb;
    QLabel metric_lbl;
    dbscan clasterizer;
public:
    dbscan_node(int width, int height);
    void run() override;
    void on_input_changed() override;
    ~dbscan_node() = default;
public slots:
    void preview_b();

    void metric_change(QString str);
    void eps_change(double eps);
    void num_change(int n);

};

#endif // DBSCAN_NODE_HPP

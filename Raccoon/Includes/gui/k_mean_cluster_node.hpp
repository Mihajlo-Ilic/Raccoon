
#ifndef K_MEAN_CLUSTER_NODE_HPP
#define K_MEAN_CLUSTER_NODE_HPP

#include "node.hpp"
#include "../k_mean_cluster.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>

class k_mean_cluster_node : public node
{
    Q_OBJECT
private:
    QComboBox distance_box;
    QPushButton previewBtn;
    QSpinBox k_neighbours;
    QLabel distance_label;
    QLabel k_neighbours_label;
    k_mean_cluster model;

    std::unordered_map<std::string, QColor> cluster_colors;
public:
    k_mean_cluster_node(int width, int height);
    void run() override;
    void on_input_changed() override;
    packet get_msg() override;
    void preview() override;
public slots:
    void preview_b();
    void combo_changed(QString str);
    void sb_changed(int val);
};

#endif

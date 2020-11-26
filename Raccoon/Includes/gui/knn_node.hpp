#ifndef KNN_NODE_HPP
#define KNN_NODE_HPP


#include "node.hpp"
#include "../knn.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>

class knn_node : public node
{
    Q_OBJECT
private:
    QComboBox distance_box;
    QPushButton previewBtn;
    QSpinBox k_neighbours;
    QLabel distance_label;
    QLabel k_neighbours_label;
    knn classifier;
public:
    knn_node(int width, int height);
    void run() override;
    void on_input_changed() override;
    packet get_msg() override;
public slots:
    void preview_b();
    void combo_changed( QString str);
    void k_changed(int v);
};

#endif // KNN_NODE_HPP

#ifndef DECISION_TREE_NODE_HPP
#define DECISION_TREE_NODE_HPP
#include "node.hpp"
#include "../decision_tree.hpp"
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>

class decision_tree_node : public node
{
    Q_OBJECT;
private:
    QPushButton preview_btn;
    QLabel min_row_lbl;
    QSpinBox min_row_sb;
    QLabel max_depth_lbl;
    QSpinBox max_depth_sb;
    QLabel min_clean_lbl;
    QDoubleSpinBox min_clean_sb;
    QComboBox clean_func;
    QLabel clean_func_lbl;
    decision_tree tree;
public slots:
    void preview_b();
    void combo_changed(QString str);
    void clean_changed(double v);
    void depth_changed(int v);
    void row_changed(int v);
public:
    decision_tree_node(int width, int height);
    void on_input_changed() override;
    void run() override;
    packet get_msg() override;

};

#endif

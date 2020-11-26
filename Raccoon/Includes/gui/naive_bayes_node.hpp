#ifndef NB_NODE_HPP
#define NB_NODE_HPP


#include "node.hpp"
#include "../naive_bayes.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>

class nb_node : public node
{
    Q_OBJECT
private:
    QPushButton previewBtn;
    QDoubleSpinBox alpha_sbox;
    QLabel alpha_label;
    QCheckBox table_checkbox;
    QLabel table_label;

    naive_bayes classifier;
public:
    nb_node(int width, int height);
    void run() override;
    void on_input_changed() override;
    packet get_msg() override;
public slots:
    void preview_b();
};

#endif


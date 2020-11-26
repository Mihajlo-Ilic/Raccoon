#ifndef PARTITION_NODE_HPP
#define PARTITION_NODE_HPP
#include "node.hpp"
#include <QSpinBox>
#include <QPushButton>

class partition_node : public node
{
    Q_OBJECT
public:
    partition_node(int width, int height);
    void on_input_changed() override;
    void run() override;
    packet get_msg() override;
private:
    QSpinBox sb_percent;
    QSpinBox sb_random;
    QPushButton preview_btn;
    QLabel lbl_percent;
    QLabel lbl_random;

public slots:
    void preview_b();
    void changed(int i);
};

#endif // PARTITION_NODE_HPP

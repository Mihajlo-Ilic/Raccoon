#ifndef STATS_NODE_HPP
#define STATS_NODE_HPP
#include "node.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

class stats_node : public node
{
    Q_OBJECT
public:
    stats_node(int width, int height);
    void on_input_changed() override;
    void run() override;
public slots:
    void preview_b();
private:
    QLabel stats_lbl;
    QListWidget stats;
    QPushButton preview_btn;
};

#endif // STATS_NODE_HPP

#ifndef FILTER_NODE_HPP
#define FILTER_NODE_HPP

#include "node.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

class filter_node : public node
{
    Q_OBJECT
public:
    filter_node(int width, int height);
    void on_input_changed() override;
    void run() override;
public slots:
    void preview_b();
    void list_changed();
private:
      QLabel columns_lbl;
      QListWidget columns;
      QPushButton preview_btn;
};

#endif // FILTER_NODE_HPP

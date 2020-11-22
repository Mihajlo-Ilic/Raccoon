#ifndef NORMALIZATION_NODE_HPP
#define NORMALIZATION_NODE_HPP

#include "node.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

class normalization_node : public node
{
    Q_OBJECT
public:
    normalization_node(int width, int height);
    void on_input_changed() override;
    void run() override;
public slots:
    void preview_b();
    void list_changed();
private:
      QListWidget columns;
      QPushButton preview_btn;
};

#endif // NORMALIZATION_NODE_HPP

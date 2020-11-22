#ifndef STANDARDIZATION_NODE_HPP
#define STANDARDIZATION_NODE_HPP

#include "node.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

class standardization_node : public node
{
    Q_OBJECT

public:
    standardization_node(int width, int height);
    void on_input_changed() override;
    void run() override;
public slots:
    void preview_b();
    void list_changed();
private:
      QListWidget columns;
      QPushButton preview_btn;
};

#endif // STANDARDIZATION_NODE_HPP

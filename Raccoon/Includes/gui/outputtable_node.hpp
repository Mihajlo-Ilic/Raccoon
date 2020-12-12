#ifndef OUTPUTTABLE_NODE_HPP
#define OUTPUTTABLE_NODE_HPP

#include "node.hpp"
#include <QPushButton>
#include <QObject>

class outputTable_node : public node
{
    Q_OBJECT
public:
    outputTable_node(int width,int height);
    void on_input_changed() override;
    bool run() override;
private:
    QPushButton previewBtn;
public slots:
    void preview_b();
};

#endif // OUTPUTTABLE_NODE_HPP

#ifndef OUTPUTTABLE_NODE_HPP
#define OUTPUTTABLE_NODE_HPP

#include "node.hpp"
#include <QPushButton>
#include <QObject>
#include <QLineEdit>

class outputTable_node : public node
{
    Q_OBJECT
public:
    outputTable_node(int width,int height);
    void on_input_changed() override;
    bool run() override;
    void preview() override;

    void serialize(std::ofstream& os) override;
private:
    QLineEdit path_line;
    QPushButton file_browser_button;
public slots:
    void preview_b();
};

#endif // OUTPUTTABLE_NODE_HPP

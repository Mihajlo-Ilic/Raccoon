#ifndef CATEGORICALTOBINNARY_NODE_HPP
#define CATEGORICALTOBINNARY_NODE_HPP

#include "node.hpp"
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QObject>

class categoricalToBinnary_node : public node
{
    Q_OBJECT
public:
    categoricalToBinnary_node(int width,int height);
    void on_input_changed() override;
    bool run() override;
    packet get_msg() override;

    void serialize(std::ofstream& os) override;
private:
    QLabel selectLabel;
    QListWidget listWidget;
    QPushButton previewBtn;
public slots:
    void list_changed(QListWidgetItem *item);
    void preview_b();
};

#endif // CATEGORICALTOBINNARY_NODE_HPP

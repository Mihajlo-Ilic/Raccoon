#ifndef CSV_NODE_HPP
#define CSV_NODE_HPP

#include "node.hpp"

#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>

class csv_node : public node
{
    Q_OBJECT
private:
    QLineEdit edit;
    QPushButton browseBtn;
    QPushButton loadBtn;
    QListWidget colomns;
    QPushButton previewBtn;
    QPushButton runBtn;
    QTableWidget qtable;
    QComboBox role_box;
    QComboBox type_box;
    QLabel role_l;
    QLabel type_l;

public:
    csv_node(int width, int height);
    bool run() override;
    packet get_msg() override;
    virtual void on_input_changed() override;
public slots:
    void load();
    void browse();
    void preview_slot();
    void list_changed();
    void type_changed(QString ty);
    void role_changed(QString rol);
};

#endif // CSV_NODE_HPP

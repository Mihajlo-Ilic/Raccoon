#ifndef BINNING_NODE_HPP
#define BINNING_NODE_HPP

#include "node.hpp"
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QObject>

class binning_node : public node
{
    Q_OBJECT
private:
    QListWidget listWidget;
    QComboBox binningMethod;
    QComboBox binningShuffle;
    QLabel binningMethodLabel;
    QLabel binningShuffleLabel;
    QPushButton previewBtn;
public:
    void on_input_changed() override;
    binning_node(int width, int height);
    void run() override;

    packet get_msg() override;
public slots:
    void changed(const QString& s);
    void preview_b();
    void list_changed(QListWidgetItem *);

};

#endif // BINNING_NODE_HPP

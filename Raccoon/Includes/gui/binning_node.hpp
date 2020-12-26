#ifndef BINNING_NODE_HPP
#define BINNING_NODE_HPP

#include "node.hpp"
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QObject>
#include <QSpinBox>

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
    QLabel set_label;
    QSpinBox sets_spin;
public:
    void on_input_changed() override;
    binning_node(int width, int height);
    bool run() override;

    packet get_msg() override;

    void serialize(std::ofstream& os) override;
public slots:
    void changed(const QString& s);
    void preview_b();
    void list_changed(QListWidgetItem *);
};

#endif // BINNING_NODE_HPP

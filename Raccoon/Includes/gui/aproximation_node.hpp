#ifndef APROXIMATION_NODE_HPP
#define APROXIMATION_NODE_HPP

#include "node.hpp"
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QObject>
#include <QLineEdit>

class aproximation_node : public node
{
    Q_OBJECT
public:
    aproximation_node(int width,int height);
    void on_input_changed() override;
    bool run() override;
private:
    QComboBox aproximationCombo;
    QLabel aproximationLabel;
    QListWidget columns;
    QPushButton previewBtn;
    QLineEdit editAprox;
public slots:
    void preview_b();
    void changed();
    void help_func(const QString &text);
    void list_changed(QListWidgetItem *item);

};

#endif // APROXIMATION_NODE_HPP

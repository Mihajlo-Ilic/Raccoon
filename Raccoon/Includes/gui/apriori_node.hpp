#ifndef APRIORI_NODE_HPP
#define APRIORI_NODE_HPP

#include "node.hpp"
#include "../apriori.hpp"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QObject>

class apriori_node : public node
{
    Q_OBJECT
public:
    apriori_node(int width,int height);
    void on_input_changed() override;
    bool run() override;
    void preview() override;

    void serialize(std::ofstream& os) override;
private:
    QLabel support_label;
    QDoubleSpinBox support_spin;

    QLabel confidence_label;
    QDoubleSpinBox confidence_spin;

    QPushButton previewBtn;

    apriori model;

public slots:
    void preview_b();
};



#endif // APRIORI_NODE_HPP

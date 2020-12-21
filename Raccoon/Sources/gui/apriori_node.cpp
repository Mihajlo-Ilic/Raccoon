#include "../../Includes/gui/apriori_node.hpp"


apriori_node::apriori_node(int width, int height):node(250,250,1,1)
{
    header_text.setText("APRIORI NODE");

    support_spin.setParent(&body);
    support_label.setParent(&body);
    confidence_spin.setParent(&body);
    confidence_label.setParent(&body);

    support_label.setText("min support");
    confidence_label.setText("min confidence");

    support_spin.setGeometry(geometry().x() + 10,geometry().y() + 10,110,30);
    support_label.setGeometry(geometry().x() + 130,geometry().y() + 10,110,30);
    confidence_spin.setGeometry(geometry().x() + 10,geometry().y() + 50,110,30);
    confidence_label.setGeometry(geometry().x() + 130,geometry().y() + 50,110,30);

    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));

    connect(&support_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){
        model.set_support(d);
    });

    connect(&confidence_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){
        model.set_confidence(d);
    });
}

void apriori_node::serialize(std::ofstream &os)
{
    os<<"-n apriori"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" support="<<support_spin.value()<<std::endl;
    os<<" confidence="<<confidence_spin.value()<<std::endl;
}

void apriori_node::preview_b()
{
    preview();
}

void apriori_node::on_input_changed()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("support",INPUT_COLUMN,CONTINUOUS);
    msg.add_column("confidence",INPUT_COLUMN,CONTINUOUS);
    outputs[0]->send_packet(msg);
    needs_update = true;
}

bool apriori_node::run()
{
    t = inputs[0]->get_table();
    t = model.fit(t);
    outputs[0]->send_data(t);
    return true;
}

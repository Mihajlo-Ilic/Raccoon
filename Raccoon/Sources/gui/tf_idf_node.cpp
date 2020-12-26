#include "../../Includes/gui/tf_idf_node.hpp"

tf_idf_node::tf_idf_node(int width,int height) : node(width,height,1,1)
{
    header_text.setText("TF-IDF NODE");

    previewBtn.setParent(&body);

    previewBtn.setText("preview");

    previewBtn.setGeometry(geometry().x() + 50,geometry().y() + 40,70,30);
}

void tf_idf_node::on_input_changed() {
}

bool tf_idf_node::run() {
    t=inputs[0]->get_table();
    tf_idf(t);
    outputs[0]->send_data(t);
    return true;
}

void tf_idf_node::preview_b() {
    tf_idf(t);
    preview();
}

void tf_idf_node::serialize(std::ofstream &os)
{
    os<<"-n tf_idf"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
}

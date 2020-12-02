#include "../../Includes/gui/tf_idf_node.hpp"

tf_idf_node::tf_idf_node(int width,int height) : node(width,height,1,0)
{
    header_text.setText("TF-IDF NODE");

    previewBtn.setParent(&body);

    previewBtn.setText("preview");

    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);
}

void tf_idf_node::on_input_changed() {
    t = inputs[0]->get_table();
}

void tf_idf_node::run() {
    outputs[0]->send_data(t);
}

void tf_idf_node::preview_b() {
    tf_idf(t);
    preview();
}
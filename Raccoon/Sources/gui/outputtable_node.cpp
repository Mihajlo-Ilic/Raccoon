#include "../../Includes/gui/outputtable_node.hpp"

outputTable_node::outputTable_node(int width,int height) : node(width,height,1,0)
{
    header_text.setText("OUTPUT NODE");

    previewBtn.setParent(&body);

    previewBtn.setText("preview");

    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);
}

void outputTable_node::on_input_changed()
{

}

bool outputTable_node::run()
{
    return true;
}

void outputTable_node::preview_b() {

}

void outputTable_node::serialize(std::ofstream &os)
{
    os<<"-n output_node"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
}

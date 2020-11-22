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

void outputTable_node::run()
{

}

void outputTable_node::preview_b() {

}

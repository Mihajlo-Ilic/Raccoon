#include "../../Includes/gui/categoricaltobinnary_node.hpp"
#include "../../Includes/transformations.hpp"

categoricalToBinnary_node::categoricalToBinnary_node(int width,int height) : node(width,height,1)
{
    header_text.setText("CAT. TO BINNARY NODE");

    selectLabel.setParent(&body);
    listWidget.setParent(&body);
    previewBtn.setParent(&body);

    selectLabel.setText("Select attribute:");
    previewBtn.setText("preview");
    listWidget.setSelectionMode(QAbstractItemView::MultiSelection);

    selectLabel.setGeometry(geometry().x() + 10,geometry().y() + 10,100,20);
    listWidget.setGeometry(geometry().x() + 10,geometry().y() + 40,230,150);
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));
    connect(&listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(list_changed()));

}

void categoricalToBinnary_node::on_input_changed()
{
    t=inputs[0]->get_table();
    listWidget.clear();
    for(const auto& it:inputs[0]->get_table().col_names())
        listWidget.addItem(QString::fromStdString(it));
    needs_update=true;
}

void categoricalToBinnary_node::run()
{

}

void categoricalToBinnary_node::list_changed()
{
    t=inputs[0]->get_table();
    for(const auto& it:listWidget.selectedItems())
        categorical_to_binary(t,it->text().toStdString());

    outputs[0]->send_data(t);

    needs_update=false;
}


void categoricalToBinnary_node::preview_b() {
    preview();
}

#include "../../Includes/gui/normalization_node.hpp"
#include "../../Includes/transformations.hpp"

normalization_node::normalization_node(int width, int height) : node(width, height, 1)
{
    header_text.setText("NORMALIZE");
    columns.setParent(&body);
    columns.setGeometry(10, 10, 230, 150);
    columns.setSelectionMode(QAbstractItemView::MultiSelection);

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&columns, SIGNAL(itemSelectionChanged()), this, SLOT(list_changed()));
}

void normalization_node::on_input_changed()
{
    columns.clear();
    for(const auto& it:inputs[0]->get_table().col_names())
        columns.addItem(QString::fromStdString(it));
}

void normalization_node::run()
{

}

void normalization_node::preview_b() {
    preview();
}

void normalization_node::list_changed()
{
    table curr=inputs[0]->get_table();
    for(const auto& it:columns.selectedItems())
        normalize(curr,it->text().toStdString());
    t=curr;

    outputs[0]->send_data(t);

    needs_update=false;
}

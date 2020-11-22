#include "../../Includes/gui/standardization_node.hpp"
#include "../../Includes/transformations.hpp"

standardization_node::standardization_node(int width, int height) : node(width, height, 1)
{
    header_text.setText("STANDARDIZE");
    columns.setParent(&body);
    columns.setGeometry(10, 10, 230, 150);
    columns.setSelectionMode(QAbstractItemView::MultiSelection);

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&columns, SIGNAL(itemSelectionChanged()), this, SLOT(list_changed()));
}

void standardization_node::preview_b() {
    preview();
}

void standardization_node::on_input_changed()
{
    columns.clear();
    for(const auto& it:inputs[0]->get_table().col_names())
        columns.addItem(QString::fromStdString(it));
    needs_update=true;
}

void standardization_node::run()
{

}

void standardization_node::list_changed()
{
    table curr=inputs[0]->get_table();
    for(const auto& it:columns.selectedItems())
        standardize(curr,it->text().toStdString());
    t=curr;

    outputs[0]->send_data(t);

    needs_update=false;
}

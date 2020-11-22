#include "../../Includes/gui/decision_tree_node.hpp"

decision_tree_node::decision_tree_node(int width, int height) : node(width, height, 2) {
    header_text.setText("DECISION TREE");

    preview_btn.setText("preview");
    preview_btn.setParent(&body);
    preview_btn.setGeometry(170, 180, 70, 30);

    min_row_lbl.setParent(&body);
    min_row_lbl.setText("Min. # of rows in node:");
    min_row_lbl.setGeometry(5,5,100,30);
    min_row_sb.setParent(&body);
    min_row_sb.setGeometry(140, 5, 70, 30);
    min_row_sb.setMinimum(1);

    max_depth_lbl.setParent(&body);
    max_depth_lbl.setText("Max. depth of tree:");
    max_depth_lbl.setGeometry(5, 40, 120, 30);
    max_depth_sb.setParent(&body);
    max_depth_sb.setGeometry(140, 40, 70, 30);
    max_depth_sb.setMinimum(0);

    min_clean_lbl.setParent(&body);
    min_clean_lbl.setText("Min. node clean:");
    min_clean_lbl.setGeometry(5, 75, 120, 30);
    min_clean_sb.setParent(&body);
    min_clean_sb.setGeometry(140, 75, 70, 30);
    min_clean_sb.setMaximum(1);
    min_clean_sb.setMinimum(0);


    clean_func_lbl.setParent(&body);
    clean_func_lbl.setText("Clean function:");
    clean_func_lbl.setGeometry(5, 110, 100, 30);
    clean_func.setParent(&body);
    clean_func.setGeometry(140, 110, 100, 30);
    clean_func.addItem("Entropy");
    clean_func.addItem("Gini index");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&clean_func,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&min_clean_sb,SIGNAL(valueChanged(double)),this,SLOT(clean_changed(double)));
    connect(&max_depth_sb,SIGNAL(valueChanged(int)),this,SLOT(depth_changed(int)));
    connect(&min_row_sb,SIGNAL(valueChanged(int)),this,SLOT(row_changed(int)));
}
#include<iostream>
void decision_tree_node::on_input_changed()
{
    t=inputs[0]->get_table();
}

void decision_tree_node::run()
{

}

void decision_tree_node::preview_b() {
    t.pop("partition");
    tree.fit(t);
    table test=inputs[1]->get_table();
    test.pop("partition");
    test.pop(test.get_target());
    t=tree.classify(test);
    preview();
}

void decision_tree_node::combo_changed(QString str)
{
    if(str.toStdString()=="Entropy")
        tree.set_func(entropy);
    if(str.toStdString()=="Gini index")
        tree.set_func(gini);
}

void decision_tree_node::clean_changed(double v)
{
    tree.set_clean(v);
}

void decision_tree_node::depth_changed(int v)
{
    tree.set_max_depth(v);
}

void decision_tree_node::row_changed(int v)
{
    tree.set_min_row(v);
}


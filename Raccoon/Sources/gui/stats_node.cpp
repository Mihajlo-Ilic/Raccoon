#include "../../Includes/gui/stats_node.hpp"
#include "../../Includes/stats.hpp"
stats_node::stats_node(int width, int height) : node(width, height, 1,0)
{
    header_text.setText("TABLE STATS");

    stats_lbl.setParent(&body);
    stats_lbl.setGeometry(10, 0, 200, 30);
    stats_lbl.setText("Select stats to view:");

    stats.setParent(&body);
    stats.setGeometry(10, 30, 230, 140);
    stats.setSelectionMode(QAbstractItemView::MultiSelection);
    stats.addItem("min. element");
    stats.addItem("max. element");
    stats.addItem("sum");
    stats.addItem("mean");
    stats.addItem("range");
    stats.addItem("standard deviation");
    stats.addItem("variance");
    stats.addItem("unique");
    stats.addItem("valid");

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
}

void stats_node::on_input_changed()
{
     t = inputs[0]->get_table();
}

void stats_node::run()
{
    t = inputs[0]->get_table();
}

void stats_node::preview_b() {
    preview();
}
#include <set>
#include <QDialog>
#include <QHBoxLayout>
void stats_node::preview() {
    stat_table = table{};

   for(auto sel : stats.selectedItems()) {
       stat_table.push(sel->text().toStdString());
   }

   for(unsigned i = 0; i<t.col_names().size(); i++) {
       stat_table.push_row();
       stat_table[i].set_name(t.col_names()[i]);
   }

   //Fill the table
   for(auto stat : stat_table.col_names()) {
        if(stat == "min. element") {
            for(auto col : t.col_names()) {
                       auto min = std::min_element(t[col].cbegin(), t[col].cend());
                       stat_table[stat][stat_table.row_by_name(col)] = *min;
                }
        }
        else if(stat == "max. element") {
            for(auto col : t.col_names()) {
                       auto max = std::max_element(t[col].cbegin(), t[col].cend());
                       stat_table[stat][stat_table.row_by_name(col)] = *max;
                }
        }
        else if(stat == "range") {
            for(auto col : t.col_names()) {
                auto min = std::min_element(t[col].cbegin(), t[col].cend());
                auto max = std::max_element(t[col].cbegin(), t[col].cend());
                stat_table[stat][stat_table.row_by_name(col)] = entry("[" + min->get_string() + " - " + max->get_string() + "]");
            }
        }
        else if(stat == "sum") {
            for(auto col: t.col_names()) {
                if(t[col].type == NOMINAL)
                    continue;
                auto sum = std::accumulate(t[col].cbegin(), t[col].cend(), entry(0.0));
                stat_table[stat][stat_table.row_by_name(col)] = sum;
            }
        }
        else if(stat == "mean") {
            for(auto col: t.col_names()) {
                if(t[col].type == NOMINAL)
                    continue;

                stat_table[stat][stat_table.row_by_name(col)] = entry(column_mean(t[col]));
            }
        }
        else if(stat == "standard deviation") {
           for(auto col : t.col_names()) {
               if(t[col].type == NOMINAL)
                   continue;

               stat_table[stat][stat_table.row_by_name(col)] = entry(column_stdev(t[col]));
           }
        }
        else if(stat == "variance") {
            for(auto col : t.col_names()) {
                if(t[col].type == NOMINAL)
                    continue;

                stat_table[stat][stat_table.row_by_name(col)] = entry(column_variance(t[col]));
            }
        }
        else if(stat == "unique") {
            for(auto col : t.col_names()) {
                stat_table[stat][stat_table.row_by_name(col)] = entry(t[col].unique().size());
            }
        }
        else if(stat == "valid") {
            for(auto col : t.col_names()) {
                stat_table[stat][stat_table.row_by_name(col)] = entry(t.where(col, [] (auto e) { return e.get_string() != "n/a";}).size());
            }
        }
   }


   QDialog *tablePreview=new QDialog();
   tablePreview->setGeometry(400,400,500,500);
   tablePreview->setModal(true);

   QHBoxLayout *dialog_layout=new QHBoxLayout();
   tablePreview->setLayout(dialog_layout);

   QTabWidget *tabs=new QTabWidget;
   dialog_layout->addWidget(tabs);

   QTableWidget data_table;
   make_QTable(data_table,stat_table);

   tabs->addTab(&data_table, "Table stats");

   tablePreview->exec();
}

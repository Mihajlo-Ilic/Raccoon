#include "../Includes/decision_tree.hpp"
#include <numeric>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <QRect>
#include <queue>
#include <thread>
#include <mutex>
#define X_SPACING 100
#define Y_SPACING 100
#define NODE_WIDTH 150
#define NODE_HEIGHT 200
rule::rule() {

}


std::string rule::get_colname() {
    return colname;
}

rule_continuous::rule_continuous(std::string column, entry val) : rule() {
    this->val = val;
    colname = column;
}

rule_categorical::rule_categorical(std::string column) : rule() {
    colname = column;
}

int rule_continuous::get_child(entry e)
{
    if(e.get_double() < val)
        return 0;
    else return 1;
}

int rule_categorical::get_child(entry e){
    return vals[e.get_string()];
}

std::vector<std::string> rule_categorical::get_string()
{
    std::vector<std::string> res;
    for(const auto &it:vals) {
        res.push_back(get_colname() + ": " + it.first);
    }
    return res;
}

std::vector<std::string> rule_continuous::get_string()
{
    std::vector<std::string> res(2);
    res[0] = get_colname() + " < " + std::to_string(val);
    res[1] = get_colname() + " >= " + std::to_string(val);
    return res;
}

void rule_categorical::add_value(entry val)
{
    vals.insert(std::make_pair(val.get_string(), current_index++));
}

void rule_continuous::add_value(entry val) {
    this->val=val;
}



#include<iostream>
tree_node::tree_node(std::vector<int>&& idxs, std::function<double(std::unordered_map<std::string, int> &, int)> metric, unsigned depth,decision_tree *p) {
    this->idxes = idxs;
    this->clean_metric = metric;
    this->depth = depth;
    this->parent=p;
    this->is_leaf=false;
    if(this->parent->levels.find(depth) == this->parent->levels.end())
        this->parent->levels[depth] = 1;
    else
        this->parent->levels[depth]++;

   this->n_rows = idxs.size();

   //Form "n_classes" map
    for(int i = 0; i < n_rows; i++) {
        auto cl_name = parent->training_table[parent->training_table.get_target()][idxs[i]].get_string();
        if(n_classes.find(cl_name) == n_classes.end())
            n_classes.insert(std::make_pair(cl_name, 1));
        else
            n_classes[cl_name]++;
    }

    this->score = clean_metric(n_classes, n_rows);
    if(depth >= parent->max_depth || n_rows <= parent->min_rows || score <= parent->min_clean || n_rows ==1)
    {
        this->is_leaf = true;
        //most frequent class in node
        std::string cl_freq = "n/a";
        int max = 0;
        for(const auto & cl : n_classes)
            if(cl.second > max) {
                max = cl.second;
                cl_freq = cl.first;
            }

        this->class_name = entry(cl_freq);
    }

}

const auto processor_count = std::thread::hardware_concurrency();
std::mutex split_mutex;
#include<iostream>
//TESTING
/*
void tree_node::thread_func(std::vector<std::string> col_names, double &split_val, double &gain_optimal, std::string &split_col) {
    for(unsigned i = 0u; i<col_names.size(); i++) {
        auto curr_col = col_names[i];

        if(t[curr_col].role == TARGET)
            continue;

        if(t[curr_col].type == CONTINUOUS) {
            double curr_val=0;
            double d = split_continuous(curr_col, curr_val);
            //LOCK THE MUTEX
            split_mutex.lock();
            if(gain_optimal < d) {
                 gain_optimal = d;
                 split_col = curr_col;
                 split_val=curr_val;
            }
            split_mutex.unlock();
            //UNLOCK THE MUTEX
        }

        if(t[curr_col].type == NOMINAL) {
            double d = split_categorical(curr_col);
            //LOCK THE MUTEX
            split_mutex.lock();
            if(gain_optimal < d) {
                 gain_optimal = d;
                 split_col = curr_col;
            }
            //UNLOCK THE MUTEX
            split_mutex.unlock();
        }
    }
}


void tree_node::split_concurrent() {
    if(this->is_leaf)
        return;
    //these values need to be locked
    double split_val = 0;
    double gain_optimal = 0;
    std::string split_col_optimal = t.col_names()[0];
    //determines number of columns for each thread to process
    int k_col = t.col_n()/processor_count;
    std::vector<std::thread> threads;
    for(unsigned i = 0u; i < processor_count; i++) {

        //vector containing names of i-th k_col columns
        //ugly implementation but the best we could think of right now
        std::vector<std::string> i_colnames;
        for(unsigned k = k_col*i; k < (unsigned)t.col_n() && k < k_col*(1+i); k++)
            i_colnames.push_back(t.col_names()[k]);

        //threads.emplace_back(&tree_node::thread_func, this, i_colnames, split_val, gain_optimal, split_col_optimal);
        threads.emplace_back([&] () {this->thread_func(i_colnames, split_val, gain_optimal, split_col_optimal);});
    }

    for(auto & th : threads) {
        th.join();
    }

    //Make a split rule based on type of split column, recursively call split_concurrent for children of this node
    if(t[split_col_optimal].type == CONTINUOUS) {
        split_rule = new rule_continuous(split_col_optimal, entry(split_val));
        table t_lt = t[t.where(split_col_optimal, [split_val] (auto e) {return e.get_double() < split_val;})];
        table t_gte = t[t.where(split_col_optimal, [split_val] (auto e) {return e.get_double() >= split_val;})];
        t_lt.pop(split_col_optimal);
        t_gte.pop(split_col_optimal);
        children.push_back(new tree_node(t_lt, clean_metric, depth+1,this->parent));
        children.push_back(new tree_node(t_gte, clean_metric, depth+1,this->parent));

    }
    if(t[split_col_optimal].type == NOMINAL) {
        split_rule = new rule_categorical(split_col_optimal);
        for(const auto &u : t[split_col_optimal].unique()) {
            split_rule->add_value(u);
            table t_i = t[t.where(split_col_optimal, [u] (auto e) {return u == e;})];
            t_i.pop(split_col_optimal);
            children.push_back(new tree_node(t_i, clean_metric, depth+1,this->parent));
        }
    }

    for(auto it:children) {
            it->split();
    }
}
*/


void tree_node::split() {
    if(this->is_leaf)
        return;

    table & t = parent->training_table;
    std::vector<std::string> colnames = t.col_names();
    std::string split_col_optimal = colnames[0];

    double gain_optimal = 0;
    double split_val=0;
    //Determine which column to split by
    for(unsigned i = 0u; i < colnames.size(); i++) {
         auto curr_col = colnames[i];
         if(t[curr_col].role==TARGET)
             continue;

         if(t[curr_col].type == column_type::CONTINUOUS) {
               double curr_val=0;
               double d = split_continuous(curr_col, curr_val);
               if(gain_optimal < d) {
                    gain_optimal = d;
                    split_col_optimal = curr_col;
                    split_val=curr_val;
               }
                //std::cout<<"najbolji iz "<<curr_col<<" je "<<curr_val<<" sa "<<d<<std::endl;
         }
         if(t[curr_col].type == column_type::NOMINAL) {
             double d = split_categorical(curr_col);
             if(gain_optimal < d) {
                  gain_optimal = d;
                  split_col_optimal = curr_col;
             }
            //std::cout<<" split po "<<curr_col<<" je "<<d<<std::endl;
         }
    }

    //std::cout<<"\n\nUZIMAM: "<<split_col_optimal<<" ZA SPLIT"<<std::endl;

    if(t[split_col_optimal].type == CONTINUOUS) {
        split_rule = new rule_continuous(split_col_optimal, entry(split_val));
        std::vector<int> vec_lte;
        std::vector<int> vec_gt;
        for(unsigned i = 0; i < idxes.size(); i++) {
            if(t[split_col_optimal][idxes[i]].get_double() <= split_val)
                vec_lte.push_back(idxes[i]);
            else
                vec_gt.push_back(idxes[i]);
        }
        children.push_back(new tree_node(std::move(vec_lte), clean_metric, depth+1, parent));
        children.push_back(new tree_node(std::move(vec_gt),clean_metric, depth+1, parent));

        /*table t_lt = t[t.where(split_col_optimal, [split_val] (auto e) {return e.get_double() < split_val;})];
        table t_gte = t[t.where(split_col_optimal, [split_val] (auto e) {return e.get_double() >= split_val;})];
        t_lt.pop(split_col_optimal);
        t_gte.pop(split_col_optimal);
        children.push_back(new tree_node(t_lt, clean_metric, depth+1,this->parent));
        children.push_back(new tree_node(t_gte, clean_metric, depth+1,this->parent));*/

    }
    if(t[split_col_optimal].type == NOMINAL) {
        split_rule = new rule_categorical(split_col_optimal);
        //we need a new child for each unique value
        std::unordered_map<std::string, std::vector<int>> unique_vals;
        for(unsigned i = 0; i<idxes.size(); i++) {
            auto val_name = parent->training_table[split_col_optimal][idxes[i]].get_string();
            if(unique_vals.find(val_name) == unique_vals.end()) {
                std::vector<int> tmp;
                tmp.push_back(idxes[i]);
                unique_vals.insert(std::make_pair(val_name, tmp));
                split_rule->add_value(val_name);
            }
            else
                unique_vals[val_name].push_back(idxes[i]);
        }

        for(auto & val : unique_vals)
            children.push_back(new tree_node(std::move(val.second), clean_metric, depth + 1, parent));

    }

    for(auto it:children) {
            it->split();
    }
}

double tree_node::split_categorical(std::string col_name) {
    double parent_clean = clean_metric(n_classes, n_rows);

    double gain_sum = 0;
    double split_info = 0;

    //KEY - Name of unique value from the column "col_name"
    //VALUE - Vector of row indexes whose value for "col_name" equals the key
    std::unordered_map<std::string, std::vector<int>> unique_vals;

    for(unsigned i = 0; i<idxes.size(); i++) {
        auto val_name = parent->training_table[col_name][idxes[i]].get_string();
        if(unique_vals.find(val_name) == unique_vals.end()) {
            std::vector<int> tmp;
            tmp.push_back(idxes[i]);
            unique_vals.insert(std::make_pair(val_name, tmp));
        }
        else
            unique_vals[val_name].push_back(idxes[i]);
    }

    for(const auto & val : unique_vals) {
        //Each vector of indexes (val.second) forms a "sub-table"; we need to check how clean each sub-table is
        std::unordered_map<std::string, int> val_classes;
        for(const auto & it : val.second) {
            auto class_name = parent->training_table[parent->training_table.get_target()][it].get_string();
            if(val_classes.find(class_name) == val_classes.end())
                val_classes.insert(std::make_pair(class_name, 1));
            else
                val_classes[class_name]++;
        }

        int n_i = val.second.size();
        gain_sum += ((double)n_i/n_rows)*clean_metric(val_classes, n_i);
        split_info += -((double)n_i/n_rows)*log2(((double)n_i/n_rows));
    }

    return (parent_clean - gain_sum)/split_info;

}

/*
double tree_node::split_categorical(std::string col_name) {
    double parent_clean = clean_metric(t);
    double n = t.row_n();
    auto unique_vals = t[col_name].unique();
    double gain_sum = 0;
    double split_info = 0;
    for(const auto &val : unique_vals) {
        table t_i = t[t.where(col_name, [val] (auto e) {return val == e;})];
        int n_i = t_i.row_n();
        gain_sum += ((double)n_i/n)*clean_metric(t_i);
        split_info += -((double)n_i/n)*log2(((double)n_i/n));
    }
    return (parent_clean - gain_sum)/split_info;
}*/

double tree_node::split_continuous(std::string col_name, double &split_val) {
    double parent_clean = clean_metric(n_classes, n_rows);

    //This is a unique (singleton) table that belongs to the tree
    //I'm using this reference for the sake of brevity
    table & parent_table = (parent->training_table);
    std::sort(idxes.begin(), idxes.end(), [&] (auto& i, auto& j) {return parent_table[col_name][i].get_double() < parent_table[col_name][j].get_double();});

    std::unordered_map<std::string, int> left;
    std::unordered_map<std::string, int> right;

    //initialize the "left" and "right" child
    for(const auto & cl : n_classes) {
        left.insert(std::make_pair(cl.first, 0));
        right.insert(cl);
    }
    //std::cout<<"split po "<<col_name<<std::endl;
    double gain_ratio_column = 0;
    for(unsigned i = 0; i<idxes.size()-1; i++) {
        left[parent_table[parent_table.get_target()][idxes[i]].get_string()]++;
        right[parent_table[parent_table.get_target()][idxes[i]].get_string()]--;

        if(parent_table[col_name][idxes[i]].get_double() == parent_table[col_name][idxes[i+1]].get_double())
            continue;

        int n_lt = i+1;
        int n_gte = n_rows - (i+1);
        double gain = parent_clean - ((double)n_lt/n_rows)*clean_metric(left, n_lt) - ((double)n_gte/n_rows)*clean_metric(right, n_gte);
        double split_info = -((double)n_lt/n_rows)*log2((double)n_lt/n_rows) - ((double)n_gte/n_rows)*log2((double)n_gte/n_rows);
        //std::cout<<parent_table[col_name][idxes[i]].get_double()<<"  "<<gain/split_info<<std::endl;
        if(gain_ratio_column < gain/split_info) {
              gain_ratio_column = gain/split_info;
              split_val = parent_table[col_name][idxes[i]].get_double();
        }
    }
    return gain_ratio_column;
}


/*
double tree_node::split_continuous(std::string col_name, double &split_val) {
    double parent_clean = clean_metric(t);
    double n = t.row_n();
    std::vector<double> col_values;
    for(auto it = t[col_name].begin(); it != t[col_name].end(); it++)
        col_values.push_back(it->to_double());

    double gain_ratio_column = 0;


    for(auto val : col_values) {
        table t_lt = t[t.where(col_name, [val] (auto e) {return e.get_double() < val;})];
        int n_lt = t_lt.row_n();

        table t_gte = t[t.where(col_name, [val] (auto e) {return e.get_double() >= val;})];
        int n_gte = t_gte.row_n();
        double gain = parent_clean - ((double)n_lt/n)*clean_metric(t_lt) - ((double)n_gte/n)*clean_metric(t_gte);
        double split_info = -((double)n_lt/n)*log2((double)n_lt/n)-((double)n_gte/n)*log2((double)n_gte/n);
        if(gain_ratio_column < gain/split_info) {
               gain_ratio_column = gain/split_info;
               split_val = val;
        }
    }
    return gain_ratio_column;
}*/

#include <QGraphicsSimpleTextItem>
#include <QRectF>
#include<iostream>
entry tree_node::classify(row r) {
    if(is_leaf)
        return class_name;
    int child_idx = split_rule->get_child(r[split_rule->get_colname()]);
    return children[child_idx]->classify(r);
}

std::unordered_map<int, int> max_depth;

void tree_node::draw_node(QGraphicsScene *scene, int x, int y, int & child_x) {
    QGraphicsRectItem *rect_item = new QGraphicsRectItem(0, 0, NODE_WIDTH, NODE_HEIGHT);

    if(depth == 0)
        max_depth.clear();

    rect_item->setPos(x, y);
    if(max_depth.find(depth) == max_depth.end())
        max_depth[depth] = x + NODE_WIDTH;
    else {
        if(x <= max_depth[depth]){
            for(auto it:scene->items())
                it->moveBy(x - max_depth[depth] - X_SPACING, 0);
        }
        else
            max_depth[depth] = x + NODE_WIDTH;
    }
    if(is_leaf)
        rect_item->setBrush(QColor(16,115,38));
    else
        rect_item->setBrush(QColor(15,81,115));

    scene->addItem(rect_item);
    int child_n = children.size();
    int start_x = (x + NODE_WIDTH/2) - (child_n*(NODE_WIDTH + X_SPACING) - X_SPACING)/2;
    int xmin = std::numeric_limits<int>::max();
    int xmax = std::numeric_limits<int>::min();

    if(!is_leaf)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem();
        scene->addItem(line);
        line->setLine(rect_item->x() + NODE_WIDTH/2, y + NODE_HEIGHT, rect_item->x() + NODE_WIDTH/2, y+NODE_HEIGHT + Y_SPACING/2);
    }
    std::vector<std::string> rule_names;
    if(!is_leaf)
        rule_names = split_rule->get_string();

    for(int i = 0; i<child_n; i++) {
        children[i]->draw_node(scene, start_x + i*(NODE_WIDTH + X_SPACING), y + NODE_HEIGHT + Y_SPACING, child_x);
        if(child_x > xmax)
            xmax = child_x;
        if(child_x < xmin)
            xmin = child_x;

        QGraphicsLineItem *line1 = new QGraphicsLineItem();
        scene->addItem(line1);
        line1->setLine(rect_item->x() + NODE_WIDTH/2, y + NODE_HEIGHT + Y_SPACING/2, child_x, y+NODE_HEIGHT + Y_SPACING/2);

        QGraphicsLineItem *line2 = new QGraphicsLineItem();
        scene->addItem(line2);
        line2->setLine(child_x, y + NODE_HEIGHT + Y_SPACING/2, child_x, y+NODE_HEIGHT + Y_SPACING);

        QGraphicsSimpleTextItem *rule = new QGraphicsSimpleTextItem();
        scene->addItem(rule);
        if(rule_names.size()>0)
            rule->setText(QString::fromStdString(rule_names[i]));
        rule->setPos(child_x - 15, rect_item->y() + (double)NODE_HEIGHT + (double)Y_SPACING*(3.0/4.0));
        rule->setZValue(500);

    }
    if(!is_leaf) {
        QGraphicsSimpleTextItem *rule = new QGraphicsSimpleTextItem();
        scene->addItem(rule);
        rule->setText(QString::fromStdString(split_rule->get_colname()));
        rule->setPos(rect_item->x() + NODE_WIDTH/2 - 10, rect_item->y() + (double)NODE_HEIGHT + (double)Y_SPACING*(1.0/4.0));
        rule->setZValue(500);
    }

    int i = 0;
    for(const auto &it:n_classes) {
        QGraphicsSimpleTextItem *n_klasa = new QGraphicsSimpleTextItem();
        scene->addItem(n_klasa);
        n_klasa->setText(QString::fromStdString(it.first + ": " + std::to_string(it.second)));
        n_klasa->setPos(rect_item->x() + 10, rect_item->y() + 10 + i*15);
        n_klasa->setZValue(500);

        i++;
    }

    QGraphicsSimpleTextItem *dubina = new QGraphicsSimpleTextItem();
    scene->addItem(dubina);
    dubina->setText("Depth: " + QString::number(depth));
    dubina->setPos(rect_item->x() + 10, rect_item->y() + 10 + i*15);
    i++;


    QGraphicsSimpleTextItem *cistoca = new QGraphicsSimpleTextItem();
    cistoca->setText("Clean: " + QString::number(score, 'g', 2));
    scene->addItem(cistoca);
    cistoca->setPos(rect_item->x() + 10, rect_item->y() + 10 + i*15);
    i++;


    QGraphicsSimpleTextItem *assigned = new QGraphicsSimpleTextItem();
    assigned->setText("Assigned class: " + QString::fromStdString(class_name.get_string()));
    scene->addItem(assigned);
    assigned->setPos(rect_item->x() + 10, rect_item->y() + NODE_HEIGHT - 20);

    child_x = rect_item->x() + NODE_WIDTH/2;
}

//METRICS

double entropy(std::unordered_map<std::string, int> &n_classes, int row_n) {
    //n_classes maps the name of each class into the number of rows belonging to that class; in total, there are "row_n" rows.
    double e = 0;
    double j_freq = 0;
    for(const auto & j : n_classes) {
        j_freq = (double)j.second/(double)row_n;
        if(j_freq!=0.0)
            e -= j_freq*log2(j_freq);
    }
    return e;
}

double gini(std::unordered_map<std::string, int> &n_classes, int row_n) {
    double g = 1;
    double j_freq = 0;
    for(const auto & j : n_classes) {
        j_freq = (double)j.second/(double)row_n;
        g -= j_freq*j_freq;
    }
    return g;
}




//DECISION TREE
decision_tree::decision_tree()
{
    //default values for stop conditions
    max_depth = 100;
    min_rows = 2;
    min_clean = 0;
    //default metric
    clean_metric = entropy;
}

void decision_tree::set_func(std::function<double (std::unordered_map<std::string, int> &, int)> metric) {
    clean_metric = metric;
}

void decision_tree::fit(table t) {
    training_table = t;
    training_table.pop("partition");
    std::vector<int> rows(t.row_n());
    for(unsigned i = 0; i<rows.size(); i++)
        rows[i] = i;

    root = new tree_node(std::move(rows), clean_metric, 0,this);
    root->split();
}

table decision_tree::classify(const table& test) {
    table t=test;
    t.push("assigned");
    for(int i = 0; i<test.row_n(); i++) {
        t["assigned"][i] = root->classify(test[i]);
    }
    return t;
}

void decision_tree::draw_tree(QGraphicsScene *scene) {
    int a = 0;
    root->draw_node(scene, 500, 500, a);
}

decision_tree::~decision_tree(){

}



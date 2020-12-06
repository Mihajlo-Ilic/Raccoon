#include "../Includes/decision_tree.hpp"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <QRect>
#include <queue>
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
tree_node::tree_node(table t, std::function<double(table)> metric, unsigned depth,decision_tree *p) {
    this->t = t;
    this->clean_metric = metric;
    this->depth = depth;
    this->parent=p;
    this->is_leaf=false;
    if(this->parent->levels.find(depth) == this->parent->levels.end())
        this->parent->levels[depth] = 1;
    else
        this->parent->levels[depth]++;

    for(const auto &it : t[t.get_target()].unique()) {
         n_classes[it.get_string()] = t.where(t.get_target(), [it] (auto x) {return x.get_string() == it.get_string();}).size();
    }

    this->score = clean_metric(t);

    if(depth >= parent->max_depth || t.row_n() <= parent->min_rows || clean_metric(t) <= parent->min_clean || t.col_n()==1)
    {    this->is_leaf = true;
         //most frequent class in node
        auto classes = t[t.get_target()].unique();
        int max = 0;
        entry cl_freq;
        for(const auto &cl : classes) {
            int tmp = t.where(t.get_target(), [cl](auto e){return e == cl;}).size();
            if(tmp > max) {
                max = tmp;
                cl_freq = cl;
            }
        }
        class_name = cl_freq.get_string();
    }

}

#include<iostream>
void tree_node::split() {
    if(this->is_leaf)
        return;


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
         }

         if(t[curr_col].type == column_type::NOMINAL) {
             double d = split_categorical(curr_col);
             if(gain_optimal < d) {
                  gain_optimal = d;
                  split_col_optimal = curr_col;
             }

         }
    }

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
}

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
}

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
double entropy(table t) {
      auto target_classes = t[t.get_target()].unique();
      double e = 0;
      double j_freq = 0;
      for(const auto &j : target_classes) {
          j_freq = ((double)t.where(t.get_target(), [j](auto ent) {return ent == j;}).size())/t.row_n();
          e -= j_freq*log2(j_freq);
      }
      return e;
}

double gini(table t) {
    auto target_classes = t[t.get_target()].unique();
    double g = 1;
    double j_freq = 0;
    for(const auto &j : target_classes) {
        j_freq = ((double)t.where(t.get_target(), [j](auto ent) {return ent == j;}).size())/t.row_n();
        g -= j_freq*j_freq;
    }
    return g;
}

//DECISION TREE
decision_tree::decision_tree()
{
    //default values for stop conditions
    max_depth = 255;
    min_rows = 1;
    min_clean = 0;
    //default metric
    clean_metric = entropy;
}

void decision_tree::set_func(std::function<double (table)> metric) {
    clean_metric = metric;
}

void decision_tree::fit(table t) {
    training_table = t;
    training_table.pop("partition");
    root = new tree_node(t, clean_metric, 0,this);
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
    //TODO: This is DFS implementation, change to BFS
    root->draw_node(scene, 500, 500, a);
}

decision_tree::~decision_tree(){

}



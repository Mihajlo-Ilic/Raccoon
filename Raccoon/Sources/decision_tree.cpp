#include "../Includes/decision_tree.hpp"
#include <numeric>
#include <cmath>
#include <algorithm>

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

std::string rule_categorical::get_string()
{
    return "";
}

std::string rule_continuous::get_string()
{
    return "";
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

   // std::cout<<t<<std::endl;
   // std::cout<<"ima entropiju "<<clean_metric(t)<<std::endl;
    //std::cout<<"ciljna promenljiva "<<t.get_target()<<std::endl;
    if(depth >= parent->max_depth || t.row_n() <= parent->min_rows || clean_metric(t) <= parent->min_clean || t.col_n()==1)
    {    this->is_leaf = true;
         //most frequent class in node
        auto classes = t[t.get_target()].unique();
        int max = 0;
        entry cl_freq;
        for(auto cl : classes) {
            int tmp = t.where(t.get_target(), [cl](auto e){return e == cl;}).size();
            if(tmp > max) {
                max = tmp;
                cl_freq = cl;
            }
        }
        class_name = cl_freq.get_string();
        std::cout<<"cvor je list i ima klasu :"<<class_name<<std::endl;
    }

}

#include<iostream>
void tree_node::split() {
    if(this->is_leaf)
        return;
    //TODO: Determine if current node is a leaf

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
              // std::cout<<"split po "<<curr_col<<" donosi "<<d<<"  vr="<<split_val<<std::endl;
               if(gain_optimal < d) {
                    gain_optimal = d;
                    split_col_optimal = curr_col;
                    split_val=curr_val;
               }
         }

         if(t[curr_col].type == column_type::NOMINAL) {
             double d = split_categorical(curr_col);
             std::cout<<"split po "<<curr_col<<" donosi "<<d<<std::endl;
             if(gain_optimal < d) {
                  gain_optimal = d;
                  split_col_optimal = curr_col;
             }

         }
    }
   // std::cout<<"najbolje je split po "<<split_col_optimal<<std::endl;
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
        for(auto u : t[split_col_optimal].unique()) {
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
    for(auto val : unique_vals) {
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
#include<iostream>
entry tree_node::classify(row r) {
    if(is_leaf)
        return class_name;
    int child_idx = split_rule->get_child(r[split_rule->get_colname()]);
    return children[child_idx]->classify(r);
}

double entropy(table t) {
      auto target_classes = t[t.get_target()].unique();
      double e = 0;
      double j_freq = 0;
      for(auto j : target_classes) {
          j_freq = ((double)t.where(t.get_target(), [j](auto ent) {return ent == j;}).size())/t.row_n();
          e -= j_freq*log2(j_freq);
      }
      return e;
}

double gini(table t) {
    auto target_classes = t[t.get_target()].unique();
    double g = 1;
    double j_freq = 0;
    for(auto j : target_classes) {
        j_freq = ((double)t.where(t.get_target(), [j](auto ent) {return ent == j;}).size())/t.row_n();
        g -= j_freq*j_freq;
    }
    return g;
}

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
decision_tree::~decision_tree(){

}



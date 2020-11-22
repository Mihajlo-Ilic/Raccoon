#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP
#include "../Includes/entry.hpp"
#include "../Includes/table.hpp"
#include <unordered_map>

double entropy(table t);
double gini(table t);

class decision_tree;

//Rule
class rule {
protected:
    std::string colname;
public:
    rule();
    std::string get_colname();
    virtual int get_child(entry e) = 0;
    virtual std::string get_string() = 0;
    virtual void add_value(entry val) = 0;
    virtual ~rule() = default;

};

class rule_continuous : public rule {
private:
    entry val;
public:
    rule_continuous(std::string column, entry val);
    int get_child(entry e) override;
    std::string get_string() override;
    void add_value(entry val) override;
};

class rule_categorical : public rule {
private:
    std::unordered_map<std::string, int> vals;
    int current_index = 0;
public:
    rule_categorical(std::string column);
    int get_child(entry e) override;
    std::string get_string() override;
    void add_value(entry val) override;
};



//Tree node
class tree_node {
private:
    decision_tree * parent;
    unsigned depth;
    table t;
    rule * split_rule;
    std::vector<tree_node*> children;
    bool is_leaf;
    entry class_name;
    double split_categorical(std::string column);
    double split_continuous(std::string column, double &split_val);
    void split();
    std::function<double(table)> clean_metric;
public:
    friend class decision_tree;
    tree_node(table t, std::function<double(table)> metric, unsigned depth,decision_tree* p);
    void draw_tree();
    entry classify(row r);
    ~tree_node();
};

class decision_tree
{
    unsigned max_depth;
    int min_rows;
    double min_clean;
    tree_node * root;
    std::function<double(table)> clean_metric;
    table training_table;
public:
    decision_tree();
    void set_stop_conditions(unsigned max_depth, int min_rows, double min_clean);
    void set_func(std::function<double(table)> metric);
    void set_max_depth(int v){
        max_depth=v;
    }
    void set_clean(double v){
        min_clean=v;
    }
    void set_min_row(int v){
        min_rows=v;
    }
    void fit(table t);
    table classify(const table& test);
    friend class tree_node;
    ~decision_tree();
};



#endif // DECISION_TREE_HPP

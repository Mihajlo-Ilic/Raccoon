#ifndef NAIVE_BAYES_HPP
#define NAIVE_BAYES_HPP

#include"../Includes/table.hpp"
#include<map>

class naive_bayes{
public:
    naive_bayes();
    void fit(const table& t);
    table predict(const table& t);
    table get_table();
    void set_alpha(double a);
private:
    table training;
    double alpha;
    // < pair<"column_name = unique value","class_name"> , p(unique_value|class_name) >
    std::map<std::pair<std::string,std::string> , double> categorical_cache;
    // <pair<"column_name","class_name"> , pair<column_mean|class_name , column_stdev|class_name> >
    std::map<std::pair<std::string,std::string>, std::pair<double,double>> continuous_cache;

    std::map<std::string,double> class_chances;

    void calculate_nominal(std::string column_name,std::string class_name,table& t);
    void calculate_continuous(std::string column_name,std::string class_name,table& t);

    entry predict_row(const table &t, int index);
};

#endif

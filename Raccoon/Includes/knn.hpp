#ifndef KNN_HPP
#define KNN_HPP

#include"stats.hpp"

class knn {
public:
    knn();
    void set_function(std::function<double( const row &, const row &)> metrics);
    void set_k(int k_);
    void fit(const table& data);
    table predict(const table& data);  
private:
    table training_table;
    std::function<double( const row &, const row &)> metrics;
    int k;
    std::string target;
};


#endif      

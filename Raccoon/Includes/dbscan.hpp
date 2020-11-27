#ifndef DBSCAN_HPP
#define DBSCAN_HPP
#include "../Includes/table.hpp"
#include <set>
class dbscan
{
private:
    table t;
    double eps;
    int n_neighbors;
    std::function<double(row, row)> metric;
    std::set<int> range_query(int i, const table &t);
public:
    dbscan();
    table fit(const table &t);
    void set_eps(double eps_);
    void set_n_neighbors(int n);
    void set_metric(std::function<double(row, row)> func);
    table fit_alt(const table &t);
};

#endif // DBSCAN_HPP

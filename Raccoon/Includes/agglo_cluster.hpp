#ifndef AGGLO_CLUSTER_HPP
#define AGGLO_CLUSTER_HPP

#include "table.hpp"
#include <functional>
#include <vector>

//available metrics between 2 clusters
double min_linkage(const table& cluster_1,const table& cluster_2,std::function<double(const row&,const row&)> ins_metric);
double max_linkage(const table& cluster_1,const table& cluster_2,std::function<double(const row&,const row&)> ins_metric);
double avg_linkage(const table& cluster_1,const table& cluster_2,std::function<double(const row&,const row&)> ins_metric);

class agglo_cluster{
public:
    agglo_cluster();

    table fit(const table& t);
    table predict(const table& t);

    //metric used between 2 instances for their distance ex:euclidean
    void set_instance_metric(std::function<double(const row&,const row&)> f);
    //metric used for distance between 2 clusters ex:min
    void set_cluster_metric(std::function<double(const table&,const table&,std::function<double(const row&,const row&)>)> f);
    //if num of clusters is set then tree generation will stop at that number
    void set_num_clusters(int n);
    //if distance is set tree generation will stop when distance is lower than that
    void set_distance(double d);

    table get_distance_table() const;
private:
    table training;
    std::function<double(const table&,const table&,std::function<double(const row&,const row&)>)> cluster_metric;
    std::function<double(const row&,const row&)> instance_metric;
    int num_clusters;
    double distance;

    int cluster_n;
    std::vector<std::pair<std::pair<int,int>,double>> dendogram;
};

#endif

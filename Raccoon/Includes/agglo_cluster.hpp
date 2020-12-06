#ifndef AGGLO_CLUSTER_HPP
#define AGGLO_CLUSTER_HPP

#include "table.hpp"
#include <functional>
#include <vector>

//available metrics between 2 clusters
double min_linkage(double a,double b);
double max_linkage(double a,double b);
double avg_linkage(double a,double b);

class agglo_cluster{
public:
    agglo_cluster();

    table predict(const table& t);

    //metric used between 2 instances for their distance ex:euclidean
    void set_instance_metric(std::function<double(const row&,const row&)> f);
    //metric used for distance between 2 clusters ex:min
    void set_cluster_metric(std::function<double(double a,double b)> f);
    //if num of clusters is set then tree generation will stop at that number
    void set_num_clusters(int n);
    //if distance is set tree generation will stop when distance is lower than that
    void set_distance(double d);

    table get_distance_table() const;

    const std::vector<std::pair<std::pair<int,int>,double>>& get_dendogram() const;
private:
    table training;
    std::function<double(double a,double b)> cluster_metric;
    std::function<double(const row&,const row&)> instance_metric;
    int num_clusters;
    double distance;


    int cluster_n;
    std::vector<std::pair<std::pair<int,int>,double>> dendogram;
};

#endif

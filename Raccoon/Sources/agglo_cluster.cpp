#include "../Includes/agglo_cluster.hpp"
#include "../Includes/stats.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <map>
#include <unordered_set>
#include <queue>

double min_linkage(double a,double b){
    return std::min(a,b);
}
double max_linkage(double a,double b){
    return std::max(a,b);
}
double avg_linkage(double a,double b){
    return (a+b)/2.0;
}


//class methods

agglo_cluster::agglo_cluster() {
    distance=0;
    num_clusters=1;
    instance_metric=euclidean_dist;
    cluster_metric=min_linkage;
}

table agglo_cluster::fit(const table& t){
    training=t;
    cluster_n = training.row_n();

   training.push("cluster");
   for(int i=0;i<training.row_n();i++){
       training["cluster"][i] = entry(i);
       training[i].set_name(std::to_string(i));
   }

   while(true){

       auto cluster_vals = training["cluster"].unique();
       std::vector<table> clusters;

        if(cluster_vals.size() == num_clusters)
            break;

       for(auto it:cluster_vals)
             clusters.push_back(training[training.where("cluster",[it](auto x){
                    return it.get_string()==x.get_string();
             })]);

        double dist = std::numeric_limits<double>::max();
        std::pair<int,int> merge_clusters=std::make_pair(-1,-1);

        for(int i=0;i<clusters.size();i++){
            for(int j=i+1;j<clusters.size();j++){
                double d=0;
                //old metric
                //double d=cluster_metric(clusters[i],clusters[j],instance_metric);
                if(d<dist){
                    dist=d;
                    merge_clusters=std::make_pair(i,j);
                }
            }
        }
        if(merge_clusters == std::make_pair(-1,-1))
            break;
        if(dist<distance)
            dendogram.push_back(std::make_pair(merge_clusters,dist));
        
        for(int j=0;j<clusters[merge_clusters.first].row_n();j++){
            std::string name = clusters[merge_clusters.first][j].r_name();
            int index = training.row_by_name(name);
            training["cluster"][index]=clusters[merge_clusters.second]["cluster"][0];
        }

   }
    //mapping clusters to 0,1,..n TODO
    auto cluster_vals = training["cluster"].unique();
    for(int i=0;i<cluster_vals.size();i++){
        std::vector<int> n = training.where("cluster",[i,&cluster_vals](auto x){ return x.get_string()==cluster_vals[i].get_string();});
        for(auto it:n)
            training["cluster"][it]=entry(i);
    }
    return training;
}
struct  hash_pair{
    size_t operator()(const std::pair<int,int> &p){
        auto hash1 = std::hash<int>{}(p.first);
        auto hash2 = std::hash<int>{}(p.second);
        return hash1 ^ hash2;
    }
};

typedef std::pair<std::pair<int*,int*>,double> pr;

struct  mcmp{
    bool operator()(const pr& l ,const pr& r){
        return l.second < r.second;
    }
};

table agglo_cluster::predict(const table& t){
    dendogram.clear();
    std::vector<std::vector<double>> distances(t.row_n());
    std::unordered_set<int> labels;

    for(int i=0;i<t.row_n();i++){
        distances[i].resize(t.row_n());
        for(int j=0;j<t.row_n();j++){
                distances[i][j] = instance_metric(t[i],t[j]);
        }
        labels.insert(i);
    }

    while(true){
        if(labels.size()==num_clusters) break;

        int cl_i;
        int cl_j;
        double cl_min = std::numeric_limits<double>::max();

        for(auto it:labels)
            for(auto jt:labels)
                if(it!=jt && distances[it][jt]<cl_min){
                    cl_i=it;
                    cl_j=jt;
                    cl_min=distances[it][jt];
                }
        dendogram.push_back(std::make_pair(std::make_pair(cl_i,cl_j),cl_min));

        labels.erase(cl_i);

        for(auto it:labels){
            distances[it][cl_j] = cluster_metric(distances[it][cl_j],distances[it][cl_i]);
            distances[it][cl_i] = std::numeric_limits<double>::max();
        }
    }

    table res = t;
    res.push("cluster");

    std::vector<int> r_table;
    for(int i=0;i<t.row_n();i++)
        r_table.push_back(i);

    for(int i=0;i<t.row_n();i++)
    for(int j=0;j<r_table.size();j++)
        if(r_table[j]==dendogram[i].first.first)
            r_table[j]=dendogram[i].first.second;

    for(int i=0;i<t.row_n();i++)
       res["cluster"][i] = entry(r_table[i]);
    return res;
}

void agglo_cluster::set_instance_metric(std::function<double(const row&,const row&)> f){
    instance_metric = f;
}

void agglo_cluster::set_cluster_metric(std::function<double(double a,double b)> f){
    cluster_metric = f;
}

void agglo_cluster::set_num_clusters(int n){
    distance = 0;
    num_clusters = n;
}

void agglo_cluster::set_distance(double d){
    num_clusters = 0;
    distance = d;
}

table agglo_cluster::get_distance_table() const{
    return training;
}

const std::vector<std::pair<std::pair<int, int>, double> > &agglo_cluster::get_dendogram() const
{
    return dendogram;
}

#include "../Includes/agglo_cluster.hpp"
#include "../Includes/stats.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <map>

double min_linkage(const table& cluster_1,const table& cluster_2,std::function<double(const row&,const row&)> ins_metric){
    double d = std::numeric_limits<double>::max();
    for(int i=0;i<cluster_1.row_n();i++)
        for(int j=0;j<cluster_2.row_n();j++){

            double c = ins_metric(cluster_1[i],cluster_2[j]);
            if(c<d)
                d=c;
        }
    return d;          
}
double max_linkage(const table& cluster_1,const table& cluster_2,std::function<double(const row&,const row&)> ins_metric){
    double d=std::numeric_limits<int>::min();
    for(int i=0;i<cluster_1.row_n();i++)
        for(int j=0;j<cluster_2.row_n();j++){
            double c = ins_metric(cluster_1[i],cluster_2[j]);
            if(c>d)
                d=c;
        }
    return d;
}
double avg_linkage(const table& cluster_1,const table& cluster_2,std::function<double(const row&,const row&)> ins_metric){
    double d=0;
    for(int i=0;i<cluster_1.row_n();i++)
        for(int j=0;j<cluster_2.row_n();j++){
            d += ins_metric(cluster_1[i],cluster_2[j]);
        }
    return d/( (double) cluster_2.row_n()*cluster_1.row_n());
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
                double d=cluster_metric(clusters[i],clusters[j],instance_metric);
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
    return training;
}

table agglo_cluster::predict(const table& t){

}

void agglo_cluster::set_instance_metric(std::function<double(const row&,const row&)> f){
    instance_metric = f;
}

void agglo_cluster::set_cluster_metric(std::function<double(const table&,const table&,std::function<double(const row&,const row&)>)> f){
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

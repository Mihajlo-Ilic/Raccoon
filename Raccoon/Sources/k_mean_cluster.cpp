#include "../Includes/k_mean_cluster.hpp"
#include <numeric>
#include <algorithm>
#include <climits>
#include <random>
#include <ctime>
#include <set>
#include <map>
#include<iostream>
//std::function<std::vector<int>(const table& t,int k, std::function<double( const row &, const row &)> metrics )>

std::vector<int> k_first(const table& t,int k, std::function<double( const row &, const row &)> metrics ) {
    std::vector<int> returnVector(t.row_n(),-1);
    for(int i = 0; i < k; i++)
        returnVector[i]=i;
    return returnVector;
}

std::vector<int> init_random(const table& t,int k, std::function<double( const row &, const row &)> metrics ) {
    std::vector<int> returnVector(t.row_n(),-1);
    int i = 0;
    std::srand(time(NULL));
    int numberOfRows = t.row_n();
    std::vector<int> a(numberOfRows);
    std::generate(a.begin(),a.end(),[&i]() {
             return i++;
    });
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(a.begin(),a.end(),g);
    int cluster=0;
    for(int i = 0; i < k; i++){
        returnVector[a[i]]=cluster++;
    }
    return returnVector;
}
#include<iostream>
std::vector<int> k_means_pp(const table& t,int k, std::function<double( const row &, const row &)> metrics ) {
    //1.korak biramo nasumicni centroid
    //2.korak za svaku tacku gledamo najkrace rastojanje od nekog centroida i sabiramo je sa ukupnim rastojanjem
    //3.korak, iteriram kroz tacke racunam distancu do najblizeg centroida, stavljam da je P = distX / sum
    //         pustim rand od 0 do 1, ako je rand > P uzimam tu tacku i tako dok ne popunim k tacaka

//    std::vector<int> returnVector(t.row_n(),-1);
//    int random = rand() % t.row_n();
//    returnVector[random]=0;
//    int tek=1;
//    std::set<int> setInd;
//    setInd.insert(random);
//    double sumDistance = 0;
//    std::vector<int> centroidVector(t.row_n(),0);
//    int chosenCentroids = 1;
//    while(chosenCentroids < k){
//        centroidVector.clear();
//        for(int i = 0; i < t.row_n(); i++) {
//            if(setInd.find(i) == setInd.end())
//                continue;
//            double minDistance = std::numeric_limits<int>::max();
//            int indxCentroid=0;
//            for(int j = 0; j < t.row_n(); j++) {
//                if(setInd.find(j) == setInd.end())
//                    continue;
//                if(metrics(t[j],t[i]) < minDistance) {
//                    indxCentroid = j;
//                    minDistance = metrics(t[j],t[i]);
//                }
//            }
//            centroidVector[i] = indxCentroid;
//            sumDistance += minDistance;
//        }
//        for(int i = 0; i < t.row_n(); i++) {
//            if(setInd.find(i) == setInd.cend())
//                continue;
//            double P = metrics(t[i],t[centroidVector[i]]) / sumDistance;
//            double vrv = rand()%100 / ((double)100);
//            if(vrv > P) {
//                returnVector[tek++]=(i);
//                chosenCentroids+=1;
//                break;
//            }
//        }
//    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 0.1);
     std::uniform_int_distribution<> i_dis(0, t.row_n()-1);

    std::vector<int> returnVector(t.row_n(),-1);
    int random = i_dis(gen);
    returnVector[random]=0;

    int clusters =1;
    std::set<int> centroids;
    centroids.insert(random);

    std::map<int,double> available;
    //add into available all except first index
    for(int i=0;i<t.row_n();i++)
        available.insert(std::make_pair(i,0.0));
    available.erase(random);

    for(int i=0;i<k-1;i++){
        //for every available calculate distance to closest centroid
        int sum=0;
        for(auto& it:available){
            double dist=std::numeric_limits<double>::max();
            for(auto cen:centroids)
                dist = std::min(metrics(t[it.first],t[cen]),dist);
            it.second=dist;
            sum+=dist;
        }
        int centar=available.begin()->first;
        for(const auto& it:available){

            double p=(it.second*it.second)/sum;

            double r=dis(gen);
            std::cout<<"sansa je "<<r<<std::endl;
            std::cout<<"verovatnoca je "<<p<<std::endl;
            if(r<=p){
                centar=it.first;
                break;
            }
        }
        returnVector[centar] = clusters++;
        centroids.insert(centar);
        available.erase(centar);
        available.erase(centar);
    }
    std::cout<<"centri su"<<std::endl;
    for(auto it:centroids)
        std::cout<<it<<std::endl;
    return returnVector;
}

k_mean_cluster::k_mean_cluster()
{
    metrics=euclidean_dist;
    init_func=k_means_pp;
    k=2;
}

void k_mean_cluster::fit(const table& data) {
    training_table = data;
}

table k_mean_cluster::predict(const table& data,int indicator) {
    table centroids;
    centroids.push(training_table.col_names());
    table data_copy = data;

    bool change=true;
    std::vector<int> clusters = init_func(data,k,metrics);
    for(auto it:clusters){
        if(it==-1)
            continue;
        centroids.push_row(training_table[it].get_row());
    }

    while(change) {
        change=false;
        for(int i = 0; i < data.row_n(); i++) {
            double minimum = std::numeric_limits<int>::max();
            int cluster_property = -1;
            int old_property = clusters[i];
            for(int j = 0; j < k; j++) {
                if(metrics(centroids[j],data[i]) < minimum) {
                    minimum = metrics(centroids[j],data[i]);
                    cluster_property = j;
                }
            }
            clusters[i] = cluster_property;
            if(clusters[i] != old_property && cluster_property!=-1) {
                change = true;
            }
        }
        for(int i = 0; i < k; i++) {
            std::vector<int> cl;
            for(unsigned j=0;j<clusters.size();j++)
                if(clusters[j]==i)
                    cl.push_back(j);

            table cluster = data_copy[cl];
            for(const auto& it : cluster.col_names()) {
                double sum = std::accumulate(cluster[it].begin(),cluster[it].end(),0.0,[](auto x,auto y) {
                    return x + y.get_double();
                });
                entry a=entry(sum / (double)cluster.row_n());
                if(it!="cluster")
                    centroids[it][i] = a;
            }
        }
    }
    data_copy.push("cluster");
    for(unsigned i=0;i<clusters.size();i++)
        data_copy["cluster"][i]=entry(clusters[i]);
    return data_copy;

}


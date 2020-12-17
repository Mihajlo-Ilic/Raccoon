#include "../Includes/k_mean_cluster.hpp"
#include <numeric>
#include <algorithm>
#include <climits>
#include <random>
#include <ctime>
#include <set>
#include <map>
#include<iostream>
#include <sys/time.h>
#include <thread>
#include <mutex>

const auto processor_count = std::thread::hardware_concurrency();
std::mutex split_mutex2;

//std::function<std::vector<int>(const table& t,int k, std::function<double( const row &, const row &)> metrics )>

std::vector<int> k_first(const table& t,int k, std::function<double( const row &, const row &)> metrics ) {
    (void) metrics;
    std::vector<int> returnVector(t.row_n(),-1);
    for(int i = 0; i < k; i++)
        returnVector[i]=i;
    return returnVector;
}

std::vector<int> init_random(const table& t,int k, std::function<double( const row &, const row &)> metrics ) {
    (void)metrics;
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

void thread_func1(const table &data,std::vector<int> &clusters,const table &centroids,bool *change,long int *begin_c,long int *end_c,std::function<double( const row &, const row &)> metrics, int k) {
    for(int i = *begin_c; i < *end_c; i++) {
        double minimum = std::numeric_limits<int>::max();
        int cluster_property = -1;
        int old_property = clusters[i];
        for(int j = 0; j < k; j++) {
            if(metrics(centroids[j],data[i]) < minimum) {
                minimum = metrics(centroids[j],data[i]);
                cluster_property = j;
            }
        }
        split_mutex2.lock();
        clusters[i] = cluster_property;
        if(clusters[i] != old_property && cluster_property!=-1) {
            *change = true;
        }
        split_mutex2.unlock();
    }
}

table k_mean_cluster::predict(const table& data) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms1 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

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

    long int thread_part = (data.row_n() / processor_count);
    int brojac = 1;

    std::vector<std::pair<long int,long int>> par;
    for(long int i = 0; i <= processor_count; i++) {
        long int begin_c = i*thread_part;
        long int end_c = std::min((i+1)*thread_part,(long int)data.row_n());
        par.push_back(std::make_pair(begin_c,end_c));

    }

    while(change) {
        change=false;
        std::vector<std::thread> threads;


        for(long int i = 0; i <= processor_count; i++) {
            threads.push_back(std::thread(thread_func1,std::ref(data),std::ref(clusters),std::ref(centroids),&change,&par[i].first,&par[i].second,metrics,k));
        }
        for(auto &th : threads) {
            th.join();
        }
        brojac++;
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

    gettimeofday(&tp, NULL);
    long int ms2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    std::cout << "VREME: " << ms2 - ms1 << std::endl;

    return data_copy;

}


#ifndef K_MEAN_CLUSTER_HPP
#define K_MEAN_CLUSTER_HPP

#include"stats.hpp"

class k_mean_cluster {
public:
    k_mean_cluster();
    void fit(const table& data);
    table predict(const table& data);

    void set_init(std::function<std::vector<int>(const table&,int, std::function<double( const row &, const row &)> )> f){
        init_func = f;
    }

    void set_metric(std::function<double( const row &, const row &)> func){
        metrics = func;
    }
    void set_k(int k_){
        k=k_;
    }
private:
    table training_table;
    std::function<double( const row &, const row &)> metrics;
    std::function<std::vector<int>(const table&,int, std::function<double( const row &, const row &)> )> init_func;
    int k;
};

void thread_func(const table &data,std::vector<int> &clusters,const table &centroids,bool *change,long int *begin_c,long int *end_c,std::function<double( const row &, const row &)> metrics, int k);

std::vector<int> k_first(const table& t,int k, std::function<double( const row &, const row &)> metrics );

std::vector<int> init_random(const table& t,int k, std::function<double( const row &, const row &)> metrics );

std::vector<int> k_means_pp(const table& t,int k, std::function<double( const row &, const row &)> metrics );

#endif // K_MEAN_CLUSTER_HPP

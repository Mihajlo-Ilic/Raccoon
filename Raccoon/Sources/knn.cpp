#include "../Includes/knn.hpp"
#include <stdlib.h>
#include <mutex>
#include <thread>

const auto processor_count = std::thread::hardware_concurrency();
std::mutex split_mutex_knn;


knn::knn() {
    k=3;
    metrics=euclidean_dist;
}

void knn::set_function(std::function<double (const row &, const row &)> metric)
{
    metric = metric;
}

void knn::set_k(int k_)
{
    k = k_;
}
void knn::fit(const table& data) {
    training_table = data;
    target = data.get_target();
}

void thread_func_knn(table &table_test,table &training_table,std::function<double (const row &, const row &)> metrics,std::vector<std::string> &class_labels,std::vector<std::string> &classified_labels,int k,int begin_c,int end_c) {
    for(int i=begin_c;i<end_c;i++){
        std::vector<std::pair<int,double>> indices;
        for(int j=0;j<training_table.row_n();j++){
            indices.push_back(std::make_pair(j,metrics(training_table[j], table_test[i])));
        }
        std::sort(indices.begin(),indices.end(),[](auto x,auto y){return x.second<y.second;});

        std::vector<std::string> labels;
        for(int j=0;j<k;j++){
            labels.push_back(class_labels[indices[j].first]);
        }
        std::pair<std::string,int>winner=std::make_pair(labels[0],1);
        for(int j=1;j<k;j++){
            if(labels[j]==winner.first)
                winner.second++;
            else {
                winner.second--;
                if( winner.second==0)
                    winner=std::make_pair(labels[j],1);
            }
        }
        split_mutex_knn.lock();
        classified_labels.push_back(winner.first);
        split_mutex_knn.unlock();
    }
}

#include<sys/time.h>
#include<iostream>
table knn::predict(const table& data) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms1 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    std::vector<std::string> labels;
    for(int i = 0; i < training_table.row_n();i++) {
        labels.push_back(training_table[target][i].get_string());
    }

    std::vector<std::string> class_labels;
    std::vector<std::string> classified_labels;
    std::for_each(training_table[target].begin(),training_table[target].end(),[&class_labels](auto& x){
        class_labels.push_back(x.get_string());
    });
    table table_test = data;
    table_test.pop(target);
    training_table.pop(target);
    table_test.pop("partition");
    training_table.pop("partition");

    std::vector<std::thread> threads;
    std::vector<std::pair<int,int>> part;
    int thread_part = table_test.row_n() / processor_count;

    for(int i=0;i<=processor_count;i++){
        int begin_c = thread_part*i;
        int end_c = std::min(thread_part*(i+1),training_table.row_n());
        part.push_back(std::make_pair(begin_c,end_c));
    }

    for(int i = 0; i <= processor_count; i++) {
        threads.push_back(std::thread(thread_func_knn,std::ref(table_test),std::ref(training_table),metrics,std::ref
                                      (class_labels),std::ref(classified_labels),k,part[i].first,part[i].second));
    }

    for(auto &th : threads)
        th.join();

    table_test.push("assigned");
    for(unsigned i=0;i<classified_labels.size();i++)
        table_test["assigned"][i]=entry(classified_labels[i]);

    gettimeofday(&tp, NULL);
    long int ms2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    std::cout << "VREME KNN: " << ms2 - ms1 << std::endl;

    return table_test;
}


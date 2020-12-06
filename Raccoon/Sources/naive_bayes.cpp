#include"../Includes/naive_bayes.hpp"
#include"../Includes/file_reader.hpp"
#include"../Includes/stats.hpp"
#include<unordered_map>
#include<iostream>
#include<set>
#include<cmath>

naive_bayes::naive_bayes():alpha(1.0)
{

}

void naive_bayes::fit(const table &t)
{
    training = t;

    std::string target_name = t.get_target();
    //table has no target column
    if(target_name=="no_target")
        return;
    auto unique_targets = training[target_name].unique();

    //used for saving tables to reduce time complexity
    std::unordered_map<std::string,table> target_cache;

    //for each unique value in target column add table with those classed to cache
    for(const auto& it:unique_targets){
        target_cache[it.get_string()] = training[ training.where( target_name , [it](auto x){ return x==it; }  ) ];
        class_chances[it.get_string()]= (double)target_cache[it.get_string()].row_n()/(double)training.row_n();
    }

    for(const auto &class_name : unique_targets)
        for(const auto &column_name : training.col_names())
            if(column_name != target_name){
                if(training[column_name].type == NOMINAL)
                    calculate_nominal(column_name,class_name.get_string(),target_cache[class_name.get_string()]);
                else
                if(training[column_name].type == CONTINUOUS)
                    calculate_continuous(column_name,class_name.get_string(),target_cache[class_name.get_string()]);
            }
//    std::cout<<get_table()<<std::endl;
}

table naive_bayes::predict(const table &t)
{
    table res=t;
    res.push("assigned");

    for(int i=0;i<t.row_n();i++){
        res["assigned"][i]=predict_row(t,i);
    }


    return res;
}

void naive_bayes::calculate_nominal(std::string column_name,std::string class_name,table& t)
{
    auto unique_vals = training[column_name].unique();
    for(const auto &val : unique_vals){
        int num_occurence = t.where(column_name,[val](auto x){ return x.get_string()==val.get_string(); }).size();
        int num = t.row_n();
        categorical_cache[std::make_pair(column_name+" = "+val.get_string(),class_name)] =
                (alpha+(double)num_occurence)/((double)num + (double)class_chances.size()*alpha);
    }

}

void naive_bayes::calculate_continuous(std::string column_name,std::string class_name,table& t)
{
    auto mean = column_mean(t[column_name]);
    auto var = column_variance(t[column_name]);
    continuous_cache[std::make_pair(column_name,class_name)] = std::make_pair(mean,var);
}

entry naive_bayes::predict_row(const table &t, int index)
{
    double best_p = 0;
    entry best_e = entry("n/a");

    for(const auto &class_name : class_chances){
        //foreach class calculate chance row belongs to it
        double chance = 1;
        for(auto col_name : t.col_names()){
            if(t[col_name].type == NOMINAL){
                chance *= categorical_cache[std::make_pair(col_name+" = "+t[col_name][index].get_string(),class_name.first)];
//                std::cout<<t[col_name][index].get_string()<<" "<<categorical_cache[std::make_pair(col_name+" = "+t[col_name][index].get_string(),class_name.first)]<<std::endl;
            }
            else
            if(t[col_name].type == CONTINUOUS){
                auto p=continuous_cache[std::make_pair(col_name,class_name.first)];
                double mean=p.first;
                double var=p.second;
                chance *=std::sqrt(1.0/(6.28*var)) * std::exp(-1* ( (t[col_name][index].get_double()-mean)*(t[col_name][index].get_double()-mean)/ ( 2*var )));
//                std::cout<<col_name<<" "<<std::sqrt(1.0/(6.28*var)) * std::exp(-1* ( (t[col_name][index].get_double()-mean)*(t[col_name][index].get_double()-mean)/ ( 2*var )))<<std::endl;

            }
        }

        chance *= class_name.second;
//        std::cout<<class_name.first<<" : "<<class_name.second<<std::endl;
//        std::cout<<chance<<std::endl;
//        std::cout<<"________"<<std::endl;
        //if we find better class we assign it
        if(chance >= best_p){
            best_p=chance;
            best_e = entry(class_name.first);
        }

    }
    return best_e;
}

table naive_bayes::get_table()
{
    table res;
    std::set<std::string> uniq;
    for(const auto &it:categorical_cache)
        uniq.insert(it.first.second);
    for(const auto &it:continuous_cache)
        uniq.insert(it.first.second);

    //creating unique columns
    std::vector<std::string> unique_classes;
    for(const auto &it:uniq)
        unique_classes.push_back("P("+it+")");
    res.push(unique_classes);

    //creating unique rows
    uniq.clear();
    for(const auto &it:categorical_cache)
        uniq.insert(it.first.first);
    for(const auto &it:continuous_cache)
        uniq.insert(it.first.first);
    unique_classes.clear();
    for(const auto &it:uniq)
        unique_classes.push_back(it);

    for(const auto &it:unique_classes){
        res.push_row();
        res[res.row_n()-1].set_name(it);
    }

    //seting values in cells
     for(const auto &it:categorical_cache){
         int index=res.row_by_name(it.first.first);
         if(index != -1){
            res["P("+it.first.second+")"][index] = entry(it.second);
         }
     }
     for(const auto &it:continuous_cache){
         int index=res.row_by_name(it.first.first);
         if(index != -1){
            res["P("+it.first.second+")"][index] = entry("m="+std::to_string(it.second.first)+" std="+std::to_string(it.second.second));
         }
     }

     return res;
}

void naive_bayes::set_alpha(double a)
{
    alpha = a;
}

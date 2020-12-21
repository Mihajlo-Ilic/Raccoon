#include "../Includes/apriori.hpp"
#include<set>
#include<map>
#include<cmath>

double calculate_support(const std::set<std::string>& items,const table& t){
    double occurence = 0.0;
    for(int i=0;i<t.row_n();i++)
        if(std::all_of(items.begin(),items.end(),[&](auto& x){return t[x][i].get_double() >0.0; }))
            occurence+=1.0;
    return occurence/((double)t.row_n());
}

double calculate_support(std::string& item,const table& t){
    double occurence = std::count_if(t[item].begin(),t[item].end(),[](auto& x){ return x.get_double()>0.0; });
    return occurence/((double)t.row_n());
}

double calculate_confidence(const std::set<std::string>& x,const std::set<std::string>& y,const table& t){
    std::set<std::string> nominator;
    std::set_union(x.begin(),x.end(),y.begin(),y.end(),
                   std::inserter(nominator,nominator.begin()));
    double occurence_nom = 0.0;
    double occurence_den = 0.0;

    for(int i=0;i<t.row_n();i++){
        if(std::all_of(nominator.begin(),nominator.end(),[&](auto& a){return t[a][i].get_double() >0.0; }))
            occurence_nom+=1.0;
        if(std::all_of(x.begin(),x.end(),[&](auto& a){return t[a][i].get_double() >0.0; }))
            occurence_den+=1.0;
    }
    if(occurence_den>0)
        return occurence_nom/occurence_den;
    return 0.0;
}

double calculate_deployability(const std::set<std::string>& x,const std::set<std::string>& y,const table& t){
    std::set<std::string> nominator;
    std::set_union(x.begin(),x.end(),y.begin(),y.end(),
                   std::inserter(nominator,nominator.begin()));
    double occurence_nom = 0.0;
    double occurence_den = 0.0;

    for(int i=0;i<t.row_n();i++){
        if(std::all_of(nominator.begin(),nominator.end(),[&](auto& a){return t[a][i].get_double() >0.0; }))
            occurence_nom+=1.0;
        if(std::all_of(x.begin(),x.end(),[&](auto& a){return t[a][i].get_double() >0.0; }))
            occurence_den+=1.0;
    }
    return (occurence_den-occurence_nom)/((double)t.row_n());
}

apriori::apriori()
{
    support = 0;
    confidence = 0;
}

void join_items(std::vector<std::set<std::string>>& items){

    std::vector<std::set<std::string>> curr;

    for(int i=0;i<items.size();i++)
        for(int j=i+1;j<items.size();j++){
            int k = std::count_if(items[i].begin(),items[i].end(),[&](auto& x){ return items[j].find(x)!=items[j].end(); });
            if(k>=items[i].size()-1){
                std::set<std::string> res;
                std::set_union(items[i].begin(),items[i].end(),items[j].begin(),items[j].end(),std::inserter(res,res.begin()));
                if(std::find(curr.begin(),curr.end(),res)==curr.end())
                    curr.push_back(res);
            }
        }
    items.clear();
    for(auto &it:curr)
        items.push_back(it);
}

void generate_subsets(const std::set<std::string>& t_set,std::vector<std::set<std::string>>& res){
    int n = t_set.size();

    std::vector<std::string> ref;
    for(auto& it:t_set)
        ref.push_back(it);

    int n_sub = std::pow(2,n);
    for(int i=0;i<n_sub;i++){
        int t=i;
        std::set<std::string> curr;
        for(int j=0;j<n;j++){
            if(t&1)
                curr.insert(ref[j]);
            t>>=1;
        }
        res.push_back(curr);
    }
}

#include<iostream>
#include<tuple>

void generate_confidence_sets(const std::set<std::string>& items,const table& in_table,table& res,double min_c){
    std::vector<std::set<std::string>> subsets;
    generate_subsets(items,subsets);

    std::map<std::string,std::tuple<double,double,double,double,double>> good_sets;
    double sup = calculate_support(items,in_table);
    for(int i=0;i<subsets.size()-1;i++)
            if(subsets[i].size()!=0){

                //for given item subsets create right side and calculate confidence

                std::set<std::string> right_side;
                std::set_difference(items.begin(),items.end(),subsets[i].begin(),subsets[i].end(),
                                    std::inserter(right_side,right_side.begin()));
                double conf = calculate_confidence(subsets[i],right_side,in_table);

                //if confidence is good then create string to put in table format item set -> item set
                if(conf>=min_c){
                    std::string t_s;
                    for(auto& it:subsets[i])
                        t_s+=it+",";
                    t_s.pop_back();
                    t_s+=" -> ";
                    for(auto& it:right_side)
                        t_s+=it+",";
                    t_s.pop_back();

                    double ante_s = calculate_support(subsets[i],in_table);
                    double conse_s = calculate_support(right_side,in_table);
                    double lift= conf/conse_s;
                    double deplo = calculate_deployability(subsets[i],right_side,in_table);
                    //poping back to remove last ,
                    good_sets[t_s]=std::make_tuple(conf,ante_s,conse_s,lift,deplo);
                }
            }

    for(auto& it:good_sets){
        std::vector<std::pair<std::string,entry>> t_row;
        t_row.push_back(std::make_pair("confidence",entry(std::get<0>(it.second))));
        t_row.push_back(std::make_pair("antecedent_support",entry(std::get<1>(it.second))));
        t_row.push_back(std::make_pair("consequent_support",entry(std::get<2>(it.second))));
        t_row.push_back(std::make_pair("lift",entry(std::get<3>(it.second))));
        t_row.push_back(std::make_pair("deployability",entry(std::get<4>(it.second))));
        t_row.push_back(std::make_pair("item_sets",entry(it.first)));
        t_row.push_back(std::make_pair("support",sup));
        res.push_row(t_row);
    }
}


table apriori::fit(const table &t)
{
    std::vector<std::set<std::string>> frequent_items;

    std::vector<std::set<std::string>> active_items;

    for(auto it:t.col_names()){
        if(calculate_support(it,t) >= support){
            std::set<std::string> items;
            items.insert(it);
            active_items.push_back(items);
        }
    }

    join_items(active_items);

    while(active_items.empty()==false){

        //if item set is below a certain frequency we remove it
        active_items.erase(std::remove_if(active_items.begin(),active_items.end(),[&](auto& x){ return calculate_support(x,t) < support; }),active_items.end());
        for(auto& it:active_items)
            frequent_items.push_back(it);
        //from currently active item sets we combine them to form new sets
        join_items(active_items);

    }


    table res;
    res.push("confidence");
    res.push("support");
    res.push("lift");
    res.push("deployability");
    res.push("consequent_support");
    res.push("antecedent_support");
    res.push("item_sets");

    for(auto &it:frequent_items)
        generate_confidence_sets(it,t,res,confidence);

    return res;
}

void apriori::set_support(double s)
{
    support = s;
}

void apriori::set_confidence(double c)
{
    confidence = c;
}

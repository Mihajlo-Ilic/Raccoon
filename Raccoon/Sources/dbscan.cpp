#include "../Includes/dbscan.hpp"
#include "../Includes/stats.hpp"
#include "../Includes/transformations.hpp"
#include <stack>
#include <iostream>
dbscan::dbscan()
{
    eps = 0.4;
    //criterion for core point
    n_neighbors = 4;
    metric = euclidean_dist;

}

void dbscan::set_eps(double eps_) {
    eps = eps_;
}

void dbscan::set_n_neighbors(int n){
    n_neighbors = n;
}

void dbscan::set_metric(std::function<double (row, row)> func) {
    metric = func;
}

std::set<int> dbscan::range_query(int i, const table &t) {
    std::set<int> neighbors;
    for(int j = 0; j<t.row_n(); j++) {
        if(metric(t[i], t[j]) < eps) {
            neighbors.insert(j);
        }
    }
    return neighbors;
}

table dbscan::fit(const table &t)
{
    table train = t;
    std::vector<std::set<int>> eps_okolina(t.row_n());
    for(int i = 0; i<t.row_n(); i++) {
       eps_okolina[i] = range_query(i, t);
    }

    train.push("cluster");
    train["cluster"] = entry(-1); //undefined label
    int cluster_ctr = 0;
    for(int i = 0; i<t.row_n(); i++) {
        if(train["cluster"][i] != entry(-1)) //already clustered
            continue;
        if(eps_okolina[i].size() < n_neighbors) {
            train["cluster"][i] = entry("NOISE");
            continue;
        }

        cluster_ctr++;
        train["cluster"][i] = entry(cluster_ctr);
        eps_okolina[i].erase(i);
        for(int v: eps_okolina[i]) {
            if(train["cluster"][v] == entry("NOISE") || train["cluster"][v] == entry(-1)) //if it's classified as noise or not classified yet, it belongs to current cluster
                train["cluster"][v] = entry(cluster_ctr);
            if(eps_okolina[v].size() >= n_neighbors)
                eps_okolina[i].insert(eps_okolina[v].begin(), eps_okolina[v].end());
        }
    }

    return train;
}
//Not working; fix later
table dbscan::fit_alt(const table& t) {
    table train = t;

   //int = how many reachable points from point i
   //bool = is point i reachable from core point
   std::vector<std::pair<int,bool>> adj(train.row_n(), std::make_pair(0, false));
   //indexes of points reachable from point i
   std::vector<std::vector<int>> neighbors(train.row_n());

   for(int i = 0; i<train.row_n(); i++) {
       //find neighbors of point i
       for(int j = i+1; j<train.row_n(); j++)
           if(metric(train[i], train[j]) <= eps) {
               adj[i].first++;
               neighbors[i].push_back(j);
           }
       //determine if point i is a core point
       if(adj[i].first >= n_neighbors) {
          //if i is a core point then all its neighbors are reachable from core point
          for(int neighbor_idx : neighbors[i])
              adj[neighbor_idx].second = true;
       }
   }



   train.push("cluster");
   train["cluster"] = entry(0.0);
   std::vector<bool> visited (train.row_n(), false);
   int cluster_n = 1;
   for(unsigned i = 0u; i<visited.size(); i++) {
       if(visited[i] == false && adj[i].first >= n_neighbors) {
           std::cout << i << "je jezgro" << std::endl;
           //i is a core point and all its neighbors belong to the same cluster
           std::stack<int> dfs_stack;
           dfs_stack.push(i);
           while(!dfs_stack.empty()) {
               int v = dfs_stack.top();
               std::cout << "ulazim u cvor " << v << " kao sused od " << i << std::endl;
               dfs_stack.pop();
               visited[v] = true;
               train["cluster"][v] = entry(cluster_n);
               //if v is also a core point, its neighbors are also in the same cluster
               if(adj[v].first >= n_neighbors) {
                   std::cout << v << " je takodje jezgro" << std::endl;
                   for(int neighbor_idx : neighbors[v])
                       if(!visited[neighbor_idx])
                           dfs_stack.push(neighbor_idx);
               }
               //v is an edge point
               if(adj[v].second == true) {
                   for(int n:neighbors[v])
                       if(!visited[n])
                           train["cluster"][n] = train["cluster"][v];
               }
           }
           cluster_n++;
       }

   }


    return train;
}

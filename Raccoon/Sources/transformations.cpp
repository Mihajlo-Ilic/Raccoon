#include "../Includes//transformations.hpp"
#include<random>
#include<set>
#include<iostream>
#include<ctime>

//Radna verzija
void normalize(table &data,std::string att) {
        std::for_each(data[att].begin(), data[att].end(), [](auto &x){
            x = x.to_double();
        });

        auto max_it = std::max_element(data[att].begin(), data[att].end());
        auto min_it = std::min_element(data[att].begin(), data[att].end());
        entry max = *max_it;
        entry min = *min_it;
        std::for_each(data[att].begin(), data[att].end(), [min, max](auto &x){
            x = (x - (min))/((max) - (min));
        });
}


// standardise to stdev = 1
void standardize(table &data,std::string att) {
        std::for_each(data[att].begin(),data[att].end(),[](auto &x){
            x = x.to_double();
        });
        auto sum = std::accumulate(data[att].begin(),data[att].end(),entry(0.0).get_double());
        auto mean = sum / ((double)data.row_n());
        double sum_stdev = 0;
        std::for_each(data[att].begin(),data[att].end(),[&sum_stdev,mean](auto x){
            sum_stdev += (x.get_double() - mean)*(x.get_double() - mean);
        });
        double stdev = std::sqrt( (1.0 / (double) data.row_n()  ) * sum_stdev);
        std::for_each(data[att].begin(),data[att].end(),[stdev,mean](auto &x){
            x = (x - entry(mean)) / entry(stdev);
        });
}

// has bag for taking variable
void categorical_to_binary(table &data,std::string attribute) {
    std::set<std::string> uniq;
    std::for_each(data[attribute].begin(),data[attribute].end(),[&uniq](const auto &x) {
        uniq.insert(x.get_string());
    });
    for(const auto& it : uniq) {
        data.push(it);
    }
    for(int i = 0; i < data.row_n();i++) {
        for(const auto& it : uniq) {
            if(data[attribute][i].get_string() == it) {
                data[it][i] = entry(1.0);
            }
            else {
                data[it][i] = entry(0.0);
            }
        }
    }

}

void partition(table &data,float percent,int rand_state) {
    std::srand(time(NULL));
    data.push("partition");
    data["partition"].role = PARTITION;
    if(rand_state == 0) {
        int train_size = (double) data.row_n() * percent;
        for(int i = 0; i <= train_size;i++)
            data["partition"][i] = entry("training");
        for(int i = train_size+1;i < data.row_n();i++)
            data["partition"][i] = entry("test");
    } else {
        int i = 0;
        std::vector<int> a(data.row_n());
        std::generate(a.begin(),a.end(),[&i]() {
            return i++;
        });
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(a.begin(),a.end(),g);
        int train_size = (double) data.row_n() * percent;
        for(int i = 0; i <= train_size;i++)
            data["partition"][a[i]] = entry("training");
        for(int i = train_size+1;i < data.row_n();i++)
            data["partition"][a[i]] = entry("test");

    }
}
#include<iostream>
void remove_na_rows(table &data) {
    data.pop_rows(data.where([](auto x) {  {
        for(auto y : x) {
            if(y.get_string() == entry("n/a").get_string()){
                return true;
            }
        }
        return false;
    } ;} ));
}

void binning_width(table &data,std::string attribute,int sets) {
    data.push("bin_" + attribute);
    std::vector<std::pair<int,entry>> bin_vector(data.row_n());
    int i = 0;
    std::for_each(bin_vector.begin(),bin_vector.end(),[data,attribute,&i](auto &x){ x = std::make_pair(i,data[attribute][i]); i++;});
    std::sort(bin_vector.begin(),bin_vector.end(),[](auto x,auto y) {return x.second < y.second;});
    double max = bin_vector.back().second.get_double();
    double min = bin_vector.front().second.get_double();
    double width = (max- min) / ((double)sets);
    double package = 0;
    for(const auto& it : bin_vector) {
        if(it.second.get_double() > min + width) {
            package ++;
            min += width;
        }
        data["bin_" + attribute][it.first] = entry(package);
    }
}

void binning_frequency(table &data,std::string attribute,int sets) {
    data.push("bin_" + attribute);
    std::vector<std::pair<int,entry>> bin_vector(data.row_n());
    int i = 0;
    std::for_each(bin_vector.begin(),bin_vector.end(),[data,attribute,&i](auto &x){ x = std::make_pair(i,data[attribute][i]); i = i + 1;});
    std::sort(bin_vector.begin(),bin_vector.end(),[](auto x,auto y) {return x.second < y.second;});
    int width = (bin_vector.size() / sets);
    i = 0;
    int package = 0;
    int cur_width = width;
    for(const auto& it : bin_vector) {
        data["bin_" + attribute][it.first] = entry(package);
        i += 1;
        if(i == width && package != sets - 1) {
            package ++;
            width += cur_width;
        }
    }     
}

void binning_mean(table &data,std::string attribute,int sets) {
    for(int i = 0; i < sets; i++) {
        double mean = 0;
        int j = 0;
        for(int k = 0; k < data.row_n();k++) {
            if(data["bin_" + attribute][k].get_double() == (double)i) {
                mean += data[attribute][k];
                j += 1;
            }
        }
        mean = mean / j;
        for(int k = 0; k < data.row_n();k++) {
            if(data["bin_" + attribute][k].get_double() == (double)i) {
                data["bin_" + attribute][k] = entry(mean);
            }
        }
    }
}

void binning_boundry(table &data,std::string attribute,int sets) {
    for(int i = 0; i < sets; i++) {
        std::vector<std::pair<int,double>> bin_vector;
        int j = 0;
        std::for_each(data["bin_" + attribute].begin(),data["bin_" + attribute].end(),[attribute,&bin_vector,&j,data,i](auto &x) {
            if(x.get_double() == (double)i) {
                bin_vector.push_back(std::make_pair(j,data[attribute][j].get_double()));
            }
            j = j + 1;
        });
        std::sort(bin_vector.begin(),bin_vector.end(),[](auto x, auto y){
            return x.second < y.second;
        });
        int width = bin_vector.size();
        std::cout << width << std::endl;
        double minimum = bin_vector.front().second;
        double maximum = bin_vector.back().second; 
        for(int k = 0; k < width; k++) {
            if(k < width/2) 
                data["bin_" + attribute][bin_vector[k].first] = entry(minimum);
            else 
                data["bin_" + attribute][bin_vector[k].first] = entry(maximum);  
        }  
    }
}

void aproximation_val(table &data,std::string attribute,double value) {
    std::for_each(data[attribute].begin(),data[attribute].end(),[value](auto &x) {
        if(x.get_string() == entry("n/a").get_string())
            x = entry(value);
    });
}

void aproximation_mean(table &data,std::string attribute) {
    double value = column_mean(data[attribute]);
    std::for_each(data[attribute].begin(),data[attribute].end(),[value](auto &x) {
        if(x.get_string() == entry("n/a").get_string())
            x = entry(value);
    });
}

std::vector<double> idf(table &t){
    std::vector<std::string> att = t.col_names();
    int documents_number = t.row_n();
    std::vector<double> idf_vector(t.col_n());
    int i = 0;
    for(const auto &at : att) {
        double number_of_files = 0.0;
        std::for_each(t[at].begin(),t[at].end(),[&number_of_files](auto &x){
            if(x.get_double() > 0)
                number_of_files += 1;
        });
        idf_vector[i++] = log(((double)documents_number)/number_of_files) + 1;
    }
    return idf_vector;
}

void tf_idf(table &t) {
   std::vector<double> idf_vector = idf(t);

   std::cout << std::endl;
   for(int i = 0; i < t.row_n(); i++) {
       std::vector<std::string> att = t.col_names();
       int j = 0;
       for(const auto &at : att)
           t[i][at] = entry(t[i][at].get_double() * idf_vector[j++]);
   }
}

#ifndef APRIORI_HPP
#define APRIORI_HPP

#include "table.hpp"

class apriori{
   public:
    apriori();

    table fit(const table& t);

    void set_support(double s);
    void set_confidence(double c);

   private:
    double support;
    double confidence;
};


#endif // APRIORI_HPP

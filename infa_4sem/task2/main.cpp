#include "Montecarlo.h"
#include <QtCore/QCoreApplication>

double f(double x)
{
    return x;
}

int main(){

    double res = Montecarlo(-40.0 , 40.0, 3);
    std::cout<< res<<"\n";

    return 0;
}

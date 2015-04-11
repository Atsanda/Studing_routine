#include "Montecarlo.h"

Calculator::Calculator( double a, double b, double& res):
    from(a), to(b), result(res)
{}

Calculator::~Calculator(){}

void Calculator::run()
{

    double len = to - from;
    double tmp;
    double sum_func_val = 0;
    for(int i=0; i < POINTS_NUM; i++)
    {
        sum_func_val += f( from + len * modf(qrand() / len, &tmp));
    }

    result += len * sum_func_val / POINTS_NUM;
    this->thread()->quit();
    return ;
}


double Montecarlo(double from, double to, int threads_num)
{
    if( threads_num <= 0 )
        throw Error("Error: invalid arguments of Montecarlo() funcrtion, threads_num argument can not be less than zero\n");

    double result = -1;
    double lenth = (to - from) / threads_num;

    std::vector<Calculator*> Calculators(threads_num);
    std::vector<QThread*> Threads(threads_num);

    for(int i=0; i < threads_num; i++)
    {
        try{
            Calculators[i] = new Calculator( from + lenth*i, from + lenth*(i+1), result);
            Threads[i] = new QThread;
        }
        catch( std::exception& e )
        {
            std::cout << e.what() << std::endl;
        }

        Calculators[i]->moveToThread(Threads[i]);

        QObject::connect( Threads[i],       SIGNAL(started()),      Calculators[i], SLOT(run()));

        Threads[i]->start();
    }


    for(int i=0; i< threads_num; i++)
    {
        Threads[i]->wait();
        delete Threads[i];
        delete Calculators[i];
    }

    return result;
}

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <QThread>
#include <QtCore/QCoreApplication>
#include <iostream>
#include <QString>
#include <vector>
#include <iterator>
#include <QtGlobal>
#include <assert.h>
#include <QDebug>
#include <cmath>

#define POINTS_NUM 1000

double f(double x);

double Montecarlo(double from, double to, int threads_num);

//calculates integral with Montecarlo method
class Calculator: public QObject
{
    Q_OBJECT

public:
    explicit Calculator( double a, double b, double& res);
    virtual ~Calculator();

private:
    double from;
    double to;
    double& result;

public slots:
    void run();

signals:
    void finished();

};


class Error: public std::exception
{
            public:
                Error(const char* m){messege=m;}
                virtual const char* what() const throw(){return messege;}
            private:
                const char* messege;
};

#endif // MONTECARLO_H

#include <iostream>
#include <vector>
#include <time.h>
#define N 10
#define mut_prob 0.2
using namespace std;

double func(double x1, double x2, double x3, double x4)
{
    return x1+2*x2+3*x3+4*x4;
}
void cross_prob(vector<double> &surv, int &p1, int &p2)
{
    vector<int> dist;

    for(int i=0;i<N;i++)
        for(int j=0;j<(int)100*surv[i];j++)
            dist.push_back(i);

    int number,p1,p2;
    p1 = rand() % dist.size();
    for(int i=0;i<dist.size;i++)
        if(p1 == i) number = i;
    dist.erase(i,i+(int)100*surv[i]);
    p2 = rand() % dist.size();
}

int main()
{
    srand(time(0));



    return 0;
}

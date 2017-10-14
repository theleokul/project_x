#include <iostream>
#include <vector>
#include <time.h>
#define N 10
#define K 4
#define MIN -100
#define MAX 100
#define MUT 0.2
using namespace std;

double func(double x1, double x2, double x3, double x4);
void cross_prob(vector<double> &surv, int &p1, int &p2);
void def_surv(vector<double> &surv, vector<vector<int>> &gen);
double random(double min, double max);

int main()
{
    srand(time(0));
    vector<vector<double>> gen(N, vector<double>());
    vector<double> surv(N);

    // Creation of first generation
    for(int i=0;i<N;i++)
        for(int j=0;j<K;j++)
            gen[i].push_back(random());

    // Def of surv coef's
    def_surv(surv, gen);

    //Crossover
    int p1, p2;
    for(int i=0;i<N/2;i++)
    {
        cross_prob(surv, p1, p2);
        swap(gen[p1][0],gen[p2][0]);
        swap(gen[p1][1],gen[p2][1]);
    }



    return 0;
}

double func(double x1, double x2, double x3, double x4)
{
    return x1+2*x2+3*x3+4*x4-30;
}

double random()
{
    return (double)(rand())/RAND_MAX*(MAX - MIN) + MIN;
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

void def_surv(vector<double> &surv, vector<vector<int>> &gen)
{
    double sum = 0.0;
    for(int i=0;i<N;i++) {
        surv[i] = fabs(func(gen[i][0], gen[i][1], gen[i][2], gen[i][3]));
        sum += surv[i];
    }
    sum = fabs(sum);
    for(int i=0;i<N;i++)
        surv[i] = surv[i]/sum;
}
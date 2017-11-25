#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iterator>
#define N 6 // Amount of members in one generation
#define K 4 // Amount of variables
#define MIN 0 // Min value of variable
#define MAX 30 // Max value of variable
#define MUT 0.2 // Coefficient of mutation
#define EPS 1.0 // It's a value of suitable mistake
using namespace std;

//Prototypes of all functions in program
double func(double x1, double x2, double x3, double x4); // investigated function
void cross_prob(vector<double> &surv, int &p1, int &p2); // Chooses the most suitable(with highest surv.) parents (p1 & p2)
void def_surv(vector<double> &surv, vector<double> &gen, double &sum, int i); // Defines the survival coef. each member in generation
double random(); // Returns a random double number in range of MIN to MAX

int main()
{
    srand(time(0));
    vector< vector<double> > gen(N, vector<double>());
    vector<double> surv(N);

    // Creation of first generation
    for(int i=0;i<N;i++)
        for(int j=0;j<K;j++)
            gen[i].push_back(random());

    // Definition of survival coefficients
    double sum = 0.0;
    for(int i=0;i<N;i++) {
        def_surv(surv, gen[i], sum, i);
    }
    for(int i=0;i<N;i++)
        surv[i] = (1/surv[i])/sum;

    for(int i=0;i<N;i++) {
    for(int j=0;j<4;j++)
        printf("%.2f   ",gen[i][j]);
    cout << '\n';
    }
    for(int i=0;i<N;i++) {printf("%.2f   ",surv[i]);}
    cout << '\n' << "sum = " << sum << '\n';

    //Pairing
    int p1=0, p2=0; // Parent1 & Parent 2
    //for(int i=0;i<N/2;i++)
    //{
        cross_prob(surv, p1, p2);
        swap(gen[p1][0],gen[p2][0]);
        swap(gen[p1][1],gen[p2][1]);
    //}

    cout << "p1 = " << p1 << " p2 = " << p2;

    return 0;
}

void def_surv(vector<double> &surv, vector<double> &mem, double &sum, int i) // mem = memasik :) or member
{
    surv[i] = fabs(func(mem[0], mem[1], mem[2], mem[3])); // Later I should modify it for more variables
    sum += 1/surv[i];
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
    vector<int> dist; // Distribution of indexes each member of gen. for random choose

    // I know it isn't optimized method...
    for(int i=0;i<N;i++)
        for(int j=0;j<(int) 100*surv[i];j++) // 100*surv - probability of surviving in percentage
            dist.push_back(i);

    for(int i=0;i<dist.size();i++)
        cout << dist[i] << "  ";
    cout << endl;

    int num=0;
    p1 = dist[rand() % dist.size()]; // Choose random index in dist
    cout << p1 << endl;
    for(int i=0;(unsigned) i<dist.size();i++)
        if(p1 == dist[i]) {num = i; break;}
    dist.erase(dist.begin() + num,dist.begin() + num + (int)100*surv[p1]); // It needs to choose different parents
    p2 = dist[rand() % dist.size()];
}

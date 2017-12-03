#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#define DIM 4
#define N_GEN 2000 // Amount of generations
#define N 24 // Amount of members in one generation
#define MIN -60 // Min value of variable
#define MAX 60 // Max value of variable
#define MUT 20 // Coefficient of mutation in percentage
#define EPS 3.0 // It's a value of suitable mistake
using namespace std;

//Prototypes of all functions in program
//double func(double x1, double x2, double x3, double x4); // investigated function
double mult_AiX(vector<double> &ai, vector<double> &x); // Solves linear equation with i-line matrix A and x(has column form)
void cross_prob(vector<double> &surv, int &p1, int &p2, bool &crossover); // Chooses the most suitable(with highest surv.) parents (p1 & p2)
void def_surv(vector<double> &surv, vector<double> &mem, vector<double> &ai, int i); // Defines the survival coef. each member in generation
void mutation(vector<double> &mem); // All members go to mutation - their alelles changes randomly
double random(); // Returns a random double number in range of MIN to MAX

int main()
{
    srand(time(0));
    vector< vector<double> > gen(N, vector<double>());
    vector< vector<double> > a(DIM, vector<double>());
    vector<double> surv(N);
    ifstream f;
    char filename[100];
    bool crossover=false; // In domain of chosen parents counts their probability to pairing
    bool the_end=false; // Means that the solution was found

    //Open the file and scan it
    cout << "Input name of file with matrix A: ";
    cin >> filename;
    f.open(filename);
    if (!f.is_open()) { // If file is not opened
        cout << "There is a mistake, make sure you entered the correct name of file and try again!\n";
        return 1;
    }

    double temp;
    for(int i=0;i<DIM;i++) {
        for(int j=0;j<(DIM+1);j++) {
            f >> temp;
            a[i].push_back(temp);
        }
    }
    f.close();

    /* for(int i=0;i<DIM;i++) {
        for(int j=0;j<(DIM+1);j++)
            printf("%.2f   ",a[i][j]);
        cout << '\n';
        } */

    // Creation of first generation
    for(int i=0;i<N;i++)
        for(int j=0;j<DIM;j++)
            gen[i].push_back(random());

// There begins iteration process

    for(int t=0;t<N_GEN;t++) {
        // Definition of survival coefficients
        double sum = 0.0;
        for(int i=0;i<N;i++) {
            for(int j=0;j<DIM;j++) {
                surv[i]+=fabs(mult_AiX(a[j],gen[i]));
                //def_surv(surv, gen[i], a[j], i);
            }
            sum += 1/surv[i];
        }
        for(int i=0;i<N;i++)
            surv[i] = (1/surv[i])/sum;

        /*
        // It's for test
        for(int i=0;i<N;i++) {
        for(int j=0;j<4;j++)
            printf("%.2f   ",gen[i][j]);
        cout << '\n';
        } */
        //for(int i=0;i<N;i++) {printf("%.2f   ",surv[i]);}

        //Pairing
        int p1=0, p2=0; // Parent1 & Parent 2
        int div_cross=0; // Divide chromosomes to pairing, changes from 1 to K-1
        int which_size_change=0; // Equals 0 or 1 and means what part of chromosome will be paired
        for(int i=0;i<N/2;i++)
        {
            cross_prob(surv, p1, p2, crossover);
            if(crossover) {
                div_cross = 1 + rand() % (DIM-1);
                which_size_change = rand() % 2;
                if(!which_size_change)
                    for(int i=0;i<div_cross;i++)
                        swap(gen[p1][i],gen[p2][i]);
                else
                    for(int i=div_cross;i<DIM;i++)
                        swap(gen[p1][i],gen[p2][i]);
            }
            crossover = false;
        }

        /*
        // It's for test
        cout << "\n\n";
        for(int i=0;i<N;i++) {
        for(int j=0;j<4;j++)
            printf("%.2f   ",gen[i][j]);
        cout << '\n';
        } */

        // Transit to mutation
        for(int i=0;i<N;i++) {
            mutation(gen[i]);
        }

        /*
        // It's for test
        cout << "\n\n";
        for(int i=0;i<N;i++) {
        for(int j=0;j<4;j++)
            printf("%.2f   ",gen[i][j]);
        cout << '\n';
        } */
        for(int i=0;i<N;i++) {
            double check = 0.0;
            for(int j=0;j<DIM;j++)
                check+=fabs(mult_AiX(a[j],gen[i]));
            if( check < EPS ) {
                for(int j=0;j<DIM;j++)
                    printf("x%d = %.2f\n", j, gen[i][j]);
                the_end = true;
                break;
            }

        }
        if(the_end) break;
    }

    return 0;
}

void mutation(vector<double> &mem)
{
    for(int i=0;i<DIM;i++)
        if( MUT/(1 + rand() % 100) ) mem[i] = random();
}

void def_surv(vector<double> &surv, vector<double> &mem, vector<double> &ap, int i) // mem = memasik :) or member or chromosome
{
    surv[i]+=fabs(mult_AiX(ap,mem));
    //surv[i] = fabs(func(mem[0], mem[1], mem[2], mem[3])); // Later I should modify it for more variables
}

/* double func(double x1, double x2, double x3, double x4)
{
    return x1+2*x2+3*x3+4*x4-30;
} */

double mult_AiX(vector<double> &ai, vector<double> &x)
{
    double result=0;
    for(int j=0;j<DIM;j++)
        result+=ai[j]*x[j];
    result-=ai[DIM];
    return result;
}

double random()
{
    return (double)(rand())/RAND_MAX*(MAX - MIN) + MIN;
}

void cross_prob(vector<double> &surv, int &p1, int &p2, bool &crossover)
{
    vector<int> dist; // Distribution of indexes each member of gen. for random choose, roulette-wheel selection
    char wheel[100];
    int p;

    for(int i=0;i<N;i++)
        for(int j=0;j<(int) 100*surv[i];j++) // 100*surv - probability of surviving in percentage
            dist.push_back(i);

    int num=0;
    p1 = dist[rand() % dist.size()]; // Choose random index in dist
    for(int i=0;(unsigned) i<dist.size();i++)
        if(p1 == dist[i]) {num = i; break;}
    dist.erase(dist.begin() + num,dist.begin() + num + (int)100*surv[p1]); // It needs to choose different parents
    p2 = dist[rand() % dist.size()];

    p = (int) 100*(surv[p1] + surv[p2])/2; // Common survival coefficient in percentage
    int j=p;
    for(int i=0;i<100;i++,j--) {
        if(j!=0) wheel[i]=1;
        else wheel[i]=0;
    }
    if(wheel[rand() % 100]) crossover = true; //When we chose parents we count their chance to pairing
}

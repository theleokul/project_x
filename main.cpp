#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#define N_GEN 10000 // Amount of generations
#define N 100 // Amount of members in one generation
#define MIN -60 // Min value of variable
#define MAX 60 // Max value of variable
#define MUT 20 // Coefficient of mutation in percentage
#define EPS 3.0 // It's a value of suitable mistake
using namespace std;

int dim;

//Prototypes of all functions in program
double mult_AiX(vector<double> &ai, vector<double> &x); // Solves linear equation with i-line matrix A and x(has column form)
void cross_prob(vector<double> &surv, int &p1, int &p2, bool &crossover); // Chooses the most suitable(with highest surv.) parents (p1 & p2)
void mutation(vector<double> &mem); // All members go to mutation - their alelles changes randomly
double random(); // Returns a random double number in range of MIN to MAX

int main()
{
    srand(time(0));
    vector< vector<double> > gen(N, vector<double>());
    vector<double> surv(N);
    vector< vector<double> > a(10, vector<double>());
    ifstream f;
    char filename[100];
    bool crossover=false; // In domain of chosen parents counts their probability to pairing
    bool the_end=false; // Means that the solution was found
    double sum = 0.0; // variable for counting sum of survival coefficients each gen
    double suit_defect = LONG_MAX; // It's needed for case than solution won't be found but program will keep min defect in this variable
    int suit_sol; // Number of suitable member in all gen's
    int cond; // It's for switch

    cout << "Program for solving system of linear equations. Version: 1.01. Made by XaKer333." << endl;
    cout << "Choose 1-3:" << endl << "1 - Input matrix A from file;" << endl << "2 - Input matrix A in console;" << endl << "3 - Help." << endl;
    cin >> cond;
    switch(cond) {
        case 1: //Open the file and scan it
                { cout << "Input name of file with matrix A: ";
                cin >> filename;
                f.open(filename);
                if (!f.is_open()) { // If file is not opened
                    cout << "There is a mistake, make sure you entered the correct name of file and try again!\n";
                    return 1;
                }

                f >> dim;
                //vector< vector<double> > a(dim, vector<double>());
                double temp;
                for(int i=0;i<dim;i++) {
                    for(int j=0;j<(dim+1);j++) {
                        f >> temp;
                        a[i].push_back(temp);
                    }
                }
                f.close();
                cout << "It was successfully scanned!" << endl << "Loading..." << endl;
                break; }
        case 2: { cout << "Input dimension of matrix A: ";
                cin >> dim;

                double temp;
                for(int i=0;i<dim;i++) {
                    for(int j=0;j<(dim+1);j++) {
                        cin >> temp;
                        a[i].push_back(temp);
                    }
                }
                cout << "It was successfully scanned!" << endl << "Loading..." << endl;
                break; }
        case 3: { cout << "This program solves Ax=b systems,\n" << "-- A - square matrix with dimension dim(<=10), also matrix A is extended (b is included);\n";
                cout << "-- x - vector-column;\n"; return 3; }
        default: { cout << "Incorrect input, try again!"; return 2; }
    }

    // Creation of first generation
    for(int i=0;i<N;i++)
        for(int j=0;j<dim;j++)
            gen[i].push_back(random());

// There begins iteration process

    for(int t=0;t<N_GEN;t++) {
        // Definition of survival coefficients
        sum = 0.0;
        for(int i=0;i<N;i++) {
            for(int j=0;j<dim;j++) {
                surv[i]+=fabs(mult_AiX(a[j],gen[i]));
                //def_surv(surv, gen[i], a[j], i);
            }
            sum += 1/surv[i];
        }
        for(int i=0;i<N;i++)
            surv[i] = (1/surv[i])/sum;

        //Pairing
        int p1=0, p2=0; // Parent1 & Parent 2
        int div_cross=0; // Divide members of generation to pairing, changes from 1 to K-1
        int which_size_change=0; // Equals 0 or 1 and means what part of member will be paired
        for(int i=0;i<N/2;i++)
        {
            cross_prob(surv, p1, p2, crossover);
            if(crossover) {
                div_cross = 1 + rand() % (dim-1);
                which_size_change = rand() % 2;
                if(!which_size_change)
                    for(int i=0;i<div_cross;i++)
                        swap(gen[p1][i],gen[p2][i]);
                else
                    for(int i=div_cross;i<dim;i++)
                        swap(gen[p1][i],gen[p2][i]);
            }
            crossover = false;
        }

        // Transit to mutation
        for(int i=0;i<N;i++) {
            mutation(gen[i]);
        }

        // Check generation if there is a solution
        for(int i=0;i<N;i++) {
            double check = 0.0;
            for(int j=0;j<dim;j++)
                check+=fabs(mult_AiX(a[j],gen[i]));
            if(check < suit_defect) { suit_defect = check; suit_sol = i; }
            if( check < EPS ) {
                cout << "Amount of spent generations: " << t << endl;
                for(int j=0;j<dim;j++)
                    printf("x%d = %.2f\n", j, gen[i][j]);
                the_end = true;
                break;
            }
        }
        if(the_end) break;
    }

    if(!the_end){
        printf("Solution with EPS = %.2f wasn't found, but this value the best:\n", EPS);
        for(int j=0;j<dim;j++)
            printf("x%d = %.2f\n", j, gen[suit_sol][j]);
    }

    return 0;
}

void mutation(vector<double> &mem)
{
    for(int i=0;i<dim;i++)
        if( MUT/(1 + rand() % 100) ) mem[i] = random();
}

double mult_AiX(vector<double> &ai, vector<double> &x)
{
    double result=0;
    for(int j=0;j<dim;j++)
        result+=ai[j]*x[j];
    result-=ai[dim];
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

#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#define N_GEN 10000 // Amount of generations
#define N 30 // Amount of members in one generation
#define D 101 // Max available dimension
#define MIN -10 // Min value of variable
#define MAX 10 // Max value of variable
#define MUT 30 // Coefficient of mutation in percentage
//#define EPS 500 // It's a value of suitable mistake
#define EPS2 0.1 // Defect in 1 equation
#define EPS_SYM 0.1
using namespace std;

int dim; //dimension of x - member of population

//Prototypes of all functions in program
double mult_AiX(vector<double> &ai, vector<double> &x); // Solves linear equation with i-line matrix A and x(has column form)
void cross_prob(vector<double> &surv, int &p1, int &p2); // Chooses the most suitable(with highest surv.) parents (p1 & p2)
void mutation(vector<double> &mem); // All members go to mutation - their alelles(values) changes randomly
void extreme_mutation(vector<double> &mem);
double random(); // Returns a random double number in range of MIN to MAX

int main()
{
    srand(time(0));
    vector< vector<double> > gen(N, vector<double>()); // Population
    vector<double> surv(N); // Table of survival coefficients each member of  population
    vector< vector<double> > a(D, vector<double>()); // Table A
    ifstream f;
    char filename[100];
    bool the_end=false; // Means that the solution was found
    double sum = 0.0; // variable for counting sum of survival coefficients each gen
    double suit_defect = LONG_MAX; // It's needed for case than solution won't be found but program will keep min defect in this variable
    int cond; // It's for switch

    cout << "Program for solving system of linear equations. Version: 1.02." << endl;
    loop: // It's the first label for GOTO, of course it's also the last
    cout << "Choose 0-3:" << endl << "1 - Input matrix A from file;" << endl << "2 - Input matrix A in console;" << endl << "3 - Help;" << endl << "0 - Exit." << endl;
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
        case 3: { cout << "This program solves Ax=b systems,\n" << "-- A - square matrix with dimension dim(<=10 - it can be changed in source code), also matrix A is extended (b is included);\n";
                cout << "-- x - vector-column;\n"; goto loop; }
        case 0: { return 0; }
        default: { cout << "Incorrect input, try again!\n"; goto loop; }
    }

    int amount_of_all_childs=2*(dim-1);
    vector<double> bad_sol(dim);
    vector< vector<double> > future_childs(amount_of_all_childs, vector<double>(dim));
    int best_child1 = -1, best_child2 = -1, p1 = 0, p2 = 0;
    double dtemp, nevas_p1 = 0.0, nevas_p2 = 0.0, dmin, check, bad_defect = LONG_MAX, bad_defect_temp;

    // Creation of first generation
    for(int i=0;i<N;i++)
        for(int j=0;j<dim;j++)
            gen[i].push_back(random());

// There begins iteration process
    for(int m=0;m<N_GEN;m++) {
        // Definition of survival coefficients
        //if(m==1) cout << m << endl;
        sum = 0.0;
        for(int i=0;i<N;i++) {
            for(int j=0;j<dim;j++) {
                if(fabs(mult_AiX(a[j],gen[i])) > surv[i]) surv[i] = fabs(mult_AiX(a[j],gen[i])) + 0.00000000000000000001;
                //surv[i]+=fabs(mult_AiX(a[j],gen[i]));
            }
            sum += 1/(surv[i]);
        }
        for(int i=0;i<N;i++)
            surv[i] = (1/surv[i])/sum;

        //Pairing
        p1=0, p2=0; // Parent1 & Parent 2
        for(int q=0;q<N/2;q++)
        {
            cross_prob(surv, p1, p2);

            // Finding all childs which can have parents p1 and p2
            for(int j=0, t=1, c=0;j<amount_of_all_childs;j++) {
                if(!c) {
                    for(int i=0;i<t;i++)
                        future_childs[j][i] = gen[p1][i];
                    for(int i=t;i<dim;i++)
                        future_childs[j][i] = gen[p2][i];
                    c=1;
                }
                else {
                    for(int i=0;i<t;i++)
                        future_childs[j][i] = gen[p2][i];
                    for(int i=t;i<dim;i++)
                        future_childs[j][i] = gen[p1][i];
                    t++;
                    c=0;
                }
            }

            // finding crossover. Will children survive?
            best_child1 = -1;
            best_child2 = -1;
            nevas_p1 = 0.0;
            nevas_p2 = 0.0;
            for(int j=0;j<dim;j++) {
                if(fabs(mult_AiX(a[j],gen[p1])) > nevas_p1) nevas_p1 = fabs(mult_AiX(a[j],gen[p1]));
                //nevas_p1+=fabs(mult_AiX(a[j],gen[p1]));
            }
            for(int j=0;j<dim;j++) {
                if(fabs(mult_AiX(a[j],gen[p2])) > nevas_p2) nevas_p2 = fabs(mult_AiX(a[j],gen[p2]));
                //nevas_p2+=fabs(mult_AiX(a[j],gen[p2]));
            }
            dmin = nevas_p1;

            for(int i=0;i<amount_of_all_childs;i++) {
                dtemp = 0;
                for(int j=0;j<dim;j++)
                    if(fabs(mult_AiX(a[j],future_childs[i])) > dtemp) dtemp = fabs(mult_AiX(a[j],future_childs[i]));
                    //dtemp+=fabs(mult_AiX(a[j],future_childs[i]));
                if((dtemp < nevas_p1) && (dtemp < nevas_p2) && (dtemp < dmin)) { dmin = dtemp; best_child1 = i; }
            }

            if(best_child1!=-1) {
                dmin = nevas_p1;
                for(int i=0;i<amount_of_all_childs;i++) {
                    if(i==best_child1) continue;
                    dtemp = 0;
                    for(int j=0;j<dim;j++) {
                        if(fabs(mult_AiX(a[j],future_childs[i])) > dtemp) dtemp = fabs(mult_AiX(a[j],future_childs[i]));
                        //dtemp+=fabs(mult_AiX(a[j],future_childs[i]));
                    }
                    if((dtemp < nevas_p1) && (dtemp < nevas_p2) && (dtemp < dmin)) { best_child2 = i;}
                }
            }

            if(best_child1 != -1) {
                if(best_child2 != -1) {
                    gen[p1] = future_childs[best_child1];
                    gen[p2] = future_childs[best_child2];
                }
                else {
                    if(nevas_p1<=nevas_p2) { gen[p2] = future_childs[best_child1]; }
                    else { gen[p1] = future_childs[best_child1]; }
                }
            }
        }

        // Transit to mutation
        for(int i=0;i<N;i++) {
            if(i==p1 || i==p2) continue;
            mutation(gen[i]);
        }

        // Artificial increasing entropy of the population
        for(int j=0;j<N;j++) {
            for(int i=j+1;i<N;i++) {
                int cont=0;
                for(int o=0;o<dim;o++)
                    if(fabs(gen[j][o]-gen[i][o])<EPS_SYM) cont++;
                if(cont==dim) extreme_mutation(gen[i]);
            }
        }

        // Check generation if there is a solution
        for(int i=0;i<N;i++) {
            bad_defect_temp = 0.0;
            check = 0.0;
            for(int j=0;j<dim;j++) {
                check+=fabs(mult_AiX(a[j],gen[i]));
                if(fabs(mult_AiX(a[j],gen[i])) > bad_defect_temp) bad_defect_temp = fabs(mult_AiX(a[j],gen[i]));
            }
            if(bad_defect_temp < bad_defect) { suit_defect = check; bad_sol = gen[i]; bad_defect = bad_defect_temp;}
            if(bad_defect <= EPS2) {
                cout << "Amount of spent generations to find a solution: " << m << endl << "Defect of all equations = " << suit_defect << endl << "Max defect of 1 equation = " << bad_defect << endl;
                for(int j=0;j<dim;j++)
                    printf("x%d = %.2f\n", j, gen[i][j]);
                the_end = true;
                break;
            }
        }
        if(the_end) break;
    }

    if(!the_end){
        printf("Solution with EPS2 = %.2f wasn't found, but this value has defect = %.2f, Max defect of 1 equation = %.2f and it's the best:\n", EPS2, suit_defect, bad_defect);
        for(int j=0;j<dim;j++)
            printf("x%d = %.2f\n", j, bad_sol[j]);
    }

    return 0;
}

void mutation(vector<double> &mem)
{
    for(int i=0;i<dim;i++)
        if( MUT/(1 + rand() % 100) ) mem[i] = random();
}
void extreme_mutation(vector<double> &mem)
{
    for(int i=0;i<dim;i++)
        mem[i] = random();
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

void cross_prob(vector<double> &surv, int &p1, int &p2)
{
    vector<int> dist; // Distribution of indexes each member of gen. for random choose, roulette-wheel selection

    for(int i=0;i<N;i++)
        for(int j=0;j<(int) 100*surv[i];j++) // 100*surv - probability of surviving in percentage
            dist.push_back(i);

    int num=0;
    p1 = dist[rand() % dist.size()]; // Choose random index in dist
    for(int i=0;(unsigned) i<dist.size();i++)
        if(p1 == dist[i]) {num = i; break;}
    dist.erase(dist.begin() + num,dist.begin() + num + (int)100*surv[p1]); // It needs to choose different parents
    p2 = dist[rand() % dist.size()];
}

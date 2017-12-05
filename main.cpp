//
//  main.cpp
//  md
//
//  Created by Jiahao Zhang on 12/3/17.
//  Copyright © 2017 Jiahao Zhang. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "md.h"
#include <iomanip>
#include <stdlib.h>
#include "particle.h"
#include "myrandom.h"
#include <fstream>
extern int N_min;
extern double f_inc;
extern double f_dec;
extern double alpha_start;
extern double f_alpha;
int main(){
    int N=100;//specify the number of particles.
    double Dt=0.01;
    double Dt_max=10*Dt;
    double len;
    double tempr;
    double alpha=alpha_start;
    double fraction=0.8;
    double volume=0.0;
    double e_before=0;
    double e_end=0;
    double pow;
    int count=0;
    particle* allpart=(particle*)malloc(N*sizeof(particle));
    for(size_t i=0;i<N;i++){
        do{
            tempr=gaussian(1,0.5);
        }while(tempr<0);
				tempr=0.2;
        allpart[i].radius=tempr;
        volume=volume+3.141592653*4.0/3.0*tempr*tempr*tempr;
    }
    //calculate the lenght of a cubic
    len=cbrt(volume/fraction);
    std::cout<<"the length of the cubic is: "<<len<<std::endl;
    //initial the position for the system.
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<3;j++){
            allpart[i].posit[j]=len*(genrand()-0.5);
            allpart[i].speed[j]=0.0;
        }
    }
    int i=0;
		updateforce(N,len,allpart);
    do{
        i++;
        e_before=e_end;
        leapfrogone(N,Dt,len,allpart);
        updateforce(N,len,allpart);
        leapfrogtwo(N,Dt,allpart);
        pow=power(N,allpart);
        setv(N,alpha,allpart);
        if(pow>0){
            if(count>N_min){
                Dt=Dt*f_inc<Dt_max ? Dt*f_inc:Dt_max;
                alpha=alpha*f_alpha;
            }
            count=0;
        }
        else{
            count++;
            Dt=Dt*f_dec;
            alpha=alpha_start;
            freeze(N,allpart);
        }
        e_end=energy(N,allpart);
    }while(std::fabs(e_end-e_before)>1e-14);
    std::fstream myfile;
    myfile.open("good.txt",std::fstream::out);
    for(size_t i=0;i<N;i++){
        for(size_t j=0;j<3;j++){
            myfile<<std::setprecision(20)<<allpart[i].posit[j]<<" ";
            }
        myfile<<std::setprecision(20)<<allpart[i].radius<<std::endl;
        }
    
    myfile.close();
}


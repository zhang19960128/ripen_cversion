//
//  md.cpp
//  md
//
//  Created by Jiahao Zhang on 12/3/17.
//  Copyright © 2017 Jiahao Zhang. All rights reserved.
//
#include "particle.h"
#include <iostream>
#include <cmath>
extern int N_min;
extern double f_inc;
extern double f_dec;
extern double alpha_start;
extern double f_alpha;
double energy(int size,particle* allpart){
    double ener=0;
    double rij=0;
    double dij=0;
    double temp;
    for(size_t i=0;i<size;i++)
        for(size_t j=0;j<size;j++){
            for(size_t k=0;k<3;k++){
                temp=allpart[i].posit[k]-allpart[j].posit[k];
                rij=temp*temp;
            }
            rij=sqrt(rij);
            dij=allpart[i].radius+allpart[j].radius;
            if(rij>dij) ener=ener+0;
            else{
                ener=ener+(1-rij/dij)*(1-rij/dij);
            }
        }
    ener=ener/2.0;
    return ener;
};
void updateforceone(int index,int size,double len,particle* allpart){
    double rij=0;
    double dij=0;
    double temp=0;
    double tempforce[3]={0,0,0};
    for(size_t k=0;k<size;k++){
        if(k==index) continue;
        rij=0;
        dij=allpart[index].radius+allpart[k].radius;
        for(size_t i=0;i<3;i++){
            temp=allpart[index].posit[i]-allpart[k].posit[i];
            temp=(temp/len-round(temp/len))*len;
            rij=rij+temp*temp;
            }
        rij=sqrt(rij);
        if(rij>dij){
            for(size_t j=0;j<3;j++)
                tempforce[j]=tempforce[j]+0.0;
            }
        else{
            for(size_t j=0;j<3;j++){
                temp=allpart[index].posit[j]-allpart[k].posit[j];
                temp=(temp/len-round(temp/len))*len;
                tempforce[j]=tempforce[j]+2*(1-rij/dij)*temp/dij/rij;
            }
            }
        }
    for(size_t j=0;j<3;j++){
        allpart[index].force[j]=tempforce[j];
    }
};
void updateforce(int size,double len,particle* allpart){
    for(int k=0;k<size;k++){
        updateforceone(k, size, len, allpart);
    }
}
void leapfrogone(int size,double Dt,double len,particle* allpart){
    double temp;
    for(size_t i=0;i<size;i++){
        for(size_t j=0;j<3;j++){
            temp=allpart[i].posit[j]+allpart[i].speed[j]*Dt+0.5*Dt*Dt*allpart[i].force[j];
            allpart[i].posit[j]=(temp/len-round(temp/len))*len;
            allpart[i].speed[j]=allpart[i].speed[j]+0.5*Dt*allpart[i].force[j];
        }
    }
}
void leapfrogtwo(int size,double Dt,particle* allpart){
    for(size_t i=0;i<size;i++){
        for(size_t j=0;j<3;j++){
            allpart[i].speed[j]=allpart[i].speed[j]+0.5*Dt*allpart[i].force[j];
        }
    }
}
double power(int size,particle* allpart){
    double temp=0;
    for(size_t i=0;i<size;i++){
        for(size_t k=0;k<3;k++){
            temp=temp+allpart[i].speed[k]*allpart[i].force[k];
        }
    }
    return temp;
}
void freeze(int size,particle* allpart){
    for(size_t i=0;i<size;i++){
        for(size_t k=0;k<3;k++){
            allpart[i].speed[k]=0;
        }
    }
}
void setv(int size,double alpha,particle* allpart){
    double normf=0;
    double normv=0;
    double temp;
    for(size_t i=0;i<size;i++){
        for(size_t k=0;k<3;k++){
            temp=allpart[i].force[k];
            normf=temp*temp+normf;
            temp=allpart[i].speed[k];
            normv=temp*temp+normv;
        }
    }
    normf=sqrt(normf);
    normv=sqrt(normv);
    for(size_t i=0;i<size;i++){
        for(size_t k=0;k<3;k++){
            allpart[i].speed[k]=(1-alpha)*allpart[i].speed[k]+alpha*allpart[i].force[k]/normf*normv;
        }
    }
}
void jamming(int N,double len,double Dt,double alpha,particle* allpart){
	double e_before,e_end;
	updateforce(N,len,allpart);
	int count=0;
	double Dt_max=10*Dt;
	double pow;
	do{
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
}

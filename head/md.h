//
//  md.h
//  md
//
//  Created by Jiahao Zhang on 12/3/17.
//  Copyright © 2017 Jiahao Zhang. All rights reserved.
//

#ifndef md_h
#define md_h
#include "particle.h"
double energy(int,particle*);
void leapfrogone(int,double,double,particle*);
void leapfrogtwo(int,double,particle*);
void updateforce(int,double,particle*);
void updateforceone(int,int,double,particle*);
double power(int,particle*);
void freeze(int,particle*);
void setv(int,double,particle*);
#endif /* md_h */

//
//  particle.h
//  md
//
//  Created by Jiahao Zhang on 12/3/17.
//  Copyright © 2017 Jiahao Zhang. All rights reserved.
//

#ifndef particle_h
#define particle_h
typedef struct particle{
    double posit[3];
    double speed[3];
    double force[3];
    double radius;
}particle;
#endif /* particle_h */

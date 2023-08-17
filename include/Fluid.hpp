#ifndef _FLUID_HPP_
#define _FLUID_HPP_

#include <vector>
#include <Sphere.h>
#include <Particle.hpp>


class Fluid {
public:
    Fluid( void );

    //calculates the fluid properties
    void simulate( void );

    //add more fluid particles
    void addParticles( void );          //to be defined

    //reset the simulation
    void resetSimulation( void );       //to be defined

    //parameters calculators
    float calcDensity( Vector3f );
    float calcPressure( float );
    Vector3f calcPressureForce( int, float, float, Vector3f );
    Vector3f calcViscosityForce( int, Vector3f, Vector3f );
    Vector3f calcGravitationalForce( float );
    Vector3f calcSurfaceNormal( Vector3f );
    Vector3f calcSurfaceTensionForce( Vector3f, Vector3f );
    void employEulerIntegrator( Particle &, Vector3f );
    bool detectCollision( Particle, Vector3f &, Vector3f & );
    void updateVelocity( Vector3f &, Vector3f, float );

    float useDefaultKernel( Vector3f, float );
    Vector3f useDefaultKernel_gradient( Vector3f, float );
    float useDefaultKernel_laplacian( Vector3f, float );
    Vector3f usePressureKernel_gradient( Vector3f, float );
    float useViscosityKernel_laplacian( Vector3f, float );


    Sphere sphere;
    vector<Particle> mParticles;

    //user option
    bool startSimulation;

    //constant parameters of the fluid
    float particleRadius;
    float REST_DENSITY        =       998.29f;  //density at rest condition
    float MASS                =       0.02f;    //mass
    float VISCOSITY           =       3.5f;     //viscocity 
    float SURFACE_TENSION     =       0.0728f;  //surface tension
    float THRESHOLD           =       7.065f;   //used in surface normal
    float GAS_STIFFNESS       =       3.0f;     //gas stiffness, gas constant
    float RESTITUTION         =       0.5f;     // restitution , used while handling collison
    float KERNEL_PARTICLES    =       20;       // maximum no. of particle we want to include in a kernel
    float SUPPORT_RADIUS      =       0.0457f;  //support radius of the smoothing kernel
    Vector3f GRAVITATIONAL_ACCELERATION;        //acceleration due to gravity

    float TIME_STEP       =           0.005f;
    float BOX_SIZE        =           0.3f;
};

#endif
#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

#include "utility.hpp"

class Particle {
public:
    Particle( float, Vector3f );

    float mMass;
    Vector3f mAcceleration;
    Vector3f mVelocity;
    Vector3f mPosition;
    Vector3f mForce;
    float mDensity;
    float mPressure;
    Vector3f mPressureForce;
    Vector3f mViscosityForce;
    Vector3f mGravitationalForce;
    Vector3f mSurfaceNormal;
    Vector3f mSurfaceTensionForce;
    bool isAtSurface;
    bool isDroppedFirstTime;
};

#endif
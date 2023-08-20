#include "../include/Fluid.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Fluid::Fluid( void ):GRAVITATIONAL_ACCELERATION( 0.0f, -9.81f, 0.0f),
                     sphere(),
                     startSimulation( false ) ,
                     X_BOX_SIZE( BOX_SIZE ),
                     Y_BOX_SIZE( BOX_SIZE ),
                     Z_BOX_SIZE( BOX_SIZE ){
    
    const float fluidVolume      = 1000 * MASS / REST_DENSITY;
    const float particleDiameter = powf(fluidVolume, 1.0f / 3.0f) / 10;
    particleRadius   = particleDiameter / 2;


    sphere.set( particleRadius, 36, 18, {0.21568f, 0.52549f, 0.870588f, 1.0f });    //set the params of the sphere

    //start out with 1000 particles, 10 X 10 X 10
    for (float x = -particleRadius * 9; x <= particleRadius * 9; x += particleDiameter) {
        for (float y = -particleRadius * 9; y <= particleRadius * 9; y += particleDiameter) {
            for (float z = -particleRadius * 9; z <= particleRadius * 9; z += particleDiameter)
                mParticles.push_back(Particle(MASS, Vector3f(x, y, z)));
        }
    }
}

void Fluid::simulate( void ) {
    // Compute density and pressure
    if( startSimulation){
        for (int i = 0; i < mParticles.size(); i++) {
            mParticles[i].mDensity  = calcDensity(mParticles[i].mPosition);
            mParticles[i].mPressure = calcPressure(mParticles[i].mDensity);
        }

        // Compute internal forces
        for (int i = 0; i < mParticles.size(); i++) {
            mParticles[i].mPressureForce  = calcPressureForce(i, mParticles[i].mDensity, mParticles[i].mPressure, mParticles[i].mPosition);
            mParticles[i].mViscosityForce = calcViscosityForce(i, mParticles[i].mVelocity, mParticles[i].mPosition);
        }
        
        // Compute external forces
        for (int i = 0; i < mParticles.size(); i++) {
            mParticles[i].mGravitationalForce = calcGravitationalForce(mParticles[i].mDensity);
            mParticles[i].mSurfaceNormal      = calcSurfaceNormal(mParticles[i].mPosition);
            if (mParticles[i].mSurfaceNormal.length() >= THRESHOLD){
                mParticles[i].isAtSurface = true;
                mParticles[i].mSurfaceTensionForce = calcSurfaceTensionForce(mParticles[i].mSurfaceNormal, mParticles[i].mPosition);
            }
            else{
                mParticles[i].mSurfaceTensionForce = Vector3f(0.0f, 0.0f, 0.0f);
                mParticles[i].isAtSurface = false;
            }
        }

        // Time integration and collision handling
        Vector3f totalForce;
        for (int i = 0; i < mParticles.size(); i++) {
            //totalForce = mParticles[i].mPressureForce + mParticles[i].mViscosityForce + mParticles[i].mSurfaceTensionForce;
            totalForce = mParticles[i].mPressureForce + mParticles[i].mViscosityForce + mParticles[i].mGravitationalForce + mParticles[i].mSurfaceTensionForce;
            employEulerIntegrator(mParticles[i], totalForce);

            Vector3f contactPoint;
            Vector3f unitSurfaceNormal;
            if (mParticles[i].isAtSurface && detectCollision(mParticles[i], contactPoint, unitSurfaceNormal)) {
                updateVelocity(mParticles[i].mVelocity, unitSurfaceNormal, (mParticles[i].mPosition - contactPoint).length());
                mParticles[i].mPosition = contactPoint;
            }
        }
    }
}


void Fluid::addParticles( Vector3f center ){

    //At the set location add the particles 
    for (float x = -particleRadius * 2; x <= particleRadius * 2; x += 2 * particleRadius) {
        for (float y = -particleRadius * 2; y <= particleRadius * 2; y += 2 * particleRadius) {
            for (float z = -particleRadius * 2; z <= particleRadius * 2; z += 2 * particleRadius )
                mParticles.push_back(Particle(MASS, Vector3f(x, y, z) + center));
        }
    }
}

void Fluid::resetSimulation( void ){
    std::vector<Particle >().swap(mParticles);      //delete all the existing particles
    startSimulation = false;                        // stop the simulation after resetting

    //reinitialize the particles 
    for (float x = -particleRadius * 9; x <= particleRadius * 9; x += 2 * particleRadius) {
        for (float y = -particleRadius * 9; y <= particleRadius * 9; y += 2 * particleRadius) {
            for (float z = -particleRadius * 9; z <= particleRadius * 9; z += 2 * particleRadius )
                mParticles.push_back(Particle(MASS, Vector3f(x, y, z)));
        }
    }

 
}


float Fluid::calcDensity( Vector3f position ) {
    float sum = 0.0f;
    for (int j = 0; j < mParticles.size(); j++)
        sum += mParticles[j].mMass * useDefaultKernel(position - mParticles[j].mPosition, SUPPORT_RADIUS);
    return sum;
}

float Fluid::calcPressure( float density ) {
    return GAS_STIFFNESS * (density - REST_DENSITY);
}

Vector3f Fluid::calcPressureForce( int indexOfCurrentParticle, float density, float pressure, Vector3f position ) {
    Vector3f sum(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < mParticles.size(); j++) {
        if (j == indexOfCurrentParticle)
            continue;
        sum += usePressureKernel_gradient(position - mParticles[j].mPosition, SUPPORT_RADIUS) * (pressure / (density * density) + mParticles[j].mPressure / (mParticles[j].mDensity * mParticles[j].mDensity)) * mParticles[j].mMass;
    }
    return -(sum * density);
}

Vector3f Fluid::calcViscosityForce( int indexOfCurrentParticle, Vector3f velocity, Vector3f position ) {
    Vector3f sum(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < mParticles.size(); j++) {
        if (j == indexOfCurrentParticle)
            continue;
        sum += (mParticles[j].mVelocity - velocity) * (mParticles[j].mMass / mParticles[j].mDensity) * useViscosityKernel_laplacian(position - mParticles[j].mPosition, SUPPORT_RADIUS);
    }
    return sum * VISCOSITY;
}

Vector3f Fluid::calcGravitationalForce( float density ) {
    return GRAVITATIONAL_ACCELERATION * density;
}

Vector3f Fluid::calcSurfaceNormal( Vector3f position ) {
    Vector3f sum(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < mParticles.size(); j++)
        sum += useDefaultKernel_gradient(position - mParticles[j].mPosition, SUPPORT_RADIUS) * (mParticles[j].mMass / mParticles[j].mDensity);
    return sum;
}

Vector3f Fluid::calcSurfaceTensionForce( Vector3f surfaceNormal, Vector3f position ) {
    float sum = 0.0f;
    for (int j = 0; j < mParticles.size(); j++)
        sum += (mParticles[j].mMass / mParticles[j].mDensity) * useDefaultKernel_laplacian(position - mParticles[j].mPosition, SUPPORT_RADIUS);
    return -(surfaceNormal.normalize() * SURFACE_TENSION * sum);
}

void Fluid::employEulerIntegrator( Particle &particle, Vector3f totalForce ) {
    particle.mAcceleration = totalForce / particle.mDensity;
    particle.mVelocity     = particle.mVelocity + particle.mAcceleration * TIME_STEP;
    particle.mPosition     = particle.mPosition + particle.mVelocity * TIME_STEP;
}
bool Fluid::detectCollision( Particle &particle, Vector3f &contactPoint, Vector3f &unitSurfaceNormal ) {
    
    //return false if all the coordinates lie between the bounding box
    if (abs(particle.mPosition.x ) + particleRadius <= X_BOX_SIZE / 2 && abs(particle.mPosition.y) + particleRadius <= Y_BOX_SIZE / 2 && abs(particle.mPosition.z) + particleRadius <= Z_BOX_SIZE / 2)
        return false;
    
    // Vector3f tempCordinate ={ abs(particle.mPosition.x ) - X_BOX_SIZE / 2, abs( particle.mPosition.y ) - Y_BOX_SIZE / 2, abs(particle.mPosition.z ) - Z_BOX_SIZE / 2};
    // Vector3f tempCordinate = particle.mPosition; 

    // float x, y, z;
    // std::cout<<particle.mPosition<<std::endl;
    
    // x = abs(particle.mPosition.x  > 0 ?  (particle.mPosition.x - X_BOX_SIZE / 2.0f ) :(particle.mPosition.x +  X_BOX_SIZE / 2.0f));
    // y = abs(particle.mPosition.y > 0 ?( particle.mPosition.y -Y_BOX_SIZE / 2.0f ):(particle.mPosition.y + Y_BOX_SIZE / 2.0f));
    // z = abs(particle.mPosition.z > 0 ?(particle.mPosition.z -Z_BOX_SIZE / 2.0f ):( particle.mPosition.z +  Z_BOX_SIZE / 2.0f));

    // std::cout<<x<< ", "<<y<<", "<<z<<std::endl;

    // char maxComponent = particle.mPosition.x >  (particle.mPosition.y)?
    //                     x >  z  ? 'x' : 'z' :
    //                     y >  z  ? 'y' : 'z';

    char maxComponent = abs( particle.mPosition.x ) > abs(particle.mPosition.y) ?
                        abs( particle.mPosition.x) > abs(particle.mPosition.z) ? 'x': 'y':
                        abs(particle.mPosition.y ) > abs(particle.mPosition.z) ? 'y' : 'z';


    // std::cout<<maxComponent<<std::endl;
    
    // 'unitSurfaceNormal' is based on the current position component with the largest absolute value
    switch( maxComponent ){
        case 'x':
            if ((particle.mPosition.x - particleRadius) < -X_BOX_SIZE / 2) {
                contactPoint = particle.mPosition;            contactPoint.x = particleRadius-X_BOX_SIZE / 2;
                if (( particle.mPosition.y - particleRadius) < -Y_BOX_SIZE / 2)     contactPoint.y = particleRadius-Y_BOX_SIZE / 2;
                else if (( particle.mPosition.y + particleRadius) > Y_BOX_SIZE / 2) contactPoint.y =  -particleRadius + Y_BOX_SIZE / 2;
                if ((particle.mPosition.z - particleRadius ) < -Z_BOX_SIZE / 2)     contactPoint.z = particleRadius-Z_BOX_SIZE / 2;
                else if ((particle.mPosition.z + particleRadius ) > Z_BOX_SIZE / 2) contactPoint.z =  -particleRadius + Z_BOX_SIZE / 2;
                unitSurfaceNormal = Vector3f( 1.0f,  0.0f,  0.0f);
                return true;
            }
            else if( (particle.mPosition.x + particleRadius)> X_BOX_SIZE / 2 ){
                contactPoint = particle.mPosition;            contactPoint.x =  -particleRadius + X_BOX_SIZE / 2;
                if (( particle.mPosition.y - particleRadius) < -Y_BOX_SIZE / 2)     contactPoint.y = particleRadius-Y_BOX_SIZE / 2;
                else if (( particle.mPosition.y + particleRadius) > Y_BOX_SIZE / 2) contactPoint.y = -particleRadius + Y_BOX_SIZE / 2;
                if ((particle.mPosition.z - particleRadius ) < -Z_BOX_SIZE / 2)     contactPoint.z = particleRadius-Z_BOX_SIZE / 2;
                else if ((particle.mPosition.z + particleRadius ) > Z_BOX_SIZE / 2) contactPoint.z =  -particleRadius + Z_BOX_SIZE / 2;
                unitSurfaceNormal = Vector3f(-1.0f,  0.0f,  0.0f);
                return true;
            }
            break;
        case 'y':
            if ((particle.mPosition.y - particleRadius)< -Y_BOX_SIZE / 2) {
                if( particle.isDroppedFirstTime ){
                    particle.isDroppedFirstTime = false;
                }
                contactPoint = particle.mPosition;            contactPoint.y = particleRadius-Y_BOX_SIZE / 2;
                if ((particle.mPosition.x - particleRadius) < -X_BOX_SIZE / 2)     contactPoint.x = particleRadius-X_BOX_SIZE / 2;
                else if ((particle.mPosition.x + particleRadius) > X_BOX_SIZE / 2) contactPoint.x =  -particleRadius + X_BOX_SIZE / 2;
                if ((particle.mPosition.z - particleRadius ) < -Z_BOX_SIZE / 2)     contactPoint.z = particleRadius-Z_BOX_SIZE / 2;
                else if ((particle.mPosition.z + particleRadius ) > Z_BOX_SIZE / 2) contactPoint.z =  -particleRadius + Z_BOX_SIZE / 2;
                unitSurfaceNormal = Vector3f( 0.0f,  1.0f,  0.0f);
                return true;
            }
            else if (( particle.mPosition.y + particleRadius)> Y_BOX_SIZE / 2){
                if( particle.isDroppedFirstTime ){
                    return false;
                }
                contactPoint = particle.mPosition;            contactPoint.y =  -particleRadius + Y_BOX_SIZE / 2;
                if ((particle.mPosition.x - particleRadius) < -X_BOX_SIZE / 2)     contactPoint.x = particleRadius-X_BOX_SIZE / 2;
                else if ((particle.mPosition.x + particleRadius) > X_BOX_SIZE / 2) contactPoint.x = -particleRadius + X_BOX_SIZE / 2;
                if ((particle.mPosition.z - particleRadius )< -Z_BOX_SIZE / 2)     contactPoint.z = particleRadius-Z_BOX_SIZE / 2;
                else if ((particle.mPosition.z + particleRadius )> Z_BOX_SIZE / 2) contactPoint.z =  -particleRadius + Z_BOX_SIZE / 2;
                unitSurfaceNormal = Vector3f( 0.0f, -1.0f,  0.0f);
                return true;
            }
        default:
            if ((particle.mPosition.z - particleRadius ) < -Z_BOX_SIZE / 2) {
                contactPoint = particle.mPosition;            contactPoint.z = particleRadius-Z_BOX_SIZE / 2;
                if ((particle.mPosition.x - particleRadius) < -X_BOX_SIZE / 2)     contactPoint.x = particleRadius-X_BOX_SIZE / 2;
                else if ((particle.mPosition.x + particleRadius) > X_BOX_SIZE / 2) contactPoint.x = -particleRadius+ X_BOX_SIZE / 2;
                if (( particle.mPosition.y - particleRadius) < -Y_BOX_SIZE / 2)     contactPoint.y = particleRadius-Y_BOX_SIZE / 2;
                else if (( particle.mPosition.y + particleRadius)> Y_BOX_SIZE / 2) contactPoint.y = -particleRadius + Y_BOX_SIZE / 2;
                unitSurfaceNormal = Vector3f( 0.0f,  0.0f,  1.0f);
                return true;
            }
            else if( (particle.mPosition.z + particleRadius ) >Z_BOX_SIZE / 2 ){
                contactPoint = particle.mPosition;            contactPoint.z =  -particleRadius + Z_BOX_SIZE / 2;
                if ((particle.mPosition.x - particleRadius)  < -X_BOX_SIZE / 2)     contactPoint.x = particleRadius-X_BOX_SIZE / 2;
                else if ((particle.mPosition.x + particleRadius) > X_BOX_SIZE / 2) contactPoint.x = -particleRadius + X_BOX_SIZE / 2;
                if (( particle.mPosition.y - particleRadius) < -Y_BOX_SIZE / 2)     contactPoint.y = particleRadius-Y_BOX_SIZE / 2;
                else if (( particle.mPosition.y + particleRadius) > Y_BOX_SIZE / 2) contactPoint.y = -particleRadius + Y_BOX_SIZE / 2;
                unitSurfaceNormal = Vector3f( 0.0f,  0.0f, -1.0f);
            }

    }
    return true;
}
void Fluid::updateVelocity( Vector3f &velocity, Vector3f unitSurfaceNormal, float penetrationDepth ) {
    velocity = velocity - unitSurfaceNormal * (1 + RESTITUTION * penetrationDepth / (TIME_STEP * velocity.length())) * velocity.dot(unitSurfaceNormal);
}

float Fluid::useDefaultKernel( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return 0.0f;
    else
        return (315 / (64 * M_PI * powf(supportRadius, 9.0f))) * powf(supportRadius * supportRadius - dist * dist, 3.0f);
}

Vector3f Fluid::useDefaultKernel_gradient( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return Vector3f(0.0f, 0.0f, 0.0f);
    else
        return -(distVector * (945 / (32 * M_PI * powf(supportRadius, 9.0f))) * powf(supportRadius * supportRadius - dist * dist, 2.0f));
}

float Fluid::useDefaultKernel_laplacian( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return 0.0f;
    else
        return -(945 / (32 * M_PI * powf(supportRadius, 9.0f))) * (supportRadius * supportRadius - dist * dist) * (3 * supportRadius * supportRadius - 7 * dist * dist);
}

Vector3f Fluid::usePressureKernel_gradient( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return Vector3f(0.0f, 0.0f, 0.0f);
    else if (dist < 10e-5) // If ||r|| -> 0+
        return -(Vector3f(1.0f, 1.0f, 1.0f).normalize() * (45 / (M_PI * powf(supportRadius, 6.0f))) * powf(supportRadius - dist, 2.0f));
    else
        return -(distVector.normalize() * (45 / (M_PI * powf(supportRadius, 6.0f))) * powf(supportRadius - dist, 2.0f));
}

float Fluid::useViscosityKernel_laplacian( Vector3f distVector, float supportRadius ) {
    float dist = distVector.length();
    if (dist > supportRadius)
        return 0.0f;
    else
        return (45 / (M_PI * powf(supportRadius, 6.0f))) * (supportRadius - dist);
}
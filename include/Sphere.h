///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and the min number of stacks are 2.
// The default up axis is +Z axis. You can change the up axis with setUpAxis():
// X=1, Y=2, Z=3.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2023-03-11
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <vector>
#include <glm/glm.hpp>

class Sphere
{
public:
    // ctor/dtor
    Sphere(float radius=1.0f, int sectorCount=18, int stackCount=9,  glm::vec4 color_param = { 0.0f, 0.0f, 1.0f, 1.0f });
    ~Sphere() {}

    // getters/setters
    float getRadius() const                 { return radius; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    void set(float radius, int sectorCount, int stackCount);
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);

    // for vertex data
    unsigned int getVertexCount() const     { return (unsigned int)vertices.size() / 3; }
    unsigned int getNormalCount() const     { return (unsigned int)normals.size() / 3; }
    unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
    unsigned int getTriangleCount() const   { return getIndexCount() / 3; }
    unsigned int getVertexSize() const      { return (unsigned int)vertices.size() * sizeof(float); }
    unsigned int getNormalSize() const      { return (unsigned int)normals.size() * sizeof(float); }
    unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const float* getVertices() const        { return vertices.data(); }
    const float* getNormals() const         { return normals.data(); }
    const unsigned int* getIndices() const  { return indices.data(); }

    // for interleaved vertices: V/N/T
    unsigned int getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
    unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes
    const float* getInterleavedVertices() const     { return interleavedVertices.data(); }


    // debug
    void printSelf() const;

protected:

private:
    // member functions
    void buildVertices();
    void buildInterleavedVertices();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    //color
    glm::vec4 color;
    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

};

#endif

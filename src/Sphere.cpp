#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <glad/glad.h>
#endif

#include <iostream>
#include <iomanip>
#include <cmath>
#include <Sphere.h>

#define BOX_SIZES                   0.15f


// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT  = 2;



// ctor
//default constructor
Sphere::Sphere(){}

Sphere::Sphere(float radius, int sectors, int stacks, glm::vec4 color_param):color( color_param)
{
    set(radius, sectors, stacks, color_param);
}



// setters
void Sphere::set(float radius, int sectors, int stacks, glm::vec4 color_param)
{
    color = color_param;
    if(radius > 0)
        this->radius = radius;
    this->sectorCount = sectors;
    if(sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if(stacks < MIN_STACK_COUNT)
        this->stackCount = MIN_STACK_COUNT;
    buildVertices();
}

void Sphere::setRadius(float radius)
{
    if(radius != this->radius)
        set(radius, sectorCount, stackCount, color);
}

void Sphere::setSectorCount(int sectors)
{
    if(sectors != this->sectorCount)
        set(radius, sectors, stackCount, color);
}

void Sphere::setStackCount(int stacks)
{
    if(stacks != this->stackCount)
        set(radius, sectorCount, stacks, color);
}





// print itself
void Sphere::printSelf() const
{
    std::cout << "===== Sphere =====\n"
              << "        Radius: " << radius << "\n"
              << "  Sector Count: " << sectorCount << "\n"
              << "   Stack Count: " << stackCount << "\n"
              << "Triangle Count: " << getTriangleCount() << "\n"
              << "   Index Count: " << getIndexCount() << "\n"
              << "  Vertex Count: " << getVertexCount() << "\n"
              << "  Normal Count: " << getNormalCount() << "\n";
}

//build the vertices off of the parameters set set
void Sphere::buildVertices()
{
    const float PI = acos(-1.0f);



    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);

        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
                addIndices(k1, k2, k1+1);   // k1---k2---k1+1
            }

            if(i != (stackCount-1))
            {
                addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
            }

        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();

}



// generate interleaved vertices: V/N/T
void Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);
    
    float X_BOX_SIZE = BOX_SIZES;
    float Y_BOX_SIZE = BOX_SIZES;
    float Z_BOX_SIZE = BOX_SIZES;

    std::size_t i, j;
    std::size_t count = vertices.size();

    glm::vec4 boxColor({0.0f, 0.0f, 0.0f, 1.0f });

    //another vertex
    //( -x, -y, -z )
    //1
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( -x , -y, z )
    //2
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);
    
    //another vertex
    // ( -x. -y , -z )
    //3
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // (x, -y, -z )
    //4
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( -x. -y , -z )
    //5
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    //( -x , y, -z )
    //6
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back(Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, y , -z)
    //7
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x. -y, -z )
    //8
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, y, -z )
    //9
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( -x, y, -z )
    //10
    interleavedVertices.push_back(-X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, y, -z )
    //11
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, y, z)
    //12
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, -y, z )
    //13
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( -x, -y,  z )
    // 14
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, -y, z )
    //15
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, y, z)
    //16
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // (x, -y, z)
    // 17
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, -y, -z )
    //18
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( -x, y, z )
    //19
    interleavedVertices.push_back(-X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( -x, -y, z)
    //20
    interleavedVertices.push_back(-X_BOX_SIZE );
    interleavedVertices.push_back(-Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // (-x, y, z )
    //21
    interleavedVertices.push_back( -X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    //(-x, y, -z)
    //22
    interleavedVertices.push_back(-X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back(-Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // (-x, y, z)
    //23
    interleavedVertices.push_back(-X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);

    //another vertex
    // ( x, y, z )
    //24
    interleavedVertices.push_back( X_BOX_SIZE );
    interleavedVertices.push_back( Y_BOX_SIZE);
    interleavedVertices.push_back( Z_BOX_SIZE);

    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);
    interleavedVertices.push_back(1.0f);


    interleavedVertices.push_back(boxColor[0]);
    interleavedVertices.push_back(boxColor[1]);
    interleavedVertices.push_back(boxColor[2]);
    interleavedVertices.push_back(boxColor[3]);



    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);


        interleavedVertices.push_back(color[0]);
        interleavedVertices.push_back(color[1]);
        interleavedVertices.push_back(color[2]);
        interleavedVertices.push_back(color[3]);

    }
}



// add single vertex to array
void Sphere::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}



// add single normal to array
void Sphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}



// add 3 indices to array
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

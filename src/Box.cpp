#include<Box.hpp>

//ctor and dtor
Box::Box(){}
Box::~Box(){}

void Box::buildVertices( std::vector<float>& destination ){
    float X_BOX_SIZE = 0.15f, Y_BOX_SIZE = 0.15f, Z_BOX_SIZE = 0.15f;
    float boxColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //another vertex
    // ( -x, -y, -z )
    //1
    destination.push_back( -X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( -x , -y, z )
    //2
    destination.push_back( -X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);
    
    //another vertex
    // ( -x. -y , -z )
    //3
    destination.push_back( -X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // (x, -y, -z )
    //4
    destination.push_back( X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( -x. -y , -z )
    //5
    destination.push_back( -X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    //( -x , y, -z )
    //6
    destination.push_back( -X_BOX_SIZE );
    destination.push_back(Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, y , -z)
    //7
    destination.push_back( X_BOX_SIZE );
    destination.push_back(Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x. -y, -z )
    //8
    destination.push_back( X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, y, -z )
    //9
    destination.push_back( X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( -x, y, -z )
    //10
    destination.push_back(-X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, y, -z )
    //11
    destination.push_back( X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, y, z)
    //12
    destination.push_back( X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, -y, z )
    //13
    destination.push_back( X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( -x, -y,  z )
    // 14
    destination.push_back( -X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, -y, z )
    //15
    destination.push_back( X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, y, z)
    //16
    destination.push_back( X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // (x, -y, z)
    // 17
    destination.push_back( X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, -y, -z )
    //18
    destination.push_back( X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( -x, y, z )
    //19
    destination.push_back(-X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( -x, -y, z)
    //20
    destination.push_back(-X_BOX_SIZE );
    destination.push_back(-Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // (-x, y, z )
    //21
    destination.push_back( -X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    //(-x, y, -z)
    //22
    destination.push_back(-X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back(-Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // (-x, y, z)
    //23
    destination.push_back(-X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

    //another vertex
    // ( x, y, z )
    //24
    destination.push_back( X_BOX_SIZE );
    destination.push_back( Y_BOX_SIZE);
    destination.push_back( Z_BOX_SIZE);

    destination.push_back(1.0f);
    destination.push_back(1.0f);
    destination.push_back(1.0f);


    destination.push_back(boxColor[0]);
    destination.push_back(boxColor[1]);
    destination.push_back(boxColor[2]);
    destination.push_back(boxColor[3]);

}

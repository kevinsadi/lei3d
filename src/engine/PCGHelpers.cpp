#include "include/pcg/PCGHelpers.hpp"

namespace kek3d 
{

    PlaneMesh* createPlaneMesh()
    {
        /*     DEFINE PLANE      */
        const int dim = 128;

        float groundPlaneVertices[dim*dim*5];
        unsigned int groundPlaneIndices[(dim - 1) * (dim - 1) * 6];
        
        createGroundPlane(groundPlaneVertices, dim);
        createGroundPlaneTris(groundPlaneIndices, dim);

        unsigned int planeVAO;
        unsigned int planeVBO;
        unsigned int planeEBO;

        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glGenBuffers(1, &planeEBO);

        glBindVertexArray(planeVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundPlaneVertices), groundPlaneVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundPlaneIndices), groundPlaneIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(1);

        PlaneMesh* planeMesh = new PlaneMesh{planeVAO, planeVBO, planeEBO};
        return planeMesh;
    }


    std::vector<unsigned char> getElevationData()
    {
        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char* data = stbi_load("./data/textures/elevation.png", &width, &height, &nrChannels, 0);
        if (!data)
        {
            std::cout << "ERROR: Elevation loading failed" << std::endl;
        }

        std::vector<unsigned char> imageVec;
        imageVec.assign(data, data + width * height);
        stbi_image_free(data); // no memory leaks here, no sir

        return imageVec;
    }

    /*
    * Takes array by reference and populates them with the vertices to create a square ground plane made by vertices
    * 
    * Array should have size dim*dim*(3+2)           - explanation: dim*dim vertices. 3 vert points + 2 uv coords per vertex
    * 
    * @param groundPlaneVertices - 
    * @param dim - this should be 128 for our purposes
    */
    void createGroundPlane(float* groundPlaneVertices, int dim)
    {
        std::vector<unsigned char> elevation = getElevationData();
        int vertItr = 0;

        // okay now lets try to make these ground plane verts and indices, but not manually
        for (int vertX = 0; vertX < dim; ++vertX)
        {
            for (int vertY = 0; vertY < dim; ++vertY)
            {
                int vertArrIndex = vertX * 5 + (vertY * 5 * dim); // start of this vertex in the array
                // vertex position
                groundPlaneVertices[vertArrIndex] = float(vertX);
                groundPlaneVertices[++vertArrIndex] = elevation[vertItr];
                groundPlaneVertices[++vertArrIndex] = float(vertY);
                // uvs
                groundPlaneVertices[++vertArrIndex] = float(vertX) / dim;
                groundPlaneVertices[++vertArrIndex] = float(vertY) / dim;

                vertItr++;
            }
        }


    }

    /*
    * Takes in array by reference and populates it with the values to create an EBO
    *
    * Array should have size (dim-1)*(dim-1)*6.              - explanation = this gives us cubes * cubes. 2 tris per cube. 3 indices per tri.
    *
    * @param groundPlaneIndices
    * @param dim - this should be 128 for our purposes
    */
    void createGroundPlaneTris(unsigned int* groundPlaneIndices, int dim)
    {
        int colCheck = 0;
        int tNum = 0;

        // okay now lets try to make these ground plane verts and indices, but not manually
        for (int vi = 0; vi < (dim * dim) - dim; vi++)
        {
            colCheck += 1;
            if (colCheck % dim != 0) // do not include vertices on the last column
            {
                groundPlaneIndices[tNum]   = vi+1;     // right of current index
                groundPlaneIndices[tNum+1] = vi+dim+1; // right of directly above index
                groundPlaneIndices[tNum+2] = vi;       // current index

                groundPlaneIndices[tNum+3] = vi;       // current index
                groundPlaneIndices[tNum+4] = vi+dim+1; // right of directly above index
                groundPlaneIndices[tNum+5] = vi+dim;   // directly above index

                tNum += 6;
            }
        }
    }


}
#include "include/pcg/PCGHelpers.hpp";

namespace kek3d 
{

    std::vector<unsigned char>* getElevationData()
    {
        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char* data = stbi_load("./data/textures/elevation.png", &width, &height, &nrChannels, 0);
        if (!data)
        {
            std::cout << "ERROR: Elevation loading failed" << std::endl;
        }

        std::vector<unsigned char>* imageVec = new std::vector<unsigned char>;
        imageVec->assign(data, data + width * height);
        stbi_image_free(data); // no memory leaks here, no sir

        return imageVec;
    }

    /*
    * Takes in two arrays by reference and populates them with the vertices to create a square ground plane made by vertices
    * 
    * 
    * Array should have size dim*dim*(3+2)           - explanation: dim*dim vertices. 3 vert points + 2 uv coords per vertex
    * 
    * @param groundPlaneVertices - 
    * @param groundPlaneUVs -
    * @param dim - this should be 128 for our purposes
    */
    void createGroundPlane(float* groundPlaneVertices, int dim)
    {
        std::vector<unsigned char>* elevation = getElevationData();
        int vertItr = 0;

        // okay now lets try to make these ground plane verts and indices, but not manually
        for (int vertX = 0; vertX < dim; ++vertX)
        {
            for (int vertY = 0; vertY < dim; ++vertY)
            {
                int vertArrIndex = vertX * 5 + (vertY * 5 * dim); // start of this vertex in the array
                // vertex position
                groundPlaneVertices[vertArrIndex] = float(vertX);
                groundPlaneVertices[++vertArrIndex] = (*elevation)[vertItr];
                groundPlaneVertices[++vertArrIndex] = float(vertY);
                // uvs
                groundPlaneVertices[++vertArrIndex] = float(vertX) / dim;
                groundPlaneVertices[++vertArrIndex] = float(vertY) / dim;

                vertItr++;
            }
        }
    }

    /*
    * Takes in array by reference and populates it with the 
    *
    * Array should have size (dim-1)*(dim-1)*6.              - explanation = this gives us cubes * cubes. 2 tris per cube. 3 indices per tri.
    *
    * @param groundPlaneVertices -
    * @param groundPlaneUVs -
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
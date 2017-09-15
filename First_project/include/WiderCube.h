#ifndef WIDERCUBE_H
#define WIDERCUBE_H

#include "Include.h"
#include "Object.h"
#include <glm/glm.hpp>

class WiderCube: public Object
{
    public:
        WiderCube();

        void setVertices(GLfloat[]);

        void setVAO_VBO();
        void show(GLuint WIDTH, GLuint HEIGHT, GLuint dimension, glm::mat4 projection, glm::mat4 view);
};


#endif


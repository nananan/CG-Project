#ifndef SMALLCUBE_H
#define SMALLCUBE_H

#include "Include.h"
#include "Property.h"
#include "Object.h"

#include <glm/glm.hpp>

class SmallCube: public Object
{
    public:
        SmallCube();

        void setVertices(GLfloat[]);

        void setVAO_VBO();
        void show(GLuint WIDTH, GLuint HEIGHT, GLfloat dimension, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos, glm::vec3 cameraPos);

//        void updateRotate(float updateRotate, bool add);

};


#endif


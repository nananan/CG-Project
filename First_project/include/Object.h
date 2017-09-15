#ifndef OBJECT_H
#define OBJECT_H

#include "Include.h"
#include "Property.h"
#include <glm/glm.hpp>

class Object
{
    protected:
        GLfloat vertices[Property::VERTICES_SIZE];
        Shader* shader;
        GLuint VBO, VAO;

        float rotate;

    public:
        Object();

        Shader* getShader();

        GLuint getVAO();
        void setVAO_VBO();
        void deleteVAO_VBO();

        void setVAO(GLuint VAO);
        void setVBO(GLuint VBO);

        void setLightingShader(Shader* shader, glm::vec3 lightPos, glm::vec3 cameraPos, glm::mat4 projection, glm::mat4 view);

        void updateRotate(float updateRotate, bool add);
};


#endif


#ifndef PARTICLE_H
#define PARTICLE_H

#include "Include.h"

#define STEP 0.05f
#define DIMENSION 1.0f

class Particle
{
    private:
        float x;
        float y;
        float z;
        float rotate;
        GLuint VAO, VBO;
        float range = (DIMENSION) - 0.05f;

        float randomPosition(float min, float max);

    public:
        Particle();

        glm::vec3 getPosition();
        void update(int i);

        GLuint getVAO();
        void deleteVAO_VBO();
        void setVAO_VBO(int size);

        void updateRotate(float updateRotate, bool add);

        void show(Shader shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos);
};

#endif




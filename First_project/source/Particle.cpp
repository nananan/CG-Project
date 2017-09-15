#include "../include/Particle.h"

Particle::Particle() {
    this->x = randomPosition(-range,range)/3;
    this->y = randomPosition(-range,range)/3;
    this->z = randomPosition(-range,range)/3;
}

glm::vec3 Particle::getPosition() {
   return glm::vec3(x,y,z);
}

float Particle::randomPosition(float min, float max){
    float random = ((float) rand()/ (float) RAND_MAX);
    float range = max - min;
    return (random * range) + min;
}

void Particle::update(int i) {

    glm::vec3 move (randomPosition(-STEP, STEP), randomPosition(-STEP, STEP), randomPosition(-STEP, STEP));
    this->x += move[0];
    this->y += move[1];
    this->z += move[2];

    float limit = (DIMENSION)- 0.02f;
    if (this->x > limit)
        this->x -= move[0];
    else if (this->x <= -limit)
        this->x += move[0]*-1;

    if (this->y > limit)
        this->y -= move[1];
    else if (this->y <= -limit)
        this->y += move[1]*-1;

    if (this->z > limit)
        this->z -= move[2];
    else if (this->z <= -limit)
        this->z += move[2]*-1;

}

void Particle::setVAO_VBO(int size) {

    glGenVertexArrays(1, &VAO); // vertex array object
    glGenBuffers(1, &VBO); // vertex buffer object

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

}

GLuint Particle::getVAO() {
    return VAO;
}

void Particle::deleteVAO_VBO() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}




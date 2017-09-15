#include "../include/Particle.h"

Particle::Particle()
{
    this->rotate = 55.f;

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

void Particle::show(Shader shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 cameraPos) {
//    glUniform3f( glGetUniformLocation( shader.Program, "uColor" ), 1.0, 1.0, 1.0 );

    glUniform3f(glGetUniformLocation(shader.Program, "objectColor"),  1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shader.Program, "lightColor"),  1.0f, 1.0f, 1.0f);

    glUniform3f(glGetUniformLocation(shader.Program, "lightPos"),  lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shader.Program, "viewPos"),  cameraPos.x, cameraPos.y, cameraPos.z);

    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // world transformation
    glm::mat4 model;
//    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
//    model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
    model = glm::translate(model, glm::vec3(x,y,z));
//    model = glm::rotate(model, glm::radians(rotate), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
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

void Particle::updateRotate(float updateRotate, bool add) {
    if (add)
        rotate += updateRotate;
    else
        rotate -= updateRotate;
}



#include "../include/Include.h"
#include "../include/Object.h"

Object::Object() {
    rotate = 55.0f;
}

GLuint Object::getVAO() {
    return VAO;
}

void Object::setVAO_VBO() {

    glGenVertexArrays(1, &VAO); // vertex array object
    glGenBuffers(1, &VBO); // vertex buffer object

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Object::deleteVAO_VBO() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Shader* Object::getShader() {
    return this->shader;
}

void Object::setVAO(GLuint VAO) {
    VAO = this->VAO;
}

void Object::setVBO(GLuint VBO) {
    VBO = this->VBO;
}

void Object::setLightingShader(Shader* shader, glm::vec3 lightPos, glm::vec3 cameraPos, glm::mat4 projection, glm::mat4 view){
//    glUniform3f(glGetUniformLocation(shader->Program, "objectColor"),  1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shader->Program, "lightColor"),  1.0f, 1.0f, 1.0f);

    glUniform3f(glGetUniformLocation(shader->Program, "lightPos"),  lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shader->Program, "viewPos"),  cameraPos.x, cameraPos.y, cameraPos.z);

    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // world transformation
//    glm::mat4 model  = glm::mat4();
//    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Object::updateRotate(float updateRotate, bool add) {
    if (add)
        this->rotate += updateRotate;
    else
        this->rotate -= updateRotate;
}

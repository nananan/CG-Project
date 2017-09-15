#include "../include/Include.h"
#include "../include/WiderCube.h"

WiderCube::WiderCube() {

    GLfloat vert []=  {

        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,

         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,


        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,

         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,


         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,

         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
      };

    for (int i = 0; i < 72; i++) {
        this->vertices[i] = vert[i];
    }

    this->shader = new Shader("/home/eliana/MEGA/Qt/Init_OpenGL_Core/shaders/widerCube.vs", "/home/eliana/MEGA/Qt/Init_OpenGL_Core/shaders/widerCube.frag");

}


void WiderCube::setVAO_VBO() {

    Object::setVAO_VBO();

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void WiderCube::show(GLuint WIDTH, GLuint HEIGHT,GLuint dimension, glm::mat4 projection, glm::mat4 view){
    glUniform3f(glGetUniformLocation(shader->Program, "uColor"), 0.50f, 0.50f, 0.50f);
    // Create transformations
    glm::mat4 model;
//    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));

    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(shader->Program, "model");
    GLint viewLoc = glGetUniformLocation(shader->Program, "view");
    GLint projLoc = glGetUniformLocation(shader->Program, "projection");
    // Pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

}


void WiderCube::setVertices(GLfloat vertices[]) {
    for (int i = 0; i < 72; i++) {
        vertices[i] = this->vertices[i];
    }
}

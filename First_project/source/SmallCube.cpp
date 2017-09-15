#include "../include/Include.h"
#include "../include/SmallCube.h"

SmallCube::SmallCube() {
//    rotate = 55.0f;

    //vertices = new GLfloat[26];
    GLfloat vert []=  {
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f

      };

    for (int i = 0; i < Property::VERTICES_SIZE; i++)
        this->vertices[i] = vert[i];

//    this->shader = new Shader("/home/eliana/MEGA/Qt/Init_OpenGL_Core/shaders/cube.vs", "/home/eliana/MEGA/Qt/Init_OpenGL_Core/shaders/cube.frag");
    this->shader = new Shader("/home/eliana/MEGA/Qt/Init_OpenGL_Core/shaders/smallCube.vs", "/home/eliana/MEGA/Qt/Init_OpenGL_Core/shaders/smallCube.frag");

}

void SmallCube::setVAO_VBO() {

    Object::setVAO_VBO();

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void SmallCube::show(GLuint WIDTH, GLuint HEIGHT, GLfloat dimension, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos, glm::vec3 cameraPos){
    glUniform3f(glGetUniformLocation(shader->Program, "objectColor"), 0.60f, 0.60f, 0.0f);

//---    // Create transformations
    glm::mat4 model;

//    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(dimension, dimension, dimension));
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

//void SmallCube::updateRotate(float updateRotate, bool add) {
//    if (add)
//        rotate += updateRotate;
//    else
//        rotate -= updateRotate;
//}

void SmallCube::setVertices(GLfloat vertices[]) {
    for (int i = 0; i < Property::VERTICES_SIZE; i++) {
        vertices[i] = this->vertices[i];
    }
}

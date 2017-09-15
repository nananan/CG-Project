#ifndef FLOOR_H
#define FLOOR_H

#include "../include/Include.h"
#include "Object.h"

class Floor: public Object
{
    private:
//        GLfloat* floorCoordinates[];
        GLuint VBO, VAO;

        int width, height;
        GLuint texture;

    public:
        Floor() {}


//        GLfloat getFloorCoordinates() {
//            return floorCoordinates;
//        }

        GLuint getVAO() {
            return VAO;
        }

        void setVAO_VBO(GLfloat floorCoordinates[], float size) {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, size, floorCoordinates, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
        }

        void deleteVAO_VBO() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }


        void setModel(Shader* shader, glm::vec3 translate, float scale) {
            Object::setModel(shader, translate, scale);

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        void setUniformMaterial(Shader* shader) {
            glUniform1i(glGetUniformLocation(shader->Program, "material.diffuse"),  0);
            glUniform1i(glGetUniformLocation(shader->Program, "material.specular"), 1);
            glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), 32.0f);
        }
};


#endif



#ifndef OBJECT_H
#define OBJECT_H

#include "../include/Include.h"

class Object
{
    private:
        glm::mat4 model;

    public:
        Object() {}

        void setModel(Shader* shader, glm::vec3 translate, float scale) {
            model = glm::mat4();
            model = glm::translate(model, translate);
            model = glm::scale(model, glm::vec3(scale));
            model = glm::rotate(model,  glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        }

        void setUniform(Shader* shader, Camera camera, glm::mat4 view, glm::mat4 projection) {
            glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        }

        void setUniformLight(Shader* shader, float light_ambient, glm::vec3 lightpos, glm::vec3 lightdir) {
            glUniform3f(glGetUniformLocation(shader->Program, "light.position"),lightpos.x, lightpos.y, lightpos.z);
            glUniform3f(glGetUniformLocation(shader->Program, "light.direction"), lightdir.x, lightdir.y, lightdir.z);
            glUniform1f(glGetUniformLocation(shader->Program, "light.cutOff"), glm::cos(glm::radians(40.5f)));
            glUniform1f(glGetUniformLocation(shader->Program, "light.outerCutOff"), glm::cos(glm::radians(45.5f)));
            glUniform3f(glGetUniformLocation(shader->Program, "light.ambient"),   light_ambient, light_ambient, light_ambient);
            glUniform3f(glGetUniformLocation(shader->Program, "light.diffuse"),   0.8f, 0.8f, 0.8f);
            glUniform3f(glGetUniformLocation(shader->Program, "light.specular"),  1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shader->Program, "light.constant"),  1.0f);
            glUniform1f(glGetUniformLocation(shader->Program, "light.linear"),    0.09);
            glUniform1f(glGetUniformLocation(shader->Program, "light.quadratic"), 0.032);
        }

        void setUniformMaterial(Shader* shader, glm::vec3 color, bool hasColor, bool option) {
            if (hasColor) {
                glUniform3f(glGetUniformLocation(shader->Program, "material.ambient"),  color.x, color.y, color.z);
                glUniform3f(glGetUniformLocation(shader->Program, "material.diffuse"),  color.x, color.y, color.z);
            }

            if (option) {
                glUniform3f(glGetUniformLocation(shader->Program, "material.specular"),  1.0f, 1.0f, 1.0f); // Specular doesn't have full effect on this object's material
                glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), 64.0f);
            }
        }

        static bool detectCollision(glm::vec3 object, float radius, Camera camera){
            float distance = sqrt(pow(camera.Position.x - object.x, 2) + pow(camera.Position.y - object.y, 2) + pow(camera.Position.z - object.z, 2));
            if(distance <= radius)
                return true;
            return false;
        }
};


#endif

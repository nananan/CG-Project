#include "../include/Include.h"
#include "../include/Model.h"
#include <SOIL/SOIL.h>

#include <iostream>
// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 720;

// camera
Camera camera(glm::vec3(2.07124f, 0.0281185f, 2.51323f));
glm::vec3 lightPos(0.0f, 0.0f, 1.5f);
glm::vec3 target(0.0f, 0.0f, 5.0f);
glm::vec3 lightDir;


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool pressStart = false;

float rotX = 0.0f;
float rotY = 0.0f;

#define MOV_LIGHT 0.01f;
#define NUM_PARTICLES 2000;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

WiderCube* widerCube;
SmallCube* cube;

vector <Particle*> particles;

int main() {

    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "First Project", NULL, NULL);
    if (window == NULL) {

        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);


    Shader particle_shader("/home/eliana/MEGA/Qt/First_project/shaders/particle.vs", "/home/eliana/MEGA/Qt/First_project/shaders/particle.frag");

    char pathSphere[] = "/home/eliana/MEGA/Qt/First_project/model/sphere/sphere.obj";
    char pathTourch[] = "/home/eliana/MEGA/Qt/First_project/model/bulb/lamp.obj";
    Model modelSphere(pathSphere);
    Model modelTourch(pathTourch);

    widerCube = new WiderCube();
    cube = new SmallCube();

//    vector <Particle*> particles;
    for (int i = 0; i < NUM_PARTICLES i++) {
      Particle* particle = new Particle();
      particles.push_back(particle);
    }

    widerCube->setVAO_VBO();
    glBindVertexArray(0); // Unbind VAO
    cube->setVAO_VBO();
    glBindVertexArray(0); // Unbind VAO

    Shader lampShader("/home/eliana/MEGA/Qt/First_project/shaders/lamp.vs", "/home/eliana/MEGA/Qt/First_project/shaders/lamp.frag");

    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    particle_shader.Use();
    glUniform1f(glGetUniformLocation(particle_shader.Program, "material.diffuse"), 0);
    glUniform1f(glGetUniformLocation(particle_shader.Program, "material.specular"), 1);

    while(!glfwWindowShouldClose(window)) {

        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // Rendering commands here
        glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
//        glm::mat4 projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

//        glm::mat4 view;
//        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        widerCube->getShader()->Use();
        widerCube->show(WIDTH, HEIGHT, Property::dimension, projection, view);
        // render the lines (widerCube)
        glBindVertexArray(widerCube->getVAO());
        glDrawArrays(GL_LINES, 0, 24);
        glBindVertexArray(0);

        cube->getShader()->Use();
        cube->show(WIDTH, HEIGHT, (Property::dimension/3.0f), projection, view, lightPos, camera.Position);

        glBindVertexArray(cube->getVAO());
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDisable(GL_BLEND);
        glDepthMask(1);
        glBindVertexArray(0);

        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model;
        model = glm::translate(model, lightPos);
        model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f)); // a smaller cube
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        modelTourch.Draw(lampShader);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // DRAW PARTICLES
        particle_shader.Use();
        lightDir = target - lightPos;

        glUniform1f(glGetUniformLocation(particle_shader.Program, "light.cutOff"), glm::cos(glm::radians(25.5f)));
        glUniform1f(glGetUniformLocation(particle_shader.Program, "light.outerCutOff"), glm::cos(glm::radians(30.5f)));

        // light properties
        glUniform3f(glGetUniformLocation(particle_shader.Program, "light.ambient"), 0.5f, 0.5f, 0.5f);
        // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
        // each environment and lighting type requires some tweaking to get the best out of your environment.
        glUniform3f(glGetUniformLocation(particle_shader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(particle_shader.Program, "light.specular"), 0.1f, 0.1f, 0.1f);

        glUniform1f(glGetUniformLocation(particle_shader.Program, "light.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(particle_shader.Program, "light.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(particle_shader.Program, "light.quadratic"), 0.032f);

        // material properties
        glUniform1f(glGetUniformLocation(particle_shader.Program, "material.shininess"), 64.0f);

        glUniform3f(glGetUniformLocation(particle_shader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(particle_shader.Program, "light.direction"), -lightPos.x, -lightPos.y, -lightPos.z);
        glUniform3f(glGetUniformLocation(particle_shader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

        glUniformMatrix4fv(glGetUniformLocation(particle_shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(particle_shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        for(int i=0;i<NUM_PARTICLES i++){
           glm::mat4 spheres;
           spheres = glm::translate(spheres, particles[i]->getPosition());
           spheres = glm::scale(spheres, glm::vec3(0.01f, 0.01f, 0.01f));
           glUniformMatrix4fv(glGetUniformLocation(particle_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(spheres));
           modelSphere.Draw(particle_shader);
           if (pressStart)
               particles[i]->update(i);
        }

        glfwSwapBuffers(window);
    }

    widerCube->deleteVAO_VBO();
    cube->deleteVAO_VBO();

    for (int i = 0; i < NUM_PARTICLES i++)
        particles[i]->deleteVAO_VBO();

    delete widerCube;
    delete cube;

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

//    float cameraSpeed = 2.5 * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        cameraPos += cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // Camera controls
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        if (rotX >= -43.0) {
            lightPos.y += MOV_LIGHT;
            target.y += MOV_LIGHT;

            rotX -= 0.4f;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        if (rotX <= 43.0) {
            lightPos.y -= MOV_LIGHT;
            target.y -= MOV_LIGHT;

            rotX += 0.4f;
        }

    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        if (rotY <= 50.0) {
            lightPos.x += MOV_LIGHT;
            target.x += MOV_LIGHT;

            rotY += 0.4f;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        if (rotY >= -43.0) {
            lightPos.x -= MOV_LIGHT;
            target.x -= MOV_LIGHT;

            rotY -= 0.4f;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        lightPos.z += MOV_LIGHT;
        target.z += MOV_LIGHT;
    }
    if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
        lightPos.z -= MOV_LIGHT;
        target.z -= MOV_LIGHT;
    }


    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        if (pressStart)
            pressStart = false;
        else
            pressStart = true;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}



//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

//    if (firstMouse) {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }

//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//    lastX = xpos;
//    lastY = ypos;

//    float sensitivity = 0.1f; // change this value to your liking
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;

//    yaw += xoffset;
//    pitch += yoffset;

//    // make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (pitch > 89.0f)
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;

//    glm::vec3 front;
//    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front.y = sin(glm::radians(pitch));
//    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFront = glm::normalize(front);
//}

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// // ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

//    if (fov >= 1.0f && fov <= 45.0f)
//        fov -= yoffset;
//    if (fov <= 1.0f)
//        fov = 1.0f;
//    if (fov >= 45.0f)
//        fov = 45.0f;
//}

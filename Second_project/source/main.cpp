#include "../include/Include.h"

#include "Floor.h"
#include "../include/Texture.h"

GLuint WIDTH = 1280, HEIGHT = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.2f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool enableMove =false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
const float ray = 0.5f;

glm::vec3 lightpos(0.0f, 1.5f, 0.0f);
glm::vec3 lightdir(0.0f, -1.0f, 0.0f);

glm::vec3 position[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, -0.3f, 0.0f),
    glm::vec3(-1.0f, -0.3f, -1.5f)
};


int main() {

    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //MacOS X only
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
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

    GLfloat floorCoordinates[]={
        //Coordiantes           //normals              //texture
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  30.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  30.0f,  30.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  30.0f,  30.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  30.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
    };

    Shader* floor_shader = new Shader("/home/eliana/MEGA/Qt/Second_project/shaders/floor.vs", "/home/eliana/MEGA/Qt/Second_project/shaders/floor.frag");
    Shader* sphere_shader = new Shader("/home/eliana/MEGA/Qt/Second_project/shaders/sphere.vs", "/home/eliana/MEGA/Qt/Second_project/shaders/sphere.frag");
    Shader* cube_shader = new Shader("/home/eliana/MEGA/Qt/Second_project/shaders/cube.vs", "/home/eliana/MEGA/Qt/Second_project/shaders/cube.frag");
    char pathSphere[] = "/home/eliana/MEGA/Qt/Second_project/model/sphere2.obj";
    char pathCube[] = "/home/eliana/MEGA/Qt/Second_project/model/cube.obj";
    Model sphereModel(pathSphere);
    Model cubeModel(pathCube);

    Floor* floor = new Floor();
    floor->setVAO_VBO(floorCoordinates, sizeof(floorCoordinates));
    MyTexture texture("/home/eliana/MEGA/Qt/Second_project/textures/floor2.jpg");
    texture.setParameters(GL_REPEAT, GL_REPEAT,GL_NEAREST, GL_NEAREST);
//    texture.loadTexture("/home/eliana/MEGA/Qt/Second_project/textures/floor2.jpg");
//    floor->setTexParameter(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    Object* cube = new Object();
    Object* sphere = new Object();

    glBindVertexArray(0); // Unbind VAO

    glm::vec3 color[] = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        Do_Movement();

        // Rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
//        view = glm::rotate(view,  glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

        floor_shader->Use();
//        glUniform1f(glGetUniformLocation(floor_shader->Program, "tex"), true);

        floor->setUniform(floor_shader, camera, view, projection);
        floor->setUniformLight(floor_shader, 0.7f, lightpos, lightdir);
        floor->setUniformMaterial(floor_shader);

        //floor construsction
//        floor->bindTexture();
        texture.bindTexture(floor_shader);
        glBindVertexArray(floor->getVAO());
        floor->setModel(floor_shader, glm::vec3(0.0f, -5.3f, 0.0f), 10.0f);
//        floor->unbindTexture();
        texture.unbindTexture();
        glBindVertexArray(0);

        cube_shader->Use();
        cube->setUniform(cube_shader, camera, view, projection);
        cube->setUniformLight(cube_shader, 0.3f, lightpos, lightdir);
        cube->setUniformMaterial(cube_shader, color[1], true, true);
        cube->setModel(cube_shader, glm::vec3(0.0f, -0.099f, 0.0f), 0.2f);
        cubeModel.Draw(cube_shader);

        sphere_shader->Use();
        sphere->setUniform(sphere_shader, camera, view, projection);
        sphere->setUniformLight(sphere_shader, 0.3f, lightpos, lightdir);
        sphere->setUniformMaterial(sphere_shader, glm::vec3(), false, true);

//        glUniform1f(glGetUniformLocation(shader.Program, "tex"), false);
        for(int i = 1; i <= 4; i++){
            sphere->setUniformMaterial(sphere_shader, color[i-1], true, false);
            sphere->setModel(sphere_shader, position[i], 0.3f);
            sphereModel.Draw(sphere_shader);
        }


        glfwSwapBuffers(window);
    }

    floor->deleteVAO_VBO();

    delete floor_shader;
    delete sphere_shader;
    delete cube_shader;

    delete floor;
    delete cube;
    delete sphere;

    glfwTerminate();
    return 0;
}



// Moves/alters the camera positions based on user input
void Do_Movement()
{
    if(keys[GLFW_KEY_W]){
        camera.ProcessKeyboard(FORWARD, deltaTime);
        bool collision = false;
        for(int i = 0; i < 4; i++)
            if(Object::detectCollision(position[i], ray, camera)){
                collision = true;
                break;
            }
        if(collision)
           camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S]){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        bool collision = false;
        for(int i = 0; i < 4; i++)
            if(Object::detectCollision(position[i], ray, camera)){
                collision = true;
                break;
            }
        if(collision)
         camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_A]){
        camera.ProcessKeyboard(LEFT, deltaTime);
        bool collision = false;
        for(int i = 0; i < 4; i++)
            if(Object::detectCollision(position[i], ray, camera)){
                collision = true;
                break;
            }
        if(collision)
          camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if(keys[GLFW_KEY_D]){
        camera.ProcessKeyboard(RIGHT, deltaTime);
        bool collision = false;
        for(int i = 0; i < 4; i++)
            if(Object::detectCollision(position[i], ray, camera)){
                collision = true;
                break;
            }
        if(collision)
         camera.ProcessKeyboard(LEFT, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

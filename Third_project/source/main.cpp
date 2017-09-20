#include "../include/Include.h"
#include "Reader.h"
#include "Altitude.h"
#include "../include/Texture.h"

GLuint WIDTH = 1280, HEIGHT = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void getVertices(vector<tuple<float,float,float>> vert, GLfloat* vertices, vector<tuple<float,float>> vert_tex);
void drawModel(Shader* volcano_shader, glm::mat4 view, glm::mat4 projection, float nRows, float nCols);

glm::vec3 calculateNormal(GLfloat** matrix, int i, int j, int rows, int cols);
GLuint* indexVector(int& dimension, int rows, int cols);

glm::vec3 get_face_normal (float p1[3], float p2[3], float p3[3]);
void setPoint(float p[3], float x, float y, float z);

//camera
//for view near lava
// Camera camera(glm::vec3(-4576.68f, 2267.16f, 3357.31f));
//first: front of lava1
Camera camera(glm::vec3(-3055.73f, 4643.06f, 415.618f));
// in the surface, front of lava
// Camera camera(glm::vec3(-3046.46f, 3650.31f, 2681.76f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool enableMove =false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int sizeEBO = 0;
GLuint* indicesEBO = 0;
int sizeVBO = 0;

// POS: -3046.46 3650.31 2681.76
// 336.196 -6.76645
// 178.2 102.8
// 0 1480 0


//in comment, parameters for view front of lava
float angleX = 102.8f;
float angleY = 178.2f;
float angleZ = 0.0f;

float transX = 0.0f;
float transY = 1480.0f;
//---- for view near lava
// float angleX = 102.8f;
// float angleY = 202.6f;
// float angleZ = 0.0f;
// 
// float transX = 280.0f;
// float transY = 1440.0f;

//---- in the surface, front of lava
// float angleX = 102.8f;
// float angleY = 178.2f;
// float angleZ = 0.0f;
// 
// float transX = 0.0f;
// float transY = 1480.0f;


float transZ = 0.0f;
float changeTranslation = 1.0f;

#define SPEED_ROTATE 0.40f;

int main()
{
    srand(time(NULL));

    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Third Project", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);


    Reader* reader_altitude = new Reader("../dataset_lava/altitudes.dat");
    reader_altitude->loadData(false);
    Reader* reader_temperature = new Reader("../dataset_lava/temperature.dat");
    reader_temperature->loadData(true);
    Reader* reader_lava = new Reader("../dataset_lava/lava.dat");
    reader_lava->loadData(true);

    float max = reader_temperature->getMatrix()[0][0];
    float min = numeric_limits<float>::max();

    float distance = 10.0;
    int nrows = reader_altitude->getValues()["nrows"];
    int ncols = reader_altitude->getValues()["ncols"];
    Altitude* altitudes = new Altitude(nrows, ncols);

//     for (int i = 0; i < nrows+1; i++) {
//         for (int j = 0; j < ncols+1; j++) {
//             altitudes->setValueMatrix(i,j, reader_lava->getValueMatrix(i,j));
//         }
//     }
// 
//     delete reader_lava;

    for (int i = 0; i < reader_temperature->getValues()["nrows"]; i++) {
        for (int j = 0; j < reader_temperature->getValues()["ncols"]; j++) {
            float value = reader_temperature->getValueMatrix(i,j);
            float no_value = reader_temperature->getValues()["NODATA_value"];
            if (value > max && value != no_value)
                max = value;
            if (value < min && value != no_value)
                min = value;
        }
    }

    MyTexture texture("../textures/texture.png");
    texture.setParameters(GL_REPEAT, GL_REPEAT,GL_NEAREST, GL_NEAREST);

    Shader* volcano_shader = new Shader("../shaders/volcano.vs", "../shaders/volcano.frag");
    Shader* light_shader = new Shader("../shaders/light.vs", "../shaders/light.frag");

    altitudes->fillVertexMatrix(reader_altitude->getMatrix(), nrows, ncols, reader_altitude->getValues()["NODATA_value"]);
    delete reader_altitude;
    
    
    for (int i = 0; i < nrows+1; i++) {
        for (int j = 0; j < ncols+1; j++) {
            altitudes->setValueMatrix(i,j, reader_lava->getValueMatrix(i,j));
        }
    }

    delete reader_lava;

    
    altitudes->calculateNormal();    

    vector<tuple<float,float,float>> vert;
    vector<tuple<float,float>> vert_tex;

    int index_norm = 0;
    int ind = 1;
    for (int i = 0; i < nrows+1; i++) {
        for (int j = 0; j < ncols+1; j++) {
            float valueZ = altitudes->getValueMatrix(i,j);
            vert.push_back(make_tuple(i*distance, j*distance, valueZ)); //if change i and j, remove comment in camera's property

            if (ind == 3)
                index_norm++;

            vert.push_back(altitudes->getNormal(index_norm));

            float temperature = reader_temperature->getValueMatrix(i,j);
            float x = 0.0;
            if(temperature > 0.0f)
               x = (temperature-min) / (max - min);

            vert.push_back(make_tuple(temperature, x, 0.0f));

            float r = 1-(float)i/nrows; //add "1-" se vuoi come prof
            float s = (float)j/ncols;

            vert_tex.push_back(make_tuple(s,r));

            ind++;
        }
    }

    delete altitudes;
    delete reader_temperature;

    sizeVBO = vert.size()*3 + vert_tex.size() *2;
    GLfloat* vertices = new GLfloat[sizeVBO];
    getVertices(vert, vertices, vert_tex);

    GLuint* indicesEBO = indexVector(sizeEBO, nrows+1, ncols+1);

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeVBO*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeEBO*sizeof(GLuint), indicesEBO, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    //texture attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);


    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 10000.0f);

//         cout<<"POS: "<<camera.Position.x<<" "<<camera.Position.y<<" "<<camera.Position.z<<endl;
//         cout<<camera.Yaw<<" "<<camera.Pitch<<endl;
//        
//         cout<<angleY<<" "<<angleX<<endl;
//         cout<<transX<<" "<<transY<<" "<<transZ<<endl;
//         
        volcano_shader->Use();

        glUniform1i(glGetUniformLocation(volcano_shader->Program, "material.diffuse"),  0);
        glUniform1i(glGetUniformLocation(volcano_shader->Program, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(volcano_shader->Program, "material.shininess"), 32.0f);

        glUniform3f(glGetUniformLocation(volcano_shader->Program, "light.position"), camera.Position.x, camera.Position.y, camera.Position.z);
//        glUniform3f(glGetUniformLocation(volcano_shader->Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(volcano_shader->Program, "light.ambient"),   0.7f, 0.7f, 0.7f);
        glUniform3f(glGetUniformLocation(volcano_shader->Program, "light.diffuse"),   0.9f, 0.9f, 0.9f);
//         glUniform3f(glGetUniformLocation(volcano_shader->Program, "light.specular"),  1.0f, 1.0f, 1.0f);


        glUniform1f(glGetUniformLocation(volcano_shader->Program, "light.constant"),  1.0f);
        glUniform1f(glGetUniformLocation(volcano_shader->Program, "light.linear"),    0.000014);
        glUniform1f(glGetUniformLocation(volcano_shader->Program, "light.quadratic"), 0.00000007);

        drawModel(volcano_shader, view, projection, (float)(nrows+1)/2, (float)(ncols+1)/2);

//         texture.bindTexture(volcano_shader);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, sizeEBO, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//         texture.unbindTexture();

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    delete volcano_shader;
    delete light_shader;

    delete [] vertices;
    delete [] indicesEBO;

    return 0;
}




GLuint* indexVector(int& dimension, int rows, int cols) {
   dimension = (rows-1) * (cols-1) * 2 * 3;
   GLuint* indexVector = new GLuint[dimension];
   int index = 0;
   for (int i = 0; i < rows-1; i++) {
          for (int j = 0; j < cols-1; j++) {
              indexVector[index++] = j + i * cols;
              indexVector[index++] = (j+1) + i * cols;
              indexVector[index++] = (j+1) + (i+1) * cols;

              indexVector[index++] = (j+1) + (i+1) * cols;
              indexVector[index++] = j + (i+1) * cols;
              indexVector[index++] = j + i * cols;
          }
   }

    dimension = index;
    return indexVector;

}

void drawModel(Shader* volcano_shader, glm::mat4 view, glm::mat4 projection, float nRows, float nCols) {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(transX, transY, transZ));
    model = glm::rotate(model,  glm::radians(angleX), glm::vec3(nRows, 0.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(angleY), glm::vec3(0.0f, nCols, 0.0f));

    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(volcano_shader->Program, "model");
    GLint viewLoc = glGetUniformLocation(volcano_shader->Program, "view");
    GLint projLoc = glGetUniformLocation(volcano_shader->Program, "projection");
    // Pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void getVertices(vector<tuple<float,float,float>> vert, GLfloat* vertices, vector<tuple<float,float>> vert_tex) {
    int index = 0;
    int index_tex = 0;
    int ind = 1;

    for(auto e: vert) {
        vertices[index] = get<0>(e);
        vertices[index+1] = get<1>(e);
        vertices[index+2] = get<2>(e);

        if (ind == 3) {
            vertices[index+3] = get<0>(vert_tex[index_tex]);
            vertices[index+4] = get<1>(vert_tex[index_tex]);
            index_tex++;
            ind = 1;
            index += 5;
        }
        else {
            index += 3;
            ind++;
        }
    }
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

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

    //rotation movement
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        angleY -= SPEED_ROTATE;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        angleY += SPEED_ROTATE;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        angleX += SPEED_ROTATE;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        angleX -= SPEED_ROTATE;
    }
    //traslation movement
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        transX += 40.00f * changeTranslation;
    }
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
        transY += 40.0f * changeTranslation;
    }
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        transZ += 40.0f * changeTranslation;
    }
    if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
        changeTranslation *= -1.0f;
    }
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


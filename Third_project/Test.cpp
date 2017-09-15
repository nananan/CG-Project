#include "../include/Include.h"
#include "Reader.h"

GLuint WIDTH = 1280, HEIGHT = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void movement();

vector<tuple<int,int,int>> createVertices(int i, int j, int value);
void getVertices(vector<tuple<int,int,float>> vert, float* vertices);
void draw(unsigned int VAO, vector<tuple<int,int,float>> vert);
void drawModel(Shader volcano_shader, glm::mat4 view, glm::mat4 projection);

//camera
Camera camera(glm::vec3(50.0f, 50.0f, 700.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool enableMove =false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

    Reader* reader = new Reader("/home/eliana/MEGA/Qt/Third_project/data/DEM_Albano.asc");
    cout<<"BEH"<<endl;
    reader->loadData();

    Shader volcano_shader("/home/eliana/MEGA/Qt/Third_project/shaders/volcano.vs", "/home/eliana/MEGA/Qt/Third_project/shaders/volcano.frag");
    float val = 5.0;
    float value = 0.0;
    float distance = 10.0;

    vector<tuple<int,int,float>> vert;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
//                cout<<reader->getValueAltitude(i,j)<<endl;
            if (reader->getValueAltitude(i,j) != reader->getValues()["NODATA_value"]) {

                //first triangle (up, for me, but down for OpenGL)
                vert.push_back(make_tuple(i*distance,j*distance,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(i*distance+val,j*distance-val,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(i*distance-val,j*distance-val,reader->getValueAltitude(i,j)));

                vert.push_back(make_tuple(i*distance,j*distance,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(i*distance-val,j*distance+val,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(i*distance+val,j*distance+val,reader->getValueAltitude(i,j)));

                vert.push_back(make_tuple(j*distance,i*distance,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(j*distance+val,i*distance-val,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(j*distance+val,i*distance+val,reader->getValueAltitude(i,j)));

                vert.push_back(make_tuple(j*distance,i*distance,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(j*distance-val,i*distance+val,reader->getValueAltitude(i,j)));
                vert.push_back(make_tuple(j*distance-val,i*distance-val,reader->getValueAltitude(i,j)));
            }
        }
    }

    for (auto e: vert)
        cout<<get<0>(e)<<" "<<get<1>(e)<<" "<<get<2>(e)<<endl;

    cout<<vert.size()<<endl;


    float* vertices = new float[vert.size()*3];
    getVertices(vert, vertices);

//    cout<<"WE"<<endl;
//    unsigned int VBO;
//    unsigned int VAOs[4];
//    glGenBuffers(1, &VBO);
//    glGenVertexArrays(1, &VAOs[0]);
//    glBindVertexArray(VAOs[0]);

//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);

//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * dimVectorCoordinates), vectorCoordinates, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(GLuint) * vert.size()*3), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        movement();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f);


        volcano_shader.Use();
        drawModel(volcano_shader, view, projection);
        draw(VAOs[0], vert);
        glDeleteVertexArrays(1, &VAOs[0]);


//        float vertices[3];

//        for (int i = 0; i < 20; i++) {
//            for (int j = 0; j < 20; j++) {
// //                cout<<reader->getValueAltitude(i,j)<<endl;
//                if (reader->getValueAltitude(i,j) != reader->getValues()["NODATA_value"]) {
//                    vector<tuple<int,int,float>> vert;

//                    //first triangle (up, for me, but down for OpenGL)
//                    vert.push_back(make_tuple(i*distance,j*distance,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(i*distance+val,j*distance-val,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(i*distance-val,j*distance-val,reader->getValueAltitude(i,j)));


//                    for (auto e: vert)
//                        cout<<get<0>(e)<<" "<<get<1>(e)<<" "<<get<2>(e)<<endl;

//                    float vertices[9];
//                    getVertices(vert, vertices);


//                    glGenVertexArrays(1, &VAOs[0]);
//                    glBindVertexArray(VAOs[0]);

//                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                    glEnableVertexAttribArray(0);

//                    glBindBuffer(GL_ARRAY_BUFFER, 0);
//                    glBindVertexArray(0);

//                    cout<<"-----------------"<<endl;

//                    volcano_shader.Use();
//                    drawModel(volcano_shader, view, projection);
//                    draw(VAOs[0]);
//                    glDeleteVertexArrays(1, &VAOs[0]);


//                    vert.clear();
//                    //second triangle (down, for me, but up for OpenGL)
//                    vert.push_back(make_tuple(i*distance,j*distance,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(i*distance-val,j*distance+val,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(i*distance+val,j*distance+val,reader->getValueAltitude(i,j)));


//                    for (auto e: vert)
//                        cout<<get<0>(e)<<" "<<get<1>(e)<<" "<<get<2>(e)<<endl;

//                    getVertices(vert, vertices);

//                    glGenVertexArrays(1, &VAOs[1]);
//                    glBindVertexArray(VAOs[1]);

//                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                    glEnableVertexAttribArray(0);

//                    glBindBuffer(GL_ARRAY_BUFFER, 0);
//                    glBindVertexArray(0);

//                    cout<<"-----------------"<<endl;

//                    volcano_shader.Use();

//                    drawModel(volcano_shader, view, projection);

//                    draw(VAOs[1]);

//                    glDeleteVertexArrays(1, &VAOs[1]);


//                    //third triangle
//                    vert.clear();
//                    vert.push_back(make_tuple(j*distance,i*distance,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(j*distance+val,i*distance-val,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(j*distance+val,i*distance+val,reader->getValueAltitude(i,j)));

//                    for (auto e: vert)
//                        cout<<get<0>(e)<<" "<<get<1>(e)<<" "<<get<2>(e)<<endl;

//                    getVertices(vert, vertices);

//                    glGenVertexArrays(1, &VAOs[2]);
//                    glBindVertexArray(VAOs[2]);

//                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                    glEnableVertexAttribArray(0);

//                    glBindBuffer(GL_ARRAY_BUFFER, 0);
//                    glBindVertexArray(0);
//                    cout<<"-----------------"<<endl;

//                    volcano_shader.Use();
//                    drawModel(volcano_shader, view, projection);
//                    draw(VAOs[2]);
//                    glDeleteVertexArrays(1, &VAOs[2]);


//                    //forth triangle
//                    vert.clear();
//                    vert.push_back(make_tuple(j*distance,i*distance,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(j*distance-val,i*distance+val,reader->getValueAltitude(i,j)));
//                    vert.push_back(make_tuple(j*distance-val,i*distance-val,reader->getValueAltitude(i,j)));


//                    for (auto e: vert)
//                        cout<<get<0>(e)<<" "<<get<1>(e)<<" "<<get<2>(e)<<endl;

//                    getVertices(vert, vertices);

//                    glGenVertexArrays(1, &VAOs[3]);
//                    glBindVertexArray(VAOs[3]);

//                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                    glEnableVertexAttribArray(0);

//                    glBindBuffer(GL_ARRAY_BUFFER, 0);
//                    glBindVertexArray(0);
//                    cout<<"-----------------"<<endl;

//                    volcano_shader.Use();
//                    drawModel(volcano_shader, view, projection);
//                    draw(VAOs[3]);
//                    glDeleteVertexArrays(1, &VAOs[3]);
//                }
//            }
//        }

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    return 0;
}


void calcuteEBO ()
{
    sizeEBO = (coordsAltitude.nRows-1) * (coordsAltitude.nCols-1) * 2 * 3;
    indicesEBO = new unsigned int [sizeEBO];
    unsigned int globalIndex = 0;
    for (int i = 0; i < coordsAltitude.nRows-1; ++i) {
        for (int j = 0; j < coordsAltitude.nCols-1; ++j) {

            indicesEBO[globalIndex++] = linearIndex(i,j);
            indicesEBO[globalIndex++] = linearIndex(i,j+1);
            indicesEBO[globalIndex++] = linearIndex(i+1,j);
        }
    }

    for (int i = 1; i < coordsAltitude.nRows; ++i) {
        for (int j = 0; j < coordsAltitude.nCols-1; ++j) {

            indicesEBO[globalIndex++] = linearIndex(i,j);
            indicesEBO[globalIndex++] = linearIndex(i-1,j+1);
            indicesEBO[globalIndex++] = linearIndex(i,j+1);
        }
    }

}

int linearIndex (unsigned int i, unsigned j)
{
    return i*coordsAltitude.nCols+j;
}

void drawModel(Shader volcano_shader, glm::mat4 view, glm::mat4 projection) {
    glm::mat4 model;
//        model = glm::rotate(model, glm::radians(55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
//        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(volcano_shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(volcano_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(volcano_shader.Program, "projection");
    // Pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


void draw(unsigned int VAO, vector<tuple<int,int,float>> vert) {
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//    glDrawArrays(GL_TRIANGLES, 0, vert.size()/3);
    glDrawElements(GL_TRIANGLES, vert.size()*3, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


vector<tuple<int,int,int>> createVertices(int i, int j, float value) {
    vector<tuple<int,int,int>> vert;
    int val= 5.0;

    vert.push_back(make_tuple(i*val,j*val,value));
    vert.push_back(make_tuple((i+1)*val,(j+1)*val,value));
    vert.push_back(make_tuple((i-1)*val,(j+1)*val,value));

    return vert;
}


void getVertices(vector<tuple<int,int,float>> vert, float* vertices) {
//    float vertices[9];
    int index = 0;
    for(auto e: vert) {
        vertices[index] = get<0>(e);
        vertices[index+1] = get<1>(e);
        vertices[index+2] = get<2>(e);
        index += 3;
//                    cout<<get<0>(e)<<" "<<get<1>(e)<<" "<<get<2>(e)<<endl;
    }
//    for (int i = 0; i < 9; i++)
//        cout<<vertices[i]<<endl;

//    return vertices;
}


void movement() {
    if(keys[GLFW_KEY_W]){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S]){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(keys[GLFW_KEY_A]){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(keys[GLFW_KEY_D]){
        camera.ProcessKeyboard(RIGHT, deltaTime);
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


//
//  main.cpp
//  Amusement Park
//
//  Created by Waliul Islam Sumon on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "sphere.h"

#include "stb_image.h"

#include <iostream>
#include <stdlib.h>
#include<windows.h>  

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void Table(Shader ourshader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void Chair(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void Floor(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void drawCube(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest = 0, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float spec = 1.0f, float shininess = 32.0f);
void drawCube1(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest = 0, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float spec = 1.0f, float shininess = 32.0f);
void Road(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void Boundary(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void Gate(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void GuardRoom(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, unsigned int texture);
void Shop(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, unsigned int texture);
void LampPost(Shader ourShader, glm::mat4 moveMatrix);
void load_texture(unsigned int& texture, string image_name, GLenum format);
void SetupPointLight(PointLight &pointLight, Shader ourShader, int lightNum);
void Stage(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void FoodPlace(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void TableChairSet(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void read_file(string file_name, vector<float> &vec);
long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices);
void drawCurve(Shader ourShader, glm::mat4 moveMatrix, unsigned int VAO, vector<int> indices, glm::vec4 color = glm::vec4(1.0f), float rotateAngleTest_Y = 0);
void FerrisWheel(Shader ourShader, glm::mat4 moveMatrix);
void FerrisWheelSeat(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z = 0);
void PirateShip(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z = 0);
void Rotor(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z = 0);
void SkyDrop(Shader ourShader, glm::mat4 moveMatrix);
void Carousal(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z = 0);
void AliBaba(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z = 0);
void ShowFunctions();


// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

float rotateAngleTest_Y = 0.0;


// camera               8.0   1.0   18.1
Camera camera(glm::vec3(8.0f, 1.0f, 18.1f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 2.5;
float lookAtX = 0.0, lookAtY = 1.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float lastTime = 0.0f, lastTimeSky = 0.0f;


// Light initialization
const int noOfPointLights = 4;

glm::vec3 lightPositions[] = {
        glm::vec3(-0.95f, 1.4f, -2.7f),         //Directional Light
        glm::vec3(10.2f, 1.35f, 14.48f),
        glm::vec3(6.2f, 1.35f, -4.52f),
        glm::vec3(10.2f, 1.35f, 1.48f),
        glm::vec3(6.2f, 1.35f, 7.98f),
        glm::vec3(5.0f, 1.35f, 8.18f)

};

glm::vec3 lightDirections[] = {
    glm::vec3(-2.0f, -0.2f, -1.3f)
};


DirectionalLight directionalLight(-lightPositions[0], glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(0.2f, 0.2f, 0.2f, 0.2f), 1);
SpotLight spotLight(lightPositions[5], lightDirections[0], 4.5f, 7.5f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0014f, 0.000007f, 1);
PointLight pointLight1(lightPositions[1], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);
PointLight pointLight2(lightPositions[2], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 2);
PointLight pointLight3(lightPositions[3], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 3);
PointLight pointLight4(lightPositions[4], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 4);



//***********************************Curve*******************
vector <float> cntrlPoints, cntrlPointsBoat, cntrlPointsRotor, cntrlPointsCylinder, cntrlPointsCarousal, cntrlPointsHead;
vector <float> coordinates, coordinatesBoat, coordinatesRotor, coordinatesCylinder, coordinatesCarousal, coordinatesHead;
vector <float> normals, normalsBoat, normalsRotor, normalsCylinder, normalsCarousal, normalsHead;
vector <int> indices, indicesBoat, indicesRotor, indicesCylinder, indicesCarousal, indicesHead;
vector <float> vertices, verticesBoat, verticesRotor, verticesCylinder, verticesCarousal, verticesHead;

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 30;

unsigned int cubeVAO, cubeVBO, cubeEBO;
unsigned int bezierVAO, boatVAO, rotorVAO, cylinderVAO, carousalVAO, headVAO;




// texture
float extra = 4.0f;
float TXmin = 0.0f;
float TXmax = 1.0f + extra;
float TYmin = 0.0f;
float TYmax = 1.0f + extra;



//Switches
float gateAngle = 0.0f;
float isGateOpening = -1.0f;
float gateSpeed = 1.0f;

float speed = 9.0f;

float ferrisWheelAngle = 0.0f;
float ferrisWheelSpeed = 0.02f;
float ferrisIncrement = 0.012f;
bool isFerrisWheelOn = true;

float boatAngle = 0.0f;
float boatSpeed = 1.2f;
bool isBoatOn = true;
float boatMaxAngle = 60.0f;

bool isRotorOn = true;
float rotorSpeed = 0.02f;
float rotorAngle = 0.0f;
float rotorIncrement = 0.012f;

bool isSkyDropOn = true;
float skyDropMax = 8.03f;
float skyDropMin = 0.0f;
float skyDropUpSpeed = 0.04f;
float skyDropDownSpeed = 0.09f;
float skyDropY = 0.0f;
float skyDropCurrent = 1.0f;

bool isCarousalOn = true;
float carousalSpeed = 0.02f;
float carousalAngle = 0.0f;
float carousalIncrement = 0.012f;

float aliAngle = 0.0f;
float aliSpeed = 0.02f;
float aliIncrement = 0.012f;
bool isAliOn = true;


//light switches
bool lightingOn = true;
float ambientOn = 1.0;
float diffuseOn = 1.0; 
float specularOn = 1.0;
bool dark = false;

float directionalLightOn = 1.0;
float pointLightOn[noOfPointLights] = {0.0, 0.0, 0.0, 0.0};
float spotLightOn = 0.0;


// Textures
unsigned int texture0, texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9;

// Skybox

float skyboxVertices[] =
{
    -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    // Right
    1, 2, 6, 
    6, 5, 1,
    //Left
    0, 4, 7,
    7, 3, 0,
    //Top
    4, 5, 6, 
    6, 7, 4,
    //Bottom
    0, 3, 2,
    2, 1, 0,
    //Back
    0, 1, 5,
    5, 4, 0,
    //Front
    3, 7, 6,
    6, 2, 3
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Amusement Park - Waliul (1807034)", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightCubeShader("lightVertexShader.vs", "lightFragmentShader.fs");
    Shader skyboxShader("skybox.vert", "skybox.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    float cube_vertices[] = {
        // positions      // normals         // texture coords
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

        0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
        0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
        0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
        0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
        0.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    //Waliul

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
    glEnableVertexAttribArray(2);

    //light's VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //For Skybox
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Day
    string facesCubemapDay[6] = {
        "right.png",
        "left.png",
        "top.png",
        "bottom.png",
        "front.png",
        "back.png"
    };

    unsigned int cubemapTextureDay;
    glGenTextures(1, &cubemapTextureDay);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureDay);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemapDay[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        }
        else
            cout << "Failed to load texture: " << facesCubemapDay[i] << endl;
        stbi_image_free(data);
    }

    //Night
    string facesCubemapNight[6] = {
        "night_right.jpeg",
        "night_left.jpeg",
        "night_top.jpeg",
        "night_bottom.jpeg",
        "night_front.jpeg",
        "night_back.jpeg"
    };

    unsigned int cubemapTextureNight;
    glGenTextures(1, &cubemapTextureNight);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureNight);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemapNight[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        }
        else
            cout << "Failed to load texture: " << facesCubemapNight[i] << endl;
        stbi_image_free(data);
    }




    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    // Texture loading

    //Texture 0
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("transparent.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture 0" << endl;
    }
    stbi_image_free(data);


    load_texture(texture1, "grass_2.png", GL_RGBA);
    load_texture(texture2, "brickwall_1.jpg", GL_RGB);
    load_texture(texture3, "brickwall_2.jpg", GL_RGB);
    load_texture(texture4, "road.jpg", GL_RGB);
    load_texture(texture5, "concrete1.png", GL_RGBA);
    load_texture(texture6, "concrete2.png", GL_RGBA);
    load_texture(texture7, "concrete3.png", GL_RGBA);
    load_texture(texture8, "leather.png", GL_RGBA);
    load_texture(texture9, "leather2.png", GL_RGBA);


    Sphere sphere = Sphere();


    //*******************************Curve***************************

    read_file("ferris_wheel_points.txt", cntrlPoints);
    bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1, coordinates, normals, indices, vertices);

    read_file("boat_points.txt", cntrlPointsBoat);
    boatVAO = hollowBezier(cntrlPointsBoat.data(), ((unsigned int)cntrlPointsBoat.size() / 3) - 1, coordinatesBoat, normalsBoat, indicesBoat, verticesBoat);

    read_file("rotor_points.txt", cntrlPointsRotor);
    rotorVAO = hollowBezier(cntrlPointsRotor.data(), ((unsigned int)cntrlPointsRotor.size() / 3) - 1, coordinatesRotor, normalsRotor, indicesRotor, verticesRotor);

    read_file("cylinder_points.txt", cntrlPointsCylinder);
    cylinderVAO = hollowBezier(cntrlPointsCylinder.data(), ((unsigned int)cntrlPointsCylinder.size() / 3) - 1, coordinatesCylinder, normalsCylinder, indicesCylinder, verticesCylinder);

    read_file("carousal_points.txt", cntrlPointsCarousal);
    carousalVAO = hollowBezier(cntrlPointsCarousal.data(), ((unsigned int)cntrlPointsCarousal.size() / 3) - 1, coordinatesCarousal, normalsCarousal, indicesCarousal, verticesCarousal);

    read_file("head_points.txt", cntrlPointsHead);
    headVAO = hollowBezier(cntrlPointsHead.data(), ((unsigned int)cntrlPointsHead.size() / 3) - 1, coordinatesHead, normalsHead, indicesHead, verticesHead);


    /*for (int i = 0; i < cntrlPointsBoat.size(); i++)
        cout << cntrlPointsRotor[i] << endl;*/
    
    ShowFunctions();
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();
        glBindVertexArray(cubeVAO);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        ////testing rotating around lookat point
        //const float radius = 2.0f;
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //glm::mat4 view;
        //view = glm::lookAt(glm::vec3(camX, 1.0, camZ), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        // camera/view transformation
        //glm::mat4 view = basic_camera.createViewMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);


        //**************lighting**************

        //Setting up Directional Light
        directionalLight.setUpLight(ourShader);
        if (!directionalLightOn)
            directionalLight.turnOff();
        if (!ambientOn)
            directionalLight.turnAmbientOff();
        if(!diffuseOn)
            directionalLight.turnDiffuseOff();
        if(!specularOn)
            directionalLight.turnSpecularOff();

        //Setting up Point Light
        SetupPointLight(pointLight1, ourShader, 1);
        SetupPointLight(pointLight2, ourShader, 2);
        SetupPointLight(pointLight3, ourShader, 3);
        SetupPointLight(pointLight4, ourShader, 4);

        //Setting up Spot Light
        spotLight.setUpLight(ourShader);
        if (!spotLightOn)
            spotLight.turnOff();
        if (!ambientOn)
            spotLight.turnAmbientOff();
        if (!diffuseOn)
            spotLight.turnDiffuseOff();
        if (!specularOn)
            spotLight.turnSpecularOff();

        //Setting up Camera and Others
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setBool("lightingOn", lightingOn);

        //cout << camera.Position[0] << " " << camera.Position[1] << " " << camera.Position[2] << endl;


        // Starting
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, scaleMatrix, rotateMatrix;
        glm::vec4 color1;

        float chairZ = 0.4f;
        float fixY = 0.45f;
        float chairX = 0.1f;



        
        //********** Object making ***********


        glActiveTexture(GL_TEXTURE0);
        //Ground
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -2.15f, 2.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture1);
        Floor(ourShader, translateMatrix, color1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.2f, -2.145f, 2.0f));
        color1 = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture4);
        Road(ourShader, translateMatrix, color1);


        //Boundary
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        Boundary(ourShader, translateMatrix, color1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        Gate(ourShader, translateMatrix, color1);


        //Guard Room
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
        color1 = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        GuardRoom(ourShader, translateMatrix, color1, texture3);


        //Shops
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 0.0f, -2.0f));
        
        Shop(ourShader, translateMatrix, color1, texture5);
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::mat4 reflectionMatrix(1.0f);
        glm::vec3 normal(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                reflectionMatrix[i][j] = (i == j) ? 1.0f - 2.0f * normal[i] * normal[j] : -2.0f * normal[i] * normal[j];
            }
        }
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-12.6f, 0.0f, -4.0f));
        color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        Shop(ourShader, reflectionMatrix * translateMatrix, color1, texture6);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 0.0f, -6.0f));
        color1 = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        Shop(ourShader, translateMatrix, color1, texture7);


        // Food place
        glBindTexture(GL_TEXTURE_2D, texture0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        FoodPlace(ourShader, translateMatrix, rotateAngleTest_Y);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.13f, 0.0f, -6.93f));
        LampPost(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 1.2f, 8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        drawCube1(ourShader, translateMatrix * scaleMatrix, 60.0f, glm::vec4(0.2f));


        //*****************************Rides***********************************************************************
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        FerrisWheel(ourShader, translateMatrix);
        PirateShip(ourShader, translateMatrix);
        Rotor(ourShader, translateMatrix, rotateAngleTest_Y);
        SkyDrop(ourShader, translateMatrix);
        Carousal(ourShader, translateMatrix, rotateAngleTest_Y);
        AliBaba(ourShader, translateMatrix, rotateAngleTest_Y);




        //Lamp Posts
        glBindTexture(GL_TEXTURE_2D, texture0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, -0.7f));
        LampPost(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, -19.7f));
        LampPost(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, -13.7f));
        LampPost(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, -7.2f));
        LampPost(ourShader, translateMatrix);


       


        //********* END of Object Making **********

        //Lights
        lightCubeShader.use();
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.13f, 0.1f, 0.13f));

        for (int i = 1; i <= 5; i++)
        {

            /*glm::vec3 lightColor;
            lightColor.x = sin(glfwGetTime() * 1.0f);
            lightColor.y = sin(glfwGetTime() * 0.35f);
            lightColor.z = sin(glfwGetTime() * 0.7f);
            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
            lightCubeShader.setVec4("bodyColor", glm::vec4(diffuseColor, 1.0f));*/

            glm::vec4 bodyColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            //emissive
            glm::vec3 val = glm::vec3(0.5f);
            if (i == 1 and pointLightOn[0] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 2 and pointLightOn[1] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 3 and pointLightOn[2] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 4 and pointLightOn[3] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 5 and spotLightOn == 0.0)
                bodyColor = glm::vec4(val, 1.0f);


            lightCubeShader.setVec4("bodyColor", bodyColor);
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            lightCubeShader.setMat4("projection", projection);
            glm::mat4 view = camera.GetViewMatrix();
            lightCubeShader.setMat4("view", view);
            glm::mat4 tempModel = glm::mat4(1.0f);
            tempModel = glm::translate(tempModel, lightPositions[i]);
            lightCubeShader.setMat4("model", tempModel * scaleMatrix);
            sphere.drawSphere(lightCubeShader);

        }


        //Texture drawing
        glDepthFunc(GL_LEQUAL);

        skyboxShader.use();
        glm::mat4 view1 = glm::mat4(1.0f);
        glm::mat4 projection1 = glm::mat4(1.0f);
        view1 = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up)));
        projection1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        skyboxShader.setMat4("view", view1);
        skyboxShader.setMat4("projection", projection1);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        if (dark)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureNight);
        }
        else
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureDay);
        }
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {                 //Forward
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {                 //Backward
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {                 //Left
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {                 //Right
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)                   //Down
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)                   //Pitch positive
    {
        camera.ProcessYPR(0.0f, 3.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Pitch negative
    {
        camera.ProcessYPR(0.0f, -3.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)                   //Yaw positive
    {
        camera.ProcessYPR(3.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)                   //Yaw negative
    {
        camera.ProcessYPR(-3.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)                   //Roll positive
    {
        camera.ProcessYPR(0.0f, 0.0f, 0.5f);

    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)                   //Roll negative
    {
        camera.ProcessYPR(0.0f, 0.0f, -0.5f);

    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)                   //Rotate camera around a look at point
    {                                                                   //Right
        camera.RotateAroundLookAt(2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)                   //Rotate camera around a look at point
    {                                                                   //Left
        camera.RotateAroundLookAt(-2.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)                   //Up
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);

        camera.ProcessKeyboard(UP, deltaTime);

    }


    //if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)               //Basic Camera rotate around lookat
    //{
    //    //lookAtX += 2.5 * deltaTime;
    //    //basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    //    float change = -0.3f;
    //    float x, y, z;
    //    x = eyeX * cos(glm::radians(change)) + eyeZ * sin(glm::radians(change));
    //    y = eyeY;
    //    z = - eyeX * sin(glm::radians(change)) + eyeZ * cos(glm::radians(change));

    //    eyeX = x, eyeY = y, eyeZ = z;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    //{
    //    //lookAtX -= 2.5 * deltaTime;
    //    //basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);

    //    float change = 0.3f;
    //    float x, y, z;
    //    x = eyeX * cos(glm::radians(change)) + eyeZ * sin(glm::radians(change));
    //    y = eyeY;
    //    z = -eyeX * sin(glm::radians(change)) + eyeZ * cos(glm::radians(change));

    //    eyeX = x, eyeY = y, eyeZ = z;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_G && action == GLFW_PRESS)                   //Gate Open/Close
    {
        isGateOpening *= -1.0f;
    }


    //***************Lighting***************

    if ((key == GLFW_KEY_L && action == GLFW_PRESS))                   //Lighting On/Off
    {
        lightingOn ^= true;
    }
    if ((key == GLFW_KEY_0 && action == GLFW_PRESS))                   //Dark On/Off
    {
        dark ^= true;
        if (dark)
        {
            pointLightOn[0] = 1.0;
            pointLight1.turnOn();
            pointLightOn[1] = 1.0;
            pointLight2.turnOn();
            pointLightOn[2] = 1.0;
            pointLight3.turnOn();
            pointLightOn[3] = 1.0;
            pointLight4.turnOn();
            directionalLightOn = 0.0;
            directionalLight.turnOff();
        }
        else
        {
            pointLightOn[0] = 0.0;
            pointLight1.turnOff();
            pointLightOn[1] = 0.0;
            pointLight2.turnOff();
            pointLightOn[2] = 0.0;
            pointLight3.turnOff();
            pointLightOn[3] = 0.0;
            pointLight4.turnOff();
            directionalLightOn = 1.0;
            directionalLight.turnOn();
        }
    }

    if (key == GLFW_KEY_7 && action == GLFW_PRESS)          //Ambient On/Off
    {
        if (ambientOn == 0.0)                   
        {
            ambientOn = 1.0;
            pointLight1.turnAmbientOn();
            pointLight2.turnAmbientOn();
            pointLight3.turnAmbientOn();
            pointLight4.turnAmbientOn();
            spotLight.turnAmbientOn();
            directionalLight.turnAmbientOn();
        }
        else
        {
            ambientOn = 0.0;
            pointLight1.turnAmbientOff();
            pointLight2.turnAmbientOff();
            pointLight3.turnAmbientOff();
            pointLight4.turnAmbientOff();
            spotLight.turnAmbientOff();
            directionalLight.turnAmbientOff();
        }
    }

    if (key == GLFW_KEY_8 && action == GLFW_PRESS)          //Diffuse On/Off
    {
        if (diffuseOn == 0.0)
        {
            diffuseOn = 1.0;
            pointLight1.turnDiffuseOn();
            pointLight2.turnDiffuseOn();
            pointLight3.turnDiffuseOn();
            pointLight4.turnDiffuseOn();
            spotLight.turnDiffuseOn();
            directionalLight.turnDiffuseOn();
        }
        else
        {
            diffuseOn = 0.0;
            pointLight1.turnDiffuseOff();
            pointLight2.turnDiffuseOff();
            pointLight3.turnDiffuseOff();
            pointLight4.turnDiffuseOff();
            spotLight.turnDiffuseOff();
            directionalLight.turnDiffuseOff();
        }
    }

    if (key == GLFW_KEY_9 && action == GLFW_PRESS)          //Specular On/Off
    {
        if (specularOn == 0.0)
        {
            specularOn = 1.0;
            pointLight1.turnSpecularOn();
            pointLight2.turnSpecularOn();
            pointLight3.turnSpecularOn();
            pointLight4.turnSpecularOn();
            spotLight.turnSpecularOn();
            directionalLight.turnSpecularOn();
        }
        else
        {
            specularOn = 0.0;
            pointLight1.turnSpecularOff();
            pointLight2.turnSpecularOff();
            pointLight3.turnSpecularOff();
            pointLight4.turnSpecularOff();
            spotLight.turnSpecularOff();
            directionalLight.turnSpecularOff();
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)          //Point Light 1 On/Off
    {
        if (pointLightOn[0] == 0.0)
        {
            pointLightOn[0] = 1.0;
            pointLight1.turnOn();
        }
        else
        {
            pointLightOn[0] = 0.0;
            pointLight1.turnOff();
        }
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)          //Point Light 2 On/Off
    {
        if (pointLightOn[1] == 0.0)
        {
            pointLightOn[1] = 1.0;
            pointLight2.turnOn();
        }
        else
        {
            pointLightOn[1] = 0.0;
            pointLight2.turnOff();
        }
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)          //Point Light 3 On/Off
    {
        if (pointLightOn[2] == 0.0)
        {
            pointLightOn[2] = 1.0;
            pointLight3.turnOn();
        }
        else
        {
            pointLightOn[2] = 0.0;
            pointLight3.turnOff();
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)          //Point Light 4 On/Off
    {
        if (pointLightOn[3] == 0.0)
        {
            pointLightOn[3] = 1.0;
            pointLight4.turnOn();
        }
        else
        {
            pointLightOn[3] = 0.0;
            pointLight4.turnOff();
        }
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS)          //Directional Light 1 On/Off
    {
        if (directionalLightOn == 0.0)
        {
            directionalLightOn = 1.0;
            directionalLight.turnOn();
        }
        else
        {
            directionalLightOn = 0.0;
            directionalLight.turnOff();
        }
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)          //Spot Light 1 On/Off
    {
        if (spotLightOn == 0.0)
        {
            spotLightOn = 1.0;
            spotLight.turnOn();
        }
        else
        {
            spotLightOn = 0.0;
            spotLight.turnOff();
        }
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS)                   //Ferris Wheel On/Off
    {
        isFerrisWheelOn ^= true;
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)                   //Boat On/Off
    {
        isBoatOn ^= true;
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS)                   //Rotor On/Off
    {
        isRotorOn ^= true;
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)                   //SkyDrop On/Off
    {
        isSkyDropOn ^= true;
    }

    if (key == GLFW_KEY_J && action == GLFW_PRESS)                   //Carousel On/Off
    {
        isCarousalOn ^= true;
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS)                   //Ali Baba On/Off
    {
        isAliOn ^= true;
    }
    


    if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)                   //Rotation testing
    {
        rotateAngleTest_Y += speed;
        rotateAngleTest_Y = fmod(rotateAngleTest_Y, 360);
        /*cout << rotateAngleTest_Y << endl;*/
    }
    if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)                   //Rotation testing
    {
        rotateAngleTest_Y -= speed;
        rotateAngleTest_Y = fmod(rotateAngleTest_Y, 360);
        /*cout << rotateAngleTest_Y << endl;*/
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


//Texture Loading

void load_texture(unsigned int &texture, string image_name, GLenum format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture "<<image_name << endl;
    }
    stbi_image_free(data);
}

void SetupPointLight(PointLight &pointLight, Shader ourShader, int lightNum)
{
    pointLight.setUpLight(ourShader);
    if (!pointLightOn[pointLight.lightNumber - 1])
        pointLight.turnOff();
    if (!ambientOn)
        pointLight.turnAmbientOff();
    if (!diffuseOn)
        pointLight.turnDiffuseOff();
    if (!specularOn)
        pointLight.turnSpecularOff();
}



//OBJECTS

void drawCube(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y, glm::vec4 color, float spec, float shininess)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix *scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);

    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * spec);
    ourShader.setFloat("material.shininess", shininess);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawCube1(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y, glm::vec4 color, float spec, float shininess)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);

    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * spec);
    ourShader.setFloat("material.shininess", shininess);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Table(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{

    float tableY = 0.05f;
    // Rotation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot;

    pivot = glm::vec3(0.25, 0.25 + tableY, 0.25);
    
    //Top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + tableY, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.1f, 1.3f));

    translateToPivotOrg = scaleMatrix * glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = scaleMatrix * glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    
    //model = translateMatrix * rotateYMatrix * scaleMatrix;
    //model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix*model);
    ourShader.setVec4("material.ambient", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setFloat("material.shininess", 128.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    pivot = translateFromPivotOrg;
    // Leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.02f, -0.46f + tableY, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));
    
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.02f, -0.46f + tableY, 0.559f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.92f, -0.46f + tableY, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.92f, -0.46f + tableY, 0.559f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Chair(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;
    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot;

    float fixLeg = 0.7f;
    float fixY = 0.1f;
    float fixYWhole = -0.08f;

    pivot = glm::vec3(0.25 + 0.11, 0.25 - fixY + fixYWhole, 0.25);

    // Top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.11f, 0.0f - fixY + fixYWhole, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.75f, 0.07f, 0.7f));

    translateToPivotOrg = scaleMatrix * glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = scaleMatrix * glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    pivot = translateFromPivotOrg;
    // Leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    /*ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);*/
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //// Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.43f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    /*ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);*/
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.43f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    /*ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);*/
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Left up 
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.17f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.25f, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Right up
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.39f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.25f, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Back support
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.15f, 0.09f - fixY + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Floor (Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 1.25f + 0.45f, -16.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(40.3f, 0.1f, 60.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Road(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 1.25f + 0.45f, -16.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 0.1f, 60.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Boundary(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    //Left Side
    glBindTexture(GL_TEXTURE_2D, texture2);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -0.42f, 15.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(18.2f, 2.2f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //Left Wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -0.42f, 15.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.2f, -57.8f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Right Side
    translateMatrix = glm::translate(identityMatrix, glm::vec3(18.15f, -0.42f, 15.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-17.2f, 2.2f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //Right Wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(18.15f, -0.42f, 15.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.2f, 2.2f, -57.8f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -0.42f, -14.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(19.2f, 2.2f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(18.15f, -0.42f, -14.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-21.2f, 2.2f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Left Side Pillar
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -0.42f, 14.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 2.6f, 0.6f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Right Side Pillar
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.35f, -0.42f, 14.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 2.6f, 0.6f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void GuardRoom(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, unsigned int texture = texture0)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    //roof
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.35f, 0.95f, 12.86f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.25f, 4.3f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //back side
    glBindTexture(GL_TEXTURE_2D, texture);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.4f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.75f, 3.80f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right side
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 2.75f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //left side
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5f, -0.42f, 14.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 2.75f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //font side
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.75f, 0.6f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5f, -0.42f, 13.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.75f, 0.8f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5f, -0.42f, 13.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.2f, 1.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5f, 0.6f, 13.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.9f, 1.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //5
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5f, 0.7f, 14.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.6f, 1.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //floor
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.401f, -0.42f, 13.001f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.38f, 0.1f, 3.7f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.1f);
    ourShader.setVec4("material.diffuse", color * 0.1f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.1f);
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



}

void Shop(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, unsigned int texture = texture0)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;


    //roof
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.7f, 0.95f, 12.86f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-4.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.8f, 0.29f, 6.0f));
    model = translateMatrix * rotateZMatrix *scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.5f);
    ourShader.setFloat("material.shininess", 256.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //back side
    glBindTexture(GL_TEXTURE_2D, texture);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.4f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.75f, 5.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right side
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.99f, -0.419f, 12.99f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 2.75f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //left side
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.99f, -0.419f, 15.651f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 2.75f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //front side
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.2f, 4.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //floor
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.01f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.9f, 0.1f, 5.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.1f);
    ourShader.setVec4("material.diffuse", color * 0.1f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.1f);
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    
}

void Gate(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glBindTexture(GL_TEXTURE_2D, texture0);
    // Rotation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    //static parts
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.3f, 0.6f, 15.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.05f, 0.05f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.3f, -0.2f, 15.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.3f, 0.6f, 15.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.3f, -0.2f, 15.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    

    //dynamic parts
    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot1, pivot2;

    pivot1 = glm::vec3(7.35, 0.0, 15.1125);
    pivot2 = glm::vec3(9.30, 0.0, 15.1125);

    gateAngle += isGateOpening * gateSpeed;
    if (isGateOpening == -1.0f)
        gateAngle = max(0.0f, gateAngle);
    if (isGateOpening == 1.0f)
        gateAngle = min(90.0f, gateAngle);

    //***left side***//
    translateToPivot = glm::translate(identityMatrix, -pivot1);
    translateFromPivot = glm::translate(identityMatrix, pivot1);
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(gateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    //square
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.34f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.0f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.26f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.34f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.9f, 0.1f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.34f, 0.65f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.9f, 0.1f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //bars
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.52f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.1f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.7f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.2f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.88f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.3f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.06f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



    //***right side//
    translateToPivot = glm::translate(identityMatrix, -pivot2);
    translateFromPivot = glm::translate(identityMatrix, pivot2);
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-gateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    //square
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.25f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.0f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.33f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.33f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.9f, 0.1f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.33f, 0.65f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.9f, 0.1f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //bars
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.07f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.1f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.89f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.2f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.71f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.3f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.53f, -0.3f, 15.09f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


}

void LampPost(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;

    //base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.01f, -0.42f, 15.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.3f, 0.7f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //pillar
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.17f, -0.42f, 15.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.5f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.3f, 0.4f, 0.3f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.3f, 0.4f, 0.3f, 1.0f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Stage(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{
    glBindTexture(GL_TEXTURE_2D, texture0);
    // Rotation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot;

    pivot = glm::vec3(0.25, 0.25, 0.25);

    //Base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -0.43f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 1.2f, 3.5f));

    translateToPivotOrg = scaleMatrix * glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = scaleMatrix * glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;

    //model = translateMatrix * rotateYMatrix * scaleMatrix;
    //model = translateMatrix * scaleMatrix;
    glm::vec4 clr = glm::vec4(100.0/256, 100.0/256, 100/256, 1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", clr);
    ourShader.setVec4("material.diffuse", clr);
    ourShader.setVec4("material.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    pivot = translateFromPivotOrg;
    // Back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -0.43f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 3.5f, 0.2f));

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    clr = glm::vec4(17.0 / 256, 17.0 / 256, 20 / 256, 1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", clr);
    ourShader.setVec4("material.diffuse", clr);
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 256.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Stairs
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, -0.43f, 0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 1.5f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    clr = glm::vec4(43.0 / 256, 22.0 / 256, 4 / 256, 1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", clr);
    ourShader.setVec4("material.diffuse", clr);
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.43f, 0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.8f, 1.5f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.43f, 0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.8f, 1.5f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, -0.43f, 0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 1.5f));
    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void FoodPlace(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, scaleMatrix, model;

    //Floor
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, -0.44f, 2.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(8.3f, 0.1f, 11.0f));
    model = translateMatrix * scaleMatrix;
    glm::vec4 color = glm::vec4(8.0 / 256, 20.0 / 256, 4.0 / 256, 1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, 0.0f, 4.0f));
    Stage(ourShader, moveMatrix * translateMatrix, 90);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 2.0f));
    TableChairSet(ourShader, moveMatrix * translateMatrix, rotateAngleTest_Y);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
    TableChairSet(ourShader, moveMatrix * translateMatrix, rotateAngleTest_Y);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, 0.66f));
    TableChairSet(ourShader, moveMatrix * translateMatrix, rotateAngleTest_Y);


}

void TableChairSet(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 4.0f));
    Table(ourShader, moveMatrix * translateMatrix, 90.0);

    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.70f, 0.0f, 4.75f));
    Chair(ourShader, moveMatrix * translateMatrix, 0);

    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.25f, 0.0f, 4.15f));
    Chair(ourShader, moveMatrix * translateMatrix, 90.0f);

    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.73f, 0.0f, 3.55f));
    Chair(ourShader, moveMatrix * translateMatrix, 180.0f);

    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.2f, 0.0f, 4.15f));
    Chair(ourShader, moveMatrix * translateMatrix, -90.0f);
}

void read_file(string file_name, vector<float> &vec)
{
    ifstream file(file_name);
    float number;

    while (file >> number)
        vec.push_back(number);

    file.close();
}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float> &coordinates, vector<float> &normals, vector<int> &indices, vector<float> &vertices)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void drawCurve(Shader ourShader, glm::mat4 moveMatrix, unsigned int VAO, vector<int> indices, glm::vec4 color, float rotateAngleTest_Y)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;

    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);                       
    glBindVertexArray(0);
}

void FerrisWheel(Shader ourShader, glm::mat4 moveMatrix)
{

    //front
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateTemp;

    if (isFerrisWheelOn)
    {
        ferrisWheelSpeed += ferrisIncrement;
        ferrisWheelSpeed = min(2.5f, ferrisWheelSpeed);
    }
    else 
    {
        ferrisWheelSpeed -= ferrisIncrement;
        ferrisWheelSpeed = max(0.02f, ferrisWheelSpeed);
    }

    if (ferrisWheelSpeed != 0.02f)
        ferrisWheelAngle += ferrisWheelSpeed;

    ferrisWheelAngle = fmod(ferrisWheelAngle, 360);
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(ferrisWheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateZMatrix * scaleMatrix * rotateTemp;

    glm::vec4 color = glm::vec4(48.0 / 256, 45.0 / 256, 181.0 / 256, 1.0f);
    //glm::vec4 color = glm::vec4(1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    //bars
    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec4 pivot = glm::vec4(glm::vec3(0.0f), 1.0f);

    pivot = translateMatrix * pivot;

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -0.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.0f, 0.1f));
    for (float i = 0.0f; i >= -360.0f; i -= 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }



    //back
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(ferrisWheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateZMatrix * scaleMatrix * rotateTemp;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    //bars

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -1.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.0f, 0.1f));
    for (float i = 0.0f; i >= -360.0f; i -= 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


    //connecting bars
    //middle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.975f, 1.975f, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 4.0f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //top ones
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 3.5f, -1.005f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 2.0f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    for (float i = 0.0f; i >= -360.0f; i -= 90.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    //pillars
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, -0.41f, 0.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -0.47f, 0.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, -0.41f, -1.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -0.47f, -1.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    color = glm::vec4(1.0f);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    //sitting places
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    FerrisWheelSeat(ourShader, translateMatrix, ferrisWheelAngle);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 1.5f, 0.0f));
    FerrisWheelSeat(ourShader, translateMatrix, ferrisWheelAngle);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 3.0f, 0.0f));
    FerrisWheelSeat(ourShader, translateMatrix, ferrisWheelAngle);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 1.5f, 0.0f));
    FerrisWheelSeat(ourShader, translateMatrix, ferrisWheelAngle);


}

void FerrisWheelSeat(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrixLocal, rotateZMatrixMain, rotateTemp;
    glm::mat4 translateToPivotMain, translateFromPivotMain;
    glm::vec4 translateToPivotOrgMain, translateFromPivotOrgMain;
    glm::mat4 translateToPivotLocal, translateFromPivotLocal;
    glm::vec4 translateToPivotOrgLocal, translateFromPivotOrgLocal;
    glm::vec4 pivot1, pivot2;
    glm::vec4 mainPivotPoint = glm::vec4(3.0f, 2.0f, 0.0f, 1.0);
    glm::vec4 localPivotPoint = glm::vec4(2.975f, 0.475f, -1.005f, 1.0);

    pivot1 = mainPivotPoint;
    translateToPivotMain = glm::translate(identityMatrix, glm::vec3(-pivot1));
    translateFromPivotMain = glm::translate(identityMatrix, glm::vec3(pivot1));
    rotateZMatrixMain = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    pivot2 = translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * moveMatrix* localPivotPoint;
    translateToPivotLocal = glm::translate(identityMatrix, glm::vec3(-pivot2));
    translateFromPivotLocal = glm::translate(identityMatrix, glm::vec3(pivot2));
    rotateZMatrixLocal = glm::rotate(identityMatrix, glm::radians(-rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    glBindTexture(GL_TEXTURE_2D, texture3);
    //base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.1f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, texture8);
    //left
    //seat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //seat right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.2f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //top bars
    glBindTexture(GL_TEXTURE_2D, texture0);
    //left
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.899f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.15f, 0.1f, 0.05f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.1751f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.15f, 0.1f, 0.05f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //right
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.899f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.15f, 0.1f, 0.05f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.1751f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.15f, 0.1f, 0.05f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}

void PirateShip(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z)
{

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateTemp;

    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec4 pivot = glm::vec4(glm::vec3(14.0f, 2.0f, 4.25f), 1.0f);

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    glm::vec4 color = glm::vec4(186.0 / 256, 60.0 / 256, 123.0 / 256, 1.0f);

    float currentTime = static_cast<float>(glfwGetTime());
    if (currentTime - lastTime > 0.1)
    {
        if (isBoatOn)
            boatAngle += boatSpeed;
        if (abs(boatAngle) >= boatMaxAngle)
        {
            boatSpeed *= -1;
            lastTime = static_cast<float>(glfwGetTime());
        }
    }
    if (!isBoatOn)
    {
        if (boatAngle < 0)
        {
            boatAngle += abs(boatSpeed);
        }
        if (boatAngle > 0)
        {
            boatAngle -= abs(boatSpeed);
        }
    }


    rotateAngleTest_Z = boatAngle;
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    //dynamic
    //boat
    glBindTexture(GL_TEXTURE_2D, texture3);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -1.3f, 4.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 0.3f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(boatVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesBoat.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    //base
    color = glm::vec4(150.0 / 256, 12.0 / 256, 35.0 / 256, 1.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, 0.2f, 4.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.02f, 0.248f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(boatVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesBoat.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, texture0);
    //bars
    color = glm::vec4(1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    
    float moveX, moveY, moveZ;
    moveX = 11.0f, moveY = 0.0f, moveZ = 5.0f;
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.975f + moveX, 0.5f + moveY, -1.2f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.95f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.575f + moveX, 0.5f + moveY, -1.2f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.95f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.375f + moveX, 0.5f + moveY, -1.2f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.95f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //middle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.975f + moveX, 1.975f + moveY, -2.0f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 5.0f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //front bars
    //1
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-18.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(13.5f, 0.6f, 4.85f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.3f, 0.01f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.5f, 0.6f, 4.85f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.3f, 0.01f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //back bars
    //1
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-18.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(13.5f, 0.6f, 3.745f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.3f, 0.01f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.5f, 0.6f, 3.745f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 3.3f, 0.01f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //static
    //pillars
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f + moveX, -0.41f + moveY, 0.1f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f + moveX, -0.47f + moveY, 0.1f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f + moveX, -0.41f + moveY, -1.75f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f + moveX, -0.47f + moveY, -1.75f + moveZ));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Rotor(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateTemp, rotateYMatrix;

    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec4 pivot = glm::vec4(glm::vec3(3.0f, 0.0f, -5.3f), 1.0f);

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    if (isRotorOn)
    {
        rotorSpeed += rotorIncrement;
        rotorSpeed = min(3.5f, rotorSpeed);
    }
    else
    {
        rotorSpeed -= rotorIncrement;
        rotorSpeed = max(0.02f, rotorSpeed);
    }

    if (rotorSpeed != 0.02f)
        rotorAngle += rotorSpeed;

    rotorAngle = fmod(rotorAngle, 360);
    rotateAngleTest_Z = rotorAngle;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 1.0f, 0.0f));
    
    //base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.75f, -0.4f, -5.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.55f, 1.0f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    glm::vec4 color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    color = glm::vec4(60.0 / 256, 106.0 / 256, 117.0 / 256, 1.0f);
    //Circle
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 0.0f, -5.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.35f, 0.7f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(rotorVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesRotor.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    color = glm::vec4(112.0 / 256, 161.0 / 256, 151.0 / 256, 1.0f);
    //cylinder
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, -0.2f, -5.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cylinderVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesCylinder.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);


    //handles
    ourShader.setVec4("material.ambient", color * 0.3f);
    ourShader.setVec4("material.diffuse", color * 0.3f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cylinderVAO);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.9f, 0.62f, -5.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.05f, 0.1f));

    for (float i = 0.0f; i <= 360.f; i += 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateFromPivot * rotateYMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesCylinder.size(), GL_UNSIGNED_INT, (void*)0);
    }
    
}

void SkyDrop(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    //base
    glBindTexture(GL_TEXTURE_2D, texture3);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.50f, -0.4f, -12.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.0f, 0.5f, 5.0f));
    model = translateMatrix * scaleMatrix;
    glm::vec4 color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //back
    glBindTexture(GL_TEXTURE_2D, texture5);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, -0.4f, -12.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 20.0f, 0.5f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    float currentTime = static_cast<float>(glfwGetTime());
    if ((currentTime - lastTimeSky < 0.2) && skyDropCurrent == 1.0f)
    { }
    else if(isSkyDropOn)
    {
        if (skyDropCurrent == 1.0f)
            skyDropY += skyDropUpSpeed;
        else
            skyDropY -= skyDropDownSpeed;
        
        if (skyDropY >= skyDropMax)
            skyDropCurrent = -1.0f;
        if (skyDropY <= skyDropMin)
        {
            skyDropCurrent = 1.0f;
            lastTimeSky = static_cast<float>(glfwGetTime());
            skyDropY = skyDropMin;
        }

    }
    //dynamic
    //seat
    //base
    glBindTexture(GL_TEXTURE_2D, texture8);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.35f + skyDropY, -11.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 0.2f, 1.5f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.35f + skyDropY, -11.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 1.5f, 0.2f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //left handle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.35f + skyDropY, -11.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 1.5f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //right handle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.65f, 0.35f + skyDropY, -11.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 1.5f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //left back bar
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.2f, 0.9f + skyDropY, -12.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 1.38f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //right back bar
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.95f, 0.9f + skyDropY, -12.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 1.38f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



}

void Carousal(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateTemp, rotateYMatrix, rotateTempZ, rotateTempX;

    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec4 pivot = glm::vec4(glm::vec3(14.0f, 1.2f, -4.25f), 1.0f);

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    if (isCarousalOn)
    {
        carousalSpeed += carousalIncrement;
        carousalSpeed = min(3.5f, carousalSpeed);
    }
    else
    {
        carousalSpeed -= carousalIncrement;
        carousalSpeed = max(0.02f, carousalSpeed);
    }

    if (carousalSpeed != 0.02f)
        carousalAngle += carousalSpeed;

    carousalAngle = fmod(carousalAngle, 360);
    rotateAngleTest_Z = -carousalAngle;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec4 color = glm::vec4(39.0 / 256, 145.0 / 256, 68.0 / 256, 1.0f);
    //Circle
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, 1.2f, -4.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(carousalVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesCarousal.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    color = glm::vec4(112.0 / 256, 161.0 / 256, 151.0 / 256, 1.0f);
    //cylinder
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -0.46f, -4.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.15f, 0.2f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cylinderVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesCylinder.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    //Seats
    color = glm::vec4(0.7f);
    ourShader.setVec4("material.ambient", color * 0.7f);
    ourShader.setVec4("material.diffuse", color * 0.7f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cubeVAO);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(12.2f, 0.60f, -4.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.4f, 0.1f));
    rotateTempZ = glm::rotate(identityMatrix, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateTempX = glm::rotate(identityMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    for (float i = 0.0f; i <= 360.f; i += 60.0f)
    {
        //handles
        glBindTexture(GL_TEXTURE_2D, texture0);
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 2.25f, -4.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
        model = translateFromPivot * rotateYMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * rotateTempZ * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesCylinder.size(), GL_UNSIGNED_INT, (void*)0);

        //boxes
        glBindTexture(GL_TEXTURE_2D, texture6);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(11.875f, 2.25f, -4.55f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 1.0f));
        model = translateFromPivot * rotateYMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * rotateTempZ * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesCylinder.size(), GL_UNSIGNED_INT, (void*)0);

        //head
        translateMatrix = glm::translate(identityMatrix, glm::vec3(11.6f, 4.55f, -3.70f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.5f, 0.7f));
        model = translateFromPivot * rotateYMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * rotateTempX * rotateTempZ * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(headVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesHead.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

    }

}

void AliBaba(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z)
{
    //front
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrixMain, rotateTemp, rotateZMatrixLocal;

    if (isAliOn)
    {
        aliSpeed += aliIncrement;
        aliSpeed = min(2.5f, aliSpeed);
    }
    else
    {
        aliSpeed -= aliIncrement;
        aliSpeed = max(0.02f, aliSpeed);
    }

    if (aliSpeed != 0.02f)
        aliAngle += aliSpeed;

    aliAngle = fmod(aliAngle, 360);
    rotateAngleTest_Z = aliAngle;

    //base
    glBindTexture(GL_TEXTURE_2D, texture5);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, -0.4f, -12.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 0.3f, 3.5f));
    model = translateMatrix * scaleMatrix;
    glm::vec4 color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //back
    glBindTexture(GL_TEXTURE_2D, texture4);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(13.5f, -0.4f, -12.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 8.0f, 0.5f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Circle
    glBindTexture(GL_TEXTURE_2D, texture0);
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(13.74f, 3.48f, -11.84f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(carousalVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesCarousal.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    //connector
    glBindTexture(GL_TEXTURE_2D, texture4);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(13.715f, 3.455f, -12.2f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.8f));
    model = translateMatrix * scaleMatrix;
    color = glm::vec4(0.5f);
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



    glm::mat4 translateToPivotMain, translateFromPivotMain;
    glm::vec4 translateToPivotOrgMain, translateFromPivotOrgMain;
    glm::mat4 translateToPivotLocal, translateFromPivotLocal;
    glm::vec4 translateToPivotOrgLocal, translateFromPivotOrgLocal;
    glm::vec4 pivot1, pivot2;
    glm::vec4 mainPivotPoint = glm::vec4(13.74f, 3.48f, -11.84f, 1.0);
    glm::vec4 localPivotPoint = glm::vec4(13.74f, 0.475f, -12.0f, 1.0);

    pivot1 = mainPivotPoint;
    translateToPivotMain = glm::translate(identityMatrix, glm::vec3(-pivot1));
    translateFromPivotMain = glm::translate(identityMatrix, glm::vec3(pivot1));
    rotateZMatrixMain = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    pivot2 = translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * localPivotPoint;
    translateToPivotLocal = glm::translate(identityMatrix, glm::vec3(-pivot2));
    translateFromPivotLocal = glm::translate(identityMatrix, glm::vec3(pivot2));
    rotateZMatrixLocal = glm::rotate(identityMatrix, glm::radians(-rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));


    //hand
    glBindTexture(GL_TEXTURE_2D, texture6);
    color = glm::vec4(1.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(13.62f, 0.4f, -11.99f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 10.0f, 0.25f));
    model = translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 256.0f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //seat
    //base
    glBindTexture(GL_TEXTURE_2D, texture9);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(12.5f, 0.05f, -11.89f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 0.2f, 1.5f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(12.5f, 0.05f, -11.89f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 1.5f, 0.2f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //left handle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(12.5f, 0.05f, -11.89f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 1.5f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //right handle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.65f, 0.05f, -11.89f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 1.5f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


}

void ShowFunctions()
{
    cout << "Functions:\n~~~~~~~~~~~~~~~~~~~~\n";

    cout << "\nMovements:\n-----------------------------------\n";
    cout << "Move (Forward, Backward, Left, Right): (W, S, A, D)\n";
    cout << "Move (Up, Down): (E, R)\n";
    cout << "Pitch (Up, Down): (X, C)\n";
    cout << "Yaw (Right, Left): (Y, U)\n";
    cout << "Roll (Right, Left): (Z, Q)\n";
    cout << "-----------------------------------\n\n";

    cout << "\nLighting:\n-----------------------------------\n";
    cout << "Day and Night Toggle: 0\n";
    cout << "Point Light 1 ON/OFF: 1\n";
    cout << "Point Light 2 ON/OFF: 2\n";
    cout << "Point Light 3 ON/OFF: 3\n";
    cout << "Point Light 4 ON/OFF: 4\n";
    cout << "Directional Light ON/OFF: 5\n";
    cout << "Spot Light ON/OFF: 6\n";
    cout << "Ambient ON/OFF: 7\n";
    cout << "Diffuse ON/OFF: 8\n";
    cout << "Specular ON/OFF: 9\n";
    cout << "-----------------------------------\n\n";

    cout << "\nDynamic Objects:\n-----------------------------------\n";
    cout << "Gate Open/Close: G\n";
    cout << "Ferris Wheel Start/Stop: V\n";
    cout << "Pirate Boat Start/Stop: B\n";
    cout << "Rotor Start/Stop: N\n";
    cout << "SkyDrop Start/Stop: M\n";
    cout << "Carousel Start/Stop: J\n";
    cout << "Ali Baba Start/Stop: K\n";
    cout << "-----------------------------------\n\n";
}

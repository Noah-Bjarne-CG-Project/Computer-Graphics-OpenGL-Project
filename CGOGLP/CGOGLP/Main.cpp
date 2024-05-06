#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader_ogl.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

#include <iostream>

#include "Model.h"

//is momenteel in model.h PAS OP MAG MAAR 1 KEER TOEGEVOEGD WORDEN ANDERS BIG PROBLEM
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>


#include <vector>


//Functions
void inputProcessor(GLFWwindow* window);
void mouse_movement(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadCubemap(vector<std::string> faces);
glm::vec3 calculateRayDirection( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
void terrainHeightEdditor(float x, float z, int height, int width);
glm::vec3 calculateRayTerrainIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, int width, int height);
void drawCrosshair();

//Settings
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float PLAYER_HEIGHT = 1.0f;
const glm::vec4 LIGHTCOLLOR_SUN = glm::vec4(0.85f, 0.86f, 0.80f, 1.0f);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


//Cube
float verticesCube[] = {
    //      Coordinates       /    normals
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
  
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f, 
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f, 
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f, 
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f, 
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f, 
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f, 
  
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 
  
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f, 
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f, 
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f, 
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f, 
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f, 
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f, 
  
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f, 
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f, 
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f, 
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f, 
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f, 
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f, 
  
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f, 
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f, 
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f, 
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f, 
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f, 
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f, 
};

float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    // Right
    1, 2, 6,
    6, 5, 1,
    // Left
    0, 4, 7,
    7, 3, 0,
    // Top
    4, 5, 6,
    6, 7, 4,
    // Bottom
    0, 3, 2,
    2, 1, 0,
    // Back
    0, 1, 5,
    5, 4, 0,
    // Front
    3, 7, 6,
    6, 2, 3
};


// world space positions of our cubes
std::vector <glm::vec3> cubePositions = {
        glm::vec3(10.0f,  18.0f,  -10.0f),
        glm::vec3(12.0f,  15.0f, -15.0f)
};

/*
Licht wordt nu overal opgesmete vanboven met directional light. Aangezien enkel zon nu er is cv.
Maar denk dat effectief meer licht bronnen toegevoegd moetten worden ik denk da enkel
voor die lichtbronnen een array met licht posities nodig is, niet voor de zon want directional.
*/
glm::vec3 lightPositions[] = {
    glm::vec3(10.0f,  15.0f,  -10.0f),
    glm::vec3(20.0f,  15.0f,  28.0f)// light at shadowy place for test
};

std::vector<float> vertices;


int main()
{


    //configure glfw
    glfwInit();

    //Open Gl 3.3 voor compatibiliteit
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bjarne loves Jens xxx", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_movement);

    //Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Diepte buffer opengl
    glEnable(GL_DEPTH_TEST);

    //Loading shaders
    Shader terrainShaders("TerrainShader.vert", "TerrainShader.frag");
    Shader objectShaders("ObjectShader.vert", "ObjectShader.frag");
    Shader lightningShaders("LightShader.vert", "LightShader.frag");
    Shader modelShaders("ModelShader.vert", "ModelShader.frag");
    Shader skyboxShader("Skybox.vert", "Skybox.frag");
    std::vector<std::string> faces
    {
        "Resources/Skybox/right.jpg",
        "Resources/Skybox/left.jpg",
        "Resources/Skybox/top.jpg",
        "Resources/Skybox/bottom.jpg",
        "Resources/Skybox/front.jpg",
        "Resources/Skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);



    // load height map texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Resources/Heightmaps/iceland_heightmap.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width <<" nr channels: "<< nrChannels << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }


    //Uv coordinaten berekenen
    std::vector<float> texCoords;
    float repeat = 100.0f; // Adjust this value to change the number of times the texture repeats
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float u = ((float)j / (float)(width - 1)) * repeat;
            float v = ((float)i / (float)(height - 1)) * repeat;
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }


    // vertex generation
    float lowestPoint = 100.0f, heighestPoint = 0.0f;
    
    float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    int rez = 1;
    unsigned bytePerPixel = nrChannels;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            float vx = -height / 2.0f + height * i / (float)height;
            float vy = (int)y * yScale - yShift;
            float vz = -width / 2.0f + width * j / (float)width;
            vertices.push_back(vx);   // vx
            vertices.push_back(vy);   // vy
            vertices.push_back(vz);   // vz

            if (vy > heighestPoint) {
                heighestPoint = vy;
            }
            if (vy < lowestPoint) {
                lowestPoint = vy;
            }

            // texture coordinates
            vertices.push_back(texCoords[2 * (i * width + j)]);   // u
            vertices.push_back(texCoords[2 * (i * width + j) + 1]); // v

            // calculate normals (Dit iets beter denk)
            if (i > 0 && j > 0 && i < height - 1 && j < width - 1)
            {
                int index1 = ((i - 1) * width + j) * 8; // 8 because each vertex has 8 components (vx, vy, vz, u, v, nx, ny, nz)
                int index2 = (i * width + (j - 1)) * 8;
                glm::vec3 v1 = glm::vec3(vx - vertices[index1], vy - vertices[index1 + 1], vz - vertices[index1 + 2]);
                glm::vec3 v2 = glm::vec3(vx - vertices[index2], vy - vertices[index2 + 1], vz - vertices[index2 + 2]);
                glm::vec3 normal = glm::normalize(glm::cross( v2, v1));
                vertices.push_back(normal.x); //nx
                vertices.push_back(normal.y); //ny
                vertices.push_back(normal.z); //nz
            }
            else
            {
                vertices.push_back(0.0f); //nx
                vertices.push_back(1.0f); //ny
                vertices.push_back(0.0f); //nz
            }

        }
    }

    //Voor debugging nog weg halen
    std::cout << "maxsize: " << vertices.max_size() << std::endl;
    std::cout << "size: " << vertices.size() << std::endl;
  
    //mess extra uv array leegmaken voor data besparing

    //freeup image data
    stbi_image_free(data);

    // index generation
    std::vector<unsigned> indices;
    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }

    const unsigned int NUM_STRIPS = (height - 1) / rez;
    const unsigned int NUM_VERTS_PER_STRIP = (width / rez) * 2 - 2;

    // Mess dit naar andere file/ apparte classes vor cleanup?
    //EBO = element array buffer
    unsigned int terrainVAO, terrainVBO, terrainEBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Configure position attribute
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Configure texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configure texture coordinate attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    //shaders.use();

    //Texture grass
    int widthImg, heigthImg, numColCh;
    unsigned char* bytes = stbi_load("Resources/Textures/grassy2.jpg", &widthImg, &heigthImg, &numColCh, 0);
    if (bytes)
    {
        std::cout << "Loaded himage " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    //soort van texture array gedoe
    glActiveTexture(GL_TEXTURE0); // texture 0 = grass now
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //gl repear gedoe is per axis ingesteld
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //GL_RGB = jpg -> GL_RGBA = png
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heigthImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D,0);

    GLuint tex0Uni = glGetUniformLocation(terrainShaders.ID, "tex0");
    terrainShaders.use();
    glUniform1i(tex0Uni,0);

    //Texture snow
    int widthImga, heigthImga, numColCha;
    unsigned char* bytesSnow = stbi_load("Resources/Textures/snowy.jpg", &widthImga, &heigthImga, &numColCha, 0);
    if (bytesSnow)
    {
        std::cout << "Loaded SNOW " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    GLuint textureSnow;
    glGenTextures(1, &textureSnow);
    //soort van texture array gedoe
    glActiveTexture(GL_TEXTURE1); // texture 1 = snow
    glBindTexture(GL_TEXTURE_2D, textureSnow);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //gl repear gedoe is per axis ingesteld
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //GL_RGB = jpg -> GL_RGBA = png
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImga, heigthImga, 0, GL_RGB, GL_UNSIGNED_BYTE, bytesSnow);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytesSnow);
    glBindTexture(GL_TEXTURE_2D, 1);

    GLuint tex1Uni = glGetUniformLocation(terrainShaders.ID, "tex1");
    terrainShaders.use();
    glUniform1i(tex1Uni, 1);

    //Texture Sand
    int widthImgs, heigthImgs, numColChs;
    unsigned char* bytesSand = stbi_load("Resources/Textures/sandy.jpg", &widthImgs, &heigthImgs, &numColChs, 0);
    if (bytesSand)
    {
        std::cout << "Loaded sand " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    GLuint textureSand;
    glGenTextures(1, &textureSand);
    //soort van texture array gedoe
    glActiveTexture(GL_TEXTURE2); // texture 2 =Sand
    glBindTexture(GL_TEXTURE_2D, textureSand);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //gl repear gedoe is per axis ingesteld
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //GL_RGB = jpg -> GL_RGBA = png
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImgs, heigthImgs, 0, GL_RGB, GL_UNSIGNED_BYTE, bytesSand);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytesSand);
    glBindTexture(GL_TEXTURE_2D, 2);

    GLuint tex2Uni = glGetUniformLocation(terrainShaders.ID, "tex2");
    terrainShaders.use();
    glUniform1i(tex2Uni, 2);

    //Texture stone
    int widthImgb, heigthImgb, numColChb;
    unsigned char* bytesStone = stbi_load("Resources/Textures/stone.jpg", &widthImgb, &heigthImgb, &numColChb, 0);
    if (bytesStone)
    {
        std::cout << "Loaded Stone " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    GLuint textureStone;
    glGenTextures(1, &textureStone);
    //soort van texture array gedoe
    glActiveTexture(GL_TEXTURE3); // texture 3 = stone
    glBindTexture(GL_TEXTURE_2D, textureStone);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //gl repear gedoe is per axis ingesteld
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //GL_RGB = jpg -> GL_RGBA = png
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImgb, heigthImgb, 0, GL_RGB, GL_UNSIGNED_BYTE, bytesStone);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytesStone);
    glBindTexture(GL_TEXTURE_2D, 3);

    GLuint tex3Uni = glGetUniformLocation(terrainShaders.ID, "tex3");
    terrainShaders.use();
    glUniform1i(tex3Uni, 3);

    //lightning
    unsigned int lightVAO, lightVBO, lightEBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    lightningShaders.use();



    //objects
    unsigned int objectVAO, objectVBO, objectEBO;
    glGenVertexArrays(1, &objectVAO);
    glGenBuffers(1, &objectVBO);

    glBindVertexArray(objectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    objectShaders.use();

    // load models
    // -----------
    Model cartman("Resources/Models/Cartman/cartman.obj");//kleine reminder, vond dll niet. opgtelost me env path maar mess toch in vs oplossen


    //uniform dingen initialiseren
    objectShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
    lightningShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
    terrainShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
    //objectShaders.setVec3("lightPos", lightPositions[0]);
    //point lights
    objectShaders.setInt("amountOfLights", lightPositions->length());
    objectShaders.setVec3("pointLightPoses[0]", lightPositions[0]);
    objectShaders.setVec3("pointLightPoses[1]", lightPositions[1]);

    
    terrainShaders.setInt("amountOfLights", lightPositions->length());
    terrainShaders.setVec3("pointLightPoses[0]", lightPositions[0]);
    terrainShaders.setVec3("pointLightPoses[1]", lightPositions[1]);
    float heigthcalc = (heighestPoint - lowestPoint) / 4; //want 4 textures
    terrainShaders.setFloat("heigth0", 25.0f);
    terrainShaders.setFloat("heigth1", lowestPoint + (heigthcalc*2));
    terrainShaders.setFloat("heigth2", lowestPoint + (heigthcalc * 3));
    

    modelShaders.use();
    modelShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
    modelShaders.setInt("amountOfLights", lightPositions->length());
    modelShaders.setVec3("pointLightPoses[0]", lightPositions[0]);
    modelShaders.setVec3("pointLightPoses[1]", lightPositions[1]);

    

    skyboxShader.use();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

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

    





    int counter = 0;
    // render loop
    while (!glfwWindowShouldClose(window)) //order: Terrain -> Lights -> Objects
    {
        //Time calcs
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        inputProcessor(window);

        //render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        //Draw terrain
        terrainShaders.use();
        terrainShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
        terrainShaders.setVec3("camPos", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
        
        terrainShaders.setInt("amountOfLights", lightPositions->length());
        terrainShaders.setVec3("pointLightPoses[0]", lightPositions[0]);
        terrainShaders.setVec3("pointLightPoses[1]", lightPositions[1]);
        terrainShaders.setFloat("heigth0", lowestPoint + heigthcalc);
        terrainShaders.setFloat("heigth1", lowestPoint + (heigthcalc * 2));
        terrainShaders.setFloat("heigth2", lowestPoint + (heigthcalc * 3));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureSnow);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureSand);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textureStone);


        //coordinaten systemen (projection, view,model)
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.FieldOfVieuw), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f); //fov, aspect ratio, dichts zichtbare en verst zichtbare
        terrainShaders.setMat4("projection", projection);

        //Momenteel blokkerig maar wss oplosbaar
        //-------------------------------------------------------------- DIT OPRUIMEN LIKE IN FUNCTIE ZETTE OFZO NOG
        // Assuming camX and camZ are the x and z coordinates of the camera
        float camX, camZ;
        camX = camera.Position.x;
        camZ = camera.Position.z;
        /* PRINT ALS DEBUG MODE ME VARIABEL? OF TEVEEL INPAKT OP SYSTEEM?
        std::cout << "camX? " << camX << std::endl;
        std::cout << "camY? " << camZ << std::endl;
        */

        // Normalize the camera's position to the range of the terrain
        int normCamX = (camX + height / 2.0f) * (float)height / height;
        int normCamZ = (camZ + width / 2.0f) * (float)width / width;

        // Find the corresponding vertex in the terrain data
        int vertexIndex = 8 * (normCamX * width + normCamZ); // 8 because each vertex has 8 components (vx, vy, vz, u, v, nx, xy, xz)

        // Get the height of the terrain at the camera's position
        float terrainHeight = vertices[vertexIndex + 1]; // +1 because vy is the second component

        //std::cout << "Heigth? " << terrainHeight << std::endl;

        //--------------------------------------------------------------



        // camera/view transformation
        camera.SetCameraHeight(terrainHeight + PLAYER_HEIGHT);
        //camera.SetCameraHeight(12.0f);
        //std::cout << "camera height " << camera.GetHeight() << std::endl;
        glm::mat4 view = camera.GetViewMatrix();
        terrainShaders.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        terrainShaders.setMat4("model", model);


        // draw mesh
        glBindVertexArray(terrainVAO);

        //cool wiremesh thingy
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for (unsigned strip = 0; strip < NUM_STRIPS; strip++)
        {
            glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                NUM_VERTS_PER_STRIP + 2,   // number of indices to render
                GL_UNSIGNED_INT,     // index data type
                (void*)(sizeof(unsigned) * (NUM_VERTS_PER_STRIP + 2) * strip)); // offset to starting index
        }

        
        //Draw Lights 
        lightningShaders.use();
        lightningShaders.setMat4("view", view);
        lightningShaders.setMat4("model", model);
        lightningShaders.setMat4("projection", projection);
        lightningShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);

        // render sun
        glBindVertexArray(lightVAO);
        //lightPositions->length()
        for (unsigned int i = 0; i < 2; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 modelLight = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            modelLight = glm::translate(modelLight, lightPositions[i]);
            lightningShaders.setMat4("model", modelLight);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        


        //Draw objects
        objectShaders.use();
        objectShaders.setMat4("view", view);
        objectShaders.setMat4("model", model);
        objectShaders.setMat4("projection", projection);
        objectShaders.setMat4("camMatrix", (view*projection));// dit nazien!!
        
        objectShaders.setVec3("camPos", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
        objectShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
        
        objectShaders.setInt("amountOfLights", lightPositions->length());
        objectShaders.setVec3("pointLightPoses[0]", lightPositions[0]);
        objectShaders.setVec3("pointLightPoses[1]", lightPositions[1]);
        


        // render boxes
        glBindVertexArray(objectVAO);
        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objectShaders.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //render models
        //SHADERS MAKEEEEE
        
        modelShaders.use();
        modelShaders.setMat4("model", model);
        modelShaders.setMat4("camMatrix", (view * projection));
        modelShaders.setMat4("view", view);
        modelShaders.setMat4("projection", projection);
        modelShaders.setVec3("camPos", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
        modelShaders.setInt("amountOfLights", lightPositions->length());
        modelShaders.setVec3("pointLightPoses[0]", lightPositions[0]);
        modelShaders.setVec3("pointLightPoses[1]", lightPositions[1]);
        
        // render the loaded model
        glm::mat4 model1 = glm::mat4(1.0f);
        //height aanpassen op positie van waar die staat maar nu niet zo belangerijk
        model1 = glm::translate(model1, glm::vec3(-30.0f, 15.0f, 1.0f)); // translate it down so it's at the center of the scene
        model1 = glm::scale(model1, glm::vec3(1.0f, 1.0f, 1.0f));	// it's still too big (kleiner nr kleinre grootte)
        model1 = glm::rotate(model1, glm::radians(90.0f),glm::vec3(0.0f, 1.0f, 0.0f)); //rotate (licht wordt precies nie mee gedraait)
        modelShaders.setMat4("model", model1);
        cartman.Draw(modelShaders);

        glDepthFunc(GL_LEQUAL);

        skyboxShader.use();
        

        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
        // The last row and column affect the translation of the skybox (which we don't want to affect)
        view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up)));
        projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 64, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
        drawCrosshair();

        //Swap buffers
        glfwSwapBuffers(window);
        //Check for events
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &terrainVAO);
    glDeleteBuffers(1, &terrainVBO);
    glDeleteBuffers(1, &terrainEBO);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &objectVAO);
    glDeleteBuffers(1, &objectVBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &lightEBO);

    glfwTerminate();
    return 0;

}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


//Processes key inputs
void inputProcessor(GLFWwindow* window)
{   
    bool shift = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        shift = true;
        std::cout << "LSHIFT pressed" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, shift,deltaTime);
        std::cout << "W pressed" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, shift, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, shift, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, shift, deltaTime);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        std::cout << "Mouse left" << std::endl;

        glm::vec3 aaaa = calculateRayDirection( glm::perspective(glm::radians(camera.FieldOfVieuw), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f),camera.GetViewMatrix());
        glm::vec3 bbbb = calculateRayTerrainIntersection(camera.Position, aaaa, 1756, 2624);
        //std::cout << " x:" << aaaa.x << " y:" << aaaa.y << " z:"<< aaaa.z << std::endl;
        std::cout << " x:" << bbbb.x << " y:" << bbbb.y << " z:"<< bbbb.z << std::endl;
        //width and heigth of terrain file
        //terrainHeightEdditor(aaaa.x, aaaa.z,1756,2624);
        //cubePositions.push_back(glm::vec3(bbbb.x , bbbb.y, bbbb.z));
        cubePositions.push_back(glm::vec3(bbbb.x, bbbb.y, bbbb.z));
        std::cout << " size of thingy" << cubePositions.size() << std::endl;
    }
}

void terrainHeightEdditor(float x, float z, int height, int width) {

    /* PRINT ALS DEBUG MODE ME VARIABEL? OF TEVEEL INPAKT OP SYSTEEM?
    std::cout << "camX? " << camX << std::endl;
    std::cout << "camY? " << camZ << std::endl;
    */

    // Normalize the camera's position to the range of the terrain
    int normX = (x + height / 2.0f) * (float)height / height;
    int normZ = (z + width / 2.0f) * (float)width / width;

    // Find the corresponding vertex in the terrain data
    int vertexIndex = 8 * (normX * width + normZ); // 8 because each vertex has 8 components (vx, vy, vz, u, v, nx, xy, xz)

    // Get the height of the terrain at the camera's position
    //float terrainHeight = vertices[vertexIndex + 1];
    vertices[vertexIndex + 1] += 2.0f;

}

void mouse_movement(GLFWwindow* window, double xposIn, double yposIn)
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

//Lets program know the size of window was changed by user
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::vec3 calculateRayDirection(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
    
    // altijd midde van scherm
    float x = 0.0f;
    float y = 0.0f;

    // Apply perspective and view transformations
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    glm::vec4 rayWorld = glm::inverse(viewMatrix) * rayEye;

    // Normalize the direction vector
    glm::vec3 rayDir = glm::normalize(glm::vec3(rayWorld));
    return rayDir;
    
}

glm::vec3 calculateRayTerrainIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,  int height,int width ) {
    float t = 0.0f;
    float tStep = 0.1f; // You can adjust this value based on your needs
    float MAX_RAYCAST_DISTANCE = 100.0f;

    glm::vec3 currentPoint = rayOrigin;

    // Step along the ray until we find an intersection with the terrain
    while (true) {
        currentPoint += rayDirection * tStep;
        t += tStep;

        // Calculate the indices of the vertices that form the square that the current point is in
        int xIndex = (currentPoint.x + height / 2.0f) * (float)height / height;
        int zIndex = (currentPoint.z + width / 2.0f) * (float)width / width;
        int xIndex2 = ((currentPoint.x+0.5f) + height / 2.0f) * (float)height / height;
        int zIndex2 = (currentPoint.z + width / 2.0f) * (float)width / width;
        int xIndex3 = (currentPoint.x + height / 2.0f) * (float)height / height;
        int zIndex3 = ((currentPoint.z+0.5f) + width / 2.0f) * (float)width / width;
        int xIndex4 = ((currentPoint.x+0.5f) + height / 2.0f) * (float)height / height;
        int zIndex4 = ((currentPoint.z+0.5f) + width / 2.0f) * (float)width / width;

        if (xIndex < 0 || xIndex >= width - 1 || zIndex < 0 || zIndex >= height - 1) {
            // The current point is outside the terrain
            std::cout << " The current point is outside the terrain"  << std::endl;
            break;
        }

        // Get the heights of the four corners of the square
        float height1 = vertices[(xIndex * width + zIndex) * 8 + 1]; // 8 because each vertex has 8 components (vx, vy, vz, u, v, nx, ny, nz)
        float height2 = vertices[(xIndex2 * width + zIndex2) * 8 + 1];
        float height3 = vertices[(xIndex3 * width + zIndex3) * 8 + 1];
        float height4 = vertices[(xIndex4 * width + zIndex4) * 8 + 1];

        // Interpolate the height at the current point
        float heightAtCurrentPoint = (height1 + height2 + height3 + height4) / 4.0f;

        if (currentPoint.y <= heightAtCurrentPoint) {
            // We've hit the terrain
            currentPoint.y = heightAtCurrentPoint;
            return currentPoint;
        }

        if (t >= MAX_RAYCAST_DISTANCE) {
            // We've gone too far without hitting anything
            break;
        }
    }

    // No intersection found
    return glm::vec3(0.0f);
}

// Draw crosshair
void drawCrosshair() {
    // Define the vertices for the lines

    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;

    float lineLength = 0.05f; // Define the desired length of the lines (adjust as needed)

    float crosshairVertices[] = {
        // Horizontal line
        -lineLength * WINDOW_HEIGHT / 1500, 0.0f, 0.0f,  // start point
        lineLength* WINDOW_HEIGHT / 1500, 0.0f, 0.0f,   // end point
        // Vertical line
        0.0f, -lineLength * WINDOW_WIDTH / 1500  , 0.0f,  // start point
        0.0f, lineLength* WINDOW_WIDTH / 1500, 0.0f    // end point
    };


    // Create and bind a VAO (Vertex Array Object) and VBO (Vertex Buffer Object) for the crosshair
    unsigned int crosshairVAO, crosshairVBO;
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
    // Set the attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Use a simple shader for rendering the crosshair
    // You might need to adjust this shader according to your shader setup
    Shader crosshairShader("2dshader.vert", "2dshader.frag");
    crosshairShader.use();

    // Draw the crosshair
    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &crosshairVAO);
    glDeleteBuffers(1, &crosshairVBO);
}


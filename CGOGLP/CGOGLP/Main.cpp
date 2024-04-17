#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader_ogl.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <vector>

//Functions
void inputProcessor(GLFWwindow* window);
void mouse_movement(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


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



//Cube -> veranderen naar sphere voor zon effect
//As light source
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

// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3(10.0f,  18.0f,  -10.0f),
    glm::vec3(12.0f,  15.0f, -15.0f)
};

/*
Licht wordt nu overal opgesmete vanboven met directional light. Aangezien enkel zon nu er is cv.
Maar denk dat effectief meer licht bronnen toegevoegd moetten worden ik denk da enkel
voor die lichtbronnen een array met licht posities nodig is, niet voor de zon want directional.

*/
glm::vec3 lightPositions[] = {
    glm::vec3(10.0f,  30.0f,  -10.0f)
};



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

    Shader terrainShaders("TerrainShader.vert", "TerrainShader.frag");
    Shader objectShaders("ObjectShader.vert", "ObjectShader.frag");
    Shader lightningShaders("LightShader.vert", "LightShader.frag");

    // load height map texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("iceland_heightmap.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width <<" nr channels: "<< nrChannels << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }


    
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
    std::vector<float> vertices;
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
            vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
            vertices.push_back((int)y * yScale - yShift);   // vy
            vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
            
            // texture coordinates
            vertices.push_back(texCoords[2 * (i * width + j)]);   // u
            vertices.push_back(texCoords[2 * (i * width + j) + 1]); // v
        }
    }

  

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
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Configure texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    //shaders.use();

    //Texture
    int widthImg, heigthImg, numColCh;
    unsigned char* bytes = stbi_load("grassy2.jpg", &widthImg, &heigthImg, &numColCh, 0);
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


    objectShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
    lightningShaders.setVec4("lightColor", LIGHTCOLLOR_SUN);
    objectShaders.setVec3("lightPos", lightPositions[0]);


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
        
        glBindTexture(GL_TEXTURE_2D, texture);


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
        int vertexIndex = 5 * (normCamX * width + normCamZ); // 5 because each vertex has 5 components (vx, vy, vz, u, v)

        // Get the height of the terrain at the camera's position
        float terrainHeight = vertices[vertexIndex + 1]; // +1 because vy is the second component

        //std::cout << "Heigth? " << terrainHeight << std::endl;

        //--------------------------------------------------------------



        // camera/view transformation
        camera.SetCameraHeight(terrainHeight + PLAYER_HEIGHT);
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
        for (unsigned int i = 0; i < 1; i++)
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


        // render boxes
        glBindVertexArray(objectVAO);
        for (unsigned int i = 0; i < cubePositions->length(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objectShaders.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
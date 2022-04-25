#include "Input.h"
#include "Terrain.h"
#include "Time.h"
#include "Camera.h"
#include "Shader.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//--------------------

float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,//        7--------6
     1.0f, -1.0f,  1.0f,//       /|       /|
     1.0f, -1.0f, -1.0f,//      4--------5 |
    -1.0f, -1.0f, -1.0f,//      | |      | |
    -1.0f,  1.0f,  1.0f,//      | 3------|-2
     1.0f,  1.0f,  1.0f,//      |/       |/
     1.0f,  1.0f, -1.0f,//      0--------1
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Real Scene Rendering and Interaction", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    ///////////
    Time* time = new Time();

    Shader shader("Source/shader.vs", "Source/shader.fs");


    Camera cam = Camera(time);

    // shader
    const glm::vec4 white(1);
    const glm::vec4 black(0);
    const glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
    const glm::vec3 lightPos(64.0f, 64.0f, 10.0f);

    Input::getInstance().SetCamera(cam);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    int map_size = 128;

    Terrain NewTerrain(map_size + 1, 0.1, 0.01);

    NewTerrain.grassToRockPercentage = 0.6f;
    NewTerrain.rockToSnowPercentage = 0.8f;
    NewTerrain.mixingThreshold = 0.1f;
    NewTerrain.Generate(100, 0.6);

    NewTerrain.LoadTexture("Textures/grass.jpg", 0);
    NewTerrain.LoadTexture("Textures/rock.jpg", 1);
    NewTerrain.LoadTexture("Textures/snow.jpg", 2);

    shader.Enable();

    shader.setInt("OutTexture1", 0);
	shader.setInt("OutTexture2", 1);
	shader.setInt("OutTexture3", 2);

    // bind terrain to camera
    Input::getInstance().cam.setTerrain(&NewTerrain);

    // Generates Shader objects
    Shader skyboxShader("Source/skybox.vert", "Source/skybox.frag");

    skyboxShader.Enable();
    glUniform1i(glGetUniformLocation(skyboxShader.shaderProgram, "skybox"), 0);

    glEnable(GL_DEPTH_TEST);
    // Enables Cull Facing
    //glEnable(GL_CULL_FACE);
    // Keeps front faces
    glCullFace(GL_FRONT);
    // Uses counter clock-wise standard
    glFrontFace(GL_CCW);

    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    // Create VAO, VBO, and EBO for the skybox
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


    // All the faces of the cubemap (make sure they are in this exact order)
    std::string facesCubemap[6] =
    {
        parentDir + "/Real Scene Rendering and Interaction/Resources/skybox/right.jpg",
        parentDir + "/Real Scene Rendering and Interaction/Resources/skybox/left.jpg",
        parentDir + "/Real Scene Rendering and Interaction/Resources/skybox/top.jpg",
        parentDir + "/Real Scene Rendering and Interaction/Resources/skybox/bottom.jpg",
        parentDir + "/Real Scene Rendering and Interaction/Resources/skybox/front.jpg",
        parentDir + "/Real Scene Rendering and Interaction/Resources/skybox/back.jpg"
    };

    // Creates the cubemap texture object
    unsigned int cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // These are very important to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // This might help with seams on some systems
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Cycles through all the textures and attaches them to the cubemap object
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9, 0.1f, 100.0f);
        glm::mat4 view = Input::getInstance().cam.getPosition();
        glm::mat4 model = NewTerrain.m_Xmw;
        glm::mat4 lightModel = glm::rotate(model, glm::radians((float) glfwGetTime() * 90), glm::vec3(0, -1, 0)) * glm::translate(model, glm::vec3(90, 0, 0));

        // shader setup
        shader.Enable();
        shader.setVec3("viewPos", glm::value_ptr(cam.position));
        shader.setFloat("tileSize", (3 / (float)map_size));

        // light setup
        glm::vec3 lightColor = white;
        //lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
        //lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
        //lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
        glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        shader.setVec3("light.color", glm::value_ptr(glm::vec3(lightColor)));
        shader.setVec3("light.ambient", glm::value_ptr(glm::vec3(ambientColor)));
        shader.setVec3("light.diffuse", glm::value_ptr(glm::vec3(diffuseColor)));
        shader.setVec3("light.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        shader.setVec3("light.position", glm::value_ptr(glm::vec3(lightPos)));

        // material properties
        shader.setVec3("material.ambient", glm::value_ptr(ambient));
        shader.setVec3("material.diffuse", glm::value_ptr(white));
        shader.setVec3("material.specular", glm::value_ptr(white)); // specular lighting doesn't have full effect on this object's material
        shader.setFloat("material.shininess", 2.0f);


        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        /*
        shader.setVec4("LightPosW", glm::value_ptr(lightModel[3]));
        shader.setVec4("LightColor", glm::value_ptr(white));
        shader.setVec4("Ambient", glm::value_ptr(ambient));

        shader.setVec4("EyePosW", glm::value_ptr(glm::vec4(cam.position, 1)));

        shader.setVec4("MaterialEmissive", glm::value_ptr(black));
        shader.setVec4("MaterialDiffuse", glm::value_ptr(white));
        shader.setVec4("MaterialSpecular", glm::value_ptr(white));
        shader.setFloat("MaterialShininess", 50.0f);
        */

        NewTerrain.Render();

        // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
        glDepthFunc(GL_LEQUAL);

        skyboxShader.Enable();
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
        // The last row and column affect the translation of the skybox (which we don't want to affect)
        view = glm::mat4(glm::mat3(Input::getInstance().cam.getPosition()));
        projection = glm::perspective(glm::radians(45.0f), (float)1920 / 1081, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
        // where an object is present (a depth of 1.0f will always fail against any object's depth value)
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Switch back to the normal depth function
        glDepthFunc(GL_LESS);

        // update cam position everything frame to match terrain height
        Input::getInstance().cam.update();

        time->update();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"
#include "Renderer.h"
#include "Texture.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestBatching.h"

static void GLCLearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    int screenWidth = 640*2;
    int screenHeight = 480*2;

    window = glfwCreateWindow(screenWidth, screenHeight, "C++", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*glViewport(0.0f, 0.0f, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); */

    /*float positions[] = {
        -50, -50, 0.0f, 0.0f, 0.0f, 0,
        50, -50, 0.0f, 1.0f, 0.0f, 0,
        50,  50, 0.0f, 1.0f, 1.0f, 0,
       -50,  50, 0.0f, 0.0f, 1.0f, 0,

        100, 100, 0.0f, 0.0f, 0.0f, 0,
        300, 100, 0.0f, 1.0f, 0.0f, 0,
        300, 300, 0.0f, 1.0f, 1.0f, 0,
        100, 300, 0.0f, 0.0f, 1.0f, 0
    }; 

    struct Vertex {
        glm::vec3 Position[3];
        //float Color[4];
        glm::vec2 TexCoords[2];
        float TexID;
    };

   unsigned int indices[] = {
        0,1,2,
        2,3,0,

        4,5,6,
        6,7,4
    }; 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    VertexArray va;
    VertexBuffer vb;

    vb.Set(positions, sizeof(positions));

    //vb.Add(positions2, sizeof(positions2));
    


    // TODO: Maybe insted of (3) use offsetof(Vertex, Position)
    VertexBufferLayout layout;
    layout.Push<float>(3);
    //layout.Push<float>(4);
    layout.Push<float>(2);
    layout.Push<float>(1);
    va.AddBuffer(vb, layout); 

    IndexBuffer ib(12); 
    ib.Set(indices, 12);

    glm::mat4 proj = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f);
    glm::mat4 view =glm::translate(glm::mat4(1.0f), glm::vec3(00, 0, 0));


    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    GLCLearError();
    ///shader.SetUniform4f("u_Color", vector4(1.0f, 0.0f, 0.0f, 1.0f));
    GLCheckError();

    Texture texture("res/textures/text.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0); 
    Renderer renderer;
    */
    
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark;
   

    float translationX = 0;
    float translationY = 0;
    float translationZ = 0;

    glm::vec3 rotation(1.0f);

    float color[4] = {1,1,1,1};

    Model m;
    test::TestBatching test;

    while (!glfwWindowShouldClose(window))
    {
       // renderer.Clear();
        test.OnUpdate(0.0f);
        test.OnRender(glm::vec3(translationX, translationY, translationZ), rotation, color);
       


        ImGui_ImplGlfwGL3_NewFrame();
        test.OnImGuiRender();


       /*shader.Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), tanslationA);
            glm::mat4 mvp = proj * view * model;

            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
        }  */

        /*renderer.Draw(va, ib, shader);
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), tanslationB);
            glm::mat4 mvp = proj * view * model;

            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
        } */
        //GLCheckError();

        {

            static float f = 0.0f;
            ImGui::SliderFloat("Translation X", &translationX, -20, 20);
            ImGui::SliderFloat("Translation Y", &translationY, -20, 20);
            ImGui::SliderFloat("Translation Z", &translationZ, -100.0f, 0);
            ImGui::ColorEdit4("Color", color);

            ImGui::SliderFloat3("Rotation", &rotation.x, -360, 360);
            //ImGui::SliderFloat("Translation B", &tanslationB.x, 0.0f, screenWidth);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }




        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();


    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);

}

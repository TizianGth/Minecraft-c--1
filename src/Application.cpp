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

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestBatching.h"
#include "Application.h"

#include <chrono>

int ScreenWidth = 640 * 2;
int ScreenHeight = 480 * 2;

GLFWwindow* window;

test::TestBatching* test1 = nullptr;

float elapsed_time_ms;

auto currentTime = std::chrono::high_resolution_clock::now();
auto previousTime = std::chrono::high_resolution_clock::now();
void Draw() {
    if (test1 != nullptr) {
        currentTime = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<float, std::milli>(currentTime - previousTime).count();

        test1->OnUpdate(elapsed_time_ms);
        test1->OnRender(ScreenWidth, ScreenHeight);

        ImGui_ImplGlfwGL3_NewFrame();
        test1->OnImGuiRender();

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        previousTime = currentTime;
    }
    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;

    glViewport(0, 0, width, height);

    Draw();
}


int main(void)
{

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 2);
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "C++", NULL, NULL);
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
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    
    glEnable(GL_MULTISAMPLE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark;
   
    glfwSetWindowSizeCallback(window, OnWindowResize);

    test1 = new test::TestBatching;

    while (!glfwWindowShouldClose(window))
    {
        Draw();

        // Poll for and process events
        glfwPollEvents();

    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();


    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

GLFWwindow* Application::GetWindow()
{
    return window;
}

int Application::GetWindowWidth()
{
    return ScreenWidth;
}

int Application::GetWindowHeight()
{
    return ScreenHeight;
}

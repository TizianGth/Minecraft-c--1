#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Model.h"
#include "Shapes.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"
#include <string>
#include "DeltaTime.h"
#include "TextureAtlas.h"
#include "ChunkGenerator.h"

#include "Line.h"
#include "AABB.h"

// chunkAmount = Worlds half-sidelength in chunks => total chunks = (chunkAmount * 2) ^ 2
const int chunkAmount = 25;

class MainWindow {
public:
    GLFWwindow* m_Window = nullptr;
    int m_WindowWidth, m_WindowHeight;

    std::vector<Model> m_Models;
    Camera m_Camera;
    TextureAtlas m_Ta;
    ChunkGenerator m_ChunkGen = ChunkGenerator(chunkAmount);
    MainWindow(int width, int height);
    ~MainWindow();


private:
    void GlInit();
    void Init();

    void OnUpdate();
    void OnRender();

    void OnMouseUpdate();
    void OnKeyboardUpdate();

    bool isColiding(const glm::vec3& velocity);
    int Sign(int i);

    float m_FogOpacity = 0;
    float m_Fog1= 0.004f / ((1.0f/23.5f) * chunkAmount);
    float m_Fog2= 15.0f;

	bool m_Wireframe = false;
};

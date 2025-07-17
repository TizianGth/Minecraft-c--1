#include "MainWindow.h"

#include "Textures.h"
#include "Input.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

MainWindow::MainWindow(int width, int height)
{
    m_WindowWidth = width;
    m_WindowHeight = height;
    GlInit();
    Init();
}

MainWindow::~MainWindow()
{
}


void MainWindow::GlInit()
{
    if (!glfwInit())
        return;

    // Create a windowed mode window and its OpenGL context 
    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Minecraft c++", NULL, NULL);

    if (!m_Window)
    {
        glfwTerminate();
        return;
    }
    // Make the window's context current 
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }


    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(m_Window, true);
    ImGui::StyleColorsDark;

}

Shader* shader;

void MainWindow::Init()
{
    // setup camera
    m_Camera.SetFov(90, m_WindowWidth, m_WindowHeight);
    m_Camera.MoveTo(glm::vec3(0, 50, 0));

    m_ChunkGen.m_ShouldUpdate = true;
    m_ChunkGen.Update(m_Camera);

    // load shader
    shader = new Shader("/res/shaders/cube_basic.shader");
    shader->Bind();

    // Load and bind textures
    m_Ta.Init();
    m_Ta.Bind(0);


    // set shader uniforms
    glm::mat4 mvp = m_Camera.m_Proj * m_Camera.m_Mat4;
    shader->SetUniformMat4f("u_MVP", mvp);
    shader->SetUniform1i("u_Texture", 0);

    auto color = Renderer::Get().m_ClearColor;
    shader->SetUniform4f("u_FogColor", glm::vec4(color.x, color.y, color.z, 1));

    // m_ChunkGen.BindChunks();

    while (!glfwWindowShouldClose(m_Window))
    {
        Time::Start();

        OnUpdate();

        OnRender();

        glm::ivec3 playerBottomPos = glm::ivec3((int)m_Camera.m_Position.x, (int)m_Camera.m_Position.y - 2, (int)m_Camera.m_Position.z);
        glm::vec2 chunkPos = glm::vec2(std::floor(m_Camera.m_Position.x / CHUNK_SIZE), std::floor(m_Camera.m_Position.z / CHUNK_SIZE));

        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Position: x=%.1f, y=%.1f, z=%.1f", m_Camera.m_Position.x, m_Camera.m_Position.y, m_Camera.m_Position.z);
        ImGui::Text("Chunk position: x=%.1f, y=%.1f, z=%.1f", chunkPos.x, m_Camera.m_Position.y, chunkPos.y);
        ImGui::Text("Block: %.1i", m_ChunkGen.GetBlock(m_Camera.m_Position));

        ImGui::Text(std::to_string(Time::deltaTime).c_str());

        ImGui::SliderFloat("Fog opacity", &m_FogOpacity, 0.0f, 1.0f);
        ImGui::SliderFloat("Fog 1", &m_Fog1, 0.0f, 0.01f);
        ImGui::SliderFloat("Fog 2", &m_Fog2, 0.0f, 30.0f);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();

        glfwSwapBuffers(m_Window);

    }

    glfwTerminate();
}


void MainWindow::OnUpdate()
{
    m_ChunkGen.m_ShouldUpdate = true;

    OnMouseUpdate();
    OnKeyboardUpdate();

    m_ChunkGen.DeleteChunks();
    m_ChunkGen.BindChunks();
}

void MainWindow::OnRender()
{
    m_ChunkGen.m_Mutex.lock();
    shader->SetUniform1f("u_FogOpacity", m_FogOpacity);
    shader->SetUniform1f("u_Fog1", m_Fog1);
    shader->SetUniform1f("u_Fog2", m_Fog2);

    glm::mat4 projCam = m_Camera.m_Proj * m_Camera.m_Mat4;


    auto r = Renderer::Get();
    r.Clear();


    auto m = m_ChunkGen.m_AllChunks;
    for (const auto& it : m) {
            const auto chunk = it.second;

            if (chunk != nullptr && chunk->m_Generated && (chunk->m_Model != nullptr) && !chunk->m_Hide) {


                glm::mat4 mvp = projCam * chunk->m_ModelMat4;
                {
                    shader->Bind();
                    shader->SetUniformMat4f("u_MVP", mvp);
                    r.Draw(chunk->m_Model->m_Vao, chunk->m_Model->m_IndicesCount);
                }
            }
        
    }



    m_ChunkGen.m_Mutex.unlock();
}

int lastX, lastY;
double rotationX, rotationY;

void MainWindow::OnMouseUpdate()
{
    if (!m_MouseLocked) return;

    auto xy = Input::GetMousePosition(m_Window);
    std::pair<double, double> delta = { xy.first - lastX, xy.second - lastY };

    rotationX += delta.first * m_Camera.m_RotationSpeed * Time::deltaTime;
    rotationY += delta.second * m_Camera.m_RotationSpeed * Time::deltaTime;
    rotationY = std::clamp(rotationY, -89.0, 89.0);

    m_Camera.Rotate(glm::vec3(rotationX, rotationY, 0));

    int centerX = m_WindowWidth >> 1, centerY = m_WindowHeight >> 1;
    int maxRadius = glm::min(m_WindowWidth, m_WindowHeight) / 3;
    glm::vec2 d = glm::vec2(lastX, lastY) - glm::vec2(centerX, centerY);
    float len = glm::length(d);
    if (len > maxRadius) {
        glfwSetCursorPos(m_Window, (int)centerX, (int)centerY);
        lastX = centerX, lastY = centerY;
    }
    else {
        lastX = xy.first;
        lastY = xy.second;
    }

}




void MainWindow::OnKeyboardUpdate()
{
    
    if (Input::GetMouseDown(m_Window, GLFW_MOUSE_BUTTON_LEFT)) {
        std::cout << "Clicked" << std::endl;
        glm::ivec2 chunkPos(0, 0);

        Chunk* chunk = m_ChunkGen.m_AllChunks[chunkPos];

        chunk->Changeblock(0, 29, 0, 1);
        chunk->Changeblock(0, 30, 0, 1);
        chunk->Changeblock(0, 31, 0, 1);
        chunk->Changeblock(0, 32, 0, 1);


        m_ChunkGen.m_GenChunks[chunkPos] = chunk;
        m_ChunkGen.m_StartUp = true;
    }
    
    

    if (Input::GetKeyDown(m_Window, GLFW_KEY_L)) {
        m_MouseLocked = !m_MouseLocked;

        if (!m_MouseLocked) {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    if (Input::GetKeyPressed(m_Window, GLFW_KEY_F)) {
        m_Camera.SetFov(20, m_WindowWidth, m_WindowHeight);
    }
    else {
        m_Camera.SetFov(90, m_WindowWidth, m_WindowHeight);
    }


    glm::vec3 velocity(0.0f);
    // Basic Movement
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_W)) {
        velocity += m_Camera.m_Direction * m_Camera.m_MovementSpeed;
    }
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_S)) {
        velocity += m_Camera.m_Direction * -m_Camera.m_MovementSpeed;
    }
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_A)) {
        velocity += m_Camera.m_Right * -m_Camera.m_MovementSpeed;
    }
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_D)) {
        velocity += m_Camera.m_Right * m_Camera.m_MovementSpeed;
    }

    // Flying: Up/Down
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_SPACE)) {
        velocity.y += m_Camera.m_MovementSpeed;
    }
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_LEFT_SHIFT)) {
        velocity.y -= m_Camera.m_MovementSpeed;
    }

    // Running
    if (Input::GetKeyPressed(m_Window, GLFW_KEY_LEFT_CONTROL)) {
        velocity *= m_Camera.m_MovementSpeedMaxMulti;
    }

    //m_Camera.MoveBy(velocity * Time::deltaTime);

    //

    //isColiding((velocity * glm::vec3(1, 0, 1)) * Time::deltaTime);
    //isColiding(velocity * Time::deltaTime * glm::vec3(1, 0, 0));
    //isColiding(velocity * Time::deltaTime * glm::vec3(0, 0, 1));

	std::cout << "Velocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
    m_Camera.MoveBy(velocity * Time::deltaTime);



}

//AABB b(glm::vec3(0, 28, 0), glm::vec3(1, 1, 1));
bool MainWindow::isColiding(const glm::vec3& velocity)
{
    glm::vec3 posBefore = m_Camera.m_Position;
    m_Camera.MoveBy(velocity);

    AABB c(glm::vec3(m_Camera.m_Position.x - 0.5f, m_Camera.m_Position.y - 1.5, m_Camera.m_Position.z - 0.5f),glm::vec3(1, 2, 1));

    for (int y = std::round(c.m_Position.y - 1); y <= std::round(c.m_Position.y + 2); y++) {
        for (int x = std::round(m_Camera.m_Position.x - c.m_Size.x); x <= std::round(m_Camera.m_Position.x + c.m_Size.x); x++) {
            for (int z = std::round(m_Camera.m_Position.z - c.m_Size.z); z <= std::round(m_Camera.m_Position.z + c.m_Size.z); z++) {

                glm::vec3 position(x, y, z);
                Settings::BlockTypes block = m_ChunkGen.GetBlock(position);
                if (block > 0) {
                    AABB b(position, glm::vec3(1, 1, 1));
                    b.m_Position = position;
                    if (c.checkCollision(b)) {
                        c.resolveCollision(b);
                    }
                }

            }
        }
    }
    m_Camera.MoveTo(glm::vec3(c.m_Position.x + 0.5f, c.m_Position.y + 1.5, c.m_Position.z + 0.5f));
    return true;
}

int MainWindow::Sign(int i)
{
    return (i > 0) - (i < 0);
}





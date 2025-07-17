#include "Camera.h"

Camera::Camera(int width, int height, GLFWwindow* window)
{
	m_WindowHeight = height;
	m_WindowWidth = width;
	m_Window = window;

	SetFov(m_Fov);

	m_Collider.m_Position = m_Position - glm::vec3(0.5f, m_CameraHeight, 0.5f);
	m_Collider.m_Size = glm::vec3(1.0f, m_ColliderHeight, 1.0f);

	m_IsCollider = true;
}

void Camera::SetFov(unsigned int fov)
{
	m_Fov = fov;
	m_AspectRatio = (float)m_WindowWidth / (float)m_WindowHeight;
	m_Proj = glm::perspective(glm::radians((float)fov), m_AspectRatio, m_NearPlane, m_FarPlane);
}

void Camera::UpdateMat4()
{
	m_Mat4 = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
}

void Camera::MoveByCollider(glm::vec3 velocity, BoxCollider& collider)
{
}

void Camera::OnMouseMove()
{
	if (!m_MouseLocked) return;

	auto xy = Input::GetMousePosition(m_Window);
	double deltaX = xy.first - m_LastX;
	double deltaY = xy.second - m_LastY;

	m_RotationX += deltaX * m_RotationSpeed * Time::deltaTime;
	m_RotationY += deltaY * m_RotationSpeed * Time::deltaTime;
	m_RotationY = std::clamp(m_RotationY, -89.0, 89.0);

	Rotate(glm::vec3(m_RotationX, m_RotationY, 0));

	int centerX = m_WindowWidth / 2, centerY = m_WindowHeight / 2;
	int maxRadius = std::min(m_WindowWidth, m_WindowHeight) / 3;
	glm::vec2 d = glm::vec2(m_LastX - centerX, m_LastY - centerY);
	float len = glm::length(d);
	if (len > maxRadius) {
		glfwSetCursorPos(m_Window, (int)centerX, (int)centerY);
		m_LastX = centerX, m_LastY = centerY;
	}
	else {
		m_LastX = xy.first;
		m_LastY = xy.second;
	}
}

void Camera::OnKeyboardMove()
{
	glm::vec3 velocity(0.0f);
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_W)) {
		velocity += m_Direction;
	}
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_S)) {
		velocity += -m_Direction;
	}
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_A)) {
		velocity += -m_Right;
	}
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_D)) {
		velocity += m_Right;
	}

	if(velocity.x + velocity.y + velocity.z != 0)
		velocity = glm::normalize(velocity);

	velocity *= m_MovementSpeed;

	// Flying: Up/Down
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_SPACE)) {
		velocity.y += m_MovementSpeed;
	}
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_LEFT_SHIFT)) {
		velocity.y -= m_MovementSpeed;
	}

	// Running
	if (Input::GetKeyPressed(m_Window, GLFW_KEY_LEFT_CONTROL)) {
		velocity *= m_MovementSpeedMaxMulti;
	}
	MoveBy(velocity * Time::deltaTime);
}

void Camera::OnZoom()
{

	if (Input::GetKeyPressed(m_Window, GLFW_KEY_F)) {
		ZoomIn();
	}
	else {
		ZoomOut();
	}
}

void Camera::ZoomIn()
{
	SetFov(20);
}

void Camera::ZoomOut()
{
	SetFov(90);
}

void Camera::OnMouseLock()
{
	if (Input::GetKeyDown(m_Window, GLFW_KEY_L)) {
		m_MouseLocked = !m_MouseLocked;

		if (!m_MouseLocked) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

void Camera::SetWindow(GLFWwindow* window)
{
	m_Window = window;
}

bool Camera::IsInFrustum(const AABB& aabb)
{
	for (int i = 0; i < 6; ++i) {
		glm::vec3 normal = glm::vec3(m_Planes[i]);
		float d = m_Planes[i].w;

		// Compute the positive vertex (closest to the plane normal direction)
		glm::vec3 positiveVertex = aabb.m_Min;

		if (normal.x >= 0) positiveVertex.x = aabb.m_Max.x;
		if (normal.y >= 0) positiveVertex.y = aabb.m_Max.y;
		if (normal.z >= 0) positiveVertex.z = aabb.m_Max.z;

		float distance = glm::dot(normal, positiveVertex) + d;

		if (distance < 0) {
			// AABB is fully outside this plane
			return false;
		}
	}
	return true; // AABB intersects or is inside all Planes
}

void Camera::SetPlanes(const glm::mat4& m)
{
	//glm::mat4 m = m_Proj * m_Mat4;
	// Left
	m_Planes[0] = glm::vec4(
		m[0][3] + m[0][0],
		m[1][3] + m[1][0],
		m[2][3] + m[2][0],
		m[3][3] + m[3][0]
	);

	// Right
	m_Planes[1] = glm::vec4(
		m[0][3] - m[0][0],
		m[1][3] - m[1][0],
		m[2][3] - m[2][0],
		m[3][3] - m[3][0]
	);

	// Bottom
	m_Planes[2] = glm::vec4(
		m[0][3] + m[0][1],
		m[1][3] + m[1][1],
		m[2][3] + m[2][1],
		m[3][3] + m[3][1]
	);

	// Top
	m_Planes[3] = glm::vec4(
		m[0][3] - m[0][1],
		m[1][3] - m[1][1],
		m[2][3] - m[2][1],
		m[3][3] - m[3][1]
	);

	// Near
	m_Planes[4] = glm::vec4(
		m[0][3] + m[0][2],
		m[1][3] + m[1][2],
		m[2][3] + m[2][2],
		m[3][3] + m[3][2]
	);

	// Far
	m_Planes[5] = glm::vec4(
		m[0][3] - m[0][2],
		m[1][3] - m[1][2],
		m[2][3] - m[2][2],
		m[3][3] - m[3][2]
	);

	// Optional: Normalize all Planes
	for (int i = 0; i < 6; i++) {
		float length = glm::length(glm::vec3(m_Planes[i]));
		m_Planes[i] /= length;
	}
}


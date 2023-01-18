#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_Color;
void main() {
	color = vec4(u_Color.x, u_Color.y, u_Color.z, 1.0f);
};
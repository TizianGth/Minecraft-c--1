#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 texCoord;

out vec3 v_TexCoord;

uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;

uniform sampler2DArray u_Textures;

void main() {
	int index = 1;

	if (v_TexCoord.z == -1) {
		index = 0;
	}
	else if (v_TexCoord.z == 1) {
		index = 5;
	}

	vec4 texColor = texture(u_Textures, vec3(v_TexCoord.x, v_TexCoord.y, 2));
	color = texColor;
};
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 texCoord;
layout(location = 2) in float materialID;

out vec3 v_TexCoord;
out float v_MaterialID;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_MaterialID = materialID;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;
in float v_MaterialID;

uniform samplerCube u_Texture;
uniform samplerCube u_Overlay;
uniform vec4 u_Color;

void main() {

	vec4 layer = texture(u_Texture, v_TexCoord);
	vec4 overlay = texture(u_Overlay, v_TexCoord) * u_Color;
	int id = int(round(v_MaterialID));
	if (id == 2) {
		color = mix(layer, overlay, overlay.a);
	}
	else if (id == 1) {
		color = layer;
	}
	else if(id == 0) {
		color = layer;
	}
};
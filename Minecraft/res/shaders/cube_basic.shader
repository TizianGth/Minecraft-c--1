#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 texCoord;
layout(location = 2) in float materialID;

out vec3 v_TexCoord;
out float v_MaterialID;
out float v_Fog;

uniform mat4 u_MVP;
uniform float u_FogOpacity;
uniform float u_Fog1;
uniform float u_Fog2;

void main() {
	gl_Position = u_MVP * position;

	v_TexCoord = texCoord;
	v_MaterialID = materialID;

	v_Fog = exp(-pow((length(gl_Position.xyz) * u_Fog1), -u_Fog2));
	v_Fog = clamp(v_Fog * u_FogOpacity, 0, 1);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;
in float v_MaterialID;
in float v_Fog;

uniform sampler2DArray u_Texture;

uniform vec4 u_Color;
uniform vec4 u_FogColor;

void main() {
	int id = int(round(v_MaterialID));

	if (id == 1) {
		vec4 layer = texture(u_Texture, vec3(v_TexCoord.x, v_TexCoord.y, 3));
		color = layer;
	}
	else if (id == 2) {
		vec4 layer = texture(u_Texture, vec3(v_TexCoord.x, v_TexCoord.y, 0));
		color = layer;
	}
	else if (id == 3) {
		int d = 1;
		if (round(v_TexCoord.z) == 2 || round(v_TexCoord.z) == 3) {
			d = 2;
		}
		vec4 layer = texture(u_Texture, vec3(v_TexCoord.x, v_TexCoord.y, d));
		color = layer;
	}
	else if (id == 4) {
		vec4 layer = texture(u_Texture, vec3(v_TexCoord.x, v_TexCoord.y, 5));
		color = layer;
	}

	//if (id == 1) { // Grass
	//	vec4 overlay = texture(u_Overlay, v_TexCoord) * u_Color;
	//	color = mix(layer, overlay, overlay.a);
	//}
	//else {
	//	color = layer;
	//}
	//color = layer;
	color = mix(color, u_FogColor, v_Fog);
	//color = mix(color, u_FogColor, v_Fog);
};
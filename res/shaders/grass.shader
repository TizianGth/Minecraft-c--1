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

void main() {
	gl_Position = u_MVP * position;

	v_TexCoord = texCoord;
	v_MaterialID = materialID;

	v_Fog = exp(-pow((length(gl_Position.xyz)*0.02f), -4.5f));
	v_Fog = clamp(v_Fog * u_FogOpacity, 0, 1);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;
in float v_MaterialID;
in float v_Fog;

uniform samplerCube u_Texture;
uniform samplerCube u_Overlay;
uniform vec4 u_Color;
uniform vec4 u_FogColor;

void main() {

	vec4 layer = texture(u_Texture, v_TexCoord);
	int id = int(round(v_MaterialID));
	if (id == 2) { // Grass
		vec4 overlay = texture(u_Overlay, v_TexCoord) * u_Color;
		color = mix(layer, overlay, overlay.a);
	}
	else {
		color = layer;
	}

	//color = mix(color, vec4(0.639f,0.776f,1,1), v_Fog);
	color = mix(color, u_FogColor, v_Fog);
};
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 texCoord;
layout(location = 2) in float materialID;

out vec3 v_TexCoord;
out float v_MaterialID;
out float v_Fog;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;

	v_TexCoord = texCoord;
	v_MaterialID = materialID;

	v_Fog = exp(-pow((length(gl_Position.xyz)*0.01f), 5.5f));
	v_Fog = clamp(v_Fog, 0, 1);
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

void main() {

	vec4 layer = texture(u_Texture, v_TexCoord);
	int id = int(round(v_MaterialID));
	if (id == 2) {
		vec4 overlay = texture(u_Overlay, v_TexCoord) * u_Color;
		color = mix(layer, overlay, overlay.a);
	}
	else if (id == 1) {
		color = layer;
	}
	else if(id == 0) {
		color = layer;
	}

	color = mix(vec4(0.639f,0.776f,1,1), color, v_Fog);
};
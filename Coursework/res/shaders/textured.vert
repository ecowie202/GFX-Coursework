#version 330

// Inputs
layout (location = 0) in vec3 v_Position;
layout (location = 3) in vec2 v_TexCoord;

// Outputs
out vec2 f_TexCoord;

// Uniforms
uniform mat4 modelViewProjection;

void main()
{
	// Apply model-view-projection matrix to the vertex position
	gl_Position = modelViewProjection * vec4(v_Position, 1);

	// Pass the vertex attributes to the fragment shader
	f_TexCoord = v_TexCoord;
}

#version 330

#define MAX_DIR_LIGHTS   1
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS  16

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	float linear;
	float quadratic;
	float constant;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// Inputs
in vec3 f_Position;
in vec3 f_Normal;
in vec2 f_TexCoord;

// Outputs
out vec4 p_Color;

// Uniform
uniform mat4 view;
uniform mat4 normal;
uniform Material material;
uniform vec3 ambientLight;
uniform DirectionalLight directionalLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numDirectionalLights;
uniform int numPointLights;
uniform int numSpotLights;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(-vec3(normal * vec4(light.direction, 1)));
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0);
	vec3 specular = light.specular * pow(max(dot(viewDir, reflectDir), 0), material.shininess);

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec3 viewDir)
{
	vec3 lightPos = vec3(view * vec4(light.position, 1));
	vec3 lightDir = normalize(lightPos - f_Position);
	vec3 reflectDir = reflect(-lightDir, norm);

	float dist = length(lightPos - f_Position);
	float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient = light.ambient * attenuation;
	vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0) * attenuation;
	vec3 specular = light.specular * pow(max(dot(viewDir, reflectDir), 0), material.shininess) * attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir)
{
	vec3 lightPos = vec3(view * vec4(light.position, 1));
	vec3 lightDir = normalize(lightPos - f_Position);
	vec3 reflectDir = reflect(-lightDir, norm);

	float dist = length(lightPos - f_Position);
	float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));  

	float theta = dot(lightDir, normalize(-vec3(normal * vec4(light.direction, 1)))); 
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0, 1);

	vec3 ambient = light.ambient * attenuation * intensity;
	vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0) * attenuation * intensity;
	vec3 specular = light.specular * pow(max(dot(viewDir, reflectDir), 0), material.shininess) * attenuation * intensity;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(f_Normal);
	vec3 viewDir = normalize(-f_Position);

	vec3 result = ambientLight;

	for (int i = 0; i < numDirectionalLights; i++)
		result += calculateDirectionalLight(directionalLights[i], norm, viewDir);

	for (int i = 0; i < numPointLights; i++)
		result += calculatePointLight(pointLights[i], norm, viewDir);

	for (int i = 0; i < numSpotLights; i++)
		result += calculateSpotLight(spotLights[i], norm, viewDir);

	// Set the fragment colour
	vec4 baseColour = texture(material.diffuse, f_TexCoord);
	float intensity = (result.x + result.y + result.z) / 3;
	if      (intensity > 0.95) p_Color = vec4(1.0, 1.0, 1.0, 1) * vec4(result, 1) * baseColour;
	else if (intensity > 0.5)  p_Color = vec4(0.6, 0.6, 0.6, 1) * vec4(result, 1) * baseColour;
	else if (intensity > 0.25) p_Color = vec4(0.3, 0.3, 0.3, 1) * vec4(result, 1) * baseColour;
	else                       p_Color = vec4(0.2, 0.2, 0.2, 1) * vec4(result, 1) * baseColour;
}

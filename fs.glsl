#version 330 core

out vec4 FragColor;

in vec3 color;
in vec3 normal;
in vec3 position;

uniform float time = 1.0f;
float near = 0.1f;
float far = 100.0f;
uniform float worldWidth;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 cameraDirection;


float ambient = 0.01f;
float spectralStrength = 0.5f;

float linearizedDepth(float depth){
    float zR = depth / far + (1 - depth) / near;
    float z = 1/zR;
    return (z - near) / (far - near);
}
float logDepth(float depth, float steepness, float offset){
    float lD = linearizedDepth(depth);
    return 1/(1+exp(-steepness * (lD - offset)));
}
void main()
{
    vec3 lightDirection = normalize(lightPos - position);
    vec3 norm = normalize(normal);
    vec3 reflectDir = reflect(-lightDirection, norm);
    vec3 viewDir = normalize(-cameraDirection);
    float spectral = spectralStrength * pow(abs(dot(reflectDir, viewDir)), 32);
    

    float fog = 1 - logDepth(gl_FragCoord.z, 500.0f, 0.0215);
    float diffuse = abs(dot(norm, lightDirection));
    FragColor =  vec4(color * lightCol * (diffuse + ambient + spectral) * fog, 1.0);
}
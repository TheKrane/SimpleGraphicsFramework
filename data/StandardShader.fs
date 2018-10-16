#version 330

#define MAX_LIGHTS 8

struct Light
{
    int Type;
    vec3 Position;
    vec3 Rotation;
    vec3 Intensity;
    float Ambient;
};

uniform mat4 ModelMatrix;
uniform vec3 CameraPosition;
uniform vec4 Color;
uniform sampler2D DiffuseTexture;
uniform float Ambient;
uniform float Diffuse;
uniform float Specular;

uniform Light Lights[MAX_LIGHTS];

in vec3 FragPosition;
in vec2 FragUV;
in vec3 FragNormal;

out vec4 FragColor;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    vec3 normal = normalMatrix * normalize(FragNormal);
    vec3 fragPosition = vec3(ModelMatrix * vec4(FragPosition, 1));
    vec3 cameraDirection = normalize(CameraPosition - fragPosition);

    vec3 resultColor = vec3(0,0,0);
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        if(Lights[i].Type == 0)
        {
            vec3 lightVector = Lights[i].Position - fragPosition;
            vec3 lightDirection = normalize(lightVector);
            vec3 reflectDirection = reflect(-lightDirection, normal);

            float ambient = Lights[i].Ambient * Ambient;
            float diffuse = clamp(dot(normal, lightDirection), 0, 1) * Diffuse;
            float specular = pow(max(dot(cameraDirection, reflectDirection), 0.0f), 32) * Specular;
            float attenuation = 1.0f / (1.0f + 0.02f * pow(length(lightVector), 2));

            resultColor += vec3(Color.rgb * Lights[i].Intensity * (attenuation * (ambient + diffuse + specular)));
        }
        else
        {
            float lightX = radians(Lights[i].Rotation.x);
            float lightY = radians(Lights[i].Rotation.y); 
            vec3 lightDirection = vec3(cos(lightY)*cos(lightX), sin(lightY)*cos(lightX), sin(lightX));
            vec3 reflectDirection = reflect(-lightDirection, normal);

            float ambient = Lights[i].Ambient * Ambient;
            float diffuse = clamp(dot(normal, lightDirection), 0, 1) * Diffuse;
            float specular = pow(max(dot(cameraDirection, reflectDirection), 0.0), 32) * Specular;

            resultColor += vec3(Color.rgb * Lights[i].Intensity * (ambient + diffuse + specular));
        }
    }
    
    FragColor = vec4(texture(DiffuseTexture, FragUV).rgb * resultColor, Color.w);
}

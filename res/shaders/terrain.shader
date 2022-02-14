#shader vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_Normal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 u_CameraMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_Transform;

void main()
{
    // multiplication of the texture coords for tiling the texture
    // many times accross the whole plane
    vs_out.FragPos = vec3(u_ModelMatrix * vec4(a_Pos, 1.0));
    vs_out.Normal = (u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;
    vs_out.TexCoords = a_UV * 50;
	gl_Position = u_CameraMatrix * vec4(u_Transform * u_ModelMatrix * vec4(a_Pos, 1.0f));
}

#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform int u_HasTexture;
uniform sampler2D u_Texture;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform float u_LightStrength;
uniform vec3 u_LightColor;

void main()
{
    vec3 normal = normalize(fs_in.Normal);

    // Compute ambient lighting
    vec3 ambient = u_LightStrength * u_LightColor;

    // Compute light direction
    vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);

    // Compute diffuse lighting
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Compute view direction
    vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);

    // Compute reflectionDirection
    vec3 reflectDir = reflect(-lightDir, normal);

    // Compute specular lighting
    float spec = 0.0;
    float specStrength = 0.1;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);
    vec3 specular = specStrength * spec * u_LightColor; 
    
    vec3 color = vec3(0);

    if(u_HasTexture == 1){
        color = texture(u_Texture, fs_in.TexCoords).rgb;
    }
    else{
        color = vec3(0.5, 0.5, 0.5);
    }

    vec3 lighting = vec3(0.0f, 0.0f, 0.0f);
    lighting = (ambient + diffuse + specular) * color;
    
    FragColor = vec4(lighting, 1.0);
}
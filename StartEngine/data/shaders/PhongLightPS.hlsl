struct PointLight
{
    float3 position;
    float3 color;
    float3 attenuation;
    float intensity;
    float range;
};

struct DirectionalLight
{
    float3 direction;
    float3 color;
    float intensity;
};

struct AmbientLight
{
    float3 color;
    float intensity;
};

struct MaterialParameters
{
    // Color components
    float3 diffuseColor;
    float3 specularColor;
    float3 emissiveColor;
    float opacity;
    
    // Vertex shader output
    float4 position;
    float4 worldPosition;
    float3 normal;
 
    // TODO: add normal maps
    // TODO: implement PBR
};

struct VS_Output
{
    float4 position      : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float3 cameraPos     : CAMERA_POSITION;
    float3 normal        : NORMAL;
    float2 texCoord      : TEXCOORD;
};

cbuffer LightCB : register(b2)
{
    PointLight pointLight;
    DirectionalLight directionalLight;
    AmbientLight ambientLight;
};

Texture2D modelTextureDiffuse           : register(t0);
SamplerState modelSamplerStateDiffuse   : register(s0);

Texture2D modelTextureSpecular          : register(t1);
SamplerState modelSamplerStateSpecular  : register(s1);

Texture2D modelTextureEmissive          : register(t2);
SamplerState modelSamplerStateEmissive  : register(s2);

float4 GetUnlit(VS_Output input)
{
    return modelTextureSpecular.Sample(modelSamplerStateSpecular, input.texCoord);
    return modelTextureDiffuse.Sample(modelSamplerStateDiffuse, input.texCoord);
}

MaterialParameters GetMaterialParameters(VS_Output input)
{
    MaterialParameters materialParameters;
    
    // Color
    float4 color = modelTextureDiffuse.Sample(modelSamplerStateDiffuse, input.texCoord);
    //float4 color = modelTextureSpecular.Sample(modelSamplerStateSpecular, input.texCoord);
    materialParameters.diffuseColor = float3(1.0, 1.0, 0.94);
    //materialParameters.diffuseColor = color.rgb;
    materialParameters.opacity = color.a; 
    materialParameters.specularColor = float3(1.0, 1.0, 1.0); //(float3) modelTextureSpecular.Sample(modelSamplerStateSpecular, input.texCoord);
    materialParameters.emissiveColor = (float3)modelTextureEmissive.Sample(modelSamplerStateEmissive, input.texCoord);
    
    // Vertex shader output
    materialParameters.normal = input.normal;
    materialParameters.position = input.position;
    materialParameters.worldPosition = input.worldPosition;
    
    return materialParameters;
}

float4 gamma(float4 input)
{
    return float4(pow(input.rgb, 1.0 / 2.2), input.a);
}

float CalculateBrightness(PointLight pointLight, float dist)
{
    if (dist > pointLight.range)
        return 0.0;
    
    float3 att = pointLight.attenuation;
    float fadeCoefficient = 1.0 / (att.x + att.y * dist + att.z * (dist * dist));
    
    return fadeCoefficient * pointLight.intensity;
}

float3 CalculateDiffuseColor(MaterialParameters material, PointLight pointLight, DirectionalLight directionalLight, AmbientLight ambientLight)
{
    // Point light
    float3 lightDirection = pointLight.position - (float3)material.worldPosition;
    float distanceToPointLight = length(lightDirection);
    lightDirection /= distanceToPointLight;
    
    float3 pointLightComponent = dot(material.normal, lightDirection);
    pointLightComponent *= CalculateBrightness(pointLight, distanceToPointLight);
    
    float3 result = saturate(pointLightComponent) * material.diffuseColor * pointLight.color;
    
    // Directional light
    lightDirection = normalize(directionalLight.direction);
    result += saturate(max(0.0f, dot(material.normal, -lightDirection)) * directionalLight.intensity) * material.diffuseColor * directionalLight.color;
    
    // Ambient light
    //result += material.diffuseColor * ambientLight.color * ambientLight.intensity;
    
    return result;
}

// TODO: should be calculated differently (as specular, not as diffuse)
float3 CalculateSpecularColor(MaterialParameters material, PointLight pointLight, DirectionalLight directionalLight, float3 cameraPos)
{
    // Point light specular
    float3 lightDir = pointLight.position - material.worldPosition.xyz;
    float distToLight = length(lightDir);
    lightDir /= distToLight;
    float3 reflectedLightDir = reflect(-lightDir, material.normal); // ok
    float3 toCameraDir = normalize(cameraPos - material.worldPosition.rgb);
    
    float reflectedToCameraDot = max(dot(reflectedLightDir, toCameraDir), 0.0);
    float lightPower = pow(reflectedToCameraDot, 8.0) * CalculateBrightness(pointLight, distToLight);
    
    float3 result = saturate(lightPower) * material.specularColor * pointLight.color;
    
    // Directional light specular
    lightDir = normalize(directionalLight.direction);
    reflectedLightDir = reflect(lightDir, material.normal);
    reflectedToCameraDot = max(dot(reflectedLightDir, toCameraDir), 0.0);
    lightPower = pow(reflectedToCameraDot, 16.0);
    
    result += saturate(lightPower) * material.specularColor * directionalLight.color;
    
    return result;
}

float3 CalculateEmissiveColor(MaterialParameters material)
{
    return saturate(material.emissiveColor);
}

float4 main(VS_Output input) : SV_TARGET
{
 
    MaterialParameters materialParameters = GetMaterialParameters(input);
    
    clip(materialParameters.opacity - .01);
    
    float4 outColor = float4(0.0, 0.0, 0.0, materialParameters.opacity);

    outColor.rgb += CalculateDiffuseColor(materialParameters, pointLight, directionalLight, ambientLight);
    outColor.rgb += CalculateSpecularColor(materialParameters, pointLight, directionalLight, input.cameraPos);
    outColor.rgb += CalculateEmissiveColor(materialParameters);
    
    outColor = gamma(outColor);
    
    
    //return GetUnlit(input);
    return outColor;
}

#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosDecalSpace;
} fs_in;

uniform sampler2D decalTexture;
uniform sampler2D depthMap;

uniform vec3 decalPosition;
uniform vec3 cameraPosition;

float BIAS = 0.005f;

float DecalCalculation(vec4 fragPosDecalSpace)
{
    vec3 projCoords = fragPosDecalSpace.xyz / fragPosDecalSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 decalDir = normalize(decalPosition - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, decalDir)), BIAS);

    // PCF
    float decalFactor = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            decalFactor += currentDepth - bias <= pcfDepth  ? 1.0 - pcfDepth : 0.0; 
        }    
    }
    
    decalFactor = clamp(decalFactor, 0.0, 1.0);

    // keep the decalFactor at 0.0 when outside the far_plane region of the light's frustum.
    if(currentDepth > 1.0)
        decalFactor = 0.0;
        
    return decalFactor;
}

void main()
{
  float epsilon = 0.0001;
//  if (fs_in.FragPosDecalSpace.w < epsilon)
//  {
//      FragColor = vec4(1.0, 1.0, 0.0, 1.0);
//      return;
//  }
  
  float decalFactor = DecalCalculation(fs_in.FragPosDecalSpace);

  if (decalFactor > BIAS)
  {
      vec3 projCoords = fs_in.FragPosDecalSpace.xyz / fs_in.FragPosDecalSpace.w;
      projCoords = projCoords * 0.5 + 0.5;
      FragColor = texture(decalTexture, projCoords.xy);
  }
  else
  {
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
      discard;
  }
}
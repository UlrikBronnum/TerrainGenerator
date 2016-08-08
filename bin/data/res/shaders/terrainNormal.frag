// fragment shader
#version 150


uniform vec4            tiling = vec4(5,5,0,0);

// this is how we receive the texture
uniform sampler2D       texColor0;
uniform sampler2D       texNormal0;

uniform sampler2D       texColor1;
uniform sampler2D       texNormal1;

uniform sampler2D       texColor2;
uniform sampler2D       texNormal2;

uniform sampler2D       texColor3;
uniform sampler2D       texNormal3;

uniform sampler2D       texColorWater;

uniform vec3            ambientLight = vec3(0.7);

uniform float           nearClip;
uniform float           farClip;


in vec4                 light;
in vec4                 vPosition;
in mat3                 tbnMat;
in vec2                 varyingtexcoord;

in vec3                 vNormal;

out vec4                outputColor;

struct textureInfo
{
    vec3 texColor;
    vec3 texNormal;
};

textureInfo getTextureInfo()
{
    float theDot = dot(normalize(vNormal),vec3(0.0,0.0,1.0));

    textureInfo texInfo;

    if(theDot < 0.60)
    {
        texInfo.texColor = texture(texColor3,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        texInfo.texNormal = normalize(tbnMat * (255.0/128.0 * texture(texNormal3,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
    }
    if(theDot >= 0.59 && theDot <= 0.65 )
    {
        float mixAmount = clamp((theDot - 0.6) / 0.05, 0.0, 1.0);
        vec3 c1 = texture(texColor3,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        vec3 c2 = texture(texColor2,varyingtexcoord * tiling.xy + tiling.zw).rgb;

        vec3 n1 = normalize(tbnMat * (255.0/128.0 * texture(texNormal3,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
        vec3 n2 = normalize(tbnMat * (255.0/128.0 * texture(texNormal2,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);

        texInfo.texColor = mix(c1 , c2 , mixAmount);//* (1.0 - mixAmout) + c2 * mixAmout;
        texInfo.texNormal = normalize(n1 * (1.0 - mixAmount) + n2 * mixAmount);
    }
    if(theDot > 0.64 && theDot < 0.70 )
    {
        texInfo.texColor = texture(texColor2,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        texInfo.texNormal = normalize(tbnMat * (255.0/128.0 * texture(texNormal2,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
    }
    if(theDot >= 0.69 && theDot <= 0.75 )
    {
        float mixAmount = clamp((theDot - 0.7) / 0.05,0.0,1.0);
        vec3 c1 = texture(texColor2,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        vec3 c2 = texture(texColor1,varyingtexcoord * tiling.xy + tiling.zw).rgb;

        vec3 n1 = normalize(tbnMat * (255.0/128.0 * texture(texNormal2,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
        vec3 n2 = normalize(tbnMat * (255.0/128.0 * texture(texNormal1,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);

        texInfo.texColor = mix(c1 , c2 , mixAmount);
        texInfo.texNormal = normalize(n1 * (1.0 - mixAmount) + n2 * mixAmount);
    }
    if(theDot > 0.74 && theDot < 0.90)
    {
        texInfo.texColor = texture(texColor1,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        texInfo.texNormal = normalize(tbnMat * (255.0/128.0 * texture(texNormal1,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
    }
    if(theDot >= 0.89 && theDot <= 0.95 )
    {
        float mixAmount = clamp((theDot - 0.90) / 0.05, 0.0,1.0);
        vec3 c1 = texture(texColor1,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        vec3 c2 = texture(texColor0,varyingtexcoord * tiling.xy + tiling.zw).rgb;

        vec3 n1 = normalize(tbnMat * (255.0/128.0 * texture(texNormal1,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
        vec3 n2 = normalize(tbnMat * (255.0/128.0 * texture(texNormal0,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);

        texInfo.texColor = mix(c1 , c2 , mixAmount);
        texInfo.texNormal = normalize(n1 * (1.0 - mixAmount) + n2 * mixAmount);
    }
    if(theDot > 0.94)
    {
        texInfo.texColor = texture(texColor0,varyingtexcoord * tiling.xy + tiling.zw).rgb;
        texInfo.texNormal = normalize(tbnMat * (255.0/128.0 * texture(texNormal0,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);
    }
    return texInfo;
}


float LinearizeDepth(float camNearClip, float camFarClip, float zoverw)
{
	float n = camNearClip; // camera z near
	float f = camFarClip; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{
    textureInfo tInfo   = getTextureInfo();

    vec3 pColor         = tInfo.texColor;
    vec3 pNormal        = tInfo.texNormal;

    vec3 wColor         = texture(texColorWater,varyingtexcoord).rgb ;//dot(, vec3(0.73,0.13,0.14));

    vec3    lightDir;
    float   attenuation;
    vec3    attenConst = vec3(0.0,1.0,0.0);

    if(light.w == 1.0){
        vec3 fragToLightSource = normalize(light.xyz-vPosition.xyz);

        float distance  = length(fragToLightSource);

        attenuation     = 1.0 / (attenConst.x
                           + attenConst.y * distance
                           + attenConst.z * distance * distance);

        lightDir       = normalize(fragToLightSource);
    }
    else
    {
        attenuation     = 1.0;
        lightDir        = normalize(light.xyz);
    }

    vec3 lightDiffuse   = attenuation * min ( pColor + wColor,1.0) * clamp( dot( pNormal, -lightDir ), 0.0, 1.0);
    vec3 lightAmbient   = pColor * ambientLight;

    outputColor  = vec4(lightAmbient + lightDiffuse,1.0) ;
    //outputColor  = vec4(vNormal,1.0) ;

    float depth =  gl_FragCoord.z;
    depth = LinearizeDepth(nearClip,farClip,depth);

	gl_FragDepth = depth;

}

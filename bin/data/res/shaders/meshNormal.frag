// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform sampler2D       tex1;


uniform vec3            ambientLight = vec3(0.7);

uniform vec4            tiling = vec4(1,1,0,0);

in vec4                 light;
in vec4                 vPosition;
in mat3                 tbnMat;
in vec2                 varyingtexcoord;

in vec3                 tNormal;

out vec4                outputColor;

void main()
{
    vec3 pColor         = texture(tex0,varyingtexcoord * tiling.xy + tiling.zw).rgb;//
    vec3 pNormal        = normalize(tbnMat * (255.0/128.0 * texture(tex1,varyingtexcoord * tiling.xy + tiling.zw).xyz - vec3(1.0) ).rgb);

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

    vec3 lightDiffuse   = attenuation * pColor * clamp( dot( pNormal, -lightDir ), 0.0, 1.0);
    vec3 lightAmbient   = pColor * ambientLight;

    outputColor  = vec4(lightAmbient + lightDiffuse,1.0) ;
    //outputColor  = vec4(pNormal,1.0) ;

}

// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform sampler2D       tex1;

uniform vec3            ambientLight = vec3(0.7);

in vec3                 lightPos;
in vec4                 vPosition;
in mat3                 tbnMat;
in vec2                 varyingtexcoord;

out vec4                outputColor;

void main()
{
    vec3 pColor         = texture(tex0,varyingtexcoord ).rgb;
    vec3 pNormal        = normalize(tbnMat * (255.0/128.0 * texture(tex1,varyingtexcoord ).xyz - vec3(1.0) ).rgb);

    vec3 lightDir       = tbnMat *  normalize(lightPos-vPosition.xyz);
    vec3 lightDiffuse   = pColor * clamp( dot( pNormal, lightDir ), 0.0, 1.0);
    vec3 lightAmbient   = pColor * ambientLight;

    outputColor  = vec4(lightDiffuse + lightAmbient,1.0) ;//
}

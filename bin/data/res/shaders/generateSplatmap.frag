// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       slopeMap;
uniform sampler2D       waterMap;


in vec2                 varyingtexcoord;
in vec3                 fNormal;

out vec4                outputColor;

void main()
{
    vec3 c = texture( waterMap,varyingtexcoord ).rgb;
    //vec3 n = normalize(texture( slopeMap,varyingtexcoord).xyz);
    vec3 n = normalize((255.0/128.0 * texture(slopeMap,varyingtexcoord).xyz - vec3(1.0) ).rgb);


    float theDot = dot(normalize(n),normalize(vec3(0.0,0.0,1.0)));
    vec4 splatColor;
    if(theDot < 0.60)
    {
        splatColor = vec4(0.0);
    }
    if(theDot >= 0.60 && theDot <= 0.65 )
    {
        float mixAmount = clamp((theDot - 0.6) / 0.05, 0.0, 1.0);
        mix(vec4(0.0) , vec4(1.0,0.0,0.0,1.0) , mixAmount);
    }
    if(theDot > 0.64 && theDot < 0.70 )
    {
        splatColor = vec4(1.0,0.0,0.0,1.0);
    }
    if(theDot >= 0.70 && theDot <= 0.75 )
    {
        float mixAmount = clamp((theDot - 0.7) / 0.05, 0.0, 1.0);
        mix(vec4(0.0) , vec4(0.0,1.0,0.0,1.0) , mixAmount);
    }
    if(theDot > 0.74 && theDot < 0.90)
    {
        splatColor = vec4(0.0,1.0,0.0,1.0);
    }
    if(theDot >= 0.9 && theDot <= 0.95 )
    {
        float mixAmount = clamp((theDot - 0.9) / 0.05, 0.0, 1.0);
        mix(vec4(0.0) , vec4(0.0,0.0,1.0,1.0) , mixAmount);
    }
    if(theDot > 0.94)
    {
        splatColor = vec4(0.0,0.0,1.0,1.0);
    }


    outputColor = vec4(splatColor);
}

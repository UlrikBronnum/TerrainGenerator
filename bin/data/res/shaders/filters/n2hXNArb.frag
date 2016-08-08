// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;

uniform int             texSize;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    float stepSize = 1.0 / float(texSize);

    vec3 texelBase = normalize( 255.0/128.0 * texture(tex0,varyingtexcoord ).xyz - vec3(1.0) ).rgb;

    vec3 texelStepX = texture(tex0, varyingtexcoord - vec2(stepSize,0)).xyz;
    vec3 texelStepY = texture(tex0, varyingtexcoord - vec2(0,stepSize)).xyz;

    float maxValue = min(texelStepX.x - texelBase.x * 0.5, texelStepY.y - texelBase.y * 0.5);
    outputColor = vec4( vec3(maxValue)  , 1.0) ;
}

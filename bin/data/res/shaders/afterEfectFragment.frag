// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       depthTexture;
uniform sampler2D       sceneTexture;

uniform vec4            light;
uniform vec3            cameraForward;

in vec2                 varyingtexcoord;

out vec4                outputColor;


void main()
{
    vec3 scene = texture(sceneTexture,varyingtexcoord).rgb;
    float depth = texture(depthTexture,varyingtexcoord).r;

    vec3 color = mix(scene, vec3(0.5) , depth*depth*depth);



    outputColor = vec4(vec3(color),1.0);
    //outputColor = vec4( vec3(1-depth),1.0);
}

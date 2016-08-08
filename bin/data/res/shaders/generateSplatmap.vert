#version 150


// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

// this is the end of the default functionality

// this is something we're creating for this shader
out vec2 varyingtexcoord;
out vec3 fNormal;

void main()
{
    // here we move the texture coordinates
    varyingtexcoord = texcoord ;

    fNormal = normalize(vec4(normalize(normal),0.0)).xyz;

    gl_Position = modelViewProjectionMatrix * position;;
}

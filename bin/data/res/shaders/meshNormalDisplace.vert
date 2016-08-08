#version 150


// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;
in vec3 tangent;

// this is the end of the default functionality

uniform sampler2D       tex2;
uniform vec4            light_pos = vec4(100,100,100,1);

// this is something we're creating for this shader
out vec2 varyingtexcoord;
out vec4 vPosition;
out vec3 lightPos;
out mat3 tbnMat;

void main()
{
    // here we move the texture coordinates
    varyingtexcoord = texcoord ;

    vec3 n = normalize(modelViewMatrix * vec4(normalize(normal),0.0)).xyz;
    vec3 t = normalize(modelViewMatrix * vec4(normalize(tangent),0.0)).xyz;
    t = normalize(t - dot(t,n) * n);
    vec3 bT = cross(t,n);
    tbnMat = mat3(t,bT,n);

    float height = texture(tex2,texcoord).r ;

	vec4 newPosition = position + normalize(normal) * height;

    vPosition = modelViewProjectionMatrix * newPosition;
    lightPos = light_pos.xyz;

    gl_Position = modelViewProjectionMatrix * newPosition;
}

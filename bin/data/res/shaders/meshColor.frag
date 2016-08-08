// fragment shader
#version 150

// this is how we receive the texture
uniform vec3            ambientLight = vec3(0.7);

in vec3                 lightPos;

in vec2                 varyingtexcoord;

out vec4                outputColor;

void main(){

    outputColor  = vec4(ambientLight,1.0) ;////vec4(tbnMat * vec3(0.0,-1.0,0.0), 1.0 );
}

// fragment shader
#version 150

uniform sampler2D       depthTexture;

uniform float           nearClip;
uniform float           farClip;

in vec2                 varyingtexcoord;

out vec4                outputColor;

float LinearizeDepth(float camNearClip, float camFarClip, float zoverw)
{
	float n = camNearClip; // camera z near
	float f = camFarClip; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{
    float depth = texture(depthTexture,varyingtexcoord).r;
    depth = LinearizeDepth(nearClip,farClip,depth);

	outputColor = vec4(depth,0,0,1);
}

// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D tex0;
uniform int diameter;
uniform int kernel[625];
uniform int kernelCount;

in vec2 varyingtexcoord;
out vec4 outputColor;


void main()
{
    int size = diameter;
    if(size > 25)size = 25;

    vec3 color;
    int radius = (size-1)/2;
    vec2 pos  = varyingtexcoord;

    int count[3] = { 0, 0, 0};

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
            if(kernel[(y+radius) * size + (x+radius)] == 1){
				vec3 pixelColor = texture(tex0, pos + vec2(float(x), float(y))).rgb;
				if(pixelColor.r > 0.5){
					count[0]++;
				}
				if(pixelColor.g > 0.5){
					count[1]++;
				}
                if(pixelColor.b > 0.5){
					count[2]++;
				}

            }
        }
    }
    if(kernelCount == 0){
        if(0 < count[0]){
            color.r = 1.0;
        }else{
            color.r = 0.0;
        }
		if(0 < count[1]){
            color.g = 1.0;
        }else{
            color.g = 0.0;
        }
		if(0 < count[2]){
            color.b = 1.0;
        }else{
            color.b = 0.0;
        }
    }else{
		if(kernelCount == count[0]){
            color.r = 1.0;
        }else{
            color.r = 0.0;
        }
		if(kernelCount == count[1]){
            color.g = 1.0;
        }else{
            color.g = 0.0;
        }
        if(kernelCount == count[2]){
            color.b = 1.0;
        }else{
            color.b = 0.0;
        }
    }
    outputColor = vec4(color*channel,1.0);
}

//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect kinect;     //it can be named in any way, GLSL just links it
uniform sampler2DRect texture1;   //second image

in vec2 texCoordVarying;

void main(){
    vec2 pos = gl_TexCoord[0].xy;     
    // Imagem da camera
    vec4 corKinect = texture2DRect(kinect, pos);
    // Imagem da textura
    vec4 corTexture =  texture2DRect(texture1, pos);
    vec4 color;

    if(corKinect.r > 0.5) {
        color = corTexture;
    } else {
        color.a = 0;
    }
    color.r = corTexture.r;
    color.g = corTexture.g;
    color.b = corTexture.b;
    color.a = corKinect.r;

    gl_FragColor = color;

}

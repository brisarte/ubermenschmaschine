//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it

void main(){
    vec2 pos = gl_TexCoord[0].st;     

    // Imagem do video
    vec4 color0 = texture2DRect(texture0, pos);

    vec4 color;

    color.a = (color0.r + color0.g + color0.b)/3;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    gl_FragColor = color;
}

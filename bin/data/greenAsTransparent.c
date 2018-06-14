//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it

uniform sampler2DRect texture1;   //second image

#define N (256)
uniform float specArray[N];

//uniform float time;             //Uniform value "time"

//blurring
void main(){
  vec2 pos = gl_TexCoord[0].st;     
  
  // Imagem da camera
  vec4 color0 = texture2DRect(texture0, pos);
  // Imagem do sensor de profundidade
  vec4 corDepth =  texture2DRect(texture1, pos);

  vec4 color;

  color = color0;  
  if(color.r < 0.1 && color.g > 0.7 && color.b < 0.1) {
      color.a = 0;
  }
  if(color.g > 0.6 && color.r < 0.5 && color.b <0.5 && color.a > 0.2) {
      color.a = 1 - color.g;
      color.g = 1 - color.g;
  }
  gl_FragColor = color;
}

//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect kinect;     //it can be named in any way, GLSL just links it
uniform sampler2DRect texture1;   //second image

#define WIDTH (1024)
#define HEIGHT (768)

//uniform float time;             //Uniform value "time"

//blurring
void main(){
    vec2 pos = gl_TexCoord[0].xy;     
    // Imagem da camera
    vec4 corKinect = texture2DRect(kinect, pos);
    vec4 corLateral;
    vec2 direcaoAux;

    vec2 direcao;
    direcao.x = 0;
    direcao.y = 0;

    for(int i = -3; i <=3; i++) {
        for(int j = -3; j <=3; j++) {
            if( i != 0 && j != 0) { // Não processa o do centro
                direcaoAux = pos;
                if(pos.x > 0 && pos.x < WIDTH) {
                    direcaoAux.x += i;
                    corLateral = texture2DRect(kinect, direcaoAux);
                    direcao.x += (direcaoAux.x - pos.x) * (corLateral.r + corLateral.g + corLateral.b)/3;
                }
                if(pos.y > 0 && pos.y < HEIGHT) {
                    direcaoAux.y += j;
                    corLateral = texture2DRect(kinect, direcaoAux);
                    direcao.y += (direcaoAux.y - pos.y) * corLateral.r;
                }
            }
        }
    }

    vec4 color;
    vec4 corTeste;
    corTeste.r = (direcao.x);
    corTeste.g = (direcao.y);
    corTeste.b = (direcao.x);

    corTeste.a = 1;
    //color = corTeste;  
    color = texture2DRect(texture1, pos + direcao);
    gl_FragColor = color;
}

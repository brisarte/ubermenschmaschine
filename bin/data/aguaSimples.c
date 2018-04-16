void mainImage( out vec4 fragColor, in vec2 fragCoord )
{

    vec2 uv = fragCoord.xy / iResolution.xy;
    vec4 maskColor = texture(iChannel1,uv);//Grab the mask color (this is created in the Buf A tab)

    if(maskColor.r == 0.0){//Apply our funky water shader only in parts that are red in the mask
        float X = uv.x*25.+iTime;
        float Y = uv.y*25.+iTime;
        uv.y += cos(X+Y)*0.01*cos(Y);
        uv.x += sin(X-Y)*0.01*sin(Y);
    }

    fragColor = texture(iChannel0,uv);
    //Uncomment this line to see the mask 
    //    //fragColor = maskColor;
}

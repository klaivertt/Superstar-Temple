#version 130

uniform sampler2D texture;

uniform float timeElapsed;
uniform bool reverseEffect;
uniform sampler2D degrade;


void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, texcoord);

     float color = 
     (pixel.r * 0.299 + 
      pixel.g * 0.587 +
      pixel.b * 0.114
     );

     color = fract(color + timeElapsed * (reverseEffect ? -1: 1));
     pixel = texture2D(degrade, vec2(color,color));

	gl_FragColor = pixel ;
}


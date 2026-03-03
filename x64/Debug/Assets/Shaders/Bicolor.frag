#version 130

uniform sampler2D texture;

uniform float intensitySeparation;
uniform float aboveIntensity;
uniform float belowIntensity;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, texcoord);

    float gray = (pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
    float finalColor;

    if(gray > intensitySeparation) finalColor = aboveIntensity;
    else            finalColor = belowIntensity;

    pixel.rgb = vec3(finalColor);

	gl_FragColor = pixel ;
}
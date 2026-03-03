#version 130

uniform sampler2D texture;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, texcoord);

    float gray = (pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
    pixel.rgb = vec3(gray);

	gl_FragColor = pixel ;
}
#version 100

precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform vec2 lightPos;
uniform vec2 size;

float circleDist(vec2 p, float radius)
{
	return length(p) - radius;
}

float fillMask(float dist)
{
	return clamp(-dist, 0.0, 1.0);
}

vec4 drawLight(vec2 p, vec2 pos, vec4 color, float range, float radius)
{
	// distance to light
	float ld = length(p - pos);
	
	// out of range
	if (ld > range) return vec4(0.0);
	
	// shadow and falloff
	//float shad = shadow(p, pos, radius);
	float fall = (range - ld)/range;
	fall *= fall;
	float source = fillMask(circleDist(p - pos, radius));
	return (fall + source) * color;
}


float luminance(vec4 col)
{
	return 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
}


void setLuminance(inout vec4 col, float lum)
{
	lum /= luminance(col);
	col *= lum;
}

void main()
{
    vec2 p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 invertedYLightPos = vec2(lightPos.x, size.y - lightPos.y);


    vec4 col = gl_FragColor;

    col += drawLight(p, invertedYLightPos, vec4(0.5, 0.75, 1.0, 1.0), 500.0, 10.0);

    gl_FragColor = clamp(col, 0.0, 1.0);
}

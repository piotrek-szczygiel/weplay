#version 100

precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float seconds;
uniform vec2 size;

vec2 freq = vec2(25, 25);
vec2 amp = vec2(5, 5);
vec2 speed = vec2(1, 1);

void main()
{
    float pixelWidth = 1.0 / size.x;
    float pixelHeight = 1.0 / size.y;

    vec2 p = fragTexCoord;
    p.x += cos(fragTexCoord.y * freq.x / (pixelWidth * 750.0) + (seconds * speed.x)) * amp.x
        * pixelWidth;
    p.y += sin(fragTexCoord.x * freq.y / (pixelHeight * 750.0) + (seconds * speed.y)) * amp.y
        * pixelHeight;

    gl_FragColor = texture2D(texture0, p) * fragColor * colDiffuse;
}

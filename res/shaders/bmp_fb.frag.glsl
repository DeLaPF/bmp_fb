#version 330 core

in vec2 vTexCoord;

void main()
{
    gl_FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);
}

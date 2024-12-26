#version 330 core

layout(location = 0) in uint packedData;

uniform mat3 u_WorldProj;

out vec2 vTexCoord;

const uint MASK_X = uint(0x0003FF);
const uint MASK_Y = uint(0x0FFC00);
const uint MASK_S = uint(0x100000);
const uint MASK_T = uint(0x200000);
const uint MASK_NONE = uint(0xFFC00000);

void main()
{
    float x = float((packedData&MASK_X) >> 0);
    float y = float((packedData&MASK_Y) >> 10);
    float s = float((packedData&MASK_S) >> 20);
    float t = float((packedData&MASK_T) >> 21);
    vec3 hPos = vec3(x, y, 1.0);
    vec2 projected = (u_WorldProj * hPos).xy;
    vec2 texCoord = vec2(s, t);

    vTexCoord = texCoord;
    gl_Position = vec4(projected.xy, 1.0, 1.0);
    // gl_Position = vec4(x, y, 1.0, 1.0);
}

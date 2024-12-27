#version 450 core

layout(location = 0) in uint packedData;

uniform mat3 u_WorldProj;
uniform ivec2 u_BitmapDim;
uniform ivec2 u_Res;

out vec2 vTexCoord;
flat out uint vColorInd;

const uint MASK_X = uint(0x0003FF);
const uint MASK_Y = uint(0x0FFC00);
const uint MASK_S = uint(0x100000);
const uint MASK_T = uint(0x200000);

void main()
{
    uint x = (packedData&MASK_X) >>  0;
    uint y = (packedData&MASK_Y) >> 10;
    uint s = (packedData&MASK_S) >> 20;
    uint t = (packedData&MASK_T) >> 21;
    vec3 hPos = vec3(float(x), float(y), 1.0);
    vec2 projected = (u_WorldProj * hPos).xy;
    vec2 texCoord = vec2(float(s), float(t));

    vTexCoord = texCoord;
    vColorInd = (y-t)*u_BitmapDim.x + (x-s);
    gl_Position = vec4(projected.xy, 1.0, 1.0);
}

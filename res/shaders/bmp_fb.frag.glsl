#version 450 core

layout(binding = 0, std430) readonly buffer bmpData { // TODO: does the name matter?
    uint bitmap[];
};
in vec2 vTexCoord;
flat in uint vColorInd;

const uint MASK_R = uint(0x0000FF);
const uint MASK_G = uint(0x00FF00);
const uint MASK_B = uint(0xFF0000);

void main()
{
    float r = float((bitmap[vColorInd]&MASK_R) >>  0);
    float g = float((bitmap[vColorInd]&MASK_G) >>  8);
    float b = float((bitmap[vColorInd]&MASK_B) >> 16);

    // gl_FragColor = vec4(r, g, b, 1.0);
    gl_FragColor = vec4(vTexCoord.x+r, vTexCoord.y+g, b, 1.0);
    // gl_FragColor = vec4(vColorInd/255.0, 0.0, 0.0, 1.0);
}

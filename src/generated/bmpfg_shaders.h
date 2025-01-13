#ifndef BMPFG_SHADERS_H
#define BMPFG_SHADERS_H

const char* BMP_FB_VERT =
    "#version 450 core\n"
    "\n"
    "layout(location = 0) in uint packedData;\n"
    "\n"
    "uniform mat3 u_ModelViewMat;\n"
    "\n"
    "out uvec2 vBitmapCoord;\n"
    "out vec2 vTexCoord;\n"
    "\n"
    "const uint MASK_X = uint(0x0003FF);\n"
    "const uint MASK_Y = uint(0x0FFC00);\n"
    "const uint MASK_S = uint(0x100000);\n"
    "const uint MASK_T = uint(0x200000);\n"
    "\n"
    "void main()\n"
    "{\n"
    "    uint x = (packedData&MASK_X) >>  0;\n"
    "    uint y = (packedData&MASK_Y) >> 10;\n"
    "    uint s = (packedData&MASK_S) >> 20;\n"
    "    uint t = (packedData&MASK_T) >> 21;\n"
    "\n"
    "    vec3 hPos = vec3(float(x), float(y), 1.0);\n"
    "    vec2 projected = (u_ModelViewMat * hPos).xy;\n"
    "    vec2 texCoord = vec2(float(s), float(t));\n"
    "\n"
    "    vBitmapCoord = uvec2(x-s, y-t);\n"
    "    vTexCoord = texCoord;\n"
    "    gl_Position = vec4(projected.xy, 1.0, 1.0);\n"
    "}\n";

const char* BMP_FB_FRAG =
    "#version 450 core\n"
    "\n"
    "layout(std430, binding = 0) readonly buffer ssbo0 {\n"
    "    uint bitmap[];\n"
    "};\n"
    "flat in uvec2 vBitmapCoord;\n"
    "in vec2 vTexCoord;\n"
    "\n"
    "uniform uvec2 u_BitmapDim;\n"
    "uniform uint u_ColorFormatFlag;\n"
    "\n"
    "out vec4 fColor;\n"
    "\n"
    "// Color Packing format\n"
    "const uint MASK_RGBA = uint(0x1);\n"
    "const uint MASK_SINGLE_BIT = uint(0x2);\n"
    "\n"
    "// RGBA\n"
    "const uint MASK_R = uint(0xFF000000);\n"
    "const uint MASK_G = uint(0x00FF0000);\n"
    "const uint MASK_B = uint(0x0000FF00);\n"
    "const uint MASK_A = uint(0x000000FF);\n"
    "\n"
    "void main()\n"
    "{\n"
    "    uint selRGBA = (u_ColorFormatFlag&MASK_RGBA) >> 0;\n"
    "    uint selSINGLE_BIT = (u_ColorFormatFlag&MASK_SINGLE_BIT) >> 1;\n"
    "    uint pixelInd = vBitmapCoord.y*u_BitmapDim.x + vBitmapCoord.x;\n"
    "\n"
    "    uint colorUintInd = pixelInd*selRGBA;\n"
    "    colorUintInd += uint(floor(pixelInd/32))*selSINGLE_BIT;\n"
    "    uint colorBitInd = 0*selRGBA;\n"
    "    colorBitInd += (pixelInd%32)*selSINGLE_BIT;\n"
    "\n"
    "    // Unpack rgba\n"
    "    float r = float((bitmap[colorUintInd]&MASK_R) >> 24) / (0xFF-1);\n"
    "    float g = float((bitmap[colorUintInd]&MASK_G) >> 16) / (0xFF-1);\n"
    "    float b = float((bitmap[colorUintInd]&MASK_B) >>  8) / (0xFF-1);\n"
    "    float a = float((bitmap[colorUintInd]&MASK_A) >>  0) / (0xFF-1);\n"
    "    // Unpack singleBit\n"
    "    float singleBit = float((bitmap[colorUintInd] >> colorBitInd)&uint(1));\n"
    "\n"
    "    vec4 color = vec4(r, g, b, a)*selRGBA;\n"
    "    color += vec4(vec3(singleBit), 1.0)*selSINGLE_BIT;\n"
    "\n"
    "    fColor = color;\n"
    "}\n";

#endif
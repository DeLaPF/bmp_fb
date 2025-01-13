import os
from dataclasses import dataclass

@dataclass
class ShaderHeader:
    dest_path: str
    src_path: str
    files: list[str]

SHADER_HEADERS = {
    "bmpfg_shaders.h" : ShaderHeader(
        dest_path="src/generated/",
        src_path="res/shaders/",
        files=["bmp_fb.vert.glsl", "bmp_fb.frag.glsl"],
    ),
}

def main():
    for name, shader_header in SHADER_HEADERS.items():
        # Create dest_path if does not exist
        if not os.path.exists(shader_header.dest_path):
            os.makedirs(shader_header.dest_path)

        dest = os.path.join(shader_header.dest_path, name)
        # Create new file at dest (or rewrite content of file at dest)
        with open(dest, "w+") as df:
            # Open header guard
            h_guard = name.replace(".", "_").upper()
            df.write(f"#ifndef {h_guard}\n")
            df.write(f"#define {h_guard}\n\n")

            # Add const char* defs containing contents of each file
            for shader_file in shader_header.files:
                const_name = shader_file.split(".glsl")[0].replace(".", "_").upper();
                src = os.path.join(shader_header.src_path, shader_file)

                # Some shenanigans to align text, and place semicolon correctly
                df.write(f"const char* {const_name} =")
                with open(src, "r") as sf:
                    for line in sf:
                        df.write(f"\n    \"{line.rstrip()}\\n\"")
                df.write(";\n\n")

            # Close header guard
            df.write("#endif")

if __name__ == "__main__":
    main()

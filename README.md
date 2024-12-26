# Bitmap Framebuffer
"library" for rendering bitmapped displays

## Setup
### Cloning repo
Remeber `git clone --recursive ...`

### `glad_lib`
- `pip install git+https://github.com/Dav1dde/glad.git@f49229523c58ad7698e941f7c51454e77dd9d6a8`
- `glad --api=gl:core --out-path deps/glad_lib c` (Generate deps/glad_lib with required files)

Note:
- `python -m glad --help` (For more info: generating for vulkan, rust, etc.)
- Ran with python 3.13, likely works with older as well, but not tested

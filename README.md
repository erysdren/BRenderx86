# BRenderx86

A port of the BRender x86 SDK 1.1.2 from 1995 to modern systems.

Next step: using a mix of objconv and other tools to assemble a 32-bit BRender application with SDL2.

## How This Works

So, the main reason this port is possible is because of Open Watcom's backwards compatibility. The BRender x86 SDK 1.1.2 from 1995 was compiled with Watcom C 10, and the precompiled LIBs do not contain any DOS-specific code. All of that is contained in the DOSIO module. Using Open Watcom v2, I can link an x86 Linux application against the 1995 BRender LIBs and create a working headless BRender executable, which cleanly runs on my x86_64 machine without segfaulting. Using SDL 1.2 (which supports Watcom, but Win32 only) I can create a proper graphical BRender application which opens and properly displays BRender's output.

I think it's kind of amazing that I can coax my modern CPU into running compiled code created by Argonaut Software engineers almost 30 years ago.

## Source Files Guide

- `minimal_offscreen.c` - A headless "minimal" BRender application that draws a cube, saves the buffer to a pixelmap file, and then exits.
- `minimal_sdl.c` - A graphical BRender application using SDL 1.2, which opens a window and renders a cube in 8 bpp (but displayed in 32 bpp) and stays open until the user closes it. This one works on Win32 only.
- `thirdparty/brender/` - The BRender x86 1.1.2 SDK, precompiled under Watcom C 10 in 1995.
- `thirdparty/sdl/` - SDL 1.2, precompiled under Open Watcom v2 in 2023.

## Building

To build the executables, enter the project directory and type `wmake`.

## License

MIT License

Copyright (c) 1998 Argonaut Software Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

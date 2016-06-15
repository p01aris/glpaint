//========================================================================
//                      -  glpaint 0.1  - 
//------------------------------------------------------------------------
// Copyright (c) 2016-2017 Zheng Lu <polaris.varlin@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

//Demo
#include "glpaint_foo.hpp"



// void drawPoint(Vertex v1, GLfloat size);
// void drawPointDemo(int width, int height);

int main(void)
{
    glpaint_foo _glpaint(1024,768);
    //_glpaint.setEventHandling();
    //_glpaint.startDraw();
    _glpaint.gaussianDemo();
    return 0;
}

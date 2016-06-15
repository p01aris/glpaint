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

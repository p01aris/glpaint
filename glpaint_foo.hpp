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
#include "glpaint.h"
#include <iostream>

class glpaint_foo : public glpaint
{
    virtual void key_callback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods); 
    virtual void mouse_button_callback(GLFWwindow* window, int button,
		         int action, int mods);

    virtual void cursor_position_callback(GLFWwindow* window, double x,
		         double y);

	virtual void scroll_callback(GLFWwindow* window, double x, double y);

    virtual void error_callback(
    	int error, 
    	const char* description);
    virtual void DrawContent();
public:
    
    glpaint_foo(int windows_width,int windows_height):glpaint(windows_width, windows_height)
	{
		event_handling_instance = this;
	}
    
    void line_test()
    {
        alpha=0;
        beta=0;
        float phase_shift=0.0f;
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.0, 0.0, -2.0);
            // rotate by beta degrees around the x-axis
            glRotatef(beta, 1.0, 0.0, 0.0);
            // rotate by alpha degrees around the z-axis
            glRotatef(alpha, 0.0, 0.0, 1.0);
            
            drawOrigin();
            
            //gaussianDemo(sigma);
            phase_shift+=0.01f;
            linePlot(phase_shift);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }	
    }
    
    void drawPointsDemo(int width, int height){
        GLfloat size=5.0f;
        for(GLfloat x = 0.f; x<=1.0f; x+=0.2f, size+=5)
        {
            Vertex v1 = {x, 0.0f, 0.0f, x, 1.0f, 1-x, 1.0f};
            drawPoint(v1, size);
        }
    }
    
    void drawLineDemo(){
        //draw a simple grid
        drawGrid(5.0f, 1.0f, 0.1f);
        //define the vertices and colors of the line segments
        Vertex v1 = {-10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
        Vertex v2 = {10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
        drawLineSegment(v1, v2, 2.0f);
        v1.x = 0.0f;
        v2.x = 0.0f;
        v1.y = -1.0f;
        v2.y = 1.0f;
        drawLineSegment(v1, v2, 2.0f);
    }
    
    void drawTriangleDemo(){
        //Triangle Demo
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        Vertex v1 = {0.0f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 0.3f};
        Vertex v2 = {-0.4f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 0.3f};
        Vertex v3 = {0.4f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.3f};
        drawTriangle(v1, v2, v3);
    }
    void gaussianDemo(){
        //construct a 1000x1000 grid
        float sigma = 0.1f;
        float sign = 1.0f;
        float step_size = 0.005f;
        
        const int grid_x = 400;
        const int grid_y = 400;
        const int num_points = grid_x*grid_y;
        Data *data=(Data*)malloc(sizeof(Data)*num_points);
        int data_counter=0;
        
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(0.0, 0.0, -2.0);
            // rotate by beta degrees around the x-axis
            glRotatef(beta, 1.0, 0.0, 0.0);
            // rotate by alpha degrees around the z-axis
            glRotatef(alpha, 0.0, 0.0, 1.0);
            
            drawOrigin();
            sigma=sigma+sign*step_size;
            //standard deviation
            float sigma2=sigma*sigma;
            //amplitude
            float sigma_const = 10.0f*(sigma2*2.0f*(float)M_PI);
            
            if(sigma>1.0f){
                sign = -1.0f;
            }
            if(sigma<0.1){
                sign = 1.0f;
            }
            data_counter=0;
            for(float x = -grid_x/2.0f; x<grid_x/2.0f; x+=1.0f){
                for(float y = -grid_y/2.0f; y<grid_y/2.0f; y+=1.0f){
                    float x_data = 2.0f*x/grid_x;
                    float y_data = 2.0f*y/grid_y;
                    //compute the height z based on a
                    //2D Gaussian function.
                    float z_data = exp(-0.5f*(x_data*x_data)/(sigma2)
                                       -0.5f*(y_data*y_data)/(sigma2))/sigma_const;
                    data[data_counter].x = x_data;
                    data[data_counter].y = y_data;
                    data[data_counter].z = z_data;
                    data_counter++;
                } }
            //visualize the result using a 2D heat map
            draw2DHeatMap(data, num_points);
            
            //gaussianDemo(sigma);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        //free(data);
    }

};

void glpaint_foo::key_callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (action != GLFW_PRESS)
        return;
	switch (key)
    {
        case GLFW_KEY_ESCAPE:
             glfwSetWindowShouldClose(window, GL_TRUE);
             break;
        case GLFW_KEY_SPACE:
             locked=!locked;
             break;
        case GLFW_KEY_LEFT:
             alpha += 5.0f;
             break;
        case GLFW_KEY_RIGHT:
             alpha -= 5.0f;
             break;
        case GLFW_KEY_UP:
             beta -= 5.0f;
             break;
        case GLFW_KEY_DOWN:
             beta += 5.0f;
             break;
        case GLFW_KEY_8:
             zoom -= 0.25f;
             if (zoom < 0.0f)
                zoom = 0.0f;
             break;
        case GLFW_KEY_9:
             zoom += 0.25f;
             break;
        default:
             break;
    }
}

void glpaint_foo::mouse_button_callback(GLFWwindow* window, int button,
         int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
           return;
    if (action == GLFW_PRESS)
      {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = GL_TRUE;
      }
    else 
      {
        locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
}
void glpaint_foo::cursor_position_callback(GLFWwindow* window, double x,
         double y)
{
    //if the mouse button is pressed
     if (locked)
     {
        alpha += (GLfloat) (x - cursorX) / 10.0f;
        beta += (GLfloat) (y - cursorY) / 10.0f;
     }
     //update the cursor position
     cursorX = (int) x;
     cursorY = (int) y;
}

void glpaint_foo::scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.0f;
    if (zoom < 0.0f)
        zoom = 0.0f;
}

void glpaint_foo::error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void glpaint_foo::DrawContent()
{
    alpha=0.0f, beta=0.0f, zoom=1.0f;
    float phase_shift=((float)global_coutor)*0.05;
    linePlot(phase_shift);
}
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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <math.h>

class glpaint
{
	public:
		
		GLFWwindow* window;
		int width, height;
		GLboolean locked = GL_FALSE;
		GLfloat alpha=210.0f, beta=-70.0f, zoom=0.5f;
        int global_coutor;
		int cursorX, cursorY;
		virtual void error_callback(
			int error, 
			const char* description)=0;

		static void error_callback_dispatcher(int error, const char* description)
		{
			if(event_handling_instance)
	            event_handling_instance->error_callback(error, description);
		}
		virtual void key_callback(
				GLFWwindow* window,
				int key, 
				int scancode,
				int action, 
				int mods)=0;

		static void keycallback_dispatcher(
	        GLFWwindow *window,
	        int key,
	        int scancode,
	        int action,
	        int mods)
	    {
	        if(event_handling_instance)
	            event_handling_instance->key_callback(window,key,scancode,action,mods);
	    }
		virtual void mouse_button_callback(GLFWwindow* window, int button,
		         int action, int mods)=0;

		static void mouse_button_callback_dispatcher(GLFWwindow* window, int button,
		         int action, int mods)
		{
			if(event_handling_instance)
	            event_handling_instance->mouse_button_callback(window,button,action,mods);
		}

		virtual void cursor_position_callback(GLFWwindow* window, double x,
		         double y)=0;

		static void cursor_position_callback_dispatcher(GLFWwindow* window, double x,
		         double y)
		{
			if(event_handling_instance)
	            event_handling_instance->cursor_position_callback(window,x,y);
		}

		virtual void scroll_callback(GLFWwindow* window, double x, double y)=0;

		static void scroll_callback_dispatcher(GLFWwindow* window, double x, double y)
		{
			if(event_handling_instance)
	            event_handling_instance->scroll_callback(window,x,y);
		}		
		static glpaint *event_handling_instance;

		void setEventHandling() { event_handling_instance = this; }
        virtual void DrawContent()=0;
    
        void startDraw()
    {
        while (!glfwWindowShouldClose(window))
        {
            ++global_coutor;
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
            DrawContent();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    
		glpaint(int windows_width,int windows_height)
		{
            global_coutor=0;
            WINDOWS_WIDTH=windows_width;
            WINDOWS_HEIGHT=windows_height;
            
			event_handling_instance = this; //

		    glfwSetErrorCallback(error_callback_dispatcher);
		    if (!glfwInit())
		        exit(EXIT_FAILURE);
		    window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Simple example", NULL, NULL);
		    if (!window)
		    {
		        glfwTerminate();
		        exit(EXIT_FAILURE);
		    }
		    glfwMakeContextCurrent(window);
		    glfwSwapInterval(1);

		    glfwSetKeyCallback(window, glpaint::keycallback_dispatcher);
		    glfwGetFramebufferSize(window, &width, &height);
		    framebuffer_size_callback(window, width, height);
		    //mouse button callback
		    glfwSetMouseButtonCallback(window, mouse_button_callback_dispatcher);
		   //mouse movement callback
		    glfwSetCursorPosCallback(window, cursor_position_callback_dispatcher);
		   //mouse scroll callback
		    glfwSetScrollCallback(window, scroll_callback_dispatcher);

		    //enable anti-aliasing
		    glEnable(GL_BLEND);
		    //smooth the points
		    glEnable(GL_LINE_SMOOTH);
		    //smooth the lines
		    glEnable(GL_POINT_SMOOTH);
		    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		    //needed for alpha blending
		    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		    glEnable(GL_ALPHA_TEST) ;

		}

		~glpaint()
		{
			glfwDestroyWindow(window);
		    glfwTerminate();
		    exit(EXIT_SUCCESS);
		}

		void gluPerspective(GLdouble fovY,
		                    GLdouble aspect,
		                    GLdouble zNear,
		                    GLdouble zFar)
		{
		    const double DEG2RAD = 3.14159265 / 180;
		     // tangent of half fovY
		     double tangent = tan(fovY/2 * DEG2RAD);
		     // half height of near plane
		     double height_f = zNear * tangent;
		     // half width of near plane
		     double width_f = height_f * aspect;
		     //Create the projection matrix based on the near clipping
		     //plane and the location of the corners
		     glFrustum(-width_f, width_f, -height_f, height_f, zNear, zFar);
		}
		void framebuffer_size_callback(GLFWwindow* window, int width,
		         int height)
		{
		    const float fovY = 45.0f;
		    const float front = 0.1f;
		    const float back = 128.0f;
		    float ratio = 1.0f;
		    if (height > 0)
		       ratio = (float) width / (float) height;
		    glViewport(0, 0, width, height);
		    glMatrixMode(GL_PROJECTION);
		    glLoadIdentity();
		    gluPerspective(fovY, ratio, front, back);
		}
		typedef struct 
		{
		    GLfloat x, y, z;
		    GLfloat r, g, b, a; 
		} Vertex;

		typedef struct
		{
		    GLfloat x, y, z;
		} Data;


		int drawLineSegment(Vertex v1, Vertex v2, GLfloat width)
		{
		    glLineWidth(width);
		    glBegin(GL_LINES);
		    glColor4f(v1.r, v1.g, v1.b, v1.a);
		    glVertex3f(v1.x, v1.y, v1.z);
		    glColor4f(v2.r, v2.g, v2.b, v2.a);
		    glVertex3f(v2.x, v2.y, v2.z);
		    glEnd();
            return 0;
		}
		void drawGrid(GLfloat width, GLfloat height, GLfloat grid_width)
		{
		     //horizontal lines
		    for(float i=-height; i<height; i+=grid_width){
		        Vertex v1 = {-width, i, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};
		        Vertex v2 = {width, i, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};
		        drawLineSegment(v1, v2, 1.f);
		    }
		     //vertical lines
		    for(float i=-width; i<width; i+=grid_width){
		       Vertex v1 = {i, -height, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};
		       Vertex v2 = {i, height, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};
		       drawLineSegment(v1, v2 , 1.f);
		    } 
		}
		void drawPoint(Vertex v1, GLfloat size)
		{
		    glEnable(GL_POINT_SMOOTH);
		    glEnable(GL_BLEND);
		    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		    glPointSize(size);
		    glBegin(GL_POINTS);
		    glColor4f(v1.r, v1.g, v1.b, v1.a);
		    glVertex3f(v1.x, v1.y, v1.z);
		    glEnd();
		}
		void draw2DScatterPlot(const Data *data, int num_points)
		{
		    Vertex v1 = {-10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		    Vertex v2 = {10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		    drawLineSegment(v1, v2, 2.0f);
		    v1.x = 0.0f;
		    v2.x = 0.0f;
		    v1.y = -1.0f;
		    v2.y = 1.0f;
		    drawLineSegment(v1, v2, 2.0f);
		    //draw point
		    for(int i=0; i<num_points; i++){
		       GLfloat x=data[i].x;
		       GLfloat y=data[i].y;
		       Vertex v={x, y, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f};
		       drawPoint(v, 8.0f);
		    }
		}
		void draw2DLineSegments(const Data *data, int num_points){
		     for(int i=0; i<num_points-1; i++){
		       GLfloat x1=data[i].x;
		       GLfloat y1=data[i].y;
		       GLfloat x2=data[i+1].x;
		       GLfloat y2=data[i+1].y;
		       Vertex v1={x1, y1, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f};
		       Vertex v2={x2, y2, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f};
		       drawLineSegment(v1, v2, 10.0f);
		    } 
		}
    void linePlot(float phase_shift){
        drawGrid(5.0f, 1.0f, 0.1f);
        GLfloat range = 10.0f;
        const int num_points = 500;
        Data *data=(Data*)malloc(sizeof(Data)*num_points);
        for(int i=0; i<num_points; i++){
            data[i].x=((GLfloat)i/num_points)*range-range/2.0f;
            data[i].y= 0.8f*cosf(data[i].x*3.14f-phase_shift);
        }
        draw2DScatterPlot(data, num_points);
        draw2DLineSegments(data, num_points);
        free(data);
    }
		void draw2DHeatMap(const Data *data, int num_points){
		    glPointSize(3.0f);
		    glBegin(GL_POINTS);
		    float transparency = 0.25f;
		    //locate the maximum and minimum values in the dataset
		   float max_value=-999.9f;
		   float min_value=999.9f;
		   for(int i=0; i<num_points; i++){
		     const Data d = data[i];
		     if(d.z > max_value){
		       max_value = d.z;
		     }
		   if(d.z < min_value){
		       min_value = d.z;
		   } }
		   const float halfmax = (max_value + min_value) / 2;
		    //display the result
		   glPointSize(2.0f);
		   glBegin(GL_POINTS);
		   for(int i = 0; i<num_points; i++){
		        const Data d = data[i];
		        float value = d.z;
		        float b = 1.0f - value/halfmax;
		        float r = value/halfmax - 1.0f;
		        if(b < 0)
		            b=0; 
		        if(r < 0)
		            r=0;
		        float g = 1.0f - b - r;
		        glColor4f(r, g, b, 0.5f);
		        glVertex3f(d.x, d.y, d.z);
		   }
		   glEnd(); 
		}

		void drawTriangle(Vertex v1, Vertex v2, Vertex v3)
		{
		    glBegin(GL_TRIANGLES);
		    glColor4f(v1.r, v1.g, v1.b, v1.a);
		    glVertex3f(v1.x, v1.y, v1.z);
		    glColor4f(v2.r, v2.g, v2.b, v2.a);
		    glVertex3f(v2.x, v2.y, v2.z);
		    glColor4f(v3.r, v3.g, v3.b, v3.a);
		    glVertex3f(v3.x, v3.y, v3.z);
		    glEnd();
		}

		void drawOrigin(){

		     glLineWidth(4.0f);
		     glBegin(GL_LINES);
		     float transparency = 0.5f;
		     //draw a red line for the x-axis
		     glColor4f(1.0f, 0.0f, 0.0f, transparency);
		     glVertex3f(0.0f, 0.0f, 0.0f);
		     glColor4f(1.0f, 0.0f, 0.0f, transparency);
		     glVertex3f(0.3f, 0.0f, 0.0f);
		     //draw a green line for the y-axis
		     glColor4f(0.0f, 1.0f, 0.0f, transparency);
		     glVertex3f(0.0f, 0.0f, 0.0f);
		     glColor4f(0.0f, 1.0f, 0.0f, transparency);
		     glVertex3f(0.0f, 0.0f, 0.3f);
		     //draw a blue line for the z-axis
		     glColor4f(0.0f, 0.0f, 1.0f, transparency);
		     glVertex3f(0.0f, 0.0f, 0.0f);
		     glColor4f(0.0f, 0.0f, 1.0f, transparency);
		     glVertex3f(0.0f, 0.3f, 0.0f);
		     glEnd();
		}
	
	private:
		int WINDOWS_WIDTH = 1280;
		int WINDOWS_HEIGHT = 720;
		
		//Vertex v = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
};
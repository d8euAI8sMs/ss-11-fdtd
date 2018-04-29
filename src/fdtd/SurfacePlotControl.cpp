// SurfacePlotControl.cpp : implementation file
//

#include "stdafx.h"
#include "fdtd.h"
#include "SurfacePlotControl.h"

#include <gl/GLU.h>
#include <gl/GL.h>

static void _interpolate_color(double value, GLfloat *color)
{
    const float aR = 0; const float aG = 0; const float aB = 1;
    const float bR = 1; const float bG = 1; const float bB = 0;

    color[0] = (float)((bR - aR) * (value + 1) / 2) + aR;
    color[1] = (float)((bG - aG) * (value + 1) / 2) + aG;
    color[2] = (float)((bB - aB) * (value + 1) / 2) + aB;
}


// CSurfacePlotControl

IMPLEMENT_DYNAMIC(CSurfacePlotControl, COglControl)

CSurfacePlotControl::CSurfacePlotControl()
    : top_view(false)
    , perspective_view(true)
    , scale(1)
    , zangle(30)
    , xangle(-45)
{
}

CSurfacePlotControl::~CSurfacePlotControl()
{
}


BEGIN_MESSAGE_MAP(CSurfacePlotControl, COglControl)
END_MESSAGE_MAP()



// CSurfacePlotControl message handlers

void CSurfacePlotControl::OnDrawItemOGL()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (perspective_view)
        gluPerspective(45, 1, 2, 7);
    else
        glOrtho(-1, 1, -1, 1, -100, 100);
    if (!top_view)
    {
        glTranslatef(0, 0, -4);
        glRotatef(xangle, 1, 0, 0);
        glRotatef(zangle, 0, 0, 1);
        glScaled(scale, scale, scale);
    }

    glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat color[3 * 4];

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i + 1 < points.size(); ++i)
    for (size_t j = 0; j + 1 < points[i].size(); ++j)
    {
        _interpolate_color(values[i][j], color);
        _interpolate_color(values[i + 1][j], color + 3);
        _interpolate_color(values[i][j + 1], color + 6);
        _interpolate_color(values[i + 1][j + 1], color + 9);

        glColor3fv(color); glVertex3d(points[i][j].x, points[i][j].y, values[i][j]);
        glColor3fv(color + 3); glVertex3d(points[i + 1][j].x, points[i + 1][j].y, values[i + 1][j]);
        glColor3fv(color + 9); glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, values[i + 1][j + 1]);
        glColor3fv(color); glVertex3d(points[i][j].x, points[i][j].y, values[i][j]);
        glColor3fv(color + 6); glVertex3d(points[i][j + 1].x, points[i][j + 1].y, values[i][j + 1]);
        glColor3fv(color + 9); glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, values[i + 1][j + 1]);
    }
    glEnd();

    glFlush();
}

void CSurfacePlotControl::OnDestroyOGL()
{
    glFinish();
}

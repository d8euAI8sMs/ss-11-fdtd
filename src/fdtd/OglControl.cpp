// OglControl.cpp : implementation file
//

#include "stdafx.h"
#include "fdtd.h"
#include "OglControl.h"


// COglControl

IMPLEMENT_DYNAMIC(COglControl, CStatic)

COglControl::COglControl()
    : glRC(NULL)
{
}

COglControl::~COglControl()
{
}


static PIXELFORMATDESCRIPTOR _pfd =
{
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
    1,                              // version number
    PFD_DRAW_TO_WINDOW |            // support window
    PFD_SUPPORT_OPENGL |            // support OpenGL
    PFD_DOUBLEBUFFER,               // double buffered
    PFD_TYPE_RGBA,                  // RGBA type
    24,                             // 24-bit color depth
    0, 0, 0, 0, 0, 0,               // color bits ignored
    0,                              // no alpha buffer
    0,                              // shift bit ignored
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // accum bits ignored
    32,                             // 32-bit z-buffer
    0,                              // no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main layer
    0,                              // reserved
    0, 0, 0                         // layer masks ignored
};


BEGIN_MESSAGE_MAP(COglControl, CStatic)
    ON_WM_DRAWITEM()
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()



// COglControl message handlers

void COglControl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CStatic::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void COglControl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC * pDC = GetDC();
    HDC hDC = pDC->GetSafeHdc();

    __try
    {
        wglMakeCurrent(hDC, glRC);
        OnDrawItemOGL();
        SwapBuffers(hDC);
        wglMakeCurrent(hDC, NULL);
    }
    __finally
    {
        ReleaseDC(pDC);
    }
}

void COglControl::OnDestroy()
{
    CStatic::OnDestroy();

    if (!glRC) return;

    OnDestroyOGL();

    wglDeleteContext(glRC);
}

void COglControl::PreSubclassWindow()
{
    CDC * pDC = GetDC();
    HDC hDC = pDC->GetSafeHdc();

    __try
    {
        int pixelformat;

        if (!(pixelformat = ChoosePixelFormat(hDC, &_pfd)))
            return;

        if (!SetPixelFormat(hDC, pixelformat, &_pfd))
            return;

        glRC = wglCreateContext(hDC);
        if (!glRC)
            return;

        OnCreateOGL();
    }
    __finally
    {
        ReleaseDC(pDC);
    }

    CStatic::PreSubclassWindow();
}

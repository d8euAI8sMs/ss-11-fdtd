// fdtdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fdtd.h"
#include "fdtdDlg.h"
#include "afxdialogex.h"

#include "model.h"
#include <GL/GL.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFdtdDlg dialog



CFdtdDlg::CFdtdDlg(CWnd* pParent /*=NULL*/)
    : CSimulationDialog(CFdtdDlg::IDD, pParent)
    , m_fTopView(FALSE)
    , m_fPerspectiveView(TRUE)
    , m_fAutoRotate(FALSE)
    , m_lfRotationDelta(1)
    , p(model::make_default_parameters())
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFdtdDlg::DoDataExchange(CDataExchange* pDX)
{
    CSimulationDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_2DPLOT, m_cPlot2d);
    DDX_Check(pDX, IDC_CHECK1, m_fTopView);
    DDX_Check(pDX, IDC_CHECK2, m_fPerspectiveView);
    DDX_Control(pDX, IDC_SLIDER1, m_cScaleFactorCtrl);
    DDX_Control(pDX, IDC_SLIDER2, m_cRotationAngleCtrl);
    DDX_Check(pDX, IDC_CHECK3, m_fAutoRotate);
    DDX_Control(pDX, IDC_SLIDER4, m_cXRotationAngleCtrl);
    DDX_Text(pDX, IDC_EDIT4, p.dx);
    DDX_Text(pDX, IDC_EDIT5, p.dy);
    DDX_Text(pDX, IDC_EDIT3, p.T);
    DDX_Text(pDX, IDC_EDIT7, p.eps);
    DDX_Text(pDX, IDC_EDIT10, p.sx);
    DDX_Text(pDX, IDC_EDIT11, p.sy);
    DDX_Text(pDX, IDC_EDIT8, p.smag);
    DDX_Text(pDX, IDC_EDIT9, p.d);
}

BEGIN_MESSAGE_MAP(CFdtdDlg, CSimulationDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CFdtdDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CFdtdDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_CHECK1, &CFdtdDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &CFdtdDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK3, &CFdtdDlg::OnBnClickedCheck1)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CFdtdDlg message handlers

BOOL CFdtdDlg::OnInitDialog()
{
    CSimulationDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here

    m_cPlot2d.color_factor = 5;
    m_cPlot2d.xangle = 0;
    m_cPlot2d.zangle = 270;
    m_cPlot2d.scale = 1.5;
    m_cPlot2d.custom_painter = [this] () {
        glColor3b(0, 0, 0);
        glLineWidth(5);
        glBegin(GL_LINES);
        {
            glVertex3d(-1, 0, 0.1);
            glVertex3d(-p.d / 2, 0, 0.1);
            glVertex3d(p.d / 2, 0, 0.1);
            glVertex3d(1, 0, 0.1);
            glVertex3d(-1, 0, -0.1);
            glVertex3d(-p.d / 2, 0, -0.1);
            glVertex3d(p.d / 2, 0, -0.1);
            glVertex3d(1, 0, -0.1);
        }
        glEnd();
        glBegin(GL_QUADS);
        {
            glVertex3d(-1, 0, -0.1);
            glVertex3d(-p.d / 2, 0, -0.1);
            glVertex3d(-p.d / 2, 0, 0.1);
            glVertex3d(-1, 0, 0.1);
            glVertex3d(p.d / 2, 0, -0.1);
            glVertex3d(1, 0, -0.1);
            glVertex3d(1, 0, 0.1);
            glVertex3d(p.d / 2, 0, 0.1);
        }
        glEnd();
    };

    int rmin, rmax;
    m_cXRotationAngleCtrl.GetRange(rmin, rmax);
    m_cXRotationAngleCtrl.SetPos((int) ((m_cPlot2d.xangle / 180 + 1) / 2 * (rmax - rmin) + rmin));
    m_cRotationAngleCtrl.GetRange(rmin, rmax);
    m_cRotationAngleCtrl.SetPos((int) (m_cPlot2d.zangle / 360 * (rmax - rmin) + rmin));
    m_cScaleFactorCtrl.GetRange(rmin, rmax);
    m_cScaleFactorCtrl.SetPos((int) ((m_cPlot2d.scale - 1) / 5 * (rmax - rmin) + rmin));

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFdtdDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CSimulationDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFdtdDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CFdtdDlg::OnSimulation()
{
    model::adjust_parameters(p);
    model::reset_model_data(p, d);
    model::fdtd_solve solver(p, d);
    while(m_bWorking)
    {
        m_cPlot2d.points = d.points;
        m_cPlot2d.values.resize(d.points.size());
        for (size_t i = 0; i < d.points.size(); ++i)
        {
            m_cPlot2d.values[i].resize(d.points[i].size());
            for (size_t j = 0; j < d.points[i].size(); ++j)
            {
                m_cPlot2d.values[i][j] = d.cells[i][j].ez;
            }
        }
        if (m_fAutoRotate) m_cPlot2d.zangle += m_lfRotationDelta;
        m_cPlot2d.RedrawWindow();
        solver.next();
    }

    CSimulationDialog::OnSimulation();
}


void CFdtdDlg::OnBnClickedButton1()
{
    UpdateData(TRUE);
    StartSimulationThread();
}


void CFdtdDlg::OnBnClickedButton2()
{
    StopSimulationThread();
}


void CFdtdDlg::OnBnClickedCheck1()
{
    UpdateData(TRUE);
    m_cPlot2d.top_view = (m_fTopView == TRUE);
    m_cPlot2d.perspective_view = (m_fPerspectiveView == TRUE);
}


void CFdtdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    int rmin, rmax;
    m_cScaleFactorCtrl.GetRange(rmin, rmax);
    m_cPlot2d.scale = (1 + 5 * (double) (m_cScaleFactorCtrl.GetPos() - rmin) / (rmax - rmin));
    m_cXRotationAngleCtrl.GetRange(rmin, rmax);
    m_cPlot2d.xangle = 180 * 2 * ((double) (m_cXRotationAngleCtrl.GetPos() - rmin) / (rmax - rmin) - 0.5);
    if (m_fAutoRotate)
    {
        m_cRotationAngleCtrl.GetRange(rmin, rmax);
        m_lfRotationDelta = (1 + (double) (m_cRotationAngleCtrl.GetPos() - rmin) / (rmax - rmin));
    }
    else
    {
        m_cRotationAngleCtrl.GetRange(rmin, rmax);
        m_cPlot2d.zangle = 360 * (double) (m_cRotationAngleCtrl.GetPos() - rmin) / (rmax - rmin);
    }
    CSimulationDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// fdtdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fdtd.h"
#include "fdtdDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFdtdDlg dialog



CFdtdDlg::CFdtdDlg(CWnd* pParent /*=NULL*/)
    : CSimulationDialog(CFdtdDlg::IDD, pParent)
    , p(model::make_default_parameters())
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFdtdDlg::DoDataExchange(CDataExchange* pDX)
{
    CSimulationDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFdtdDlg, CSimulationDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CFdtdDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CFdtdDlg::OnBnClickedButton2)
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

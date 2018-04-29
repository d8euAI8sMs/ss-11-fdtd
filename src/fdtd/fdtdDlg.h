// fdtdDlg.h : header file
//

#pragma once

#include <util/common/gui/SimulationDialog.h>
#include "SurfacePlotControl.h"
#include "afxcmn.h"
#include "model.h"

// CFdtdDlg dialog
class CFdtdDlg : public CSimulationDialog
{
// Construction
public:
    CFdtdDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    enum { IDD = IDD_FDTD_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
    void OnSimulation();
public:
    CSurfacePlotControl m_cPlot2d;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedCheck1();
    BOOL m_fTopView;
    BOOL m_fPerspectiveView;
    CSliderCtrl m_cScaleFactorCtrl;
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    CSliderCtrl m_cRotationAngleCtrl;
    BOOL m_fAutoRotate;
    double m_lfRotationDelta;
    CSliderCtrl m_cXRotationAngleCtrl;
    model::parameters p;
    model::model_data d;
};

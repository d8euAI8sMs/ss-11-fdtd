// fdtdDlg.h : header file
//

#pragma once

#include <util/common/gui/SimulationDialog.h>

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
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
};

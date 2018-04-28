#pragma once

// COglControl

class COglControl : public CStatic
{
    DECLARE_DYNAMIC(COglControl)

private:

    HGLRC glRC;

public:

    HGLRC GetGLRC() { return glRC; }

public:
    COglControl();
    virtual ~COglControl();
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual void OnCreateOGL() { }
    virtual void OnDrawItemOGL() { }
    virtual void OnDestroyOGL() { }
    afx_msg void OnDestroy();
    virtual void PreSubclassWindow();
};

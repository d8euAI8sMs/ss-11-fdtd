#pragma once

#include <vector>

#include <util/common/geom/point.h>
#include <util/common/plot/shape.h>

#include "OglControl.h"

// CSurfacePlotControl

class CSurfacePlotControl : public COglControl
{
    DECLARE_DYNAMIC(CSurfacePlotControl)

public:

    std::vector < std::vector < geom::point < double > > > points;
    std::vector < std::vector < double > > values;

public:
    CSurfacePlotControl();
    virtual ~CSurfacePlotControl();
    virtual void OnDrawItemOGL();
    virtual void OnDestroyOGL();

protected:
    DECLARE_MESSAGE_MAP()
};

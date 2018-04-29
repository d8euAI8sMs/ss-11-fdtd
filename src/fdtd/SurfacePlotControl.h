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
    bool top_view;
    bool perspective_view;
    double scale;
    double zangle;
    double xangle;
    double color_factor;

public:
    CSurfacePlotControl();
    virtual ~CSurfacePlotControl();
    virtual void OnDrawItemOGL();
    virtual void OnDestroyOGL();

protected:
    DECLARE_MESSAGE_MAP()
};

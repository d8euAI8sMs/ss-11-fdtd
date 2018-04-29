#pragma once

#include <util/common/geom/point.h>

#include <vector>
#include <map>

namespace model
{

    /*****************************************************/
    /*                     params                        */
    /*****************************************************/

    struct parameters
    {
        // system params
        double eps;

        // geometry params
        double d;
        double sx, sy;

        // PML params
        double pml_nx, pml_ny;

        // source params
        double T0, T, smag;

        // other params
        double dt, dx, dy;
    };

    inline void adjust_parameters(parameters & p)
    {
        p.dy = p.dx;
        p.dt = p.dx * 6e-8;
    }

    inline parameters make_default_parameters()
    {
        parameters p =
        {
            // system params
            1,

            // geometry params
            0.1,
            -0.5, 0,

            // PML params
            32, 32,

            // source params
            75, 15, 0.00025,

            // other params
            0, 0.005, 0.005
        };
        adjust_parameters(p);
        return p;
    }

    /*****************************************************/
    /*                     data                          */
    /*****************************************************/

    struct cell_ref
    {
        size_t i, j;
    };
    inline bool operator < (const cell_ref & o1, const cell_ref & o2) { return (o1.i == o2.i) ? (o1.j < o2.j) : (o1.i < o2.i); }

    struct cell
    {
        double hx, hy, ez, dz;
        double ihx, ihy;
    };

    struct layer
    {
        double f0, g0;
        double f1() { return f0; }
        double f2() { return 1. / (1. + f0); }
        double f3() { return (1. - f0) / (1. + f0); }
        double g1() { return g0; }
        double g2() { return 1. / (1. + g0); }
        double g3() { return (1. - g0) / (1. + g0); }
    };

    struct model_data
    {
        std::vector < std::vector < geom::point2d_t > > points;
        std::vector < std::vector < cell > > cells;
        std::vector < layer > x_layers;
        std::vector < layer > y_layers;
        std::map < cell_ref, std::function < double /* dz */ (size_t t) > > ss;
        std::map < cell_ref, double /* dz */ > hs;
    };

    inline void reset_model_data(const parameters & p, model_data & d)
    {
        size_t rows = (size_t) std::ceil(2 / p.dy);
        size_t cols = (size_t) std::ceil(2 / p.dx);

        d.ss.clear(); d.hs.clear();
        d.points.resize(rows, std::vector < geom::point2d_t > (cols));
        d.cells.resize(rows, std::vector < cell > (cols));
        d.y_layers.resize(rows); d.x_layers.resize(cols);

        for (size_t i = 0; i < rows; ++i)
        {
            d.points[i].resize(cols); /* ensure vector size */
            d.cells[i].resize(cols);  /* ensure vector size */
            for (size_t j = 0; j < cols; ++j)
            {
                d.points[i][j] = { i * p.dx - 1, j * p.dy - 1 };
                d.cells[i][j] =
                {
                    0, 0, 0, 0,
                    0, 0
                };
            }
        }

        /* calculate PML params */

        for (size_t i = 0; i < rows; ++i)
        {
            d.y_layers[i] = { 0, 0 };
        }

        for (size_t j = 0; j < cols; ++j)
        {
            d.y_layers[j] = { 0, 0 };
        }

        for (size_t i = 0; i <= p.pml_ny; ++i)
        {
            double xn = (p.pml_ny - i) / p.pml_ny;
            d.y_layers[i].g0 = 0.33 * xn * xn * xn;
            d.y_layers[rows - i - 1].g0 = d.y_layers[i].g0;
            xn = (p.pml_ny - (i + 0.5)) / p.pml_ny;
            d.y_layers[i].f0 = 0.25 * xn * xn * xn;
            d.y_layers[rows - i - 1].f0 = d.y_layers[i].f0;
        }

        for (size_t j = 0; j <= p.pml_nx; ++j)
        {
            double xn = (p.pml_nx - j) / p.pml_nx;
            d.x_layers[j].g0 = 0.33 * xn * xn * xn;
            d.x_layers[rows - j - 1].g0 = d.x_layers[j].g0;
            xn = (p.pml_nx - (j + 0.5)) / p.pml_nx;
            d.x_layers[j].f0 = 0.25 * xn * xn * xn;
            d.x_layers[rows - j - 1].f0 = d.x_layers[j].f0;
        }

        /* calculate source and slit params */

        size_t srows = (size_t) std::ceil(p.d / p.dy);

        for (size_t i = 0; i < rows; ++i)
        {
            if (((rows / 2 - srows / 2) < i) && (i < (rows / 2 + srows / 2))) continue;
            d.hs[{ i, cols / 2 }] = 0;
        }

        size_t si = (size_t) std::ceil((p.sy + 1) / p.dy);
        size_t sj = (size_t) std::ceil((p.sx + 1) / p.dx);

        d.ss[{ si, sj }] = [p] (size_t ti)
        {
            double t = std::remainder((double) ti, p.T0) + p.T0 / 4, rt;
            rt = t - p.T0 / 2;
            double pos_pulse = std::exp(- 0.5 * rt * rt / p.T / p.T);
            rt = t - p.T0;
            double neg_pulse = std::exp(- 0.5 * rt * rt / p.T / p.T);
            return p.smag * (pos_pulse - neg_pulse);
        };
    }

    /*****************************************************/
    /*                     FDTD                          */
    /*****************************************************/

    class fdtd_solve
    {
    private:
        const parameters & p;
        model_data & d;
        size_t t;
    public:
        fdtd_solve(const parameters & p, model_data & d)
            : p(p), d(d), t(0)
        {
        }
    public:
        void next()
        {
            size_t rows = (size_t) std::ceil(2 / p.dy);
            size_t cols = (size_t) std::ceil(2 / p.dx);

            for (size_t j = 1; j < cols; ++j)
            for (size_t i = 1; i < rows; ++i)
            {
                d.cells[i][j].dz =
                    d.y_layers[i].g3() * d.x_layers[j].g3() * d.cells[i][j].dz +
                    d.y_layers[i].g2() * d.x_layers[j].g2() * 0.5 * (
                        d.cells[i][j].hy - d.cells[i - 1][j].hy -
                        d.cells[i][j].hx + d.cells[i][j - 1].hx);
            }

            for each(auto & c in d.ss)
            {
                d.cells[std::get<0>(c).i][std::get<0>(c).j].dz += std::get<1>(c)(t) / p.dx / p.dy;
            }

            for each(auto & c in d.hs)
            {
                d.cells[std::get<0>(c).i][std::get<0>(c).j].dz = std::get<1>(c) / p.dx / p.dy;
            }

            for (size_t j = 0; j < cols; ++j)
            for (size_t i = 0; i < rows; ++i)
            {
                d.cells[i][j].ez = d.cells[i][j].dz / p.eps;
            }

            for (size_t j = 0; j < cols - 1; ++j)
            {
                d.cells[0][j].ez = 0;
                d.cells[rows - 1][j].ez = 0;
            }

            for (size_t i = 0; i < rows - 1; ++i)
            {
                d.cells[i][0].ez = 0;
                d.cells[i][cols - 1].ez = 0;
            }

            for (size_t j = 0; j < cols - 1; ++j)
            for (size_t i = 1; i < rows; ++i)
            {
                d.cells[i][j].ihx +=
                    d.y_layers[i].f1() * (d.cells[i][j].ez - d.cells[i][j + 1].ez);
                d.cells[i][j].hx =
                    d.x_layers[j].f3() * d.cells[i][j].hx +
                    d.x_layers[j].f2() * 0.5 * (
                        d.cells[i][j].ez - d.cells[i][j + 1].ez + d.cells[i][j].ihx
                    );
            }

            for (size_t j = 0; j < cols; ++j)
            for (size_t i = 0; i < rows - 1; ++i)
            {
                d.cells[i][j].ihy +=
                    d.x_layers[j].f1() * (d.cells[i + 1][j].ez - d.cells[i][j].ez);
                d.cells[i][j].hy =
                    d.y_layers[i].f3() * d.cells[i][j].hy +
                    d.y_layers[i].f2() * 0.5 * (
                        d.cells[i + 1][j].ez - d.cells[i][j].ez + d.cells[i][j].ihy
                    );
            }

            ++t;
        }
    };
}

#include "splitfxm/cell.h"
#include "splitfxm/derivatives.h"
#include "splitfxm/schemes.h"
#include "app/equation.h"

AppEquation::AppEquation(double g)
    : g_(g) {}

Eigen::VectorXd AppEquation::non_stiff_residuals(const std::vector<CellPtr> &cell_sub,
                                                 Schemes scheme,
                                                 const std::map<std::string, double> &scheme_opts) const
{
    // Get y values at each cell (assumes single DOF per cell)
    std::vector<double> y(cell_sub.size());
    for (size_t i = 0; i < cell_sub.size(); ++i)
    {
        y[i] = cell_sub[i]->values()(0);
    }

    // Compute first and second derivatives
    double yp = dx(y, cell_sub, scheme);
    double ypp = d2x(y, cell_sub, scheme);

    // Compute residual only at center index
    double res_val = ypp + g_ * yp;

    Eigen::VectorXd res(1);
    res(0) = res_val;
    return res;
}

Eigen::VectorXd AppEquation::stiff_residuals(const std::vector<CellPtr> &cell_sub,
                                             Schemes scheme,
                                             const std::map<std::string, double> &scheme_opts) const
{
    int size = cell_sub[0]->values().size();
    return Eigen::VectorXd::Zero(size);
}

Eigen::VectorXd AppEquation::dae_mask(int size) const
{
    return Eigen::VectorXd::Ones(size);
}

Eigen::VectorXd AppEquation::stiff_mask(int size) const
{
    return Eigen::VectorXd::Zero(size);
}

Eigen::VectorXd AppEquation::non_stiff_mask(int size) const
{
    return Eigen::VectorXd::Ones(size);
}

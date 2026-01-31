#include "splitfxm/cell.h"
#include "splitfxm/operators/operators.h"
#include "splitfxm/schemes.h"
#include "app/equation.h"

using namespace Operators::FDM;

AppEquation::AppEquation(double g)
    : g_(g) {}

Eigen::VectorXd AppEquation::non_stiff_residuals(const std::vector<CellPtr>& cell_sub) const
{
    // Get y values at each cell (assumes single DOF per cell)
    std::vector<double> y(cell_sub.size());
    for (size_t i = 0; i < cell_sub.size(); ++i)
    {
        y[i] = cell_sub[i]->values()(0);
    }

    // Compute first and second derivatives
    double yp = dx(y, cell_sub, scheme_);
    double ypp = d2x(y, cell_sub, scheme_);

    // Compute residual only at center index
    double res_val = ypp + g_ * yp;

    Eigen::VectorXd res(1);
    res(0) = res_val;
    return res;
}

Eigen::VectorXd AppEquation::stiff_residuals(const std::vector<CellPtr>& cell_sub) const
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

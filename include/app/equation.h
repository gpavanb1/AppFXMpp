#ifndef APP_EQUATION_H
#define APP_EQUATION_H

#include <functional>
#include <map>
#include <vector>
#include <Eigen/Dense>
#include "splitfxm/cell.h"
#include "splitfxm/equation.h"
#include "splitfxm/schemes.h"

class AppEquation : public Equation
{
   public:
    AppEquation(double g);

    Eigen::VectorXd stiff_residuals(const std::vector<CellPtr>& cell_sub) const override;
    Eigen::VectorXd non_stiff_residuals(const std::vector<CellPtr>& cell_sub) const override;

    Eigen::VectorXd dae_mask(int size) const override;
    Eigen::VectorXd stiff_mask(int size) const override;
    Eigen::VectorXd non_stiff_mask(int size) const override;

   private:
    double g_;
};

#endif  // APP_EQUATION_H

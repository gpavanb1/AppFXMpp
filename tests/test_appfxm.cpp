#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include <string>
#include <map>
#include "splitfxm/domain.h"
#include "splitfxm/simulation.h"
#include "splitfxm/schemes.h"
#include "splitfxm/bc.h"
#include "app/model.h"

TEST(AppEquationTest, MatchesAnalyticalSolution)
{
    double g = 9.81;

    // Create the model and domain
    auto model = std::make_shared<AppModel>(g);
    auto domain = Domain::from_size_1d(60, 1, 1, {"y"}, 0.0, 1.0);

    // Boundary conditions
    BCMap bcs;
    bcs["y"]["LEFT"] = std::make_pair("dirichlet", 0.0);
    bcs["y"]["RIGHT"] = std::make_pair("dirichlet", 1.0);

    std::map<std::string, std::string> ics;

    // Simulation
    auto sim = std::make_shared<Simulation>(domain, model, ics, bcs, Schemes::FD_CENTRAL);

    // Steady-state solve
    std::vector<int> split_locs = {};
    std::vector<std::vector<double>> bounds = {{-1.0}, {1.0}};
    sim->steady_state(split_locs, 50, 0.0, 0.0, bounds);

    // Check against analytical solution: y(x) = (1 - exp(-g x)) / (1 - exp(-g))
    for (const auto& cell : domain->interior())
    {
        double x = cell->coords()[0];
        double expected = (1.0 - std::exp(-g * x)) / (1.0 - std::exp(-g));
        double numerical = cell->value(0);
        EXPECT_NEAR(numerical, expected, 1e-3);  // Tolerance of 1e-3 is good enough for this FD scheme
    }
}

#include "app/model.h"
#include "splitfxm/bc.h"
#include "splitfxm/domain.h"
#include "splitfxm/schemes.h"
#include "splitfxm/simulation.h"
#include "splitfxm/visualize/visualize.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>

int main()
{
    // Create the model (uses TrajectoryEquation with g = 9.81)
    auto model = std::make_shared<AppModel>();

    // Create a domain: 80 interior cells, 1 left + 1 right ghost cell, 1 variable
    // "y"
    auto domain = Domain::from_size_1d(80, 1, 1, {"y"}, 0.0, 1.0);

    // Define boundary conditions: Dirichlet at both ends
    BCMap bcs;
    bcs["y"]["LEFT"] = std::make_pair("dirichlet", 0.0);
    bcs["y"]["RIGHT"] = std::make_pair("dirichlet", 1.0);

    // Empty initial conditions
    std::map<std::string, std::string> ics;

    // Create the simulation with default finite difference scheme
    auto sim = std::make_shared<Simulation>(domain, model, ics, bcs,
                                            Schemes::FD_CENTRAL);

    // Evolve to steady state
    double t_diff = 0.1;
    std::vector<int> split_locs = {};
    sim->evolve_sim(t_diff, split_locs, "NoSplit", "Euler", 1e-6, 1e-3);
    std::vector<std::vector<double>> bounds = {{-1.0}, {1.0}};
    int num_iter = sim->steady_state(split_locs, 50, 0.0, 0.0, bounds);

    std::cout << "Converged in " << num_iter << " iterations.\n";

    // Output the result
    for (const auto& cell : domain->interior())
    {
        std::cout << cell->coords()[0] << " " << cell->values().transpose() << "\n";
    }

    visualize(domain, false, "y");

    return 0;
}

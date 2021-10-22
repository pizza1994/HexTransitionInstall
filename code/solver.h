#ifndef SOLVER_H
#define SOLVER_H

#include <cinolib/meshes/meshes.h>
#include <gurobi_c++.h>
#include "utils.h"

uint div_factor=4;
void solve(cinolib::Polyhedralmesh<> &m);

#include "solver.cpp"

#endif // SOLVER_H

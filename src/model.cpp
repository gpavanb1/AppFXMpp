#include "splitfxm/error.h"
#include "app/model.h"
#include "app/equation.h"

AppModel::AppModel(double g)
    : Model(nullptr), g_(g), equation_(nullptr)
{
    // Create the equation
    equation_ = std::make_shared<AppEquation>(g_);

    // Register it in the base Model
    Model::set_equation(equation_);
}

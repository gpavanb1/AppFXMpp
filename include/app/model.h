#ifndef APP_MODEL_H
#define APP_MODEL_H

#include <memory>
#include "splitfxm/model.h"
#include "splitfxm/equation.h"
#include "equation.h"

class AppModel : public Model
{
public:
    double g_;
    EquationPtr equation_;

    AppModel(double g = 9.81);
};

#endif // APP_MODEL_H

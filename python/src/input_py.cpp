//
//  input_py.cpp
//  blocksci_interface
//
//  Created by Harry Kalodner on 12/7/17.
//

#include "input_py.hpp"
#include "caster_py.hpp"
#include "self_apply_py.hpp"

#include <pybind11/operators.h>

namespace py = pybind11;

using namespace blocksci;

const char *inputDocstring(std::string docstring) {
    return strdup(docstring.c_str());
}

void init_input(py::module &m) {
    py::class_<Input> inputClass(m, "Input", "Class representing a transaction input");
    inputClass
    .def("__repr__", &Input::toString)
    .def(py::self == py::self)
    .def(hash(py::self))
    .def_property_readonly("_access", &Input::getAccess, py::return_value_policy::reference)
    ;
    
    applyMethodsToSelf(inputClass, AddInputMethods{inputDocstring});
}

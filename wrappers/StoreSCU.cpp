/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <boost/python.hpp>

#include "odil/StoreSCU.h"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(storeMethod, odil::StoreSCU::store, 1, 3)

void wrap_StoreSCU()
{
    using namespace boost::python;
    using namespace odil;

    class_<StoreSCU>("StoreSCU", init<Association &>())
        .def(
            "get_affected_sop_class",
            &StoreSCU::get_affected_sop_class,
            return_value_policy<copy_const_reference>()
        )
        .def(
            "set_affected_sop_class",
            static_cast<void(StoreSCU::*)(DataSet const &)>(&StoreSCU::set_affected_sop_class)
        )
        .def(
            "store", 
            &StoreSCU::store,
            storeMethod()
        )
    ;
}

/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _c80c338c_36d7_4724_9732_c7afed87902b
#define _c80c338c_36d7_4724_9732_c7afed87902b

#include "odil/Association.h"
#include "odil/DataSet.h"
#include "odil/odil.h"
#include "odil/SCU.h"

namespace odil
{

/// @brief SCU for C-Store services.
class ODIL_API NSetSCU: public SCU
{
public:
    /// @brief Constructor.
    NSetSCU(Association & association);

    /// @brief Destructor.
    virtual ~NSetSCU();
    
    /// @brief Set the affected SOP class based on the dataset.
    void set_affected_sop_class(DataSet const & dataset);

	using SCU::set_affected_sop_class;
    
    /// @brief Perform the N-SET.
    void set( DataSet const & dataset) const;
};

}

#endif // _c80c338c_36d7_4724_9732_c7afed87902b

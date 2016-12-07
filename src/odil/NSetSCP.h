/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef __NSETSCP_H__
#define __NSETSCP_H__

#include <functional>

#include "odil/Association.h"
#include "odil/SCP.h"
#include "odil/Value.h"
#include "odil/message/Message.h"
#include "odil/message/NSetRequest.h"

namespace odil
{

/// @brief SCP for N-Set services.
class NSetSCP: public SCP
{
public:
    /**
     * @brief Callback called when a request is received, shall throw an
     * SCP::Exception on error.
     */
    typedef std::function<Value::Integer(message::NSetRequest const &)> Callback;

    /// @brief Constructor.
    NSetSCP(Association & association);

    /// @brief Constructor.
    NSetSCP(Association & association, Callback const & callback);

    /// @brief Destructor.
    virtual ~NSetSCP();

    /// @brief Return the callback.
    Callback const & get_callback() const;

    /// @brief Set the callback.
    void set_callback(Callback const & callback);

    /// @brief Process a C-NSet request.
    virtual void operator()(message::Message const & message);


private:
    Callback _callback;
};

}

#endif // __NSETSCP_H__

/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _5d350855_531e_405a_a221_af3b0e720fd1
#define _5d350855_531e_405a_a221_af3b0e720fd1

#include "odil/registry.h"
#include "odil/message/Response.h"
#include "odil/odil.h"
#include "odil/Value.h"

namespace odil
{

namespace message
{

/// @brief N-Set-RSP message.
class ODIL_API NSetResponse: public Response
{
public:
    /**
     * @brief Create an NSet response with given Message ID, status,
     * affected SOP class UID and affected  SOP instance UID
     */
    NSetResponse(
            Value::Integer const & message_id_being_responded_to,
            Value::Integer const & status,
            Value::String const & requested_sop_class_uid  ,
            Value::String const & requested_sop_instance_uid
            );

    /**
     * @brief Create a N-Set-RSP from a generic Message.
     *
     * Raise an exception if the Message does not contain a N-Set-RSP.
     */
    NSetResponse(Message const & message);

    /// @brief Destructor.
    virtual ~NSetResponse();
    
    ODIL_MESSAGE_OPTIONAL_FIELD_STRING_MACRO(affected_sop_class_uid, registry::AffectedSOPClassUID)
    ODIL_MESSAGE_OPTIONAL_FIELD_STRING_MACRO(affected_sop_instance_uid, registry::AffectedSOPInstanceUID)

};

}

}

#endif // _5d350855_531e_405a_a221_af3b0e720fd1

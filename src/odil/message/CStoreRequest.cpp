/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include "CStoreRequest.h"

#include <string>

#include "odil/Exception.h"
#include "odil/message/Request.h"

namespace odil
{

namespace message
{

CStoreRequest
::CStoreRequest(
    Value::Integer message_id, Value::String const & affected_sop_class_uid,
    Value::String const & affected_sop_instance_uid,
    Value::Integer priority, DataSet const & dataset,
    Value::String const & move_originator_ae_title,
    Value::Integer move_originator_message_id)
: Request(message_id)
{
    this->set_command_field(Command::C_STORE_RQ);
    this->set_affected_sop_class_uid(affected_sop_class_uid);
    this->set_affected_sop_instance_uid(affected_sop_instance_uid);
    this->set_priority(priority);

    if(!move_originator_ae_title.empty())
        this->set_move_originator_ae_title(move_originator_ae_title);
    if(move_originator_message_id >= 0)
        this->set_move_originator_message_id(move_originator_message_id);

    if(dataset.empty())
    {
        throw Exception("Data set is required");
    }
    this->set_data_set(dataset);
}

CStoreRequest
::CStoreRequest(Message const & message)
: Request(message)
{
    if(message.get_command_field() != Command::C_STORE_RQ)
    {
        throw Exception("Message is not a C-STORE-RQ");
    }
    this->set_command_field(message.get_command_field());

    this->set_affected_sop_class_uid(
        message.get_command_set().as_string(registry::AffectedSOPClassUID, 0));
    this->set_affected_sop_instance_uid(
        message.get_command_set().as_string(registry::AffectedSOPInstanceUID, 0));
    this->set_priority(message.get_command_set().as_int(registry::Priority, 0));

    ODIL_MESSAGE_SET_OPTIONAL_FIELD_MACRO(
        message.get_command_set(), move_originator_ae_title,
        registry::MoveOriginatorApplicationEntityTitle, as_string)
    ODIL_MESSAGE_SET_OPTIONAL_FIELD_MACRO(
        message.get_command_set(), move_originator_message_id,
        registry::MoveOriginatorMessageID, as_int)

    if(!message.has_data_set() || message.get_data_set().empty())
    {
        throw Exception("Data set is required");
    }
    this->set_data_set(message.get_data_set());
}

CStoreRequest
::~CStoreRequest()
{
    // Nothing to do.
}

}

}

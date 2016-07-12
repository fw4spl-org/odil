/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <iterator>
#include <map>
#include <vector>

#include "odil/base64.h"
#include "odil/registry.h"
#include "odil/xml_converter.h"

namespace odil
{

/// @brief Element visitor converting to XML.
struct ToXMLVisitor
{
    typedef boost::property_tree::ptree result_type;

    result_type operator()(VR const vr) const
    {
        result_type result;

        result.put("<xmlattr>.vr", as_string(vr)); // Mandatory

        return result;
    }

    template<typename T>
    result_type operator()(VR const vr, T const & value) const
    {
        result_type result;

        result.put("<xmlattr>.vr", as_string(vr)); // Mandatory

        unsigned int count = 0;
        for(auto const & item: value)
        {
            ++count;
            boost::property_tree::ptree tag_value;
            tag_value.put("<xmlattr>.number", count); // Mandatory
            tag_value.put_value(item);

            result.add_child("Value", tag_value);
        }

        return result;
    }

    result_type operator()(VR const vr, Value::Integers const & value) const
    {
        result_type result;

        result.put("<xmlattr>.vr", as_string(vr)); // Mandatory

        unsigned int count = 0;
        for(auto const & item: value)
        {
            ++count;
            boost::property_tree::ptree tag_value;
            tag_value.put("<xmlattr>.number", count); // Mandatory
            tag_value.put_value(item);

            result.add_child("Value", tag_value);
        }

        return result;
    }

    result_type operator()(VR const vr, Value::Strings const & value) const
    {
        result_type result;

        result.put("<xmlattr>.vr", as_string(vr)); // Mandatory

        if(vr == VR::PN)
        {
            auto const fields = { "Alphabetic", "Ideographic", "Phonetic" };

            unsigned int count = 0;
            for(auto const & item: value)
            {
                ++count;
                boost::property_tree::ptree tag_value;
                tag_value.put("<xmlattr>.number", count); // Mandatory

                auto fields_it = fields.begin();
                std::string::size_type begin=0;
                while(begin != std::string::npos)
                {
                    std::string::size_type const end = item.find("=", begin);

                    std::string::size_type size = 0;
                    if(end != std::string::npos)
                    {
                        size = end-begin;
                    }
                    else
                    {
                        size = std::string::npos;
                    }

                    boost::property_tree::ptree tag_field;
                    std::string value_name = item.substr(begin, size);

                    auto const fields_name = { "FamilyName", "GivenName",
                                               "MiddleName", "NamePrefix",
                                               "NameSuffix" };
                    auto fields_name_it = fields_name.begin();
                    std::string::size_type begin_name=0;
                    while(begin_name != std::string::npos)
                    {
                        std::string::size_type const end_name =
                                value_name.find("^", begin_name);

                        std::string::size_type size_name = 0;
                        if(end_name != std::string::npos)
                        {
                            size_name = end_name-begin_name;
                        }
                        else
                        {
                            size_name = std::string::npos;
                        }

                        boost::property_tree::ptree tag_name;

                        tag_name.put_value(value_name.substr(begin_name,
                                                             size_name));

                        tag_field.add_child(*fields_name_it, tag_name);

                        if(end_name != std::string::npos)
                        {
                            begin_name = end_name+1;
                            ++fields_name_it;
                            if(fields_name_it == fields_name.end())
                            {
                                throw Exception("Invalid Person Name");
                            }
                        }
                        else
                        {
                            begin_name = end_name;
                        }
                    }

                    tag_value.add_child(*fields_it, tag_field);

                    if(end != std::string::npos)
                    {
                        begin = end+1;
                        ++fields_it;
                        if(fields_it == fields.end())
                        {
                            throw Exception("Invalid Person Name");
                        }
                    }
                    else
                    {
                        begin = end;
                    }
                }
                result.add_child("PersonName", tag_value);
            }
        }
        else
        {
            unsigned int count = 0;
            for(auto const & item: value)
            {
                ++count;
                boost::property_tree::ptree tag_value;
                tag_value.put("<xmlattr>.number", count); // Mandatory
                tag_value.put_value(item);

                result.add_child("Value", tag_value);
            }
        }

        return result;
    }

    result_type operator()(VR const vr, Value::DataSets const & value) const
    {
        result_type result;

        result.put("<xmlattr>.vr", as_string(vr)); // Mandatory

        unsigned int count = 0;
        for(auto const & item: value)
        {
            ++count;
            boost::property_tree::ptree tag_value;
            tag_value.put("<xmlattr>.number", count); // Mandatory
            boost::property_tree::ptree tag_result = as_xml(item);
            tag_value.insert(tag_value.end(), tag_result.front().second.begin(),
                                              tag_result.front().second.end());
            result.add_child("Item", tag_value);
        }

        return result;
    }

    result_type operator()(VR const vr, Value::Binary const & value) const
    {
        if(value.size() > 1)
        {
            // PS3.18 2016b, F.2.7: There is a single InlineBinary value
            // representing the entire Value Field.
            // PS3.18 2016b, Figure 6.5-1: Pixel data is not encoded in
            // JSON/XML, but transfered using a different content type
            throw Exception("Binary element is multiple-valued");
        }

        result_type result;

        result.put("<xmlattr>.vr", as_string(vr)); // Mandatory

        boost::property_tree::ptree tag_value;

        std::string encoded;
        encoded.reserve(value[0].size()*4/3);
        base64::encode(value[0].begin(), value[0].end(), std::back_inserter(encoded));
        tag_value.put_value(encoded);

        result.add_child("InlineBinary", tag_value);

        return result;
    }

};

std::string get_person_name(boost::property_tree::ptree const & xml)
{
    std::vector<std::string> const fields_name = { "NameSuffix", "NamePrefix",
                                                   "MiddleName", "GivenName",
                                                   "FamilyName" };

    std::map<std::string, std::string> values;
    for(auto it = xml.begin(); it != xml.end(); ++it)
    {
        if (std::find(fields_name.begin(),
                      fields_name.end(), it->first) != fields_name.end())
        {
            auto const value = it->second.get_value<std::string>();

            values.insert(std::pair<std::string, std::string>(it->first, value));
        }
        else
        {
            std::stringstream error;
            error << "Bad sub-Tag '" << it->first
                  << "' for PersonName Tag";
            throw Exception(error.str());
        }
    }

    std::string return_value;
    auto itfield = fields_name.begin();
    do
    {
        std::stringstream current_value;

        std::string value;
        if (values.find(*itfield) != values.end())
        {
            value = values[*itfield];
        }

        ++itfield;

        if (itfield != fields_name.end() && (value != "" || return_value != ""))
        {
            current_value << "^";
        }
        if (value != "")
        {
            current_value << value;
        }
        if (return_value != "")
        {
            current_value << return_value;
        }

        return_value = current_value.str();
    }
    while (itfield != fields_name.end());

    return return_value;
}

template<typename TValueType>
std::map<int, TValueType>
parse_value(boost::property_tree::ptree const & xml, VR const & vr)
{
    std::map<int, TValueType> values;
    for(auto it_value = xml.begin(); it_value != xml.end(); ++it_value)
    {
        if (it_value->first == "<xmlattr>")
        {
            continue;
        }
        else if (it_value->first != "Value")
        {
            std::stringstream error;
            error << "Bad sub-Tag '" << it_value->first
                  << "' for DicomAttribute Tag with VR = "
                  << as_string(vr);
            throw Exception(error.str());
        }

        int const position =
                it_value->second.get<int>("<xmlattr>.number");

        values.insert(std::pair<int, TValueType>(
            position,
            it_value->second.get_value<TValueType>()));
    }

    return values;
}

boost::property_tree::ptree as_xml(DataSet const & data_set)
{
    // XML dataset element
    boost::property_tree::ptree nativedicommodel;
    for(auto const & it: data_set)
    {
        auto const & tag = it.first;
        auto const & element = it.second;

        boost::property_tree::ptree dicomattribute =
                apply_visitor(ToXMLVisitor(), element);

        // Add Mandatory attribute Tag
        dicomattribute.put("<xmlattr>.tag",  std::string(tag));
        // Add Optional attribute Keyword
        auto const dictionary_it = registry::public_dictionary.find(tag);
        if(dictionary_it != registry::public_dictionary.end())
        {
            dicomattribute.put(
                "<xmlattr>.keyword", dictionary_it->second.keyword);
        }
        // Add Optional attribute PrivateCreator
        //dicomattribute.put("<xmlattr>.privateCreator", todo);

        nativedicommodel.add_child("DicomAttribute", dicomattribute);
    }

    // root element
    boost::property_tree::ptree dataset_xml;

    // Add XML dataset into root element
    dataset_xml.add_child("NativeDicomModel", nativedicommodel);

    return dataset_xml;
}

DataSet as_dataset(boost::property_tree::ptree const & xml)
{
    // XML contains only one NativeDicomModel
    // <NativeDicomModel>
    //     ...
    // </NativeDicomModel>
    if (xml.size() < 1 || xml.front().first != "NativeDicomModel")
    {
        throw Exception("Missing root node NativeDicomModel");
    }

    DataSet data_set;

    for(auto it = xml.front().second.begin();
        it != xml.front().second.end(); ++it)
    {
        // NativeDicomModel tag should only contains DicomAttribute tag
        if (it->first != "DicomAttribute")
        {
            throw Exception("Bad DICOM tag: " + it->first);
        }

        // Get VR and tag
        Tag const tag(it->second.get<std::string>("<xmlattr>.tag"));
        VR const vr = as_vr(it->second.get<std::string>("<xmlattr>.vr"));

        Element element;

        if(vr == VR::AE || vr == VR::AS || vr == VR::AT || vr == VR::CS ||
           vr == VR::DA || vr == VR::DT || vr == VR::LO || vr == VR::LT ||
           vr == VR::SH || vr == VR::ST || vr == VR::TM || vr == VR::UI ||
           vr == VR::UT)
        {
            element = Element(Value::Strings(), vr);

            auto values = parse_value<Value::Strings::value_type>(it->second,
                                                                  vr);

            for (auto it = values.begin(); it != values.end(); ++it)
            {
                element.as_string().push_back(it->second);
            }
        }
        else if(vr == VR::PN)
        {
            element = Element(Value::Strings(), vr);

            std::map<int, Value::Strings::value_type> values;
            for(auto it_value = it->second.begin();
                it_value != it->second.end(); ++it_value)
            {
                if (it_value->first == "<xmlattr>")
                {
                    continue;
                }
                else if (it_value->first != "PersonName")
                {
                    std::stringstream error;
                    error << "Bad sub-Tag '" << it_value->first
                          << "' for DicomAttribute Tag with VR = "
                          << as_string(vr);
                    throw Exception(error.str());
                }

                int const position =
                        it_value->second.get<int>("<xmlattr>.number");

                std::map<std::string, std::string> names;
                auto const fields = { "Alphabetic", "Ideographic", "Phonetic" };
                for(auto it_person = it_value->second.begin();
                    it_person != it_value->second.end(); ++it_person)
                {
                    if (it_person->first == "<xmlattr>")
                    {
                        continue;
                    }
                    else if (std::find(fields.begin(),
                                       fields.end(),
                                       it_person->first) != fields.end())
                    {
                        names.insert(std::pair<std::string, std::string>(
                                         it_person->first,
                                         get_person_name(it_person->second)));
                    }
                    else
                    {
                        std::stringstream error;
                        error << "Bad sub-Tag '" << it_person->first
                              << "' for PersonName Tag";
                        throw Exception(error.str());
                    }
                }

                Value::Strings::value_type dicom_item;
                for(auto const & field: fields)
                {
                    if (names.find(field) != names.end())
                    {
                        dicom_item += names[field];
                    }
                    dicom_item += "=";
                }

                while(*dicom_item.rbegin() == '=')
                {
                    dicom_item = dicom_item.substr(0, dicom_item.size()-1);
                }

                values.insert(std::pair<int, Value::Strings::value_type>(
                                  position, dicom_item));
            }

            for (auto it = values.begin(); it != values.end(); ++it)
            {
                element.as_string().push_back(it->second);
            }
        }
        else if(vr == VR::DS || vr == VR::FD || vr == VR::FL)
        {
            element = Element(Value::Reals(), vr);

            auto values = parse_value<Value::Reals::value_type>(it->second, vr);

            for (auto it = values.begin(); it != values.end(); ++it)
            {
                element.as_real().push_back(it->second);
            }
        }
        else if(vr == VR::IS || vr == VR::SL || vr == VR::SS ||
                vr == VR::UL || vr == VR::US)
        {
            element = Element(Value::Integers(), vr);

            auto values = parse_value<Value::Integers::value_type>(it->second,
                                                                   vr);

            for (auto it = values.begin(); it != values.end(); ++it)
            {
                element.as_int().push_back(it->second);
            }
        }
        else if(vr == VR::SQ)
        {
            element = Element(Value::DataSets(), vr);

            std::map<int, Value::DataSets::value_type> values;
            for(auto it_value = it->second.begin();
                it_value != it->second.end(); ++it_value)
            {
                if (it_value->first == "<xmlattr>")
                {
                    continue;
                }
                else if (it_value->first != "Item")
                {
                    std::stringstream error;
                    error << "Bad sub-Tag '" << it_value->first
                          << "' for DicomAttribute Tag with VR = "
                          << as_string(vr);
                    throw Exception(error.str());
                }

                boost::property_tree::ptree nativedicommodel;
                nativedicommodel.insert(nativedicommodel.end(),
                                        it_value->second.begin(),
                                        it_value->second.end());
                nativedicommodel.erase("<xmlattr>");

                boost::property_tree::ptree dataset_xml;
                dataset_xml.add_child("NativeDicomModel", nativedicommodel);

                int const position =
                        it_value->second.get<int>("<xmlattr>.number");

                values.insert(std::pair<int, Value::DataSets::value_type>(
                    position, as_dataset(dataset_xml)));
            }

            for (auto it = values.begin(); it != values.end(); ++it)
            {
                element.as_data_set().push_back(it->second);
            }
        }
        else if(is_binary(vr))
        {
            element = Element(Value::Binary(), vr);

            bool find_inline_binary = false; // only one Tag InlineBinary
            for(auto it_value = it->second.begin();
                it_value != it->second.end(); ++it_value)
            {
                if (it_value->first == "<xmlattr>")
                {
                    continue;
                }
                else if (it_value->first != "InlineBinary")
                {
                    std::stringstream error;
                    error << "Bad sub-Tag '" << it_value->first
                          << "' for DicomAttribute Tag with VR = "
                          << as_string(vr);
                    throw Exception(error.str());
                }
                else if (find_inline_binary)
                {
                    std::stringstream error;
                    error << "Too many sub-Tag '" << it_value->first
                          << "' for DicomAttribute Tag with VR = "
                          << as_string(vr);
                    throw Exception(error.str());
                }

                auto const & encoded = it_value->second.get_value<std::string>();
                // cf. ToXMLVisitor::operator()(VR, Value::Binary): InlineBinary
                // is single-valued
                auto & decoded = element.as_binary();
                decoded.resize(1);
                decoded[0].reserve(encoded.size()*3/4);
                base64::decode(
                    encoded.begin(), encoded.end(),
                    std::back_inserter(decoded[0]));

                find_inline_binary = true;
            }
        }
        else
        {
            throw Exception("Unknown VR: "+as_string(vr));
        }

        data_set.add(tag, element);
    }

    return data_set;
}

} // namespace odil

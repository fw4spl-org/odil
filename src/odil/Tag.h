/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _5faf4691_e936_476e_8ad3_40f36a167a74
#define _5faf4691_e936_476e_8ad3_40f36a167a74

#include <cstddef>
#include <ostream>
#include <string>

#include "odil/odil.h"

namespace odil
{

/**
 * @brief A DICOM element tag.
 */
class ODIL_API Tag
{
public:
    /// @brief Create a tag based on its group and element as two 16-bits words.
    Tag(uint16_t group, uint16_t element);

    /// @brief Create a tag based on its group and element as one 32-bits word.
    Tag(uint32_t tag=0);

    /**
     * @brief Create a tag based on its name or string representation of its
     * numeric value.
     *
     * If the name cannot be found in the public data dictionary, or if the
     * string is not the representation of a numeric value, a odil::Exception
     * is raised.
     */
    Tag(std::string const & string);

    /**
     * @brief Create a tag based on its name or string representation of its
     * numeric value.
     *
     * If the name cannot be found in the public data dictionary, or if the
     * string is not the representation of a numeric value, a odil::Exception
     * is raised.
     */
    Tag(char const * string);

    /** @addtogroup default_operations Default class operations
     * @{
     */
    ~Tag() =default;
    Tag(Tag const &) =default;
    Tag(Tag &&) =default;
    Tag & operator=(Tag const &) =default;
    Tag & operator=(Tag &&) =default;
    /// @}

    /// @brief Group of the tag.
    uint16_t group;

    /// @brief Element of the tag.
    uint16_t element;

    /// @brief Test whether the tag is private.
    bool is_private() const;

    /**
     * @brief Return the name of the tag.
     *
     * If the tag cannot be found in the public data dictionary,
     * a odil::Exception is raised.
     */
    std::string get_name() const;

    /// @brief Equality test.
    bool operator==(Tag const & other) const;

    /// @brief Difference test.
    bool operator!=(Tag const & other) const;

    /// @brief Strict inferiority test.
    bool operator<(Tag const & other) const;

    /// @brief Strict superiority test.
    bool operator>(Tag const & other) const;

    /// @brief Loose inferiority test.
    bool operator<=(Tag const & other) const;

    /// @brief Loose superiority test.
    bool operator>=(Tag const & other) const;

    /// @brief Convert to string
    operator std::string() const;

private:
    void _from_string(std::string const & string);
};

/// @brief Stream inserter
ODIL_API std::ostream & operator<<(std::ostream & stream, Tag const & tag);

}

#endif // _5faf4691_e936_476e_8ad3_40f36a167a74

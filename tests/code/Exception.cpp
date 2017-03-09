#define BOOST_TEST_MODULE Exception
#include <boost/test/included/unit_test.hpp>

#include "odil/Exception.h"

BOOST_AUTO_TEST_CASE(Exception)
{
    odil::Exception const exception("foo");
    BOOST_REQUIRE_EQUAL(exception.what(), "foo");
}

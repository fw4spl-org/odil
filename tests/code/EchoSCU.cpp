#define BOOST_TEST_MODULE EchoSCU
#include <boost/test/included/unit_test.hpp>

#include "odil/EchoSCU.h"
#include "odil/registry.h"

#include "../PeerFixtureBase.h"

struct Fixture: public PeerFixtureBase
{
    Fixture()
    : PeerFixtureBase({
        {
            1, odil::registry::VerificationSOPClass,
            {odil::registry::ImplicitVRLittleEndian}, true, false
        }
    })
    {
        // Nothing else
    }
};

BOOST_FIXTURE_TEST_CASE(Echo, Fixture)
{
    odil::EchoSCU scu(this->association);
    scu.echo();
}


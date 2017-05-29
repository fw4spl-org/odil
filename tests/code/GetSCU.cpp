#define BOOST_TEST_MODULE GetSCU
#include <boost/test/included/unit_test.hpp>

#include "odil/DataSet.h"
#include "odil/GetSCU.h"
#include "odil/registry.h"

#include "../PeerFixtureBase.h"

struct Fixture: public PeerFixtureBase
{
    static bool store_callback_called;
    static bool get_callback_called;

    odil::DataSet query;

    Fixture()
    : PeerFixtureBase({
            {
                1, odil::registry::PatientRootQueryRetrieveInformationModelGET,
                { odil::registry::ImplicitVRLittleEndian }, true, false
            },
            {
                3, odil::registry::RawDataStorage,
                { odil::registry::ImplicitVRLittleEndian }, false, true
            }
        })
    {
        Fixture::store_callback_called = false;
        Fixture::get_callback_called = false;

        this->query.add("QueryRetrieveLevel", {"PATIENT"});
        this->query.add("PatientName", {"Doe^John"});
    }


    static void store_callback(odil::DataSet const &)
    {
        Fixture::store_callback_called = true;
    }

    static void get_callback(odil::message::CGetResponse const &)
    {
        Fixture::get_callback_called = true;
    }
};

bool Fixture::store_callback_called = false;
bool Fixture::get_callback_called = false;

BOOST_FIXTURE_TEST_CASE(Get, Fixture)
{
    odil::GetSCU scu(this->association);

    scu.set_affected_sop_class(
        odil::registry::PatientRootQueryRetrieveInformationModelGET);
    auto const results = scu.get(this->query);

    BOOST_REQUIRE_EQUAL(results.size(), 1);
    BOOST_CHECK(
        results[0].as_string("SOPInstanceUID") ==
            odil::Value::Strings(
                {"2.25.95090344942250266709587559073467305647"}));
}

BOOST_FIXTURE_TEST_CASE(GetMove, Fixture)
{
    odil::GetSCU scu(this->association);

    scu.set_affected_sop_class(
        odil::registry::PatientRootQueryRetrieveInformationModelGET);
    auto const results = scu.get(std::move(this->query));

    BOOST_REQUIRE_EQUAL(results.size(), 1);
    BOOST_CHECK(
        results[0].as_string("SOPInstanceUID") ==
            odil::Value::Strings(
                {"2.25.95090344942250266709587559073467305647"}));
}

BOOST_FIXTURE_TEST_CASE(GetBothCallbacks, Fixture)
{
    odil::GetSCU scu(this->association);

    scu.set_affected_sop_class(
        odil::registry::PatientRootQueryRetrieveInformationModelGET);
    scu.get(this->query, Fixture::store_callback, Fixture::get_callback);

    BOOST_CHECK(Fixture::store_callback_called);
    BOOST_CHECK(Fixture::get_callback_called);
}

BOOST_FIXTURE_TEST_CASE(GetBothCallbacksMove, Fixture)
{
    odil::GetSCU scu(this->association);

    scu.set_affected_sop_class(
        odil::registry::PatientRootQueryRetrieveInformationModelGET);
    scu.get(
        std::move(this->query), Fixture::store_callback, Fixture::get_callback);

    BOOST_CHECK(Fixture::store_callback_called);
    BOOST_CHECK(Fixture::get_callback_called);
}

BOOST_FIXTURE_TEST_CASE(GetOnlyStoreCallback, Fixture)
{
    odil::GetSCU scu(this->association);

    scu.set_affected_sop_class(
        odil::registry::PatientRootQueryRetrieveInformationModelGET);
    scu.get(this->query, Fixture::store_callback);

    BOOST_CHECK(Fixture::store_callback_called);
    BOOST_CHECK(!Fixture::get_callback_called);
}

BOOST_FIXTURE_TEST_CASE(GetOnlyStoreCallbackMove, Fixture)
{
    odil::GetSCU scu(this->association);

    scu.set_affected_sop_class(
        odil::registry::PatientRootQueryRetrieveInformationModelGET);
    scu.get(std::move(this->query), Fixture::store_callback);

    BOOST_CHECK(Fixture::store_callback_called);
    BOOST_CHECK(!Fixture::get_callback_called);
}

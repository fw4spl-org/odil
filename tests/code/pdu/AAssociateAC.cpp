#define BOOST_TEST_MODULE AAssociateAC
#include <boost/test/included/unit_test.hpp>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "odil/pdu/AAssociateAC.h"
#include "odil/pdu/ApplicationContext.h"
#include "odil/pdu/PresentationContextAC.h"
#include "odil/pdu/UserIdentityAC.h"
#include "odil/pdu/UserInformation.h"
#include "odil/Exception.h"

struct Fixture
{
public:
    static std::string const read_data;
    static std::string const write_data;

    odil::pdu::ApplicationContext application_context;
    std::vector<odil::pdu::PresentationContextAC> presentation_contexts;
    odil::pdu::UserInformation user_information;

    Fixture()
    : application_context("foo")
    {
        odil::pdu::PresentationContextAC pc1(3, "transfer_syntax", 1);
        odil::pdu::PresentationContextAC pc2(5, "transfer_syntax_2", 2);
        this->presentation_contexts = {pc1, pc2};

        this->user_information.set_sub_items<odil::pdu::MaximumLength>(
            { { 0x12345678 } });
        this->user_information.set_sub_items<odil::pdu::UserIdentityAC>(
            { { "bar" } });
    }

    void check_application_context(
        odil::pdu::ApplicationContext const & context) const
    {
        BOOST_REQUIRE_EQUAL(
            context.get_name(), this->application_context.get_name());
    }

    void check_presentation_contexts(
        std::vector<odil::pdu::PresentationContextAC> const & contexts)
    {
        BOOST_REQUIRE_EQUAL(contexts.size(), presentation_contexts.size());
        for(int i=0; i<contexts.size(); ++i)
        {
            BOOST_REQUIRE_EQUAL(
                contexts[i].get_item_type(),
                presentation_contexts[i].get_item_type());
            BOOST_REQUIRE_EQUAL(
                contexts[i].get_id(),
                presentation_contexts[i].get_id());
            BOOST_REQUIRE_EQUAL(
                contexts[i].get_result_reason(),
                presentation_contexts[i].get_result_reason());
        }
    }

    void check_user_information(
        odil::pdu::UserInformation const & user_information) const
    {
        BOOST_REQUIRE(
            !user_information.get_sub_items<odil::pdu::MaximumLength>().empty());
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::pdu::MaximumLength>()[0].get_maximum_length(),
            0x12345678);

        BOOST_REQUIRE(
            !user_information.get_sub_items<odil::pdu::UserIdentityAC>().empty());
        BOOST_REQUIRE_EQUAL(
            user_information.get_sub_items<odil::pdu::UserIdentityAC>()[0].get_server_response(),
            "bar");
    }
};

std::string const
Fixture
::read_data(
    // Header, 6 bytes

    "\x02\x00"
    "\x00\x00\x00\x98"

    // Items: 68 bytes

    "\x00\x01\x00\x00"
    "       CALLED_AE"
    "CALLING_AE      "
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

    // Application Context, 7 bytes

    "\x10\x00\x00\x03"
    "foo"

    // Presentation Contexts, 27+29 bytes

    "\x21\x00\x00\x17"
    "\x03\x00\x01\x00"
    "\x40\x00\x00\x0f""transfer_syntax"

    "\x21\x00\x00\x19"
    "\x05\x00\x02\x00"
    "\x40\x00\x00\x11""transfer_syntax_2"

    // User Information, 21 bytes

    "\x50\x00\x00\x11"
    "\x51\x00\x00\x04"
    "\x12\x34\x56\x78"
    "\x59\x00\x00\x05"
    "\x00\x03" "bar"
    ,
    158
);

std::string const
Fixture
::write_data(
    // Header, 6 bytes

    "\x02\x00"
    "\x00\x00\x00\x98"

    // Items: 68 bytes

    "\x00\x01\x00\x00"
    "CALLED_AE       "
    "CALLING_AE      "
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

    // Application Context, 7 bytes

    "\x10\x00\x00\x03"
    "foo"

    // Presentation Contexts, 27+29 bytes

    "\x21\x00\x00\x17"
    "\x03\x00\x01\x00"
    "\x40\x00\x00\x0f""transfer_syntax"

    "\x21\x00\x00\x19"
    "\x05\x00\x02\x00"
    "\x40\x00\x00\x11""transfer_syntax_2"

    // User Information, 21 bytes

    "\x50\x00\x00\x11"
    "\x51\x00\x00\x04"
    "\x12\x34\x56\x78"
    "\x59\x00\x00\x05"
    "\x00\x03" "bar"
    ,
    158
);

BOOST_AUTO_TEST_CASE(Constructor)
{
    odil::pdu::AAssociateAC const pdu;

    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "");
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "");
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 0);
    BOOST_REQUIRE_THROW(pdu.get_application_context(), odil::Exception);
    BOOST_REQUIRE(pdu.get_presentation_contexts().empty());
    BOOST_REQUIRE_THROW(pdu.get_user_information(), odil::Exception);
}

BOOST_FIXTURE_TEST_CASE(ConstructorStream, Fixture)
{
    std::istringstream stream(read_data);
    odil::pdu::AAssociateAC const pdu(stream);

    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "CALLED_AE");
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "CALLING_AE");
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 1);
    this->check_application_context(pdu.get_application_context());
    this->check_presentation_contexts(pdu.get_presentation_contexts());
    this->check_user_information(pdu.get_user_information());
}

BOOST_AUTO_TEST_CASE(ProtocolVersion)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 0);
    pdu.set_protocol_version(2);
    BOOST_REQUIRE_EQUAL(pdu.get_protocol_version(), 2);
}

BOOST_AUTO_TEST_CASE(CalledAETitle)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "");
    pdu.set_called_ae_title("called");
    BOOST_REQUIRE_EQUAL(pdu.get_called_ae_title(), "called");
}

BOOST_AUTO_TEST_CASE(CallingAETitle)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "");
    pdu.set_calling_ae_title("calling");
    BOOST_REQUIRE_EQUAL(pdu.get_calling_ae_title(), "calling");
}

BOOST_FIXTURE_TEST_CASE(ApplicationContext, Fixture)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_THROW(pdu.get_application_context(), odil::Exception);
    pdu.set_application_context(application_context);
    this->check_application_context(pdu.get_application_context());
}

BOOST_FIXTURE_TEST_CASE(PresentationContexts, Fixture)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE(pdu.get_presentation_contexts().empty());
    pdu.set_presentation_contexts(presentation_contexts);
    this->check_presentation_contexts(pdu.get_presentation_contexts());
}

BOOST_FIXTURE_TEST_CASE(UserInformation, Fixture)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_THROW(pdu.get_user_information(), odil::Exception);
    pdu.set_user_information(user_information);
    this->check_user_information(pdu.get_user_information());
}

BOOST_FIXTURE_TEST_CASE(Write, Fixture)
{
    odil::pdu::AAssociateAC pdu;
    pdu.set_protocol_version(1);
    pdu.set_called_ae_title("CALLED_AE");
    pdu.set_calling_ae_title("CALLING_AE");

    pdu.set_application_context(application_context);
    pdu.set_presentation_contexts(presentation_contexts);
    pdu.set_user_information(user_information);

    std::ostringstream stream;
    stream << pdu;

    BOOST_REQUIRE(stream.str() == write_data);
}

BOOST_AUTO_TEST_CASE(CalledAETitleEmpty)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_THROW(pdu.set_called_ae_title(""), odil::Exception);
}

BOOST_AUTO_TEST_CASE(CalledAETitleTooLong)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_THROW(
        pdu.set_called_ae_title("123456789abcdef01"), odil::Exception);
}

BOOST_AUTO_TEST_CASE(CallingAETitleEmpty)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_THROW(pdu.set_calling_ae_title(""), odil::Exception);
}

BOOST_AUTO_TEST_CASE(CallingAETitleTooLong)
{
    odil::pdu::AAssociateAC pdu;
    BOOST_REQUIRE_THROW(
        pdu.set_calling_ae_title("123456789abcdef01"), odil::Exception);
}

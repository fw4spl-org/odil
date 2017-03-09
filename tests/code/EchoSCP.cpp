#define BOOST_TEST_MODULE EchoSCP
#include <boost/test/included/unit_test.hpp>

#include <chrono>
#include <cstdlib>
#include <thread>

#include <boost/asio.hpp>

#include "odil/Association.h"
#include "odil/EchoSCP.h"
#include "odil/Exception.h"
#include "odil/message/CEchoRequest.h"
#include "odil/message/Response.h"

struct Status
{
    int client;
    std::string server;
    bool called;
};

void run_server(Status * status)
{
    odil::Association association;
    association.set_tcp_timeout(boost::posix_time::seconds(1));

    try
    {
        association.receive_association(boost::asio::ip::tcp::v4(), 11113);

        odil::EchoSCP echo_scp(association,
            [status](odil::message::CEchoRequest const &)
            {
                status->called = true;
                return odil::message::Response::Success;
            });

        // Get echo message
        auto const message = association.receive_message();
        echo_scp(message);
        // Should throw with peer closing connection
        association.receive_message();
    }
    catch(odil::AssociationAborted const &)
    {
        status->server = "abort";
    }
    catch(odil::AssociationReleased const &)
    {
        status->server = "release";
    }
    catch(odil::Exception const &)
    {
        status->server = "Other Odil exception";
    }
    catch(...)
    {
        status->server = "Other exception";
    }
}

void run_client(Status * status, bool use_abort)
{
    std::string command = "echoscu -ll error";
    if(use_abort)
    {
        command += " --abort";
    }
    command += " 127.0.0.1 11113";
    status->client = system(command.c_str());
}

BOOST_AUTO_TEST_CASE(Callback)
{
    odil::Association association;
    odil::EchoSCP scp(association);

    bool called = false;
    auto const callback =
        [&called](odil::message::CEchoRequest const &)
        {
            called = true;
            return odil::message::Response::Success;
        };

    scp.set_callback(callback);
    scp.get_callback()(odil::message::CEchoRequest(1, ""));
    BOOST_REQUIRE_EQUAL(called, true);
}

BOOST_AUTO_TEST_CASE(Release)
{
    Status status = { -1, "", false };

    std::thread server(run_server, &status);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::thread client(run_client, &status, false);

    server.join();
    client.join();

    BOOST_REQUIRE_EQUAL(status.client, 0);
    BOOST_REQUIRE_EQUAL(status.server, "release");
    BOOST_REQUIRE_EQUAL(status.called, true);
}

BOOST_AUTO_TEST_CASE(Abort)
{
    Status status = { -1, "", false };

    std::thread server(run_server, &status);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::thread client(run_client, &status, true);

    server.join();
    client.join();

    BOOST_REQUIRE_EQUAL(status.client, 0);
    BOOST_REQUIRE_EQUAL(status.server, "abort");
    BOOST_REQUIRE_EQUAL(status.called, true);
}

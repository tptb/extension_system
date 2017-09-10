/**
    @file
    @copyright
        Copyright Bernd Amend and Michael Adam 2014-2017
        Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt)
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Interfaces.hpp"
#include <extension_system/ExtensionSystem.hpp>

using namespace extension_system;

TEST_CASE("test if the test file can be loaded")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.setVerifyCompiler(false);
    extensionSystem.addDynamicLibrary("dummy_test_extension");
    auto e = extensionSystem.extensions();
    INFO(messages);
    REQUIRE(e.size() == 1);
    CHECK(e[0].getExtended()["compiler"] == "test");
    CHECK(e[0].getExtended()["compiler_version"] == "1");
    CHECK(e[0].name() == "ext_name");
    CHECK(e[0].interface_name() == "ext_interface");
    CHECK(e[0].description() == "extension");
}

TEST_CASE("all expected extensions were found")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.searchDirectory(".", true);
    auto e = extensionSystem.extensions();
    INFO(messages);
    CHECK(e.size() == 5);

    for (const auto& i : e) {
        if (i.name() == "Example2Extension") {
            CHECK(i.interface_name() == "Interface2");
            CHECK(i.description() == "Example 2 extension");
            CHECK(i.version() == 100);
        } else if (i.name() == "Example1Extension") {
            CHECK(i.interface_name() == "Interface1");
            CHECK(i.description() == "Example 1 extension");
            CHECK(i.version() == 100);
        } else if (i.name() == "Ext1" && (i.version() == 100 || i.version() == 110)) {
            CHECK(i.interface_name() == "IExt1");
            if (i.version() == 100) {
                CHECK(i.description() == "extension 1 for testing purposes");
            } else {
                CHECK(i.description() == "extension 2 for testing purposes");
            }
        } else if (i.name() == "Ext2") {
            CHECK(i.interface_name() == "extension_system::IExt2");
            CHECK(i.description() == "extension 3 for testing purposes");
        } else {
            CHECK(false); // unexpected extension
        }
    }
}

TEST_CASE("all expected extensions with a given interface were found")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.searchDirectory(".", true);
    auto e = extensionSystem.extensions<IExt1>();
    INFO(messages);
    CHECK(e.size() == 2);

    for (const auto& i : e) {
        CHECK(i.interface_name() == "IExt1");
    }
}

TEST_CASE("load extension by name")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.searchDirectory(".", true);

    auto e = extensionSystem.createExtension<IExt1>("Ext1");

    INFO(messages);

    REQUIRE(e != nullptr);

    CHECK(e->test1() == 21);

    auto desc = extensionSystem.findDescription(e);
    CHECK(desc.isValid());
    CHECK(desc.version() == 110);
}

TEST_CASE("load extension by name and version")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.searchDirectory(".", true);

    auto e = extensionSystem.createExtension<IExt1>("Ext1", 100);

    INFO(messages);

    REQUIRE(e != nullptr);

    CHECK(e->test1() == 42);

    auto desc = extensionSystem.findDescription(e);
    CHECK(desc.isValid());
    CHECK(desc.version() == 100);
}

TEST_CASE("load extension by name 2")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.searchDirectory(".", true);

    auto e = extensionSystem.createExtension<IExt2>("Ext2");

    INFO(messages);

    REQUIRE(e != nullptr);

    CHECK_THAT(e->test2(), Catch::Matchers::Equals("Hello from Ext2"));

    auto desc = extensionSystem.findDescription(e);
    CHECK(desc.isValid());
    CHECK(desc.version() == 100);
}

#if 0
TEST_CASE("check if filter work as expected")
{
    std::string     messages;
    ExtensionSystem extensionSystem;
    extensionSystem.setEnableDebugOutput(true);
    extensionSystem.setMessageHandler([&](const std::string& msg) { messages += msg + "\n"; });
    extensionSystem.searchDirectory(".", true);

    auto filteredExtensions = extensionSystem.extensions({{"Test1", "desc1"}, {"Test1", "desc2"}, {"Test3", "desc3"}});
    INFO(messages);
    for (const auto& i : filteredExtensions) {
        auto e4 = extensionSystem.createExtension<IExt2>(i);
        if (e4 != nullptr) {
            std::cout << i.toString() << "\n";
            e4->test2();
        } else {
            std::cout << "Wrong interface:\n" << i.toString() << "\n";
        }
    }
}
#endif

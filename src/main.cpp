#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "nlohmann/json.hpp"

using namespace std;

using json_t = nlohmann::json;

int main(int argc, const char* argv[])
{
    std::cout << "hello " << __cplusplus << std::endl;

    auto console = spdlog::stdout_color_mt("console");

    SPDLOG_LOGGER_INFO(console, "{}.{}.{}.{}", 192, 168, 1, 1);

    auto j = json_t::parse(R"({"happy": true, "pi": 3.141})");

    SPDLOG_LOGGER_INFO(console, "{}", j["happy"].dump());

    return 0;
}
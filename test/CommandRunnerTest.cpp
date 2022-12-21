#include "getopt.h"
#include "CommandRunner.h"

int main(int argc, char* argv[])
{
    CLogger::SetLogConfig("log/CommandRunner.log", 30, 1048576);
    std::shared_ptr<spdlog::logger> g_logger =
        CLogger::GetInstance()->CreateLogger("Main");
    g_logger->info("Starting Command Runner");

    int opt;
    bool parseOnly = false;
    bool verbosity = false;
    while ((opt = getopt(argc, argv, "pv")) != -1) {
        switch (opt) {
        case 'v':
            verbosity = true;
            CLogger::GetInstance()->SetLogLevel(spdlog::level::debug);
            g_logger->set_level(spdlog::level::debug);
            g_logger->debug("Verbosity is on");
            break;
        case 'p':
            parseOnly = true;
            g_logger->debug("Parse-only mode");
            break;
        default:
            g_logger->error("Usage: {} [-p|-v] command-spec-file", argv[0]);
            ::exit(EXIT_FAILURE);
        }
        optind++;
    }
    if (optind >= argc) {
        g_logger->error("Expected argument after options");
        ::exit(EXIT_FAILURE);
    }

    CommandRunner::GetInstance()->ParseSpecFile(argv[optind]);
    if (!parseOnly)
        CommandRunner::GetInstance()->executeSequence();
    g_logger->info("Command Runner is done");
}
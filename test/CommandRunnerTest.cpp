#include "getopt.h"
#include "CommandRunner.h"

std::shared_ptr<spdlog::logger> g_logger = NULL;

void usage(string exec_path) { g_logger->error("Usage: {} [-p|-v] command-spec-file", exec_path); }

int main(int argc, char* argv[])
{
    CLogger::SetLogConfig("log/CommandRunner.log", 30, 1048576);
    g_logger = CLogger::GetInstance()->CreateLogger("Main");
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
            usage(argv[0]);
            ::exit(EXIT_FAILURE);
        }
        optind++;
    }
    if (optind >= argc) {
        g_logger->error("Expected argument after options");
        usage(argv[0]);
        ::exit(EXIT_FAILURE);
    }

    if (int parseResult = CommandRunner::GetInstance()->ParseSpecFile(argv[optind]))
        return parseResult;

    if (!parseOnly)
        CommandRunner::GetInstance()->executeSequence();
    g_logger->info("Command Runner is done");
}
#include <tnt/tntnet.h>
#include <tntdb.h>

#include "web_logmachine.h"

#include <logger.h>
#include <config.h>

#include <templater.h>
#include <csignal>

void exit_work(const char* source)
{
    log_critical() << "WORK SOURCE:" << source;
}

void exit_on_error()
{
    exit_work("exit_on_error");
}

void signal_handler(int signum)
{
    log_warning() << "Signal caught " << signum;
    exit_work("signal_handler");
    _Exit(1);
}

void atexit_handler()
{
    std::cerr << "Atexit handler" <<  std::endl;
    exit_work("atexit_handler");
}

int main(int argc, char* argv[])
{
    std::cout << "thymian 0.1 starting" << std::endl;
    (void)argc;(void)argv;

    unafrog::logger::instance().add_log_machine(new web_logmachine);

    info() << "--------------------- App starting [PID:" << getpid() << "] ---------------------";

    std::set_unexpected (exit_on_error);
    std::set_terminate(exit_on_error);
    atexit(atexit_handler);

    struct sigaction new_action, old_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = signal_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction (SIGINT, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction (SIGINT, &new_action, nullptr);
    }

    sigaction (SIGHUP, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction (SIGHUP, &new_action, nullptr);
    }

    sigaction (SIGTERM, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction (SIGTERM, &new_action, nullptr);
    }

    sigaction (SIGSEGV, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction (SIGSEGV, &new_action, nullptr);
    }

    sigaction (SIGKILL, nullptr, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction (SIGKILL, &new_action, nullptr);
    }

    srand(static_cast<unsigned int>(::time(nullptr)));

    try
    {

        if(!config::instance().valid())
        {
            log_critical() << "configuration failure, exiting";
            return 1;
        }

        // check that all the templates are where they are supposed to be
        if(!template_warehouse::instance().checkTemplates())
        {
            log_critical() << "Cannot find some templates. Giving up";
            return 1;
        }

        // start the web server
        tnt::Tntnet app;
        app.listen(8999);

        auto pngMap = app.mapUrl("^/img/(.*).png$", "filer"); // the supported images
        auto filer1 = app.mapUrl("^/img/(.*).ico$", "filer");
        auto filer2 = app.mapUrl("^/img/(.*).gif$", "filer");
        auto filer3 = app.mapUrl("^/img/(.*).jpg$", "filer");
        auto filer10Global = app.mapUrl("/(.*).js$", "filer");    // javascript handler

        auto pngMapCurrentTheme = app.mapUrl("^/theme/current/img/(.*).png$", "filer"); // the supported images from the current theme
        auto filer1CurrentTheme = app.mapUrl("^/theme/current/img/(.*).ico$", "filer");
        auto filer2CurrentTheme = app.mapUrl("^/theme/current/img/(.*).gif$", "filer");
        auto filer3CurrentTheme = app.mapUrl("^/theme/current/img/(.*).jpg$", "filer");
        auto filer10 = app.mapUrl("^/theme/current/(.*).js$", "filer");    // javascript handler for current theme
        auto filer10scriptDirTheme = app.mapUrl("^/theme/current/script/(.*).js$", "filer");    // javascript handler for current theme
        auto filer12CurrentTheme = app.mapUrl("/theme/current/(.*).css$", "filer");      // the css files regardless ofwhere they are
        auto filer12DirTheme = app.mapUrl("/theme/current/css/(.*).css$", "filer");      // the css files regardless ofwhere they are

        auto filer4 = app.mapUrl("/(.*).png$", "filer");
        auto filer5 = app.mapUrl("/(.*).ico$", "filer");
        auto filer6 = app.mapUrl("/(.*).gif$", "filer");
        auto filer7 = app.mapUrl("/(.*).jpg$", "filer");
        auto filer8 = app.mapUrl("/(.*).html$", "filer");
        auto filer9 = app.mapUrl("/(.*).htm$", "filer");
        auto filer10ScriptDirGlobal = app.mapUrl("/js/(.*).js$", "filer");    // javascript handler
        auto filer12 = app.mapUrl("/(.*).css$", "filer");      // the css files regardless ofwhere they are
        auto filer12StyleDirGlobal = app.mapUrl("/css/(.*).css$", "filer");      // the css files in a global directory
        auto filer13 = app.mapUrl("/robots.txt$", "filer");

        auto homepage = app.mapUrl("^/$", "root"); // / (ie: homepage)

        auto langNo = app.mapUrl("^/no$", "main_root_sender"); // / (ie: homepage in norwegian)
        auto langGb = app.mapUrl("^/gb$", "main_root_sender"); // / (ie: homepage in english)
        auto langRo = app.mapUrl("^/ro$", "main_root_sender"); // / (ie: homepage in romanian)
        auto langHu = app.mapUrl("^/hu$", "main_root_sender"); // / (ie: homepage in hungarian)


        app.run();
    }
    catch (const std::exception& e)
    {
        emergency() << e.what();
        exit(1);
    }
    log_critical() << "bye bye cruel world";
    exit(0);
}

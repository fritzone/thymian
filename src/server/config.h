#ifndef CONFIG
#define CONFIG

#include <stdint.h>

class config final
{
public:

    config();

    static config& instance();

    bool valid() const {return m_valid;}

    struct c_flood final
    {
        // How many times a host can request from us in a second before we decide that it is flooding us.
        int max_offense_per_second  = 100;

        // How many times do we accept a flooding offense/second before we consider to put the host on a ban list
        int max_accepted_offenses   = 10;

        //how long time must pass after the first offense in order to really put the host on ban list
        double max_offense_time     = 60.0;

        // the default time an offenser gets kicked out, 1 minute
        int default_suspension_time = 60;
    };

    struct c_server final
    {
        // how many seconds do we allow for a host to be invisible before we switch it'sstatus to red on the dashboard
        int max_offline_seconds_allowed = 10;

        // how many seconds do we allow for waiting for a client to appear before showing an error message
        int max_client_notconnected_seconds_allowed = 60;

        // how many seconds do we allow for waiting for a client to appear before showing an error message
        int max_client_grace_seconds = 60;

        // how many extra grace seconds can a client get
        int max_extra_grace_seconds = 60;

        // how many seconds when we check in the pending query handling if a host has checked out or not
        int host_logout_check_seconds = 5;

        // how many grace seconds do we allow to a host when dealing with query
        int host_grace_time_extension = 10;

        // how many seconds must pass before we consider the host to be offline
        int host_offline_confirmed_seconds = 360;

        // how many seconds must pass before we suspect the host to be offline
        int host_offline_suspect_seconds = 10;

        // initially we support 50MB as download
        uint64_t max_download_support = 52428800;
    };

public:

    c_flood flood;
    c_server server;

private:

    bool m_valid;
};

#endif // CONFIG


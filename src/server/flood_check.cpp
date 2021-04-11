#include "flood_check.h"

#include <config.h>

#include <stdint.h>
#include <log.h>
#include <sstream>

std::mutex flood_check::locker;

std::map<std::time_t, flood_check::flood_map> flood_check::floods;
std::map<std::string, flood_check::count_started> flood_check::hostd_flood;
std::map<std::string, int> flood_check::suspension_times;
std::map<std::string, std::time_t> flood_check::rejected_hosts;

void flood_check::attempt(std::string ip)
{
    std::lock_guard<std::mutex> guard(locker);

    std::chrono::time_point<std::chrono::system_clock> p2;
    p2 = std::chrono::system_clock::now();
    std::time_t c = std::chrono::duration_cast<std::chrono::seconds>(p2.time_since_epoch()).count();
    static std::time_t last_second = 0;

    // see if this host is in the kicked out hosts map and if his sentence is still valid or not
    if(rejected_hosts.count(ip) > 0)
    {
        if(suspension_times.count(ip) > 0)
        {
            // sentence valid, kick him out
            auto timediff = std::difftime(rejected_hosts[ip], c);
            if(timediff <= suspension_times[ip] * config::instance().flood.default_suspension_time && timediff >= 0)
            {
                // debug() << "IP:" << ip << " still rejected for " << timediff << " seconds";
                // debug() << "LEAVE rejected throw";

                throw std::runtime_error("rejected");
            }
            if(timediff < 0)
            {
                // sentence invalid, host has served ts sentence, however we are suspicious.
                if(abs(timediff) < 5) // if he tries again after 5 seconds of the expiration time
                {
                    // kick him out again, with greater penalty
                    rejected_hosts[ip] = c + ( ++suspension_times[ip] * config::instance().flood.default_suspension_time );
                    // debug() << "IP:" << ip << " rejection increased for " << rejected_hosts[ip] - c << " seconds since it tried between " << abs(timediff) << " seconds";
                }
                else
                {
                    // debug() << "IP:" << ip << " lifting rejection: timediff=" << abs(timediff);
                    // let's hope his intentions are honest for now and forgive him by erasing his sin
                    suspension_times.erase(ip);
                    hostd_flood.erase(ip);
                    rejected_hosts.erase(ip);
                }
            }
            else
            {
                // debug() << "IP:" << ip << " is strangely behaving: " << timediff << " seconds, rejecting regardless";
                throw std::runtime_error("rejected");
            }
        }
        else
        {
            // we shouldn't really be here
            log_critical() << "IP:" << ip << "has sentence, however no suspension counter";
            suspension_times.erase(ip);
            hostd_flood.erase(ip);
            rejected_hosts.erase(ip);
        }
    }
    else
    {
        // debug() << "IP:" << ip << " not in rejected_hosts";
    }

    std::string key = std::to_string(static_cast<uint64_t>(c)) + "_" + ip;

    if(floods.count(c) > 0)
    {
        flood_map& cfm = floods[c];
        if(cfm.count(key) > 0)
        {
            auto& flood_value = ++ cfm[key];
            // now check if flood_value > 100 and do something about it
            if(flood_value > config::instance().flood.max_offense_per_second)
            {
                debug() << "IP:" << ip << " tries to flood";
                if(hostd_flood.count(ip) > 0)
                {
                    auto& flood_cnt = ++ hostd_flood[ip].count;
                    debug() << "IP:" << ip << " tries to flood: " << flood_cnt;

                    if(flood_cnt > config::instance().flood.max_accepted_offenses)
                    {
                        auto tdiff = std::difftime(c, hostd_flood[ip].first_time);
                        if(tdiff < config::instance().flood.max_offense_time)
                        {
                            // this host has tried to flood our system at least 10 times during the last minute, suspend him
                            if(suspension_times.count(ip) > 0)
                            {
                                rejected_hosts.emplace(ip, c + ( ++suspension_times[ip] * config::instance().flood.default_suspension_time ));
                                debug() << "IP:" << ip << " rejected for " << rejected_hosts[ip] - c << " seconds";
                            }
                            else
                            {
                                debug() << "IP:" << ip << "mandated for rejection since:" << suspension_times.count(ip);
                                suspension_times.emplace(ip, 0);
                            }
                        }
                        else
                        {
                            // first offence happened long time ago, time to set it up again, don't reset count since we don't like flooding us
                            hostd_flood[ip].first_time = c;
                            debug() << "IP:" << ip << " tries to flood but too long time has passed:" << tdiff;
                        }
                    }
                    else
                    {
                        debug() << "IP:" << ip << " tries to flood but hasn't got enough flood_cnt: " << flood_cnt;
                    }
                }
                else
                {
                    hostd_flood.emplace(ip, count_started());
                }
                debug() << "LEAVE flood throw";
                throw std::runtime_error("flood");
            }
        }
        else
        {
            cfm[key] = 1;
        }
    }
    else
    {
        // create a new entry
        flood_map fm;
        fm[key] = 1;
        floods.emplace(c, fm);
    }

    // just debug:
    for(const auto& k : rejected_hosts)
    {
        std::stringstream ss;
        ss << "rejected IP:" << k.first << " time:" << k.second - c;
        // debug()  << ss.str();
    }

    // now remove the previous seconds' attempts from the memory
    if(last_second != c)
    {
        if(floods.count(last_second) > 0)
        {
            floods.erase(last_second);
        }
        last_second = c;
    }
}

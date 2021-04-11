#ifndef FLOOD_CHECK
#define FLOOD_CHECK

#include <string>
#include <map>
#include <chrono>
#include <mutex>

/**
 * @brief The flood_check class will check if there is a flood attempt from some specific
 * IP. The following is the method: If it can identify that in a given second a given IP
 * has tried more than 100 (configurable) requests it will reject the requests without
 * going further.
 */
class flood_check
{
public:

    static void attempt(std::string ip);

private:

    static std::mutex locker;

    // holds the flood attempts and when they have first occured
    struct count_started
    {
        int count = 0;
        std::time_t first_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    };

    // structures used to determine the flood rate / second
    using flood_map = std::map<std::string, int>;
    static std::map<std::time_t, flood_map> floods;

    // holds the flood attempts of a host. For each IP we have an attempt counter
    // how many times it tried to flood us and when the last attempt happened
    static std::map<std::string, count_started> hostd_flood;

    // holds the evil hosts that are flooding the system and how long they are locked out
    // the content of this map is determined based on the content of the hostd_flood
    // map. If a count from there reaches over 10 and the first_time of the
    // entry is < 1 minute then we consider the host flooding us.
    static std::map<std::string, std::time_t> rejected_hosts;

    // holds how many times the given host was suspended. Each suspension will give another minute to the host
    static std::map<std::string, int> suspension_times;
};

#endif // FLOOD_CHECK


#ifndef TRANSIT_TIME_H
#define TRANSIT_TIME_H

#include <chrono>
#include <string>

namespace stransit {

    /** time representation */
    class time_hm {
    public:
        time_hm(std::chrono::hours hours, std::chrono::minutes minutes);
        std::chrono::minutes difference_to(const time_hm& other) const;
        std::string to_string() const;
    private:
        std::chrono::hours m_hours;
        std::chrono::minutes m_minutes;
    };

}

#endif /* TRANSIT-TIME_H */

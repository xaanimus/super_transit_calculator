#include "transit_time.hpp"

using namespace stransit;
using namespace std::literals;

time_hm::time_hm(std::chrono::hours hours, std::chrono::minutes minutes)
  : m_hours(hours), m_minutes(minutes) {}

std::string time_hm::to_string() const {
    return ""s + std::to_string(m_hours.count()) + "h" + std::to_string(m_minutes.count()) + "min";
}

std::chrono::minutes time_hm::difference_to(const stransit::time_hm &other) const {
    std::chrono::hours h_diff = other.m_hours - m_hours;
    std::chrono::minutes m_diff = other.m_minutes - m_minutes;

    if (h_diff > 0h || (h_diff == 0h && m_diff >= 0min)) {
        std::chrono::minutes minute_h_diff(h_diff);
        minute_h_diff += m_diff;
        return minute_h_diff;
    } else {
        std::chrono::minutes minute_h_diff(24h + h_diff);
        minute_h_diff += m_diff;
        return minute_h_diff;
    }
}

time_hm time_hm::add(const time_hm& other) const {
    int total_minutes = other.m_minutes.count() + m_minutes.count();
    int excess_minutes_in_hours = total_minutes / 60;
    int total_hours = excess_minutes_in_hours + other.m_hours.count() + m_hours.count();
    return {std::chrono::hours(total_hours % 24), std::chrono::minutes(total_minutes % 60)};
}

time_hm time_hm::add(const std::chrono::minutes& other) const {
    return add({0h, other});
}

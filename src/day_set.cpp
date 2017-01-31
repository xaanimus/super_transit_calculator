#include "day_set.hpp"

using namespace stransit;


day_set::day_set(uint8_t day) : m_payload(day) {}

day_set::day_set(std::string days) {
    if (days == "Monday") {
        m_payload = day_monday;
    } else if (days == "Tuesday") {
        m_payload = day_tuesday;
    } else if (days == "Wednesday") {
        m_payload = day_wednesday;
    } else if (days == "Thursday") {
        m_payload = day_thursday;
    } else if (days == "Friday") {
        m_payload = day_friday;
    } else if (days == "Saturday") {
        m_payload = day_saturday;
    } else if (days == "Sunday") {
        m_payload = day_sunday;
    } else if (days == "Weekdays" || days == " Monday-Friday AM" ||
               days == "EXPRESS Weekdays" || days == "Monday - Friday" ||
               days == "Morning Weekdays" || days == "Afternoon Weekdays") {
        m_payload = day_set_weekdays.m_payload;
    } else if (days == "Monday - Wednesday") {
        m_payload = 0b00000111;
    } else if (days == "Monday-Thursday PM") {
        m_payload = 0b00001111;
    } else {
        throw day_set_parse_exception();
    }
}

/*
  "Weekdays"
  " Monday-Friday AM"
  "EXPRESS Weekdays"
  "Monday - Friday"
  "Morning Weekdays"
  "Monday - Wednesday"
  "Afternoon Weekdays"
  "Monday-Thursday PM"
*/

bool day_set::contains(day_type day) const {
    return (m_payload & day) == day;
}

bool day_set::contains(const day_set& days) const {
    return (m_payload & days.m_payload) == days.m_payload;
}

bool day_set::overlap(const day_set& days) const {
    return (m_payload & days.m_payload) != 0;
}

bool day_set::operator == (const day_set& days) const {
    return m_payload == days.m_payload;
}

const char* day_set_parse_exception::what() const noexcept {
    return "day_set parse exception.";
}

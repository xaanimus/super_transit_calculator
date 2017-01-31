#ifndef DAY_SET_H
#define DAY_SET_H

#include <exception>
#include <string>

namespace stransit {
    enum day_type : uint8_t {
        day_monday    = 1 << 0,
        day_tuesday   = 1 << 1,
        day_wednesday = 1 << 2,
        day_thursday  = 1 << 3,
        day_friday    = 1 << 4,
        day_saturday  = 1 << 5,
        day_sunday    = 1 << 6
    };

    class day_set {
        uint8_t m_payload;
    public:
        day_set(uint8_t day);
        day_set(std::string days); //throws

        bool contains(const day_type day) const;
        bool contains(const day_set& days) const;
        bool overlap(const day_set& days) const;
        bool operator == (const day_set& days) const;
    };

    const day_set day_set_monday    = {day_monday};
    const day_set day_set_tuesday   = {day_tuesday};
    const day_set day_set_wednesday = {day_wednesday};
    const day_set day_set_thursday  = {day_thursday};
    const day_set day_set_friday    = {day_friday};
    const day_set day_set_saturday  = {day_saturday};
    const day_set day_set_sunday    = {day_sunday};
    const day_set day_set_everyday  = {0b01111111};
    const day_set day_set_weekends  = {0b01100000};
    const day_set day_set_weekdays  = {0b00011111};

    class day_set_parse_exception : public std::exception {
    public:
        const char* what() const noexcept override;
    };
}

#endif /* DAY_SET_H */

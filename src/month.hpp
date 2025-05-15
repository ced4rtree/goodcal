#pragma once

#include <chrono>
#include <optional>
#include <string>

class Month {
private:
    std::string _name;
    int _days;
    constexpr Month(std::string name, int days) {
        this->_name = name;
        this->_days = days;
    }

public:
    // Return a read-only form of the name of this month
    constexpr std::string name() const {
        return _name;
    };

    // Return a read-only form of the amount of days in this month
    constexpr int days() const {
        return _days;        
    };

    // get the month based on the provided year_month_day info
    // return empty optional if invalid date is provided
    static constexpr std::optional<Month> getMonth(std::chrono::year_month_day date) {
        unsigned int month_index = static_cast<unsigned>(date.month());
        int year = static_cast<int>(date.year());
        bool is_leap_year = year % 4 == 0;
    
        switch (month_index) {
            case 1:
                return Month("January", 31);
            case 2:
                if (is_leap_year) {
                    return Month("February", 29);
                } else {
                    return Month("February", 28);
                }
            case 3:
                return Month("March", 31);
            case 4:
                return Month("April", 30);
            case 5:
                return Month("May", 31);
            case 6:
                return Month("June", 30);
            case 7:
                return Month("July", 31);
            case 8:
                return Month("August", 31);
            case 9:
                return Month("September", 30);
            case 10:
                return Month("October", 31);
            case 11:
                return Month("November", 30);
            case 12:
                return Month("December", 31);
            default:
                return {};
        }
    }
};

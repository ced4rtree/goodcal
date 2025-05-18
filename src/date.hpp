#pragma once

#include <chrono>

// returns the current date in year_month_day type
inline std::chrono::year_month_day current_date() {
    return {floor<std::chrono::days>(std::chrono::system_clock::now())};
}

// returns the amount of days in the provided date
// can return empty optional if the date has an invalid month
constexpr std::optional<unsigned int> days_in_month(
    std::chrono::year_month_day date) {
    unsigned int month_index = static_cast<unsigned>(date.month());
    int year                 = static_cast<int>(date.year());
    bool is_leap_year        = year % 4 == 0;

    switch (month_index) {
    case 1:
        return 31;
    case 2:
        if (is_leap_year) {
            return 29;
        } else {
            return 28;
        }
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    default:
        return {};
    }
}

// returns the weekday that the first day of the month in the provided date
// starts on
constexpr std::chrono::weekday first_day_of_month(
    std::chrono::year_month_day date) {
    std::chrono::year_month_day first_day
        = std::chrono::year_month(date.year(), date.month())
          / std::chrono::day(1);
    return std::chrono::weekday(first_day);
}

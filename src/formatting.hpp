#pragma once

#include "date.hpp"
#include <string>
#include <chrono>
#include <cmath>

// how tall each row should be
constexpr const int LINE_HEIGHT = 2;

// width of each day cell in columns
constexpr const int DAY_WIDTH = 4;

// two extra days for padding
constexpr const int WINDOW_WIDTH = DAY_WIDTH * 9;

// how many days are in a week
constexpr const double DAYS_IN_WEEK = 7.0;

// how many lines should be allocated for the header
constexpr const int HEADER_HEIGHT = 2;

// how much space the border is given
constexpr const int BORDER_WIDTH = 1;

// return how tall the calendar should be in units of cells
// can return empty optional if the supplied date has an invalid month index
constexpr std::optional<int> calculate_window_height(std::chrono::year_month_day date) {
    std::optional<unsigned int> month_days_opt = days_in_month(date);
    if (!month_days_opt.has_value()) {
        return {};
    }
    unsigned int month_days = month_days_opt.value();
    month_days += first_day_of_month(date).c_encoding();

    return (std::ceil(month_days / DAYS_IN_WEEK) + HEADER_HEIGHT)
           * LINE_HEIGHT
           + BORDER_WIDTH;
}

// returns a string with each day of the week shortened to day_width-1
// characters with a space in between each day. e.g. format_week(3) -> "Su Mo Tu
// We Th Fr Sa "
constexpr std::string format_week(unsigned int day_width) {
    std::string days[] = {
        "Sunday",   "Monday", "Tuesday",  "Wednesday",
        "Thursday", "Friday", "Saturday",
    };
    std::string ret = "";

    int max_length = day_width - 1;
    for (const std::string &day : days) {
        if (max_length < day.size()) {
            ret += day.substr(0, max_length) + " ";
        } else {
            ret += day;
        }
    }
    return ret;
}

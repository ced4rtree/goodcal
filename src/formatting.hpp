#pragma once

#include "month.hpp"
#include <cmath>
#include <string>

// how tall each row should be
constexpr const int LINE_HEIGHT = 2;

// width of each day cell in columns
constexpr const int DAY_WIDTH = 4;

// two extra days for padding
constexpr const int WINDOW_WIDTH = DAY_WIDTH * 9;

constexpr int calculate_window_height(Month& month) {
    return (std::ceil(month.days()/7.0) + 2) * LINE_HEIGHT + 1; // +2 for headers
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

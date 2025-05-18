#include "date.hpp"
#include "formatting.hpp"
#include "key.hpp"
#include <chrono>
#include <cmath>
#include <curses.h>
#include <iostream>
#include <ncurses.h>
#include <string>

void err_n_die(std::string msg, WINDOW* window) {
    std::cerr << msg;
    // close window and quit the program
    refresh();
    wrefresh(window);
    exit(endwin() && delwin(window));
}

void err_n_die(std::string msg) {
    std::cerr << msg;
    exit(1);
}

int main(void) {
    std::chrono::year_month_day now = current_date();

    // ncurses init
    std::optional<int> window_height_opt = calculate_window_height(now);
    if (!window_height_opt.has_value()) {
        err_n_die("Could not retrieve window height due to invalid date.\n");
    }
    int cal_window_height = window_height_opt.value();

    initscr();
    WINDOW* cal_window = newwin(cal_window_height, CAL_WINDOW_WIDTH,
        LINES / 2 - cal_window_height / 2, COLS / 2 - CAL_WINDOW_WIDTH / 2);
    // WINDOW* details_window = newwin(LINES-cal_window_height, )
    noecho(); // don't print keybord inputs
    start_color();
    refresh();
    curs_set(0); // hide cursor
    box(cal_window, 0, 0);
    wrefresh(cal_window);

    // create the date header, e.g. 'January 2025'
    std::string date_header = std::format("{}", now.month()) + " "
                              + std::to_string(static_cast<int>(now.year()));

    // stuff for interactive date selection
    unsigned int selected_day = static_cast<unsigned>(now.day());

    // event loop
    while (true) {
        // print the calendar
        mvwprintw(cal_window, BORDER_WIDTH,
            CAL_WINDOW_WIDTH / 2 - date_header.size() / 2, "%s",
            date_header.c_str());

        mvwprintw(cal_window, HEADER_HEIGHT + BORDER_WIDTH, DAY_WIDTH, "%s",
            format_week(DAY_WIDTH).c_str());

        for (int day = 1; day <= days_in_month(now); day++) {
            // offset to account for the month not starting on Sunday
            int day_offset = day + first_day_of_month(now).c_encoding();

            // to make floor(n/7) still fit the last weekday on
            // saturday instead of moving to the next row, where n is
            // the cell being focused
            double weekdays_more = DAYS_IN_WEEK + 0.00001;
            if (day == selected_day) {
                wattron(cal_window, A_REVERSE);
            }

            if (day == static_cast<unsigned>(now.day())) {
                wattron(cal_window, A_UNDERLINE);
            }

            mvwprintw(cal_window,
                // cell's y position
                std::floor(day_offset / weekdays_more) * LINE_HEIGHT
                    + HEADER_HEIGHT + BORDER_WIDTH + LINE_HEIGHT,
                // cell's x position
                DAY_WIDTH
                    * (day_offset % (int)DAYS_IN_WEEK == 0
                            ? DAYS_IN_WEEK
                            : day_offset % (int)DAYS_IN_WEEK),
                "%d", day);
            wattroff(cal_window, A_REVERSE);
            wattroff(cal_window, A_UNDERLINE);
        }

        // Handle key processing
        int key = wgetch(cal_window);
        switch (key) {
        case 'q':
            // close window and quit the program
            refresh();
            wrefresh(cal_window);
            return endwin() && delwin(cal_window);
        // interactively navigate the calendar
        case WASD_UP:
        case VIM_UP:
            if (selected_day > DAYS_IN_WEEK) {
                selected_day -= DAYS_IN_WEEK;
            }
            break;
        case WASD_DOWN:
        case VIM_DOWN:
            if (selected_day + DAYS_IN_WEEK <= days_in_month(now)) {
                selected_day += DAYS_IN_WEEK;
            }
            break;
        case WASD_LEFT:
        case VIM_LEFT:
            if (selected_day > 1) {
                selected_day--;
            }
            break;
        case WASD_RIGHT:
        case VIM_RIGHT:
            if (selected_day < days_in_month(now)) {
                selected_day++;
            }
            break;
        }

        refresh();
        wrefresh(cal_window);
    }

    return 0;
}

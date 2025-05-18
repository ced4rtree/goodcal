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
    std::optional<int> window_height_opt =
        calculate_window_height(now);
    if (!window_height_opt.has_value()) {
        err_n_die("Could not retrieve window height due to invalid date.\n");
    }
    int window_height = window_height_opt.value();

    initscr();
    WINDOW *window = newwin(window_height, WINDOW_WIDTH, 0, 0);
    noecho(); // don't print keybord inputs
    start_color();
    refresh();
    curs_set(0); // hide cursor
    box(window, 0, 0);
    wrefresh(window);

    // create the date header, e.g. 'January 2025'
    std::string date_header = std::format("{}", now.month()) + " " +
                              std::to_string(static_cast<int>(now.year()));

    // stuff for interactive date selection
    unsigned int selected_day = static_cast<unsigned>(now.day());

    // event loop
    while (true) {
        // print the calendar
        mvwprintw(window, BORDER_WIDTH, WINDOW_WIDTH / 2 - date_header.size() / 2, "%s",
                  date_header.c_str());

        mvwprintw(window, HEADER_HEIGHT + BORDER_WIDTH, DAY_WIDTH, "%s", format_week(DAY_WIDTH).c_str());

        for (int day = 1; day <= days_in_month(now); day++) {
            // offset to account for the month not starting on Sunday
            int day_offset = day + first_day_of_month(now).c_encoding();
            double weekdays_more = DAYS_IN_WEEK + 0.00001;
            if (day == selected_day) {
                wattron(window, A_REVERSE);
                // attron(A_BLINK);
            }
            mvwprintw(
                window,
                std::floor(day_offset / weekdays_more)
                    * LINE_HEIGHT + HEADER_HEIGHT
                    + BORDER_WIDTH + LINE_HEIGHT,
                DAY_WIDTH * (day_offset % (int)DAYS_IN_WEEK == 0
                             ? DAYS_IN_WEEK
                             : day_offset % (int)DAYS_IN_WEEK),
                "%d", day
            );
            wattroff(window, A_REVERSE);
            // attroff(A_BLINK);
        }

        // Handle key processing
        int key = wgetch(window);
        switch (key) {
            case 'q':
                // close window and quit the program
                refresh();
                wrefresh(window);
                return endwin() && delwin(window);
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
        wrefresh(window);
    }

    return 0;
}

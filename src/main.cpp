#include "date.hpp"
#include "formatting.hpp"
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
        }

        // Handle key processing
        int key = wgetch(window);

        switch (key) {
            case 'q':
                // close window and quit the program
                refresh();
                wrefresh(window);
                return endwin() && delwin(window);
        }

        refresh();
        wrefresh(window);
    }

    return 0;
}

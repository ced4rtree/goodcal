#include "formatting.hpp"
#include "month.hpp"
#include <chrono>
#include <cmath>
#include <curses.h>
#include <iostream>
#include <ncurses.h>
#include <string>

int main(void) {
    // create the date header, e.g. 'January 2025'
    const std::chrono::year_month_day now{
        floor<std::chrono::days>(std::chrono::system_clock::now())};

    std::optional<Month> month_opt = Month::getMonth(now);
    if (!month_opt.has_value()) {
        std::cerr << "Invalid date used to get month data\n";
        exit(1);
    }
    Month month = month_opt.value();

    std::string date_header =
        month.name() + " " + std::to_string(static_cast<int>(now.year()));

    // ncurses init
    initscr();
    WINDOW *window = newwin(calculate_window_height(month), WINDOW_WIDTH, 0, 0);
    noecho(); // don't print keybord inputs
    start_color();
    refresh();
    curs_set(0); // hide cursor
    box(window, 0, 0);
    wrefresh(window);

    // event loop
    while (true) {
        // print the calendar
        mvwprintw(window, 1, WINDOW_WIDTH / 2 - date_header.size() / 2, "%s",
                  date_header.c_str());

        mvwprintw(window, 3, DAY_WIDTH, "%s", format_week(DAY_WIDTH).c_str());

        for (int day = 1; day <= month.days(); day++) {
            mvwprintw(window,
                      std::floor(day / 7.001) * LINE_HEIGHT + 3 + LINE_HEIGHT,
                      DAY_WIDTH * (day%7 == 0 ? 7 : day%7),
                      "%d", day);
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

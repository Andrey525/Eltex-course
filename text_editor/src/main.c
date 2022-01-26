#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

void sig_winch(int signo) {
  // struct winsize size;
  // ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  // resizeterm(size.ws_row, size.ws_col);
  // nodelay(stdscr, 1);
  // while (wgetch(stdscr) != ERR)
  //   ;
  // nodelay(stdscr, 0);

  endwin();
  printf(
      "Попытка изменения размеров окна!\n"
      "Программа завершена!\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  WINDOW *frame;
  WINDOW *workspace;
  WINDOW *menu;
  WINDOW *submenu_open;
  WINDOW *submenu_save;
  WINDOW *submenu_input;
  const int menu_height = 3;
  const int menu_width = size.ws_col;
  const int count_of_submenu = 3;
  const int submenu_width = menu_width / count_of_submenu;
  const int frame_height = size.ws_row - menu_height;
  const int frame_width = size.ws_col;
  const int workspace_offset = 1;
  const int workspace_height = frame_height - workspace_offset;
  const int workspace_width = frame_width - workspace_offset;
  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  curs_set(0);
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  refresh();

  frame = newwin(frame_height, frame_width, 0, 0);
  workspace = derwin(frame, workspace_height, workspace_width, 0, 0);
  menu = newwin(menu_height, menu_width, size.ws_row - menu_height, 0);
  submenu_open = derwin(menu, menu_height, submenu_width, 0, 0);
  submenu_input = derwin(menu, menu_height, submenu_width, 0, submenu_width);
  submenu_save = derwin(menu, menu_height, submenu_width, 0, submenu_width * 2);
  wbkgd(submenu_open, COLOR_PAIR(1) | A_BOLD);
  wbkgd(submenu_save, COLOR_PAIR(2) | A_BOLD);
  wbkgd(submenu_input, COLOR_PAIR(3) | A_BOLD);
  wattron(frame, A_BOLD);
  box(frame, '|', '-');
  box(submenu_open, '|', '-');
  box(submenu_save, '|', '-');
  box(submenu_input, '|', '-');
  wmove(submenu_open, 1, (submenu_width - 17) / 2);
  wprintw(submenu_open, "O p e n   f i l e");
  wmove(submenu_save, 1, (submenu_width - 17) / 2);
  wprintw(submenu_save, "S a v e   f i l e");
  wrefresh(frame);
  wrefresh(workspace);
  wrefresh(menu);
  wrefresh(submenu_save);
  wrefresh(submenu_open);
  wrefresh(submenu_input);
  delwin(submenu_input);
  delwin(submenu_save);
  delwin(submenu_open);
  delwin(menu);
  delwin(workspace);
  delwin(frame);
  getch();
  endwin();
  exit(EXIT_SUCCESS);
}
/*========================================
 *    sl.c: SL version 6.00
 *        Copyright 1993,1998,2014
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2022/03/11
 *========================================
 */
/* sl version 6.00 : shachonize                                              */
/*                   delete -l -c options                                    */
/*                                             by Miyano Yami     2022/03/11 */
/* sl version 5.1e : mayakatasonize                                          */
/*                   restore -l -c options.                                  */
/*                                             by Taichi Sugiyama 2014/ 6/10 */
/* sl version 5.02 : Fix compiler warnings.                                  */
/*                                              by Jeff Schwab    2014/06/03 */
/* sl version 5.01e: erutasonize                                             */
/*                   delete -l -c options.                                   */
/*                                             by Taichi Sugiyama 2014/ 3/26 */
/* sl version 5.01 : removed cursor and handling of IO                       */
/*                                              by Chris Seymour  2014/01/03 */
/* sl version 5.00 : add -c option                                           */
/*                                              by Toyoda Masashi 2013/05/05 */
/* sl version 4.00 : add C51, usleep(40000)                                  */
/*                                              by Toyoda Masashi 2002/12/31 */
/* sl version 3.03 : add usleep(20000)                                       */
/*                                              by Toyoda Masashi 1998/07/22 */
/* sl version 3.02 : D51 flies! Change options.                              */
/*                                              by Toyoda Masashi 1993/01/19 */
/* sl version 3.01 : Wheel turns smoother                                    */
/*                                              by Toyoda Masashi 1992/12/25 */
/* sl version 3.00 : Add d(D51) option                                       */
/*                                              by Toyoda Masashi 1992/12/24 */
/* sl version 2.02 : Bug fixed.(dust remains in screen)                      */
/*                                              by Toyoda Masashi 1992/12/17 */
/* sl version 2.01 : Smoke run and disappear.                                */
/*                   Change '-a' to accident option.                         */
/*                                              by Toyoda Masashi 1992/12/16 */
/* sl version 2.00 : Add a(all),l(long),F(Fly!) options.                     */
/*                                              by Toyoda Masashi 1992/12/15 */
/* sl version 1.02 : Add turning wheel.                                      */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.01 : Add more complex smoke.                                 */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.00 : SL runs vomitting out smoke.                            */
/*                                              by Toyoda Masashi 1992/12/11 */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include "sl.h"

void add_shachodayo(int y, int x);
int add_shacho(int x);
void option(char *str);
int my_mvaddstr(int y, int x, char *str);

int ACCIDENT  = 0;
int FLY       = 0;

int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

void option(char *str)
{
    extern int ACCIDENT, FLY;

    while (*str != '\0') {
		switch (*str++) {
		    case 'a': ACCIDENT = 1; break;
		    case 'F': FLY      = 1; break;
		    default:                break;
		}
    }
}

int main(int argc, char *argv[])
{
    int x, i;

    for (i = 1; i < argc; ++i) {
        if (*argv[i] == '-') {
            option(argv[i] + 1);
        }
    }
    initscr();
    signal(SIGINT, SIG_IGN);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    for (x = COLS - 1; ; --x) {
        if (add_shacho(x) == ERR) break;
        getch();
        refresh();
        usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
}


int add_shacho(int x)
{
    static char *shacho[SHACHOPATTERNS][SHACHOHIGHT + 1]
	= {{SHACHO11, SHACHO12, SHACHO13, SHACHO14, SHACHO15, SHACHO16, SHACHO17, SHACHO18, SHACHO19, SHACHO1DEL},
	   {SHACHO21, SHACHO22, SHACHO23, SHACHO24, SHACHO25, SHACHO26, SHACHO27, SHACHO28, SHACHO29, SHACHO2DEL},
	   {SHACHO31, SHACHO32, SHACHO33, SHACHO34, SHACHO35, SHACHO36, SHACHO37, SHACHO38, SHACHO39, SHACHO3DEL},
	   {SHACHO41, SHACHO42, SHACHO43, SHACHO44, SHACHO45, SHACHO46, SHACHO47, SHACHO48, SHACHO49, SHACHO4DEL},
	   {SHACHO31, SHACHO32, SHACHO33, SHACHO34, SHACHO35, SHACHO36, SHACHO37, SHACHO38, SHACHO39, SHACHO3DEL},
	   {SHACHO21, SHACHO22, SHACHO23, SHACHO24, SHACHO25, SHACHO26, SHACHO27, SHACHO28, SHACHO29, SHACHO2DEL}};

    int i, y;

    if (x < - SHACHOLENGTH)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
		y = (x / 6) + LINES - (COLS / 6) - SHACHOHIGHT;
    }
    for (i = 0; i <= SHACHOHIGHT; ++i) {
		my_mvaddstr(y + i, x, shacho[(SHACHOLENGTH + x) / 3 % SHACHOPATTERNS][i]);
    }
    if (ACCIDENT == 1) {
		add_shachodayo(y , x + 16);
    }
    return OK;
}


void add_shachodayo(int y, int x)
{
	static char *man[2] = {"", "< SHACHO DAYO! "};
	my_mvaddstr(y , x, man[(SHACHOLENGTH + x) / 12 % 2]);
}


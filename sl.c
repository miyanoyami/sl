/*========================================
 *    sl.c: SL version 5.02
 *        Copyright 1993,1998,2014
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2014/06/03
 *========================================
 */
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

void add_kininarimasu(int y, int x);
void add_fukuchan(int y, int x);
void add_lolinarimasu(int y, int x);
void add_smoke(int y, int x);
int add_erutaso(int x);
int add_mayakataso(int x);
int add_lolitaso(int x);
void option(char *str);
int my_mvaddstr(int y, int x, char *str);

int ACCIDENT  = 0;
int FLY       = 0;
int MAYAKA    = 0;
int LOLI    = 0;

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
    extern int ACCIDENT, FLY, LOLI, MAYAKA;

    while (*str != '\0') {
		switch (*str++) {
		    case 'a': ACCIDENT = 1; break;
		    case 'F': FLY      = 1; break;
		    case 'l': LOLI     = 1; break;
		    case 'c': MAYAKA   = 1; break;
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
        if (LOLI == 1) {
            if (add_lolitaso(x) == ERR) break;
        }
        else if (MAYAKA == 1) {
            if (add_mayakataso(x) == ERR) break;
        }
        else {
            if (add_erutaso(x) == ERR) break;
        }
        getch();
        refresh();
        usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
}


int add_erutaso(int x)
{
    static char *erutaso[ERUTASOPATTERNS][ERUTASOHIGHT + 1]
	= {{ERUTASO11, ERUTASO12, ERUTASO13, ERUTASO14, ERUTASO15, ERUTASO16, ERUTASO17, ERUTASO18, ERUTASO19, ERUTASO1DEL},
	   {ERUTASO21, ERUTASO22, ERUTASO23, ERUTASO24, ERUTASO25, ERUTASO26, ERUTASO27, ERUTASO28, ERUTASO29, ERUTASO2DEL},
	   {ERUTASO31, ERUTASO32, ERUTASO33, ERUTASO34, ERUTASO35, ERUTASO36, ERUTASO37, ERUTASO38, ERUTASO39, ERUTASO3DEL},
	   {ERUTASO41, ERUTASO42, ERUTASO43, ERUTASO44, ERUTASO45, ERUTASO46, ERUTASO47, ERUTASO48, ERUTASO49, ERUTASO4DEL},
	   {ERUTASO31, ERUTASO32, ERUTASO33, ERUTASO34, ERUTASO35, ERUTASO36, ERUTASO37, ERUTASO38, ERUTASO39, ERUTASO3DEL},
	   {ERUTASO21, ERUTASO22, ERUTASO23, ERUTASO24, ERUTASO25, ERUTASO26, ERUTASO27, ERUTASO28, ERUTASO29, ERUTASO2DEL}};

    int i, y;

    if (x < - ERUTASOLENGTH)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
		y = (x / 6) + LINES - (COLS / 6) - ERUTASOHIGHT;
    }
    for (i = 0; i <= ERUTASOHIGHT; ++i) {
		my_mvaddstr(y + i, x, erutaso[(ERUTASOLENGTH + x) / 3 % ERUTASOPATTERNS][i]);
    }
    if (ACCIDENT == 1) {
		add_kininarimasu(y , x + 16);
    }
    add_smoke(y - 1, x + ERUTASOFUNNEL);
    return OK;
}


int add_mayakataso(int x)
{
    static char *mayakataso[MAYAKATASOPATTERNS][MAYAKATASOHIGHT + 1]
	= {{MAYAKATASO11, MAYAKATASO12, MAYAKATASO13, MAYAKATASO14, MAYAKATASO15, MAYAKATASO16, MAYAKATASO17, MAYAKATASO18, MAYAKATASO19, MAYAKATASO1DEL},
	   {MAYAKATASO21, MAYAKATASO22, MAYAKATASO23, MAYAKATASO24, MAYAKATASO25, MAYAKATASO26, MAYAKATASO27, MAYAKATASO28, MAYAKATASO29, MAYAKATASO2DEL},
	   {MAYAKATASO31, MAYAKATASO32, MAYAKATASO33, MAYAKATASO34, MAYAKATASO35, MAYAKATASO36, MAYAKATASO37, MAYAKATASO38, MAYAKATASO39, MAYAKATASO3DEL},
	   {MAYAKATASO41, MAYAKATASO42, MAYAKATASO43, MAYAKATASO44, MAYAKATASO45, MAYAKATASO46, MAYAKATASO47, MAYAKATASO48, MAYAKATASO49, MAYAKATASO4DEL},
	   {MAYAKATASO31, MAYAKATASO32, MAYAKATASO33, MAYAKATASO34, MAYAKATASO35, MAYAKATASO36, MAYAKATASO37, MAYAKATASO38, MAYAKATASO39, MAYAKATASO3DEL},
	   {MAYAKATASO21, MAYAKATASO22, MAYAKATASO23, MAYAKATASO24, MAYAKATASO25, MAYAKATASO26, MAYAKATASO27, MAYAKATASO28, MAYAKATASO29, MAYAKATASO2DEL}};

    int i, y;

    if (x < - MAYAKATASOLENGTH)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
		y = (x / 6) + LINES - (COLS / 6) - MAYAKATASOHIGHT;
    }
    for (i = 0; i <= MAYAKATASOHIGHT; ++i) {
		my_mvaddstr(y + i, x, mayakataso[(MAYAKATASOLENGTH + x) / 3 % MAYAKATASOPATTERNS][i]);
    }
    if (ACCIDENT == 1) {
		add_fukuchan(y , x + 16);
    }
    add_smoke(y - 1, x + MAYAKATASOFUNNEL);
    return OK;
}


int add_lolitaso(int x)
{
    static char *LOLITASO[LOLITASOPATTERNS][LOLITASOHIGHT + 1]
	= {{LOLITASO11, LOLITASO12, LOLITASO13, LOLITASO14, LOLITASO15, LOLITASO16, LOLITASO17, LOLITASO1DEL},
	   {LOLITASO21, LOLITASO22, LOLITASO23, LOLITASO24, LOLITASO25, LOLITASO26, LOLITASO27, LOLITASO2DEL},
	   {LOLITASO31, LOLITASO32, LOLITASO33, LOLITASO34, LOLITASO35, LOLITASO36, LOLITASO37, LOLITASO3DEL},
	   {LOLITASO41, LOLITASO42, LOLITASO43, LOLITASO44, LOLITASO45, LOLITASO46, LOLITASO47, LOLITASO4DEL},
	   {LOLITASO31, LOLITASO32, LOLITASO33, LOLITASO34, LOLITASO35, LOLITASO36, LOLITASO37, LOLITASO3DEL},
	   {LOLITASO21, LOLITASO22, LOLITASO23, LOLITASO24, LOLITASO25, LOLITASO26, LOLITASO27, LOLITASO2DEL}};

    int i, y;

    if (x < - LOLITASOLENGTH)  return ERR;
    y = LINES / 2 - 3;

    if (FLY == 1) {
		y = (x / 6) + LINES - (COLS / 6) - LOLITASOHIGHT;
    }
    for (i = 0; i <= LOLITASOHIGHT; ++i) {
		my_mvaddstr(y + i, x, LOLITASO[(LOLITASOLENGTH + x) / 3 % LOLITASOPATTERNS][i]);
    }
    if (ACCIDENT == 1) {
		add_lolinarimasu(y , x + 13);
    }
    add_smoke(y - 1, x + LOLITASOFUNNEL);
    return OK;
}

void add_kininarimasu(int y, int x)
{
	static char *man[2] = {"", "< KININARIMASU! "};
	my_mvaddstr(y , x, man[(ERUTASOLENGTH + x) / 12 % 2]);
}

void add_fukuchan(int y, int x)
{
	static char *man[2] = {"", "< FUKUCHAN! "};
	my_mvaddstr(y , x, man[(MAYAKATASOLENGTH + x) / 12 % 2]);
}

void add_lolinarimasu(int y, int x)
{
	static char *man[2] = {"", "< kininarimasu! "};
	my_mvaddstr(y , x, man[(LOLITASOLENGTH + x) / 12 % 2]);
}


void add_smoke(int y, int x)
#define SMOKEPTNS        16
{
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0             };
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                                 2,  2, 2, 3, 3, 3             };
    int i;

    if (x % 4 == 0) {
        for (i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}

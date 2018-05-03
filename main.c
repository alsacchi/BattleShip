#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

#define DELAY 5000
#define BOAT 'x'
#define MAX_BOAT 15
#define MIN_BOAT 5

void refreshgriglia(WINDOW *wingriglia,char (*griglia)[10]);
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	//Variabili
	char griglia_p1[10][10] =  {
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	};	
	char griglia_p2[10][10] =  {
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	};	
	char griglia_p1_shoot[10][10] = {
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	};
	char griglia_p2_shoot[10][10] = {
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	};	
	char (*griglia)[10];
	int pos_y = 1, pos_x = 1;
	int ch = 0;
	int offset = 1;
	int boat_plc_p1 = 0;
	int boat_plc_p2 = 0;
	int turn = 0;
	int player_1_score = 0;
	int player_2_score = 0;
	bool started = false;
	WINDOW *wingriglia, *winmove, *winscore, *lettere;
	//Inizializzo le finestre
	initscr();
	noecho();
	curs_set(FALSE);
	wingriglia = newwin(12, 12, 1, 1);
	winmove = newwin(11, 11, 1, 1);
	winscore = newwin(12, 17, 1, 15);
	lettere = newwin(13, 13, 0, 0);
	nodelay(winmove, TRUE);
	keypad(winmove, TRUE);
	griglia = griglia_p1;
	//Inizio ciclo
	mvwprintw(lettere, 0, 2, "ABCDEFGJKL");
	mvwprintw(lettere, 2, 0, "1\n2\n3\n4\n5\n6\n7\n8\n9\n0");
	wrefresh(lettere);
	while (true) {
		//clear();
		//Disegno i contorni di wingriglia
		box(wingriglia, 0, 0);
		box(winscore, 0, 0);
		//Stampo lo score
		if(started == false) {
			mvwprintw(winscore, 1, 1, "PREPARATION");
		} else {
			mvwprintw(winscore, 1, 1, "BATTLE");
		}
		mvwprintw(winscore, 2, 1, "Boats-P1: %d", boat_plc_p1);
		mvwprintw(winscore, 3, 1, "Boats-P2: %d", boat_plc_p2);
		mvwprintw(winscore, 4, 1, "MAX-BOAT: %d", MAX_BOAT);
		mvwprintw(winscore, 5, 1, "Player TURN: %d", turn);
		mvwprintw(winscore, 6, 1, "P1-SCORE: %d", player_1_score);
		mvwprintw(winscore, 7, 1, "P2-SCORE: %d", player_2_score);
		//Stampo il cursore Virtuale
		mvwprintw(winmove, pos_y, pos_x, "#");
		//Controllo se si preme un tasto
		ch = wgetch(winmove);
		switch(ch) {
			case KEY_RIGHT:
				pos_x++;
				break;
			case KEY_LEFT:
				pos_x--;
				break;
			case KEY_UP:
				pos_y--;
				break;
			case KEY_DOWN:
				pos_y++;
				break;
			case 'c':
				if(turn == 0 && griglia[pos_y-offset][pos_x-offset] == ' ' && boat_plc_p1 < MAX_BOAT && started == false) {
					griglia[pos_y-offset][pos_x-offset] = 'x';
					boat_plc_p1++;
				} 
				if(turn == 1 && griglia[pos_y-offset][pos_x-offset] == ' ' && boat_plc_p2 < MAX_BOAT && started == false) {
					griglia[pos_y-offset][pos_x-offset] = 'x';
					boat_plc_p2++;
				}
				break;
			case 'r':
				if(turn == 0 && griglia[pos_y-offset][pos_x-offset] == BOAT && boat_plc_p1 >= 0 && started == false) {
					griglia[pos_y-offset][pos_x-offset] = ' ';
					boat_plc_p1--;
				}
				if(turn == 1 && griglia[pos_y-offset][pos_x-offset] == BOAT && boat_plc_p2 >= 0 && started == false) {
					griglia[pos_y-offset][pos_x-offset] = ' ';
					boat_plc_p2--;
				}
				break;
			case 's':
				started = true;
				griglia = griglia_p1_shoot;
				turn = 0;
				werase(winscore);
				break;
			case ' ':
				werase(winscore);
				if (turn == 0 && griglia_p2[pos_y-offset][pos_x-offset] == BOAT && started == true) {
					turn = 1;
					griglia[pos_y-offset][pos_x-offset] = 'C';
					griglia = griglia_p2_shoot;
					player_1_score++;
					boat_plc_p2--;
					mvwprintw(winscore, 8, 1, "P1 HIT!");
					break;	
					
				}
				if (turn == 0 && griglia_p2[pos_y-offset][pos_x-offset] == ' ' && started == true) {
					turn = 1;
					griglia[pos_y-offset][pos_x-offset] = 'M';
					griglia = griglia_p2_shoot;
					mvwprintw(winscore, 8, 1, "P1 MISS!");
					break;	
				}
				if (turn == 1 && griglia_p1[pos_y-offset][pos_x-offset] == BOAT && started == true) {
					turn = 0;
					griglia[pos_y-offset][pos_x-offset] = 'C';
					griglia = griglia_p1_shoot;
					player_2_score++;
					boat_plc_p1--;
					mvwprintw(winscore, 8, 1, "P2 HIT!");
					break;	
				}
				if (turn == 1 && griglia_p1[pos_y-offset][pos_x-offset] == ' ' && started == true) {
					turn = 0;
					griglia[pos_y-offset][pos_x-offset] = 'M';
					griglia = griglia_p1_shoot;
					mvwprintw(winscore, 8, 1, "P2 MISS!");
					break;	
				}
				break;
			case 10:
				if(turn == 1 && started == false && boat_plc_p2 >= MIN_BOAT) {
					//griglia_p2 = griglia;
					griglia = griglia_p1;
					pos_x = 1;
					pos_y = 1;
					
					break;
				} 
				if(turn == 1 && started == false && boat_plc_p2 < MIN_BOAT) { 
					mvwprintw(winscore, 8, 1, "P2 MIN_BOAT!");
					break;
				}
				if(turn == 0 && started == false && boat_plc_p1 >= MIN_BOAT) {
					//griglia_p1 = griglia;
					griglia = griglia_p2;
					pos_x = 1;
					pos_y = 1;
					turn = 1;
					break;
				}
				if(turn == 0 && started == false && boat_plc_p1 < MIN_BOAT) { 
					mvwprintw(winscore, 8, 1, "P1 MIN_BOAT!");
					break;
				}
				break;
			case 'q':
				endwin();
				return 0;
		}
		//Controllo della posizione del cursore virtuale
		//Probabilmente Ottimizabile
		if(pos_x + 1 > 11) {
			pos_x = 1;
		}
		if(pos_x <= 0) {
			pos_x = 10;
		}
		if(pos_x + 1 > 11) {
			pos_x--;
		}
		if(pos_x - 1 < 0) {
			pos_x++;
		}
		if(pos_y - 1 < 0) {
			pos_y++;
		}
		if(pos_y + 1 > 11) {
			pos_y--;
		}
		//Refresh delle finestre,
		//Controllare se esiste la funzione per refreshararle tutte in una volta, refresh() non va
		wrefresh(winmove);
		wrefresh(winscore);
		refreshgriglia(wingriglia, griglia);
	}
	endwin();
}
void refreshgriglia(WINDOW *wingriglia,char (*griglia)[10]) {
	for(int i = 0;i<10; ++i) {
		for(int j = 0; j<10; ++j) {
			mvwprintw(wingriglia, i+1, j+1, "%c", griglia[i][j]);
			//griglia[i][j]++;
		}
		printw("\n");
		wrefresh(wingriglia);
	}
}
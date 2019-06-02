#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include "main.h"

//Costanti
#define DELAY 5000
#define BOAT 'x'
#define MAX_BOAT 15
#define MIN_BOAT 5


int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	//Griglie di gioco, da ottimizzare
	char **griglia_p1 =  calloc(10, sizeof(char));
	for(int i = 0; i < 10; i++) {
		griglia_p1[i] = calloc(10, sizeof(char));
	}
	char **griglia_p2 = calloc(10, sizeof(char));
	for(int i = 0; i < 10; i++) {
		griglia_p2[i] = calloc(10, sizeof(char));
	}
	char **griglia_p1_shoot =  calloc(10, sizeof(char));
	for(int i = 0; i < 10; i++) {
		griglia_p1_shoot[i] = calloc(10, sizeof(char));
	}
	char **griglia_p2_shoot =  calloc(10, sizeof(char));
	for(int i = 0; i < 10; i++) {
		griglia_p2_shoot[i] = calloc(10, sizeof(char));
	}
	// char griglia_p2[10][10] =  {
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	// };	
	// char griglia_p1_shoot[10][10] = {
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	// };
	// char griglia_p2_shoot[10][10] = {
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	// {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
	// };	
	//Vars
	char **griglia = calloc(10, sizeof(char));
	for(int i = 0; i < 10; i++) {
		griglia[i] = calloc(10, sizeof(char));
	}
	int pos_y = 1, pos_x = 1;
	int ch = 0;
	int offset = 1;
	int boat_plc_p1 = 0;
	int boat_plc_p2 = 0;
	int turn = 0;
	int player_1_score = 0;
	int player_2_score = 0;
	bool started = false;
	char *player;
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
	for(int i = 0; i < 10; i++) {
		memcpy(griglia[i], griglia_p1[i], 10);
	}
	mvwprintw(lettere, 0, 2, "ABCDEFGHIJ");
	mvwprintw(lettere, 2, 0, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9");
	wrefresh(lettere);
	//Loop-Update finestre
	while (true) {
		//clear();
		//Disegno i contorni di wingriglia
		mvwprintw(lettere, 0, 2, "ABCDEFGHIJ");
		mvwprintw(lettere, 2, 0, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9");
		box(wingriglia, 0, 0);
		box(winscore, 0, 0);
		//Stampo lo score
		if(started == false) {
			mvwprintw(winscore, 1, 1, "PREPARATION");
		} else {
			mvwprintw(winscore, 1, 1, "BATTLE");
		}
		switch (turn) {
			case 0:
				player = "P1";
				break;
			case 1:
				player = "P2";
				break;
		}
		mvwprintw(winscore, 2, 1, "Boats-P1: %d", boat_plc_p1);
		mvwprintw(winscore, 3, 1, "Boats-P2: %d", boat_plc_p2);
		mvwprintw(winscore, 4, 1, "MAX-BOAT: %d", MAX_BOAT);
		mvwprintw(winscore, 5, 1, "Player TURN: %s", player);
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
				wrefresh(wingriglia);
				if(turn == 0 && griglia[pos_y-offset][pos_x-offset] == 0 && boat_plc_p1 < MAX_BOAT && started == false) {
					griglia[pos_y-offset][pos_x-offset] = 'x';
					boat_plc_p1++;
				} 
				if(turn == 1 && griglia[pos_y-offset][pos_x-offset] == 0 && boat_plc_p2 < MAX_BOAT && started == false) {
					griglia[pos_y-offset][pos_x-offset] = 'x';
					boat_plc_p2++;
				}
				break;
			case 'r':
				wrefresh(wingriglia);
				if(turn == 0 && griglia[pos_y-offset][pos_x-offset] == BOAT && boat_plc_p1 >= 0 && started == false) {
					griglia[pos_y-offset][pos_x-offset] = 0;
					boat_plc_p1--;
				}
				if(turn == 1 && griglia[pos_y-offset][pos_x-offset] == BOAT && boat_plc_p2 >= 0 && started == false) {
					griglia[pos_y-offset][pos_x-offset] = 0;
					boat_plc_p2--;
				}
				break;
			case 's':
				werase(wingriglia);
				wrefresh(wingriglia);
				werase(winscore);
				if(boat_plc_p1 < MIN_BOAT || boat_plc_p2 < MIN_BOAT) {
					mvwprintw(winscore, 8, 1, "P2|P1 NOT SET!");
					break;
				}
				started = true;
				for(int i = 0; i < 10; i++) {
					memcpy(griglia[i], griglia_p1_shoot[i], 10);
				}
				turn = 0;
				break;
			case 'b':
				wrefresh(wingriglia);
				werase(stdscr);
				printmain(20, 20, &griglia_p2[pos_y-offset][pos_x-offset]);
				if (turn == 0 && griglia_p2[pos_y-offset][pos_x-offset] == BOAT && started == true && griglia_p1_shoot[pos_y-offset][pos_x-offset] == 0) {
					turn = 1;
					griglia[pos_y-offset][pos_x-offset] = 'C';
					for(int i = 0; i < 10; i++) {
						memcpy(griglia[i], griglia_p2_shoot[i], 10);
					}
					player_1_score++;
					boat_plc_p2--;
					printmain(14, 1, "P1 HIT");
					printmain(15, 1, &griglia_p2[pos_y-offset][pos_x-offset]);
					wrefresh(lettere);
					break;	
					
				}
				if (turn == 0 && griglia_p2[pos_y-offset][pos_x-offset] == 0 && started == true && griglia_p1_shoot[pos_y-offset][pos_x-offset] == ' ') {
					turn = 1;
					griglia[pos_y-offset][pos_x-offset] = 'M';
					for(int i = 0; i < 10; i++) {
						memcpy(griglia[i], griglia_p2_shoot[i], 10);
					}
					printmain(14, 1, "P1 MISS");
					printmain(15, 1, &griglia_p2[pos_y-offset][pos_x-offset]);
					wrefresh(lettere);
					break;	
				}
				if (turn == 1 && griglia_p1[pos_y-offset][pos_x-offset] == BOAT && started == true && griglia_p2_shoot[pos_y-offset][pos_x-offset] == ' ') {
					turn = 0;
					griglia[pos_y-offset][pos_x-offset] = 'C';
					for(int i = 0; i < 10; i++) {
						memcpy(griglia[i], griglia_p1_shoot[i], 10);
					}
					player_2_score++;
					boat_plc_p1--;
					printmain(14, 1, "P2 HIT");
					printmain(15, 1, &griglia_p1[pos_y-offset][pos_x-offset]);
					wrefresh(lettere);
					break;	
				}
				if (turn == 1 && griglia_p1[pos_y-offset][pos_x-offset] == 0 && started == true && griglia_p2_shoot[pos_y-offset][pos_x-offset] == 0) {
					turn = 0;
					griglia[pos_y-offset][pos_x-offset] = 'M';
					for(int i = 0; i < 10; i++) {
						memcpy(griglia[i], griglia_p1_shoot[i], 10);
					}
					printmain(14, 1, "P2 MISS");
					printmain(15, 1, &griglia_p1[pos_y-offset][pos_x-offset]);
					wrefresh(lettere);
					break;	
				}
				break;
			case 10:
				werase(winscore); 
				if(turn == 0 && started == false && boat_plc_p1 >= MIN_BOAT) {
					//griglia_p1 = griglia;
					werase(wingriglia);
					for(int i = 0; i < 10; i++) {
						memcpy(griglia[i], griglia_p2[i], 10);
					}
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
				for(int i = 0; i < 10; i++) {
					free(griglia_p1[i]);
					free(griglia_p2[i]);
					free(griglia_p1_shoot[i]);
					free(griglia_p2_shoot[i]);
					free(griglia[i]);
				}
				free(griglia_p1);
				free(griglia_p2);
				free(griglia_p1_shoot);
				free(griglia_p2_shoot);
				free(griglia);
				endwin();
				return 0;
		}
		//Controllo se un player ha vinto
		if(started == true && boat_plc_p1 == 0) {
			werase(wingriglia);
			werase(winmove);
			werase(winscore);
			printmain(0, 0, "HA VINTO IL GIOCATORE P2!");
			sleep(2);
			endwin();
			return 0;
		}
		if(started == true && boat_plc_p2 == 0) {
			werase(wingriglia);
			werase(winmove);
			werase(winscore);
			printmain(0, 0, "HA VINTO IL GIOCATORE P1!");
			sleep(2);
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
	//Chiudo le finestre
	endwin();
}
void refreshgriglia(WINDOW *wingriglia,char **griglia) {
	for(int i = 0;i<10; ++i) {
		for(int j = 0; j<10; ++j) {
			mvwprintw(wingriglia, i+1, j+1, "%c", griglia[i][j]);
			//griglia[i][j]++;
		}
		printw("\n");
		wrefresh(wingriglia);
	}
}
void printmain(int y, int x, char *mes) {
	mvwprintw(stdscr, y, x, "%s", mes);
	wrefresh(stdscr);
}
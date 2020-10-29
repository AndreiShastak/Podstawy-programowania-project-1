#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<cstdio>
#include<cstring>
#include"conio2.h"
#include <Windows.h>

#define OBRAZX 30
#define OBRAZY 1
#define MENUX 1
#define MENUY 1

int minimum(int a1, int a2) {
	if (a1<a2) {
		return a1;
	}
	else {
		return a2;
	}
}

int maximum(int a1, int a2) {
	if (a1>a2) {
		return a1;
	}
	else {
		return a2;
	}
}

struct prostokat {
	int x_pocz;
	int y_pocz;
	int x_koniec;
	int y_koniec;
	int kolor;
};

void wczytaj_string(char* komunikat, char* string) {
	clrscr();
	textbackground(BLACK);
	textcolor(WHITE);
	gotoxy(MENUX, MENUY);
	cputs(komunikat);
	gotoxy(MENUX, MENUY+1);
	char zn = getche();
	int i = 0;
	while (zn != '\r') {
		string[i] = zn;
		zn = getche();
		i++;
	}
	string[i] = 0;
}

void wczytaj_liczbe(char* komunikat, int* liczba) {
	char string[30];
	wczytaj_string(komunikat, string);
	*liczba = atoi(string);
}

struct obraz {
	int piksele[200][200];
	int szerokosc;
	int wysokosc;
	int kursorx;
	int kursory;
	int attr;
};

void zmien_rozmiar(obraz* obr, int nowa_szerokosc, int nowa_wysokosc) {
	obr->szerokosc = nowa_szerokosc;
	obr->wysokosc = nowa_wysokosc;
	obr->kursorx = 0;
	obr->kursory = 0;
	obr->attr = BLACK;
}

void wypelnij(obraz* obr,int kolor) {

	for (int y = 0; y < obr->wysokosc; y++) {
		for (int x = 0; x < obr->szerokosc; x++) {
			obr->piksele[y][x] = kolor;
		}
	}
}

void ruch(obraz* obr, int zmianax, int zmianay) {
	int nowyx = obr->kursorx + zmianax;
	int nowyy = obr->kursory + zmianay;
	if (0 <= nowyx && nowyx < obr->szerokosc && 0 <= nowyy && nowyy < obr->wysokosc) {
		obr->kursorx = nowyx;
		obr->kursory = nowyy;
	}
}

void wypisz_pozycje_kursora(obraz* obr) {
	textbackground(BLACK);
	textcolor(WHITE);
	gotoxy(MENUX, MENUY + 28);
	char string[3];
	cputs("Kursor: x=");
	string[0] = '0' + obr->kursorx/10;
	string[1] = '0' + obr->kursorx % 10;
	string[2] = 0;
	cputs(string);
	cputs(", y=");
	string[0] = '0' + obr->kursory / 10;
	string[1] = '0' + obr->kursory % 10;
	string[2] = 0;
	cputs(string);
}

void wypisz_wymiary(obraz* obr) {
	textbackground(BLACK);
	textcolor(WHITE);
	gotoxy(MENUX, MENUY + 25);
	char string[3];
	cputs("Schowek: x=");
	string[0] = '0' + obr->szerokosc / 10;
	string[1] = '0' + obr->szerokosc % 10;
	string[2] = 0;
	cputs(string);
	cputs(", y=");
	string[0] = '0' + obr->wysokosc / 10;
	string[1] = '0' + obr->wysokosc % 10;
	string[2] = 0;
	cputs(string);
}

void rysuj(obraz* obr) {

	for (int y = 0; y < obr->wysokosc; y++) {
		gotoxy(OBRAZX, OBRAZY + y);
		for (int x = 0; x < obr->szerokosc; x++) {
			textbackground(obr->piksele[y][x]);
			if (x == obr->kursorx && y == obr->kursory) {
				textcolor(obr->attr);
			}
			else {
				textcolor(obr->piksele[y][x]);
			}
			putch('*');
		}
	}
}

void zmien_kolor(obraz* obr, int kolor) {
	obr->piksele[obr->kursory][obr->kursorx] = kolor;
}

void wczytaj(obraz* obr, const char* nazwa) {
	FILE* plik = fopen(nazwa, "r");
	if (plik == NULL) {
		cputs("Nie udalo sie.");
	}
	else {
		int n_wysokosc;
		int n_szerokosc;
		fscanf(plik, "%d%d", &n_szerokosc, &n_wysokosc);
		zmien_rozmiar(obr, n_szerokosc, n_wysokosc);
		for (int y = 0; y < obr->wysokosc; y++) {
			char in[2];
			for (int x = 0; x < obr->szerokosc; x++) {
				fscanf(plik, "%1s", in);
				obr->piksele[y][x] = in[0] - 'A';
			}
		}
		fclose(plik);
	}

}

void zapisz(obraz* obr, const char* nazwa) {
	FILE* plik = fopen(nazwa, "w");
	if (plik == NULL) {
		cputs("Nie udalo sie.");
	}
	else {
		fprintf(plik, "%d %d\n", obr->szerokosc, obr->wysokosc);
		for (int y = 0; y < obr->wysokosc; y++) {
			char out;
			for (int x = 0; x < obr->szerokosc; x++) {
				char out = obr->piksele[y][x] + 'A';
				fprintf(plik, "%c ", out);
			}
			fprintf(plik, "\n");
		}
		fclose(plik);
	}

}

void kopiuj(obraz* zrodlo, obraz* cel, prostokat* p) {
	
	int x_start = minimum(p->x_koniec, p->x_pocz);
	int y_start = minimum(p->y_koniec, p->y_pocz);
	int x_koniec = maximum(p->x_koniec, p->x_pocz);
	int y_koniec = maximum(p->y_koniec, p->y_pocz);
	int szerokosc = x_koniec - x_start + 1;
	int wysokosc = y_koniec - y_start + 1;
	zmien_rozmiar(cel, szerokosc, wysokosc);
	for (int y = 0; y < wysokosc; y++) {
		for (int x = 0; x < szerokosc; x++) {
			cel->piksele[y][x] = zrodlo->piksele[y_start + y][x_start + x];
		}
	}
}

bool czy_mozna_wkleic_cale(obraz* zrodlo, obraz* cel) {
	int x_start = cel->kursorx;
	int y_start = cel->kursory;
	int x_koniec = minimum(x_start + zrodlo->szerokosc, cel->szerokosc);
	int y_koniec = minimum(y_start + zrodlo->wysokosc, cel->wysokosc);
	if (x_koniec == x_start + zrodlo->szerokosc && y_koniec == y_start + zrodlo->wysokosc) {
		return true;
	}
	else {
		return false;
	}
}

void wklej(obraz* zrodlo, obraz* cel) {
	int x_start = cel->kursorx;
	int y_start = cel->kursory;
	int x_koniec = minimum(x_start + zrodlo->szerokosc, cel->szerokosc);
	int y_koniec = minimum(y_start + zrodlo->wysokosc, cel->wysokosc);
	for (int y = y_start; y < y_koniec; y++) {
		for (int x = x_start; x < x_koniec; x++) {
			cel->piksele[y][x] = zrodlo->piksele[y - y_start][x - x_start];
		}
	}
}


void rysuj_prostokat(prostokat*  p) {
	int x_start = minimum(p->x_koniec, p->x_pocz);
	int y_start = minimum(p->y_koniec, p->y_pocz);
	int x_koniec = maximum(p->x_koniec, p->x_pocz) + 1;
	int y_koniec = maximum(p->y_koniec, p->y_pocz) + 1;
	for (int y = y_start; y < y_koniec; y++) {
		for (int x = x_start; x < x_koniec; x++) {
			gotoxy(OBRAZX + x, OBRAZY + y);
			textbackground(p->kolor);
			textcolor(p->kolor);

			putchar('*');
			
		}
	}
}

void wklej_prostokat(obraz* obr, prostokat*  p) {
	int x_start = minimum(p->x_koniec, p->x_pocz);
	int y_start = minimum(p->y_koniec, p->y_pocz);
	int x_koniec = maximum(p->x_koniec, p->x_pocz) + 1;
	int y_koniec = maximum(p->y_koniec, p->y_pocz) + 1;
	for (int y = y_start; y < y_koniec; y++) {
		for (int x = x_start; x < x_koniec; x++) {
			obr->piksele[y][x] = p->kolor;
		}
	}
}

void menu(obraz* obr, obraz* schowek, bool tryb_rys_prost, bool tryb_zaznacz, char* nazwa) {
	textcolor(WHITE);
	textbackground(BLACK);
	gotoxy(MENUX, MENUY);
	cputs("nazwa: ");
	cputs(nazwa);
	gotoxy(MENUX, MENUY + 1);
	cputs("strzalki = poruszanie");
	gotoxy(MENUX, MENUY + 2);
	cputs("esc=wyjscie/anulowanie trybu");
	gotoxy(MENUX, MENUY + 3);
	cputs("i=wczytanie pliku z obrazkiem");
	gotoxy(MENUX, MENUY + 4);
	cputs("o=wczytac plik+podac nazwe");
	gotoxy(MENUX, MENUY + 5);
	cputs("n=nowy plik+podac nazwe");
	gotoxy(MENUX, MENUY + 6);
	cputs("s=zapis pliku+podac nazwe");
	gotoxy(MENUX, MENUY + 7);
	cputs("l=rysowanie linii");
	gotoxy(MENUX, MENUY + 8);
	cputs("k=rysowanie prostokata");
	gotoxy(MENUX, MENUY + 9);
	cputs("o=wczytanie pliku+nazwa");
	gotoxy(MENUX, MENUY + 10);
	cputs("1234567890qwerty=kolory");
	gotoxy(MENUX, MENUY + 11);
	cputs("alt+strzalki=zaznacznie");
	gotoxy(MENUX, MENUY + 12);
	cputs("obszaru do kopiowania");
	gotoxy(MENUX, MENUY + 13);
	cputs("c=kopiuj fragment");
	
	textcolor(YELLOW);
	if (tryb_zaznacz == true) {
		gotoxy(MENUX, MENUY + 17);
		cputs("tryb zaznacznia");
		gotoxy(MENUX, MENUY + 18);
		cputs("c=skopiuj do schowka");

	}
	else if (tryb_rys_prost == true) {
		gotoxy(MENUX, MENUY + 19);
		cputs("tryb prostokata");
		gotoxy(MENUX, MENUY + 20);
		cputs("k=zapisz prostakat");
	}
	
	if (czy_mozna_wkleic_cale(schowek,obr) == true) {
		gotoxy(MENUX, MENUY + 21);
		cputs("mozna wkleic w calosci");
		gotoxy(MENUX, MENUY + 22);
		cputs("p=wklej ze schowka");
	}
}

int main(int argc, char**argv) {
	int zn = 0, attr = 7, back = 0, zero = 0;
	char txt[32] = "kod klawisza: 0x";
	// jeїeli program jest kompilowany w czystym jкzyku C
	// proszк odkomentowaж poniїsz№ liniк
	// Conio2_Init();
	settitle("Andrej Shastak 165666");
	obraz obr;
	char nazwa[100] = "beznazwy.txt";
	zmien_rozmiar(&obr, 50, 20);
	wypelnij(&obr, WHITE);
	bool tryb_rys_prost = false;
	bool tryb_zaznacz = false;
	bool nowy_tryb_zaznacz = false;
	bool tryb_linia = false;
	bool czy_zamknac = false;

	prostokat rys_prost;
	obraz schowek;
	zmien_rozmiar(&schowek, 0, 0);
	prostokat zaznacz;
	if (argc > 1) {
		wczytaj(&obr, argv[1]);
	}
	do {
		textbackground(BLACK);
		textcolor(WHITE);
		obr.attr = attr;
		clrscr();
		menu(&obr, &schowek, tryb_rys_prost, tryb_zaznacz, nazwa);
		rysuj(&obr);
		wypisz_pozycje_kursora(&obr);
		wypisz_wymiary(&schowek);
		if (tryb_rys_prost == true) {
			rys_prost.kolor = attr;
			rys_prost.x_koniec = obr.kursorx;
			rys_prost.y_koniec = obr.kursory;
			rysuj_prostokat(&rys_prost);
		}
		zero = 0;
		zn = getch();
		nowy_tryb_zaznacz = false;
		if (zn == 0) {
			zero = 1;
			zn = getch();
			if (zn == 0x48) {
				ruch(&obr, 0, -1);
			}
			else if (zn == 0x50) {
				ruch(&obr, 0, 1);
			}

			else if (zn == 0x4b) {
				ruch(&obr, -1, 0);
			}
			else if (zn == 0x4d) {
				ruch(&obr, 1, 0);
			}

			else if (zn == 0x98) {
				if (tryb_zaznacz == false) {
					zaznacz.x_pocz = obr.kursorx;
					zaznacz.y_pocz = obr.kursory;
					tryb_zaznacz = true;
				}
				nowy_tryb_zaznacz = true;
				ruch(&obr, 0, -1);
			}
			else if (zn == 0xa0) {
				if (tryb_zaznacz == false) {
					zaznacz.x_pocz = obr.kursorx;
					zaznacz.y_pocz = obr.kursory;
					tryb_zaznacz = true;
				}
				nowy_tryb_zaznacz = true;
				ruch(&obr, 0, 1);
			}
			else if (zn == 0x9b) {
				if (tryb_zaznacz == false) {
					zaznacz.x_pocz = obr.kursorx;
					zaznacz.y_pocz = obr.kursory;
					tryb_zaznacz = true;
				}
				nowy_tryb_zaznacz = true;
				ruch(&obr, -1, 0);
					
			}
			else if (zn == 0x9d) {
				if (tryb_zaznacz == false) {
					zaznacz.x_pocz = obr.kursorx;
					zaznacz.y_pocz = obr.kursory;
					tryb_zaznacz = true;
				}
				nowy_tryb_zaznacz = true;
				ruch(&obr, 1, 0);
			}

		}
		else if (zn == ' ') attr = (attr + 1) % 16;
		else if (zn == 0x0d) back = (back + 1) % 16;
		else if (zn == '1') {
			zmien_kolor(&obr, BLUE);
		}
		else if (zn == '2') {
			zmien_kolor(&obr, GREEN);
		}
		else if (zn == '3') {
			zmien_kolor(&obr, CYAN);
		}
		else if (zn == '4') {
			zmien_kolor(&obr, RED);
		}
		else if (zn == '5') {
			zmien_kolor(&obr, MAGENTA);
		}
		else if (zn == '6') {
			zmien_kolor(&obr, BROWN);
		}
		else if (zn == '7') {
			zmien_kolor(&obr, LIGHTGRAY);
		}
		else if (zn == '8') {
			zmien_kolor(&obr, DARKGRAY);
		}
		else if (zn == '9') {
			zmien_kolor(&obr, LIGHTBLUE);
		}
		else if (zn == '0') {
			zmien_kolor(&obr, BLACK);
		}
		else if (zn == 'q' || zn == 'Q') {
			zmien_kolor(&obr, LIGHTGREEN);
		}
		else if (zn == 'w' || zn == 'W') {
			zmien_kolor(&obr, LIGHTCYAN);
		}
		else if (zn == 'e' || zn == 'E') {
			zmien_kolor(&obr, LIGHTRED);
		}
		else if (zn == 'r' || zn == 'R') {
			zmien_kolor(&obr, LIGHTMAGENTA);
		}
		else if (zn == 't' || zn == 'T') {
			zmien_kolor(&obr, YELLOW);
		}
		else if (zn == 'y' || zn == 'Y') {
			zmien_kolor(&obr, WHITE);
		}
		else if (zn == 'i' || zn == 'I') {
			wczytaj(&obr, "domyslny.txt");
		}
		else if (zn == 'o' || zn == 'O') {
			wczytaj_string("Nazwa pliku:",nazwa);
			wczytaj(&obr, nazwa);
		}
		else if (zn == 's' || zn == 'S') {
			char nowa_nazwa[100];
			wczytaj_string("Nowa nazwa pliku (enter=poprzednia):", nowa_nazwa);
			if (nowa_nazwa[0] != 0) {
				int i = 0;
				while (nowa_nazwa[i] != 0) {
					nazwa[i] = nowa_nazwa[i];
				}
				nazwa[i] = 0;
			}
			zapisz(&obr, nazwa);
		}
		else if (zn == 'n' || zn == 'N') {
			int wysokosc;
			int szerokosc;
			wczytaj_string("Nazwa pliku:", nazwa);
			wczytaj_liczbe("Szerokosc:", &szerokosc);
			wczytaj_liczbe("Wysokosc:", &wysokosc);
			zmien_rozmiar(&obr,  szerokosc, wysokosc);
			wypelnij(&obr, WHITE);
			
		}
		else if (zn == 'k' || zn == 'K') {

			if (tryb_rys_prost == false) {
				rys_prost.x_pocz = obr.kursorx;
				rys_prost.y_pocz = obr.kursory;
				tryb_rys_prost = true;
			}
			else {
				wklej_prostokat(&obr, &rys_prost);
				tryb_rys_prost = false;
			}

		}
		else if (zn == 'c' || zn == 'C') {
			if (tryb_zaznacz == true) {
				zaznacz.x_koniec = obr.kursorx;
				zaznacz.y_koniec = obr.kursory;
				kopiuj(&obr, &schowek, &zaznacz);
			}
		}
		else if (zn == 'p' || zn == 'P') {

			wklej(&schowek, &obr);
		}
		else if (zn == 0x1b) {
			if (tryb_rys_prost == true ) {
				tryb_rys_prost = false;
			}
			else {
				czy_zamknac = true;
			}
		}
	tryb_zaznacz = nowy_tryb_zaznacz;
	} while (czy_zamknac == false);
	
	return 0;
}


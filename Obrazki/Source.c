#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>


struct obraz
{
	char standard[3];
	int szerokosc;
	int wysokosc;
	int skala_szarosci;
	char nazwa[256];
	int** pixele;
	int licz_kom;
	char kom[32][128];
	struct obraz* nastepny;
};

void Wczytaj_plik_pgm(struct obraz* Obraz)
{
	FILE* plik;
	do {
		printf("Podaj nazwe pliku .PGM: ");
		(void)scanf("%s\0", &Obraz->nazwa);
	} while ((plik = fopen(Obraz->nazwa, "r")) == NULL);
	(void)fscanf(plik, "%s", Obraz->standard);
	if (Obraz->standard[1] == '5')
	{
		printf("xD");
		fclose(plik);
		Wczytaj_plik_pgm(Obraz);
	}
	else
	{
		Obraz->standard[2] = '\0';
		char znak = 0;
		int tmp = 0, licznik = 0, x = 0, y = 0;
		Obraz->licz_kom = 0;
		znak = getc(plik);
		while ((znak = getc(plik)) != EOF)
		{
			if (znak == 35) { //35 to # w ascii
				int i;
				for (i = 0; (znak = getc(plik)) != '\n'; i++) {
					Obraz->kom[Obraz->licz_kom][i] = znak;     
				}
				Obraz->kom[Obraz->licz_kom][i] = '\0';
				Obraz->licz_kom++;
			}
			else if (znak>47 && znak<58) //47 to 0 a 58 to 9 w ascii
			{
				fseek(plik, -1, SEEK_CUR);
				if (licznik == 0) {
					(void)fscanf(plik, "%d", &Obraz->szerokosc);
					licznik++;
				}
				else if (licznik == 1) {
					(void)fscanf(plik, "%d", &Obraz->wysokosc);
					licznik++;
				}

				else if (licznik == 2) {
					(void)fscanf(plik, "%d", &Obraz->skala_szarosci);
					licznik++;
					Obraz->pixele = calloc((size_t)Obraz->wysokosc, sizeof(int));
					for (int i = 0; i < Obraz->wysokosc; i++) {
						Obraz->pixele[i] = calloc((size_t)Obraz->szerokosc, sizeof(int));
					}
				}
				else if (Obraz->pixele && y < Obraz->wysokosc) {
					(void)fscanf(plik, "%d", &Obraz->pixele[y][x]);
					x++;
					if (x == Obraz->szerokosc) {
						y++;
						x = 0;
					}
				}
				else break;
			}
		}
	}
	fclose(plik);
}
void Wypisz_dane(struct obraz *Obraz)
{
	printf("%s\n", Obraz->standard);
	printf("%d ", Obraz->szerokosc);
	printf("%d\n", Obraz->wysokosc);
	printf("%d\n", Obraz->skala_szarosci);
	for (int i = 0; i < Obraz->wysokosc; i++) {
		for (int j = 0; j < Obraz->szerokosc; j++) {
			printf("%d ", Obraz->pixele[i][j]);
		}
		printf("\n");
	}
}
void Wypisz_komentarze(struct obraz *Obraz)
{
	for (int i = 0; i < Obraz->licz_kom; i++) {
		printf("%s\n", Obraz->kom[i]);
	}
}

void Zapis_do_pliku(struct obraz* Obraz)
{

	FILE* plik;
	char buf[100];
	printf("Jak chcesz nazwac swoj plik?: ");
	(void)scanf("%s\0", buf);
	(void)strcat(buf,".pgm\0");
	if ((plik = fopen(buf, "w")) == NULL) {
		printf("Nie udalo sie otworzyc pliku przy zapisywaniu do pliku. Sprobuj inna opcje.\n");
	}
	else
	{
		fprintf(plik,"%s\n", Obraz->standard);
		fprintf(plik, "%d ", Obraz->szerokosc);
		fprintf(plik, "%d\n", Obraz->wysokosc);
		fprintf(plik, "%d\n", Obraz->skala_szarosci);
		for (int i = 0; i < Obraz->wysokosc; i++) {
			for (int j = 0; j < Obraz->szerokosc; j++) {
				fprintf(plik, "%d ", Obraz->pixele[i][j]);
			}
			fprintf(plik, "\n");
		}
		fclose(plik);
	}
}
void Wyswietl_liste_obrazow(struct obraz* Obraz, int ile_obrazow, int aktywny)
{
	struct obraz* kursor=0;
	for (int i = 0; i < ile_obrazow; i++) {
		kursor = &Obraz[i];
		if (aktywny == i) printf("Aktywny: ");
		printf("%d.%s\n", i + 1, kursor->nazwa);
	}
}
int Wybierz_aktywny(struct obraz* kursor, int ile_obrazow, struct obraz* Obraz,int aktywny) {
	printf("Ktory obraz ma byc aktywny?\n");
	for (int i = 0; i < ile_obrazow; i++) {
		kursor = &Obraz[i];
		if (aktywny == i) printf("Aktywny: ");
		printf("%d.%s\n", i+1, kursor->nazwa);
	}
	printf("Podaj jego numer: ");
	(void)scanf("%d", &aktywny);
	return aktywny - 1;
}
int Usun_obraz(struct obraz* kursor, int ile_obrazow, struct obraz* Obraz,int aktywny)
{
	int usun = 0;
	for (int i = 0; i < ile_obrazow; i++) {
		kursor = &Obraz[i];
		if (aktywny == i) printf("Aktywny: ");
		printf("%d.%s\n", i + 1, kursor->nazwa);
	}
	printf("Podaj jego numer: ");
	(void)scanf("%d", &usun);
	usun = usun - 1;
	int przesun = usun + 1;
	struct obraz * kursor2 = 0;
	kursor2 = &Obraz[usun];
	for (int i = kursor2->wysokosc - 1; i >= 0; i--)
	{
		free(kursor2->pixele[i]);
	}
	free(kursor2->pixele);
	while (przesun < ile_obrazow)
	{
		kursor = &Obraz[przesun];
		Obraz[przesun - 1] = *kursor;
		przesun++;
	}
	return usun;
}

void zakoncz_program(struct obraz* kursor, int ile_obrazow, struct obraz* Obraz)
{
	--ile_obrazow;
	struct obraz* tmp;
	while (ile_obrazow)
	{
		if (kursor && Obraz) {
			kursor = &Obraz[ile_obrazow];
			for (int i = kursor->wysokosc - 1; i >= 0; i--)
			{
				free(kursor->pixele[i]); 
			}
			if (kursor && Obraz && kursor->pixele) free(kursor->pixele);
			if (Obraz) {
				tmp = (struct obraz*)realloc(Obraz, sizeof(struct obraz) * (ile_obrazow--));
				if (tmp) Obraz = tmp;
			}
		}
	}
}
void skopiuj_na_koniec_bazy(struct obraz* kursor, int ile_obrazow, int aktywny, struct obraz* Obraz)
{
	struct obraz* kursor2 = &Obraz[ile_obrazow-1];
	kursor = &Obraz[aktywny];
	kursor2->standard[0]= kursor->standard[0];
	kursor2->standard[1] = kursor->standard[1];
	kursor2->standard[2] = '\0';
	kursor2->szerokosc= kursor->szerokosc;
	kursor2->wysokosc= kursor->wysokosc;
	kursor2->skala_szarosci = kursor->skala_szarosci;
	kursor2->pixele = calloc((size_t)kursor2->wysokosc, sizeof(int));
	for (int i = 0; i < kursor2->wysokosc; i++) {
		if (kursor2->pixele) kursor2->pixele[i] = calloc((size_t)kursor2->szerokosc, sizeof(int));
		else exit(1);
	}
	kursor=&Obraz[aktywny];
	for (int j = 0; j < kursor2->wysokosc; j++) {
		for (int k = 0; k < kursor2->szerokosc; k++) {
			if(kursor && kursor->pixele &&  kursor->pixele[j] && kursor2 && kursor2->pixele && kursor2->pixele[j]) kursor2->pixele[j][k]= (*kursor).pixele[j][k];
		}
	}
	kursor2->nazwa[0] = 'z'; 
	kursor2->nazwa[1] = 'o'; 
	kursor2->nazwa[2] = 'p'; 
	kursor2->nazwa[3] = 'e';
	kursor2->nazwa[4] = 'r';
	kursor2->nazwa[5] = 'o';
	kursor2->nazwa[6] = 'w';
	kursor2->nazwa[7] = 'a';
	kursor2->nazwa[8] = 'n';
	kursor2->nazwa[9] = 'y';
	kursor2->nazwa[10] = ' ';
	kursor2->nazwa[11] = '\0';
	strcat(kursor2->nazwa, kursor->nazwa);
}
int menu_przetwarzanie(struct obraz* kursor)
{
	int menu=0;
	printf("-------MENU PRZETWARZANIA-------\nCo zrobic z obrazem %s?\n1. Obroc w prawo o K*90 stopni\n2. Zapisz histogram do pliku\n", kursor->nazwa);
	printf("3. Negatyw\n4. Dodaj szum typu pieprz i sol\n5.Potraktuj obraz filtrem medianowym\n9.Wroc do menu glownego\n");
	(void)scanf("%d", &menu); // dodaæ ¿e ma byæ z przedzia³u 1-9 inaczej zapytaæ jeszcze raz no i ¿e siê nie wypierdoli na krzywy ryj jak wpisze s³owo
	return menu;
}
void obrot(struct obraz* kursor) 
{
	int k = 0;
	int pixel = 0;
	printf("Obraz zostanie obrocony o K*90 w prawo. Podaj K: ");
	(void)scanf("%d", &k);
	k = k % 4;
	if (k == 0);
	else if (k == 2)
	{
		for (int i = 0; i < kursor->wysokosc/2; i++) {
			for (int j = 0; j < kursor->szerokosc; j++) {
				pixel = kursor->pixele[i][j];
				kursor->pixele[i][j] = kursor->pixele[kursor->wysokosc - 1 - i][kursor->szerokosc - 1 - j];
				kursor->pixele[kursor->wysokosc - 1 - i][kursor->szerokosc - 1 - j] = pixel;
			}
		}
	}
	else
	{
		int tmp = 0;
		tmp = kursor->wysokosc;
		kursor->wysokosc = kursor->szerokosc;
		kursor->szerokosc = tmp;
		int** obrocony = (int**) calloc(kursor->wysokosc, sizeof(int*));
		for (int i = 0; i < kursor->wysokosc; i++) {
			obrocony[i] = (int*) calloc(kursor->szerokosc, sizeof(int));
		}
		if (k == 1)
		{
			for (int i = 0; i < kursor->wysokosc; i++) {
				for (int j = 0; j < kursor->szerokosc; j++) {
					if (obrocony[i]) obrocony[i][j] =kursor->pixele[kursor->szerokosc-1-j][i];
				}
			}
		}
		else if (k == 3) 
		{
			for (int i = 0; i < kursor->wysokosc; i++) {
				for (int j = 0; j < kursor->szerokosc; j++) {
					obrocony[i][j] = kursor->pixele[j][kursor->wysokosc-1-i];
				}
			}
		}
		if (kursor->pixele) {
			int** tmp = (int**)realloc(kursor->pixele, sizeof(int*) * kursor->wysokosc);
			if (tmp) kursor->pixele = tmp;
			for (int i = 0; i < kursor->wysokosc; i++) 
			{
				if (i < kursor->szerokosc) 
				{
					if (kursor->pixele[i]) {
						tmp[i] = (int*)realloc(kursor->pixele[i], sizeof(int) * kursor->szerokosc);
						if (tmp[i]) kursor->pixele[i] = tmp[i];
					}
				}
				else if (kursor->pixele[i]) kursor->pixele[i] = (int*)calloc(kursor->szerokosc, sizeof(int));
			}
			for (int i = 0; i < kursor->wysokosc; i++) {
				for (int j = 0; j < kursor->szerokosc; j++) {
					if (kursor->pixele[i])  kursor->pixele[i][j]= obrocony[i][j];
				}
			}
		}
		for (int i = kursor->wysokosc-1; i >= 0; i--)
		{
			if(obrocony[i]) free(obrocony[i]);
		}
		if (obrocony) free(obrocony);
	}
}
void histogram(struct obraz* kursor)
{
	FILE* plik;
	char buf[100];
	printf("Jak chcesz nazwac swoj histogram?: ");
	(void)scanf("%s\0", buf);
	(void)strcat(buf, ".csv\0");
	if ((plik = fopen(buf, "w")) == NULL) {
		printf("Nie udalo sie otworzyc pliku przy zapisywaniu do pliku. Sprobuj inna opcje.\n");
	}
	else
	{
		int* ilosc = calloc(kursor->skala_szarosci+1, sizeof(int));
		for (int i = 0; i <= kursor->skala_szarosci; i++) {
			ilosc[i] = 0;
		}
		for (int i = 0; i < kursor->wysokosc; i++) {
			for (int j = 0; j < kursor->szerokosc; j++) {
				 ilosc[kursor->pixele[i][j]]++;
			}
		}
		fprintf(plik, "odcien: ; ilosc: \n");
		for (int i = 0; i <= kursor->skala_szarosci; i++)
		{
			fprintf(plik, "%d; %d\n", i, ilosc[i]);
		}
		fclose(plik);
	}
}
void negatyw(struct obraz* kursor) {
	for (int i = 0; i < kursor->wysokosc; i++) {
		for (int j = 0; j < kursor->szerokosc; j++) {
			kursor->pixele[i][j]= kursor->skala_szarosci-kursor->pixele[i][j];
		}
	}
}
void sol_i_pieprz(struct obraz* kursor)
{
	int random = 0,sol=0,pieprz=255;
	for (int i = 0; i < kursor->wysokosc; i++) {
		for (int j = 0; j < kursor->szerokosc; j++) {
			random = rand() % (int)100;
			if (random == 0)kursor->pixele[i][j] = sol;
			if (random == 1)kursor->pixele[i][j] = pieprz;
		}
	}
}
int odfiltruj(int tablica[9])
{
	int tymczasowa;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8 - i; j++)
		{
			if (tablica[j] > tablica[j + 1])
			{
				tymczasowa = tablica[j];
				tablica[j] = tablica[j + 1];
				tablica[j + 1] = tymczasowa;
			}
		}
	}
	return tablica[4];
}
void filtr_medianowy(struct obraz* kursor)
{
	int tablica[9] = { 0 };
	for (int i = 1; i < kursor->wysokosc-1; i++) {
		for (int j = 1; j < kursor->szerokosc-1; j++) {
			tablica[0] = kursor->pixele[i-1][j-1];
			tablica[1] = kursor->pixele[i-1][j];
			tablica[2] = kursor->pixele[i-1][j+1];
			tablica[3] = kursor->pixele[i][j-1];
			tablica[4] = kursor->pixele[i][j];
			tablica[5] = kursor->pixele[i][j+1];
			tablica[6] = kursor->pixele[i+1][j-1];
			tablica[7] = kursor->pixele[i+1][j];
			tablica[8] = kursor->pixele[i+1][j+1];
			kursor->pixele[i][j] = odfiltruj(tablica);
		}
	}
	kursor->pixele[0][0] = kursor->pixele[1][1];
	kursor->pixele[0][kursor->szerokosc - 1] = kursor->pixele[1][kursor->szerokosc - 2];
	kursor->pixele[kursor->wysokosc - 1][kursor->szerokosc - 1] = kursor->pixele[kursor->wysokosc - 2][kursor->szerokosc - 2];
	kursor->pixele[kursor->wysokosc - 1][0] = kursor->pixele[kursor->wysokosc - 2][1];
	for (int i = 1; i < kursor->wysokosc-1; i++) {
		kursor->pixele[i][0] = kursor->pixele[i][1];
		kursor->pixele[i][kursor->szerokosc - 1] = kursor->pixele[i][kursor->szerokosc - 2];
	}
	for (int i = 1; i < kursor->szerokosc - 1; i++) {
		kursor->pixele[0][i] = kursor->pixele[1][i];
		kursor->pixele[kursor->wysokosc - 1][i] = kursor->pixele[kursor->wysokosc - 2][i];
	}
}
int main()
{
	srand((unsigned int)time(NULL));
	struct obraz *Obraz=0, * kursor=0;
	struct obraz* tmp = 0;
	int ile_obrazow = 0,aktywny=0,usun=0,zmiana=0,czy_nowy=1;
	Obraz = malloc(sizeof(struct obraz));
	if (Obraz) kursor = &Obraz[0];
	int wybor = 0, program_dzialaj = 1;
	while (program_dzialaj)
	{
		printf("-------MENU-------\n1. Dodaj obraz do bazy\n2. Wybierz aktywny obraz\n3. Usun obraz z bazy\n4. Wyswietl liste obrazow\n5.Wykonaj operacje na aktywnym obrazie\n");
		printf("6. Zapisz obraz do pliku\n7. Wypisz dane obrazu\n8.Wypisz komentarze w obrazie\n9.Zakoncz program\n");
		scanf_s("%d", &wybor);
		switch (wybor)
		{
		case 1:
			if (aktywny == -1) Obraz = malloc(sizeof(struct obraz));
			ile_obrazow++; 
			tmp = 0;
			if (Obraz) {
				tmp = (struct obraz*)realloc(Obraz, sizeof(struct obraz) * (ile_obrazow));
				if (tmp) Obraz = tmp;
			}
			aktywny = ile_obrazow - 1;
			kursor = &Obraz[aktywny];
			if(Obraz && kursor) Wczytaj_plik_pgm(kursor);
			break;
		case 2:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else if (kursor && Obraz) {
				aktywny = Wybierz_aktywny(kursor, ile_obrazow,Obraz,aktywny); 
				kursor = &Obraz[aktywny];
			}
			else printf("Error\n");
			break;
		case 3:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else
			{
				usun = Usun_obraz(kursor, ile_obrazow, Obraz, aktywny);
				tmp = 0;
				if (Obraz && ile_obrazow >= 1) {
					tmp = (struct obraz*)realloc(Obraz, sizeof(struct obraz) * (--ile_obrazow));
					if (tmp) Obraz = tmp;
				}
				if (aktywny > usun && aktywny > 0 || aktywny == usun && usun == ile_obrazow) aktywny--;
				if (aktywny == -1) printf("Brak obrazow w bazie\n");
				else {
					kursor = &Obraz[aktywny];
					printf("Obraz usunieto obraz nr %d.\n", usun + 1);
					printf("Obraz %d jest teraz aktywny\n", aktywny + 1);
				}
			}
			break;
		case 4:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else Wyswietl_liste_obrazow(Obraz, ile_obrazow,aktywny);
			break;
		case 5:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else
			{
				zmiana = menu_przetwarzanie(kursor);
				while (zmiana != 9)
				{
					if (czy_nowy)
					{
						czy_nowy = 0;
						ile_obrazow++;
						tmp = 0;
						if (Obraz) {
							tmp = (struct obraz*)realloc(Obraz, sizeof(struct obraz) * (ile_obrazow));
							if (tmp) Obraz = tmp;
						}
						skopiuj_na_koniec_bazy(kursor, ile_obrazow, aktywny, Obraz);
						aktywny = ile_obrazow - 1;
						kursor = &Obraz[aktywny];
					}
					if (zmiana == 1) obrot(kursor);
					else if (zmiana == 2) histogram(kursor);
					else if (zmiana == 3) negatyw(kursor);
					else if (zmiana == 4) sol_i_pieprz(kursor);
					else if (zmiana == 5) filtr_medianowy(kursor);
					zmiana = menu_przetwarzanie(kursor);
				}
				czy_nowy = 1;
			}
			break;
		case 6:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else if(Obraz && kursor) Zapis_do_pliku(kursor);
			break;
		case 7:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else if (Obraz && kursor) Wypisz_dane(kursor);
			break;
		case 8:
			if (aktywny == -1) printf("Brak obrazow w bazie\n");
			else if (Obraz && kursor) Wypisz_komentarze(kursor);
			break;
		case 9:
			system("cls");
			program_dzialaj = 0;
			if(kursor && Obraz) zakoncz_program(kursor, ile_obrazow, Obraz);
			break;
		}
	}
}
/* ==============================================================================
		RAZANAKANAMBININTSOA Joseph
		Programmation Systeme
================================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <string.h>
#include <time.h>
#include <graphics.h>

int hms(char indication); // renvoi l'heure, la minute la seconde. (liste d'arg : 'h', 'm', 's')
void effacerEcran();
void bordureHorloge(); // traçage du forme de l'horloge + chiffre d'indication
void tracerMinuteSeconde(char type, int valeur); // type : 'm' ou 's'
void tracerHeure(int valeurHeure);
void horlogeNumerique(); // Affiche HH:MM:SS
void calendrier(); // Affiche le jour, le mois et l'année => Ex: Lundi 12 Févririer 2024. 

void main()
{
	int h = 0, m = 0, s = 0;

	// Graph initialisation :
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

	while(1)
	{
		effacerEcran();
		s = hms('s');
		m = hms('m');
		h = hms('h');

		bordureHorloge();
		tracerMinuteSeconde('s', s);
		tracerMinuteSeconde('m', m);

		if(m >= 40)
		{
			if(h == 23 || h == 11)
			{
				tracerHeure(0);
			}
			else
			{
				tracerHeure(h+1);
			}
		}
		else
		{
			tracerHeure(h);
		}

		
		if(h == 24)
		{
			tracerHeure(0);
		}
		

		horlogeNumerique();
		calendrier();
		delay(1000);
	}
}

// Recupere heure, minute, seconde :
int hms(char indication)
{
	union REGS entre, horaire;
	int res = 0;

	entre.h.ah = 0x2c;
	int86(0x21, &entre, &horaire);

	switch(indication)
	{
		case 'h':
			res = horaire.h.ch;
			break;
		case 'm':
			res = horaire.h.cl;
			break;
		case 's':
			res =  horaire.h.dh;
			break;
	}
	return res;
}

void effacerEcran() {
	union REGS entree, sortie;

	entree.h.ah = 0x06;
	entree.h.al = 0x00;
	entree.h.bh = 0x00; // ecran noir.
	entree.h.ch = 0x00;
	entree.h.cl = 0x00;
	entree.h.dh = 0x24;
	entree.h.dl = 0x79;
	int86(0x10, &entree, &sortie);
}

void bordureHorloge()
{
	// Bordure externe de l'horloge (carre)
	line(100, 100, 400, 100); // haut
	line(100, 400, 400, 400); // bas
	line(100, 100, 100, 400); // gauche
	line(400, 100, 400, 400); // droite

	// Indications heure 1 a 12 :
	line(325, 90, 325, 100); // 1
	line(400, 175, 410, 175);  // 2
	line(400, 250, 410, 250); // 3
	line(400, 325, 410, 325); // 4
	line(325, 400, 325, 410); // 5
	line(250, 400, 250, 410); // 6
	line(175, 400, 175, 410); // 7
	line(90, 325, 100, 325); // 8
	line(90, 250, 100, 250); // 9
	line(90, 175, 100, 175); // 10
	line(175, 90, 175, 100); // 11
	line(250, 90, 250, 100); // 12

	// Chiffre 1 a 12 :
	outtextxy(322, 80, "1");
	outtextxy(415, 170, "2");
	outtextxy(415, 247, "3");
	outtextxy(415, 320, "4");
	outtextxy(322, 413, "5");
	outtextxy(247, 413, "6");
	outtextxy(172, 412, "7");
	outtextxy(80, 322, "8");
	outtextxy(80, 247, "9");
	outtextxy(72, 172, "10");
	outtextxy(169, 80, "11");
	outtextxy(245, 80, "12");
}

void tracerMinuteSeconde(char type, int valeur)
{
	int milieuX = 250, milieuY = 250, rayon = 0, angle = 0;
	int x2, y2, variation;

	if(type == 's')
	{
		rayon = 150;
	}
	else
	{
		rayon = 100;
	}

	if(valeur >= 15 || valeur <= 59)
	{
		variation = valeur - 15;
	}
	else
	{
		valeur += 60;
		variation = valeur - 15;
	}

	angle = variation * 6;

	x2 = (int) milieuX + rayon * cos(angle * M_PI/180);
	y2 = (int) milieuY + rayon * sin(angle * M_PI/180);
	line(milieuX, milieuY, x2, y2);
}

void tracerHeure(int valeurHeure)
{
    int angle[12] = {270, 306, 336, 6, 36, 66, 96, 126, 156, 186, 216, 246};
    int milieuX = 250, milieuY = 250;
    int x2 = 0, y2 = 0;
    int rayon = 55;

    if(valeurHeure >= 12)
    {
		valeurHeure -= 12;
    }

    if(valeurHeure == 0)
    {
		x2 = (int) milieuX + rayon * cos(269 * M_PI / 180);
		y2 = (int) milieuY + rayon * sin(269 * M_PI / 180);
    }
	else 
	{
		x2 = (int) milieuX + rayon * cos(angle[valeurHeure] * M_PI / 180);
    	y2 = (int) milieuY + rayon * sin(angle[valeurHeure] * M_PI / 180);
	}
    

    line(milieuX, milieuY, x2, y2);
}

void horlogeNumerique()
{
	int h = hms('h'), m = hms('m'), s = hms('s');
	char chaineHorloge[20] = "";

	sprintf(chaineHorloge, "%d:%d:%d", h, m, s);
	outtextxy(220, 310, chaineHorloge);
}

void calendrier()
{
	time_t t = time(NULL);
	struct tm *tempsinfo = localtime(&t);
	char jourSemaineFrancais[20];
	union REGS entree, sortie;
	int annee, mois, jour, jourSemaine; 
	char cal[30], moisChaine[20]; 

	 jourSemaine = tempsinfo->tm_wday; 

	switch(jourSemaine)
	{
		case 0: 
			strcpy(jourSemaineFrancais, "dimanche"); 
			break; 
		case 1: 
			strcpy(jourSemaineFrancais, "lundi"); 
			break; 
		case 2: 
			strcpy(jourSemaineFrancais, "mardi"); 
			break; 
		case 3: 
			strcpy(jourSemaineFrancais, "mercredi"); 
			break; 
		case 4: 
			strcpy(jourSemaineFrancais, "jeudi"); 
			break; 
		case 5: 
			strcpy(jourSemaineFrancais, "vendredi"); 
			break; 
		case 6: 
			strcpy(jourSemaineFrancais, "samedi"); 
			break; 
	}
	
	
	entree.h.ah = 0x2A; 
	int86(0x21, &entree, &sortie); 

	annee = sortie.x.cx;
	mois = sortie.h.dh; 
	jour = sortie.h.dl; 

	switch(mois)
	{
		case 1:
			strcpy(moisChaine, "janvier"); 
			break; 
		case 2:
			strcpy(moisChaine, "fevrier"); 
			break; 
		case 3:
			strcpy(moisChaine, "mars"); 
			break; 
		case 4:
			strcpy(moisChaine, "avril"); 
			break; 
		case 5:
			strcpy(moisChaine, "mai"); 
			break; 
		case 6:
			strcpy(moisChaine, "juin"); 
			break; 
		case 7:
			strcpy(moisChaine, "juillet"); 
			break; 
		case 8:
			strcpy(moisChaine, "aout"); 
			break; 
		case 9:
			strcpy(moisChaine, "sepembre"); 
			break; 
		case 10:
			strcpy(moisChaine, "octobre"); 
			break; 
		case 11:
			strcpy(moisChaine, "novembre"); 
			break; 
		case 12:
			strcpy(moisChaine, "decembre"); 
			break; 
	}

	sprintf(cal, "%s %d %s %d", jourSemaineFrancais, jour, moisChaine, annee); 
	outtextxy(150, 330, cal);
}

/*!
 \file
 \page Général

 \author GOURDON Gabriel
 \version 1.0
 \date 22 octobre 2024
 \brief Un snake pour la SAE 1.01
**/

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Déclaration des constantes
/**
 * \def TAILLE_TABLEAU
 * \brief La taille du tableau dans lequel le serpent va évoluer
 */
#define TAILLE_TABLEAU 40

/**
 * \def TAILLE_SERPENT
 * \brief La taille du serpent
 * Elle ne changera pas pour le moment
 */
#define TAILLE_SERPENT 10

/**
 * \def VITESSE_JEU
 * \brief La vitesse à laquelle le jeu va avancer (en microsecondes)
 */
#define VITESSE_JEU 200000

/**
 * \def TOUCHE_ARRET
 * \brief La touche sur laquelle appuyer pour arrêter le jeu
 */
#define TOUCHE_ARRET 'a'

/**
 * \def CHAR_TETE
 * \brief Le caractère qui correspond à la tête du serpent
 */
#define CHAR_TETE 'O'

/**
 * \def CHAR_CORPS
 * \brief Le caractère qui correspond au corps du serpent
 */
#define CHAR_CORPS 'X'

#define X_DEBUT 20
#define Y_DEBUT 20

#define DIRECTION_INITIALE 'd'
/**
 * \def TOUCHE_DROITE
 * \brief La touche à appuyer pour aller vers la droite
 */
#define TOUCHE_DROITE 'd'

/**
 * \def TOUCHE_GAUCHE
 * \brief La touche à appuyer pour aller vers la gauche
 */
#define TOUCHE_GAUCHE 'q'

/**
 * \def TOUCHE_HAUT
 * \brief La touche à appuyer pour aller vers le haut
 */
#define TOUCHE_HAUT 'z'

/**
 * \def TOUCHE_BAS
 * \brief La touche à appuyer pour aller vers le bas
 */
#define TOUCHE_BAS 's'

/**
 * \def X_DEBUT
 * \brief La position horizontale à laquelle le serpent doit démarrer
 */
#define X_DEBUT 20

/**
 * \def Y_DEBUT
 * \brief La position horizontale à laquelle le serpent doit démarrer
 */
#define Y_DEBUT 20

/**
 * \def DIRECTION_INITIALE
 * \brief La direction dans laquelle le serpent doit se déplacer au début
 */
#define DIRECTION_INITIALE 'd'

// Déclaration des fonctions fournies
void gotoXY(int x, int y);
int kbhit();
int checkAKeyPress();
void enableEcho();
void disableEcho();


// Déclaration des fonctions demandées
void genererSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], int x, int y); // Check
void afficher(int x, int y, char c);															   // Check
void effacer(int x, int y);																		   // Check
void dessinerSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT]);			   // Check
void progresser(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], char direction);   // Partiel
void effacerEcran();	
void changerDirection(char* direction);																		   // Check

/**
 * \fn int main()
 * \brief Programme principal.
 * \return Code de sortie du programme (0 : sortie normale).
 *
 * Le programme principal exécute le code du jeu
 */
int main()
{
	int positionsX[TAILLE_SERPENT];
	int positionsY[TAILLE_SERPENT];
	int x, y;
	char direction = DIRECTION_INITIALE;
	bool devraitQuitter = false;

    x = X_DEBUT;
    y = Y_DEBUT;

	effacerEcran(); // Préparer l'écran
	genererSerpent(positionsX, positionsY, x, y);

	dessinerSerpent(positionsX, positionsY);
    disableEcho();
	while (!devraitQuitter) // Boucle du jeu, tester la touche d'arrêt, sinon, continuer
	{
		usleep(VITESSE_JEU);
		effacerEcran();
		if (checkAKeyPress())
		{
			devraitQuitter = true;
		}
		changerDirection(&direction);
		progresser(positionsX, positionsY, direction);
		dessinerSerpent(positionsX, positionsY);
	}
    enableEcho();
	printf("\n");

	return EXIT_SUCCESS;
}

/**
 * \fn void gotoXY(int x, int y) 
 * \brief Met le curseur aux coordonnées x et y
 *
 * Met le curseur aux coordonnées x et y passées en paramètre d'entrée
 */
void gotoXY(int x, int y)
{
	printf("\033[%d;%df", y, x);
}

/**
 * \fn void changerDirection(char* direction)
 * \brief Met le curseur aux coordonnées x et y
 *
 * Met le curseur aux coordonnées x et y passées en paramètre d'entrée
 */
void changerDirection(char* direction)
{
    char ch;
    if(kbhit())
    {
        ch = getchar();
    }
	if (ch == TOUCHE_DROITE && *direction != TOUCHE_GAUCHE)
	{
		*direction = TOUCHE_DROITE;
	}
	if (ch == TOUCHE_HAUT && *direction != TOUCHE_BAS)
	{
		*direction = TOUCHE_HAUT;
	}
	if (ch == TOUCHE_GAUCHE && *direction != TOUCHE_DROITE)
	{
		*direction = TOUCHE_GAUCHE;
	}
	if (ch == TOUCHE_BAS && *direction != TOUCHE_HAUT)
	{
		*direction = TOUCHE_BAS;
	}
}
/*!
 \fn int checkAKeyPress()
 \brief La fonction qui vérifie si la touche A est appuyée
 \return true si la touche A est appuyée, false sinon

 La fonction qui vérifie si A est appuyé, en utilisant kbhit
*/
int checkAKeyPress()
{
	char ch;
	if(kbhit())
	{
		ch = getchar();
	}
	if (ch == TOUCHE_ARRET) // Si la touche détectée est celle d'arrêt, retourner true
	{
		return true;
	}
	return false;
}

// Definition des fonctions demandées
void afficher(int x, int y, char c)
{
	gotoXY(x, y);
	printf("%c", c);
}

// Efface l'écran
void effacer(int x, int y)
{
	gotoXY(x, y);
	printf(" ");
}

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void effacerEcran()
{
	system("clear");
}

/*!
 \fn void genererSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], int x, int y)
 \brief La fonction qui génère le serpent
 \param positionsX La liste des positionsX du serpent
 \param positionsY La liste des positionsY du serpent
 \param x Le X de la tête du serpent
 \param y Le Y de la tête du serpent

 Cette fonction créé la liste des positions (positionsX, positionsY) du serpent dans la liste en argument en utilisant x et y
*/
void genererSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], int x, int y)
{
	for (int nbCellule = 0; nbCellule < TAILLE_SERPENT; nbCellule++) // Génerer des coordonées de x à (x + TAILLE_SERPENT) pour le serpent
	{
		positionsX[nbCellule] = x - nbCellule;
		positionsY[nbCellule] = y;
	}
}


/*!
 \fn void dessinerSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT])
 \brief La fonction qui dessine le serpent
 \param positionsX La liste des positions X du serpent
 \param positionsY La liste des positions Y du serpent

 Cette fonction dessine le serpent en fonction des positions en argument générées par la fonction genererSerpent, en dessinant un O pour la tête et un X pour le corps
*/
void dessinerSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT])
{
    for (int iDessine = 0; iDessine < TAILLE_SERPENT; iDessine++)
    {
        int aDessinerX = positionsX[iDessine];
        int aDessinerY = positionsY[iDessine];

        // Vérifier que les coordonnées sont dans les limites de l'écran
        if (aDessinerX >= 0 && aDessinerX < TAILLE_TABLEAU && 
            aDessinerY >= 0 && aDessinerY < TAILLE_TABLEAU)
        {
            // Dessiner la tête ou le corps en fonction de la position dans le tableau
            if (iDessine == 0)
            {
                afficher(aDessinerX, aDessinerY, CHAR_TETE);
            }
            else
            {
                afficher(aDessinerX, aDessinerY, CHAR_CORPS);
            }
        }
    }
}

/*!
 \fn void progresser(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], char direction)
 \brief La fonction qui fait avancer le jeu d'une étape
 \param positionsX La liste des positions X du serpent
 \param positionsY La liste des positions Y du serpent
 \param direction La direction dans laquelle le serpent avance

 La fonction qui fait avancer le corps du serpent, puis bouge la tête dans la direction dans laquelle elle est sensée avancer
*/
void progresser(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], char direction)
{
	for (int i = TAILLE_SERPENT; i > 0; i--) // Avancer chacune des cellules du serpent
	{
		positionsX[i] = positionsX[i - 1];
		positionsY[i] = positionsY[i - 1];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	}
	switch (direction)
    {
        case TOUCHE_DROITE: // droite
            positionsX[0]++;
            break;
        case TOUCHE_HAUT: // haut
            positionsY[0]--;
            break;
        case TOUCHE_GAUCHE: // gauche
            positionsX[0]--;
            break;
        case TOUCHE_BAS: // bas
            positionsY[0]++;
            break;
    }
}

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

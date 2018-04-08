#ifndef GRMAN_H_INCLUDED
#define GRMAN_H_INCLUDED


#include <allegro.h>

#include <string>

/*!
*\file grman\grman.cpp
*\brief Fichier regroupant les fonctions utile a l'affichage graphique via allegro
*\author Robin Fercoq
*/

/*!
*\namespace grman
*\brief espace regroupant des fonctions utile a l'affichage graphique via allegro
*\author Robin Fercoq
*/

#include "grman_couleurs.h"
#include "coords.h"

namespace grman
{

//! Variables globales : visibles partout ou grman.h est inclus

//! La couleur de fond effective utilisee
extern int page_color;

//! Le buffer sur lequel tout est dessine
extern BITMAP *page;

//! La frame associee pour les widgets
extern Frame page_frame;

//! Gestion des widget : avec quel widget la souris interagit ?
class Widget;
extern Widget *gui_over;
extern Widget *gui_last_over;
extern Widget *gui_focus;
extern Widget *gui_leave;


//! Les globales suivantes necessitent un appel a rafraichir_clavier_souris en debut de boucle d'interaction pour etre mises a jour a chaque tour de boucle

extern char key_last;

extern Coords mouse_pos;

extern int mouse_click;

extern int mouse_unclick;

extern int key_press[KEY_MAX];
extern int key_unpress[KEY_MAX];

extern int mouse_click_x;
extern int mouse_click_y;

extern int mouse_move_x;
extern int mouse_move_y;

extern int mouse_click;

//! Gestion des ressources image (fichiers images et BITMAP chargees)
unsigned get_picture_nb(std::string name);
BITMAP *get_picture(std::string pic_name);
void show_picture(BITMAP *dest, std::string file_name, int x, int y, unsigned idx=0);
void set_pictures_path(std::string path_name);

//! A appeler une fois en fin de boucle de jeu
void mettre_a_jour();

//! Lancement et fermeture services Allegro

void init();

void fermer_allegro();


//! Buffer et sorties graphiques
void buf_effacer_page();

void buf_afficher_page();


//! Entrees clavier/souris
void rafraichir_clavier_souris();


//! Auxiliaires : complements aux fonctions graphiques allegro
void thick_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int thickness, int color);

}

//! Cette inclusion en fin de header parce que widget dépend de grman Ceci est evitable (et à eviter) en re-factorisant le projet et les dependance...
#include "widget.h"


#endif // GRMAN_H_INCLUDED

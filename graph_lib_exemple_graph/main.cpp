#include "grman/grman.h"
#include <iostream>

#include "graph.h"

/*!
*\file main.cpp
*\brief Le main du programme de theorie des graphes
*Nous tenons à rappler que la base du code que vous allez lire a ete realise par Robin FERCOQ
*Et que durant cette semaine de projet "piscine", nous nous le sommes approprie afin de l'exploiter le mieux possible
*Pour remplir le cahier des charges demande
*
*\date 08/04/2018
*\author Sylvain BENISTAND, Jonathan BOUTAKHOT, Jean Prouvost, sur un code de base fournit par Robin FERCOQ.
*
*/

int main()
{

    grman::init();//! instanciation des objets graphiques


    grman::set_pictures_path("pics");//! Nom du dossier ou se trouve les images a utiliser

    Graph g; //! Declaration du graph

    g.menu(); //! Appel de la fonction menu qui gere la boucle de jeu et tout les sous programme sous-jacent

    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();


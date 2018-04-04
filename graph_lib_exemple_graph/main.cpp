#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    int xm,ym;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;

        g.menu();

    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();



#include <iostream>

using namespace std;

int main()
{
    ///n_init population initial
    ///vitesse a laquelle n tend vers l'equilibre (vitesse de reproduction?)
    ///k population maximum accepté par le système (variable pour les éléments non basique de la chaine)
    ///les coefficient représente l'importance de l'aliment mangé dans l'alimentation du mangeur.

    ///herbe
    int n_init_herbe=10000;
    int n_herbe;
    float r_herbe=1.5;
    float k_herbe=1000000;

    ///fleur
    int n_init_fleur=5000;
    int n_fleur;
    float r_fleur=1.3;
    float k_fleur=500000;

    ///lichens
    int n_init_lichen=2500;
    int n_lichen;
    float r_lichen=3;
    float k_lichen=250000;

    ///lapin
    int n_init_lapin=100;
    int n_lapin;
    float r_lapin=0.03;

    float k_lapin;

    float coef_herbe_lapin=0.35*10;
    float coef_fleur_lapin=0.5*10;
    float coef_lichen_lapin=0.15*10;

    ///raton
    int n_init_raton=80;
    int n_raton;
    float r_raton=0.02;

    float k_raton;

    float coef_herbe_raton=0.3*10;
    float coef_fleur_raton=0.7*10;

    ///caribou

    int n_init_caribou=80;
    int n_caribou;
    float r_caribou=0.015;

    float k_caribou;

    float coef_herbe_caribou=0.7*10;
    float coef_fleur_caribou=0.15*10;
    float coef_lichen_caribou=0.15*10;

    ///buffle

    int n_init_buffle=75;
    int n_buffle;
    float r_buffle=0.015;

    float k_buffle;

    float coef_herbe_buffle=1*10;

    ///loup

    int n_init_loup=75;
    int n_loup;
    float r_loup=0.015;

    float k_loup;

    float coef_lapin_loup=0.25*10;
    float coef_caribou_loup=0.5*10;
    float coef_raton_loup=0.25*10;



    ///initialisation

    n_herbe=n_init_herbe;
    n_lichen=n_init_lichen;
    n_fleur=n_init_fleur;

    n_lapin=n_init_lapin;
    n_raton=n_init_raton;
    n_caribou=n_init_caribou;
    n_buffle=n_init_buffle;

    n_loup=n_init_loup;

    for (int i=0;i<500;i++)
    {
        k_lapin=(coef_fleur_lapin*n_fleur)+(coef_herbe_lapin*n_herbe)+(coef_lichen_lapin*n_lichen);
        k_caribou=(coef_fleur_caribou*n_fleur)+(coef_herbe_caribou*n_herbe)+(coef_lichen_caribou*n_lichen);
        k_raton=(coef_fleur_raton*n_fleur)+(coef_herbe_raton*n_herbe);
        k_buffle=(coef_herbe_buffle*n_herbe);

        k_loup=(coef_lapin_loup*n_lapin)+(coef_caribou_loup*n_caribou)+(coef_raton_loup*n_raton);

        n_herbe=n_herbe+(r_herbe*n_herbe*(1-(n_herbe/k_herbe)))-(coef_herbe_caribou*n_caribou)-(coef_herbe_lapin*n_lapin)-(coef_herbe_raton*n_raton)-(coef_herbe_buffle*n_buffle);
        n_fleur=n_fleur+(r_fleur*n_fleur*(1-(n_fleur/k_fleur)))-(coef_fleur_caribou*n_caribou)-(coef_fleur_lapin*n_lapin)-(coef_fleur_raton*n_raton);
        n_lichen=n_lichen+(r_lichen*n_lichen*(1-(n_lichen/k_lichen)))-(coef_lichen_caribou*n_caribou)-(coef_lichen_lapin*n_lapin);

        n_lapin=n_lapin+(r_lapin*n_lapin*(1-(n_lapin/k_lapin)))-(coef_lapin_loup*n_loup);
        n_caribou=n_caribou+(r_caribou*n_caribou*(1-(n_caribou/k_caribou)))-(coef_caribou_loup*n_loup);
        n_raton=n_raton+(r_raton*n_raton*(1-(n_raton/k_raton)))-(coef_raton_loup*n_loup);
        n_buffle=n_buffle+(r_buffle*n_buffle*(1-(n_buffle/k_buffle)));

        n_loup=n_loup+(r_loup*n_loup*(1-(n_loup/k_loup)));

        if (n_herbe<=0){n_herbe=0;cout << " ----------L'herbe a disparu de la zone...------------"<< endl;}
        if (n_fleur<=0){n_fleur=0;cout << " ----------Les fleurs ont disparu de la zone...------------"<< endl;}
        if (n_lichen<=0){n_lichen=0;cout << " ----------Les lichens ont disparu de la zone...------------"<< endl;}
        if (n_lapin<=0){n_lapin=0;cout << " ----------Les lapins ont disparu de la zone...------------"<< endl;}
        if (n_caribou<=0){n_caribou=0;cout << " ----------Les caribous ont disparu de la zone...------------"<< endl;}
        if (n_raton<=0){n_raton=0;cout << " ----------Les ratons ont disparu de la zone...------------"<< endl;}
        if (n_buffle<=0){n_buffle=0;cout << " ----------Les buffles ont disparu de la zone...------------"<< endl;}
        if (n_loup<=0){n_loup=0;cout << " ----------Les loups ont disparu de la zone...------------"<< endl;}

        cout << "herbe: "<< n_herbe << endl;
        cout << "fleur: "<< n_fleur << endl;
        cout << "lichen: "<< n_lichen << endl;
        cout << "lapin: "<< n_lapin << endl;
        cout << "caribou: "<< n_caribou <<endl;
        cout << "raton: "<< n_raton << endl;
        cout << "buffle: "<< n_buffle << endl;
        cout << "loup: "<< n_loup << endl;
        cout << endl;
    }

    return 0;
}

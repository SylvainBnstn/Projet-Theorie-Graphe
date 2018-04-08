#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Vertex et Edge se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours à itérateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <list>
#include <stack>
#include "grman/grman.h"

/*!
*\file graph.h
*\brief Le fichier qui gere toutes les fonctions du projet.
*Nous tenons à rappler que la base du code que vous allez lire a ete realise par Robin FERCOQ
*Et que durant cette semaine de projet "piscine", nous nous le sommes approprie afin de l'exploiter le mieux possible
*Pour remplir le cahier des charges demande
*
*\date 08/04/2018
*\author Sylvain BENISTAND, Jonathan BOUTAKHOT, Jean Prouvost, sur un code de base fournit par Robin FERCOQ.
*
*/

/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    /*!
    *\class VertexInterface graph.h Graph
    *\brief Class regroupant toute les particularite graphique et leurs relation pour un sommet (Vertex en anglais)
    *
    *
    *\version 2.0
    */

    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

    private :


        grman::WidgetBox m_top_box;//! La boite qui contient toute l'interface d'un sommet


        grman::WidgetVSlider m_slider_value;//! Un slider de visualisation/modification de la valeur du sommet


        grman::WidgetText m_label_value;//! Un label de visualisation de la valeur du sommet


        grman::WidgetImage m_img;//! Une image cense representer l'element du sommet


        grman::WidgetText m_label_idx;//! Un label indiquant l'index du sommet


        grman::WidgetText m_box_label_idx;//! Une boite pour le label precedent


        grman::WidgetButton m_suppress;//!bouton pour supprimer un sommet

    public :


        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);//! Le constructeur
};


class Vertex
{
    /*!
    *\class Vertex graph.h Graph
    *\brief Class regroupant toute les particularite d'un sommet (Vertex en anglais)
    *
    *
    *\version 3.0
    */

    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :

        std::vector<int> m_in;//! liste des indices des arcs arrivant au sommet : acces aux predecesseurs


        std::vector<int> m_out;//! liste des indices des arcs partant du sommet : acces aux successeurs


        double m_value;//! Donnee associee à l'arc


        bool m_to_delete;//! Indication de suppression

        std::shared_ptr<VertexInterface> m_interface = nullptr;/// le POINTEUR sur l'interface associee

    public:


        Vertex (double value=0, VertexInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }//! Les constructeurs

        float m_K; //! le k associe pour la temporalite
        float m_coef; //! le coef pour la temporalite
        float m_r=0.001; //! le r pour la temporalite

        void pre_update(); //! methode appelee par update de graph
        void post_update(); //! methode appelee par update de graph
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    /*!
    *\class EdgeInterface graph.h Graph
    *\brief Class regroupant toute les particularite graphique et leurs relation pour un arc (Edge en anglais)
    *
    *
    *\version 2.0
    */

    friend class Edge;
    friend class Graph;

    private :


        grman::WidgetEdge m_top_edge; //! Le WidgetEdge qui "contient" toute l'interface d'un arc


        grman::WidgetBox m_box_edge; //! Une boite pour englober les widgets de reglage associés


        grman::WidgetVSlider m_slider_weight; //! Un slider de visualisation/modification du poids valeur de l'arc


        grman::WidgetText m_label_weight; //! Un label de visualisation du poids de l'arc

        grman::WidgetButton m_suppress; //! Un bouton pour la suppression de l'arc
    public :


        EdgeInterface(Vertex& from, Vertex& to, int color=1);//! Le constructeur
};


class Edge
{
    /*!
    *\class Edge graph.h Graph
    *\brief Class regroupant toute les particularite d'un arc (Edge en anglais)
    *
    *
    *\version 3.0
    */
    friend class Graph;
    friend class EdgeInterface;

    private :

        int m_from;//! indice du sommet de depart de l'arc


        int m_to;//! indice du sommet d'arrivee de l'arc


        double m_weight; //!  poids de l'arc


        bool m_to_delete; //!indication de suppression

        int m_color; //!indice de couleur

        std::shared_ptr<EdgeInterface> m_interface = nullptr; //! le POINTEUR sur l'interface associee


    public:


        Edge (double weight=0, EdgeInterface *interface=nullptr, int id_vert1=0, int id_vert2=0, int color=1) :
            m_weight(weight), m_interface(interface), m_from(id_vert1), m_to(id_vert2), m_color(color)  {  };//! constructeurs

        void pre_update(); //! methode appelee par update de graph
        void post_update(); //! methode appelee par update de graph
};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    /*!
    *\class GraphInterface graph.h Graph
    *\brief Class regroupant toute les particularite graphique et leurs relation pour le Graph
    *
    *
    *\version 3.0
    */

    friend class Graph;

    private :


        grman::WidgetBox m_top_box; //! La boite qui contient toute l'interface d'un graphe

        grman::WidgetBox m_main_box; //! boite des sommets et des arcs

        grman::WidgetBox m_tool_box; //! boite des boutons de controle

        grman::WidgetCheckBox m_check_box_1; //!check box pour le graph reduit

        grman::WidgetBox m_button_global; //!boite englobant les bouttons d'ajout et de suppression

        grman::WidgetButton m_button_add_edge; //!bouton ajout edge
        grman::WidgetButton m_button_add_vertex; //!bouton ajout vertex
        grman::WidgetButton m_button_reset; //!bouton de reset

        grman::WidgetText m_txt_a_e; //! texte associe au bouton ajout edge
        grman::WidgetText m_txt_a_v; //! texte associe au bouton ajout vertex
        grman::WidgetText m_txt_reset; //! texte associe au bouton reset

        grman::WidgetBox m_box_temp;//! boite de temporalite
        grman::WidgetCheckBox m_check_temp; //!check box lancant la simulation
        grman::WidgetText m_txt_temp; //! texte associe a la temporalite

    public :

        int ajout_suppression();//! la methode retournant un int pour l'ajout et la suppression
        GraphInterface(int x, int y, int w, int h);//! le constructeur
};


class Graph
{
    /*!
    *\class Graph graph.h Graph
    *\brief Class regroupant toute les particularite pour le Graph
    *
    *
    *\version 3.0
    */
    private :
            //Vecteur de vecteur pour contenir toutes les composantes fortement connexes du graphe
        std::vector<std::vector<int>> m_forte_connexite;
        //Nombre de composantes fortement connexes
        int m_nb_forte_connexite=0;

        std::map<int, Edge> m_edges;//! La liste des aretes


        std::map<int, Vertex> m_vertices;//! La liste des sommets


        std::shared_ptr<GraphInterface> m_interface = nullptr;//! le POINTEUR sur l'interface associee

        std::string m_name="";
//Ordre du graphe
        int m_ordre;
        //Nombre d'arete du graphe
        int m_arete;
        //Liste d'adjacence des sommets du graphe
        std::list<int> *adj;

        //Méthode de remplissage de la pile pour les composantes fortement connexes
        void remplissage(int v, bool visited[], std::stack<int> &Stack);

        //Méthode de DFS
        void DFS(int v, bool visited[], std::vector<int> *conn);
    public:
        void adjacence();

        //Méthode d'initialisation du graphe
        void initGraph(int v);

        //Méthode pour trouver toutes les composantes fortement connexes du graphe
        void ComposantesFortementConnexes();

        //Fonction qui retourne le transposé d'un graphe
        Graph getTranspose();

        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }//! Les constructeurs

        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0 );//! methode ajoutant un sommet en interface
        void user_add_vertex(); //!methode ajoutant un sommet

        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0, int color=1);//! methode ajoutant un arc en interface
        void user_add_edge();//!methode ajoutant un arc

        void suppress_edge(int idx);//! methode supprimant un arc en interface
        void user_suppress_edge();//! methode supprimant un arc

        void suppress_vertex(int idx); //!methode supprimant un sommet

        int choix_menu=0; //! entier correspondant au choix dans le menu

        void menu(); //! methode affichant le menu
        void boucle(); //! methode correspondant a la boucle de jeu

        void temporalite(); //! methode simulant les effets temporels

        void update(); //! La méthode update
        void load_graph(std::string name); //! methode de chargement d'un graph
        void save_graph(std::string name); //! methode de sauvegarde d'un graph
        void unload_graph(); //! methode de suppression memoire d'un graph
        void load_backup_graph(); //! methode de chargement du graph initiale
};


#endif // GRAPH_H_INCLUDED

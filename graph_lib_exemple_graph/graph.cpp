#include "graph.h"
#include <fstream>
#include <iostream>

/***************************************************
                    VERTEX
****************************************************/

//! Le constructeur met en place les elements de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    //! La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    //! Le slider de reglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0);
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    //! Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    //! Une illustration
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    //! Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    m_top_box.add_child( m_suppress);
    m_suppress.set_dim(16,16);
    m_suppress.set_gravity_xy(grman::GravityX::Right,grman::GravityY::Up);
}


//! Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    //! Copier la valeur locale de la donnée m_value vers le slider associe
    m_interface->m_slider_value.set_value(m_value);

    //! Copier la valeur locale de la donnee m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
    m_to_delete=m_interface->m_suppress.get_value();
}


//! Gestion du Vertex apres l'appel a l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    //! Reprendre la valeur du slider dans la donnee m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

//! Le constructeur met en place les elements de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to, int color)
{
    //! Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();
    m_top_edge.set_color(color);
    //! Une boite pour englober les widgets de reglage associes
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,70);
    m_box_edge.set_bg_color(BLANCBLEU);

    //! Le slider de reglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0);
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    //! Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_posy(48);


    //!bouton de suppression
    m_box_edge.add_child( m_suppress);
    m_suppress.set_dim(10,10);
    m_suppress.set_gravity_xy(grman::GravityX::Center,grman::GravityY::Down);
}


//! Gestion du Edge avant l'appel a l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    //! Copier la valeur locale de la donnee m_weight vers le slider associe
    m_interface->m_slider_weight.set_value(m_weight);

    //! Copier la valeur locale de la donnee m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
    m_to_delete=m_interface->m_suppress.get_value();
}

//! Gestion du Edge après l'appel a l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    //! Reprendre la valeur du slider dans la donnee m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

//! Ici le constructeur se contente de preparer un cadre d'accueil des elements qui seront ensuite ajoutes lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    //!check box pour le graph reduit

    m_tool_box.add_child(m_check_box_1);
    m_check_box_1.set_value(true);
    m_check_box_1.set_dim(20,20);
    m_check_box_1.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_check_box_1.set_bg_color(BLANCROSE);

    //!boite englobant les bouttons d'ajout et de suppression

    m_tool_box.add_child(m_button_global);
    m_button_global.set_dim(76,76);
    m_button_global.set_gravity_y(grman::GravityY::Down);
    m_button_global.set_bg_color(BLANCROSE);

    //!bouton ajout edge en haut a gauche de la boite

    m_button_global.add_child(m_button_add_edge);
    m_button_add_edge.set_dim(31,31);
    m_button_add_edge.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_button_add_edge.set_bg_color(VERTCLAIR);

    m_button_add_edge.add_child(m_txt_a_e);
    m_txt_a_e.set_message("A-E");

    //!bouton k_connexité
    m_button_global.add_child(m_button_k_connex);
    m_button_k_connex.set_dim(31,31);
    m_button_k_connex.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_button_k_connex.set_bg_color(FUCHSIA);
    m_button_k_connex.add_child(m_txt_k_connexe);
    m_txt_k_connexe.set_message("K-CO");

    //!bouton ajout vertex en bas a gauche de la boite

    m_button_global.add_child(m_button_add_vertex);
    m_button_add_vertex.set_dim(31,31);
    m_button_add_vertex.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    m_button_add_vertex.set_bg_color(VERT);

    m_button_add_vertex.add_child(m_txt_a_v);
    m_txt_a_v.set_message("A-V");

    //!bouton ajout vertex en bas a gauche de la boite

    m_button_global.add_child(m_button_reset);
    m_button_reset.set_dim(31,31);
    m_button_reset.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_button_reset.set_bg_color(ROUGE);

    m_button_reset.add_child(m_txt_reset);
    m_txt_reset.set_message("Res");

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    ///bouton forte connexite
    m_tool_box.add_child(m_button_forte_connexite);
    m_button_forte_connexite.set_dim(80,60);
    m_button_forte_connexite.set_posy(30);
    m_button_forte_connexite.set_bg_color(BLEUCLAIR);
    m_button_forte_connexite.add_child(m_txt_forte_connexite);
    m_txt_forte_connexite.set_message("FORTE CO");

    //!bouton temporalite

    m_tool_box.add_child(m_box_temp);
    m_box_temp.set_dim(74,38);
    m_box_temp.set_gravity_x(grman::GravityX::Left);
    m_box_temp.set_bg_color(BLANCROSE);

    m_box_temp.add_child(m_check_temp);
    m_check_temp.set_dim(22,22);
    m_check_temp.set_gravity_y(grman::GravityY::Up);
    m_check_temp.set_bg_color(BLANCBLEU);

    m_box_temp.add_child(m_txt_temp);
    m_txt_temp.set_gravity_y(grman::GravityY::Down);
    m_txt_temp.set_message("Simulate");


}

int GraphInterface::ajout_suppression()
{
    if(m_button_add_edge.get_value()==true)
    {
        m_button_add_edge.set_value(false);
        return(1);
    }
    if(m_button_k_connex.get_value()==true)
    {
        m_button_k_connex.set_value(false);
        return(2);
    }
    if(m_button_add_vertex.get_value() ==true)
    {
        m_button_add_vertex.set_value(false);
        return(3);
    }
        if(m_button_reset.get_value() ==true)
    {
        m_button_reset.set_value(false);
        return(4);
    }
        if(m_button_forte_connexite.get_value()==true)
    {
        m_button_forte_connexite.set_value(false);
        return(5);
    }
    return(0);
}

void Graph::menu()
{
    BITMAP *image_menu=NULL, *buff=NULL;
    buff=create_bitmap(1024,768);
    image_menu=load_bitmap("xp.bmp",NULL);
    if(image_menu == NULL)
    {
        printf("Erreur de chargement xp.bmp");
        exit(0);
    }
    while (choix_menu!=5)
    {
        choix_menu=0;
        draw_sprite(buff,image_menu,0,0);
        if ((mouse_b&1)&&(mouse_x>290)&&(mouse_y>440)&&(mouse_x<700)&&(mouse_y<495))
        {
            choix_menu=1;
            m_name="thundra";
            boucle();
        }
        if((mouse_b&1)&&(mouse_x>290)&&(mouse_y>555)&&(mouse_x<700)&&(mouse_y<610))
        {
            choix_menu=2;
            m_name="mer";
            boucle();
        }
        if((mouse_b&1)&&(mouse_x>290)&&(mouse_y>660)&&(mouse_x<700)&&(mouse_y<715))
        {
            choix_menu=3;
            m_name="yellowstone";
            boucle();
        }
        if((mouse_b&1)&&(mouse_x>930)&&(mouse_y>10)&&(mouse_x<1010)&&(mouse_y<95))
        {
            choix_menu=5;
        }
        draw_sprite(screen,buff,0,0);
        rest(20);
        clear_bitmap(buff);
    }
};

void Graph::boucle()
{
    std::string name=m_name+".txt";
    load_graph(name);


    //! Vous gardez la main sur la "boucle de jeu"
    while ( !key[KEY_ESC] )
    {
        //! Il faut appeler les methodes d'update des objets qui comportent des widgets
        update();
        temporalite();
        //! Mise a jour generale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    save_graph(name);
    unload_graph();
};

//! La methode update a appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;
    int button=m_interface->ajout_suppression();
    if(button==1)   user_add_edge();
    if(button==2)   initialisation_k_connexite();
    if(button==3)   user_add_vertex();
    if(button==4)   load_backup_graph();
    if(button==5)   ComposantesFortementConnexes();
    for (auto &elt : m_vertices)
        {
            elt.second.pre_update();
            if(elt.second.m_to_delete==true)
            {
                this->suppress_vertex(elt.first);
                return;
            }
        }

    for (auto &elt : m_edges)
    {
        elt.second.pre_update();
        if(elt.second.m_to_delete==true)
            {
                this->suppress_edge(elt.first);
                return;
            }
    }
    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        {elt.second.post_update();
        }
    for (auto &elt : m_edges)
        elt.second.post_update();

}

//! Aide a l'ajout de sommets interfaces
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    //! Creation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    //! Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    //! On peut ajouter directement des vertices dans la map avec la notation crochet
    m_vertices[idx] = Vertex(value, vi);
}

void Graph::user_add_vertex()
{
    std::cout<<"Entrez le nom du sommet que vous voulez créer (nom de l'image)"<<std::endl;
    std::string name;
    std::cin>>name;
    int key=m_vertices.rbegin()->first;
    key++;
    add_interfaced_vertex(key, 0.0, 50, 50, name);
}

//! Aide a l'ajout d'arcs interfaces
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight, int color)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2], color);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei, id_vert1, id_vert2, color);
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::user_add_edge()
{
    std::cout<<"Entrez l'id du sommet de depart"<<std::endl;
    int id_out, id_in;
    std::cin>>id_out;
    std::cout<<"Entrez l'id du sommet d'arrivé"<<std::endl;
    std::cin>>id_in;
    int key=m_edges.rbegin()->first;
    key++;
    add_interfaced_edge(key, id_out, id_in);

}

void Graph::load_graph(std::string name)
{
    std::ifstream fichier(name, std::ios::in);
    if(fichier)
    {
        m_interface = std::make_shared<GraphInterface>(50,0,750,600);
        int nb, idx, x, y, id_vert1, id_vert2, color;
        double value, weight;
        std::string pic_name;
        fichier>>nb;
        for(int i=0;i<nb;i++)
        {
            fichier>>idx>>value>>x>>y>>pic_name;
            add_interfaced_vertex(idx, value, x, y,pic_name);
        }
        fichier>>nb;
        for(int j=0;j<nb;j++)
        {
            fichier>>idx>>id_vert1>>id_vert2>>weight>>color;
            add_interfaced_edge(idx, id_vert1, id_vert2, weight, color);
        }
        fichier.close();
    }
}

void Graph::save_graph(std::string name)
{
    std::ofstream fichier(name, std::ios::out | std::ios::trunc);
    if(fichier)
    {
        int nb, idx, x, y, id_vert1, id_vert2, color;
        double value, weight;
        std::string pic_name;
        nb=m_vertices.size();
        fichier<<nb<<std::endl;
        for(std::map<int,Vertex>::iterator i=m_vertices.begin();i!=m_vertices.end();i++)
        {
            fichier<<i->first<<" "<<i->second.m_value<<" "<<i->second.m_interface->m_top_box.get_posx()<<" "<<i->second.m_interface->m_top_box.get_posy()<<" "<<i->second.m_interface->m_img.get_pic_name()<<std::endl;
        }
        nb=m_edges.size();
        fichier<<nb<<std::endl;
        for(std::map<int,Edge>::iterator i=m_edges.begin();i!=m_edges.end();i++)
        {
            fichier<<i->first<<" "<<i->second.m_from<<" "<<i->second.m_to<<" "<<i->second.m_weight<<" "<<i->second.m_color<<std::endl;
        }
        fichier.close();
    }
}

void Graph::suppress_edge(int idx)
{
    Edge &remed=m_edges.at(idx);
    if (remed.m_interface)

    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), idx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), idx ), veto.end() );

    m_edges.erase( idx );

}

void Graph::user_suppress_edge()
{
    std::cout<<"Cliquez sur le sommet de depart"<<std::endl;

}

void Graph::suppress_vertex(int idx)
{
    std::vector<int> temp;
     //!pointe sur le edge
        for(std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
        {
            if (it->second.m_from==idx||it->second.m_to==idx)
            {
                temp.push_back(it->first);
            }
        }
        for(auto elem : temp)
        {
            suppress_edge(elem);
        }
        if(m_interface && m_vertices[idx].m_interface)
        {
            m_interface->m_main_box.remove_child(m_vertices[idx].m_interface->m_top_box);
        }
        m_vertices.erase(idx);
}

void Graph::unload_graph()
{
    std::vector<int> temp_vertex;
    for(std::map<int,Vertex>::iterator i=m_vertices.begin();i!=m_vertices.end();i++)
    {
        temp_vertex.push_back(i->first);
    }
    for(auto elem : temp_vertex)
    {
        suppress_vertex(elem);
    }
}

void Graph::load_backup_graph()
{
    std::string name=m_name+"_backup.txt";
    unload_graph();
    load_graph(name);
}

void Graph::temporalite()
{
    if (m_interface->m_check_temp.get_value()==true)
    {
        //!calcul des K
        for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end(); it++)
            {
                it->second.m_K=0.001;
                for (auto elem : it->second.m_in)
                {
                    it->second.m_K= it->second.m_K + (m_edges[elem].m_weight*m_vertices[m_edges[elem].m_from].m_value);
                }
            }

        //!calcul des n
        for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end(); it++)
            {
                it->second.m_coef=0;
                for (auto elem : it->second.m_out)
                {
                    it->second.m_coef= (it->second.m_coef + (m_edges[elem].m_weight*m_vertices[m_edges[elem].m_to].m_value))/1000;
                }

                it->second.m_value=(it->second.m_value+((it->second.m_r)*(it->second.m_value)*(1-((it->second.m_value)/(it->second.m_K))))-it->second.m_coef);

                if (it->second.m_interface->m_img.get_pic_name()=="soleil.png"){it->second.m_value=100;}
                if (it->second.m_value<0){it->second.m_value=0;}
                if (it->second.m_value>100){it->second.m_value=100;}

            }
    }


}

//!Méthode pour appliquer le DFS à partir du sommet v
void Graph::DFS(int v, bool visited[], std::vector<int> *conn)
{
    //!On marque le sommet
    visited[v] = true;
    //!On rajoute le sommet dans le vecteur de connexité
    conn->push_back(v);

    //!Iterateur
    std::list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        //!Si le sommet n'est pas visité
        if (!visited[*i])
        {
            //!Récursité
            DFS(*i, visited, conn);
        }
    }
}

//!Méthode pour initialiser le graphe
void Graph::initGraph(int v)
{
    m_ordre = v;
    adj = new std::list<int>[v];
}

//!Méthode pour trouver le transposé d'un graphe
Graph Graph::getTranspose()
{
    //!Déclaration du graphe
    Graph gr;
    //!On initialise les parametres du graphe
    gr.initGraph(m_arete);

    for (int v = 0; v < m_arete; v++)
    {
        std::list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            //!On ajoute le sommet v dans la liste d'adjacence du sommet *i
            gr.adj[*i].push_back(v);
        }
    }
    //!On retourne le graphe obtenu
    return gr;
}

//!Méthode pour remplir notre pile
void Graph::remplissage(int v, bool visited[], std::stack<int> &Stack)
{
    //!On marque le sommet à 'visité'
    visited[v] = true;

    //! Recur for all the vertices adjacent to this vertex
    std::list<int>::iterator i;
    //!On parcourt les adjacents du sommet v
    for(i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        //!Si le sommet n'est pas visité
        if(!visited[*i])
        {
            //!Récursivité
            remplissage(*i, visited, Stack);
        }
    }
    Stack.push(v);
}


//!Méthode pour trouver toutes les composantes fortement connexes du graphe
void Graph::ComposantesFortementConnexes()
{
    //!On récupére l'ordre du graphe
    m_ordre = m_vertices.size();

    //!On récupére le nombre d'arete du graphe
    m_arete = m_edges.size();

    //!On alloue la liste d'adjacence
    adj = new std::list<int>[m_arete];

    //!On remplit la liste d'adjacence grace au vecteur d'arete
    std::map<int, Edge>::iterator it;
    for(it = m_edges.begin() ; it != m_edges.end() ; ++it)
    {
        adj[it->second.m_from].push_back(it->second.m_to);
    }

    //!Déclaration des variables
    std::stack<int> Stack;
    std::vector<int> connexite;

    //!On marque tout les sommets à non visités
    bool *visited = new bool[m_ordre];
    for(int i = 0; i < m_ordre; i++)
    {
        visited[i] = false;
    }

    //!On remplit la pile
    for(int i = 0; i < m_ordre; i++)
    {
        //!Si le sommet n'est pas visité
        if(visited[i] == false)
        {
            remplissage(i, visited, Stack);
        }
    }

    //!Création d'un graphe transposé
    Graph gr = getTranspose();

    //!On réinitialise les sommets à non visités
    for(int i = 0; i < m_ordre; i++)
    {
        visited[i] = false;
    }

    //!On traite la pile tant qu'elle n'est pas vide
    while (Stack.empty() == false)
    {
        //!On récupére d'apres la pile
        int v = Stack.top();
        //!On supprime l'élément que l'on vient de prendre de la pile
        Stack.pop();

        //!Si le sommet n'est pas visité
        if (visited[v] == false)
        {
            std::vector<int> connexite;
            //!Parcours en DFS
            gr.DFS(v, visited, &connexite);
            std::cout << std::endl;
            //!Ajout d'une composante fortement connexe
            m_nb_forte_connexite++;
            //!On rajoute le vecteur de la composante fortement connexe dans notre vecteur de vecteur
            m_forte_connexite.push_back(connexite);
            //!On nettoie le vecteur
            connexite.clear();
        }
    }

    //!Affichage des composantes fortement connexes
    std::cout << std::endl;
    std::vector<int>::iterator p;
    for(int unsigned i=0;i<m_forte_connexite.size();i++)
    {
        std::cout << "Composante Fortement connexe " << i+1 << " : ";
        for(p= m_forte_connexite[i].begin(); p!= m_forte_connexite[i].end(); p++)
        {
            std::cout << *p << " ";
        }
        std::cout << std::endl;
    }
}

//!Lance la boucle k connexite
void Graph::initialisation_k_connexite()
{
    bool necessite=true;
    int nb_sommets_adj=0;
    std::vector<std::vector <int>> combinaisons;
    std::vector<int> intermediaire;
    for(auto& element:m_vertices)
    {

        nb_sommets_adj=element.second.m_out.size()+element.second.m_in.size();
        if(nb_sommets_adj==0)
        {
            necessite=false;
        }
    }
    if(necessite==false)
    {
        std::cout<<"le graphe est deja non connexe"<<std::endl;
    }
    else if(necessite==true)
    {
        for(auto &element:m_vertices)
        {
            element.second.m_existe=true;
        }
        m_tdb=0;
        m_tdb_max=6666;
        k_connexe(intermediaire,combinaisons);
        int poids_minimum=1000000;
    std::vector<std::vector <int>> combinaisons_min;
    std::vector<std::vector <int>> combinaisons_min1;
    std::vector<std::vector <int>> utilises;
    int indice=0;
    bool bon;
    for(int i=0; i<combinaisons.size(); i++)
    {
        if(combinaisons[i].size()<poids_minimum)
        {
            poids_minimum=combinaisons[i].size();
        }
    }
    for(int i=0; i<combinaisons.size();i++)
    {
        if(combinaisons[i].size()==poids_minimum)
        {
            combinaisons_min.push_back(combinaisons[i]);
        }
    }
    for(int i=0;i<combinaisons_min.size();i++)
    {
        intermediaire=combinaisons_min[i];
        indice=0;
        bon=true;
        for(int j=0;j<utilises.size();j++)
        {
            for(int k=0;k<utilises[i].size();i++)
            {
                for(int l=0;l<utilises.size();l++)
                {
                    if(intermediaire[l]==utilises[j][k])
                    {
                        indice++;
                    }
                    if(indice==poids_minimum)
                    {
                        bon=false;
                    }
                }
            }
        }
        if(bon==true)
        {
            combinaisons_min1.push_back(intermediaire);
            utilises.push_back(intermediaire);
        }
    }
    std::cout<<"Le graphe devient non connexe en supprimant les sommets suivants"<<std::endl;
    for(int i=0;i<combinaisons_min1.size();i++)
    {
        if(combinaisons_min1.size()!=0)
        {
            std::cout<<"- ";
            for(int j=0;j<combinaisons_min1[i].size();j++)
            {
                std::cout<<combinaisons_min1[i][j]<<" ";
            }
            std::cout<<std::endl;
        }

    }

    }
}

//! methode qui verifie la k connexite
void Graph::k_connexe(std::vector<int>& intermediaire, std::vector<std::vector <int>>& combinaisons)
{
    bool bon;
    for(auto & element:m_vertices)
    {
        if(element.second.m_existe)
        {
            element.second.m_existe=false;
            if(graph_connexite(element.first)==true)
            {
                intermediaire.push_back(element.first);
                m_tdb++;
                if(m_tdb<m_tdb_max)
                {
                    k_connexe(intermediaire,combinaisons);
                }
                intermediaire.pop_back();
            }
            else if(graph_connexite(element.first)==false)
            {
                m_tdb++;
                bon=true;
                for(int i=0; i<combinaisons.size(); i++)
                {
                    if(combinaisons[i]==intermediaire)
                    {
                        bon=false;
                    }
                }
                if(bon==true)
                    combinaisons.push_back(intermediaire);
                if(m_tdb<m_tdb_max)
                {
                    m_tdb_max=m_tdb;
                }
            }
            m_tdb--;
            element.second.m_existe=true;
        }
    }
}

//! methode qui verifie que le graph reste connexe une fois les sommets supprimé
bool Graph::graph_connexite(int idx)
{

    int intermediaire;
    int indice=0;
    int indice1=0;
    std::queue<int> file_bfs;
    for(auto & element:m_vertices)
    {
        element.second.m_marqueur=false;
    }
    m_vertices[idx].m_marqueur=true;
    file_bfs.push(idx);
    while(file_bfs.size()!=0)
    {
        intermediaire=file_bfs.front();
        file_bfs.pop();
        for(int i=0; i<m_vertices[intermediaire].m_in.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[intermediaire].m_in[i]].m_from].m_existe&&!m_vertices[m_edges[m_vertices[intermediaire].m_in[i]].m_from].m_marqueur)
            {
                file_bfs.push(m_edges[m_vertices[intermediaire].m_in[i]].m_from);
                m_vertices[m_edges[m_vertices[intermediaire].m_in[i]].m_from].m_marqueur=true;
            }

        }
        for(int i=0; i<m_vertices[intermediaire].m_out.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[intermediaire].m_out[i]].m_to].m_existe&&!m_vertices[m_edges[m_vertices[intermediaire].m_out[i]].m_to].m_marqueur)
            {
                file_bfs.push(m_edges[m_vertices[intermediaire].m_out[i]].m_to);
                m_vertices[m_edges[m_vertices[intermediaire].m_out[i]].m_to].m_marqueur=true;
            }
        }
    }
    for(auto& element:m_vertices)
    {
        if(element.second.m_existe)
        {
            indice++;
        }
        if(element.second.m_marqueur)
        {
            indice1++;
        }
    }
    if(indice>indice1)
    {
        return false;
    }
    else if(indice==indice1)
    {
        return true;
    }

}

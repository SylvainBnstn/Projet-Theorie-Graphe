#include "graph.h"
#include <fstream>
#include <iostream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
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


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
    m_to_delete=m_interface->m_suppress.get_value();
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to, int color)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();
    m_top_edge.set_color(color);
    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,70);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_posy(48);


    ///bouton de suppression
    m_box_edge.add_child( m_suppress);
    m_suppress.set_dim(10,10);
    m_suppress.set_gravity_xy(grman::GravityX::Center,grman::GravityY::Down);
}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
    m_to_delete=m_interface->m_suppress.get_value();
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    ///check box pour le graph reduit

    m_tool_box.add_child(m_check_box_1);
    m_check_box_1.set_value(true);
    m_check_box_1.set_dim(20,20);
    m_check_box_1.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_check_box_1.set_bg_color(BLANCROSE);

    ///boite englobant les bouttons d'ajout et de suppression

    m_tool_box.add_child(m_button_global);
    m_button_global.set_dim(76,76);
    m_button_global.set_gravity_y(grman::GravityY::Down);
    m_button_global.set_bg_color(BLANCROSE);

    ///bouton ajout edge en haut a gauche de la boite

    m_button_global.add_child(m_button_add_edge);
    m_button_add_edge.set_dim(31,31);
    m_button_add_edge.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_button_add_edge.set_bg_color(VERTCLAIR);

    m_button_add_edge.add_child(m_txt_a_e);
    m_txt_a_e.set_message("A-E");

    ///bouton ajout vertex en bas a gauche de la boite

    m_button_global.add_child(m_button_add_vertex);
    m_button_add_vertex.set_dim(31,31);
    m_button_add_vertex.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    m_button_add_vertex.set_bg_color(VERT);

    m_button_add_vertex.add_child(m_txt_a_v);
    m_txt_a_v.set_message("A-V");

    ///bouton ajout vertex en bas a gauche de la boite

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
}
int GraphInterface::ajout_suppression()
{
    if(m_button_add_edge.get_value()==true)
    {
        m_button_add_edge.set_value(false);
        return(1);
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
    return(0);
}

/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
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

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    save_graph(name);
    unload_graph();
};
/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;
    int button=m_interface->ajout_suppression();
    if(button==1)   user_add_edge();
    if(button==2)   user_suppress_edge();
    if(button==3)   user_add_vertex();
    if(button==4)   load_backup_graph();
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

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}
void Graph::user_add_vertex()
{
    std::cout<<"Entrez le nom du sommet que vous voulez cr�er (nom de l'image)"<<std::endl;
    std::string name;
    std::cin>>name;
    int key=m_vertices.rbegin()->first;
    key++;
    add_interfaced_vertex(key, 0.0, 50, 50, name);
}
/// Aide � l'ajout d'arcs interfac�s
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
    std::cout<<"Entrez l'id du sommet d'arriv�"<<std::endl;
    std::cin>>id_in;
    int key=m_edges.rbegin()->first;
    key++;
    add_interfaced_edge(key, id_out, id_in);

}
void Graph::load_graph(std::string name)
{
    ///v1
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
     //pointe sur le edge
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
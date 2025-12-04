// par souci de convention, et après avoir lu : https://google.github.io/styleguide/cppguide.html#Naming et une grande quantitée de post reddit,
// les variable de ce code on été uniformisée en snake_case

#include "fingerprint.hpp"
#include "test_helper.hpp"

#include <stdexcept> // for std::invalid_argument
#include <iostream>
#include <limits>
#include <math.h>

#include "helper.hpp"
#include "utils.hpp" // for NotImplemented()

// provided constants
constexpr unsigned int ORIENTATION_DISTANCE(16);
constexpr int MATCH_ANGLE_OFFSET(2);
constexpr unsigned int DISTANCE_THRESHOLD(5);
constexpr unsigned int ORIENTATION_THRESHOLD(20);
constexpr unsigned int FOUND_THRESHOLD(20);
//implementaton du macro infinity
constexpr double POSITIVE_INFINITY(std::numeric_limits<double>::infinity());
// on renome la valeur magique 8 en un macro NUM_NEIGHBOOR
constexpr unsigned int NUM_NEIGHBOOR(8);


using namespace std;

//structure de coordonnée x et y
//devoir refaire les math a chaque fois pour trouver les coordonnée = merci au revoir, git add .env...
//x et y, simple, efficace, universel.
struct Coord{
    int x;
    int y;
};

std::vector<bool> get_neighbours(const BinaryImage &binary_image, size_t row, size_t column) {
    //Verification si row et colummn sont dans l image, autrement on output une erreur :
    //le ">=" vient du fait que si indices vont de 0 à 99 la taille est de 100, or si on a un indice 100 et qu'on veut le
    //mettre dans un vecteur de taille 100, bah ca marche pas...
    if(row >= binary_image.size()){
        throw invalid_argument("row out of range");
    }
    else if(column >= binary_image[row].size()) {
        throw invalid_argument("column out of range");
    }
    //le reste de la fonction se lance uniquement si on est PAS out of range
    else {
        vector<bool> neighboor_vector(NUM_NEIGHBOOR, false); //initialisation du vecteur contenant la liste des voisins du pixel
        //7 0 1
        //6 . 2
        //5 4 3 <- strucrture des indices du vecteur !!!!!


        //verif si les vecteur adjacent sont dans la range d'analyse sinon les set a false
        // on pose des array constant qui contiennes les "coordonnée" des pixel adjacents, cela nous permet de ne pas verif le pixel analysé (ia pas besoin lol...)
        //{-1,-1}  {-1,0}  {-1,1}
        //
        //{0,-1}     P      {0,1}
        //
        //{1,-1}   {1,0}    {1,1} <- structure des indices du vecteur (dans la base nRow,nCLoumn)
        //
        // organiser de maniere a ce que ca concorde avc la structure des indices du neighbourVector
        //
        const int n_row[8] =    {-1,-1, 0, 1, 1, 1, 0, -1};
        const int n_column[8] = { 0, 1, 1, 1, 0,-1,-1, -1};


        for (size_t i = 0; i < NUM_NEIGHBOOR; ++i) {

            //on verifie si c est dans l image
            // !! il faut convertir les  row et column en int pour la comparaison...
            // psq vu que c est des size_t bah les operation elle bug (genre size_t on peut pas faire moins...)

            int int_row = static_cast<int>(row);
            int int_column = static_cast<int>(column);

            //DEBUG
            //cout << "test du voisin " << i << " a row=" << intRow+nRow[i]<< " et col=" << intColumn+nColumn[i] << endl;
            //DEBUG

            if((int_row+n_row[i]>=0 and int_row+n_row[i]<=binary_image.size()-1) and (int_column+n_column[i]>=0 and int_column+n_column[i]<=binary_image[int_row+n_row[i]].size()-1)){
                //si oui on regarde la valeur et on la stock dans le neighbourVector

                //le pixel est noir
                if(binary_image[int_row+n_row[i]][int_column+n_column[i]]){
                    neighboor_vector[i]=true;
                }
                //le pixel est blanc
                else{
                    neighboor_vector[i]=false;
                }

            }
            else{
                //si pas dans l image :
                neighboor_vector[i]=false;
            }
        }
        return neighboor_vector;
    }

}

unsigned int black_neighbours(std::vector<bool> neighbours) {
    // on regarde juste combien on a de voisin noir
    unsigned int count(0);
    for (size_t i=0; i< NUM_NEIGHBOOR; ++i) {
        if(neighbours[i]){
            count++;
        }
    }
    return count;

}

unsigned int transitions(std::vector<bool> neighbours) {
    //on veut compter combien de transition au noir on a
    //pour ca on va utiliser une boucle avec un ptit modulo histoire de pas calculer l indice 8 quand on transitionne de 7->0
    unsigned int count(0);
    //7 -> nombre de voisin
    for (size_t i=0; i< NUM_NEIGHBOOR; ++i) {
        //pour pas boucler sur la transition 7->0
        size_t next=(i+1)%8;
        if(!neighbours[i] and neighbours[next]){
            count++;
        }

    }
    return count;
}

bool identical(const BinaryImage &binary_image_1, const BinaryImage &binary_image_2) {
  //on verifie si les image on le meme nombre de colonne/ligne
    if(binary_image_1.size()!=binary_image_2.size()){
        return false;
    }
    else if(binary_image_1[0].size()!=binary_image_2[0].size()){
        return false;
    }
    else{
        //on regarde si les pixel sont différent ou pas, et si c'est bon on retourne true
        for (size_t i = 0; i < binary_image_1.size(); ++i) {
            for (size_t j = 0; j < binary_image_1[0].size(); ++j) {
                //DEBUG
                //cout<<"size debug ----   img1 : "<<binary_image_1.size()<<"   - img2 : "<<binary_image_2.size()<<endl;
                //cout<<"img1 : "<<binary_image_1[i][j]<<" indices i: "<<i<<" indices j:"<<j<<endl;
                //cout<<"img2 : "<<binary_image_2[i][j]<<" indices i: "<<i<<" indices j:"<<j<<endl;
                //DEBUG
                if(binary_image_1[i][j] != binary_image_2[i][j]){
                    return false;
                }
            }
        }
    }
    return true;
}

BinaryImage thinning_step(const BinaryImage &binary_image, int step) {    
    //on cree notre image de sortie, qui est exactement parreille que l image d entrée au debut :
    BinaryImage output=binary_image;

    if(step==0){
        //ETAPE 1
        // pixel considere comme non-pertinent si (toutes les conditions doivent etre renplient) :
        // 1. Le pixel est noir,
        // 2. Le tableau des 8 voisins du pixel est non nul,
        // 3. 2 ≤ blackNeighbours() ≤ 6,
        // 4. transitions() = 1,
        // 5. P0 ou P2 ou P4 est blanc,
        // 6. P2 ou P4 ou P6 est blanc

        //on boucle sur tout les pixel de l image de base
        for (size_t i = 0; i < binary_image.size(); ++i) {
            for (size_t j = 0; j < binary_image[0].size(); ++j) {
                //initialisation de get_neighbours pour plus de rapidité, de simplicité, d'aimabilité et de congolexicomatisation des lois du marché
                vector<bool> neighbours(get_neighbours(binary_image,i,j));
                //gestion des neighbours nul :
                if(neighbours.size()==0){
                    throw invalid_argument("null neighbours vector");
                }
                //verification 1. 3. 4. 5. 6. 7. 8.
                if(binary_image[i][j]
                    and(2 <= black_neighbours(neighbours) and black_neighbours(neighbours)  <= 6 )
                    and transitions(neighbours)==1
                    and (!neighbours[0] or !neighbours[2] or !neighbours[4])
                    and (!neighbours[2] or !neighbours[4] or !neighbours[6])){
                    //si toutes les conditions sont remplient, alors on set le pixel comme étant blanc (false) dans l image de sortie :
                    output[i][j]=false;
                }
            }
        }
        return output;
    }
    else if(step==1){
        //ETAPE 2
        // pixel considere comme non-pertinent si (toutes les conditions doivent etre renplient)
        //1. Le pixel est noir,
        //2. Le tableau des 8 voisins du pixel est non nul,
        //3. 2 ≤ blackNeighbours() ≤ 6,
        //4. transitions() = 1,
        //5. P0 ou P2 ou P6 est blanc, ← différence ici avec l’étape 1
        //6. P0 ou P4 ou P6 est blanc. ← différence ici avec l’étape 1

        //on boucle sur tout les pixel de l image de base
        for (size_t i = 0; i < binary_image.size(); ++i) {
            for (size_t j = 0; j < binary_image[0].size(); ++j) {
                //initialisation de get_neighbours pour plus de rapidité, de simplicité, d'aimabilité et de congolexicomatisation des lois du marché
                vector<bool> neighbours(get_neighbours(binary_image,i,j));
                //gestion des neighbours nul :
                if(neighbours.size()==0){
                    throw invalid_argument("null neighbours vector");
                }
                //verification 1. 3. 4. 5. 6. 7. 8.
                if(binary_image[i][j]
                    and(2 <= black_neighbours(neighbours) and black_neighbours(neighbours)  <= 6 )
                    and transitions(neighbours)==1
                    and (!neighbours[0] or !neighbours[2] or !neighbours[6])
                    and (!neighbours[0] or !neighbours[4] or !neighbours[6])){
                    //si toutes les conditions sont remplient, alors on set le pixel comme étant blanc (false) dans l image de sortie :
                    output[i][j]=false;
                }
            }
        }
        return output;
    }
    else{
        throw invalid_argument("invalid step argument");
    }
}

BinaryImage thin(const BinaryImage &binary_image) {
    //boucle while pour boucler les fonctions de thining_step
    //on definit une variable correspondant aux valeur de binary_image
    BinaryImage image = binary_image;
    BinaryImage previous;

    //implementations d un compteur debug, au cas ou on a une boucle infinie
    //DEBUG
    //size_t i(0);
    //DEBUG

    do{
        previous=image;

        //DEBUG
        //cout<<i<<endl;
        //i++;
        //DEBUG

        //on applique les deux thining step
        // apres inchallah...
        image = thinning_step(image,0);
        image = thinning_step(image,1);
    }
    while (!identical(previous,image)); //tant que les deux image sont differente la boucle continue
    return image;
}

BinaryImage connected_pixels(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {
    //on convertis les colonne et les ligne en int pour pouvoir manipuler leur valeur:
    int int_row = static_cast<int>(row);
    int int_rolumn = static_cast<int>(column);

    //creation de notre matrice de sortie, completement vide, pour l'instant... :
    BinaryImage output(binary_image.size(), vector<bool>(binary_image[row].size(),false));
    //initialisation de la minutie
    output[row][column]=true;
    //boucle while avec condition de sortie si on a analyser tout les pixel adjacent a la minutie
    bool loop(true);
    while (loop) {
        loop=false;
        for (size_t i = 0;i<binary_image.size(); i++) {
            for (size_t j = 0; j < binary_image[i].size(); ++j) {
                // on pose les distance absolue la distance entre le pixel analysé et la minutie
                int dist_row = abs(static_cast<int>(i) - int_row);
                int dist_column = abs(static_cast<int>(j) - int_rolumn);
                //on verifie si les pixels ne sont pas dans une distance convenable
                if(dist_row<=distance and dist_column<=distance){
                    // vérification des condition pour que le pixel soit affiché :
                    //si le pixel est blanc dans output et qu'il est noir dans binary_image
                    if(!output[i][j]
                        and binary_image[i][j]
                        ){
                        if(
                            //si il y a un pixel adjacent dans output
                            //verification des exception
                            //on verifie que i-1/j-1 ne soit pas négatif
                            //ensuite on verifie que les condition soit respectée
                            //on ne fait pas de size_t negatif car on verifie a chaque fois avant que i/j est positif
                            (i>0 and output[i-1][j])
                            or (i<output.size()-1 and output[i+1][j])
                            or (j>0 and output[i][j-1])
                            or (j<output[i].size()-1 and output[i][j+1])
                            or ((j<output[i].size()-1 and i<output.size()-1) and output[i+1][j+1])
                            or ((j>0 and i>0) and output[i-1][j-1])
                            or ((i<output.size()-1 and j>0) and output[i+1][j-1])
                            or ((j<output[i].size()-1 and i>0) and output[i-1][j+1])){
                            //si toutes les conditions sont respectée, on met le pixel a true
                            output[i][j]=true;
                            loop=true;
                        }
                    }
                }
            }
        }
    }
    return output;
}
// !!!!! implementation de la fonction compute_relative_coordinate, qui calcul les coordonnée relative a un pixel definit par ses coordonnée row et column
vector<Coord> compute_relative_coordinate(const BinaryImage &binary_image, size_t row, size_t column){
    //liste de coordonnée x et y
    vector<Coord> neighboor_list;
    // on recherche les pixel noir et on met leur coordonnée dans la liste
    for (size_t i=0;i<=binary_image.size()-1; ++i) {
        for (size_t j=0;j<=binary_image[i].size()-1; ++j) {
            if(binary_image[i][j]){
                Coord temp;
                temp.y = static_cast<int>(i);
                temp.x = static_cast<int>(j);
                neighboor_list.push_back(temp);
            }
        }
    }
    //maintenant qu'on a la liste des coordoonée avsolue, on definit les coordonnée par rapport a notre nouvelle origine (minutie)
    //on a pas besoin de gerer les exception car on est dans une base d origine minutie
    vector<Coord> relative_coord(neighboor_list.size());
    for (size_t i = 0; i < neighboor_list.size(); ++i) {
        relative_coord[i].x = neighboor_list[i].x - static_cast<int>(column);
        relative_coord[i].y = static_cast<int>(row) - neighboor_list[i].y;
    }
    return relative_coord;
}

double compute_slope(const BinaryImage &connected_pixels, size_t row, size_t column) {
    //implementation de la valeur de sortie slope
    double slope(0);
    //implemetation des somme
    double sum_x2 = 0.0;
    double sum_y2 = 0.0;
    double sum_xy = 0.0;
    //appel a la fonction calculate relative coord
    vector<Coord> relative_coord = compute_relative_coordinate(connected_pixels,row,column);
    // on calcul les sommes
    // somme des x*y, x² et y²
    for (size_t i=0; i<relative_coord.size(); ++i) {
        int x=relative_coord[i].x;
        int y=relative_coord[i].y;
        sum_xy+=x*y;
        sum_x2+=x*x;
        sum_y2+=y*y;
    }
    // calcul du a de y=ax -> pente de la droite
    // gestion des exception si x² = 0
    //! DEBUG
    //int k = 5;
    //for (int i = 0; i < relativeCoord.size(); ++i) {
    //    cout<<" {"<<relativeCoord[i].x<<";"<<relativeCoord[i].y<<"},";
    //    if(i%k==0 and i!=0){
    //        cout<<endl;
    //    }
    //}
    //cout<<endl<<"sommes des x² : "<<sumX2<<" | sommes des y² : "<<sumY2<<" | sommes des XY : "<<sumXY<<" | ";
    //! DEBUG
    if(sum_x2==0){
        return POSITIVE_INFINITY;
    }else{
        if(sum_x2>=sum_y2){
            slope=sum_xy/sum_x2;
        }
        else if(sum_x2<sum_y2){
            slope=sum_y2/sum_xy;
        }
        return slope;
    }
 }

double compute_angle(const BinaryImage &connected_pixels, size_t row, size_t column, double slope) {
    //implementation du tableau de coordoonnée des point de l'image
    vector<Coord> relative_coord = compute_relative_coordinate(connected_pixels,row,column);
    // implementation de la gestion des exception
    if(slope==POSITIVE_INFINITY){
        //calcul pour voir si les pixels sont en haut ou en bas
        int sum_y(0);
        for (size_t i = 0; i < relative_coord.size(); ++i) {
            sum_y+= relative_coord[i].y;
        }
        if(sum_y>0){
            return M_PI/2.0;
        }else{
            return (-M_PI)/2.0;
        }
    }
    // calcul de l angle avec arctan(a)
    double angle = atan(slope);
    // on regarde si les pixel sont au dessus ou au dessous de la droite normal a la pente (perpendiculaire)
    int up_count(0);
    int down_count(0);
    for (size_t i = 0; i < relative_coord.size(); ++i) {
        //comme on a une inégalité et que la pente peut etre négative on dois verifier tout les cas
        if(slope>0 and (relative_coord[i].y*slope>=-relative_coord[i].x)){
            up_count++;
        }
        else if(slope<0 and (relative_coord[i].y*slope<=-relative_coord[i].x)){
            up_count++;
        }
        else if(slope==0 and (relative_coord[i].y>=0)){
            up_count++;
        }
        else{
            down_count++;
        }
    }
    //! DEBUG (oue je sais c est des comm doxygen mais az comme ca je sais)
    //!cout<<"minutie {"<<row<<";"<<column<<"}, | pixel en dessus : "<<upCount<<" | pixel en dessous : "<<downCount<<" |"<<" pente : "<<slope<<"| angle : "<<radians_to_degrees(angle)<<endl;
    //! DEBUG
    if((angle>0 and down_count>up_count) or (angle<0 and down_count<up_count)){
        return angle + M_PI;
    }else{
        return angle;
    }
}

int compute_orientation(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {
    // on appelle toutes ls fonctions afin de calculer l oreientation finale
    BinaryImage connected_list = connected_pixels(binary_image, row, column, distance);
    double slope = compute_slope(connected_list, row, column);
    double angle = compute_angle(connected_list, row, column, slope);
    double angle_deg = radians_to_degrees(angle);
    //si l angle est negatif
    if(angle_deg<0){
        angle_deg+=360;
    }
    return static_cast<int>(std::round(angle_deg));
}

std::vector<Minutia> extract(const BinaryImage &binary_image) {
    vector<Minutia> minutia_list;
    for (size_t i = 1; i < binary_image.size()-1; ++i) {
        for (size_t j = 1; j < binary_image[i].size()-1; ++j) {
            if(binary_image[i][j]){
                vector<bool> neighboor = get_neighbours(binary_image,i,j);
                if(transitions(neighboor)==1 or transitions(neighboor)==3){
                    Minutia temp = {static_cast<int>(i), static_cast<int>(j), compute_orientation(binary_image, i, j, ORIENTATION_DISTANCE)};
                    minutia_list.push_back(temp);
                }
            }
        }
    }
    return minutia_list;
}

Minutia apply_rotation(const Minutia &minutia, int center_row, int center_column, int rotation_in_degrees) {
    //conversion des angle en radians
    double rad_rotation = degrees_to_radians(rotation_in_degrees);
    //initialisation des valeur relative
    int x = minutia.column - center_column;
    int y = center_row - minutia.row;
    //calcul des nouvelles coordonnées
    double new_x = x*cos(rad_rotation)-y*sin(rad_rotation);
    double new_y = x*sin(rad_rotation)+y*cos(rad_rotation);
    //calcul des nouvelles collone/ligne
    int new_row = static_cast<int>(round(center_row-new_y));
    int new_column = static_cast<int>(round(new_x+center_column));
    //calcul de la nouvelle orientation
    int new_orientation = (minutia.angle_in_degrees+rotation_in_degrees)%360;
    //on retourne la nnouvelle minutie
    Minutia new_minutia = {new_row, new_column, new_orientation};
    return new_minutia;
}

Minutia apply_translation(const Minutia &minutia, int row_translation, int column_translation) {
    //on calcul les nouvelles coordoonne des row et column apres la translation
    int new_row = minutia.row-row_translation;
    int new_column = minutia.column-column_translation;
    Minutia new_minutia = {new_row, new_column, minutia.angle_in_degrees};
    return new_minutia;
}

Minutia apply_transformation(const Minutia &minutia, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
    //on applique les transfo
    Minutia rot_minutia = apply_rotation(minutia, center_row, center_column, rotation_in_degrees);
    Minutia final_minutia = apply_translation(rot_minutia, row_translation, column_translation);
    return final_minutia;
}

std::vector<Minutia> apply_transformation(const std::vector<Minutia> &minutiae, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
    //on les applique ENCORE PLUS, POUR TOUTES LES MINUTIE, HAHAHAHAHA (rire de méchant disney)
    vector<Minutia> final_minutiae(minutiae.size());
    for (size_t i = 0; i < minutiae.size(); ++i) {
        final_minutiae[i] = apply_transformation(minutiae[i], center_row, center_column, row_translation, column_translation, rotation_in_degrees);
    }
    return final_minutiae;
}

unsigned int matching_minutiae_count(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2, unsigned int max_distance, unsigned int max_orientation) {
    //compteur des toutes les minuties matchy matchy
    unsigned int minutia_count(0);
    //calcul du carré de la distance max
    unsigned int squared_max_dist = max_distance * max_distance;
    for (size_t i = 0; i < minutiae_1.size(); ++i) {
        for (size_t j = 0; j < minutiae_2.size(); ++j) {
            //distance euclidienne
            //diference row1 et row2
            int dif_row = minutiae_1[i].row-minutiae_2[j].row;
            int dif_column = minutiae_1[i].column-minutiae_2[j].column;
            unsigned int squared_dist = dif_row*dif_row+dif_column*dif_column;
            // on verifie si il sont assez proche et si leur orientation est similaire (la valeur absolue de la difference ds angles)
            if(squared_dist<=squared_max_dist and abs(minutiae_1[i].angle_in_degrees-minutiae_2[j].angle_in_degrees)<=max_orientation){
                minutia_count++;
            }
        }
    }
    return minutia_count;
}

bool match(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2) {
    if(matching_minutiae_count(minutiae_1, minutiae_2, DISTANCE_THRESHOLD, ORIENTATION_THRESHOLD)>=FOUND_THRESHOLD){
        return true;
    }
    for (size_t i = 0; i < minutiae_1.size(); ++i) {
        for (size_t j = 0; j < minutiae_2.size(); ++j) {
            //minutiae 1 est le point de reference, on calcul tout autour des minutie des cett liste
            int center_row = minutiae_1[i].row;
            int center_column = minutiae_1[i].column;
            int row_trans = minutiae_2[j].row-minutiae_1[i].row;
            int column_trans = minutiae_2[j].column-minutiae_1[i].column;
            int rotation = minutiae_2[j].angle_in_degrees-minutiae_1[i].angle_in_degrees;
            int rot_start = rotation - MATCH_ANGLE_OFFSET;
            for (int rot = rot_start; rot <= (rotation+MATCH_ANGLE_OFFSET); ++rot) {
                int match_count(0);
                vector<Minutia> modified_minutia=apply_transformation(minutiae_2, center_row, center_column, row_trans, column_trans, rot);
                match_count=matching_minutiae_count(minutiae_1, modified_minutia,DISTANCE_THRESHOLD, ORIENTATION_THRESHOLD);
                if(match_count>=FOUND_THRESHOLD){
                    return true;
                }
            }
        }
    }
    return false;
}


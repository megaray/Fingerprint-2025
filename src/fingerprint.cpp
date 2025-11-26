#include "fingerprint.hpp"
#include "test_helper.hpp"

#include <stdexcept> // for std::invalid_argument
#include <iostream>

#include "helper.hpp"
#include "utils.hpp" // for NotImplemented()

// provided constants
constexpr unsigned int ORIENTATION_DISTANCE(16);
constexpr int MATCH_ANGLE_OFFSET(2);
constexpr unsigned int DISTANCE_THRESHOLD(5);
constexpr unsigned int ORIENTATION_THRESHOLD(20);
constexpr unsigned int FOUND_THRESHOLD(20);

//gnagnan cest moi eulalie

using namespace std;

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
        vector<bool> neighbourVector(8, false); //initialisation du vecteur contenant la liste des voisins du pixel
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
        const int nRow[8] =    {-1,-1, 0, 1, 1, 1, 0, -1};
        const int nColumn[8] = { 0, 1, 1, 1, 0,-1,-1, -1};


        for (int i = 0; i <= 7; ++i) {

            //on verifie si c est dans l image
            // !! il faut convertir les  row et column en int pour la comparaison...
            // psq vu que c est des size_t bah les operation elle bug (genre size_t on peut pas faire moins...)

            int intRow = static_cast<int>(row);
            int intColumn = static_cast<int>(column);

            //DEBUG
            //cout << "Test du voisin " << i << " a row=" << intRow+nRow[i]<< " et col=" << intColumn+nColumn[i] << endl;
            //DEBUGs

            if((intRow+nRow[i]>=0 and intRow+nRow[i]<=binary_image.size()-1) and (intColumn+nColumn[i]>=0 and intColumn+nColumn[i]<=binary_image[intRow+nRow[i]].size()-1)){
                //si oui on regarde la valeur et on la stock dans le neighbourVector

                //le pixel est noir
                if(binary_image[intRow+nRow[i]][intColumn+nColumn[i]]){
                    neighbourVector[i]=true;
                }
                //le pixel est blanc
                else{
                    neighbourVector[i]=false;
                }

            }
            else{
                //si pas dans l image :
                neighbourVector[i]=false;
            }
        }
        return neighbourVector;
    }

}

unsigned int black_neighbours(std::vector<bool> neighbours) {
    // on regarde juste combien on a de voisin noir
    unsigned int count(0);
    for (size_t i=0; i<= 7; ++i) {
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
    for (int i=0; i<=7; ++i) {
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
  NotImplemented(); // TODO: implement and remove this line
}

double compute_slope(const BinaryImage &connected_pixels, size_t row, size_t column) {
  NotImplemented(); // TODO: implement and remove this line
}

double compute_angle(const BinaryImage &connected_pixels, size_t row, size_t column, double slope) {
  NotImplemented(); // TODO: implement and remove this line
}

int compute_orientation(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {
  NotImplemented(); // TODO: implement and remove this line
}

std::vector<Minutia> extract(const BinaryImage &binary_image) {
  NotImplemented(); // TODO: implement and remove this line
}

Minutia apply_rotation(const Minutia &minutia, int center_row, int center_column, int rotation_in_degrees) {
  NotImplemented(); // TODO: implement and remove this line
}

Minutia apply_translation(const Minutia &minutia, int row_translation, int column_translation) {
  NotImplemented(); // TODO: implement and remove this line
}

Minutia apply_transformation(const Minutia &minutia, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
  NotImplemented(); // TODO: implement and remove this line
}

std::vector<Minutia> apply_transformation(const std::vector<Minutia> &minutiae, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
  NotImplemented(); // TODO: implement and remove this line
}

unsigned int matching_minutiae_count(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2, unsigned int max_distance, unsigned int max_orientation) {
  NotImplemented(); // TODO: implement and remove this line
}

bool match(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2) {
  NotImplemented(); // TODO: implement and remove this line
}


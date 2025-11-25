#include "fingerprint.hpp"

#include <stdexcept> // for std::invalid_argument

#include "helper.hpp"
#include "utils.hpp" // for NotImplemented()

// provided constants
constexpr unsigned int ORIENTATION_DISTANCE(16);
constexpr int MATCH_ANGLE_OFFSET(2);
constexpr unsigned int DISTANCE_THRESHOLD(5);
constexpr unsigned int ORIENTATION_THRESHOLD(20);
constexpr unsigned int FOUND_THRESHOLD(20);



using namespace std;

std::vector<bool> get_neighbours(const BinaryImage &binary_image, size_t row, size_t column) {
    //Verification si row et colummn sont dans l image, autrement on output une erreur :
    //le ">=" vient du fait que si indices vont de 0 à 99 la taille est de 100, or si on a un indice 100 et qu'on veut le
    //mettre dans un vecteur de taille 100, bah ca marche pas...
    if(row >= binary_image.size()){
        vector<bool> err(1,false);
        throw invalid_argument("row out of range");
        return err;
    }
    else if(column >= binary_image[row].size()) {
        vector<bool> err(1,false);
        throw invalid_argument("column out of range");
        return err;
    }
    //le reste de la fonction se lance uniquement si on est PAS out of range
    else {
        vector<bool> neighbourVector(8, true); //initialisation du vecteur contenant la liste des voisins du pixel
        //7 0 1
        //6 . 2
        //5 4 3 <- strucrture des indices du vecteur !!!!!


        //verif si les vecteur adjacent sont dans la range d'analyse sinon les set a false
        // on pose des array constant qui contiennes les "coordonnée" des pixel adjacents, cela nous permet de ne pas verif le pixel analysé (ia pas besoin lol...)
        //{1,-1}  {1,0}  {1,1}
        //
        //{0,-1}     P    {0,1}
        //
        //{-1,-1} {-1,0} {-1,1} <- structure des indices du vecteur (dans la base nRow,nCLoumn)
        //
        // organiser de maniere a ce que ca concorde avc la structure des indices du neighbourVector
        //
        const int nRow[8] = {1,1,0,-1,-1,-1,0,1};
        const int nColumn[8] = {0,1,1,1,0,-1,-1,-1};

        for (int i = 0; i <= 7; ++i) {
            //on verifie si c est dans l image (note le ! au tout debut de la condition) :
            if(!(row+nRow[i]<0 or row+nRow[i]>=binary_image.size()-1 or column+nColumn[i]<0 or column+nColumn[i]>=binary_image[0].size()-1)){
                //si oui on regarde la valeur et on la stock dans le neighbourVEctor

                //le pixel est noir
                if(binary_image[row+nRow[i]][column+nColumn[i]]){
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
  NotImplemented(); // TODO: implement and remove this line
}

unsigned int transitions(std::vector<bool> neighbours) {
  NotImplemented(); // TODO: implement and remove this line
}

bool identical(const BinaryImage &binary_image_1, const BinaryImage &binary_image_2) {
  NotImplemented(); // TODO: implement and remove this line
}

BinaryImage thinning_step(const BinaryImage &binary_image, int step) {
  NotImplemented(); // TODO: implement and remove this line
}

BinaryImage thin(const BinaryImage &binary_image) {
  NotImplemented(); // TODO: implement and remove this line
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


/**
* --- Notes de développement ---
*
*
* - Les variable de ce code on été uniformisée en snake_case
* - Nous avons apporté un soin particulier à la gestion des type de variables, en particulier en ce qui concerne type : 'int' et 'size_t',
*   en raison de l'impossibilitée du size_t à être négatif.
*   Les morceau de code comprenant ces conversions ne seront pas commentée.
*
* --- Astuces mathématique utilisée ---
*
*
* --  Gestion des inégalitée --
*
*   L'inégalitée y >= -1/a*x, a été remodelée afin d'éviter la division par 0, de ce fait on a deux nouvelles inégalitées :
*   - Si 'a' est positif : y*a >= -x
*   - Si 'a' est négatif : y*a <= -x
*
*   @see compute_angle() ligne 407
*
* -- [TECH_2] Optimisation de la comparaison des distances (Euclidiennes)
*
*   L'idée était de se débarraser de la fonction native 'sqrt()', celle-ci étant appelée beaucoup, est étant très gourmande en ressource, nous avons transformée l'équation :
*   -> sqrt((row1-row2)² + (col1-col2)²) <= max_distance
*   par
*   -> (row1-row2)² + (col1-col2)² <= max_distance²
*
*   @see matching_minutiae_count() ligne 527
*
*
* --- Nomenclature des variables ---
*
*   - Variable : 'output' -> Correspond a la variable de retour, sa fonction est implicite à ce que doit retourner la fonction (dans le prototypage)
*     [Excepté pour les fonctions retournant un compteur ('count') ou un booléen ('bool')].
*   - Variable : 'count' -> Correspond à un compteur, utilisé comme output dans certaine fonctions retournant un compteur.
*   - Variable : 'next' -> Variable utilisée pour représenter la prochaine valeur calculée dans une boucle.
*   - Variable : 'i' -> Itérateur correspondant aux lignes
*   - Variable : 'j' -> Itérateur correspondant aux colonnes
*   - Variable : 'temp' -> Valeur temporaire n'ayant pas besoin d'être contextualisée
*
* --- Sources ---
*
* - https://www.geeksforgeeks.org/cpp/doxygen-cpp-documentation/ (documentation doxygen)
* - https://www.doxygen.nl/manual/docblocks.html (documentation doxygen)
* - https://google.github.io/styleguide/cppguide.html#Naming (convention de nommage C++)
* - https://www.quora.com/How-efficient-is-the-square-root-function-in-C-math-h (Efficacité des fonction de la librairie 'math.h')
* - Reddit.com (en grande partie pour la documentation doxygen et pour la nomenclature C++)
*/

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
/**
 * @brief Implementaton de la constante infinity
*/
constexpr double POSITIVE_INFINITY(std::numeric_limits<double>::infinity());
/**
 *  @brief Implementation de la valeur magique 8 en une constante NUM_NEIGHBOOR
*/
constexpr unsigned int NUM_NEIGHBOOR(8);

using namespace std;

/**
 * @brief Structure de coordonnées X et Y
*/
struct Coord{
    int x;
    int y;
};

//================================
//=== PARTIE 1: Squeletisation ===
//================================

std::vector<bool> get_neighbours(const BinaryImage& binary_image, size_t row, size_t column)
{
    /**
     * Vérifie que le pixel est dans l'image
    */
    if (row >= binary_image.size()) {
        throw std::invalid_argument("row out of range");
    }
    if (column >= binary_image[row].size()) {
        throw std::invalid_argument("column out of range");
    }

    vector<bool> output(NUM_NEIGHBOOR, false);

    /**
     * @brief Coordonnées relative au pixel analysé
    */
    const int n_row[NUM_NEIGHBOOR]    = {-1, -1, 0, 1, 1, 1, 0, -1};
    const int n_column[NUM_NEIGHBOOR] = { 0,  1, 1, 1, 0, -1,-1,-1};

    for (int i = 0; i < NUM_NEIGHBOOR; ++i) {

        int int_row    = static_cast<int>(row)    + n_row[i];
        int int_column = static_cast<int>(column) + n_column[i];

        /**
         * Vérifie que le voisin est dans l'image
        */
        if (int_row >= 0 &&
            int_row < binary_image.size() &&
            int_column >= 0 &&
            int_column < binary_image[int_row].size())
        {
            output[i] = binary_image[int_row][int_column];
        }
        else {
            output[i] = false;
        }
    }

    return output;
}

unsigned int black_neighbours(std::vector<bool> neighbours) {

    unsigned int count(0);
    /**
     * Décompte du nombre de pixels noir
    */
    for (size_t i=0; i< NUM_NEIGHBOOR; ++i) {
        if(neighbours[i]){
            count++;
        }
    }
    return count;

}

unsigned int transitions(std::vector<bool> neighbours) {

    unsigned int count(0);
    /**
     * Compte le nombre passage de blanc à noir
    */
    for (size_t i=0; i< NUM_NEIGHBOOR; ++i) {
        size_t next=(i+1)%8;
        if(!neighbours[i] and neighbours[next]){
            count++;
        }

    }
    return count;
}

bool identical(const BinaryImage &binary_image_1, const BinaryImage &binary_image_2) {
    /**
     * On verifie si les images ont le même nombre de colonnes/lignes
    */
    if(binary_image_1.size()!=binary_image_2.size()){
        return false;
    }
    else if(binary_image_1[0].size()!=binary_image_2[0].size()){
        return false;
    }
    else{
        /**
         *  Verifie que tout les pixels sont identique, dans le cas échéant retourne 'false'.
        */
        for (size_t i = 0; i < binary_image_1.size(); ++i) {
            for (size_t j = 0; j < binary_image_1[0].size(); ++j) {
                if(binary_image_1[i][j] != binary_image_2[i][j]){
                    return false;
                }
            }
        }
    }
    return true;
}

//=== Fonctions Principales ===

BinaryImage thinning_step(const BinaryImage &binary_image, int step) {    
    BinaryImage output=binary_image;

    if(step==0){
        /**
         * ETAPE 1
         * On vérifie tous les pixel de l'image d'entrée selon les critères de l'étape 1.
        */
        for (size_t i = 0; i < binary_image.size(); ++i) {
            for (size_t j = 0; j < binary_image[i].size(); ++j) {
                vector<bool> neighbours(get_neighbours(binary_image,i,j));
                /**
                * Gestion de l'exception, où la variable 'neighbours' est vide.
                */
                if(neighbours.size()==0){
                    throw invalid_argument("null neighbours vector");
                }

                if(binary_image[i][j]
                    and(2 <= black_neighbours(neighbours) and black_neighbours(neighbours)  <= 6 )
                    and transitions(neighbours)==1
                    and (!neighbours[0] or !neighbours[2] or !neighbours[4])
                    and (!neighbours[2] or !neighbours[4] or !neighbours[6])){
                    output[i][j]=false;
                }
            }
        }
        return output;
    }
    else if(step==1){
        /**
         * ETAPE 2
         * On vérifie tous les pixel de l'image d'entrée selon les critères de l'étape 2.
        */
        for (size_t i = 0; i < binary_image.size(); ++i) {
            for (size_t j = 0; j < binary_image[0].size(); ++j) {
                vector<bool> neighbours(get_neighbours(binary_image,i,j));
                /**
                * Gestion de l'exception, où la variable 'neighbours' est vide.
                */
                if(neighbours.size()==0){
                    throw invalid_argument("null neighbours vector");
                }

                if(binary_image[i][j]
                    and(2 <= black_neighbours(neighbours) and black_neighbours(neighbours)  <= 6 )
                    and transitions(neighbours)==1
                    and (!neighbours[0] or !neighbours[2] or !neighbours[6])
                    and (!neighbours[0] or !neighbours[4] or !neighbours[6])){
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

    BinaryImage output = binary_image;
    BinaryImage previous;

    /**
    * On reitère les étape de thining, tant que 'image' et 'previous' sont différente.
    */
    do{
        previous=output;

        output = thinning_step(output,0);
        output = thinning_step(output,1);
    }
    while (!identical(previous,output));
    return output;
}

//==========================
//=== PARTIE 2: Minuties ===
//==========================


BinaryImage connected_pixels(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {
    int int_row = static_cast<int>(row);
    int int_rolumn = static_cast<int>(column);

    BinaryImage output(binary_image.size(), vector<bool>(binary_image[row].size(),false));
    /**
     * @brief Minutie = true.
    */
    output[row][column]=true;

    bool loop(true);
    /**
     * Pour chaque pixel dans le périmètre d'analyse, on vérifie si celui-ci est connecté à la minutie.
     */
    while (loop) {
        loop=false;
        for (size_t i = 0;i<binary_image.size(); i++) {
            for (size_t j = 0; j < binary_image[i].size(); ++j) {

                int dist_row = abs(static_cast<int>(i) - int_row);
                int dist_column = abs(static_cast<int>(j) - int_rolumn);

                if(dist_row<=distance and dist_column<=distance){
                    /**
                     * Saute les pixels déja analysé
                     */
                    if(!output[i][j]
                        and binary_image[i][j]
                        ){
                        /**
                         * On vérifie si le pixel analysé (dans 'binary_image') a un voisin au même coordonnée dans 'output',
                         * si c'est le cas on initialise le pixel à true dans 'output'
                         */
                        if(
                            (i>0 and output[i-1][j])
                            or (i<output.size()-1 and output[i+1][j])
                            or (j>0 and output[i][j-1])
                            or (j<output[i].size()-1 and output[i][j+1])
                            or ((j<output[i].size()-1 and i<output.size()-1) and output[i+1][j+1])
                            or ((j>0 and i>0) and output[i-1][j-1])
                            or ((i<output.size()-1 and j>0) and output[i+1][j-1])
                            or ((j<output[i].size()-1 and i>0) and output[i-1][j+1])){
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
/**
 * @brief Calcul les coordonnée relative des pixels adjacents à la minutie.
 * @param BinaryImage binary_image
 * @param size_t row
 * @param size_t column
 * @return vector<Coord> relative_coord
 */
vector<Coord> compute_relative_coordinate(const BinaryImage &binary_image, size_t row, size_t column){
    /**
     * @brief Liste de coordonnées absolue x,y
     */
    vector<Coord> neighbour_list;
    /**
     * On cherche les pixels noir, et on met leur coordonnée absolue dans la nouvelle liste ('neighbour_list').
     */
    for (size_t i=0;i<=binary_image.size()-1; ++i) {
        for (size_t j=0;j<=binary_image[i].size()-1; ++j) {
            if(binary_image[i][j]){
                Coord temp;
                temp.y = static_cast<int>(i);
                temp.x = static_cast<int>(j);
                neighbour_list.push_back(temp);
            }
        }
    }
    /**
     * @brief Liste de coordonnées relatives x,y
     */
    vector<Coord> output(neighbour_list.size());
    /**
     * Calcul des coordonnée relatives.
     */
    for (size_t i = 0; i < neighbour_list.size(); ++i) {
        output[i].x = neighbour_list[i].x - static_cast<int>(column);
        output[i].y = static_cast<int>(row) - neighbour_list[i].y;
    }
    return output;
}

double compute_slope(const BinaryImage &connected_pixels, size_t row, size_t column) {
    double output(0);
    double sum_x2 = 0.0;
    double sum_y2 = 0.0;
    double sum_xy = 0.0;

    vector<Coord> relative_coord = compute_relative_coordinate(connected_pixels,row,column);
    /**
     * Calcul des sommes x*y, x² et y²
     */
    for (size_t i=0; i<relative_coord.size(); ++i) {
        int x=relative_coord[i].x;
        int y=relative_coord[i].y;
        sum_xy+=x*y;
        sum_x2+=x*x;
        sum_y2+=y*y;
    }
    /**
     * Gestion de l'exception où la pente est verticale.
     */
    if(sum_x2==0){
        return POSITIVE_INFINITY;
    }
    /**
     * Calcul de la pente.
     */
    else{
        if(sum_x2>=sum_y2){
            output=sum_xy/sum_x2;
        }
        else if(sum_x2<sum_y2){
            output=sum_y2/sum_xy;
        }
        return output;
    }
 }

double compute_angle(const BinaryImage &connected_pixels, size_t row, size_t column, double slope) {

    double output(0);
    vector<Coord> relative_coord = compute_relative_coordinate(connected_pixels,row,column);
    /**
     * Gestion de l'exception où la pente est verticale
     */
    if(slope==POSITIVE_INFINITY){

        int sum_y(0);
        /**
         * Calcul du nombre de pixels en haut/bas de la droite normale à la pente.
         * Le sens (et donc l angle) de la droite est définis en fonction du signe de la somme('+' -> π/2 | '-' -> -π/2).
         */
        for (size_t i = 0; i < relative_coord.size(); ++i) {
            sum_y+= relative_coord[i].y;
        }
        if(sum_y>0){
            output = M_PI/2.0;
            return output;
        }else{
            output = (-M_PI)/2.0;
            return output;
        }
    }


    output = atan(slope);

    int up_count(0);
    int down_count(0);
    /**
     * Calcul du nombre de pixels en haut/bas de la droite normale à la pente.
     * Le sens (et donc l angle) de la droite est définis en fonction du signe de la somme ET du signe de l angle :
     * '+ angle' et '- pixel en haut' -> angle + π
     * '- angle' et '+ pixel en haut' -> angle + π
     * autrement on retourne l angle inchangé.
     */
    for (size_t i = 0; i < relative_coord.size(); ++i) {

        if(slope>0 and (relative_coord[i].y*slope>=-relative_coord[i].x)){
            up_count++;
        }
        else if(slope<0 and (relative_coord[i].y*slope<=-relative_coord[i].x)){
            up_count++;
        }
        /**
         * Si la pente est nulle on regarde juste combien de coordonnées y sont positive (au dessus de la ligne)
         */
        else if(slope==0 and (relative_coord[i].y>=0)){
            up_count++;
        }
        else{
            down_count++;
        }
    }
    if((output>0 and down_count>up_count) or (output<0 and down_count<up_count)){
        return output + M_PI;
    }
    else{
        return output;
    }
}

int compute_orientation(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {

    BinaryImage connected_list = connected_pixels(binary_image, row, column, distance);
    double slope = compute_slope(connected_list, row, column);
    double angle = compute_angle(connected_list, row, column, slope);
    double angle_deg = radians_to_degrees(angle);

    /**
     * Après avoir calculé l'angle en degrés de la droite des pixels adjacents à la mintutie
     * on normalise l'angle, puis on l'arrondi.
     */
    if(angle_deg<0){
        angle_deg+=360;
    }
    int output = static_cast<int>(std::round(angle_deg));
    return output;
}

std::vector<Minutia> extract(const BinaryImage &binary_image) {
    vector<Minutia> output;
    /**
     * Vérifie si les pixels de l'image sont des minuties ou non.
     * Dans le cas ou le pixel est une minutie, la liste des voisins du pixel retourne 1 ou 3
     * lorsqu'on applique la fonction 'transition()' à la liste.
     */
    for (size_t i = 1; i < binary_image.size()-1; ++i) {
        for (size_t j = 1; j < binary_image[i].size()-1; ++j) {
            if(binary_image[i][j]){
                vector<bool> neighbour = get_neighbours(binary_image,i,j);
                if(transitions(neighbour)==1 or transitions(neighbour)==3){
                    Minutia temp = {static_cast<int>(i), static_cast<int>(j), compute_orientation(binary_image, i, j, ORIENTATION_DISTANCE)};
                    output.push_back(temp);
                }
            }
        }
    }
    return output;
}

//==============================
//=== PARTIE 3: Comparaisons ===
//==============================


Minutia apply_rotation(const Minutia &minutia, int center_row, int center_column, int rotation_in_degrees) {

    double rad_rotation = degrees_to_radians(rotation_in_degrees);
    /**
     * @brief Coordonnées 'x' relative à la minutie.
     */
    int x = minutia.column - center_column;
    /**
     * @brief Coordonnées 'y' relative à la minutie.
     */
    int y = center_row - minutia.row;

    /**
     * @brief Nouvelle coordonnée 'x' après avoir appliqué la rotation.
     */
    double new_x = x*cos(rad_rotation)-y*sin(rad_rotation);
    /**
     * @brief Nouvelle coordonnée 'y' après avoir appliqué la rotation.
     */
    double new_y = x*sin(rad_rotation)+y*cos(rad_rotation);

    /**
     * @brief Calcul de la nouvelle coordonnée absolue de la ligne.
     */
    int new_row = static_cast<int>(round(center_row-new_y));
    /**
     * @brief Calcul de la nouvelle coordonnée absolue de la colonne.
     */
    int new_column = static_cast<int>(round(new_x+center_column));


    /**
     * @brief Calcul de la nouvelle orientation.
     */
    int new_orientation = (minutia.angle_in_degrees+rotation_in_degrees)%360;


    Minutia output = {new_row, new_column, new_orientation};
    return output;
}

Minutia apply_translation(const Minutia &minutia, int row_translation, int column_translation) {
    /**
     * @brief Calcul de la nouvelle ligne après translation.
     */
    int new_row = minutia.row-row_translation;
    /**
     * @brief Calcul de la nouvelle colonne après translation.
     */
    int new_column = minutia.column-column_translation;

    Minutia output = {new_row, new_column, minutia.angle_in_degrees};
    return output;
}

Minutia apply_transformation(const Minutia &minutia, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
    /**
     * @brief Rotation de la minutie
     */
    Minutia rot_minutia = apply_rotation(minutia, center_row, center_column, rotation_in_degrees);
    /**
     * @brief Translation de la minutie
     */
    Minutia trans_minutia = apply_translation(rot_minutia, row_translation, column_translation);

    Minutia output = trans_minutia;
    return output;
}

std::vector<Minutia> apply_transformation(const std::vector<Minutia> &minutiae, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {

    vector<Minutia> output(minutiae.size());

    /**
     * Applique les transformation pour toutes les minuites de la liste
     */
    for (size_t i = 0; i < minutiae.size(); ++i) {
        output[i] = apply_transformation(minutiae[i], center_row, center_column, row_translation, column_translation, rotation_in_degrees);
    }
    return output;
}

unsigned int matching_minutiae_count(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2, unsigned int max_distance, unsigned int max_orientation) {

    unsigned int count(0);

    unsigned int squared_max_dist = max_distance * max_distance;

    /**
     * On évalue si deux minutie se superpose,
     * ce sur toutes la liste de minuties.
     */
    for (size_t i = 0; i < minutiae_1.size(); ++i) {
        for (size_t j = 0; j < minutiae_2.size(); ++j) {

            int dif_row = minutiae_1[i].row-minutiae_2[j].row;
            int dif_column = minutiae_1[i].column-minutiae_2[j].column;

            unsigned int squared_dist = dif_row*dif_row+dif_column*dif_column;
            /**
             * Si la distance euclidienne au carré est <= à la distance maximum autorisé au carré
             * et que la valeur absolue de l'angle entre les deux minuties est <= à la différence d'orientation maximum autorisé
             */
            if(squared_dist<=squared_max_dist and abs(minutiae_1[i].angle_in_degrees-minutiae_2[j].angle_in_degrees)<=max_orientation){
                count++;
            }
        }
    }
    return count;
}

bool match(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2) {
    /**
     * Premièrement, on verifie si le nombre de minutie qui se superpose est >= au seuil autorisé,
     */
    if(matching_minutiae_count(minutiae_1, minutiae_2, DISTANCE_THRESHOLD, ORIENTATION_THRESHOLD)>=FOUND_THRESHOLD){
        return true;
    }

    /**
     * On applique les transformations à la minutie 2 autour de la minutie 1 afin de verifier si les deux minutie se superpose,
     * dans le cas écheant on retourne false.
     */
    for (size_t i = 0; i < minutiae_1.size(); ++i) {
        for (size_t j = 0; j < minutiae_2.size(); ++j) {
            /**
             * @brief Ligne de réference (minutie 1)
             */
            int center_row = minutiae_1[i].row;
            /**
             * @brief Colonne de réference (minutie 1)
             */
            int center_column = minutiae_1[i].column;

            /**
             * @brief Longueur de la translation verticale
             */
            int row_trans = minutiae_2[j].row-minutiae_1[i].row;

            /**
             * @brief Longueur de la translation horizontale
             */
            int column_trans = minutiae_2[j].column-minutiae_1[i].column;
            /**
             * @brief Différence d'orientation entre minutie 1 et minutie 2
             */
            int rotation = minutiae_2[j].angle_in_degrees-minutiae_1[i].angle_in_degrees;

            /**
             * Calcul de toutes les transformations possibles et comparaison entre les deux minuties.
             */
            for (int rot = rotation - MATCH_ANGLE_OFFSET; rot <= (rotation+MATCH_ANGLE_OFFSET); ++rot) {
                int count(0);

                vector<Minutia> modified_minutia=apply_transformation(minutiae_2, center_row, center_column, row_trans, column_trans, rot);

                count=matching_minutiae_count(minutiae_1, modified_minutia,DISTANCE_THRESHOLD, ORIENTATION_THRESHOLD);
                if(count>=FOUND_THRESHOLD){
                    return true;
                }
            }
        }
    }
    return false;
}


#pragma once

#include "helper.hpp"

/**
 * Represents a minutia.
 */
typedef struct Minutia {
  /**
   * The row coordinate of the minutia.
   */
  int row;

  /**
   * The column coordinate of the minutia.
   */
  int column;

  /**
   * The angle of the minutia in degrees.
   */
  int angle_in_degrees;
} Minutia;

/**
 * Returns an array containing the value of the 8 neighbours of the pixel at
 * coordinates (row, column).
 *
 * The pixels are returned such that their indices correspond to the following
 * diagram:
 * -------------
 * | 7 | 0 | 1 |
 * -------------
 * | 6 | _ | 2 |
 * -------------
 * | 5 | 4 | 3 |
 * -------------
 *
 * If a neighbour is out of bounds of the image, it is considered white.
 *
 * @param binary_image  Array containing each pixel's boolean value.
 * @param row           The row of the pixel of interest, must be between
 *                      0 (inclusive) and image.length (exclusive).
 * @param column        The column of the pixel of interest, must be between
 *                      0 (inclusive) and image[row].length (exclusive).
 *
 * @return An array containing each neighbour's value.
 *
 * @throw std::invalid_argument If the row or column is out of bounds.
 */
std::vector<bool> get_neighbours(const BinaryImage &binary_image, size_t row, size_t column);

/**
 * Computes the number of black (true) pixels among the neighbours of a pixel.
 *
 * @param neighbours  Array containing each pixel value. The array must respect
 *                    the convention described in the get_neighbours() function.
 *
 * @return The number of black neighbours.
 */
unsigned int black_neighbours(std::vector<bool> neighbours);

/**
 * Computes the number of white to black transitions among the neighbours of
 * a pixel.
 *
 * @param neighbours  Array containing each pixel value. The array must respect
 *                    the convention described in the get_neighbours() function.
 *
 * @return The number of white to black transitions.
 */
unsigned int transitions(std::vector<bool> neighbours);

/**
 * Returns true if the images are identical and false otherwise.
 *
 * @param binary_image_1  Array containing each pixel's boolean value.
 * @param binary_image_2  Array containing each pixel's boolean value.
 *
 * @return true if they are identical, false otherwise.
 */
bool identical(const BinaryImage &binary_image_1, const BinaryImage &binary_image_2);

/**
 * Internal method used by the thin() function.
 *
 * @param binary_image  Array containing each pixel's boolean value.
 * @param step          The step to apply, Step 0 or Step 1.
 *
 * @return A new array containing each pixel's value after the step.
 */
BinaryImage thinning_step(const BinaryImage &binary_image, int step);

/**
 * Compute the skeleton of a boolean image.
 *
 * @param binary_image  Array containing each pixel's boolean value.
 *
 * @return An array containing the boolean value of each pixel of the image
 *         after applying the thinning algorithm.
 */
BinaryImage thin(const BinaryImage &binary_image);

/**
 * Computes all pixels that are connected to the pixel at coordinate
 * (row, column) and within the given distance of the pixel.
 *
 * @param binary_image  Array containing each pixel's boolean value.
 * @param row           The row of the pixel of interest.
 * @param column        The column of the pixel of interest.
 * @param distance      The maximum distance at which a pixel is considered.
 *
 * @return An array where true means that the pixel is within
 *         distance and connected to the pixel at (row, column).
 */
BinaryImage connected_pixels(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance);

/**
 * Computes the slope of the minutia at the coordinate (row, column) using
 * linear regression.
 *
 * @param connected_pixels  The result of the connected_pixels() function.
 * @param row               The row of the pixel of interest.
 * @param column            The column of the pixel of interest.
 *
 * @return The slope.
 */
double compute_slope(const BinaryImage &connected_pixels, size_t row, size_t column);

/**
 * Computes the angle of the minutia at the coordinate (row, column) in radians.
 * 
 * @param connected_pixels  The result of the connected_pixels() function.
 * @param row               The row of the pixel of interest.
 * @param column            The column of the pixel of interest.
 * @param slope             The slope as returned by the compute_slope()
 *                          function.
 *
 * @return The orientation of the minutia in radians.
 */
double compute_angle(const BinaryImage &connected_pixels, size_t row, size_t column, double slope);

/**
 * Computes the orientation of the minutia at the coordinate (row, column).
 *
 * @param binary_image  Array containing each pixel's boolean value.
 * @param row           The row of the pixel of interest.
 * @param column        The column of the pixel of interest.
 * @param distance      The distance to be considered in each direction to
 *                      compute the orientation.
 *
 * @return The orientation in degrees, between 0 (inclusive) and 360
 *         (exclusive).
 */
int compute_orientation(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance);

/**
 * Extracts the minutiae from a thinned image.
 *
 * @param binary_image  Array containing each pixel's boolean value. The result
 *                      of the thin() function.
 *
 * @return The list of all minutiae.
 */
std::vector<Minutia> extract(const BinaryImage &binary_image);

/**
 * Applies the specified rotation to the minutia.
 *
 * @param minutia              The original minutia.
 * @param center_row           The row of the center of rotation.
 * @param center_column        The column of the center of rotation.
 * @param rotation_in_degrees  The rotation in degrees.
 *
 * @return The minutia rotated around the given center.
 */
Minutia apply_rotation(const Minutia &minutia, int center_row, int center_column, int rotation_in_degrees);

/**
 * Applies the specified translation to the minutia.
 *
 * @param minutia             The original minutia.
 * @param row_translation     The translation along the rows.
 * @param column_translation  The translation along the columns.
 *
 * @return The translated minutia.
 */
Minutia apply_translation(const Minutia &minutia, int row_translation, int column_translation);

/**
 * Computes the row, column, and angle after applying a transformation
 * (translation and rotation).
 *
 * @param minutia             The original minutia.
 * @param center_column       The column around which the point is rotated.
 * @param center_row          The row around which the point is rotated.
 * @param row_translation     The vertical translation.
 * @param column_translation  The horizontal translation.
 * @param rotation            The rotation.
 *
 * @return The transformed minutia.
 */
Minutia apply_transformation(const Minutia &minutia, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees);

/**
 * Computes the row, column, and angle after applying a transformation
 * (translation and rotation) for each minutia in the given list.
 *
 * @param minutiae            The list of minutiae.
 * @param center_column       The column around which the point is rotated.
 * @param center_row          The row around which the point is rotated.
 * @param row_translation     The vertical translation.
 * @param column_translation  The horizontal translation.
 * @param rotation            The rotation.
 *
 * @return The list of transformed minutiae.
 */
std::vector<Minutia> apply_transformation(const std::vector<Minutia> &minutiae, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees);

/**
 * Counts the number of overlapping minutiae.
 *
 * Two minutiae are considered overlapping if their distance is less than or
 * equal to max_distance and their angular difference is less than or equal to
 * max_orientation.
 *
 * @param minutiae_1       The first set of minutiae.
 * @param minutiae_2       The second set of minutiae.
 * @param max_distance     The maximum distance between two minutiae to consider
 *                         them as overlapping.
 * @param max_orientation  The maximum difference of orientation between two
 *                         minutiae to consider them as overlapping.
 *
 * @return The number of overlapping minutiae.
 */
unsigned int matching_minutiae_count(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2, unsigned int max_distance, unsigned int max_orientation);

/**
 * Compares the minutiae from two fingerprints.
 *
 * @param minutiae_1  The list of minutiae of the first fingerprint.
 * @param minutiae_2  The list of minutiae of the second fingerprint.
 *
 * @return true if they match and false otherwise.
 */
bool match(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2);


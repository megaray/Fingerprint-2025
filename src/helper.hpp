#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * The value of pi.
 */
constexpr double PI(3.14159265358979);

/**
 * A 2D array representation of an image where each pixel is stored as a 32-bit
 * unsigned integer with the ARGB format (alpha, red, green, blue).
 *
 * The outer vector represents rows and the inner vector represents columns.
 */
typedef std::vector<std::vector<std::uint32_t>> ARGBImage;

/**
 * A 2D array representation of a binary image.
 *
 * This type is used to store binary images where each pixel is represented
 * by a boolean value.
 *
 * The outer vector represents rows and the inner vector represents columns.
 */
typedef std::vector<std::vector<bool>> BinaryImage;

typedef struct Minutia Minitua; // forward declaration

/**
 * Checks if a file with the given filename exists in the file system.
 *
 * @param filename  The name of the file to check.
 *
 * @return true if the file exists, false otherwise.
 */
bool exists(const std::string &filename);

/**
 * Reads an ARGB image from a file.
 *
 * @param filename  The name of the file to read.
 *
 * @return The ARGB image as a 2D vector of 32-bit unsigned integers.
 */
ARGBImage read_argb(const std::string &filename);

/**
 * Reads a binary image from a file.
 *
 * @param filename  The name of the file to read.
 *
 * @return The binary image as a 2D vector of booleans.
 */
BinaryImage read_binary(const std::string &filename);

/**
 * Writes an ARGB image to a file.
 *
 * @param image     The ARGB image to write.
 * @param filename  The name of the file to write the image to.
 */
void write_argb(const ARGBImage &image, const std::string &filename);

/**
 * Writes a binary image to a file.
 *
 * @param image     The binary image to write.
 * @param filename  The name of the file to write the image to.
 */
void write_binary(const BinaryImage &image, const std::string &filename);

/**
 * Converts an ARGB image to a binary image.
 *
 * @param argb_image  The ARGB image to convert.
 *
 * @return The corresponding binary image.
 */
BinaryImage argb_to_binary(const ARGBImage &argb_image);

/**
 * Converts a binary image to an ARGB image.
 *
 * @param binary_image  The binary image to convert.
 *
 * @return The corresponding ARGB image.
 */
ARGBImage binary_to_argb(const BinaryImage &binary_image);

/**
 * Helper function to compares two binary images.
 *
 * @param binary_image1  The first  binary image to compare.
 * @param binary_image2  The second binary image to compare.
 * @return An ARGB image where matching black pixels stay black and differing
 *         black pixels are shown in red.
 */
ARGBImage binary_diff_to_argb(const BinaryImage &binary_image1, const BinaryImage &binary_image2);

/**
 * Draws minutiae on top of an ARGB image.
 *
 * @param argb_image  The ARGB image on which to draw the minutiae.
 * @param minutiae    The list of minutiae to draw.
 */
void draw_minutia(ARGBImage &argb_image, const std::vector<Minutia> &minutiae);

/**
 * Converts degrees to radians.
 *
 * @param degrees  The angle in degrees to convert.
 *
 * @return The angle in radians.
 */
double degrees_to_radians(double degrees);

/**
 * Converts radians to degrees.
 *
 * @param radians  The angle in radians to convert.
 *
 * @return The angle in degrees.
 */
double radians_to_degrees(double radians);

/**
 * Cast int to size_t.
 *
 * @param int  The value to cast.
 *
 * @return The value casted to size_t.
 */
inline size_t to_size_t(int value)
{
    return static_cast<size_t>(value);
}

/**
 * Cast size_t to int.
 *
 * @param size_t  The value to cast.
 *
 * @return The value casted to int.
 */
inline int to_int(size_t value)
{
    return static_cast<int>(value);
}

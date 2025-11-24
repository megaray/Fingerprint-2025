#pragma once

#include <string>
#include <vector>

#include "fingerprint.hpp"

/**
 * Whether or not to show expected and actual output in print_assertion_result()
 * even if the assertion passes.
 */
extern bool verbose;

/**
 * Convert a boolean to a string.
 *
 * @param boolean  The boolean.
 *
 * @return The boolean as a string.
 */
std::string bool_to_string(bool boolean);

/**
 * Convert a 2D vector of booleans to a string.
 *
 * @param vec  The 2D vector of booleans.
 *
 * @return The 2D vector as a string.
 */
std::string vector_to_string(const std::vector<std::vector<bool>> &vec);

/**
 * Convert a vector of booleans to a string.
 *
 * @param vec  The vector of booleans.
 *
 * @return The vector as a string.
 */
std::string vector_to_string(const std::vector<bool> &vec);

/**
 * Convert a minutia to a string.
 *
 * @param minutia  The minutia.
 *
 * @return The minutia as a string.
 */
std::string minutia_to_string(const Minutia &minutia);

/**
 * Convert a list of minutia to a string.
 *
 * @param minutia  The minutiae.
 *
 * @return The minutiae as a string.
 */
std::string minutiae_to_string(const std::vector<Minutia> &minutiae, const std::string &prefix = "");

/**
 * Print a header for the tests of the given part of the project.
 *
 * @param part  The part (must be 1, 2, or 3).
 */
void print_part_header(unsigned int part);

/**
 * Print a header for a test.
 *
 * @param name  The test name.
 */
void print_test_header(const std::string &name);

/**
 * Print the result of an assertion.
 *
 * If the assertion passes, prints OK.
 * If the assertion fails, prints FAIL as well as the expected and actual outputs.
 *
 * If verbose mode is activated, also prints the expected and actual outputs when the assertion passes.
 *
 * @param passes    The assertion.
 * @param expected  The expected value represented as a printable string.
 * @param actual    The actual value represented as a printable string.
 */
void print_assertion_result(bool passes, const std::string &expected, const std::string &actual);

/**
 * Print the result of comparing the content of two images
 *
 * If the images are the same  (passes==true), prints OK.
 * else prints FAILED
 *
 *
 * @param passes    the result of the comparison
 * @param expected  The expected filename
 * @param actual    The actual filename
 */
void print_file_comparison_result(bool passes, const std::string &expected_filename,
                                  const std::string &actual_filename);


/**
 * Equality function for Minutia.
 *
 * @param minutia_1  The first minutia.
 * @param minutia_2  The second minutia.
 */
bool operator==(const Minutia &minutia_1, const Minutia &minutia_2);

/**
 * Equality function for BinaryImage
 *
 * @param binary_image_1  The first binary_image
 * @param binary_image_2  The second binary_image.
 */
bool operator==(const BinaryImage& image_1, const BinaryImage& image_2);

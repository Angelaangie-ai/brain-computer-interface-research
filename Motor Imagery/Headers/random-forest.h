#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <math.h>
#include <time.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Structure representing the brain data.
 */
struct brain {
  double value[2];  // stores two values
  string species;   // stores whether the left or the right side is more active
};

/**
 * Structure representing a node in a decision tree.
 */
struct node {
  int feature;         // feature used for splitting
  bool isLeaf;         // indicates if the node is a leaf
  double gap_value;    // threshold value used to split the data
  struct node* higher; // pointer to the higher value branch
  struct node* lower;  // pointer to the lower value branch
  vector<brain> data;  // data associated with the node
};

/**
 * Reads the data from a file and populates a vector of brain objects.
 *
 * @param data The vector to store the brain data.
 */
void InputData(vector<brain>& data);

/**
 * Calculates the entropy based on the number of occurrences in different categories.
 *
 * @param numLeft The count of left occurrences.
 * @param numRight The count of right occurrences.
 * @param datalength The total length of the data.
 * @return The calculated entropy value.
 */
double CalEntropy(int numLeft, int numRight, int datalength);

/**
 * Calculates the information gain based on three subsets of data.
 *
 * @param data The original data set.
 * @param higherr The data set that holds higher points.
 * @param lower The data set that holds lower points.
 * @param feature The feature used for splitting.
 * @return The calculated information gain.
 */
double CalculateGain(vector<brain> data, vector<brain> higherr,
                     vector<brain> lower, int feature);

/**
 * Shuffles the data in a random order.
 *
 * @param data The vector of brain data to be shuffled.
 */
void Shuffle(vector<brain>& data);

/**
 * Compares the values of brain objects based on the first value.
 *
 * @param a The first brain object.
 * @param b The second brain object.
 * @return True if the first value of a is less than the first value of b, False otherwise.
 */
bool Sort_compare0(brain a, brain b);

/**
 * Compares the values of brain objects based on the second value.
 *
 * @param a The first brain object.
 * @param b The second brain object.
 * @return True if the second value of a is less than the second value of b, False otherwise.
 */
bool Sort_compare1(brain a, brain b);

/**
 * Constructs a binary decision tree recursively.
 *
 * @param data The data associated with the current node.
 * @param root The root node of the decision tree.
 */
void Construct_tree(vector<brain>& data, struct node*& root);

/**
 * Performs K-fold cross-validation to evaluate the performance of the decision tree model.
 *
 * @param data The vector of brain data.
 */
void K_fold_Validation(vector<brain>& data);

/**
 * Performs testing on a decision tree model using test data.
 *
 * @param data The vector of brain data for testing.
 * @param root The root node of the decision tree.
 * @return The vector of test results.
 */
vector<double> Test_tree(vector<brain>& data, struct node* root);

/**
 * Outputs the decision tree structure.
 *
 * @param root The root node of the decision tree.
 */
void Output_tree(struct node* root);

#endif  // DECISION_TREE_H

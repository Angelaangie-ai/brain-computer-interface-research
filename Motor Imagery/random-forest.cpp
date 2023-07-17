#include "Headers/random-forest.h"
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
 Defining the structure for the brain
*/
struct brain {
  double value[2];  // stores two values TBA: what are the values doing
  string species;   // stores whether the left or the right side is more active
};

/**
 Represents a node in a decision tree
*/
struct node {
  int feature;
  ? bool isLeaf;
  double gap_value;     // threshold value used to split the value
  struct node *higher;  // values greater than the gap_value
  struct node *lower;   // values lower than the gap value
  vector<brain> data;
};

/**
 Reads the data and populates a vector of brain objects
*/
void InputData(vector<brain> &data) {
  ifstream input;
  input.open("BrainData.txt");
  string buffer, buffer2;
  // declaring two value to store
  for (int i = 0; i < 150; i++) {
    input >> buffer;
    buffer2 = buffer2.assign(buffer, 0, 10);  // extracts the first value
    data[i].value[0] = stod(buffer2);
    buffer2 = buffer2.assign(buffer, 12, 12);  // extracts the second value
    data[i].value[1] = stod(buffer2);
    buffer2 = buffer2.assign(buffer, 24, 4);
    data[i].species =
        buffer2;  // assigns whether the movement is for the left or right side
  }
}

/**
 Calculates the entropy based on the number of the occurences in the different
 categories
*/
double CalEntropy(int numLeft, int numRight, int datalength) {
  double result = 0;
  if (datalength == 0)  // case when the data is empty
  {
    return 0;
  }
  // Calculates the entropy for the left category
  if (numLeft != 0) {
    result += -(numLeft / (double)datalength) *
              (log(numLeft / (double)datalength) / log(2));
  }
  // Calculates the entropy for the right category
  if (numRight != 0) {
    result += -(numRight / (double)datalength) *
              (log(numRight / (double)datalength) / log(2));
  }
  return result;
}

/**
 Calculates the information gain based on three subsets of data
 Subsets include: The original data set, The data set that holds lower points,
 The data set that holds the higher
*/
double CalculateGain(vector<brain> data, vector<brain> higherr,
                     vector<brain> lower, elements feature) {
  int num_Left, num_Right;
  // stores the count of occurences of left and right species
  double data_Left_percent, data_Right_percent;
  // stores the percentage of species in the right data vector
  double higher_Left_percent, higher_Right_percent;
  double lower_Left_percent, lower_Right_percent;
  // stores the calculated remainder value
  double remainder;
  double data_entropy, higher_entropy, lower_entropy;

  // count occurences of left and right species in the data vector
  num_Left = num_Right = 0;
  for (int i = 0; i < data.size(); i++) {
    // If the species is left - we increase num_Left, and vice versa for right
    // species
    if (data[i].species == "Left") {
      num_Left++;
    } else if (data[i].species == "Right") {
      num_Right++;
    } else
      cout << "error." << endl;
  }

  // calculate the percentage of species in a data vector
  data_Left_percent = num_Left / (double)data.size();
  data_Right_percent = num_Right / (double)data.size();

  // calculate the entropy of the data vector
  data_entropy = 0;
  if (num_Left > 0) {
    data_entropy += -(data_Left_percent * log2(data_Left_percent));
  }
  if (num_Right > 0) {
    data_entropy += -(data_Right_percent * log2(data_Right_percent));
  }

  // Calculate the entropy for the higher subset
  num_Left = num_Right = 0;
  for (int i = 0; i < higher.size(); i++) {
    if (higher[i].species == "Left") {
      num_Left++;
    } else if (higher[i].species == "Right") {
      num_Right++;
    } else
      cout << "error." << endl;
  }
  // calculating the percentage of left species in the higher subset
  higher_Left_percent = num_Left / (double)higher.size();
  // calculating the percentage of right species in the higher subset
  higher_Right_percent = num_Right / (double)higher.size();
  higher_entropy = 0;
  if (num_Left > 0) {
    higher_entropy += -(higher_Left_percent * log2(higher_Left_percent));
  }
  if (num_Right > 0) {
    higher_entropy += -(higher_Right_percent * log2(higher_Right_percent));
  }

  // Calculate the entropy for the lower subset
  num_Left = num_Right = 0;
  for (int i = 0; i < lower.size(); i++) {
    if (lower[i].species == "Left") {
      num_Left++;
    } else if (lower[i].species == "Right") {
      num_Right++;
    } else
      cout << "error." << endl;
  }
  // calculating the percentage of left species in the lower subset
  lower_Left_percent = num_Left / (double)lower.size();
  // calculating the percentage of right species in the lower subset
  lower_Right_percent = num_Right / (double)lower.size();
  lower_entropy = 0;

  if (num_Left > 0) {
    lower_entropy += -(lower_Left_percent * log2(lower_Left_percent));
  }
  if (num_Right > 0) {
    lower_entropy += -(lower_Right_percent * log2(lower_Right_percent));
  }
  remainder = ((higher.size() / (double)data.size()) * higher_entropy) +
              ((lower.size() / (double)data.size()) * lower_entropy);
  return data_entropy - remainder;
}

/**
 Function to shuffle the data in a random order
*/
void Shuffle(vector<brain> &data) {
  // create a temporary vector to store the data
  vector<brain> temp;
  // boolean value to keep track of the moved items
  bool isPut[155] = {false};
  temp.assign(data.begin(), data.end());
  // parameters to keep track how much of the information has shifted
  int buffer = 0;
  int location = 0;
  for (int i = 150; i > 0; i--) {
    buffer = rand() % i + 1;
    location = -1;
    for (int j = 0; j < buffer; j++) {
      location++;
      if (isPut[location] == true) {
        continue;
      }
    }
    data[location] = temp[i - 1];
    isPut[location] = true;
  }
}
/**
 Compares the values of the brain objects
*/
bool Sort_compare0(brain a, brain b) { return (a.value[0] < b.value[0]); }
bool Sort_compare1(brain a, brain b) { return (a.value[1] < b.value[1]); }

/**
 Construct a binary tree
*/
void Construct_tree(vector<brain> &data, struct node *&root) {
  root = new struct node;
  root->data.assign(data.begin(), data.end());
  root->isLeaf = true;
  root->species = data[0].species;
  // if the data is not from the same species, we start to divide it into a root
  for (int i = 1; i < data.size(); i++) {
    if (data[i].species != data[i - 1].species) {
      root->isLeaf = false;
      root->species = "";
      break;
    }
  }
  if (root->isLeaf == true) {
    return;
  }
  // Creating a vector that stores the species label change
  vector<int> species_change_num;
  double highest_gain = 0;  // tracks the information gain
  double highest_gap[2];    // stores the values connected to the gain
  vector<brain> lower_data, higher_data;
  for (int i = 0; i < 4; i++) {
    species_change_num.clear();
    switch (i) {
        // with each iteration, the data is evaluated based on two sorting
        // functions
      case 0:
        sort(data.begin(), data.end(), Sort_compare0);
        break;
      case 1:
        sort(data.begin(), data.end(), Sort_compare1);
        break;
    }
    // iterate to find the indices change is
    for (int j = 1; j < data.size(); j++) {
      if (data[j].species != data[j - 1].species) {
        species_change_num.push_back(j);
      }
    }
    // splitting the data into two subsets: higher and lower
    for (int j = 0; j < species_change_num.size(); j++) {
      vector<brain> lower;
      vector<brain> higher;
      lower.clear();
      higher.clear();
      lower.assign(data.begin(), data.begin() + species_change_num[j]);
      higher.assign(data.begin() + species_change_num[j], data.end());
      double temp = CalculateGain(data, higher, lower, i);
      // cout << temp << endl;
      if (temp >= highest_gain) {
        // Iterate through each species in the data vector, assigning that
        // species to the lower and higher
        highest_gain = temp;
        highest_gap[0] = i;
        highest_gap[1] = data[species_change_num[j]].value[i];
        lower_data.assign(lower.begin(), lower.end());
        higher_data.assign(higher.begin(), higher.end());
      }
    }
  }
  root->feature = highest_gap[0];
  root->gap_value = highest_gap[1];
  root->higher = NULL;
  root->lower = NULL;
  // Constructing the trees
  Construct_tree(higher_data, root->higher);
  Construct_tree(lower_data, root->lower);
  return;
}

/**
 Evaluating the performance of a decision tree model, takes a reference to a
 vector of 'brain' objects
*/
void K_fold_Validation(vector<brain> &data) {
  vector<double> result;
  vector<double> mean_total_result;
  vector<brain> train_data, test_data, temp, temp2;
  // Resize the result to include 7 places
  mean_total_result.resize(7);
  for (int i = 0; i < 7; i++) {
    mean_total_result[i] = 0;
  }
  // Perform 5 iterations - 5 folds of cross-violation
  for (int i = 0; i < 5; i++) {
    train_data.clear();
    test_data.clear();
    //In each iteration, the training and test datasets are created. 
    test_data.assign(data.begin() + i * 30, data.begin() + (i + 1) * 30);
    temp.assign(data.begin(), data.begin() + i * 30);
    temp2.assign(data.begin() + (i + 1) * 30, data.begin() + 150);
    train_data.insert(train_data.end(), temp.begin(), temp.end());
    train_data.insert(train_data.end(), temp2.begin(), temp2.end());
    struct node *root = NULL;  
    Construct_tree(train_data, root);
    result = Test_tree(test_data, root);
    for (int i = 0; i < 7; i++) {
      mean_total_result[i] += result[i];
    }
  }
  for (int i = 0; i < 7; i++) {
    mean_total_result[i] = mean_total_result[i] / 5.0;
  }
  cout << mean_total_result[6] << endl;
  cout << mean_total_result[0] << " " << mean_total_result[3] << endl;
  cout << mean_total_result[1] << " " << mean_total_result[4] << endl;
  cout << mean_total_result[2] << " " << mean_total_result[5] << endl;
}

//Running the program
int main() {
  srand(time(NULL));
  vector<brain> data;
  data.resize(150);
  InputData(data);
  Shuffle(data);
  K_fold_Validation(data);
  return 0;
}

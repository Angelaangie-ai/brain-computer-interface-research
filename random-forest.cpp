#include <cstdlib>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>
#include <iomanip>
 
using namespace std;
struct iris
{
   double value[2];
   string species;
};
struct node
{
   int feature; //0~3
   bool isLeaf;
   string species;
   double gap_value;
   struct node* higher;
   struct node* lower;
   vector<iris> data;
};

void InputData(vector<iris> &data)
{
   ifstream input;
   input.open("bezdekIris.txt");
   string buffer, buffer2;
   for (int i = 0; i < 150; i++)
   {
       input >> buffer;
       buffer2 = buffer2.assign(buffer, 0, 10);
       data[i].value[0] = stod(buffer2);
       buffer2 = buffer2.assign(buffer, 12, 12);
       data[i].value[1] = stod(buffer2);
       buffer2 = buffer2.assign(buffer, 24, 4);
       data[i].species = buffer2;
   }
}
double CalEntropy(int numLeft, int numRight, int datalength)
{
   double result = 0;
   if (datalength == 0)
   {
       return 0;
   }
   if (numLeft != 0)
   {
       result += -(numLeft / (double)datalength) * (log(numLeft / (double)datalength) / log(2));
   }
   if (numRight != 0)
   {
       result += -(numRight / (double)datalength) * (log(numRight / (double)datalength) / log(2));
   }
   return result;
 
}
double CalculateGain(vector<iris>data, vector<iris> higher, vector<iris> lower, int feature) // Gain = entropy - remainder
{
   int num_Left, num_Right;
   double data_Left_percent, data_Right_percent;
   double higher_Left_percent, higher_Right_percent;
   double lower_Left_percent, lower_Right_percent;
   double remainder;
   double data_entropy, higher_entropy, lower_entropy;
   num_Left = num_Right = 0;
   for (int i = 0; i < data.size(); i++)
   {
       if (data[i].species == "Left")
       {
           num_Left++;
       }
       else if(data[i].species == "Right")
       {
           num_Right++;
       }
       else cout << "error." << endl;
   }
   data_Left_percent = num_Left / (double)data.size();
   data_Right_percent = num_Right / (double)data.size();
   data_entropy = 0;
   if (num_Left > 0)
   {
       data_entropy += -(data_Left_percent * log2(data_Left_percent));
   }
   if (num_Right > 0)
   {
       data_entropy += -(data_Right_percent * log2(data_Right_percent));
   }
   //complete
   //cout << data_entropy << endl;
   num_Left = num_Right = 0;
   for (int i = 0; i < higher.size(); i++)
   {
       if (higher[i].species == "Left")
       {
           num_Left++;
       }
       else if (higher[i].species == "Right")
       {
           num_Right++;
       }
       else cout << "error." << endl;
   }
   higher_Left_percent = num_Left / (double)higher.size();
   higher_Right_percent = num_Right / (double)higher.size();
   higher_entropy = 0;
   if (num_Left > 0)
   {
       higher_entropy += -(higher_Left_percent * log2(higher_Left_percent));
   }
   if (num_Right > 0)
   {
       higher_entropy += -(higher_Right_percent * log2(higher_Right_percent));
   }
   //complete
   //cout << higher_entropy << endl;
   num_Left = num_Right = 0;
   for (int i = 0; i < lower.size(); i++)
   {
       if (lower[i].species == "Left")
       {
           num_Left++;
       }
       else if (lower[i].species == "Right")
       {
           num_Right++;
       }
       else cout << "error." << endl;
   }
   lower_Left_percent = num_Left / (double)lower.size();
   lower_Right_percent = num_Right / (double)lower.size();
   lower_entropy = 0;
   if (num_Left > 0)
   {
       lower_entropy += -(lower_Left_percent * log2(lower_Left_percent));
   }
   if (num_Right > 0)
   {
       lower_entropy += -(lower_Right_percent * log2(lower_Right_percent));
   }
   //complete
   //cout << lower_entropy << endl;
   remainder = ((higher.size() / (double)data.size()) * higher_entropy) + ((lower.size() / (double)data.size()) * lower_entropy);
   return data_entropy - remainder;
}
void Shuffle(vector<iris> &data)
{
   vector<iris> temp;
   bool isPut[155] = {false};
   temp.assign(data.begin(), data.end());
   int buffer = 0;
   int location = 0;
   //system("pause");
   for (int i = 150; i > 0; i--)
   {
       buffer = rand() % i + 1;
       location = -1;
       for (int j = 0; j < buffer; j)
       {
           location++;
           if (isPut[location] == true)
           {
               continue;
           }
           j++;
       }
       //cout << location << " " << i - 1 << endl;
       data[location] = temp[i - 1];
       isPut[location] = true;
       //cout << "a" << endl;
   }
}
bool Sort_compare0(iris a, iris b)
{
   return (a.value[0] < b.value[0]);
}
bool Sort_compare1(iris a, iris b)
{
   return (a.value[1] < b.value[1]);
}
 
void Construct_tree(vector<iris> &data, struct node* &root)
{
  
   //cout << "new node created" << " " << data.size() << endl;
   root = new struct node;  //NULL to thing
   root->data.assign(data.begin(), data.end());  //put data into node
   root->isLeaf = true;
   root->species = data[0].species;
   for (int i = 1; i < data.size(); i++)
   {
       if (data[i].species != data[i - 1].species)
       {
           root->isLeaf = false;
           root->species = "";
           break;
       }
   }
   if (root->isLeaf == true)
   {
       //cout << "leaf" << " " << root->species << endl;
       return;
   }
   vector<int> species_change_num;
   double highest_gain = 0;
   double highest_gap[2];  // [feature][gap_location]
   vector<iris> lower_data, higher_data;
   for (int i = 0; i < 4; i++)
   {
       species_change_num.clear();
       switch (i)
       {
       case 0:
           sort(data.begin(), data.end(), Sort_compare0);
           break;
       case 1:
           sort(data.begin(), data.end(), Sort_compare1);
           break;
       }
       for (int j = 1; j < data.size(); j++)
       {
           if (data[j].species != data[j - 1].species)
           {
               species_change_num.push_back(j);
           }
       }
       for (int j = 0; j < species_change_num.size(); j++)
       {
           vector<iris> lower;
           vector<iris> higher;
           lower.clear();
           higher.clear();
           lower.assign(data.begin(), data.begin() + species_change_num[j]);
           higher.assign(data.begin() + species_change_num[j], data.end());
           double temp = CalculateGain(data, higher, lower, i);
           //cout << temp << endl;
           if (temp >= highest_gain)
           {
               highest_gain = temp;
               highest_gap[0] = i;
               highest_gap[1] = data[species_change_num[j]].value[i];
               lower_data.assign(lower.begin(), lower.end());
               higher_data.assign(higher.begin(), higher.end());
           }
       }
   }
   root->feature = highest_gap[0];
   //cout << "feature: " << root->feature << " highest gain: " << highest_gain << endl;
   root->gap_value = highest_gap[1];
   root->higher = NULL;
   root->lower = NULL;
   //cout <<" higher size "<< higher_data.size() << " lower size " << lower_data.size() << endl;
   Construct_tree(higher_data, root->higher);
   Construct_tree(lower_data, root->lower);
   return;
}
vector<double> Test_tree(vector<iris> &test_data, struct node* root)  //��Xaccuracy, precision, recall������
{
   string result[30][2];
   int correct = 0;
   for (int i = 0; i < 30; i++)
   {
       struct node* current;
       current = root;
       while (current->isLeaf == false)
       {
           if (test_data[i].value[current->feature] >= current->gap_value)
           {
               current = current->higher;
               //cout << current->feature << " higher" << endl;
           }
           else
           {
               current = current->lower;
               //cout << current->feature << " lower" << endl;
           }
       }
       if (current->species == test_data[i].species)
       {
           correct++;
       }
       result[i][0] = current->species;
       result[i][1] = test_data[i].species;
       //cout << result[i][0] << " " << result[i][1] << endl;
   }
   double accuracy = correct / (double)30;
   int TP[3] = { 0 }, TN[3] = { 0 }, FP[3] = { 0 }, FN[3] = { 0 }; //0:setosa 1:versicolor 2:virginica
   for (int i = 0; i < 30; i++)
   {
       for (int j = 0; j < 3; j++)
       {
           switch (j)
           {
           case 0:
               if (result[i][0] == "Left" && result[i][1] == "Left")
               {
                   TP[j]++;
               }
               else if (result[i][0] == "Left" && result[i][1] != "Left")
               {
                   FP[j]++;
               }
               else if (result[i][0] != "Left" && result[i][1] == "Left")
               {
                   FN[j]++;
               }
               else
               {
                   TN[j]++;
               }
               break;
           case 1:
               if (result[i][0] == "Right" && result[i][1] == "Right")
               {
                   TP[j]++;
               }
               else if (result[i][0] == "Right" && result[i][1] != "Right")
               {
                   FP[j]++;
               }
               else if (result[i][0] != "Right" && result[i][1] == "Right")
               {
                   FN[j]++;
               }
               else
               {
                   TN[j]++;
               }
               break;
          
               break;
           }
       }
   }
   double left_precision = TP[0] + FP[0] > 0 ? TP[0] / (double)(TP[0] + FP[0]) : 0; ////////////////////////todo: nan(ind)
   double right_precision = TP[1] + FP[1] > 0 ? TP[1] / (double)(TP[1] + FP[1]) : 0;
   double left_recall = TP[0] + FN[0] > 0 ? TP[0] / (double)(TP[0] + FN[0]) : 0;
   double right_recall = TP[1] + FN[1] > 0 ? TP[1] / (double)(TP[1] + FN[1]) : 0;
   vector<double> stat;
   stat.push_back(left_precision);
   stat.push_back(right_precision);
   stat.push_back(left_recall);
   stat.push_back(right_recall);
   stat.push_back(accuracy);
   return stat;
}
void K_fold_Vaidation(vector<iris> &data)
{
   vector<double> result;
   vector<double> mean_total_result;
   vector<iris> train_data, test_data, temp, temp2;
   mean_total_result.resize(7);
   for (int i = 0; i < 7; i++)
   {
       mean_total_result[i] = 0;
   }
   for (int i = 0; i < 5; i++)
   {
       train_data.clear();
       test_data.clear();
       test_data.assign(data.begin() + i * 30, data.begin() + (i + 1) * 30);
       temp.assign(data.begin(), data.begin() + i * 30);
       temp2.assign(data.begin() + (i + 1) * 30, data.begin() + 150);
       train_data.insert(train_data.end(), temp.begin(), temp.end());
       train_data.insert(train_data.end(), temp2.begin(), temp2.end());
       struct node* root = NULL;  //(point to NULL)
       Construct_tree(train_data, root);
       result = Test_tree(test_data, root);
       for (int i = 0; i < 7; i++)
       {
           mean_total_result[i] += result[i];
       }
   }
   for (int i = 0; i < 7; i++)
   {
       mean_total_result[i] = mean_total_result[i] / 5.0;
   }
   cout << mean_total_result[6] << endl;
   cout << mean_total_result[0] << " " << mean_total_result[3] << endl;
   cout << mean_total_result[1] << " " << mean_total_result[4] << endl;
   cout << mean_total_result[2] << " " << mean_total_result[5] << endl;
}
int main()
{
   srand(time(NULL));
   vector<iris> data;
   data.resize(150);
   InputData(data);
   Shuffle(data);
   K_fold_Vaidation(data);
   //system("pause");
   return 0;
}

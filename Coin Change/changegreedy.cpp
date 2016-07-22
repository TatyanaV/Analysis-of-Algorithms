//cs325_SU_Project2
//Implementation of the greedy algorithm
//This program needs  the input file name as a run time argument
//Nicholas Madani
//Tatyana Vlaskin
//Avant Mathur

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::ofstream;

/*********************************************************************
2. Greedy  Coin Change Algorithm:
Another approach to coin change problem is the greedy approach. This implementation is called changegreedy.
This is also “naive” since it may not be optimal.
• Use the largest value coin possible.
• Subtract the value of this coin from the amount of change to be made.
• Repeat.

denominations in increasing order.
*********************************************************************/
void greedy(const vector<int> &array_Available_Coins, int change, ofstream &output);

int main(int argc, char* argv[]) {
    string input_file;
    string output_file;
    string line;
    vector <int> array_Available_Coins;
    //User runs the programs on the command-line, specifying a file ([input filename].txt)
    //in which the first line contains the array V, formatted as [1, 5, 10, 25], denominations in increasing order.
    if (argc == 2){ // the user should specify 2 parameters to run th program
        input_file = string(argv[1]);//name of the input file
        //output file-----[input filename]change.txt where [input filename].txt is the input filename
        output_file = string(argv[1]);
        output_file.insert(input_file.find(".txt"),"change");
    }
    else{
        cout << "No file specified." << endl;
        exit (EXIT_FAILURE);
    }
    //tokanize the input
    ifstream input(input_file.c_str());
    ofstream output(output_file.c_str());
      int counter = 0;
  while (getline(input, line)){
       ++counter;
    //get array of coins
   // getline(input, line);
    stringstream ss(line);
    int entry;
    int change;
    if (ss.peek() == '[')
        ss.ignore();
        if (counter % 2==1){
            while (ss >> entry){
                array_Available_Coins.push_back(entry);
                if (ss.peek() == ',')
                    ss.ignore();
            }

        }
        else{
            ss >> change;
            output << "Greedy Algorithm:" << endl;
            greedy(array_Available_Coins, change, output);
            //Clear out previous values from the vector
            array_Available_Coins.clear();
        }
    }
    output.close();
    cout << "Results printed to " << output_file << "." << endl;

    return 0;
}

/*********************************************************************
2. Greedy Algorithm:
Another approach to coin change problem is the greedy approach. This implementation is called changegreedy.
This is also “naive” since it may not be optimal.
• Use the largest value coin possible.
• Subtract the value of this coin from the amount of change to be made.
• Repeat.

denominations in increasing order.
*********************************************************************/

void greedy(const vector<int> &array_Available_Coins, int change, ofstream &output){
    vector<int> array_Coin_Count;
     //copy vector of avaibale coins to a new vector that will go to the output file
    //and assign each element to 0 -- meaning 0 # of coins used at the begining
    for (int i = 0; i < (int)array_Available_Coins.size(); i++)
        array_Coin_Count.push_back(0);
    //look at each coin in the avaibale coins array -- coins are in the denominations in increasing order
    //so to look at the largest coin we start at the end of the vectore
    for (int i = (int)array_Available_Coins.size() - 1; i >= 0; i--){
        //compare a coin to the change
        //if coin is equal or less than change, subtract coin value from the change
        while (array_Available_Coins[i] <= change){
            change -= array_Available_Coins[i];
            //increment coin count
            array_Coin_Count[i]++;
        }
    }
    //count total number of coins needed to give a change
    int total_Coin_Count =0;
    for (int k = 0; k < (int)array_Coin_Count.size(); k++){
        total_Coin_Count = total_Coin_Count + array_Coin_Count[k];
    }
    //print result to the output file
    //possibly we can place this code into a function because it will be reused
   // output << "Greedy Algorithm:" << endl;
    output << "[";
    for (int i = 0; i < (int)array_Coin_Count.size(); i++){
        output << array_Coin_Count[i];
        if (i != (int)array_Coin_Count.size() - 1)
            output << ", ";
    }
    output << "]" << endl;
    output <<endl<<endl;
    output<< total_Coin_Count<<endl<<endl;
}


//cs325_SU_Project2
//Implementation of the dynamic algorithm
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
#include <climits>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::ofstream;

//coin change dynamic algorithm
void changedp(const vector<int> &array_Available_Coins, int change, ofstream &output);


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
            output << "Dynamic Algorithm:" << endl;

           // greedy(array_Available_Coins, change, output);
            changedp(array_Available_Coins, change, output);
            //Clear out previous values from the vector
            array_Available_Coins.clear();
        }
    }
    output.close();
    cout << "Results printed to " << output_file << "." << endl;

    return 0;
}



/*********************************************************************
3. Dynamic Programming:
One dynamic programming approach uses table T indexed by values of change
0, 1, 2, . . . , A where T [v] is the minimum number of coins needed to make change for v.
𝑇[𝑣]= min𝑉[𝑖}≤𝑣{𝑇[𝑣−𝑉[𝑖]]+1}
We initialize T [0] = 0. How do you store and return the number of each type of coin to return?
 (That is, how do you build C[i]?) This implementation is called changedp. Note: there are other
 versions of the DP algorithm you may use one but need to explain in your report.
*********************************************************************/

void changedp(const vector<int> &array_Available_Coins, int change, ofstream &output){
    vector<int> array_Coin_Count;
    vector<int> array_Minimum_Coins;
    vector<int> optimal_Set_of_Coins;
    int min;
    int coin;
    //initialize dp table and coins as zeros
    array_Minimum_Coins.push_back(0);
    optimal_Set_of_Coins.push_back(0);

    //copy vector of avaibale coins to a new vector that will go to the output file
    //and assign each element to 0 -- meaning 0 # of coins used at the begining
    for (int i = 0; i < (int)array_Available_Coins.size(); i++)
        array_Coin_Count.push_back(0);

    //construct dynamic programming table-- THIS IS AN ANSWER TO QUESTION #1 ON THE REPORT
    /**********************************************************
    //https://www.youtube.com/watch?v=18NVyOI_690https://www.youtube.com/watch?v=Y0ZqKpToTic
    First, the dynamic programming (dp) table is set to 0 at change amount 0 (index 0).
    For each possible amount of change from 1 to the given amount of change (change), iterate over the array of coin values.
    If the value at each point in the coin values array (array_Available_Coins) is less than the selected amount of change,
    AND if the value in the dp table at the index of the value of the amount of change minus the current coin value is less than the minimum value,
    then the minimum  value is set to the value at that index in the dp table plus 1.
    After all coin values are iterated over for each change value, the final minimum value is pushed into the dp table.
    ***************************************************************************************************/
    //for each possible amount of change from 1 to the given amount of change
    for (int i = 1; i <= change; i++){
        min = INT_MAX;
        //iterate over the arrays of coin values
        for(int j = 0; j < (int)array_Available_Coins.size(); j++){
            //if the value of the coin value is less than or equal to the selected amount of change
            if (array_Available_Coins[j] <= i){
                //and if the value in the dp table minus the coin value is less thatn the minimum value
                if(array_Minimum_Coins[i-array_Available_Coins[j]] + 1 < min){
                    //set the minimum value to the lower value +1
                    min = array_Minimum_Coins[i-array_Available_Coins[j]] + 1;
                    coin = j;
                }
            }
        }
        array_Minimum_Coins.push_back(min);
        optimal_Set_of_Coins.push_back(coin);
    }

    //fill coin array array_Coin_Count with coin values
    while (change > 0){
        //increments coin position for current change value
        array_Coin_Count[optimal_Set_of_Coins[change]]++;
        //decrease change by the coin value
        change -= array_Available_Coins[optimal_Set_of_Coins[change]];
    }

    //count total number of coins needed to give a change
    int total_Coin_Count =0;
    for (int k = 0; k < (int)array_Coin_Count.size(); k++){
        total_Coin_Count = total_Coin_Count + array_Coin_Count[k];
    }
    //print result to the output file
    //possibly we can place this code into a function because it will be reused
    //output << "Dynamic Algorithm:" << endl;
    output << "[";
    for (int i = 0; i < (int)array_Coin_Count.size(); i++){
        output << array_Coin_Count[i];
        if (i != (int)array_Coin_Count.size() - 1)
            output << ", ";
    }

    output << "]" << endl;
    output <<endl<<endl;
    output<< total_Coin_Count<<endl<<endl;
    array_Minimum_Coins.clear();
    optimal_Set_of_Coins.clear();
    array_Coin_Count.clear();
}


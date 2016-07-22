//cs325_SU_Project2
//Implementation of the Algorithm1 (Brute force method)
//This program needs  the input file name as a run time argument
//Nicholas Madani
//Tatyana Vlaskin
//Avant Mathur

#include <iostream>
#include <istream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <chrono>

using namespace std;

vector<int> compare_change(vector<int> &coins, vector<int> &c, int x);
vector<int> changeslow(vector<int> &coins, vector<int> &c, int x, int i, int SUM);  //Recursive function that calculates the minimum number of coins
bool front_Bracket(string str), end_Bracket(string str);
int check(string str);
//*********************************************************************
// Function: compare_change
// Input   : denomination of coins, a vector to store the results and
//           the total value to make change from.
// Output  : vector cointianing the minimum number of coins needed, per denomination.
// ********************************************************************
vector<int> compare_change(vector<int> &coins, vector<int> &c, int x)
{
    int numberofCoins = 0;
    int SUM = 0;
    int k = 0;
    int n = 0;
    int ii = 1;
    int g = 0;
    vector<int> CALCULATE_1, CALCULATE_2, CALCULATE_1x, CALCULATE_2y, SUM_S;

    CALCULATE_1.resize(coins.size());
    CALCULATE_2.resize(coins.size()); //Resizing the vectors
    CALCULATE_1x.resize(coins.size());
    CALCULATE_2y.resize(coins.size());
    SUM_S.resize(x);

    for(int w=1; w<x; ++w)
    {
        n = 0;
	while(n < CALCULATE_1.size())
        {
            numberofCoins = 0;
            CALCULATE_1x.at(n) = 0;
            CALCULATE_2y.at(n) = 0;
            ++n;
        }
        CALCULATE_1 = changeslow(coins, CALCULATE_1x, w, w, SUM);
        CALCULATE_2 = changeslow(coins, CALCULATE_2y, x-w, x-w, SUM);
	k = 0;
        while(k < coins.size())
        {
          numberofCoins += CALCULATE_1.at(k) + CALCULATE_2.at(k);
          ++k;
        }
        SUM_S.at(w) = numberofCoins;
    }

    int Minimum = SUM_S.at(1);
    int Minimum_Idx = 0;

    ii = 1;
    while (ii < SUM_S.size())
    {

        if(SUM_S.at(ii) < Minimum)
        {
            Minimum=SUM_S.at(ii);
            Minimum_Idx = ii;
        }
      ++ii;
    }

    for(int nn=0; nn<CALCULATE_1.size(); ++nn)
    {

        numberofCoins = 0;
        CALCULATE_1x.at(nn) = 0;
        CALCULATE_2y.at(nn) = 0;
    }

    CALCULATE_1 = changeslow(coins, CALCULATE_1x, Minimum_Idx, Minimum_Idx, SUM);
    CALCULATE_2 = changeslow(coins, CALCULATE_2y, x-Minimum_Idx, x-Minimum_Idx, SUM);
    vector<int> Minimum_Outputs;
    Minimum_Outputs.resize(CALCULATE_1.size());

    while (g<Minimum_Outputs.size())
    {

        Minimum_Outputs.at(g) = CALCULATE_1.at(g) + CALCULATE_2.at(g);
        ++g;
    }

    return Minimum_Outputs;

}
//*********************************************************************
// Function   : changeslow
// Input      : denomination of coins, a vector to store the results and
//              the total value to make change from.
// Output     : vector containing the minimum number of coins per denomination
//              needed to make change for the total value.
// ********************************************************************
vector<int> changeslow(vector<int> & coins, vector<int> &c, int x, int i, int SUM)
{
    int Tmp = 0;
    int Final = i;
    int Equal = 0;

    if(Final == 0){return c;}

    for(int w=0; w<coins.size(); ++w)
    {
        if(coins.at(w) == i)
	{
            ++c.at(w);
            SUM += coins.at(w);
            Tmp = x-SUM;
            Equal = 1;
            break;
        }
    }

    if(Equal == 1)
    {
        if(SUM == x)
        {
            return c;
        }
        else
        {
            return changeslow(coins, c, x, Tmp, SUM);// recursive call
        }
    }
    else
    {
        return changeslow(coins, c, x, Final-1, SUM);// recursive call
    }
}
//*****************************************************************************
//Function : front_bracket
//Input    : A string read from input file
//Output   : True if '[' is found else false
//*****************************************************************************

bool front_bracket(string str)
{
    return str[0] == '[';
}
//*****************************************************************************
//Function : end_bracket
//input    : A string read from input file
//Output   : True if ']' is found else false
//*****************************************************************************
bool end_bracket(string str)
{
    int length = str.length();
    for(int i=0; i<length; ++i)
    {
      if(str[i] == ']')
      {
        return true;
      }
    }
    return false;
}
//*****************************************************************************
//Function  : check
//Input     : a string read from the input file
//output    : revised string after removing [ ] and ,
//*****************************************************************************
int check(string str)
{
    int j = 0;
    int length = str.length();
    while (j < length)
    {
        //loop changes bracket and comma into spaces
        if(str[j] == ',') {str[j] = ' ';}
        else if(str[j] == '[') {str[j] = ' ';}
        else if(str[j] == ']') {str[j] = ' ';}
        ++j;
    }
    return atoi(str.c_str());
}
//*****************************************************************************
//Function  : main
//Input     : textfile as an argument while running the program.
//Output    : modified text file containing the number of each coins and minimum
//            number of coins.
//*****************************************************************************
int main(int argc, char* argv[])
{
    int y = 0;
    char File[100];
    int jj = 0;
    strcpy(File, argv[1]);
    string File_Name(File);
    string f_name;
    y = 0;
    string output_file = "slowchangep6_results.txt";
    clock_t begin;
    double duration;
    while(y<File_Name.length())
    {
        if(File_Name[y] != '.')
	{
            f_name.push_back(File_Name[y]);

        }
        else {break;}
        ++y;
    }
    ifstream myfile(File_Name);
    f_name.append("change.txt");
    ofstream write_into_file;
    write_into_file.open(f_name, ios::app);
    write_into_file << "Slow Change Algorithm:" << endl;
    ofstream question_output_file;
    question_output_file.open(output_file, ios::app);


    string in_put;
    vector<int> values;

    while(!myfile.eof())
    {
        values.clear();
        //parsing the denomination from the string read from file
        do
        {
            myfile >> in_put;
        }
        while(!myfile.fail() && !front_bracket(in_put));

	if(!myfile.fail() && front_bracket(in_put))
        {
            values.push_back(check(in_put));
        }
        myfile >> in_put;

        while(!myfile.fail() && !end_bracket(in_put))
        {
            values.push_back(check(in_put));
            myfile >> in_put;
        }
        if (!myfile.fail())
        {
            values.push_back(check(in_put));
        }

        myfile >> in_put; // reading the amount for the change required
        int T_value = atoi(in_put.c_str());
        if(!(values.size() == 0))
        {

            int x = T_value;
            int numberofCoins = values.size();
	    int print = 0;

            vector<int> coins;
            vector<int> calculate;
            vector<int> ReSults;

            coins.resize(numberofCoins);
            calculate.resize(numberofCoins);
            ReSults.resize(numberofCoins);

	    for(int k=0; k<values.size(); ++k)
            {
                coins.at(k) = values.at(k);
            }

            if(x <= 0)
            {
        	cout << "Nope" << endl;
            }
            else if(x == 1)
            {

                ReSults.at(0) = 1;
                print = 1;
            }
	    else
            {
                using namespace std::chrono;
                jj = 10;
                begin = clock();
                //Calling function compare_change to determine the minimum number of coins as change
                //Function compare_change calls function changeslow as recursive function.
		ReSults = compare_change(coins, calculate, x);
		duration = (clock() - begin) / (double) CLOCKS_PER_SEC / jj;
                print = 1;
            }
            if(print == 1)
            {
                int SUM = 0;
                int numberofCoins = 0;
                int iw = 0;
                write_into_file << '[';

                iw = 0;
                while (iw < ReSults.size()-1)
		{

                    write_into_file << ReSults.at(iw) << ", ";
		    numberofCoins += ReSults.at(iw);
                    ++iw;
                }
                numberofCoins += ReSults.at(ReSults.size()-1);
                write_into_file << ReSults.at(ReSults.size()-1) << "]" << endl;
                write_into_file << numberofCoins << endl;
                question_output_file << T_value << "," << numberofCoins << endl;
            }
        }
    }

    return 0;
}
    //Reference for using atoi
    //http://www.cplusplus.com/reference/cstdlib/atoi/
    //refereneced Github-StabbyMcDuck for parsing the input

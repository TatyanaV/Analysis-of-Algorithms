/*

  This program calculates the maximum subarray and the maximum sum.
  The output will be located in the current directory in MSS_Results.txt.
  It uses the input file as command line argument argv[1].

  for example:

           me@linuxbox:~$ ./main MSS_Problems.txt

  program must be compiled with the g++ -g -std=c++0x project_1.cpp -o main


*/



#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <climits>





/*
  Divide and conquer algorithm functions (uses struct to hold sum and sub array indicies)
  calculates the maximum sum and maximum subarray

  input: pointer to vector, size of the vector, start index, end index, vector midpoint,
         struct result a, struct result b, struct result c

  output: struct result


*/

struct result _divideAndConquer(std::vector<int>& arr, int sizeArr);
struct result maxSumSubArray(std::vector<int>& arr, int sizeArr, int start, int end);
struct result maxSumOverMidPoint(std::vector<int>& arr, int sizeArr, int start, int end, int m);
struct result max(struct result a, struct result b, struct result c);
void divide_and_conquer(std::vector<int>& arr, std::vector<int>& max_subarr, int& max);


struct result {
     int sum;
     int from;
     int to;
};

/*
  Linear time algorithm function -
  void function that calculates maximum sum and the maximum subarray

  input: vector reference, maximum sub-vector reference, max reference
  output: none

*/

void linear_time(std::vector<int>& arr, std::vector<int>& max_subarr, int& max);


/* Enumeration algorithm

   input: array reference, sub array reference, max reference
   output: none
*/

void _enumeration(std::vector<int>& array, std::vector<int>& max_sub, int& max);


/* Better Enumeration Algorithm
   input: array reference, sub array reference, max reference
   output: none
*/

void _better_enumeration(std::vector<int>& array, std::vector<int>& max_sub, int& max);

/*
  utility function -
  void function that writes results to output file

  input: output stream reference, maximum sub array reference
  output: none

*/

void write_to_file(std::ofstream& out_stream, std::vector<int>& max_sub, int& max);

/* function that handles the enumeration algorithm, opening up input file, and writing to
    output file
*/




void enumeration(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream);

/* function that handles the better enumeration algorith, opening up input file, and writing to
    output file
*/

void better_enumeration(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream);

/*
    function that handles linear algorithm, opening up input file, and writing to
    output file
*/

void linear(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream);

/* function that handleas divide and conquer algorithm, and opening up input file,
   and writing to output file.
*/

void d_and_q(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream);



int main(int argc, char* argv[])
{
  std::vector<int> array;
  std::vector<int> max_sub;
  int max = 0;

  std::ifstream in_stream;
  std::ofstream out_stream;

  char token;                   // used to hold character during parsing of input file
  std::string number;
  std::stringstream str_stream; // used to convert strings into integers
  str_stream.clear();
  str_stream.str("");

  enumeration(in_stream, out_stream, argv[1], array, max_sub, max, token, number, str_stream);
  better_enumeration(in_stream, out_stream, argv[1], array, max_sub, max, token, number, str_stream);
  linear(in_stream, out_stream, argv[1], array, max_sub, max, token, number, str_stream);
  d_and_q(in_stream, out_stream, argv[1], array, max_sub, max, token, number, str_stream);

  return 0;
}




void linear_time(std::vector<int>& arr, std::vector<int>& max_subarr, int& max)
{
  int curr_max = 0;
  int max_first_index = 0;
  int max_last_index = -1;
  int first_index = 0;

// calculate maximum sub array and maximum
  for (int i = 0; i < arr.size(); ++i) {
    curr_max = curr_max + arr.at(i);
    if (curr_max < 0) {
      curr_max = 0;
      first_index = i + 1;
    }
    if (max < curr_max) {
      max = curr_max;
      max_first_index = first_index;
      max_last_index = i;
    }
  }

  if (max_first_index <= max_last_index) {
  // copy max sub array into max_subarr vector
    while (max_first_index != max_last_index + 1) {
      max_subarr.push_back(arr[max_first_index]);
      ++max_first_index;
    }
  }

}

struct result _divideAndConquer(std::vector<int>& arr, int sizeArr) {

     struct result r;
     r = maxSumSubArray(arr, sizeArr, 0, sizeArr - 1);

     return r;

}

struct result maxSumSubArray(std::vector<int>& arr, int sizeArr, int start, int end) {

     struct result r;

     // midpoint
     int m = start + ((end - start) / 2);

     // only 1 value to check
     if ((end - start) == 0) {
          r.sum = arr[start];
          r.from = start;
          r.to = start;
          return r;
     }

     // return max of sumofsubarray from 0 to midpoint, midpoint+1 to end and suffix of first half and prefix of second half
     return max(maxSumSubArray(arr, sizeArr, start, m),
          maxSumSubArray(arr, sizeArr, m+1, end),
          maxSumOverMidPoint(arr, sizeArr, start, end, m)
          );
}

struct result maxSumOverMidPoint(std::vector<int>& arr, int sizeArr, int start, int end, int m) {

     struct result r;

     int maxSumLeft = INT_MIN;
     int cumSumLeft = 0;
     int maxSumRight = INT_MIN;
     int cumSumRight = 0;

     // left side
     for (int i = m; i >= start; i--)
     {
          cumSumLeft = cumSumLeft + arr[i];
          if (cumSumLeft > maxSumLeft)
          {
               maxSumLeft = cumSumLeft;
               r.from = i;
          }
     }

     // right side
     for (int i = m + 1; i <= end; i++)
     {
          cumSumRight = cumSumRight + arr[i];
          if (cumSumRight > maxSumRight)
          {
               maxSumRight = cumSumRight;
               r.to = i;
          }
     }

     r.sum = maxSumLeft + maxSumRight;

     return r;

}

struct result max(struct result a, struct result b, struct result c) {

     if (a.sum > b.sum) {
          if (a.sum > c.sum)
               return a;
     }
     else
     {
          if (b.sum > c.sum)
               return b;
     }
     return c;
}

void divide_and_conquer(std::vector<int>& arr, std::vector<int>& max_subarr, int& max)
{

          struct result maxSum;
          maxSum = _divideAndConquer(arr, arr.size());

          for (int i = maxSum.from; i <= maxSum.to; i++) {
            max_subarr.push_back(arr[i]);
          }

          max = maxSum.sum;
}

void _enumeration(std::vector<int>& array, std::vector<int>& max_sub, int& max)
{

  int max_first_idx = 0;
  int max_last_idx = 0;

  for (int i = 0; i < array.size(); ++i) {
    for (int j = i; j < array.size(); ++j) {
      int curr_max = 0;
      for (int k = i; k <= j; ++k) {
        curr_max += array.at(k);
        if (curr_max > max) {
          max = curr_max;
          max_first_idx = i;
          max_last_idx = j;
        }
      }
    }
  }

  if (max_first_idx <= max_last_idx) {
    while (max_first_idx != max_last_idx + 1) {
      max_sub.push_back(array.at(max_first_idx));
      ++max_first_idx;
    }
  }
}

void _better_enumeration(std::vector<int>& array, std::vector<int>& max_sub, int& max)
{
   	int maximum = 0;
	int current = 0;
	int bestupper = 0;
	int bestlower=0;
	for (int i = 0; i < array.size(); i++) {
		current = 0;
		for (int j = i; j < array.size(); j++) {
			current += array.at(j);
			if (current > maximum) {
				maximum = current;
                bestupper=j;
				bestlower=i;
			}
		}
	}

        if (bestlower <= bestupper) {
          while (bestlower != bestupper + 1) {
            max_sub.push_back(array.at(bestlower));
            ++bestlower;
          }
        }
        max = maximum;
}

void write_to_file(std::ofstream& out_stream, std::vector<int>& max_sub, int& max)
{
  out_stream.open("MSS_Results.txt", std::ios::app);          // append output file w/ results
  for (int i = 0; i < max_sub.size(); ++i) {
    if (i == 0)
      out_stream << "[" << max_sub.at(i);
    else
      out_stream << ", " << max_sub.at(i);
  }
  out_stream << "], max sum = " << max << std::endl;
  out_stream.close();
}

void enumeration(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream)
{
  in_stream.open(input_file_name);
  in_stream.get(token);                              // get first token in input file
  out_stream.open("MSS_Results.txt");                 // create output txt file
  out_stream << "Enumeration Algorithm" << std::endl << std::endl;
  out_stream.close();
  while (!in_stream.eof()) {
    if (token == '-') {
      str_stream << token;                           // if it's a negative sign add to string stream
    } else if (std::isdigit(token)) {
        str_stream << token;
    } else if (token == ',') {                       // if token is a comma, end of #, i.e. 27,
        number = str_stream.str();
        array.push_back(std::stoi(number));          // convert string stream to integer, add to array
        str_stream.clear();
        str_stream.str("");
    } else if (token == ']') {                     // if right bracket, end of array
        number = str_stream.str();
        array.push_back(std::stoi(number));        // add last number to array
        str_stream.clear();
        str_stream.str("");
        _enumeration(array, max_sub, max);
        write_to_file(out_stream, max_sub, max);
        array.clear();
        max_sub.clear();
        max = 0;
    }
    in_stream.get(token);                                         // get next token
  }
  in_stream.close();
}

void better_enumeration(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream)
{
  in_stream.open(input_file_name);
  in_stream.get(token);                              // get first token in input file
  out_stream.open("MSS_Results.txt", std::ios::app);                 // create output txt file
  out_stream << std::endl << "Better Enumeration Algorithm" << std::endl << std::endl;
  out_stream.close();
  while (!in_stream.eof()) {
    if (token == '-') {
      str_stream << token;                           // if it's a negative sign add to string stream
    } else if (std::isdigit(token)) {
        str_stream << token;
    } else if (token == ',') {                       // if token is a comma, end of #, i.e. 27,
        number = str_stream.str();
        array.push_back(std::stoi(number));          // convert string stream to integer, add to array
        str_stream.clear();
        str_stream.str("");
    } else if (token == ']') {                     // if right bracket, end of array
        number = str_stream.str();
        array.push_back(std::stoi(number));        // add last number to array
        str_stream.clear();
        str_stream.str("");
        _better_enumeration(array, max_sub, max);
        write_to_file(out_stream, max_sub, max);
        array.clear();
        max_sub.clear();
        max = 0;
    }
    in_stream.get(token);                                         // get next token
  }
  in_stream.close();
}


void linear(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream)
{
  in_stream.open(input_file_name);
  in_stream.get(token);                              // get first token in input file
  out_stream.open("MSS_Results.txt", std::ios::app);                 // create output txt file
  out_stream << std::endl << "Linear Algorithm" << std::endl << std::endl;
  out_stream.close();
  while (!in_stream.eof()) {
    if (token == '-') {
      str_stream << token;                           // if it's a negative sign add to string stream
    } else if (std::isdigit(token)) {
        str_stream << token;
    } else if (token == ',') {                       // if token is a comma, end of #, i.e. 27,
        number = str_stream.str();
        array.push_back(std::stoi(number));          // convert string stream to integer, add to array
        str_stream.clear();
        str_stream.str("");
    } else if (token == ']') {                     // if right bracket, end of array
        number = str_stream.str();
        array.push_back(std::stoi(number));        // add last number to array
        str_stream.clear();
        str_stream.str("");
        linear_time(array, max_sub, max);
        write_to_file(out_stream, max_sub, max);
        array.clear();
        max_sub.clear();
        max = 0;
    }
    in_stream.get(token);                                         // get next token
  }
  in_stream.close();
}

void d_and_q(std::ifstream& in_stream, std::ofstream& out_stream, char* input_file_name,
            std::vector<int>& array, std::vector<int>& max_sub,
            int& max, char& token, std::string number, std::stringstream& str_stream)
{
  in_stream.open(input_file_name);
  in_stream.get(token);
  out_stream.open("MSS_Results.txt", std::ios::app);
  out_stream << std::endl << "Divide & Conquer Algorithm" << std::endl << std::endl;
  out_stream.close();

  while (!in_stream.eof()) {
    if (token == '-') {
      str_stream << token;
    } else if (std::isdigit(token)) {
        str_stream << token;
    } else if (token == ',') {
        number = str_stream.str();
        array.push_back(std::stoi(number));
        str_stream.clear();
        str_stream.str("");
    } else if (token == ']') {
        number = str_stream.str();
        array.push_back(std::stoi(number));
        str_stream.clear();
        str_stream.str("");
        divide_and_conquer(array, max_sub, max);
        write_to_file(out_stream, max_sub, max);
        array.clear();
        max_sub.clear();
        max = 0;
    }
    in_stream.get(token);
  }
  in_stream.close();
}

/****************************************************************
Group 13
Avant Mathur
Tatyana Vlaskin
Nicholas Madani
CS325 Project 4
Reference: See project report
To compile the program:
g++ -g -std=c++0x 4.cpp -o 4

To run the program:
./4 input_file

*****************************************************************/

#include <fstream>
#include <iostream>
#include <cmath>
#include <climits>
#include <csignal>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::reverse;
int best_distance = INT_MAX;

//struct for city data
//keeps track of the city identifier, x, y coordinates and whether the city was visited or not
typedef struct {
    int city_Identifier;
    int x_coordinate;
    int y_coordinate;
    int visit;
} city;

//signal handler for SIGTERM in case the program is terminated by the signal
//this useful to have when run time of the programs is long
void signal_handler(int signum);

//calculates the distance between cities
int distance(city &city1, city &city2);

//calculate TSP tour using Nearest Neighbor
void nearest_Neighbor(int start, int &tour_Calculated_Distance, string best_Tour_List, vector <city> &original_City_Order, vector <int> &optimal_City_Order);

//optimizes TSP tour using 2-opt
void two_optimization(int &tour_Calculated_Distance, string best_Tour_List, vector <city> &original_City_Order, vector <int> &optimal_City_Order);


int main(int argc, char* argv[]) {
    int start, end;
    clock_t begin;
    double duration; // keeps track of the run time
    string original_List; // input file
    string best_Tour_List; //output file
    int tour_Calculated_Distance; // keeps track of the total distance
    vector <city> original_City_Order;
    vector <int> optimal_City_Order;
    // register signal SIGTERM and signal handler
    //http://en.cppreference.com/w/cpp/utility/program/signal
    signal(SIGTERM, signal_handler);

    //parses the command line
    if (argc == 2){
        original_List = string(argv[1]); //file name
        best_Tour_List = string(argv[1]) + ".tour"; //the output file will have .tour appended at the end
    }
    //if the file name was not specified, give and error and exit
    else{
        cout << "No input file was specified." << endl;
        exit (EXIT_FAILURE);
    }

    ifstream input(original_List.c_str());
    string line;
    //city variable city type
    city cities_From_File;
    //reads the input file line by line and store information about each city in the city type variable
    while (getline(input, line)){
        //parse the line about each city and store information about city identifier, x and y coordinate
        stringstream ss(line);
        if(ss >> cities_From_File.city_Identifier >> cities_From_File.x_coordinate >> cities_From_File.y_coordinate){
            //set the visit status to 0, which indicates that the city was not visited
            cities_From_File.visit = 0;
            //store each city in the vector of cities
            original_City_Order.push_back(cities_From_File);
        }
    }
    input.close();
     begin = clock(); //start timer to keep track of the algorithm time
     //lets the user know how many cities will be in the tour
     cout << endl << "There are " << (int)original_City_Order.size() << " cities. Calculating the optimal tour. " << endl;

   //for each possible starting city from the file
    for (int i = 0; i < (int)original_City_Order.size(); i++){
        double duration1; // declare the variable to keep track of the time before entering each algorithm
        // the value of chosen to be 250 because for the competition cases, it was determined that if the time
        //is above 250 at the start of the iteration it will not be possible to get the optimal withing 5 minutes
        if (duration1 < 250){
            cout << duration1 << " sec" <<endl; // output time on the screen
            //reset tour values
            tour_Calculated_Distance = 0; //set the distance to 0
            optimal_City_Order.clear(); // clears the order of the cities
            //clears the list of the visited cities
            for (int j = 0; j < (int)original_City_Order.size(); j++){
                original_City_Order[j].visit = 0;
            }
            //runs the nearest neighbor algorithm
            nearest_Neighbor(i, tour_Calculated_Distance, best_Tour_List, original_City_Order, optimal_City_Order);
            //determines the time at the end of the nearest neibours algorithm
            duration1 = (clock() - begin) / (double) CLOCKS_PER_SEC; //end timer
         }
         //if the time at the end of the neibors algorithm is less than 250, we can optimize a solution by entering the two_optimization solution
        if (duration1 < 250){
            cout << duration1 << " sec" <<endl;
            //2-opt optimization of the tour that was derived from the nearest neighbor algorithm
            two_optimization(tour_Calculated_Distance, best_Tour_List, original_City_Order, optimal_City_Order);
            //determins the end time of the algorithm
            duration1 = (clock() - begin) / (double) CLOCKS_PER_SEC; //end timer
        }
    }
    //output information about the best tour
    cout << endl << "The best tour is: " << best_distance << "." << endl << endl;
    //calculates the total time from the beginning of the problem
    duration = (clock() - begin) / (double) CLOCKS_PER_SEC; //end timer
    //report the run time to the user
    cout << "Run time of the tour:" << duration<<endl;
    return 0;
}
//signal handler function in case the program is terminated by the signal
//usefull when the run time is really long
//taken from https://github.com/ethanspiro/traveling-salesman/blob/master/cs325_project4_tsp.cpp
void signal_handler(int signum){
    cout << "Termination signal (" << signum << ") received." <<endl;
    cout << "The best tour found is " << best_distance << "." << endl << endl;
    exit(signum);
}

//function to calculate the distance between 2 cities using distance formular
//function taken from https://github.com/CaiqueReinhold/STD_GA_TSP/blob/17004e42c85db3701546708936a0af66c399c471/GA/Cities.cpp
int distance(city &city1, city &city2){
    double x = pow(city2.x_coordinate - city1.x_coordinate, 2);
    double y = pow(city2.y_coordinate - city1.y_coordinate, 2);
    double combined = sqrt(x + y);
    return (int)round(combined);
}

//the nearest_Neighbor algorithm
void nearest_Neighbor(int start, int &tour_Calculated_Distance, string best_Tour_List, vector <city> &original_City_Order, vector <int> &optimal_City_Order){
    int test_Distance;
    int minimum_Distance;
    int current;
    int next;
    cout << endl << "Starting city: " << start << endl;
    //set the current city to the start city
    current = start;
    //pushes the start city into the oder vector
    optimal_City_Order.push_back(original_City_Order[start].city_Identifier);
    //flags the city as the visited ciry
    original_City_Order[start].visit = 1;
    //iterate through all of the neighbors
    for (int i = 0; i < (int)original_City_Order.size() - 1; i++){
        minimum_Distance = INT_MAX;
        //loops though the cities
        for (int j = 0; j < (int)original_City_Order.size(); j++){
            //if the city was not visited
            if (original_City_Order[j].visit == 0){
                //find the distance between the current city and one of the neighbors
                test_Distance = distance(original_City_Order[current], original_City_Order[j]);
                //if tested distance is less than minimum distance, it becomes new minimum distance
                if (test_Distance < minimum_Distance){
                    minimum_Distance = test_Distance;
                    //we move on the the next city
                    next = j;
                }
            }
        }
        //to find the total distance of the tour add the new minimum distance
        tour_Calculated_Distance += minimum_Distance;
        //push the closest neighbor to the optimal vector list
        optimal_City_Order.push_back(original_City_Order[next].city_Identifier);
        //flag the city as visited
        original_City_Order[next].visit = 1;
        //move on the the next city
        current = next;
    }
    //add distance from last city to starting city
    minimum_Distance = distance(original_City_Order[current], original_City_Order[start]);
    tour_Calculated_Distance += minimum_Distance;
     //writes the optimum tour to the output file
    if (tour_Calculated_Distance < best_distance){
       //send results to the output file
        ofstream output(best_Tour_List.c_str());
        //outputs the best tour distance
        output << tour_Calculated_Distance << endl;
        //outputs the optimal city order to the file
        for (int i = 0; i < (int)optimal_City_Order.size(); i++){
            output << optimal_City_Order[i] << endl;
        }
        output.close();
        //set best distance to the tour distance
        best_distance = tour_Calculated_Distance;
     } else {

    }
}
//algorithm to optimize the tour obtained from the nearest neibhor algorithm
void two_optimization(int &tour_Calculated_Distance, string best_Tour_List, vector <city> &original_City_Order, vector <int> &optimal_City_Order){
    int old_distance = INT_MAX;
     int iteration = 0; //variable that keeps track of different starting cities
    int swap_Distance; // variable for the distance when the cities are swapped
    int size = (int)optimal_City_Order.size();
    int distance_After_Swap; // variable for distance after swap
    int distance_Before_Swap; //variable for distance before swap
    //perform iterations of 2-opt swap
    while (old_distance != tour_Calculated_Distance){
        old_distance = tour_Calculated_Distance;
        for (int i = 0; i < size - 1; i++){
           for (int k = i + 1; k < size; k++){
                distance_After_Swap = 0;
                distance_Before_Swap = 0;
                //calculates distance before swap and distance in case there is a swap
                //taken from http://on-demand.gputechconf.com/gtc/2014/presentations/S4534-high-speed-2-opt-tsp-solver.pdf
                if (i > 0){
                    distance_After_Swap += distance(original_City_Order[optimal_City_Order[i - 1]], original_City_Order[optimal_City_Order[k]]);
                    distance_Before_Swap += distance(original_City_Order[optimal_City_Order[i - 1]], original_City_Order[optimal_City_Order[i]]);
                }
                if (k < size - 1){
                    distance_After_Swap += distance(original_City_Order[optimal_City_Order[i]], original_City_Order[optimal_City_Order[k + 1]]);
                    distance_Before_Swap += distance(original_City_Order[optimal_City_Order[k]], original_City_Order[optimal_City_Order[k + 1]]);
                }
                if ((i == 0) && (k != size - 1)){
                    distance_After_Swap += distance(original_City_Order[optimal_City_Order[size - 1]], original_City_Order[optimal_City_Order[k]]);
                    distance_Before_Swap += distance(original_City_Order[optimal_City_Order[size - 1]], original_City_Order[optimal_City_Order[0]]);
                }
                if ((k == size - 1) && (i != 0)){
                    distance_After_Swap += distance(original_City_Order[optimal_City_Order[i]], original_City_Order[optimal_City_Order[0]]);
                    distance_Before_Swap += distance(original_City_Order[optimal_City_Order[size - 1]], original_City_Order[optimal_City_Order[0]]);
                }
                //calcualte the swap distance. It works by subtracking the old distance and adding the new distance
                swap_Distance = tour_Calculated_Distance - distance_Before_Swap + distance_After_Swap;
                //compares new and old distances, if the new distance is smaller, than we need to perform a swap
                if (swap_Distance < tour_Calculated_Distance){
                    //sets the new distance
                    tour_Calculated_Distance = swap_Distance;
                    //the actual swap is performed
                    reverse(optimal_City_Order.begin() + i, optimal_City_Order.begin() + k + 1);
                }
            }
        }
        //print out results
        if (old_distance != tour_Calculated_Distance){
            //moves on to the next itetaion. Iteration is equivalent to the starting city
            iteration++;
            //writes the optimum tour to the output file
            if (tour_Calculated_Distance < best_distance){
                //send results to the output file
                ofstream output(best_Tour_List.c_str());
                //outputs the best tour distance in the output file
                output << tour_Calculated_Distance << endl;
                //outputs the best city order in the output file
                for (int i = 0; i < (int)optimal_City_Order.size(); i++){
                    output << optimal_City_Order[i] << endl;
                }
                output.close();
                best_distance = tour_Calculated_Distance;

            } else {

                }
        }
    }
}

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include "Graph.hpp"
using namespace std;

int main(int argc, char const *argv[]) {
  int cityNumber = 0, destNum = 0, readNumber = 0;
  vector<int> warehouses;
  vector<int> destinations;
  ifstream inFile;
  inFile.open(argv[1]);

  inFile >> cityNumber; //City number is read from file
  inFile >> destNum; //Destination number is read from file

  //Warehouses are read from file
  for(int i=0; i<2; i++) {
    inFile >> readNumber;
    warehouses.push_back(readNumber);
  }

  //Destinations are read from file
  for(int i=0; i<destNum; i++) {
    inFile >> readNumber;
    destinations.push_back(readNumber);
  }


  //Graph size of cityNumber is created
  Graph g(cityNumber);

  //Distances between cities are read from file
  for(int i=0; i<cityNumber; i++) {
    for(int j=0; j<cityNumber; j++) {
        inFile >> readNumber;
        if(readNumber == 0)
          continue;
        else
          g.create_add_edge(i, j, readNumber);
    }
  }
  inFile.close();
  g.shareDestinations(warehouses, destinations);

  return 0;
}

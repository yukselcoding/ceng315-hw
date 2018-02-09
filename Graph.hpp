#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
using namespace std;

//Keeps destination and weight of the adjacency list node
struct adjNode {
  int destination, weight;
  adjNode(int d=INT_MIN, int w=INT_MIN) : destination(d), weight(w) {}
};

//Comparator class for adjNode data type
class adjNodeComparator {
public:
  bool operator()(const adjNode &a, const adjNode &b) {
    return a.weight > b.weight;
  }
};

//To sort the adjacency node increasing order with respect to weight
class weightComparator {
public:
  bool operator()(const adjNode &a, const adjNode &b) {
    return a.weight < b.weight;
  }
};

//Adjacency list representation of the graph
class Graph {
  int v;
  vector<vector<adjNode>> g;
public:
  Graph(int v);
  void create_add_edge(int src, int dest, int weight);
  int absolute(int num);
  int maxIndex(vector<adjNode> &vec);
  vector<int> shortestPath(int src);
  vector<adjNode> FindDistanceDiff(vector<adjNode> &warehouse1, vector<adjNode> &warehouse2);
  void shareDestinations(vector<int> &warehouses, vector<int> &destinations);
};

//Graph constructor
Graph::Graph(int v) {
  this->v = v;
  g = vector<vector<adjNode>>(v);
}

//Finds the maximum distance in distance difference vector
int Graph::maxIndex(vector<adjNode> &vec) {
  int i = 0, j = 0;
  adjNode max = vec[j];
  int size = vec.size();
  while(i < size) {
    if(vec[i].weight > max.weight){
      max = vec[i];
      j = i;
    }
    i++;
  }
  max = vec[j];
  vec.erase(vec.begin()+j);
  return max.destination;
}

//Finds distances difference between two warehouses to destinations
vector<adjNode> Graph::FindDistanceDiff(vector<adjNode> &warehouse1, vector<adjNode> &warehouse2) {
  vector<adjNode> distanceDiff;
  int dest_size = warehouse1.size();
  for(int i=0; i<dest_size; i++)
    distanceDiff.push_back(adjNode(i,absolute(warehouse1[i].weight - warehouse2[i].weight)));
  return distanceDiff;
}

//Takes absolute value of an integer
int Graph::absolute(int num) {
  if(num < 0)
    return (-1)*num;
  else
    return num;
}

//Takes distance vectors from shortestPath() function, and shares the destinations equally among warehouses
void Graph::shareDestinations(vector<int> &warehouses, vector<int> &destinations) {
  vector<int> shortest_1 = shortestPath(warehouses[0]);
  vector<int> shortest_2 = shortestPath(warehouses[1]);
  int dest_size = destinations.size();
  int max = 0;
  int totalWeight = 0;
  vector<adjNode> warehouse1;
  vector<adjNode> warehouse2;
  vector<adjNode> result;
  vector<adjNode> result_1;
  vector<adjNode> result_2;

  for(int i=0; i<dest_size; i++) {
    warehouse1.push_back(adjNode(destinations[i], shortest_1[destinations[i]]));
    warehouse2.push_back(adjNode(destinations[i], shortest_2[destinations[i]]));
  }

  vector<adjNode> distanceDiff = FindDistanceDiff(warehouse1, warehouse2);
  for(int i=0; i<dest_size; i++) {
    if(result_1.size() == dest_size/2 || result_2.size() == dest_size/2)
      break;
    max = maxIndex(distanceDiff);

    if(warehouse1[max].weight < warehouse2[max].weight) {
      totalWeight += warehouse1[max].weight;
      result_1.push_back(adjNode(warehouses[0], warehouse1[max].destination));
    } else {
      totalWeight += warehouse2[max].weight;
      result_2.push_back(adjNode(warehouses[1], warehouse2[max].destination));
    }
  }
  int distanceDiffSize = distanceDiff.size();
  if(result_1.size() != dest_size/2) {
    for(int i=0; i<distanceDiffSize; i++) {
      max = maxIndex(distanceDiff);
      totalWeight += warehouse1[max].weight;
      result_1.push_back(adjNode(warehouses[0], warehouse1[max].destination));
    }
  } else if(result_2.size() != dest_size/2) {
    for(int i=0; i<distanceDiffSize; i++) {
      max = maxIndex(distanceDiff);
      totalWeight += warehouse2[max].weight;
      result_2.push_back(adjNode(warehouses[1], warehouse2[max].destination));
    }
  }
  for(int i=0; i<dest_size/2; i++)
    result.push_back(adjNode(result_1[i].destination, result_1[i].weight));
  for(int i=0; i<dest_size/2; i++)
    result.push_back(adjNode(result_2[i].destination, result_2[i].weight));
  sort(result.begin(), result.end(), weightComparator());

  cout << totalWeight << endl;
  for(int i=0; i<dest_size; i++)
    cout << result[i].weight << " " << result[i].destination << endl;
}

//Djikstra's algorithm to find the shortest path from one source to all other nodes
//Implementation with adjacency list and priority queue
vector<int> Graph::shortestPath(int src) {
  //Priority queue to keep track of the  nodes
  priority_queue <adjNode, vector<adjNode>, adjNodeComparator> pri_queue;

  //Vector to keep track of the weight information
  vector<int> distances(v, INT_MAX);
  //Source vertex has been added to the priority pri_queue
  pri_queue.push(adjNode(src,0));
  distances[src] = 0; //Source vertex distance to itself is zero
  while(!pri_queue.empty()) {

    //Destination and weight information of the vertex in priority queue is kept
    int dest_node = pri_queue.top().destination;
    int dist = pri_queue.top().weight;

    //Vertex has been popped from queue
    pri_queue.pop();

    //Vertex's neighbours are traversed
    //Distance informations are updated with respect to constraints of the algorithm
    vector<adjNode>::iterator it;
    for(it = g[dest_node].begin(); it != g[dest_node].end(); ++it) {
      if(distances[(*it).destination] > distances[dest_node] + (*it).weight) {
        distances[(*it).destination] = distances[dest_node] + (*it).weight;
        pri_queue.push(adjNode((*it).destination,distances[(*it).destination]));
      }
    }
  }
  return distances;
}

//Creates and adds the adjacency nodes to adjacency list
void Graph::create_add_edge(int src, int dest, int weight) {
    g[src].push_back(adjNode(dest, weight));
}

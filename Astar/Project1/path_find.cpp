//contains the algoritim for Astar

#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <queue>
#include <functional>
using namespace std;


//declare the components of each node
struct Node {
	int x;
	int y;
	double G;
	double H;
	double F;
	vector<Node*> nodeNeighbours;
	Node* parent;
	bool visited;
	bool blocked;
};


double find_distance(Node* start, Node* end);
bool sort_helper(Node* i, Node* j);

//returns the found path as (x,y) pairs to be used in the main function

vector<pair<int, int>> find_path(int startx, int starty, int endx, int endy, char board[20][20]) {

	int board_dimension = 20;

	const int total_nodes = board_dimension * board_dimension;
	//create an array of nodes
	Node* allNodes = new Node[board_dimension * board_dimension];

	for (int x = 0; x < board_dimension; x++) {
		for (int y = 0; y < board_dimension; y++) {
			allNodes[y * board_dimension + x].x = x;
			allNodes[y * board_dimension + x].y = y;
			allNodes[y * board_dimension + x].visited = false;
			allNodes[y * board_dimension + x].parent = nullptr;
			allNodes[y * board_dimension + x].G = INFINITY;
			allNodes[y * board_dimension + x].H = INFINITY;
			allNodes[y * board_dimension + x].F = INFINITY;



			if (board[y][x] == 'B') {
				allNodes[y * board_dimension + x].blocked = true;
			}
			else {
				allNodes[y * board_dimension + x].blocked = false;

			}




		}
	}
	//create the neighbour connections (straight and diagonals)
	for (int x = 0; x < board_dimension; x++){
		for (int y = 0; y < board_dimension; y++)
		{
			if (y > 0)
				allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[(y - 1) * board_dimension + x]);
			if (y < board_dimension - 1)
				allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[(y + 1) * board_dimension + x]);
			if (x > 0)
				allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[y * board_dimension + (x - 1)]);
			if (x < board_dimension - 1)
				allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[y * board_dimension + (x + 1)]);
			if (y > 0 && x > 0)
				if (!allNodes[(y)*board_dimension + (x - 1)].blocked && !allNodes[(y - 1) * board_dimension + (x)].blocked) {
					allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[(y - 1) * board_dimension + (x - 1)]);
				}
			if (y<board_dimension-1 && x>0)
				if (!allNodes[(y)*board_dimension + (x - 1)].blocked && !allNodes[(y +1) * board_dimension + (x)].blocked) {
					allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[(y + 1) * board_dimension + (x - 1)]);
				}
			if (y>0 && x<board_dimension-1)
				if (!allNodes[(y)*board_dimension + (x + 1)].blocked && !allNodes[(y - 1) * board_dimension + (x)].blocked) {
					allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[(y - 1) * board_dimension + (x + 1)]);
				}
			if (y<board_dimension - 1 && x<board_dimension-1)
				if (!allNodes[(y)*board_dimension + (x + 1)].blocked && !allNodes[(y + 1) * board_dimension + (x)].blocked) {
					allNodes[y * board_dimension + x].nodeNeighbours.push_back(&allNodes[(y + 1) * board_dimension + (x + 1)]);
				}			
		}
	}

	Node* start = &allNodes[starty * board_dimension + startx];
	Node* end = &allNodes[endy * board_dimension + endx];

	// intially start at start point
	Node* nodeCurr = start;
	start->G = 0;
	start->H = find_distance(start, end);
	start->F = start->G + start->H;

	vector<Node*> openNodes;
	openNodes.push_back(start);


	while (!openNodes.empty() && nodeCurr != end) {

		//sort the openNodes to always explore the one with the smallest G+H=F value
		if (openNodes.size() > 1) {
			sort(openNodes.begin(), openNodes.end(), sort_helper);
		}
		while (!openNodes.empty() && openNodes.front()->visited) {
			openNodes.erase(openNodes.begin());
		}
		
		if (openNodes.empty())
			break;

		nodeCurr = openNodes.front();

		nodeCurr->visited = true; //only analyize a node once

		//go through all the neightbours
		for (auto neighbour : nodeCurr->nodeNeighbours) {

			//firstly push all accessable neighbours to the openNodes
			if (!neighbour->blocked && !neighbour->visited) {
				openNodes.push_back(neighbour);
			}

			//calculate the F (really only G changes) value for the neighbours from the path given by the current node as the parent
			//if it is a lower F value, then replace the one currently in it

			double potential_G = nodeCurr->G + find_distance(nodeCurr, neighbour);

			if (potential_G < neighbour->G) {
				neighbour->G = potential_G;
				neighbour->H = find_distance(neighbour, end);
				neighbour->F = neighbour->G + neighbour->H;
				neighbour->parent = nodeCurr;
			}

		}



	}

	//now we assume the path is found. get a point vector that contains all the x and y points from the end to start and return them
	vector <pair<int, int>>path;

	while (nodeCurr != nullptr) {
		pair<int, int>points(nodeCurr->x, nodeCurr->y);
		path.push_back(points);
		nodeCurr = nodeCurr->parent;
	}

	delete[]allNodes;

	return path;


}

double find_distance(Node* start, Node* end) {
	return sqrt((start->x - end->x) * (start->x - end->x) + (start->y - end->y) * (start->y - end->y));
}

bool sort_helper(Node* i, Node* j) {
	return i->F < j->F;
}
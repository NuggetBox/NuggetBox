#include "AStar.h"

struct AStarNodeInternal
{
	int GetValue() const { return AccumulatedCost + Heuristic; }

	std::vector<AStarNodeInternal*> Neighbours;
	int AccumulatedCost = -1;
	int Heuristic = -1;
	int Predecessor = -1;
	bool IsOpen = false;
};
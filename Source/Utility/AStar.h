#pragma once
#include <vector>

#include "MinHeap.hpp"

class AStarNode
{
public:
	AStarNode() = default;
	AStarNode(int aHeuristic, int aWeight, const std::vector<AStarNode*>& someNeighbours)
	{
		myHeuristic = aHeuristic;
		myWeight = aWeight;
		myNeighbours = someNeighbours;
	}

	void AddNeighbour(AStarNode* aNeighbour) { myNeighbours.push_back(aNeighbour); }
	void SetHeuristic(int aHeuristic) { myHeuristic = aHeuristic; }
	void SetWeight(int aWeight) { myWeight = aWeight; }

private:
	std::vector<AStarNode*> myNeighbours;
	int myHeuristic = -1;
	int myWeight = INT_MAX;
};

//TODO: Make class so threading is possible etc.
class AStarPathfinder
{
};

inline std::vector<int> GetPath(const std::vector<AStarNode*>& someNodes, int aStartIndex, int anEndIndex)
{
	if (aStartIndex == anEndIndex)
		return std::vector<int>();



	bool foundPath = false;

	Utility::MinHeap<AStarNode*> openNodes;
	openNodes.Enqueue(someNodes[aStartIndex]);

	while (!openNodes.IsEmpty() && !foundPath)
	{

	}


}
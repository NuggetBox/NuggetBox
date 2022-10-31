#pragma once
#include <vector>

#include "MinHeap.hpp"

namespace AStar
{
	struct Node
	{
		Node(int aHeuristic)
		{
			Data = new AStarData();
			Data->Heuristic = aHeuristic;
			Data->AccumulatedCost = 0;
		}

		~Node()
		{
			delete Data;
		}

		struct AStarData
		{
			int Heuristic = -1;
			int AccumulatedCost = -1;
			int Cost() const { return Heuristic + AccumulatedCost; }
		};

		AStarData* Data;
		Node* Predecessor = nullptr;
		std::vector<Node*> Neighbours;

		bool Open = false;

		bool operator()(const Node* aFirst, const Node* aSecond) const
		{
			if (aFirst->Data->Cost() < aSecond->Data->Cost())
			{
				return aFirst->Data->Heuristic < aSecond->Data->Heuristic;
			}

			return aFirst->Data->Cost() < aSecond->Data->Cost();
		}

		//TODO: Fix scuffed == operator
		/*bool operator==(const Node& aNode) const
		{
			if (Data->Heuristic != aNode.Data->Heuristic)
			{
				return false;
			}

			if (Neighbours.size() == aNode.Neighbours.size())
			{
				for (int i = 0; i < Neighbours.size(); ++i)
				{
					if (Neighbours[i] != aNode.Neighbours[i])
					{
						return false;
					}
				}
			}

			return true;
		}*/
	};

	//TODO: Make class so threading is possible etc.
	class AStarPathfinder
	{
	};

	inline std::vector<Node*> GetPath(Node* aStart, Node* aEnd)
	{
		if (aStart == aEnd)
		{
			return std::vector<Node*>();
		}

		Node* currentNode;
		bool foundPath = false;

		Utility::MinHeap<Node*> openNodes;
		aStart->Open = true;
		openNodes.Enqueue(aStart);

		while (!openNodes.IsEmpty() && !foundPath)
		{
			currentNode = openNodes.Dequeue();

			for (auto& neighbour : currentNode->Neighbours)
			{
				if (!neighbour->Open)
				{
					if (neighbour->Data->AccumulatedCost < currentNode->Data->AccumulatedCost + 1)
					{
						neighbour->Data->AccumulatedCost = currentNode->Data->AccumulatedCost + 1;
						neighbour->Predecessor = currentNode;
						neighbour->Open = true;

						if (neighbour == aEnd)
						{
							currentNode = neighbour;
							foundPath = true;
							break;
						}

						openNodes.Enqueue(neighbour);
					}
				}
			}
		}


		if (foundPath)
		{
			std::vector<Node*> path;

			int safetyCount = 0;
			while (currentNode != nullptr)
			{
				safetyCount++;
				if (safetyCount > 99999)
				{
					assert(false && L"Oops");
				}

				path.push_back(currentNode);
				currentNode = currentNode->Predecessor;
			}

			std::reverse(path.begin(), path.end());

			return path;
		}

		return std::vector<Node*>();
	}
}
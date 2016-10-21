// GraphSearch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h";
#include <string>
#include <queue>
#include<iostream>
#include<string>
#include<memory>
#include<fstream>
#include<sstream>
#include <iterator>
#include<sstream>
#include<algorithm>
#include<map>
#include<iterator>
#include<vector>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <locale>
#include<memory>
#include <iomanip>  
using namespace std;

//class _graphNode
//{
//	int nodeId;
//	vector<arc*> arcCollection;
//	const _graphNode* &operator= (const _graphNode* &rhsObject)
//	{
//		this->nodeId = (*rhsObject).nodeId;
//		this->arcCollection = (*rhsObject).arcCollection;
//	}
//};

//class _arc
//{
//	graphNode* sourceNode;
//	int weight;
//	graphNode* destinationNode;
//};
// -------------------------------------------

enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class mPoint
{
public:
	mPoint()
	{

	}
	int RowNumber;
	int ColumnNumber;
};
class Projection
{
public:
	Projection()
	{
		prevProjectionIndex = -1;
		nectProjectionIndex = -1;
		isPin = false;
		isPlotted = false;
	}
	Direction direction;
	bool isPlotted;
	bool isPin;
	mPoint startPoint;
	mPoint endPoint;
	int ownerArcId;
	int index;
	int prevProjectionIndex;
	int nectProjectionIndex;
};
class Pin
{
public :
	Pin()
	{
		isUsed = isPlotted = false;
	}
	enum PIN_DIRECTION
	{
		IN,
		OUT
	};
	enum PIN_POSITION
	{
		TOP,
		BUTTOM,
		LEFT,
		RIGHT
	};
	PIN_POSITION pinPosition;
	vector<int> connetedArcs;
	int projectionId;
	bool isUsed;
	bool isPlotted;
	PIN_DIRECTION pinType;
};
class arc
{
public:
	arc()
	{
		isPlotted = false;
	}
	enum DIRECTION
	{
		IN,
		OUT
	};
	bool isWeightPrinted = false;
	DIRECTION arcDirection;
	int arcIndex;
	bool isPlotted;
	int source_Node_Id;
	Pin::PIN_POSITION source_Pin_Position;
	int weight;
	int dest_Node_Id;
	Pin::PIN_POSITION dest_Pin_Position;
	vector<Projection> parts;
};

class graphNode
{
public:
	graphNode()
	{
		bool isRoot = false;
		// top
		Pin topPin;
		topPin.pinPosition == Pin::PIN_POSITION::TOP;
		topPin.isUsed = false;
		pins[Pin::PIN_POSITION::TOP] = topPin;
		// buttom
		Pin buttomPin;
		buttomPin.pinPosition == Pin::PIN_POSITION::BUTTOM;
		buttomPin.isUsed = false;
		pins[Pin::PIN_POSITION::BUTTOM] = buttomPin;
		// right
		Pin rightPin;
		rightPin.pinPosition = Pin::PIN_POSITION::RIGHT;
		rightPin.isUsed = false;
		pins[Pin::PIN_POSITION::RIGHT] = rightPin;
		// left
		Pin leftPin;
		leftPin.pinPosition = Pin::PIN_POSITION::LEFT;
		leftPin.isUsed = false;
		pins[Pin::PIN_POSITION::LEFT] = leftPin;
	}
	map<Pin::PIN_POSITION, Pin> pins;
	bool isRoot = false;
	int previousNodeIndex;
	bool isVisited = false;
	int nodeId;
	int cost; // used in Dijikstra algorithm.
	int transitiveCost;
	vector<arc> arcCollection;
	vector<arc> incomingArcCol;
	int columnIndex;
	int rowIndex;


	bool posibleCandidatePin(Pin::PIN_POSITION pinPos, arc::DIRECTION arcDirection)
	{
		bool result = false;
		Pin currentPin = pins[pinPos];
		
			if (currentPin.isUsed)
			{
				cout << " Pin :" << pinPos << " is connected to any other node." << endl;
				if (currentPin.pinType == arcDirection)
				{
					/*for (map<Pin::PIN_POSITION,Pin>::iterator i = pins.begin(); i != pins.end(); i++)
					{
					if (((*i).first != currentPin.pinPosition && !(*i).second.isUsed))
					{
					result = true;
					break;
					}
					}*/
					// Alternative
					int unUsedPinCount = count_if(pins.begin(), pins.end(),
						[currentPin](pair<Pin::PIN_POSITION, Pin> _dataPair)
					{
						return (_dataPair.first != currentPin.pinPosition && !_dataPair.second.isUsed);
					});

					if (unUsedPinCount == 0) // all other pins are used.
					{
						result = true;
						cout << " Pin :" << pinPos << " is connected to any other node." << endl;
					}

				}
			}
			else
			{
				cout << " Pin :" << pinPos << " is not connected to any other node." << endl;
				result = true;
				pins[pinPos].isUsed = true;
				pins[pinPos].pinType = arcDirection == arc::DIRECTION::OUT ?  
					Pin::PIN_DIRECTION::OUT : Pin::PIN_DIRECTION::IN;
			}
		
		return result;
	}
	Pin::PIN_POSITION getNodePinPosition(graphNode otherNode, arc _arc)
	{
		// this function will consider 4 sector in plan , considering current point at (0,0)
		//								|
		//		Sector 1 : TOP - RIGHT  |	Sector 2 : TOP-LEFT
		//		-----------------------------------------------
		//		sector 3 : BUTTOM-RIGHT |	sector 4 : BUTTOM-LEFT
		//								|					
		//
		//
		// check availability if not present suggest some else.
		// check weather prefered in is same direction else leave that preference
		// if this pin is used see for any other pin else keep this

		Pin::PIN_POSITION pos;
		for each (pair<Pin::PIN_POSITION,Pin> pinPair in pins)
		{
			if (!pinPair.second.isUsed)
			{
				pos = pinPair.first;
				break;
			}
		}

		pins[pos].isUsed = true;
		/*Pin::PIN_POSITION position;
		arc::DIRECTION arcDirection = _arc.source_Node_Id == nodeId ? arc::DIRECTION::OUT : arc::DIRECTION::IN;

		Pin::PIN_POSITION firstPref = rowIndex < otherNode.rowIndex ? Pin::BUTTOM : Pin::TOP;
		Pin::PIN_POSITION secondPref = columnIndex < otherNode.columnIndex ? Pin::RIGHT : Pin::LEFT;

		if (posibleCandidatePin(firstPref, arcDirection))
		{
			position = firstPref;
			pins[position].isUsed = true;
		}
		else if (posibleCandidatePin(secondPref, arcDirection))
		{
			position = secondPref;
			pins[position].isUsed = true;
		}
		else
		{
			for (map<Pin::PIN_POSITION, Pin > ::iterator it = pins.begin(); it != pins.end(); it++)
			{
				if ((*it).first != firstPref && (*it).first != secondPref && !(*it).second.isUsed)
				{
					position = (*it).second.pinPosition;	
					pins[position].isUsed = true;
					break;
				}
			}
		}	*/
		return pos;
	}

};
class pathInfo
{
public:
	int endNode;
	int rootNodeId;
	string pathStr;
	int cost;

};
class graph
{
public :
	
	map<int, pathInfo> pathCost;
	vector<Projection> projections;
	mPoint getMatrixPositionByNodeId(int nodeIndex , bool* found)
	{
		mPoint point;
		auto nodeIt = find_if(nodes.begin(), nodes.end(), [nodeIndex](graphNode node)
		{ return  nodeIndex == node.nodeId; });
		if (nodeIt != nodes.end())
		{
			(*found) = true;
			point.ColumnNumber = (*nodeIt).columnIndex;
			point.RowNumber = (*nodeIt).rowIndex;
		}
		return point;
	}
	vector<graphNode> nodes;
	vector<arc> arcs;
	void addArc(int sourceNodeId, int destinationNodeId, int weight)
	{
		// check if source/Destination node is present
			// if not present then add node for that.
		// create arc with all above data.
		int nodeCount = nodes.size();
		vector<graphNode>::iterator sourceNodeIt = std::find_if(nodes.begin(), nodes.end(), [&](graphNode n) 
													{return sourceNodeId == n.nodeId; });
		vector<graphNode>::iterator destinationNodeIt = 
			std::find_if(nodes.begin(), nodes.end(), [&](graphNode n)
													{return destinationNodeId == n.nodeId; });
		bool sourceNodePresent = sourceNodeIt!=nodes.end();
		bool desNodePresent = destinationNodeIt != nodes.end();
		arc _arc;
		_arc.dest_Node_Id = destinationNodeId; _arc.source_Node_Id = sourceNodeId; 
		_arc.weight = weight; _arc.arcIndex = arcs.size() +1; 
		graphNode sourceNode;
		if (!sourceNodePresent)
		{
			sourceNode.nodeId = sourceNodeId;
			sourceNode.cost = sourceNode.transitiveCost = nodeCount == 0 ? 0 : INT_MAX;
			
			sourceNode.arcCollection.push_back(_arc);
			nodes.push_back(sourceNode);
			//cout << "Node for " << sourceNodeId << " is been Added" << endl;
		}
		else
		{
			sourceNode = (*sourceNodeIt);
			(*sourceNodeIt).arcCollection.push_back(_arc);
		}
		if (!desNodePresent)
		{
			graphNode DestinationNode;
			DestinationNode.nodeId = destinationNodeId;
			DestinationNode.cost = DestinationNode.transitiveCost = INT_MAX;
			DestinationNode.incomingArcCol.push_back(_arc);
			nodes.push_back(DestinationNode);
		}
		else
		{
			(*destinationNodeIt).incomingArcCol.push_back(_arc);
		}
		arcs.push_back(_arc);
	}

	void display()
	{
		for each (graphNode node in nodes)
		{
			//cout << "Node : " << node.nodeId << endl;
			for each (arc _arc in node.arcCollection)
			{
				//cout << " To : " << _arc.dest_Node_Id << "  with weight : " << _arc.weight << endl;
			}
		}
	}

	void printPathDetail(int index, string preData)
	{
		string detail = preData;
		
		vector<graphNode> desendents;
		
		for each (graphNode node in nodes)
		{
			if (node.previousNodeIndex == index)
			{
				desendents.push_back(node);
			}
		}
		if (!desendents.empty())
		{
			for (vector<graphNode>::iterator i = desendents.begin() ; i != desendents.end() ; i++)
			{			
				//for each (arc _arc in (*NodeIt).arcCollection)
				//{
				ostringstream convert;  // stream used for the conversion
				convert << (*i).nodeId;
				string intPart = convert.str();
				string printData = " --> " + intPart;
				cout << preData + printData << "  With Cost/Distance : " << (*i).transitiveCost << endl;
				pathCost[(*i).nodeId].cost = (*i).transitiveCost;
				pathCost[(*i).nodeId].pathStr = preData + printData;
				printPathDetail((*i).nodeId,preData + printData);
				//}
				//it++;
			}
		}
		else
		{
			vector<graphNode>::iterator NodeIt = find_if(nodes.begin(), nodes.end(), [index](graphNode node)
			{
				return node.nodeId == index;
			});
			if (NodeIt != nodes.end())
			{
				if ((*NodeIt).isRoot && desendents.empty())
				{
					cout << "you can't reach any node from Node : " << (*NodeIt).nodeId << endl;
				}
			}
			//auto prevNodeIt = find_if(nodes.begin(), nodes.end(), [index](graphNode node) {return index == node.nodeId; });
			//detail += (" Weight : " + (*prevNodeIt).transitiveCost);
			//cout << detail << endl;
		}		
	}

	void initializeGraph(int startIndex)
	{
		for (vector<graphNode>::iterator it = nodes.begin(); it != nodes.end(); it++)
		{
			if ((*it).nodeId == startIndex)
			{
				(*it).isRoot = true;

				(*it).cost = (*it).transitiveCost = 0; (*it).previousNodeIndex = 0;
				(*it).isVisited = false; (*it).isRoot = true;
			}
			else
			{
				(*it).isRoot = false;
				(*it).cost = (*it).transitiveCost = INT_MAX; (*it).previousNodeIndex = 0;
				(*it).isVisited = false; (*it).isRoot =false;
				
				pathInfo pinfo;
				pinfo.cost = -1;
				pinfo.endNode = (*it).nodeId;
				pinfo.rootNodeId = startIndex;
				pathCost[(*it).nodeId] = pinfo;

			}
		}	
	}
	void printShortPath()
	{
		cout << left << setw(15) << "Start Node";
		cout << left << setw(15) << "End Node" ;
		cout << left << setw(35) << "Path" ;
		cout << left << setw(10) << "Cost" ;
		cout << "\n" << endl;

		//cout << "=====================================================================" << endl;
		cout << left << setw(15) << "==========" ;
		cout << left << setw(15) << "========" ;
		cout << left << setw(35) << "====" ;
		cout << left << setw(10) << "====" ;
		cout << "\n" << endl;
		int count = 1;

		for each (pair<int,pathInfo> info in pathCost)
		{
			int nodeId = info.second.rootNodeId;
			auto nodeIt = find_if(nodes.begin(), nodes.end(),[nodeId](graphNode _node)
						{
							return _node.nodeId == nodeId;
							});
			if ((*nodeIt).isRoot)
			{
				string cost = info.second.cost == -1 ? "infinte" : to_string(info.second.cost);
				string path = info.second.pathStr.empty() ? "No Path" : info.second.pathStr;
				cout << left << setw(15) << info.second.rootNodeId;
				cout << left << setw(15) << info.first ;
				cout << left << setw(35) << path;
				cout << left << setw(10) << cost;
				cout << "\n" << endl;				
				count++;
			}
		}
	}
	/*void displayShortestPath(int startIndex)
	{
		vector<graphNode>::iterator NodeIt = find_if(nodes.begin(), nodes.end(), [startIndex](graphNode node) 
							{
								return node.nodeId == startIndex;
							});


	}*/
};

vector<string> splitBySpace(string str) {
	std::stringstream ss(str);
	vector<string> elems;
	std::string item;
	while (std::getline(ss, item, ' ')) {
		elems.push_back(item);
	}
	return elems;
}

vector<int> split(string str)
{
	vector<int> numbers;
	vector<string> strCollection = splitBySpace(str);
	for each (string str in strCollection)
	{
		int strNum = 0;
		for each (char _charr in str)
		{
			if (isdigit(_charr))
				strNum = strNum * 10 + _charr - '0';					
		}
		numbers.push_back(strNum);
	}
	return numbers;
}

vector<map<int, int>> getArcInfo(string stanza)
{
	vector<int> dataVec = split(stanza);
	int counter = 0;
	vector<map<int, int>> ArcWeightCol;
	for (int i = 1; i < dataVec.size(); i++)
	{
		int nodeIndex = dataVec.at(i);
		i++;
		int nodeWeight = dataVec.at(i);
		map<int, int> _data;
		_data[nodeIndex] = nodeWeight;
		ArcWeightCol.push_back(_data);
	}
	return ArcWeightCol;
}

int getCurrentNodeIndex(string stanza)
{
	vector<int> dataVec = split(stanza);
	return dataVec.at(0);
}
class Cell
{
public :
	int cellIndex;
	Cell()
	{
		character = CHARACTER::SPACE;
	}
	enum CHARACTER {
		OPENING_BRACE,
		CLOSING_BRACE,
		SPACE,
		NUMBER,
		VERTICAL_BAR,
		HORIZONTAL_BAR,
		RIGHT_ARROW,
		LEFT_ARROW,
		DOWN_ARROW,
		UP_ARROW,
		INTERSECTION,
		OVERLAP
	};
	int lineNumber;
	//int Index;
	int charValue;
	CHARACTER character;
	char getChar()
	{
		char c;
		switch (character)
		{
		case Cell::OPENING_BRACE:
			c = '[';
			break;
		case Cell::CLOSING_BRACE:
			c = ']';
			break;
		case Cell::NUMBER:
			c = charValue;
			break;
		case Cell::SPACE:
			c = ' ';
			break;
		case Cell::VERTICAL_BAR:
			c = '|';
			break;
		case Cell::HORIZONTAL_BAR:
			c = '-';
			break;
		case Cell::RIGHT_ARROW:
			c = '<';
			break;
		case Cell::LEFT_ARROW:
			c = '>';
			break;
		case Cell::DOWN_ARROW:
			c = 'V';
			break;
		case Cell::UP_ARROW:
			c = '^';
			break;
		case Cell::INTERSECTION:
			c = '+';
			break;
		case Cell::OVERLAP:
			c = '=';
			break;
		default:
			c = 'null';
			break;
		}
		return c;
	}
};
struct compareNode
{

	bool operator()(graphNode left, graphNode right)
	{
		return left.transitiveCost > right.transitiveCost;
	}
};
class Row
{
public:
	int lineIndex;
	vector<Cell> cells;
	void move(int positions)
	{
		vector<Cell> newCells;
		for (size_t i = 1; i <= positions; i++)
		{
			Cell cell;
			//cell.Index = i;
			cell.character = Cell::CHARACTER::SPACE;
			newCells.push_back(cell);
		}

		//copy(cells.begin() , cells.end() , )
	}
};

//graphNode* getPriorityQueueNodeById(priority_queue<graphNode, vector<graphNode>, compareNode> pqReachableNodes,int index)
//{
//	for each (graphNode node in pqReachableNodes.)
//	{
//
//	}
//}
void printParent(int currentIndex, vector<graphNode> nodes)
{
	
}

priority_queue<graphNode, vector<graphNode>, compareNode> updatePriorityQueue(
	priority_queue<graphNode, vector<graphNode>, compareNode> pq, graphNode item)
{
	priority_queue<graphNode, vector<graphNode>, compareNode> newPq;
	while (!pq.empty())
	{
		graphNode tempItem = pq.top();
		pq.pop();
		if (tempItem.nodeId == item.nodeId)
			newPq.push(item);
		else
			newPq.push(tempItem);		
	}
	return newPq;
}

graph applyDijikstra(graph _graph, int startIndex)
{
	// get connected nodes
	// update traveling cost. 
	priority_queue<graphNode, vector<graphNode>, compareNode> pqReachableNodes;
	for each (graphNode node in _graph.nodes)
	{
		if (startIndex == node.nodeId)
		{
			node.transitiveCost = node.cost = 0;
			//cout << " node with Id :" << node.nodeId << " is root." << endl;
		}
		else
		{
			node.transitiveCost = node.cost = INT_MAX;
			//cout << " node with Id :" << node.nodeId << " is having INFINITE value." << endl;
		}
		pqReachableNodes.push(node);
	}


	while (!pqReachableNodes.empty())
	{
		graphNode currentNode = pqReachableNodes.top();
		pqReachableNodes.pop();
		//cout << "Control is  going to node : " << currentNode.nodeId;
		//cout << "Node with least cost from Priority_Queue : " << currentNode.nodeId << endl;
		vector<graphNode>::iterator sourceNodeIt =
			find_if(_graph.nodes.begin(), _graph.nodes.end(), [currentNode](graphNode node)
		{
			return currentNode.nodeId == node.nodeId;
		});
		/*if (sourceNodeIt != _graph.nodes.end())
		{
			(*sourceNodeIt).isVisited = true;
		}*/

		for each (arc _arc in currentNode.arcCollection)
		{
			vector<graphNode>::iterator destNodeIt = find_if(_graph.nodes.begin(), _graph.nodes.end(), [_arc](graphNode node)
			{
				return (_arc.dest_Node_Id == node.nodeId);// && !node.isVisited;
			});
			if (destNodeIt != _graph.nodes.end())
			{
				if (currentNode.transitiveCost != INT_MAX)
				{
					int newCost = currentNode.transitiveCost + _arc.weight;
					if (newCost < (*destNodeIt).transitiveCost)
					{
						//if (newCost < (*destNodeIt).transitiveCost)
						//{
						//	cout << "Prev Node : " << (*sourceNodeIt).nodeId << endl;
						//	cout << "Prev Distance : " << (*sourceNodeIt).transitiveCost << endl;
						//	//cout << "New Weight : " << (*destNodeIt).cost << endl;
						//	cout << "New Distance : " << (*destNodeIt).transitiveCost << endl;
						//}
						(*destNodeIt).transitiveCost = newCost;
						(*destNodeIt).cost = _arc.weight;
						(*destNodeIt).previousNodeIndex = currentNode.nodeId;
						pqReachableNodes = updatePriorityQueue(pqReachableNodes, (*destNodeIt));
					}
				}
			}
		}

	}
	return _graph;
}

void old_applyDijikstra(graph _graph , int startIndex)
{
	priority_queue<graphNode, vector<graphNode>, compareNode> pqReachableNodes;
	for each (graphNode node in _graph.nodes)
	{
		if (startIndex == node.nodeId)
		{
			node.transitiveCost = node.cost = 0;
			//cout << " node with Id :" << node.nodeId << " is root." << endl;
		}
		else
		{
			node.transitiveCost = node.cost = INT_MAX;
			//cout << " node with Id :" << node.nodeId << " is having INFINITE value." << endl;
		}
		pqReachableNodes.push(node);
	}


	while (!pqReachableNodes.empty())
	{
		graphNode currentNode = pqReachableNodes.top();
		pqReachableNodes.pop();
		//cout << "Node with least cost from Priority_Queue : " << currentNode.nodeId << endl;
		vector<graphNode>::iterator sourceNodeIt =
			find_if(_graph.nodes.begin(), _graph.nodes.end(), [currentNode](graphNode node)
		{
			return currentNode.nodeId == node.nodeId;
		});
		if (sourceNodeIt != _graph.nodes.end())
		{
			(*sourceNodeIt).isVisited = true;
		}

		for each (arc _arc in currentNode.arcCollection)
		{
			vector<graphNode>::iterator destNodeIt = find_if(_graph.nodes.begin(), _graph.nodes.end(), [_arc](graphNode node)
			{
				return (_arc.dest_Node_Id == node.nodeId) && !node.isVisited;
			});
			if (destNodeIt != _graph.nodes.end())
			{
				int newCost = currentNode.transitiveCost + _arc.weight;
				if (newCost < (*destNodeIt).transitiveCost || (*destNodeIt).transitiveCost == INT_MAX)
				{
					(*destNodeIt).transitiveCost = newCost;
					(*destNodeIt).cost = _arc.weight;
					(*destNodeIt).previousNodeIndex = currentNode.nodeId;
					/*if (newCost < (*destNodeIt).transitiveCost)
					{
						cout << "Prev Cost : " << (*sourceNodeIt).nodeId << endl;
						cout << "Prev Node : " << (*sourceNodeIt).transitiveCost << endl;
					}
					else if ((*destNodeIt).transitiveCost == INT_MAX)
					{
						cout << "1St time Prev Node set" << endl;
						cout << "Prev Cost : " << (*sourceNodeIt).nodeId << endl;
						cout << "Prev Node : " << (*sourceNodeIt).transitiveCost << endl;
						cout << "New Weight : " << (*destNodeIt).cost << endl;
					}*/
				}
			}
			else
				cout << "it was visited previously" << endl;
		}

	}
}
void printGraph(vector<Row> matrix)
{
	ofstream outfile;

	outfile.open("plotedGraph.dat", ios::out | ios::app);
	if (outfile)
	{
		outfile.clear();
		outfile << "Graph : " << endl;
	}
	
	cout << " Graph : " << endl;
	for each (Row row in matrix)
	{		
		string line;
		for each (Cell cell in row.cells)
		{		
			if (cell.character == cell.NUMBER)
			{
				int nodeIndex = cell.getChar();
				string _c = to_string(nodeIndex);
				if (outfile)
					line.append(_c);
				cout << _c;
			}
			else if (cell.character == cell.SPACE)
			{
				cout << ' ';
				if (outfile)
					line.append(" ");
				//cout << ' ';
			}
			else
			{
				//cell.getChar();
				//string temp = to_string(cell.getChar());
				if (outfile)
					line += cell.getChar();
				cout << cell.getChar();
			}
		}
		if (outfile)
			outfile << line << endl;
		cout << endl;
	}
	if (outfile)
		outfile.close();
}

struct compareEdgeCount
{
	bool operator()(graphNode left, graphNode right)
	{
		return (left.arcCollection.size() + left.incomingArcCol.size()) > 
			(right.arcCollection.size()+right.incomingArcCol.size());
	}
};

struct compareEdgeCountDesc
{
	bool operator()(graphNode left, graphNode right)
	{
		return (left.arcCollection.size() + left.incomingArcCol.size()) <
			(right.arcCollection.size() + right.incomingArcCol.size());
	}
};




vector<Row> paintPins(mPoint point,  Pin::PIN_POSITION PinPosition , 
	vector<Row> matrix , bool isOutGoingPin, Projection* projection)
{
	(*projection).startPoint = point;
	if (point.ColumnNumber == 36 && point.RowNumber == 21)
	{
		int a;
		a = 1;

	}
	int endPointCol;
	int endPointRow;
	switch (PinPosition)
	{
		case Pin::PIN_POSITION::BUTTOM:
			matrix.at(point.RowNumber + 1).cells.at(point.ColumnNumber).charValue = isOutGoingPin ? '|' : '^';
			matrix.at(point.RowNumber + 1).cells.at(point.ColumnNumber).character = 
					isOutGoingPin ? Cell::CHARACTER::VERTICAL_BAR : Cell::CHARACTER::UP_ARROW;
					
			matrix.at(point.RowNumber + 2).cells.at(point.ColumnNumber).charValue = '|';
			matrix.at(point.RowNumber + 2).cells.at(point.ColumnNumber).character = Cell::CHARACTER::VERTICAL_BAR;

			matrix.at(point.RowNumber + 3).cells.at(point.ColumnNumber).charValue = '|';
			matrix.at(point.RowNumber + 3).cells.at(point.ColumnNumber).character = Cell::CHARACTER::VERTICAL_BAR;
			endPointCol = point.ColumnNumber; endPointRow = point.RowNumber + 3;
			
			break;
		case Pin::PIN_POSITION::TOP:
			matrix.at(point.RowNumber - 1).cells.at(point.ColumnNumber).charValue = isOutGoingPin ? '|': 'V';
			matrix.at(point.RowNumber - 1).cells.at(point.ColumnNumber).character = 
				isOutGoingPin ? Cell::CHARACTER::VERTICAL_BAR : Cell::CHARACTER::DOWN_ARROW;

			matrix.at(point.RowNumber - 2).cells.at(point.ColumnNumber).charValue = '|';
			matrix.at(point.RowNumber - 2).cells.at(point.ColumnNumber).character = Cell::CHARACTER::VERTICAL_BAR;

			matrix.at(point.RowNumber - 3).cells.at(point.ColumnNumber).charValue = '|';
			matrix.at(point.RowNumber - 3).cells.at(point.ColumnNumber).character = Cell::CHARACTER::VERTICAL_BAR;

			endPointCol = point.ColumnNumber; endPointRow = point.RowNumber - 3;
			break;
		case Pin::PIN_POSITION::LEFT:
			matrix.at(point.RowNumber).cells.at(point.ColumnNumber - 2).charValue = isOutGoingPin ? '-':'>';
			matrix.at(point.RowNumber).cells.at(point.ColumnNumber - 2).character = 
				isOutGoingPin ? Cell::CHARACTER::HORIZONTAL_BAR : Cell::CHARACTER::LEFT_ARROW;

			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber - 3).charValue = '-';
			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber - 3).character = Cell::CHARACTER::HORIZONTAL_BAR;

			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber - 4).charValue = '-';
			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber - 4).character = Cell::CHARACTER::HORIZONTAL_BAR;

			endPointCol = point.ColumnNumber - 4; endPointRow = point.RowNumber;
			break;
		case Pin::PIN_POSITION::RIGHT:
			matrix.at(point.RowNumber).cells.at(point.ColumnNumber + 2).charValue = isOutGoingPin ? '-': '<';
			matrix.at(point.RowNumber).cells.at(point.ColumnNumber + 2).character =
				isOutGoingPin ? Cell::CHARACTER::HORIZONTAL_BAR : Cell::CHARACTER::RIGHT_ARROW;

			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber + 3).charValue = '-';
			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber + 3).character = Cell::CHARACTER::HORIZONTAL_BAR;

			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber + 4).charValue = '-';
			matrix.at(point.RowNumber ).cells.at(point.ColumnNumber + 4).character = Cell::CHARACTER::HORIZONTAL_BAR;

			endPointCol = point.ColumnNumber + 4; endPointRow = point.RowNumber;
			break;
		
		default:
			break;
	}
	mPoint endPoint;
	endPoint.ColumnNumber = endPointCol;
	endPoint.RowNumber = endPointRow;
	(*projection).endPoint = isOutGoingPin ? endPoint : point;
	(*projection).startPoint = isOutGoingPin ? point : endPoint;
	return matrix;
}
//bool isIntersecting(int startPoint, int endpoint, Direction prevProjectionDir, Direction LastProjectionDir)
//{
//
//}
vector<int> getIntInParts(int _inte)
{
	vector<int> parts;
	string intStr = to_string(_inte);
	for each (char _char in intStr)
	{
		parts.push_back(_char - '0');
	}
	return parts;
}
vector<Row> makeLines(mPoint start, mPoint end, int arcIndex,
		Pin::PIN_POSITION startPinPos, Pin::PIN_POSITION endPinPos,
		graph* _graph, vector<Row> matrix , Projection* startPin , Projection* endPin)
{
	bool isCostPrinted = false;
	Direction prevProjDir;
	switch (startPinPos)
	{
	case Pin::TOP:
		prevProjDir = Direction::UP;
		break;
	case Pin::BUTTOM:
		prevProjDir = Direction::DOWN;
		break;
	case Pin::LEFT:
		prevProjDir = Direction::LEFT;
		break;
	case Pin::RIGHT:
		prevProjDir = Direction::RIGHT;
		break;
	default:
		break;
	}
	Direction lastProDir;
	switch (endPinPos)
	{
	case Pin::TOP:
		lastProDir = Direction::UP;
		break;
	case Pin::BUTTOM:
		lastProDir = Direction::DOWN;
		break;
	case Pin::LEFT:
		lastProDir = Direction::LEFT;
		break;
	case Pin::RIGHT:
		lastProDir = Direction::RIGHT;
		break;
	default:
		break;
	}
	map<Direction, vector<int>> directions;
	vector<int> pointsOne;
	vector<int> pointsTwo;
	vector<int> pointsThree;
	//vector<Projection> 
	if (end.RowNumber < start.RowNumber)
	{
		switch (startPinPos)
		{
		case Pin::TOP:
			// -----------------------------------------------------------
			switch (endPinPos)
			{
			case Pin::TOP:
				// UP LEFT
				#pragma region Top
				// go UP				
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// go left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;
				#pragma endregion
				break;
			case Pin::BUTTOM:
				// up left
				#pragma region Buttom
				// go UP				
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// go left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;
				#pragma endregion
				break;
			case Pin::LEFT:
				// left up
				#pragma region Left
				// go left
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;
				// go UP				
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				#pragma endregion
				
				break;
			case Pin::RIGHT:
				// UP left
				#pragma region Right
				// go UP			
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// go left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;
				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
			
		case Pin::BUTTOM:
			// -----------------------------------------------------------
			switch (endPinPos)
			{
			case Pin::TOP:
				// Right -> Up -> Left
				#pragma region Top
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber + 5);
				directions[Direction::RIGHT] = pointsOne;
				// up
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(start.ColumnNumber + 5);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber + 5);
				directions[Direction::UP] = pointsTwo;
				// left
				pointsThree.push_back(end.RowNumber);
				pointsThree.push_back(start.ColumnNumber + 5);
				pointsThree.push_back(end.RowNumber);
				pointsThree.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsThree;
				#pragma endregion
				break;
			case Pin::BUTTOM:
				
				// Left -> Up
				#pragma region BUTTOM
				// go left
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsOne;
				// go UP				
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsTwo;
				#pragma endregion
				break;
			case Pin::LEFT:
				// Left Up
				#pragma region LEFT
				// go left
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsOne;
				// go UP				
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsTwo;

				#pragma endregion
				break;
			case Pin::RIGHT:
				#pragma region RIGHT
				// go left
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsOne;
				// go UP				
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsTwo;

				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		case Pin::LEFT:

			switch (endPinPos)
			{
			case Pin::TOP:
				// Up Left
				#pragma region Top
				// Up
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::BUTTOM:
				// Up Left
				#pragma region BUTTOM
				// Up
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;

				#pragma endregion
				
				break;
			case Pin::LEFT:
				
				// Left Up
				#pragma region LEFT
				// go left
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsOne;
				// go UP				
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsTwo;

				#pragma endregion
				
				break;
			case Pin::RIGHT:
				// Up Left
				#pragma region RIGHT
				// Up
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;

				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		case Pin::RIGHT:
			// -----------------------------------------------------------
			switch (endPinPos)
			{
			case Pin::TOP:
				// Up Left
				#pragma region Top
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::BUTTOM:
				// Up Left
				#pragma region BUTTOM
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::LEFT:
				// Up Left Up

				#pragma region LEFT
				// Up 
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber - 5);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(start.RowNumber - 5);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(start.RowNumber - 5);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;
				// Up
				pointsThree.push_back(start.RowNumber - 5);
				pointsThree.push_back(end.ColumnNumber);
				pointsThree.push_back(end.RowNumber);
				pointsThree.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsThree;
				#pragma endregion
				break;
			case Pin::RIGHT:
				// Up left
				#pragma region RIGHT
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::UP] = pointsOne;
				// Left
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::LEFT] = pointsTwo;

				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		default:
			break;
		}
	}
	else
	{
		switch (startPinPos)
		{
		case Pin::TOP:
			// -----------------------------------------------------------
			switch (endPinPos)
			{
			case Pin::TOP:
				// Right Down
				#pragma region Top
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::DOWN] = pointsTwo;
				#pragma endregion
				break;
			case Pin::BUTTOM:
				// left down right
				#pragma region BUTTOM
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber - 5);
				directions[Direction::LEFT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(start.ColumnNumber - 5);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber - 5);
				directions[Direction::DOWN] = pointsTwo;
				// Right
				
				pointsThree.push_back(end.RowNumber);
				pointsThree.push_back(start.ColumnNumber - 5);
				pointsThree.push_back(end.RowNumber);
				pointsThree.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsThree;

				#pragma endregion
				break;
			case Pin::LEFT:
				// right down
				#pragma region LEFT				
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::DOWN] = pointsTwo;

				#pragma endregion
				break;
			case Pin::RIGHT:
				// Right Down
				#pragma region RIGHT
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::DOWN] = pointsTwo;
				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		case Pin::BUTTOM:
			// -----------------------------------------------------------
			switch (endPinPos)
			{
			case Pin::TOP:
				// down right
				#pragma region Top
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;
				#pragma endregion
				break;
			case Pin::BUTTOM:
				// down right
				#pragma region BUTTOM
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;
				#pragma endregion
				break;
			case Pin::LEFT:
				// down right
				#pragma region LEFT
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::RIGHT:
				// Right Down
				#pragma region RIGHT
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::DOWN] = pointsTwo;
				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		case Pin::LEFT:

			switch (endPinPos)
			{
			case Pin::TOP:
				// Down Right
				#pragma region Top
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::BUTTOM:
				// Down Right
				#pragma region BUTTOM
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::LEFT:
				// Down Right
				#pragma region LEFT
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::RIGHT:
				// Down Right Up
				#pragma region RIGHT
				// down 
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber + 5);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber + 5);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber + 5);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;
				// Up
				pointsThree.push_back(end.RowNumber + 5);
				pointsThree.push_back(end.ColumnNumber);
				pointsThree.push_back(end.RowNumber);
				pointsThree.push_back(end.ColumnNumber);
				directions[Direction::UP] = pointsThree;
				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		case Pin::RIGHT:
			// -----------------------------------------------------------
			switch (endPinPos)
			{
			case Pin::TOP:
				// Right Down
				#pragma region Top
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::DOWN] = pointsTwo;
				#pragma endregion
				break;
			case Pin::BUTTOM:
				// Down Right
				#pragma region BUTTOM
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::LEFT:
				// Down Right
				#pragma region LEFT
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(end.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				directions[Direction::DOWN] = pointsOne;
				// Right
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(start.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsTwo;

				#pragma endregion
				break;
			case Pin::RIGHT:
				// Right Down
				#pragma region RIGHT
				// right
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(start.ColumnNumber);
				pointsOne.push_back(start.RowNumber);
				pointsOne.push_back(end.ColumnNumber);
				directions[Direction::RIGHT] = pointsOne;
				// Down
				pointsTwo.push_back(start.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				pointsTwo.push_back(end.RowNumber);
				pointsTwo.push_back(end.ColumnNumber);
				directions[Direction::DOWN] = pointsTwo;
				#pragma endregion
				break;
			default:
				break;
			}
			// -----------------------------------------------------------
			break;
		default:
			break;
		}
	}
	
	int directionCount = 1;
	for each (pair<Direction,vector<int>> direction in directions)
	{
		int pointCount = 1;
		bool isArcWeightPrinted = false;
		Projection projection;
		projection.ownerArcId = arcIndex;
		projection.isPlotted = true;
		projection.isPin = false;
		mPoint projectionStart;
		
		mPoint projectionEnd;
		
		for (vector<int>::iterator i = direction.second.begin(); i != direction.second.end(); i++)
		{
			if (pointCount == 1)
			{
				projectionStart.RowNumber = (*i);
			}
			else if (pointCount == 2)
			{
				projectionStart.ColumnNumber = (*i);
			}
			else if(pointCount == 3)
			{
				projectionEnd.RowNumber = (*i);
			}
			else if (pointCount == 4)
			{
				projectionEnd.ColumnNumber = (*i);
			}
			pointCount++;
		}
		Direction _dirc = direction.first;
		// -------------------------------------------------------
		/*
		auto arcIt = find_if((*_graph).arcs.begin(), (*_graph).arcs.end(), [arcIndex](arc _arc)
		{	return _arc.arcIndex == arcIndex;	});
		if (arcIt != (*_graph).arcs.end())
		{
			currentPro.index = (*arcIt).parts.size();
			(*arcIt).parts.push_back(currentPro);
		}*/
		
		Projection currentPro;
		currentPro.startPoint = projectionStart;
		currentPro.endPoint = projectionEnd;
		currentPro.direction = direction.first;
		currentPro.ownerArcId = arcIndex;
		auto arcIt = find_if((*_graph).arcs.begin(), (*_graph).arcs.end(), [arcIndex](arc temparc) {
			return arcIndex == temparc.arcIndex;
		});
		if (arcIt != (*_graph).arcs.end())
		{
			(*arcIt).parts.push_back(currentPro);
		}

		// -------------------------------------------------------
		bool completeLine = true;
		switch (_dirc)
		{
		case UP:
			for (int i = projectionEnd.RowNumber; i <= projectionStart.RowNumber; i++)
			{
				
				if ((matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue == '|' ||
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue == '-') && 
					(i != projectionEnd.RowNumber && i != projectionStart.RowNumber))
				{
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue = '=';
					matrix.at(i).cells.at(projectionStart.ColumnNumber).character = Cell::CHARACTER::OVERLAP;
					//completeLine = false;
				}
				else
				{

					bool isIntersecting = ((i == projectionEnd.RowNumber || i == projectionStart.RowNumber) &&
						_dirc != prevProjDir);
					//completeLine = ( i == projectionEnd.RowNumber || i != projectionStart.RowNumber &&
						//			matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue == '+') ? true: !isIntersecting;
					/*	isIntersecting = ((directionCount == directions.size()) &&
					(lastProDir == _dirc) &&
					i == projectionStart.RowNumber) ? false : isIntersecting;
					*/
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue =
						isIntersecting ? '+' : '|';
					matrix.at(i).cells.at(projectionStart.ColumnNumber).character =
						isIntersecting ? Cell::CHARACTER::INTERSECTION : Cell::CHARACTER::VERTICAL_BAR;
				}
				if (i!= projectionEnd.RowNumber && i != projectionStart.RowNumber &&
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue != '|')
				{
					if(completeLine)
						completeLine = false;
				}
			}
			if (!!(*arcIt).isWeightPrinted &&
				(projectionStart.RowNumber - projectionEnd.RowNumber) > 5)
			{
				int rowIndex = projectionStart.RowNumber - 3;
				int columnIndex = projectionStart.ColumnNumber;
				vector<int> intParts = getIntInParts((*arcIt).weight);
				for each (int _int in intParts)
				{
					matrix.at(rowIndex).cells.at(columnIndex).charValue = _int;
					matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
					columnIndex++;
				}
				(*arcIt).isWeightPrinted = true;
				isArcWeightPrinted = true;
				cout << "Weight printed between node : " << (*arcIt).source_Node_Id << " And " << (*arcIt).dest_Node_Id << endl;
			}
			prevProjDir = _dirc;
			break;
		case DOWN:
			//bool completeLine = true;
			for (int i = projectionStart.RowNumber; i <= projectionEnd.RowNumber; i++)
			{
				if ((i != projectionStart.RowNumber && i != projectionEnd.RowNumber )&&
					(matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue == '|' ||
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue == '-'))
				{
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue = '=';
					matrix.at(i).cells.at(projectionStart.ColumnNumber).character = Cell::CHARACTER::OVERLAP;
					completeLine = false;
				}
				else
				{
					bool isIntersecting =
						((i == projectionEnd.RowNumber || i == projectionStart.RowNumber) && _dirc != prevProjDir);
					completeLine = ((i == projectionStart.RowNumber || i == projectionEnd.RowNumber) &&
						matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue == '+') ? true : !isIntersecting;
					/*isIntersecting = (directionCount == directions.size() &&
					lastProDir == _dirc && i == projectionEnd.RowNumber) ? false : isIntersecting;*/
					completeLine = !isIntersecting;
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue =
						isIntersecting ? '+' : '|';
					matrix.at(i).cells.at(projectionStart.ColumnNumber).character =
						isIntersecting ? Cell::CHARACTER::INTERSECTION : Cell::CHARACTER::VERTICAL_BAR;
				}				
				if (i != projectionEnd.RowNumber && i != projectionStart.RowNumber &&
					matrix.at(i).cells.at(projectionStart.ColumnNumber).charValue != '|')
				{
					if (completeLine)
						completeLine = false;
				}
			}
			if (!(*arcIt).isWeightPrinted &&
				( projectionEnd.RowNumber - projectionStart.RowNumber) > 5)
			{
				int rowIndex = projectionEnd.RowNumber - 3;
				int columnIndex = projectionEnd.ColumnNumber;
				vector<int> intParts = getIntInParts((*arcIt).weight);
				for each (int _int in intParts)
				{
					matrix.at(rowIndex).cells.at(columnIndex).charValue = _int;
					matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
					columnIndex++;
				}
				isArcWeightPrinted = true;
				(*arcIt).isWeightPrinted = true;
				cout << "Weight printed between node : " << (*arcIt).source_Node_Id << " And " << (*arcIt).dest_Node_Id << endl;
			}
			prevProjDir = _dirc;
			break;
		case LEFT:
			//bool completeLine = true//;
			for (int i = projectionEnd.ColumnNumber; i <= projectionStart.ColumnNumber; i++)
			{
				if ((i != projectionEnd.ColumnNumber && i != projectionStart.ColumnNumber) &&
					(matrix.at(projectionEnd.RowNumber).cells.at(i).charValue == '|' ||
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue == '-'))
				{
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue = '=';
					matrix.at(projectionEnd.RowNumber).cells.at(i).character = Cell::CHARACTER::OVERLAP;
					//completeLine = false;
				}
				else
				{
					bool isIntersecting = ((i == projectionEnd.ColumnNumber || i == projectionStart.ColumnNumber) &&
						_dirc != prevProjDir);
					//completeLine = matrix.at(projectionEnd.RowNumber).cells.at(i).charValue == '+' ? true : !isIntersecting;
					/*	isIntersecting = (directionCount == directions.size() &&
							lastProDir == _dirc && i == projectionStart.ColumnNumber) ? false : isIntersecting;*/
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue =
						isIntersecting ? '+' : '-';
					matrix.at(projectionEnd.RowNumber).cells.at(i).character =
						isIntersecting ? Cell::CHARACTER::INTERSECTION : Cell::CHARACTER::HORIZONTAL_BAR;
				}
				if (i !=projectionEnd.ColumnNumber && i != projectionStart.ColumnNumber &&
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue != '-')
				{
					if (completeLine)
						completeLine = false;
				}
			}
			if (!(*arcIt).isWeightPrinted &&
				(projectionStart.ColumnNumber - projectionEnd.ColumnNumber) > 5)
			{
				int rowIndex = projectionStart.RowNumber;
				int columnIndex = projectionStart.ColumnNumber - (to_string((*arcIt).weight).length() + 2);
				vector<int> intParts = getIntInParts((*arcIt).weight);
				

				for each (int _int in intParts)
				{
					matrix.at(rowIndex).cells.at(columnIndex).charValue = _int;
					matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
					columnIndex++;
				}
				(*arcIt).isWeightPrinted = true;
				isArcWeightPrinted = true;
				cout << "Weight printed between node : " << (*arcIt).source_Node_Id << " And " << (*arcIt).dest_Node_Id << endl;
			}
			prevProjDir = _dirc;
			break;
		case RIGHT:
			for (int i = projectionStart.ColumnNumber; i <= projectionEnd.ColumnNumber; i++)
			{
				if ( (i != projectionStart.ColumnNumber && i != projectionEnd.ColumnNumber ) &&
					(matrix.at(projectionEnd.RowNumber).cells.at(i).charValue == '|' ||
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue == '-'))
				{
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue = '=';
					matrix.at(projectionEnd.RowNumber).cells.at(i).character = Cell::CHARACTER::OVERLAP;
				}
				else
				{

					bool isIntersecting = ((i == projectionStart.ColumnNumber || i == projectionEnd.ColumnNumber)
						&& _dirc != prevProjDir);
					/*isIntersecting = (directionCount == directions.size() &&
					lastProDir == _dirc && i == projectionEnd.ColumnNumber) ? false : isIntersecting;*/
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue =
						isIntersecting ? '+' : '-';
					matrix.at(projectionEnd.RowNumber).cells.at(i).character =
						isIntersecting ?
						Cell::CHARACTER::INTERSECTION : Cell::CHARACTER::HORIZONTAL_BAR;
				}
				if ( i!= projectionEnd.ColumnNumber&&i!= projectionStart.ColumnNumber &&
					matrix.at(projectionEnd.RowNumber).cells.at(i).charValue != '-')
				{
					if (completeLine)
						completeLine = false;
				}
			}
			if (!(*arcIt).isWeightPrinted &&
				(projectionEnd.ColumnNumber - projectionStart.ColumnNumber) > (to_string((*arcIt).weight).length() + 5))
			{
				int rowIndex = projectionStart.RowNumber;
				int columnIndex = projectionEnd.ColumnNumber - (to_string((*arcIt).weight).length() + 2);
				vector<int> intParts = getIntInParts((*arcIt).weight);
				
				for each (int _int in intParts)
				{
					matrix.at(rowIndex).cells.at(columnIndex).charValue = _int;
					matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
					columnIndex++;
				}
				isArcWeightPrinted = true;
				(*arcIt).isWeightPrinted = true;
				cout << "Weight printed between node : " << (*arcIt).source_Node_Id << " And " << (*arcIt).dest_Node_Id << endl;
			}

			prevProjDir = _dirc;
			break;
		default:
			break;
		}
		directionCount++;
	}
	return matrix;
}
bool isLineinTact(mPoint start, mPoint end, vector<Row> matrix)
{
	bool isIntact = true;
	if (start.ColumnNumber == end.ColumnNumber)
	{
		// Vertical Line
		if (start.RowNumber < end.RowNumber )
		{
			// Downward Vertical Line
			for (int i = start.RowNumber; i <= end.RowNumber; i++)
			{
				if (matrix.at(i).cells.at(start.ColumnNumber).charValue == '=')
				{
					isIntact = false;
					break;
				}
			}
		}
		else if (end.RowNumber < start.RowNumber)
		{
			for (int i = end.RowNumber; i <= start.RowNumber; i++)
			{
				if (matrix.at(i).cells.at(start.ColumnNumber).charValue == '=')
				{
					isIntact = false;
					break;
				}
			}
		}
	
	}
	else if (start.RowNumber == end.RowNumber)
	{
		// Horizontal Line
		if (start.ColumnNumber < end.ColumnNumber )
		{
			for (int i = start.ColumnNumber; i <= end.ColumnNumber; i++)
			{
				if (matrix.at(start.RowNumber).cells.at(i).charValue == '=')
				{
					isIntact = false;
					break;
				}
			}
		}
		else if (end.ColumnNumber <  start.ColumnNumber)
		{
			for (int i = end.ColumnNumber; i <= start.ColumnNumber; i++)
			{
				if (matrix.at(end.RowNumber).cells.at(i).charValue = '=')
				{
					isIntact = false;
					break;
				}
			}
		}
	}
	return isIntact;
}
vector<Row> printWeight(graph _graph , vector<Row> matrix)
{
	for each (graphNode node in _graph.nodes)
	{
		for each (arc arcItem in node.arcCollection)
		{
			vector<Projection> parts;
			for each (Projection proj in _graph.projections)
			{
				if (proj.ownerArcId == arcItem.arcIndex)
					parts.push_back(proj);
			}
			for each (Projection projection in parts)
			{

				if (!projection.isPin && isLineinTact(projection.startPoint , projection.endPoint,matrix))
				{
					if (projection.startPoint.ColumnNumber == projection.endPoint.ColumnNumber)
					{
						// Vertical Line
						
						if (projection.startPoint.RowNumber < projection.endPoint.RowNumber  && 
							( projection.endPoint.RowNumber - projection.startPoint.RowNumber) > 5)
						{
							// Downward Vertical Line
							//int midPoint = (projection.endPoint.RowNumber - projection.startPoint.RowNumber) / 2;
							int rowIndex = projection.endPoint.RowNumber - (to_string(arcItem.weight).length() + 1);
							int columnIndex = projection.startPoint.ColumnNumber;
							matrix.at(rowIndex).cells.at(columnIndex).charValue = arcItem.weight;
							matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
							break;
						}
						else if(projection.endPoint.RowNumber < projection.startPoint.RowNumber  && 
							(projection.startPoint.RowNumber - projection.endPoint.RowNumber) > 5)
						{
							// upward Vertical Line
							int rowIndex = projection.startPoint.RowNumber - 2;
							int columnIndex = projection.startPoint.ColumnNumber;
							matrix.at(rowIndex).cells.at(columnIndex).charValue = arcItem.weight;
							matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
							break;
						}					
					}
					else if (projection.startPoint.RowNumber == projection.endPoint.RowNumber)
					{
						// Horizontal Line
						if (projection.startPoint.ColumnNumber < projection.endPoint.ColumnNumber &&
							(projection.endPoint.ColumnNumber - projection.startPoint.ColumnNumber) > 5)
						{
							int columnIndex = projection.endPoint.ColumnNumber - 2;
							int rowIndex = projection.startPoint.RowNumber;
							matrix.at(rowIndex).cells.at(columnIndex).charValue = arcItem.weight;
							matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
							break;
						}
						else if (projection.endPoint.ColumnNumber<  projection.startPoint.ColumnNumber &&
							(projection.startPoint.ColumnNumber - projection.endPoint.ColumnNumber) > 5)
						{
							int columnIndex = projection.startPoint.ColumnNumber;
							int rowIndex = projection.endPoint.RowNumber;
							matrix.at(rowIndex).cells.at(columnIndex).charValue = arcItem.weight;
							matrix.at(rowIndex).cells.at(columnIndex).character = Cell::CHARACTER::NUMBER;
							break;
						}						
					}			
				}
			}
		}
	}
	return matrix;
}
void connectNodes(graph* _graph, vector<Row> matrix)
{
	priority_queue<graphNode, vector<graphNode>, compareEdgeCountDesc> pqSunil;
	for each (graphNode node in (*_graph).nodes)
	{
		pqSunil.push(node);
	}

	while (!pqSunil.empty())
	{
		graphNode sourceNode1 = pqSunil.top();
		pqSunil.pop();
		std::cout << "making connections for node :" << sourceNode1.nodeId << endl;
		for each (arc _arc in sourceNode1.arcCollection)
		{
			auto sourceNodeIt = find_if((*_graph).nodes.begin(), (*_graph).nodes.end(),
				[_arc](graphNode _node)
			{
				return _node.nodeId == _arc.source_Node_Id;
			});
			auto destNodeIt = find_if((*_graph).nodes.begin(), (*_graph).nodes.end(),
				[_arc](graphNode _node)
			{
				return _node.nodeId == _arc.dest_Node_Id;
			});
			
			Pin::PIN_POSITION sourcePinPos = (*sourceNodeIt).getNodePinPosition((*destNodeIt), _arc);

			Pin::PIN_POSITION DestPinPos = (*destNodeIt).getNodePinPosition((*sourceNodeIt), _arc);
			cout << " Pin : " << sourcePinPos << " to connect with node :" << (*destNodeIt).nodeId << " at Pin : " << DestPinPos << endl;
		
			mPoint mSourcePoint;	
			mPoint fProjEndPoint;
			mPoint sProjStartPoint;
			mSourcePoint.ColumnNumber = (*sourceNodeIt).columnIndex;
			mSourcePoint.RowNumber = (*sourceNodeIt).rowIndex;

			Projection firstPro;
			firstPro.isPin = true;
			firstPro.isPlotted = true;
			firstPro.ownerArcId = _arc.arcIndex;
			int currentIndex = firstPro.index = (*_graph).projections.size() + 1;
			firstPro.startPoint.RowNumber = (*sourceNodeIt).rowIndex;
			firstPro.startPoint.ColumnNumber = (*sourceNodeIt).columnIndex;
			(*_graph).projections.push_back(firstPro);
		
			Projection* firstPtr;
			firstPtr = &(*_graph).projections.at((*_graph).projections.size()-1);
			matrix = paintPins(mSourcePoint, sourcePinPos, matrix, true, firstPtr);
			(*sourceNodeIt).pins[sourcePinPos].isUsed = true;
			(*sourceNodeIt).pins[sourcePinPos].isPlotted = true;
			fProjEndPoint = (*firstPtr).endPoint;
						mPoint mDestPoint;
			mDestPoint.ColumnNumber = (*destNodeIt).columnIndex;
			mDestPoint.RowNumber = (*destNodeIt).rowIndex;

			
			Projection lastPro; currentIndex = lastPro.index = (*_graph).projections.size() + 1;
			lastPro.isPin = true;
			lastPro.isPlotted = true;
			lastPro.ownerArcId = _arc.arcIndex;
			lastPro.startPoint.RowNumber = (*destNodeIt).rowIndex;
			lastPro.startPoint.ColumnNumber = (*destNodeIt).columnIndex;
			(*_graph).projections.push_back(lastPro);

			Projection* secPtr;
			secPtr = &(*_graph).projections.at((*_graph).projections.size() - 1);
			matrix = paintPins(mDestPoint, DestPinPos, matrix, false, secPtr);
			(*destNodeIt).pins[DestPinPos].isPlotted = true;
			(*destNodeIt).pins[DestPinPos].isUsed = true;
			sProjStartPoint = (*secPtr).startPoint;
			// -------------------------------------------------------
			auto arcIt = find_if((*_graph).arcs.begin(), (*_graph).arcs.end(), [_arc](arc temparc) {
				return _arc.arcIndex == temparc.arcIndex;
			});
			if (arcIt != (*_graph).arcs.end())
			{
				(*arcIt).parts.push_back((*firstPtr));
				(*arcIt).parts.push_back((*secPtr));
			}

			// -------------------------------------------------------

			matrix = makeLines(fProjEndPoint, sProjStartPoint, _arc.arcIndex,
						sourcePinPos, DestPinPos, _graph, matrix, firstPtr, secPtr);
			
		}
	}
	//matrix = printWeight((*_graph),matrix);
	printGraph(matrix);
}
vector<Row> plotNodes(graph _graph)
{
	priority_queue<graphNode, vector<graphNode>, compareEdgeCount> pq;
	vector<Row> matrix;
	/*sort(_graph.nodes.begin(), _graph.nodes.end(), [](graphNode node1, graphNode node2)
	{	node1.arcCollection.size() < node2.arcCollection.size();	});*/
	for each (graphNode node in _graph.nodes)
	{
		pq.push(node);
	}
	int totalNumberOfRow = 13 * _graph.nodes.size();
	int totalNumberOfColumn = 15 * _graph.nodes.size();
	int columnpadding = 7;
	int nextRow = 7;
	bool nodeInsertedInrow = false;
	for (int i = 0; i <= totalNumberOfRow; i++)
	{
		Row _row;
		_row.lineIndex = i;
		int spaceBtnNode = 1;
		for (int j = 0; j <= totalNumberOfColumn; j++)
		{
			if (columnpadding == j && !nodeInsertedInrow && nextRow == i && !pq.empty())
			{
				nodeInsertedInrow = true;
				Cell _cell_OpeningBrace;
				_cell_OpeningBrace.cellIndex = j;
				_cell_OpeningBrace.character = Cell::CHARACTER::OPENING_BRACE;
				_cell_OpeningBrace.charValue = '[';
				_row.cells.push_back(_cell_OpeningBrace);

				j++;
				Cell _cell_Data;
				_cell_Data.cellIndex = j;
				_cell_Data.character = Cell::CHARACTER::NUMBER;

				graphNode node = pq.top();
				pq.pop();
				_cell_Data.charValue = node.nodeId;
				int nodeIndex = node.nodeId;
				_row.cells.push_back(_cell_Data);
				// ---------------------------------------------
				auto nodeIt = find_if(_graph.nodes.begin(), _graph.nodes.end(), [nodeIndex](graphNode node)
				{ return  nodeIndex == node.nodeId; });
				if (nodeIt != _graph.nodes.end())
				{
					(*nodeIt).rowIndex = i; (*nodeIt).columnIndex = j;
				}
				// ---------------------------------------------
				j++;
				Cell _cell_ClosingBrace;
				_cell_ClosingBrace.cellIndex = j;
				_cell_ClosingBrace.character = Cell::CHARACTER::CLOSING_BRACE;
				_cell_ClosingBrace.charValue = ']';
				_row.cells.push_back(_cell_ClosingBrace);

				spaceBtnNode = 1;
				columnpadding += 14;
				nextRow += 7;
			}
			else
			{
				Cell _cell;
				_cell.cellIndex = j;
				_cell.character = Cell::CHARACTER::SPACE;
				_row.cells.push_back(_cell);
				spaceBtnNode++;
			}
		}
		nodeInsertedInrow = false;
		matrix.push_back(_row);
	}
	//printGraph(matrix);
	connectNodes(&_graph, matrix);
	return matrix;

}

int main()
{
	// Developer Note : Remember for transitive cost we depend upon original data of graph.
	

	map<int, Row> plottedGraph;

	graph _graph;
	ifstream infile("graph.dat");
	string stanza;

	if (infile.fail()) 
	{
		cout << "There was a problem while opening the file." << endl;
	}
	else
	{
		// Read 
		while (getline(infile, stanza))
		{			
			cout << stanza << endl;
			vector<map<int, int>> arcInfoCol = getArcInfo(stanza);
			int nodeIndex = getCurrentNodeIndex(stanza);
			
			for each (map<int,int> arcInfoMap in arcInfoCol)
			{
				//_graph.addArc(nodeIndex,arcInfo. )
				map<int, int>::iterator arcInfoIt = arcInfoMap.begin();
				for (map<int, int>::iterator arcInfoIt = arcInfoMap.begin(); 
						arcInfoIt != arcInfoMap.end(); 
						arcInfoIt++)
				{
					_graph.addArc(nodeIndex, (*arcInfoIt).first, (*arcInfoIt).second);
				}
			}

		}
		
		// Print 
		cout << "Graph : " << endl;
		plotNodes(_graph);
		

		//cout << ""
		// Find Shortest Path
		// Get Priority Queue filled.
		vector<int> nodeIndexes;

		for each (graphNode var in _graph.nodes)
		{
			nodeIndexes.push_back(var.nodeId);			
		}
		for each (int var in nodeIndexes)
		{
			_graph.initializeGraph(var);
			_graph = applyDijikstra(_graph, var);

			cout << "\n" << endl;
			ostringstream convert;  // stream used for the conversion
			convert << var;
			string intPart = convert.str();
			
			cout << "Shortest paths with Root Node : " << var << endl;
			cout << "Path in Brief :" << endl;
			_graph.printPathDetail(var, intPart);
			cout << "\n" << endl;
			cout << "Detailed data :" << endl;
			
			_graph.printShortPath();
		}
			
	}
	//_graph.display();
	//system("pause");
    return 0;
}


/*
	Computer Science III
	Assignment #10

	This problem will solve the shortest-paths problem by using
	a dynamic programming technique called Dijkstra's algorithm.

	Due: Thursday, April 30th, 2020 @ 11:59 PM
	Programmed By: Paul Byars
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>

using namespace std;

class Vertex
{
	private:
		string label;
		int degree;
 	public:
 		Vertex();
 		void setVertex(string);
 		void setDegree(int);
 		string getVertex() const;
 		int getDegree() const;
};

class WeightedGraph
{
	private:
		int numVertices;
		Vertex *vertex;
		int **weight;
		void setVertexDegrees();
		void findVertexColoring();
	public:
		WeightedGraph();
		~WeightedGraph();
		void readFromFile();
		void showGraph();
		void shortestPaths (int) const;
};

const string COLOR[] = {"red", "blue", "yellow", "green", "orange", "violet", "gray", "brown"};

// MAIN DRIVER
int main()
{
	WeightedGraph graph;
	int first = 0;
	
	graph.readFromFile();
	graph.showGraph();
	
	cout << endl << "Enter which vertex weight you would like to use: ";
	cin >> first;
	cout << endl;
	
	graph.shortestPaths(first);
	
	return 0;
}

// Function: Vertex
// Purpose: constructor for the Vertex class
// Incoming: none
// Outgoing: none
// Return: none
Vertex::Vertex()
{
	label = "";
}

// Function: setVertex
// Purpose: setter for the private label varibale
// Incoming: theLabel - string
// Outgoing: none
// Return: none
void Vertex::setVertex(string theLabel)
{
	label = theLabel;
}

// Function: setDegree
// Purpose: setter for the private degree variable
// Incoming: theDegree - int
// Outgoing: none
// Return: none
void Vertex::setDegree(int theDegree)
{
	degree = theDegree;
}

// Function: getVertex
// Purpose: getter for the private label variable
// Incoming: none
// Outgoing: none
// Return: label - string
string Vertex::getVertex() const
{
	return label;
}

// Function: getDegree
// Purpose: detter for the private degree variable
// Incoming: none
// Outgoing: none
// Return: degree - int
int Vertex::getDegree() const
{
	return degree;
}

// Function: WeightedGraph
// Purpose: constructor for the WeightedGraph class
// Incoming: none
// Outgoing: none
// Return: none
WeightedGraph::WeightedGraph()
{
	numVertices = 0;
}

// Function: ~WeightedGraph
// Purpose: destructor for the WeightedGraph class
// Incoming: none
// Outgoing: none
// Return: none
WeightedGraph::~WeightedGraph()
{
	for (int i = 0; i < numVertices; i++)
	{
		delete[] weight[i];
	}
	delete[] vertex;
	delete[] weight;
}

// Function: readFromFile
// Purpose: read in from a text file and store the data into dynamically created arrays
// Incoming: none
// Outgoing: none
// Return: none
void WeightedGraph::readFromFile()
{
	int from = 0, to = 0, readWeight = 0;
	string filename, label = "", firstVertex = "", secondVertex = "";
    ifstream infile;
	
    cout << "Enter graph file name: ";
    cin >> filename;
    infile.open(filename);
    
    if (infile.fail())
    {
        cout << "Infile could not open" << endl;
        exit(1);
    }

	infile >> numVertices;
	infile >> ws;
	vertex = new Vertex[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		infile >> label;
		vertex[i].setVertex(label);
	}
	
	weight = new int*[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		weight[i] = new int[numVertices];
		for (int j = 0; j < numVertices; j++)
		{
			weight[i][j] = 0;
		}
	}

	while (!infile.eof())
	{
		infile >> firstVertex >> secondVertex >> readWeight;
		for (int i = 0; i < numVertices; i++)
		{
			if (vertex[i].getVertex() == firstVertex)
			{
				from = i;
			}
			else if (vertex[i].getVertex() == secondVertex)
			{
				to = i;
			}
		}
		weight[from][to] = readWeight;
 	}
    infile.close();
    
    setVertexDegrees();
}

// Function: showGraph
// Purpose: print out the stored data
// Incoming: none
// Outgoing: none
// Return: none
void WeightedGraph::showGraph()
{
	cout << "Vertices" << endl;
	for (int i = 0; i < numVertices; i++)
	{
		cout << "  " << i << left << "  " << vertex[i].getVertex() << endl;
	}
	
	cout << endl << "Edges" << endl << "From/To" << endl;
	cout << "             ";
 	for (int i = 0; i < numVertices; i++)
	{
		cout << left << setw(15) << vertex[i].getVertex();
	}
	
	cout << endl;
	for (int i = 0; i < numVertices; i++)
	{
		cout << left << setw(15) << vertex[i].getVertex();
		for (int j = 0; j < numVertices; j++)
		{
			cout << setw(15);
			if (weight[i][j] > 0)
			{
				cout << weight[i][j];
			}
			else
			{
				cout << "---";
			}
		}
		cout << endl;
	}
	
	findVertexColoring();
}

// Function: setVertexDegrees()
// Purpose: set the degree of each vertex that is read in
// Incoming: none
// Outgoing: none
// Return: none
void WeightedGraph::setVertexDegrees()
{
	int count;

	for (int i = 0; i < numVertices; i++)
	{
		count = 0;
		for (int j = 0; j < numVertices; j++)
		{
			if(weight[i][j] > 0)
			{
				count++;
			}
		}
		vertex[i].setDegree(count);
	}
}

// Function: findVertexColoring()
// Purpose: find the color of each vertex using the greedy coloring algorithm
// Incoming: none
// Outgoing: none
// Return: none
void WeightedGraph::findVertexColoring()
{
    int pick[numVertices], value, spot;
    bool check[numVertices];

    for (int i = 0; i < numVertices; i++)
    {
        pick[i] = numVertices + 1;
    }
        
    for (int i = 0; i < numVertices; i++)
    {
        value = 0;
        spot = 0;
        
        for (int w = 0; w < numVertices; w++)
		{
            if ((spot < vertex[w].getDegree()) && pick[w] == (numVertices + 1))
			{
	            value = w;
	            spot = vertex[w].getDegree();
            }
        }

        for (int x = 0; x < numVertices; x++)
        {
            check[x] = true;
        }

        for (int y = 0; y < numVertices; y++)
		{
            if (weight[value][y] > 0)
			{
                check[pick[y]] = false;
            }
        }
        
        for (int z = 0; z < numVertices; z++)
		{
            if (check[z] == true)
			{
                pick[value] = z;
                break;
            }
        }
    }

  	cout << endl << "Colors:" << endl;
    for (int i = 0; i < numVertices; i++)
    {
        cout << left << setw(12) << vertex[i].getVertex() << left << setw(10) << COLOR[pick[i]] << endl;
    }
}

// Function: shortestPaths()
// Purpose: find the shortest path of a given graph
// Incoming: first - int
// Outgoing: none
// Return: none
void WeightedGraph::shortestPaths (int first) const
{
	int numToBeChecked = numVertices, u = 0, v = 0, lowDist = 0, dest = 0;
	int *pred= new int[numVertices];
	int *currDist = new int[numVertices];
	bool *toBeChecked= new bool[numVertices];
	stack <int> st;
	
	cout << "Finding shortest path from vertex #" << first << " to all other vertices" << endl;

	for (int i = 0; i < numVertices; i++)
	{
		pred[i] = i;
	}

	for (int i = 0; i < numVertices; i++)
	{
		currDist[i] = INT_MAX;
	}
	currDist[first] = 0;

	for (int i = 0; i < numVertices; i++)
	{
		toBeChecked[i] = true;
	}

	while (numToBeChecked > 0)
	{
	 	lowDist = INT_MAX;
	 	for (int i = 0; i < numVertices; i++)
	 	{
	   		if (toBeChecked[i] && (currDist[i] < lowDist))
		    {
			    lowDist = currDist[i];
			    v = i;
			}
		}
		
	    toBeChecked[v] = false;
	    numToBeChecked--;

	  	for (u = 0; u < numVertices; u++)
	  	{
	    	if (weight[v][u] != 0 && toBeChecked[u])
	     	{
	      		if (currDist[u] > currDist[v] + weight[v][u])
	    		{
	      			currDist[u] = currDist[v] + weight[v][u];
	      			pred[u] = v;
	    		}
	     	}
	 	}
	}

	cout << endl << "Dest        Cost     Path" << endl;
	
	for (int i = 0; i < numVertices; i++)
	{
		cout << setw(12) << vertex[i].getVertex() << setw(4) << setw(7) << currDist[i] << "  ";
	 	dest = i;
	 	do
	  	{
	   		st.push(dest);
	   		dest = pred[dest];
	  	}
		while (dest != first);
		
	  	st.push(dest);

	 	while (!st.empty())
	 	{
	   		cout <<  vertex[st.top()].getVertex();
	   		st.pop();
	   		if (!st.empty())
	   		{
	     		cout << " -> ";
	     	}
	 	}
		cout << endl;
	}
}

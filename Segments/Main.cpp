#include "Algorithms.h"
#include "Tree23.h"
using namespace std;



int main()
{
	int size = 50;
	int remove = 49;
	Segment *segments = GetRandomSegments(size, 0, 100000);

	Tree23 tree;

	for (int i = 0; i < size; i++)
	{
		cout << "Insert: " << segments[i].start.y << endl;
		tree.Insert(segments[i],segments[i].start);
		
	}

	for (int i = 0; i < remove; i++)
	{
		cout << "Remove: " << segments[i].start.y << endl;
		tree.Remove(segments[i].start);
	}
	/*
	for (int i = 0; i < size; i++)
	{
		cout << "Insert: " << segments[i].start.y << endl;
		tree.Insert(segments[i], segments[i].start);

	}
	*/


	cout << endl;

	if (!tree.IsEmpty())
	{
		Segment current = tree.GetMax();
		cout << current.start.y << endl;
		while (tree.TryGetPrevious(current.start, current))
		{
			cout << current.start.y << endl;
		}
	}
	

	
	

}


#include "Algorithms.h"
#include "Tree23.h"
using namespace std;



int main()
{
	setlocale(LC_ALL, "ru");
	
	int size = 5000, time = 0;
	Segment a, b;

	//Segment* segments = GenRandomSegments(size, -100000, 100000); // Случайные отрезки
	//Segment* segments = GenParallelSegments1(size); // Отрезки расставлены лесенкой
	Segment* segments = GenParallelSegments2(size); // Отрезки, начало которых находится в точке с х = 0


	cout << "Наивный алгоритм: " << endl;
	if (NaiveAlgorithm(segments, size, a, b, time))
	{
		
		cout << "Пересечение отрезков: (" << a.start.x << ", " << a.start.y << ") (" << a.end.x << ", " << a.end.y << ") ; "
			<< "(" << b.start.x << ", " << b.start.y << ") (" << b.end.x << ", " << b.end.y << ");" << endl;
		cout << "Time: " << time << endl;
	}
	else 
	{
		cout << "Пересечений не обнаружено. " << endl;
		cout << "Time: " << time << endl;
	}

	cout << "Алгоритм заметающей прямой на 2-3 дереве: " << endl;
	if (Tree23Algorithm(segments, size, a, b, time))
	{
		
		cout << "Пересечение отрезков: (" << a.start.x << ", " << a.start.y << ") (" << a.end.x << ", " << a.end.y << ") ; "
			<< "(" << b.start.x << ", " << b.start.y << ") (" << b.end.x << ", " << b.end.y << ");" << endl;
		cout << "Time: " << time << endl;
	}
	else
	{
		cout << "Пересечений не обнаружено. " << endl;
		cout << "Time: " << time << endl;
	}
	
	


	/* Проверка работы 2-3 дерева
	Tree23 tree; int size = 100000; int time = 0;
	Segment* segments = GenRandomSegments(size, -10000000, 10000000);

	for (int i = 0; i < size; i++)
		tree.Insert(segments[i], segments[i].start, time);

	for (int i = 0; i < size; i++)
		tree.Remove(segments[i].start, time);

	if (tree.IsEmpty()) cout << "OK!";
	*/



}


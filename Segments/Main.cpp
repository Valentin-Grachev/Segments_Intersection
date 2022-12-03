#include "Algorithms.h"
#include "Tree23.h"
using namespace std;



int main()
{
	setlocale(LC_ALL, "ru");
	
	int size = 5000, time = 0;
	Segment a, b;

	//Segment* segments = GenRandomSegments(size, -100000, 100000); // ��������� �������
	//Segment* segments = GenParallelSegments1(size); // ������� ����������� ��������
	Segment* segments = GenParallelSegments2(size); // �������, ������ ������� ��������� � ����� � � = 0


	cout << "������� ��������: " << endl;
	if (NaiveAlgorithm(segments, size, a, b, time))
	{
		
		cout << "����������� ��������: (" << a.start.x << ", " << a.start.y << ") (" << a.end.x << ", " << a.end.y << ") ; "
			<< "(" << b.start.x << ", " << b.start.y << ") (" << b.end.x << ", " << b.end.y << ");" << endl;
		cout << "Time: " << time << endl;
	}
	else 
	{
		cout << "����������� �� ����������. " << endl;
		cout << "Time: " << time << endl;
	}

	cout << "�������� ���������� ������ �� 2-3 ������: " << endl;
	if (Tree23Algorithm(segments, size, a, b, time))
	{
		
		cout << "����������� ��������: (" << a.start.x << ", " << a.start.y << ") (" << a.end.x << ", " << a.end.y << ") ; "
			<< "(" << b.start.x << ", " << b.start.y << ") (" << b.end.x << ", " << b.end.y << ");" << endl;
		cout << "Time: " << time << endl;
	}
	else
	{
		cout << "����������� �� ����������. " << endl;
		cout << "Time: " << time << endl;
	}
	
	


	/* �������� ������ 2-3 ������
	Tree23 tree; int size = 100000; int time = 0;
	Segment* segments = GenRandomSegments(size, -10000000, 10000000);

	for (int i = 0; i < size; i++)
		tree.Insert(segments[i], segments[i].start, time);

	for (int i = 0; i < size; i++)
		tree.Remove(segments[i].start, time);

	if (tree.IsEmpty()) cout << "OK!";
	*/



}


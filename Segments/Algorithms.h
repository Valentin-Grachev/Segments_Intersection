#pragma once
#include "Segments.h"
#include <algorithm>
#include "Tree23.h"
#include <map>

using namespace std;



// ������� �������� ������ ���� �������������� ��������
// - ���������� true, ���� ����������� ����, � ������ ���������� �������� �������������� ��������
bool NaiveAlgorithm(const Segment *segments, int size, Segment& a, Segment& b, int &time)
{
	time = 0;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = i+1; j < size; j++)
		{
			time++;
			if (CheckIntersection(segments[i], segments[j]) == true)
			{
				a = segments[i];
				b = segments[j];
				return true;
			}
		}
	}

}




// �������� ������ ���� �������������� �������� ������� ���������� ������ � �������������� 2-3 ������
// - ���������� true, ���� ����������� ����, � ������ ���������� �������� �������������� ��������
bool Tree23Algorithm(Segment* segments, int size, Segment& a, Segment& b, int &time)
{
	time = 0;
	Tree23 tree;

	// ��������� ������� ������� ������ � ����� �������� (� ���� ��� ������ ����������� �� � �����-�������)
	map<PointX, Segment> _map;

	for (int i = 0; i < size; i++)
	{
		pair<PointX, Segment> _startSegm;
		_startSegm.first = segments[i].start;
		_startSegm.second = segments[i];

		pair<PointX, Segment> _endSegm;
		_endSegm.first = segments[i].end;
		_endSegm.second = segments[i];

		_map.insert(_startSegm);
		_map.insert(_endSegm);
	}

	for (auto iter = _map.begin(); iter != _map.end(); iter++)
	{
		bool isStartSegm = true;
		PointX point = iter->first;
		Point key(point);
		Segment checkableSegm = iter->second;
		Segment prevSegm, nextSegm;

		if (checkableSegm.end == key) isStartSegm = false;


		if (isStartSegm) // ���� ����� �������� ������� �������
		{
			// ��������� ����� ������� � ������
			tree.Insert(checkableSegm, key);

			// ��������� �� ����������� ������ ������� � ������� � ������
			if (tree.TryGetPrevious(key, prevSegm) && tree.TryGetNext(key, nextSegm))
			{
				if (CheckIntersection(checkableSegm, prevSegm))
				{
					time++;
					a = checkableSegm;
					b = prevSegm;
					return true;
					
				}
				else if (CheckIntersection(checkableSegm, nextSegm))
				{
					time++;
					a = checkableSegm;
					b = nextSegm;
					return true;
				}
			}
		}

		else // ���� ����� �� �������� ������� �������
		{
			// ��������� �� ����������� ������� � ������ �������
			if (tree.TryGetPrevious(key, prevSegm) && tree.TryGetNext(key, nextSegm))
			{
				if (CheckIntersection(prevSegm, nextSegm))
				{
					time++;
					a = prevSegm;
					b = nextSegm;
					return true;
				}
			}

			// ������� ������� �� ������
			tree.Remove(key);
		}

	}



}





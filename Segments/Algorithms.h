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

	return false;
}




// �������� ������ ���� �������������� �������� ������� ���������� ������ � �������������� 2-3 ������
// - ���������� true, ���� ����������� ����, � ������ ���������� �������� �������������� ��������
bool Tree23Algorithm(const Segment* segments, int size, Segment& a, Segment& b, int &time)
{
	time = 0;
	Tree23 tree;

	// ��������� ������� ������� ������ � ����� �������� (� ���� ��� ������ ����������� �� � �����-�������)
	map<PointX, Segment> _map;

	for (int i = 0; i < size; i++)
	{
		pair<PointX, Segment> _startSegm;
		_startSegm.first = ToPointX(segments[i].start);
		_startSegm.second = segments[i];

		pair<PointX, Segment> _endSegm;
		_endSegm.first = ToPointX(segments[i].end);
		_endSegm.second = segments[i];

		auto iter = _map.find(_startSegm.first);
		if (iter != _map.end())
		{
			a = iter->second;
			b = _startSegm.second;
			return true;
		}
		_map.insert(_startSegm);

		iter = _map.find(_endSegm.first);
		if (iter != _map.end())
		{
			a = iter->second;
			b = _endSegm.second;
			return true;
		}
		_map.insert(_endSegm);
	}

	for (auto iter = _map.begin(); iter != _map.end(); iter++)
	{
		bool isStartSegm = true;
		PointX point = iter->first;
		Point key = ToPoint(point);
		Segment checkableSegm = iter->second;
		Segment prevSegm, nextSegm;

		if (checkableSegm.end == key) isStartSegm = false;


		if (isStartSegm) // ���� ����� �������� ������� �������
		{
			// ���� � ������ ��� ���� �������� ����, �� ��� ����������� 
			// - ������, ����� ��� ������� ������� �� ����� �����
			if (tree.TryFind(key, prevSegm, time))
			{
				a = checkableSegm;
				b = prevSegm;
				return true;
			}


			// ��������� ����� ������� � ������
			tree.Insert(checkableSegm, key, time);
			

			// ��������� �� ����������� ������ ������� � ������
			if (tree.TryGetPrevious(key, prevSegm, time))
			{
				time++;
				if (CheckIntersection(checkableSegm, prevSegm))
				{
					a = checkableSegm;
					b = prevSegm;
					return true;
				}
			}


			// ��������� �� ����������� ������ ������� � �������
			if (tree.TryGetNext(key, nextSegm, time))
			{
				time++;
				if (CheckIntersection(checkableSegm, nextSegm))
				{
					a = checkableSegm;
					b = nextSegm;
					return true;
				}
				
			}
		}

		else // ���� ����� �� �������� ������� �������
		{
			// ��������� �� ����������� ������� � ������ �������
			if (tree.TryGetPrevious(key, prevSegm, time) && tree.TryGetNext(key, nextSegm, time))
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
			tree.Remove(key, time);
		}

	}


	return false;
}





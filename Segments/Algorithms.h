#pragma once
#include "Segments.h"
#include <algorithm>
#include "Tree23.h"
#include <map>

using namespace std;



// Ќаивный алгоритм поиска пары пересекающихс€ отрезков
// - возвращает true, если пересечение есть, в ссылки записывает значени€ пересекающихс€ отрезков
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




// јлгоритм поиска пары пересекающихс€ отрезков методом заметающей пр€мой с использованием 2-3 дерева
// - возвращает true, если пересечение есть, в ссылки записывает значени€ пересекающихс€ отрезков
bool Tree23Algorithm(Segment* segments, int size, Segment& a, Segment& b, int &time)
{
	time = 0;
	Tree23 tree;

	// «аполн€ем словарь точками начала и конца отрезков (¬ мапе они станут упор€дочены по х слева-направо)
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


		if (isStartSegm) // ≈сли точка €вл€етс€ началом отрезка
		{
			// ¬ставл€ем новый отрезок в дерево
			tree.Insert(checkableSegm, key);

			// ѕровер€ем на пересечение нового отрезка с верхним и нижним
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

		else // ≈сли точка не €вл€етс€ началом отрезка
		{
			// ѕровер€ем на пересечение верхний и нижний отрезки
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

			// ”дал€ем отрезок из дерева
			tree.Remove(key);
		}

	}



}





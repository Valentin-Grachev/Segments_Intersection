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

	return false;
}




// јлгоритм поиска пары пересекающихс€ отрезков методом заметающей пр€мой с использованием 2-3 дерева
// - возвращает true, если пересечение есть, в ссылки записывает значени€ пересекающихс€ отрезков
bool Tree23Algorithm(const Segment* segments, int size, Segment& a, Segment& b, int &time)
{
	time = 0;
	Tree23 tree;

	// «аполн€ем словарь точками начала и конца отрезков (¬ мапе они станут упор€дочены по х слева-направо)
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


		if (isStartSegm) // ≈сли точка €вл€етс€ началом отрезка
		{
			// ≈сли в дереве уже есть заданный ключ, то это пересечение 
			// - случай, когда два отрезка исход€т из одной точки
			if (tree.TryFind(key, prevSegm, time))
			{
				a = checkableSegm;
				b = prevSegm;
				return true;
			}


			// ¬ставл€ем новый отрезок в дерево
			tree.Insert(checkableSegm, key, time);
			

			// ѕровер€ем на пересечение нового отрезка с нижним
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


			// ѕровер€ем на пересечение нового отрезка с верхним
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

		else // ≈сли точка не €вл€етс€ началом отрезка
		{
			// ѕровер€ем на пересечение верхний и нижний отрезки
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

			// ”дал€ем отрезок из дерева
			tree.Remove(key, time);
		}

	}


	return false;
}





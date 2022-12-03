#pragma once
#include <iostream>

// —труктура точек, сравнение которых идет приоритетно по ординате, затем по обсциссе
struct Point
{
	double x;
	double y;

	

	bool operator<(Point a) {
		if (this->y != a.y) return this->y < a.y;
		else return x < a.x;
	}

	bool operator<=(Point a) {
		if (this->y != a.y) return this->y <= a.y;
		else return x <= a.x;
	}

	bool operator>(Point a) {
		if (this->y != a.y) return this->y > a.y;
		else return x > a.x;
	}

	bool operator==(Point a) {
		return x == a.x && y == a.y;
	}




};

// —труктура точек, сравнение которых идет приоритетно по абсциссе, затем по ординате
struct PointX
{
	double x;
	double y;



	bool operator<(const PointX &a) const {
		if (this->x != a.x) return this->x < a.x;
		else return y < a.y;
	}

	bool operator<=(const PointX &a) const {
		if (this->x != a.x) return this->x <= a.x;
		else return y <= a.y;
	}

	bool operator>(const PointX &a) const {
		if (this->x != a.x) return this->x > a.x;
		else return y > a.y;
	}

	bool operator==(const PointX &a) const {
		return x == a.x && y == a.y;
	}


};

PointX ToPointX(Point point)
{
	PointX res;
	res.x = point.x;
	res.y = point.y;
	return res;
}

Point ToPoint(PointX point)
{
	Point res;
	res.x = point.x;
	res.y = point.y;
	return res;
}

struct Segment
{
	Point start;
	Point end;

	Segment() = default;

	Segment(Point a, Point b)
	{
		if (a.x <= b.x)
		{
			start = a;
			end = b;
		}
		else
		{
			start = b;
			end = a;
		}
	}
};

bool CompareSegmentsByX(Segment a, Segment b)
{
	return a.start.x < b.start.x;
}



// ¬вод данных вручную
Segment* EnterSegments(int& size)
{
	int n;
	std::cout << "Enter segments quantity: ";
	std::cin >> n;
	Segment* segments = new Segment[n];

	std::cout << "Enter segment coordinates (x1,y1) (x2,y2):" << std::endl;
	for (size_t i = 0; i < n; i++)
	{
		Point a, b;
		std::cin >> a.x >> a.y >> b.x >> b.y;
		segments[i].start = a;
		segments[i].end = b;
	}

	size = n;
	return segments;
}

Segment* GenRandomSegments(int size, int min, int max)
{
	srand(time(NULL));
	Segment* segments = new Segment[size];

	for (int i = 0; i < size; i++)
	{
		double random1 = min + rand()%(max-min);
		double random2 = min + rand() % (max - min);
		double random3 = min + rand() % (max - min);
		double random4 = min + rand() % (max - min);
		Point a, b;
		a.x = random1; a.y = random2;
		b.x = random3; b.y = random4;
		Segment segment(a, b);
		segments[i] = segment;
	}

	return segments;
}

Segment* GenParallelSegments1(int size) // ќтрезки лестницей
{
	int segmLength = 10;
	srand(time(NULL));
	Segment* segments = new Segment[size];

	for (int i = 0; i < size; i++)
	{
		Point a, b;
		a.x = i;
		a.y = i;
		b.x = i + segmLength;
		b.y = i;
		Segment segment(a, b);
		segments[i] = segment;
	}

	return segments;
}

Segment* GenParallelSegments2(int size) // ќтрезки имеют одинаковую абсциссу точки начала
{
	srand(time(NULL));
	Segment* segments = new Segment[size];

	for (int i = 0; i < size; i++)
	{
		Point a, b;
		a.x = 0;
		a.y = i;
		b.x = 1 + i;
		b.y = 1 + i;
		Segment segment(a, b);
		segments[i] = segment;
	}

	return segments;
}

// ¬ывести на экран отрезки
void ShowSegments(const Segment* segments, int size)
{
	std::cout << "Segments:" << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		std::cout << i << ": " << "(" << segments[i].start.x << "," << segments[i].start.y << ") ("
			<< segments[i].end.x << "," << segments[i].end.y << ")" << std::endl;
	}
}

void ShowSegmentsPlot(const Segment* segments, int size)
{

}



Segment ToVector(Segment s)
{
	Segment vector;
	vector.start.x = 0; vector.start.y = 0;
	vector.end.x = s.end.x - s.start.x;
	vector.end.y = s.end.y - s.start.y;
	return vector;
}

// ѕроверка на то, лежат ли точки провер€емого отрезка по разные полуслоскоти образуемой секущим отрезком
bool PointsInDifferentHalfPlanes(Segment secant, Segment checkable)
{
	// ќткладываем два новых вектора - от начала секущего отрезка в концы провер€емого отрезка
	Segment v1; v1.start = secant.start; v1.end = checkable.start; v1 = ToVector(v1);
	Segment v2; v2.start = secant.start; v2.end = checkable.end; v2 = ToVector(v2);
	Segment secantVector = ToVector(secant);

	// ≈сли точки провер€емого отрезка лежат по разные полуплоскости относительно секущего отрезка,
	// то векторные произведени€ между секущим вестором и созданными векторами должны иметь разный знак у координаты z
	double vectorProduct1Z = secantVector.end.x * v1.end.y - secantVector.end.y * v1.end.x;
	double vectorProduct2Z = secantVector.end.x * v2.end.y - secantVector.end.y * v2.end.x;
	if (vectorProduct1Z == 0 || vectorProduct2Z == 0)
	{
		// ’от€ бы одна из точек отрезка совпала с секущим - возращаем true
		return true;
	}
	if (vectorProduct1Z / vectorProduct2Z < 0) return true;
	return false;
}

// ѕроверка на пересечение двух отрезков
bool CheckIntersection(Segment s1, Segment s2)
{
	if (PointsInDifferentHalfPlanes(s1, s2) &&
		PointsInDifferentHalfPlanes(s2, s1)) return true;
	return false;
}
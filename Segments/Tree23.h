#pragma once
#include "Segments.h"

struct Node
{
	Point key1;
	Point key2;
	Point max;
	int childCount;
	Segment value;

	Node* p1;
	Node* p2;
	Node* p3;
	Node* p4; // Дополнительный указатель на узел, нужен для разделения

	Node* pParent;

	Node()
	{
		childCount = 0;
		p1 = nullptr; p2 = nullptr; p3 = nullptr; p4 = nullptr;  pParent = nullptr;
	}


	void AttachNode(Node* node) // Прикрепить другой узел к этому узлу
	{
		if (p1 == nullptr) p1 = node;
		else if (p2 == nullptr) p2 = node;
		else if (p3 == nullptr) p3 = node;
		else if (p4 == nullptr) p4 = node;
		node->pParent = this;
		childCount++;
		RestoreState();
		if (pParent != nullptr) pParent->RestoreState();
	}

	void DetachNode(Node* node) // Открепить от этого узла другой узел
	{
		if (p1 == node) p1 = nullptr;
		else if (p2 == node) p2 = nullptr;
		else if (p3 == node) p3 = nullptr;
		else if (p4 == node) p4 = nullptr;
		delete node;
		childCount--;
		RestoreState();
		if (pParent != nullptr) pParent->RestoreState();
	}

	int GetPointerNumber()
	{
		if (this == pParent->p1) return 1;
		else if (this == pParent->p2) return 2;
		else return 3;
	}

	Node* GetPointerByNumber(int number)
	{
		if (number == 1) return p1;
		else if (number == 2) return p2;
		else if (number == 3) return p3;
		else if (number == 4) return p4;
		else return nullptr;
	}
	

private:

	void SwapChild(Node*& a, Node*& b)
	{
		Node* temp = b;
		b = a;
		a = temp;
	}

	void RestoreState()
	{
		// "Утрамбовываем" указатели влево
		if (p1 == nullptr) { p1 = p2; p2 = p3; p3 = p4; }
		else if (p2 == nullptr) { p2 = p3; p3 = p4; }
		else if (p3 == nullptr) p3 = p4;

		// Сортируем детей в порядке возрастания
		if (childCount == 2)
		{
			if (p1->max > p2->max) SwapChild(p1, p2);
			key1 = p1->max;
			max = p2->max;
		}
		else if (childCount == 3)
		{
			if (p1->max > p2->max) SwapChild(p1, p2);
			if (p2->max > p3->max) SwapChild(p2, p3);
			if (p1->max > p2->max) SwapChild(p1, p2);
			key1 = p1->max;
			key2 = p2->max;
			max = p3->max;
		}
		else if (childCount == 4) // Служебные поля в вершине с 4-мя детьми не нужны, так как она будет удалена
		{
			if (p1->max > p2->max) SwapChild(p1, p2);
			if (p2->max > p3->max) SwapChild(p2, p3);
			if (p3->max > p4->max) SwapChild(p3, p4);

			if (p1->max > p2->max) SwapChild(p1, p2);
			if (p2->max > p3->max) SwapChild(p2, p3);

			if (p1->max > p2->max) SwapChild(p1, p2);
		}
	}
};


class Tree23
{
private:
	Node* root;

	void Split(Node* splitableNode);// Разделить вершину, имеющую 4 потомка
	void Merge(Node* mergableNode); // Слить указанную вершину с соседней
	Node* GetLeaf(Point key);		// Получить вершину, ближайшую к заданному ключу

public:

	Tree23();

	bool IsEmpty() { return root == nullptr; }
	void Insert(Segment segment, Point key);
	void Remove(Point key);
	bool TryFind(Point key, Segment &result);

	Segment GetMin();
	Segment GetMax();
	bool TryGetNext(Point relativeToKey, Segment &result); // Получить отрезок, находящийся выше заданного
	bool TryGetPrevious(Point relativeToKey, Segment &result); // Получить отрезок, находящийся ниже заданного




};



inline void Tree23::Split(Node* splitableNode)
{
	if (splitableNode->childCount != 4) throw "Split(): Wrong conditions!";

	Node* p1, * p2, * p3, * p4, *pParent;
	p1 = splitableNode->p1;
	p2 = splitableNode->p2;
	p3 = splitableNode->p3;
	p4 = splitableNode->p4;
	pParent = splitableNode->pParent;
	if (pParent != nullptr) pParent->DetachNode(splitableNode);

	Node* newLeftNode = new Node();
	newLeftNode->AttachNode(p1);
	newLeftNode->AttachNode(p2);

	Node* newRightNode = new Node();
	newRightNode->AttachNode(p3);
	newRightNode->AttachNode(p4);

	if (pParent == nullptr) // Если разделяем корень - создаем новый узел, который станет новым корнем
	{
		pParent = new Node(); 
		root = pParent;
	}
	pParent->AttachNode(newLeftNode);
	pParent->AttachNode(newRightNode);
	if (pParent->childCount == 4) Split(pParent);
}

inline void Tree23::Merge(Node* mergableNode)
{
	Node* parent = mergableNode->pParent;
	Node* secondMergableNode;

	// Если узел для сливания оказался самым левым - то его соседом станет тот, что правее него
	if (mergableNode->GetPointerNumber() == parent->childCount)
		secondMergableNode = parent->GetPointerByNumber(parent->childCount - 1);

	// Иначе - сосед тот, что слева
	else secondMergableNode = parent->GetPointerByNumber(mergableNode->GetPointerNumber()+1);

	Node* p1 = mergableNode->p1;
	parent->DetachNode(mergableNode);
	secondMergableNode->AttachNode(p1);
	if (secondMergableNode->childCount == 4) Split(secondMergableNode);
	if (parent->childCount == 1 && parent != root) Merge(parent);
	else if (parent == root && root->childCount == 1)
	{
		Node* prevRoot = root;
		root = root->p1;
		delete prevRoot;
	}


}


inline Tree23::Tree23()
{
	root = nullptr;
}

inline Node* Tree23::GetLeaf(Point key)
{
	if (root == nullptr) return nullptr;

	Node* current = root;
	while (current->childCount == 2 || current->childCount == 3)
	{

		if (current->childCount == 2)
		{
			if (key <= current->key1) current = current->p1;
			else current = current->p2;
		}

		else if (current->childCount == 3)
		{
			if (key <= current->key1) current = current->p1;
			else if (key <= current->key2) current = current->p2;
			else current = current->p3;
		}

	}
	return current;
}

inline void Tree23::Insert(Segment segment, Point key)
{
	// Создали новый лист со значением
	Node* newLeaf = new Node();
	newLeaf->key1 = key;
	newLeaf->max = key;
	newLeaf->value = segment;
	newLeaf->childCount = 0;

	Node* leafForInsert = GetLeaf(key);

	// Если дерево пустое
	if (root == nullptr) root = newLeaf;

	// Если дерево имеет всего один лист
	else if (root->childCount == 0)
	{
		// Создаем узел - будет новым корнем
		Node* newRoot = new Node();
		newRoot->AttachNode(root);
		newRoot->AttachNode(newLeaf);
		root = newRoot;
	}

	else // Дерево имеет более одного листа
	{
		// Прикреплять новый узел будем к родителю найденного узла
		leafForInsert->pParent->AttachNode(newLeaf);
		if (leafForInsert->pParent->childCount == 4) Split(leafForInsert->pParent);
	}
}

inline void Tree23::Remove(Point key)
{
	if (root->childCount == 0)
	{
		delete root;
		root = nullptr;
		return;
	}


	Node* res = GetLeaf(key);
	Node* parent = res->pParent;
	parent->DetachNode(res);
	if (parent->childCount == 1 && parent != root) Merge(parent);
	else if (parent->childCount == 1 && parent == root)
	{
		Node* prevRoot = root;
		root = root->p1;
		delete prevRoot;
	}
}

inline bool Tree23::TryFind(Point key, Segment& result)
{
	Node *res = GetLeaf(key);
	if (res->key1 == key) {
		result = res->value;
		return true;
	} 
	else return false;
}



inline Segment Tree23::GetMin()
{
	Node* current = root;
	while (current->childCount > 0) current = current->p1;
	return current->value;
}

inline Segment Tree23::GetMax()
{
	Node* current = root;
	while (current->GetPointerByNumber(current->childCount) != nullptr)
		current = current->GetPointerByNumber(current->childCount);
	return current->value;
}

inline bool Tree23::TryGetNext(Point relativeToKey, Segment& result)
{
	if (root == nullptr || root->childCount == 0) return false;
	Node* currentNode = GetLeaf(relativeToKey);
	int previousPointerNumber = currentNode->GetPointerNumber();
	currentNode = currentNode->pParent;

	// Если уперлись вправо - поднимаемся выше по дереву
	while (currentNode->GetPointerByNumber(previousPointerNumber+1) == nullptr)
	{
		if (currentNode == root) return false; // Уперлись вправо в корне - следующего элемента нет
		previousPointerNumber = currentNode->GetPointerNumber();
		currentNode = currentNode->pParent;
	}

	// Направо есть проход - идем в него
	currentNode = currentNode->GetPointerByNumber(previousPointerNumber + 1);

	// Далее идем в самый низ по самым левым связям
	while (currentNode->p1 != nullptr)
		currentNode = currentNode->p1;

	result = currentNode->value;
	return true;
}

inline bool Tree23::TryGetPrevious(Point relativeToKey, Segment& result)
{
	if (root == nullptr || root->childCount == 0) return false;
	Node* currentNode = GetLeaf(relativeToKey);
	int previousPointerNumber = currentNode->GetPointerNumber();
	currentNode = currentNode->pParent;

	// Если уперлись влево - поднимаемся выше по дереву
	while (currentNode->GetPointerByNumber(previousPointerNumber - 1) == nullptr)
	{
		if (currentNode == root) return false; // Уперлись влево в корне - предыдущего элемента нет
		previousPointerNumber = currentNode->GetPointerNumber();
		currentNode = currentNode->pParent;
	}

	// Налево есть проход - идем в него
	currentNode = currentNode->GetPointerByNumber(previousPointerNumber - 1);

	// Далее идем в самый низ по самым правым связям
	while (currentNode->p1 != nullptr)
		currentNode = currentNode->GetPointerByNumber(currentNode->childCount);

	result = currentNode->value;
	return true;
}

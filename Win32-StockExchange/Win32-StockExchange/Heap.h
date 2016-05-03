#pragma once

struct Company {
	char name[20];
	int income;
	float variation;
};

class Heap {
private:
	Company *heap;
	int heapDimension = 0;

public:
	Heap();
	Heap(Company arr[], int heapDim);
	~Heap();
	int Father(int index);
	int Child1(int index);
	int Child2(int index);
	void PushUp(int startIndex);
	void PushDown(int startIndex);
	Company Peek(int sum);
	void Push(Company Company);
	void Pop(int index);
	void Display(int index);
	int Get();
	void RandomizeVary(int i);
	void Save();
	void Load();
	int Search(Company searchElement, int startIndex);
	Company ReturnCompany(int index);
};


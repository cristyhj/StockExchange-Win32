#include "stdafx.h"
#include "Heap.h"


void Swap(Company& a, Company& b) {
	Company aux = a;
	a = b;
	b = aux;
}

Heap::Heap() {
	heapDimension = 0;
	heap = nullptr;
	cout << "Heap created - allocated 00 int memory!\n";
}

Heap::Heap(Company arr[], int heapDim) {
	heap = nullptr;
	for (int i = 0; i < heapDim; i++) {
		Push(arr[i]);
	}
	cout << "Heap created - allocated " << heapDim << " int memory!\n";
}

Heap::~Heap() {
	free(heap);
	cout << "\nHeap deleted.\n";
}

// return index-ul parinte al nodului
int Heap::Father(int index) {
	return (index - 1) / 2;
}
// return index-ul copil stanga
int Heap::Child1(int index) {
	return index * 2 + 1;
}
// return index-ul copil stanga
int Heap::Child2(int index) {
	return index * 2 + 2;
}

// Opertia de PushUp.. Ordoneaza graful mergand in sus pana cand are din nou
// proprietatea de heap
void Heap::PushUp(int startIndex) {
	if (startIndex) {
		int fatherIndex = Father(startIndex);
		if (heap[startIndex].variation > heap[fatherIndex].variation) {
			Swap(heap[startIndex], heap[fatherIndex]);
			PushUp(fatherIndex);
		}
	}
}

// Opertia de PushDown.. Ordoneaza graful margand in jos pana cand are din nou
// proprietatea de heap
void Heap::PushDown(int startIndex) {
	int childIndex = Child1(startIndex);
	if (heapDimension - 1 < childIndex)
		return;
	if (heapDimension > childIndex && heap[childIndex].variation < heap[childIndex + 1].variation)
		childIndex++;
	if (heap[startIndex].variation < heap[childIndex].variation) {
		Swap(heap[startIndex], heap[childIndex]);
		PushDown(childIndex);
	}
}

// return valoarea maxima din heap
Company Heap::Peek(int sum) {
	heap[0].income -= sum;
	return heap[0];
}
// Operatia de Push: adaugam in coada si facem PushUp pana cand se ordoneaza
void Heap::Push(Company company) {
	heap = (Company*)realloc(heap, (heapDimension + 1) * sizeof(Company));
	heap[heapDimension] = company;
	PushUp(heapDimension++);
}
// Scoate elementul dupa index. 
//		- 0 pentru maxim
void Heap::Pop(int index) {
	heap[index] = heap[--heapDimension];
	heap = (Company*)realloc(heap, heapDimension * sizeof(int));
	PushDown(index);
}

void Heap::Display(int index) {
	for (int i = 0; i < heapDimension; i++) {
		cout << heap[i].variation << " ";
	}
}

int Heap::Get() {
	return heapDimension;
}

void Heap::RandomizeVary(int i) {
	int randValue = rand() % 200 - 100;
	float randomValue = (float)randValue / (float)1000;
	heap[i].variation += randomValue;
	if (randomValue < 0)
		PushDown(i);
	else
		PushUp(i);
}

void Heap::Save() {
	//if (heapDimension == 0)
	//	return;
	ofstream file("save.txt");
	/*for (int i = 0; i < heapDimension; i++) {
	file << heap[i].name << " " << heap[i].income << " " << heap[i].variation << "\n";
	}*/
	file.write((char*)&heapDimension, sizeof(int));
	if (heapDimension)
		file.write((char*)heap, heapDimension * sizeof(Company));
	file.close();
}

void Heap::Load() {
	ifstream file("save.txt");
	Company temp;
	/*while (!file.eof()) {
	file >> temp.name >> temp.income >> temp.variation;
	Push(temp);
	}*/
	file.read((char*)&heapDimension, sizeof(int));
	/*if (heapDimension == 48) {
	heapDimension = 0;
	Save();
	return;
	}*/
	if (heapDimension == 0) {
		return;
	}
	heap = (Company*)malloc(heapDimension * sizeof(Company));
	file.read((char*)heap, heapDimension * sizeof(Company));
	file.close();
}

int Heap::Search(Company searchElement, int startIndex) {
	if (startIndex > heapDimension)
		return -1;
	if (strcmp(searchElement.name, heap[startIndex].name) == 0)
		return startIndex;
	int l = Search(searchElement, Child1(startIndex));
	if (l > 0)
		return l;
	int r = Search(searchElement, Child2(startIndex));
	if (r > 0)
		return r;
	else
		return -1;
}

Company Heap::ReturnCompany(int index) {
	if (index < 0) {
		Company Company;
		Company.income = -1;
		return Company;
	} else
		return heap[index];
}

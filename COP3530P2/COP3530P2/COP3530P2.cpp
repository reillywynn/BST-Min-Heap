#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <ctime>
#include <random>


//Stack needs to be increased to 4MB to run
using namespace std;

class BST {
public:
	int val;
	BST* left;
	BST* right;
	BST();
	BST(int x);
};

BST::BST() {
	val = 0;
	left = nullptr;
	right = nullptr;
}

BST::BST(int x) {
	val = x;
	left = nullptr;
	right = nullptr;
}

BST* newNode(int val) {
	BST* temp = new BST;
	temp->val = val;
	temp->left = nullptr;
	temp->right = nullptr;
	return temp;
}


BST* insert(BST* root, int key) {
	if (root == nullptr) {
		return new BST(key);
	}
	if (key < root->val) {
		root->left = insert(root->left, key);
	}
	else {
		root->right = insert(root->right, key);
	}
	return root;
}

BST* minNode(BST* root) {
	while (root->left != nullptr) {
		root = root->left;
	}
	return root;
}

int height(BST* node) {
	if (node == nullptr) { //Empty case
		return 0;
	}
	if (node->left == nullptr && node->right == nullptr) //1 head node case
		return 1;

	int left = 0;
	if (node->left != nullptr)
		left = height(node->left);

	int right = 0;
	if (node->right != nullptr)
		right = height(node->right);

	return (max(left, right) + 1);
}

BST* remove(BST* root, int key) {
	if (root == nullptr) {
		return root;
	}
	
	if (key < root->val) {
		root->left = remove(root->left, key);
	}
	else if (key > root->val) {
		root->right = remove(root->right, key);
	}
	else {
		if (root->left == nullptr && root->right == nullptr) {
			return nullptr;
		}
		else if (root->left == nullptr) {
			BST* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == nullptr) {
			BST* temp = root->left;
			free(root);
			return temp;
		}
		else {
			BST* temp = minNode(root->right);
			root->val = temp->val;
			root->right = remove(root->right, temp->val);
		}
	}
}

void printLevelHelper(BST* root, int level) {
	if (root == nullptr) {
		return;
	}
	if (level == 1) {
		cout << root->val << " ";
	}
	else if (level > 1) {
		printLevelHelper(root->left, level - 1);
		printLevelHelper(root->right, level - 1);
	}
}

void levelOrderPrint(BST* root) {
	int h = height(root);
	for (int i = 1; i <= h; i++) {
		printLevelHelper(root, i);
	}
}

/*----------Min Heap----------*/

class MinHeap {
private:
	int size;
	int capacity;
	int* heapArray;
public:
	MinHeap(int capacity);
	void insert(int k);
	void remove();
	void swap(int* heapArray, int child, int parent);
	int parent(int i) { return (i - 1) / 2; }
	int leftChild(int i) { return (2 * i + 1); }
	int rightChild(int i) { return (2 * i + 2); }
	void heapPrint();
};

MinHeap::MinHeap(int cap) {
	size = 0;
	capacity = cap;
	heapArray = new int[cap];
}

void MinHeap::insert(int k) {
	if (size == capacity) {
		return;
	}

	size++;
	int index = size - 1;
	heapArray[index] = k;

	while (index != 0 && heapArray[parent(index)] > heapArray[index]) {
		swap(heapArray, index, parent(index));
		index = parent(index);
	}
}

void MinHeap::remove() {
	int index = 0;
	heapArray[index] = heapArray[--size];
	while (true) {
		int smaller = index;
		int left = 2 * index + 1;
		int right = 2 * index + 2;
		if (left < size && heapArray[left] < heapArray[smaller]) {
			smaller = left;
		}
		if (right < size && heapArray[right] < heapArray[smaller]) {
			smaller = right;
		}
		if (smaller == index) {
			break;
		}
		else {
			swap(heapArray, smaller, index);
			index = smaller;
		}
	}
}

void MinHeap::swap(int* heapArray, int child, int parent) {
	int temp = heapArray[child];
	heapArray[child] = heapArray[parent];
	heapArray[parent] = temp;
}

void MinHeap::heapPrint() {
	int power = 0;
	int value = 1;
	for (int i = 0; i < size; i++) {
		cout << heapArray[i] << " ";
	}
	cout << endl;
}




int main() {
	BST* root = nullptr;
	MinHeap small(100);
	MinHeap medium(1000);
	MinHeap large(10000);
	vector <int> data;
	mt19937 rng(random_device{}());
	
	/*----------Ascending Small----------*/

	
	ofstream ascending_small ("ascending_small.txt");
	for (int i = 0; i < 100; i++) {
		ascending_small << i << endl;
	}
	ifstream input1("ascending_small.txt");
	string line;
	while (getline(input1, line)) {
		data.push_back(stoi(line));
	}

	cout << "Level Order Traversal: Ascending Small" << endl;
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		if (i == 0) {
			root = insert(root, data[i]);
		}
		else {
			insert(root, data[i]);
		}
	}
	auto end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() <<  " nanoseconds" << endl;

	
	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 99; i >=0; i--) {
		root = remove(root, minNode(root)->val);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order Traversal: Ascending Medium" << endl;


	/*----------Ascending Medium----------*/
	ofstream ascending_med("ascending_med.txt");
	for (int i = 0; i < 1000; i++) {
		ascending_med << i << endl;
	}
	ifstream input2("ascending_med.txt");
	while (getline(input2, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		if (i == 0) {
			root = insert(root, data[i]);
		}
		else {
			insert(root, data[i]);
		}
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 999; i >= 0; i--) {
		root = remove(root, minNode(root)->val);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order Traversal: Ascending Large" << endl;
	

	/*----------Ascending Large----------*/
	ofstream ascending_large("ascending_large.txt");
	for (int i = 0; i < 10000; i++) {
		ascending_large << i << endl;
	}
	ifstream input3("ascending_large.txt");
	while (getline(input3, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		if (i == 0) {
			root = insert(root, data[i]);
		}
		else {
			insert(root, i);
		}
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 9999; i >= 0; i--) {
		root = remove(root, minNode(root)->val);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order traversal: Descending Small" << endl;


	/*----------Descending Small----------*/
	ofstream descending_small("descending_small.txt");
	for (int i = 99; i > -1; i--) {
		descending_small << i << endl;
	}
	ifstream input4("descending_small.txt");
	while (getline(input4, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		
		root = insert(root, data[i]);
		
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		root = remove(root, i);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order traversal: Descending Medium" << endl;


	/*----------Descending Medium----------*/
	ofstream descending_med("descending_med.txt");
	for (int i = 999; i > -1; i--) {
		descending_med << i << endl;
	}
	ifstream input5("descending_med.txt");
	while (getline(input5, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {

		root = insert(root, data[i]);

	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		root = remove(root, i);
	}
	
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order traversal: Descending Large" << endl;


	/*----------Descending Large----------*/
	ofstream descending_large("descending_large.txt");
	for (int i = 9999; i > -1; i--) {
		descending_large << i << endl;
	}
	ifstream input6("descending_large.txt");
	while (getline(input6, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {

		root = insert(root, data[i]);

	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		root = remove(root, i);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Feletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order traversal: Random Small" << endl;

	/*----------Random Small----------*/
	vector<int> randomNumbers;
	for (int i = 0; i < 100; i++) {
		randomNumbers.push_back(i);
	}
	
	shuffle(randomNumbers.begin(), randomNumbers.end(), rng);
	ofstream random_small("random_small.txt");

	for (int i = 0; i < randomNumbers.size(); i++) {
		random_small << randomNumbers[i] << endl;
	}
	
	ifstream input7("random_small.txt");
	
	while (getline(input7, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		root = insert(root, data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		root = remove(root, i);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	randomNumbers.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order traversal: Random Medium" << endl;


	/*----------Random Medium----------*/
	for (int i = 0; i < 1000; i++) {
		randomNumbers.push_back(i);
	}
	
	shuffle(randomNumbers.begin(), randomNumbers.end(), rng);

	ofstream random_med("random_med.txt");

	for (int i = 0; i < randomNumbers.size(); i++) {
		random_med << randomNumbers[i] << endl;
	}

	ifstream input8("random_med.txt");

	while (getline(input8, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		root = insert(root, data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		root = remove(root, i);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	randomNumbers.clear();
	cout << endl;
	cout << endl;
	cout << "Level Order traversal: Random Large" << endl;


	/*----------Random Large----------*/
	for (int i = 0; i < 10000; i++) {
		randomNumbers.push_back(i);
	}
	shuffle(randomNumbers.begin(), randomNumbers.end(), rng);

	ofstream random_large("random_large.txt");

	for (int i = 0; i < randomNumbers.size(); i++) {
		random_large << randomNumbers[i] << endl;
	}

	ifstream input9("random_large.txt");

	while (getline(input9, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		root = insert(root, data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	levelOrderPrint(root);
	cout << endl;
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		root = remove(root, i);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	randomNumbers.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Ascending Small" << endl;


	ifstream input10("ascending_small.txt");
	while (getline(input10, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		small.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl; 

	start = chrono::high_resolution_clock::now();
	small.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		small.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Ascending Medium" << endl;

	ifstream input11("ascending_med.txt");
	while (getline(input11, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		medium.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	medium.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		medium.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;
	

	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Ascending Large" << endl;

	ifstream input12("ascending_large.txt");
	while (getline(input12, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		large.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	large.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		large.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Descending Small" << endl;

	ifstream input13("descending_small.txt");
	while (getline(input13, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		small.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	small.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		small.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Descending Medium" << endl;

	ifstream input14("descending_med.txt");
	while (getline(input14, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		medium.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	medium.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		medium.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Descending Large" << endl;

	ifstream input15("descending_large.txt");
	while (getline(input15, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		large.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	large .heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		large.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Random Small" << endl;

	ifstream input16("random_small.txt");
	while (getline(input16, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		small.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	small.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		small.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Random Medium" << endl;

	ifstream input17("random_med.txt");
	while (getline(input17, line)) {
		data.push_back(stoi(line));
	}

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		medium.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	medium.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		medium.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


	data.clear();
	cout << endl;
	cout << endl;
	cout << "Min Heap Random Large" << endl;

	ifstream input18("random_large.txt");
	while (getline(input18, line)) {
		data.push_back(stoi(line));
	}
	cout << data.size() << endl;
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < data.size(); i++) {
		large.insert(data[i]);
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Insertion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	large.heapPrint();
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Traversal: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		large.remove();
	}
	end = chrono::high_resolution_clock::now();
	cout << "Elapsed time for Deletion: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " nanoseconds" << endl;


}
#include <iostream>
using namespace std;

struct Node
{
	int info;
	Node* left;
	Node* right;
	int height;
};
typedef Node* NodePtr;

//Độ cao
int height(NodePtr root)
{
	if (root == 0) return 0;
	return root->height;
}

//----------------------------------------------------XỬ LÝ QUAY----------------------------------------------------

//lệch PHẢI => quay TRÁI
void rotateLeft(NodePtr& root)
{
	if (root == NULL || root->right == NULL) return;

	//quay trái
	NodePtr pointer = root->right;
	root->right = pointer->left;
	pointer->left = root;

	//cập nhập độ cao root (vì lúc này root nằm dưới pointer => cập nhật độ cao root trước)
	int lr = height(root->left);
	int rr = height(root->right);
	root->height = 1 + (lr > rr ? lr : rr);

	//cập nhật độ cao pointer sau khi cập nhật xong độ cao root
	int lp = height(pointer->left);
	int rp = height(pointer->right);
	pointer->height = 1 + (lp > rp ? lp : rp);

	//cập nhật lại root lên làm gốc
	root = pointer;
}

//lệch TRÁI => quay PHẢI
void rotateRight(NodePtr& root)
{
	if (root == NULL || root->left == NULL) return;

	//quay phải
	NodePtr pointer = root->left;
	root->left = pointer->right;
	pointer->right = root;

	//cập nhật độ cao root
	int lr = height(root->left);
	int rr = height(root->right);
	root->height = (lr > rr ? lr : rr);

	//cập nhật độ cao pointer
	int lp = height(pointer->left);
	int rp = height(pointer->right);
	pointer->height = (lp > rp ? lp : rp);

	root = pointer; 
}

//lệch BÊN PHẢI của CÂY CON TRÁI => quay TRÁI PHẢI
void rotateLeftRight(NodePtr& root)
{
	if (root == NULL || root->left == NULL) return;
	rotateLeft(root->left);
	rotateRight(root);
}

//lệch BÊN TRÁI của CÂY CON PHẢI => quay PHẢI TRÁI
void rotateRightLeft(NodePtr& root)
{
	if (root == NULL || root->right == NULL) return;
	rotateRight(root->right);
	rotateLeft(root);
}

//cây bị mất cân bằng bên PHẢI => cân bằng TRÁI
void balanceLeft(NodePtr& root)
{
	if (root == NULL) return;
	if (height(root->right) - height(root->left) == 2)
	{
		NodePtr pointer = root->right;
		if (height(pointer->right) >= height(pointer->left))
			rotateLeft(root);
		else
			rotateRightLeft(root);
	}
}

//cây bị mất cân bằng bên TRÁI => cân bằng PHẢI
void balanceRight(NodePtr& root)
{
	if (root == NULL) return;
	if (height(root->left) - height(root->right) == 2)
	{
		NodePtr pointer = root->left;
		if (height(pointer->left) >= height(pointer->right))
			rotateRight(root);
		else
			rotateLeftRight(root);
	}
}

//----------------------------------------------------XỬ LÝ QUAY----------------------------------------------------

//Chèn
NodePtr makeNode(int value)
{
	NodePtr newNode = new Node();
	newNode->info = value;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	return newNode;
}

void insert(NodePtr& root, int value)
{
	if (root == NULL) root = makeNode(value);
	else
	{
		if (value == root->info)
			cout << "\nNode da ton tai";

		else if (value < root->info)
		{
			insert(root->left, value);
			balanceRight(root);
		}
		else
		{
			insert(root->right, value);
			balanceLeft(root);
		}

		int lr = height(root->left);
		int rr = height(root->right);
		root->height = 1 + (lr > rr ? lr : rr);
	}
}

//Tìm
NodePtr search(NodePtr root, int key)
{
	if (root == NULL || key == root->info)
		return root;
	else if (key < root->info)
		return search(root->left, key);
	else
		return search(root->right, key);
}

//Xóa
void deleteNode(NodePtr& root, int key)
{
	if (root == NULL) return;
	if (key == root->info)
	{
		if (root->left == NULL)
		{
			NodePtr temp = root;
			root = root->right;
			delete temp;
		}
		else if (root->right == NULL)
		{
			NodePtr temp = root;
			root = root->left;
			delete temp;
		}
		else
		{
			NodePtr maxRightOfLeftSubTree = root->left;
			while (maxRightOfLeftSubTree->right != NULL)
				maxRightOfLeftSubTree = maxRightOfLeftSubTree->right;
			root->info = maxRightOfLeftSubTree->info;
			deleteNode(root->left, maxRightOfLeftSubTree->info);
			balanceLeft(root);
		}
	}
	else if (key < root->info)
	{
		deleteNode(root->left, key);
		balanceLeft(root);
	}
	else
	{
		deleteNode(root->right, key);
		balanceRight(root);
	}

	if (root != NULL)
	{
		int lr = height(root->left);
		int rr = height(root->right);
		root->height = 1 + (lr > rr ? lr : rr);
	}
}

void preOrder(NodePtr root)
{
	if (root)
	{
		cout << root->info << " ";
		preOrder(root->left);
		preOrder(root->right);
	}
}



int main()
{
	NodePtr root = NULL;
	const int MAX = 11;
	int array[] = { 10,12,1,14,6,5,8,15,3,9,7 };
	for (int i = 0; i < MAX; i++)
	{
		insert(root, array[i]);
	}
	cout << "PreOrder: ";
	preOrder(root);

	cout << "\nFind 9: " << search(root, 9);
	cout << "\nFind 99: " << search(root, 99);

	deleteNode(root, 9);
	cout << "\nPreOrder after deletion: ";
	preOrder(root);


	return 0;
}
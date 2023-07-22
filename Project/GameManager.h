#include <SFML/Graphics.hpp>
#include <fstream>
// Screen dimensions
#define SCREENW 600
#define SCREENH 800

#define SPACE 10
using namespace std;

class AVLTree;

// Function that copies data from one file to another
// used to copy default files to in-game files
void copyFile(const std::string &from, const std::string &to) {
    std::ifstream is(from, ios::in | ios::binary);
    std::ofstream os(to, ios::out | ios::binary);

	string text;
    while( getline(is, text) ) {
    	os << text << endl;
	}
	
	is.close();
	os.close();
}

class Node {
	float data;
	string word;
	Node *left;
	Node *right;
	int height;
   	public:
	   	friend class AVLTree;
		
		Node() {
			data = 0;
			height = 1;
			word = "\n";
			right = left = NULL;
		}
		Node(int d, string w) {
			data = d;
			height = 1;
			word = w;
			right = left = NULL;
		}
};

int max(int a, int b) {
	return (a > b) ? a : b;
}

class AVLTree {
	Node *root;
	
	int height(Node * r) {
		if (r == NULL)
		  	return -1;
		else {
		  	/* compute the height of each subtree */
		  	int lheight = height(r -> left);
		  	int rheight = height(r -> right);
		
		  	/* use the larger height */
		  	if(lheight > rheight) {
		  		return (lheight + 1);
			} else {
				return (rheight + 1);
			}
		}
	}
	
	// Rotate right
	Node *rightRotate(Node *y) {
		Node *x = y->left;
		Node *T2 = x->right;
		x->right = y;
		y->left = T2;
		y->height = max(height(y->left), height(y->right)) + 1;
		x->height = max(height(x->left), height(x->right)) + 1;
		return x;
	}
	
	// Rotate left
	Node *leftRotate(Node *x) {
		Node *y = x->right;
		Node *T2 = y->left;
		y->left = x;
		x->right = T2;
		x->height = max(height(x->left), height(x->right)) + 1;
		y->height = max(height(y->left), height(y->right)) + 1;
		return y;
	}
	
	int getBalanceFactor(Node * r) {
		return (height(r->left) - height(r->right));
	}
	
	// Insert a node
	Node *insertNode(Node *node, float data, string word) {
		// Find the correct postion and insert the node
		if (node == NULL) {
			Node *newNode = new Node(data, word);
			return newNode;
		}
		if (data < node->data)
			node->left = insertNode(node->left, data, word);
		else if (data > node->data)
			node->right = insertNode(node->right, data, word);
		else
			return node;
		
		// Update the balance factor of each node and
		// balance the tree
		node->height = 1 + max(height(node->left), height(node->right));
		int balanceFactor = getBalanceFactor(node);
		if (balanceFactor > 1) {
			if (data < node->left->data) {
				return rightRotate(node);
			} else if (data > node->left->data) {
				node->left = leftRotate(node->left);
				return rightRotate(node);
			}
		}
		if (balanceFactor < -1) {
			if (data > node->right->data) {
			  	return leftRotate(node);
			} else if (data < node->right->data) {
			  	node->right = rightRotate(node->right);
			  	return leftRotate(node);
			}
		}
		return node;
	}
	
	Node *getMinDiffNode(Node * node) {
		Node * current = node;
		/* loop down to find the leftmost leaf */
		while (current -> right != NULL) {
			current = current -> right;
		}
		return current;
	}
	
	Node *getMaxDiffNode(Node * node) {
		Node * current = node;
		/* loop down to find the leftmost leaf */
		while (current -> left != NULL) {
			current = current -> left;
		}
		return current;
	}
	
	// Delete a node
	Node *deleteNode(Node *root, float data, bool win) {
		// Find the node and delete it
		if (root == NULL)
			return root;
		if (data < root->data)
			root->left = deleteNode(root->left, data, win);
		else if (data > root->data)
			root->right = deleteNode(root->right, data, win);
		else {
			if ((root->left == NULL) ||(root->right == NULL)) {
			  		Node *temp = root->left ? root->left : root->right;
			  	if (temp == NULL) {
			    	temp = root;
			    	root = NULL;
			  	} else
			    	*root = *temp;
			  	free(temp);
			} else {
				if(win) {
					Node *temp = getMinDiffNode(root->right);
				  	root->data = temp->data;
				  	root->word = temp->word;
				  	root->right = deleteNode(root->right,
				               temp->data, win);
				} else {
					Node *temp = getMaxDiffNode(root->left);
				  	root->data = temp->data;
				  	root->word = temp->word;
				  	root->left = deleteNode(root->left,
				               temp->data, win);
				}
			}
		}
		
		if (root == NULL)
			return root;
		/*Update the balance factor of each node and
			balance the tree*/
		root->height = 1 + max(height(root->left), height(root->right));
		int balanceFactor = getBalanceFactor(root);
		if (balanceFactor > 1) {
			if (getBalanceFactor(root->left) >= 0) {
			  	return rightRotate(root);
			} else {
			  	root->left = leftRotate(root->left);
			  	return rightRotate(root);
			}
		}
		if (balanceFactor < -1) {
			if (getBalanceFactor(root->right) <= 0) {
			  	return leftRotate(root);
			} else {
			  	root->right = rightRotate(root->right);
			  	return leftRotate(root);
			}
		}
		return root;
	}
	
	void print2DUtil(Node * r, int space) {
		if (r == NULL) // Base case  1
			return;
		space += SPACE; // Increase distance between levels   2
		print2DUtil(r -> right, space); // Process right child first 3 
		cout << endl;
		for (int i = SPACE; i < space; i++) // 5 
			cout << " "; // 5.1  
		cout << r -> data << " (" << r->word << ")" << "\n"; // 6
		print2DUtil(r -> left, space); // Process left child  7
	}

	void printInorder(Node *r) { //  (Left, current node, Right)
		if (r == NULL)
			return;
		/* first recur on left child */
		printInorder(r -> left);
		/* then print the data of node */
		cout << r->data << " (";
		cout << r->word << ")\t\t";
		/* now recur on right child */
		printInorder(r -> right);
	}
	
	public:
		AVLTree() {
			root = NULL;
		}
		
		void insert(float d, string w) {
			root = insertNode(root, d, w);
		}
		
		void remove(float d, bool win) {
			root = deleteNode(root, d, win);
		}
		
		bool isEmpty() {
			return (root == NULL);
		}
		
		void display(bool type) { // if true, prints 2D; if false, prints Inorder
			if(type)
				print2DUtil(root, 10);
			else
				printInorder(root);
			
			cout << endl << endl;
		}
		
		//overloading display function without any argument
		void display() {
			print2DUtil(root, 10);
			
			cout << endl << endl;
		}
		
		string getWord() {
			return root->word;
		}
		
		float getScore() {
			return root->data;
		}
		
		void removeCurrentWord(bool win) {
			root = deleteNode(root, root->data, win);
		}
		
		void saveUtil(Node *n, ofstream &save_word, ofstream &save_score) {
			if (n == NULL)
				return;
			
			if(save_word.is_open() && save_score.is_open()) {
				save_word << n->word << endl;
				save_score << n->data << endl;
			}
			
			saveUtil(n->left, save_word, save_score);

			saveUtil(n->right, save_word, save_score);
		}
		
		void saveTreeToFiles() {
			
			ofstream save_word("files/in_game/words.txt", ios::ate);
			ofstream save_score("files/in_game/score.txt", ios::ate);
			
			saveUtil(root, save_word, save_score);
			
			save_word.close();
			save_score.close();
		}
};


// GameManager class

class GameManager {
	sf::Text heading_text;
	sf::Text help_text;
	sf::Font font;
	sf::Texture texture;
	sf::Sprite logo;
	bool is_started;
	AVLTree *tree;
	// -1 for still in play, 0 for incorrect, 1 for correct
	int correct;
	public:
		/* constructor:
		starts the game (is_started is TRUE)
		sets correct to STILL IN PLAY
		loads up font for the game message
		creates the AVL Tree
		*/
		GameManager() {
			is_started = true;
			set_correct(-1);
			texture.loadFromFile("files/textures/LOGO.png");
			logo.setTexture(texture);
			logo.setPosition(SCREENW / 2 - ((1413.0 * 0.3 ) / 2.0), 20);
			logo.setScale(sf::Vector2f(0.3f, 0.3f));
			font.loadFromFile("fonts/Helvetica.ttf");
			help_text.setFont(font);
			help_text.setString("F1: See Instructions              F2: Peek at word              F3: Load Default");
			help_text.setCharacterSize(18);
			help_text.setFillColor(sf::Color::White);
			help_text.setPosition(10, SCREENH - 30);
			tree = new AVLTree;
			loadTree();
		}
		~GameManager() {
			delete tree;
		}
		
		void loadTree() {
			ifstream read_word;
			ifstream read_score;
			
			read_word.open("files/in_game/words.txt");
			read_score.open("files/in_game/score.txt");
			
			string word;
			string score;
			
			if(read_word.is_open() && read_score.is_open()) {
				while( getline(read_word, word) ) {
					getline(read_score, score);
					
					tree->insert(stof(score), word);
				}
			}
			
			read_word.close();
			read_score.close();
		}
		
		void draw(sf::RenderWindow &w) {
			w.draw(logo);
			w.draw(help_text);
		}
		
		void loadDefault() {

			copyFile("files/default/words.txt", "files/in_game/words.txt");
			copyFile("files/default/score.txt", "files/in_game/score.txt");

		}
		
		void saveTree() {
			tree->saveTreeToFiles();
		}
		
		// sets the CORRECT variable value and displays appropriate message corresponding to correctness value
		void set_correct(int c) {
			// -1 for still in play, 0 for incorrect, 1 for correct
			correct = c;
		}
		
		string restart(int c) {
			// Get the word
			string word = tree->getWord();
			// Convert it to UPPERCASE
			for(int i = 0; i < 5; i++)
				word[i] = word[i] - 32;
			
			// Remove the word from the library
			switch(c) {
				case 0:
					tree->removeCurrentWord(false);
					break;
				case 1:
					tree->removeCurrentWord(true);
					break;
				default:
					break;
			}
			
			return word;
		}
		// returns the correct variable
		int get_correct() {
			return correct;
		}
		// returns the display text for help (shortcuts)
		sf::Text get_help_text() {
			return help_text;
		}
		// getter and setter for is_started
		void set_is_started(bool s) {
			is_started = s;
		}
		bool get_is_started() {
			return is_started;
		}
};

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    char character;
    int freq;
    Node *left, *right, *next;
    Node(char c, int f) : character(c), freq(f), left(NULL), right(NULL), next(NULL) {}
};

class MyHuffman {
private:
    Node* root;
    Node* front;
    Node* rear;
    string charCodes[256];

public:
    MyHuffman() : root(NULL), front(NULL), rear(NULL) {}
    void findfreq(Node* characters[]);
    void buildTree();
    void dequeue();
    void enqueue(Node* newNode);
    void preOrder(Node* root, string str);
    void encodeSentence(const string& sentence);
    void decodeSentence();
};

void MyHuffman::findfreq(Node* characters[]) {
    int freq[256] = {0};
    char ch;
    ifstream myFile("HuffmanData.txt");

    string line;
    bool readingInput = false;
    while (getline(myFile, line)) {
        if (line == "##INPUT##") {
            readingInput = true;
            continue;
        }
        if (line == "##ENCODED##") {
            readingInput = false;
            break;
        }
        if (readingInput) {
            for (char c : line) freq[(unsigned char)c]++;
        }
    }

    myFile.close();

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            characters[i] = new Node((char)i, freq[i]);
            enqueue(characters[i]);
        }
    }
}

void MyHuffman::enqueue(Node* newNode) {
    if (front == NULL) {
        front = rear = newNode;
        return;
    }

    if (newNode->freq < front->freq) {
        newNode->next = front;
        front = newNode;
    } else {
        Node* temp = front;
        while (temp->next != NULL && temp->next->freq <= newNode->freq) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
        if (newNode->next == NULL) {
            rear = newNode;
        }
    }
}

void MyHuffman::dequeue() {
    if (front == NULL) {
        return;
    }

    Node* temp = front;
    front = front->next;
    temp->next = NULL;

    if (front == NULL) {
        rear = NULL;
    }
}

void MyHuffman::buildTree() {
    while (front != NULL && front->next != NULL) {
        Node* left = front;
        dequeue();
        Node* right = front;
        dequeue();

        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        enqueue(newNode);
    }

    root = front;
    preOrder(root, "");
}

void MyHuffman::preOrder(Node* root, string str) {
    if (root == NULL) {
        return;
    }
    if (!root->left && !root->right) {
        charCodes[(unsigned char)root->character] = str;
        cout << root->character << "| \t\t" << root->freq << "| \t" << str << endl;
    }
    preOrder(root->left, str + "0");
    preOrder(root->right, str + "1");
}

void MyHuffman::encodeSentence(const string& sentence) {
    string encodedSentence;
    for (char ch : sentence) {
        encodedSentence += charCodes[(unsigned char)ch];
    }

    ofstream outFile("HuffmanData.txt");
    outFile << "##INPUT##\n" << sentence << "\n";
    outFile << "##ENCODED##\n" << encodedSentence << "\n";
    outFile.close();

    cout << "Encoded data: " << encodedSentence << endl;
    cout << "Data saved to HuffmanData.txt" << endl;
}

void MyHuffman::decodeSentence() {
    ifstream inFile("HuffmanData.txt");
    string line, encodedSentence;
    bool readingEncoded = false;

    while (getline(inFile, line)) {
        if (line == "##ENCODED##") {
            readingEncoded = true;
            continue;
        }
        if (line == "##DECODED##") break;
        if (readingEncoded) {
            encodedSentence += line;
        }
    }
    inFile.close();

    string decodedSentence;
    Node* current = root;
    for (char bit : encodedSentence) {
        current = (bit == '0') ? current->left : current->right;

        if (!current->left && !current->right) {
            decodedSentence += current->character;
            current = root;
        }
    }

    cout << "Decoded sentence: " << decodedSentence << endl;

    ofstream outFile("HuffmanData.txt", ios::app);
    outFile << "##DECODED##\n" << decodedSentence << "\n";
    outFile.close();
    cout << "Decoded sentence appended to HuffmanData.txt" << endl;
}

int main() {
    MyHuffman hf;
    Node* characters[256] = {NULL};

    cout << "Choose an option: \n1. Compress a sentence\n2. Decompress the encoded file\n";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        cout << "Enter a sentence: ";
        string input;
        getline(cin, input);

        hf.findfreq(characters);
        cout << "Char \t Frequency \t Code \n";
        hf.buildTree();

        hf.encodeSentence(input);
    } else if (choice == 2) {
        hf.findfreq(characters);
        hf.buildTree();

        hf.decodeSentence();
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}

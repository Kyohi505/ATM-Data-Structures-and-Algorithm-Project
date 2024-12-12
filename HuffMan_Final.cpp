#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    char character;
    int freq;
    Node *left, *right;

    Node(char c, int f) : character(c), freq(f), left(NULL), right(NULL) {}
};

struct PQNode {
    Node* tree;
    PQNode* next;

    PQNode(Node* t) : tree(t), next(NULL) {}
};

class PriorityQueue {
private:
    PQNode* head;

public:
    PriorityQueue() : head(NULL) {}

    void enqueue(Node* tree) {
        PQNode* newNode = new PQNode(tree);
        if (!head || head->tree->freq > tree->freq) {
            newNode->next = head;
            head = newNode;
            return;
        }

        PQNode* current = head;
        while (current->next && current->next->tree->freq <= tree->freq) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    Node* dequeue() {
        if (!head) return NULL;
        PQNode* temp = head;
        head = head->next;
        Node* tree = temp->tree;
        delete temp;
        return tree;
    }

    bool isEmpty() {
        return head == NULL;
    }

    ~PriorityQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

class MyHuffman {
private:
    Node* root;
    string encodedData;

public:
    MyHuffman() : root(NULL) {}

    void buildTree(const string& input);
    void generateCodes(Node* node, string str, string codes[]);

    void serializeTree(Node* node, string& serializedTree);
    Node* deserializeTree(const string& serializedTree, int& index);
    void encodeSentence(const string& input);
    void decodeSentence();
    void freeTree(Node* node);

    ~MyHuffman() { freeTree(root); }
};

void MyHuffman::buildTree(const string& input) {
    int freq[256] = {0};
    for (char c : input) {
        freq[(unsigned char)c]++;
    }

    PriorityQueue pq;
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            pq.enqueue(new Node((char)i, freq[i]));
        }
    }

    while (!pq.isEmpty()) {
        Node* left = pq.dequeue();
        if (pq.isEmpty()) {
            root = left;
            break;
        }
        Node* right = pq.dequeue();

        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.enqueue(merged);
    }
}

void MyHuffman::generateCodes(Node* node, string str, string codes[]) {
    if (!node) return;

    if (!node->left && !node->right) {
        codes[(unsigned char)node->character] = str;
        return;
    }

    generateCodes(node->left, str + "0", codes);
    generateCodes(node->right, str + "1", codes);
}

void MyHuffman::serializeTree(Node* node, string& serializedTree) {
    if (!node) {
        serializedTree += "#";
        return;
    }
    if (!node->left && !node->right) {
        serializedTree += "L" + string(1, node->character);
        return;
    }
    serializedTree += "I";
    serializeTree(node->left, serializedTree);
    serializeTree(node->right, serializedTree);
}

Node* MyHuffman::deserializeTree(const string& serializedTree, int& index) {
    if (index >= serializedTree.size()) return NULL;

    char marker = serializedTree[index++];
    if (marker == '#') return NULL;
    if (marker == 'L') {
        char c = serializedTree[index++];
        return new Node(c, 0);
    }
    if (marker == 'I') {
        Node* node = new Node('\0', 0);
        node->left = deserializeTree(serializedTree, index);
        node->right = deserializeTree(serializedTree, index);
        return node;
    }
    return NULL;
}

void MyHuffman::encodeSentence(const string& input) {
    buildTree(input);

    string codes[256];
    generateCodes(root, "", codes);

    encodedData.clear();
    for (char c : input) {
        encodedData += codes[(unsigned char)c];
    }

    cout << "The sentence is: " << input << endl;
    cout << "\nChar      Frequency     Code" << endl;
    for (int i = 0; i < 256; ++i) {
        if (!codes[i].empty()) {
            cout << (char)i << "        |           " << codes[i] << endl;
        }
    }

    cout << "\nEncoded data: " << encodedData << endl;

    string serializedTree;
    serializeTree(root, serializedTree);

    ofstream treeFile("HuffmanTree.txt");
    if (treeFile.is_open()) {
        treeFile << serializedTree;
        treeFile.close();
    } else {
        cerr << "Error: Could not open tree file for writing!" << endl;
        return;
    }

    ofstream encodedFile("HuffmanEncoded.bin", ios::binary);
    if (encodedFile.is_open()) {
        int validBits = encodedData.size();
        encodedFile.write(reinterpret_cast<char*>(&validBits), sizeof(validBits));
        for (size_t i = 0; i < encodedData.size(); i += 8) {
            string byteStr = encodedData.substr(i, 8);
            while (byteStr.size() < 8) byteStr += "0";
            unsigned char byte = static_cast<unsigned char>(stoi(byteStr, NULL, 2));
            encodedFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
        }
        encodedFile.close();
    } else {
        cerr << "Error: Could not open binary file for writing!" << endl;
        return;
    }

    cout << "Data saved to HuffmanEncoded.bin" << endl;
}

void MyHuffman::decodeSentence() {
    ifstream treeFile("HuffmanTree.txt");
    if (!treeFile) {
        cerr << "Error: Could not open tree file for reading!" << endl;
        return;
    }
    string serializedTree((istreambuf_iterator<char>(treeFile)), istreambuf_iterator<char>());
    treeFile.close();

    int index = 0;
    root = deserializeTree(serializedTree, index);
    if (!root) {
        cerr << "Error: Failed to reconstruct the Huffman tree." << endl;
        return;
    }

    ifstream encodedFile("HuffmanEncoded.bin", ios::binary);
    if (!encodedFile) {
        cerr << "Error: Could not open binary file for reading!" << endl;
        return;
    }

    int validBits;
    encodedFile.read(reinterpret_cast<char*>(&validBits), sizeof(validBits));
    string encodedSentence;
    unsigned char byte;
    while (encodedFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        for (int i = 7; i >= 0; --i) {
            encodedSentence += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    encodedFile.close();
    encodedSentence = encodedSentence.substr(0, validBits);

    string decodedSentence;
    Node* current = root;
    for (char bit : encodedSentence) {
        current = (bit == '0') ? current->left : current->right;
        if (!current) {
            cerr << "Error: Decoding failed due to corrupted data." << endl;
            return;
        }
        if (!current->left && !current->right) {
            decodedSentence += current->character;
            current = root;
        }
    }

    if (!decodedSentence.empty()) {
        cout << "Decoded sentence: " << decodedSentence << endl;
    } else {
        cerr << "Error: Decoding resulted in an empty sentence." << endl;
    }
}

void MyHuffman::freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

int main() {
    MyHuffman hf;

    cout << "Choose an option: \n1. Compress a sentence\n2. Decompress the encoded file\n";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        cout << "Enter a sentence: ";
        string input;
        getline(cin, input);

        hf.encodeSentence(input);
    } else if (choice == 2) {
        hf.decodeSentence();
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}

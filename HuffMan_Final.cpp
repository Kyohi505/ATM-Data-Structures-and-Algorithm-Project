#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    char character;
    int freq;
    Node *left, *right;

    Node(char c, int f) : character(c), freq(f), left(NULL), right(NULL) {}
};

class MyHuffman {
private:
    Node* root;
    vector<pair<char, string>> charCodes;

public:
    MyHuffman() : root(NULL) {}

    void buildTree(const string& input);
    void generateCodes(Node* node, string str);
    void serializeTree(Node* node, string& serializedTree);
    Node* deserializeTree(const string& serializedTree, int& index);
    void encodeSentence(const string& input);
    void decodeSentence();
    void freeTree(Node* node);

    ~MyHuffman() { freeTree(root); }
};

void MyHuffman::buildTree(const string& input) {
    vector<pair<char, int>> freq;
    for (char c : input) {
        bool found = false;
        for (auto& p : freq) {
            if (p.first == c) {
                p.second++;
                found = true;
                break;
            }
        }
        if (!found) freq.push_back({c, 1});
    }

    for (size_t i = 0; i < freq.size(); ++i) {
        for (size_t j = i + 1; j < freq.size(); ++j) {
            if (freq[i].second > freq[j].second) {
                swap(freq[i], freq[j]);
            }
        }
    }

    vector<Node*> nodes;
    for (const auto& p : freq) {
        nodes.push_back(new Node(p.first, p.second));
    }

    while (nodes.size() > 1) {
        Node* left = nodes[0];
        Node* right = nodes[1];
        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        nodes.push_back(merged);

        for (size_t i = 0; i < nodes.size(); ++i) {
            for (size_t j = i + 1; j < nodes.size(); ++j) {
                if (nodes[i]->freq > nodes[j]->freq) {
                    swap(nodes[i], nodes[j]);
                }
            }
        }
    }

    root = nodes[0];
    generateCodes(root, "");
}

void MyHuffman::generateCodes(Node* node, string str) {
    if (!node) return;
    if (!node->left && !node->right) {
        charCodes.push_back({node->character, str});
        return;
    }
    generateCodes(node->left, str + "0");
    generateCodes(node->right, str + "1");
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
    string encodedSentence;
    for (char c : input) {
        for (const auto& p : charCodes) {
            if (p.first == c) {
                encodedSentence += p.second;
                break;
            }
        }
    }

    string serializedTree;
    serializeTree(root, serializedTree);
    ofstream treeFile("HuffmanTree.txt");
    if (!treeFile) {
        cerr << "Error: Could not open tree file for writing!" << endl;
        return;
    }
    treeFile << serializedTree;
    treeFile.close();

    ofstream encodedFile("HuffmanEncoded.bin", ios::binary);
    if (!encodedFile) {
        cerr << "Error: Could not open binary file for writing!" << endl;
        return;
    }

    int validBits = encodedSentence.size();
    encodedFile.write(reinterpret_cast<char*>(&validBits), sizeof(validBits));
    for (size_t i = 0; i < encodedSentence.size(); i += 8) {
        string byteStr = encodedSentence.substr(i, 8);
        while (byteStr.size() < 8) byteStr += "0";
        unsigned char byte = static_cast<unsigned char>(stoi(byteStr, nullptr, 2));
        encodedFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }
    encodedFile.close();

    cout << "Serialized tree saved to HuffmanTree.txt" << endl;
    cout << "Encoded data saved to HuffmanEncoded.bin" << endl;
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

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// TrieNode structure representing each character in the Trie
class TrieNode
{
public:
    bool isEndOfWord;      // Flag to check if a word ends at this node
    TrieNode *children[26]; // Array to store pointers to children nodes (a-z)

    TrieNode()
    {
        isEndOfWord = false;
        // Initialize all children to nullptr
        for (int i = 0; i < 26; i++)
        {
            children[i] = nullptr;
        }
    }
};

class Trie
{
public:
    TrieNode *root;

    // Constructor: Initializes the Trie with an empty root node
    Trie()
    {
        root = new TrieNode();
    }

    // Destructor: Cleans up memory when the Trie object is destroyed
    ~Trie()
    {
        clearNodes(root);
    }

    // Function to insert a word into the Trie
    void insert(string word)
    {
        TrieNode *curr = root;

        for (char c : word)
        {
            int index = c - 'a'; // Calculate index (0 for 'a', 25 for 'z')
            
            // If the path doesn't exist, create a new node
            if (curr->children[index] == nullptr)
            {
                curr->children[index] = new TrieNode();
            }

            // Move to the next node
            curr = curr->children[index];
        }

        // Mark the last node as the end of a word
        curr->isEndOfWord = true;
    }

    // Recursive function to search for a word (supports '.' as a wildcard)
    bool search(string word, int index, TrieNode *curr)
    {
        // Base Case: If we have processed the entire word
        if (index == word.size())
        {
            return curr->isEndOfWord; // Return true only if it marks the end of a valid word
        }

        // Case 1: If the character is a wildcard '.'
        if (word[index] == '.')
        {
            // Try all possible paths (a-z)
            for (int i = 0; i < 26; i++)
            {
                if (curr->children[i] != nullptr)
                {
                    // If any path returns true, propagate it up
                    if (search(word, index + 1, curr->children[i]))
                    {
                        return true;
                    }
                }
            }
            // If checking all children doesn't find a match
            return false;
        }
        // Case 2: If the character is a specific letter
        else
        {
            int currentIndex = word[index] - 'a';
            
            // If the path for this character doesn't exist
            if (curr->children[currentIndex] == nullptr)
            {
                return false;
            }
            // Continue searching along this path
            return search(word, index + 1, curr->children[currentIndex]);
        }
    }

    // Helper function to print all words starting from a given node
    void showSuggestions(string s, TrieNode *curr)
    {
        if (curr == nullptr)
        {
            return;
        }

        // If a word ends here, print the current accumulated string
        if (curr->isEndOfWord == true)
        {
            cout << s << endl;
        }

        // DFS traversal to find all child words
        for (int i = 0; i < 26; i++)
        {
            if (curr->children[i] != nullptr)
            {
                s.push_back(i + 'a'); // Add character to current string
                showSuggestions(s, curr->children[i]); // Recurse
                s.pop_back(); // Backtracking: Remove character to explore other paths
            }
        }
    }

    // Function to find and display all words with a given prefix
    void startWithPrefix(string s)
    {
        TrieNode *curr = root;
        string ans = s; // Stores the prefix

        // Traverse to the end of the prefix
        for (char c : s)
        {
            int index = c - 'a';
            if (curr->children[index] == nullptr)
            {
                // Prefix not found in Trie
                return;
            }
            curr = curr->children[index];
        }

        // Once at the end of prefix, show all completions
        showSuggestions(ans, curr);
    }

    // Helper function for the destructor to delete nodes (Post-order traversal)
    void clearNodes(TrieNode *node)
    {
        if (node == nullptr)
        {
            return;
        }

        // Recursively delete all children first
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i] != nullptr)
            {
                clearNodes(node->children[i]);
            }
        }

        // Delete the current node after children are processed
        delete node;
    }
};

int main()
{
    // Create Trie on Heap
    Trie *t = new Trie();
    
    // Inserting sample words
    t->insert("apple");
    t->insert("bad");
    t->insert("cat");
    t->insert("catterpiller");
    t->insert("cattle");
    t->insert("cap");

    cout << "--- Suggestions for prefix 'c' ---" << endl;
    t->startWithPrefix("c");

    cout << "\n--- Wildcard Search Results ---" << endl;
    
    // Testing wildcard search
    if(t->search(".ad", 0, t->root)) 
        cout << "Found match for .ad" << endl;
    else 
        cout << "No match for .ad" << endl;

    if(t->search("b..", 0, t->root)) 
        cout << "Found match for b.." << endl;
    else 
        cout << "No match for b.." << endl;
        
    // Clean up memory manually (calls destructor)
    delete t; 

    return 0;
}

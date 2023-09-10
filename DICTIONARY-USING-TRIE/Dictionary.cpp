#include<bits/stdc++.h>


using namespace std;


class TrieNode;


template <class K, class V>
class Pair {
    public:

    K key;
    V value;

    Pair(K key, V value) {
        this->key = key;
        this->value = value;
    }

};




template <class K, class V>
class HashMap {

    public:

    int size;           
    int buckets;        
    list<Pair<K, V>*> *table;

   

    HashMap(int buckets) {
        
        this->buckets = buckets;
        table = new list<Pair<K, V>*>[buckets];

        size = 0;
       

    }

    
    int hashFunction(K x) {
        int temp = x;
        return (x % buckets);
    }

    
    void insert(K key, V value) {
        
        Pair<K, V>* pair = new Pair<K, V>(key, value);
        int index = hashFunction(key);
        table[index].push_back(pair);
        size++;

        
        double loadFactor = (1.0 * size) / buckets; 
        
        if (loadFactor > 0.7)
        {
            rehash();
        }

    }

   
    void rehash() {

        list<Pair<K, V>*> *oldTable = table;
        table = new list<Pair<K, V>*>[2 * buckets];
        
        int oldBuckets = buckets;
        buckets *= 2;
        size = 0;
        
       

        for (int i = 0; i < oldBuckets; i++) { 
            for (Pair<K, V>* node : oldTable[i]) {
                K key = node->key;
                V value = node->value;
                insert(key, value);
            }
        } 

        

        delete []oldTable;

    }

    

    void deleteItem(K key) {

        int index = hashFunction(key); 
        
        typename list <Pair<K, V>*> :: iterator i; 
        for (i = table[index].begin(); i != table[index].end(); i++) { 
            if ((*i)->key == key) {
                break; 
            } 
        } 

        
        if (i != table[index].end()) {
            table[index].erase(i);     
        }

    }

    

    V getValue(K key) {

        int index = hashFunction(key); 
        
        typename list <Pair<K, V>*> :: iterator i; 
        for (i = table[index].begin(); i != table[index].end(); i++) { 
            if ((*i)->key == key) {
                break; 
            } 
        } 

        
        if (i != table[index].end()) {
            return (*i)->value;
        }

        return 0;
    }


    list<Pair<K, V>*>* getMap() {
        return table;
    }

    int getBuckets() {
        return buckets;
    }

    void displayHash() { 
        for (int i = 0; i < buckets; i++) { 
            cout << i;
            for (Pair<K, V>* x : table[i]) {
                cout << " --> " << x->key << " : ";
                cout  << x->value; 
            }
            cout << endl; 
        } 
    } 

    
};



class WordData {

    public:

    string partOfSpeech;
    string synonyms;
    WordData* next;

    WordData(string partOfSpeech, string synonyms) {
        this->partOfSpeech = partOfSpeech;
        this->synonyms = synonyms;
        next = NULL;
    }

};


  
class TrieNode { 

    public:
  
    WordData* data;
  
    /* nodes store a map to child node */
    HashMap<char, TrieNode*> *map = new HashMap<char, TrieNode*>(5); 

    TrieNode() {
        data = NULL;
    }

}; 




class Trie {

    public:

    TrieNode* root = new TrieNode();
    
    list<string>* suggestionList;
    int suggestCount;


    Trie() {
        root = new TrieNode();
        
        suggestionList = new list<string>;
        suggestCount = 0;
    }

    
    /**
     * Inserting each char of the string in a separate
     * Trie Node. 
     * --> A hashmap will point to the next char (Trie Node)
     * --> Data Node will be exists (TRUE) for the end Char
     * --> If Data already point the current node to the next pointer
     * 
     * COMPLEXITY : O(n)
     *  >> Where n is the Length of the "KEY"
     *  >> Worst case is also O(n)
     * 
    */

    void insert(string str, string partOfSpeech, string synonyms) 
    { 
        if (root == nullptr) 
            root = new TrieNode();
    
        TrieNode* temp = root; 
        for (int i = 0; i < str.length(); i++) { 
            char x = str[i]; 
    
            
            if (!temp->map->getValue(x)) {
                temp->map->insert(x, new TrieNode());
            }

            temp = temp->map->getValue(x); 
        } 

        WordData* data = new WordData(partOfSpeech, synonyms);

        
        if (temp->data) {
            temp->data->next = data;
        }
        
        else {
            temp->data = data;
        } 

    } 
    
  
   WordData* search(string prefix) {

        TrieNode* loc = root; 
        TrieNode* nextValue; 
        string word;
        for (int i = 0; i < prefix.length(); i++) { 

            nextValue = loc->map->getValue(prefix[i]); 

            if (nextValue == nullptr) {
                
                break;
            }

            word.push_back(prefix[i]);
            loc = nextValue;
        } 

        suggestionList->clear();
        suggestCount = 0;

      

        if (prefix == word && loc->data != NULL) {
            findSuffix(loc, word);
            return loc->data;
        }
        else {
            cout << "No word found like -> " << prefix << endl;
            findSuffix(loc, word);
            return NULL;
        }        
    }

   

    void findSuffix(TrieNode* value, string wordString) {
        
        if (!value->map){
            return;
        }

        HashMap<char, TrieNode*>* map = value->map;
        list <Pair<char, TrieNode*>*> *curMap = map->getMap();
        int buckets = map->getBuckets();
        
       
        for (int i = 0; i < buckets; i++) 
        {
            for (Pair<char, TrieNode*>* node : curMap[i])
            {
                TrieNode* trieNode = node->value;
                wordString.push_back(node->key);


                
                if (trieNode->data && suggestCount < 20)
                {
                    suggestionList->push_back(wordString);
                    suggestCount++;
                    findSuffix(trieNode, wordString);
                }
                else 
                {
                    findSuffix(trieNode,wordString);
                }
            }
        }

    }

  
    list<string>* getSuggestionsList() {
        return suggestionList;
    }

   

    void printWord(WordData* value, string wordString) {

        WordData* temp = value;
        while (temp) {
            
            cout << "=====================" << endl;

            
            cout << "WORD: " << wordString << endl;
            
            cout << "PART OF SPEECH: " << temp->partOfSpeech << endl;
            cout << "SYNONYMS: "  << endl;

            stringstream ss(temp->synonyms);
        
            while (ss.good()) {
                string synonym;
                getline(ss, synonym, ';');
                cout << "--> " << synonym << endl;
            }
            temp = temp->next;

            cout << "=====================" << endl;
            
        }
    }

   

    void printSuggestions() {
        cout << "=====================" << endl;
        cout << "SIMILAR WORDS: " << endl;
        cout << "=====================" << endl;

        for ( auto word : *suggestionList) {
            cout << word << endl;
        }

        cout << "=====================" << endl;
        
    }

};




class Controller  {

    public:

    void read_record(Trie* dic) {
 
        fstream fin;     
        fin.open("synonyms.csv", ios::in); 

        if(!fin.is_open()){
            cout <<"Eror Reading the file" << endl;
        }

        else{

            vector<string> row; 
            string line, word, temp; 
            int count = 0;

            getline(fin, line);

            while (fin.good()) { 
                row.clear(); 
         
                getline(fin, line); 
                stringstream s(line); 
                
                while (getline(s, word, ',')) 
                { 
                    // cout << " pushed: " << word;
                    row.push_back(word); 
                }

                dic->insert(row[0], row[1], row[2]);
                count++;
                
            } 
        }
    }


    void create(Trie* dic) { 
      
        ofstream fout; 
    
        
        fout.open("synonyms.csv", ios::app); 
    
        int i, roll, phy, chem, math, bio; 
        string word; 
        string pof; 
        string syns; 
        string temp; 

        int choice = 13;

        cout << "Enter the word: " << endl; 
        getline(cin, word);

        cout << "Enter Part of Speech: " << endl; 
        getline(cin, pof);

        cout << "Enter Synonym: " << endl; 
        getline(cin, syns);

        while (1) {
           
            cout << "Press 1 to add an other synonym" << endl;
            cout << "Press 2 to continue." << endl;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            if (choice == 2) {
                break;
            }
            cout << "Enter the Synonym: " << endl; 
            getline(cin, temp);
            syns.push_back(';');
            syns = syns + temp;
        }

        cout << "Data inserted successfully!" << endl;
        cout << "Print:" << endl;

        cout << word << endl;
        cout << pof << endl;
        cout << syns << endl;
    
    
    
     
        fout << word << "," << pof << "," << syns << "\n"; 

        dic->insert(word, pof, syns);
        
    }  


};



void display(string input, Trie* dic) {
    cout << "=====================" << endl;
   
    cout << "ENTER THE WORD: ";
    
    
    getline(cin, input);
    if (input == "0") {
        abort();
    }
    else {
        dic->printWord(dic->search(input), input);
        
        dic->printSuggestions();

       
        cout << "PRESS ENTER TO CONTINUE. 0 TO EXIT" << endl;
        
    }
}

int main() {

    Trie* dic = new Trie();
    
    Controller ctrl;
    ctrl.read_record(dic);
   

    int choice;
    string input;

    while (1)
    {        
      
       
        cout << " Enter 1 to search a word" << endl;
        cout << " Enter 2 to Add a word" << endl;
        cout << " Enter 0 to exit" << endl;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        
        if (choice == 1) {
            while (getchar() != '0') {
                system("cls");
                display(input, dic);
            }
        }
        else if (choice == 2) {
            ctrl.create(dic);
        }

        else if (choice == 0) {
            abort();
        }
    }

        

}
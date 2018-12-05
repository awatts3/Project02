/*
Andrew Watts
COSC 2030
Project 2
*/

//the base of this vector based tree is taken from www.codeproject.com/Articles/602805/Creating-a-Binary-Search-Tree-B, though modified farily heavily

#include<iostream>
#include<vector>
#include<string>
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;
using std::getline;

struct bst
{
	string ID;
	string PID;
	string rawEvent;
	string Lhash;
	string Rhash;
	vector<string> Lhist;
	vector<string> Rhist;

};

string hash(string word) //I did not make this up, I found it online then modified it to produce 8 character strings rather than its original purpose of 3 digit ints
{
	int seed = 131;
	unsigned int hash = 0;
	for (int i = 0; i < word.length(); i++)
	{
		hash = (hash * seed) + word[i];
	}
	return to_string (hash % 100000000);
}

void makeID(vector<struct bst> &v1, int i) {
	v1[i].ID = hash(v1[i].rawEvent+v1[i].PID); //the hash is being made from the sum of the relavant strings, ie. parentID and raw event for ID
}

void updateParent(vector<struct bst> &v1, int i, int pi) {

	string childhash = hash(v1[i].ID + v1[i].PID + v1[i].rawEvent + v1[i].Lhash + v1[i].Rhash);//sum of all strings aside from histories for L and R hashes

	if (i % 2 == 0) { v1[pi].Rhash = childhash; v1[pi].Rhist.push_back(childhash); } //sets the parent's left or right hash and updates hash hist
	else { v1[pi].Lhash = childhash; v1[pi].Lhist.push_back(childhash);
	}
	
	int gpi; //grandparent index

	if (pi == 1 | pi == 2) { gpi = 0; }
	else if (pi % 2 == 0) { gpi = (pi - 2) / 2; }
	else { gpi = (pi - 1) / 2; }
	if (pi != 0) { updateParent(v1, pi, gpi); }

}



void MakeNode(vector<struct bst> &v1, string aData)
{
	struct bst b1 = { aData };
	v1.push_back(b1);
	int i = v1.size() - 1;
	v1[i].rawEvent = aData;
	v1[i].PID = "NULL";
	makeID(v1,i);
}



void Insert(vector<struct bst> &v1, string aData)
{
	
	if (v1.size() == 0)
	{
		cout << "Note is not made yet. MakeNode first..." << endl;
		return;
	}
	
	struct bst b1 = { aData };
	v1.push_back(b1);

	int index = v1.size() - 1;
	int parentindex;

	if (index == 1|index == 2) { parentindex = 0; }
	else if (index % 2 == 0) { parentindex = (index - 2) / 2; }
	else { parentindex = (index - 1) / 2; }

	//cout << "Index:" << index << "  Parent Index:" << parentindex << endl; //for checking the corectness of index values

	//generate new node values
	v1[index].rawEvent = aData;  
	v1[index].PID = v1[parentindex].ID;
	makeID(v1, index);

	//update parent node values
	updateParent(v1, index, parentindex);

}


void print(vector<struct bst>&v1) {

	cout << endl<<endl;

	for (int i = 0; i < v1.size(); i++) {
		cout << "Node at index " << i << ":"<<endl;
		cout << "ID:" << v1[i].ID << endl;
		cout << "Parent ID:" << v1[i].PID << endl;
		cout << "Raw Event:" << v1[i].rawEvent << endl;
		cout << "Left child hash:" << v1[i].Lhash << endl;
		cout << "Right child hash:" << v1[i].Rhash << endl;
		cout << "Left child hash history:"; 
		for (int j = 0; j < v1[i].Lhist.size(); j++) {
			cout << v1[i].Lhist[j] << ", ";
		}
		cout <<endl<< "Right child hash history:"; 
		for (int j = 0; j < v1[i].Rhist.size(); j++) {
			cout << v1[i].Rhist[j] << ", ";
		}
		cout << endl<<endl;
	}


	//the following were print functions for the sake of testing I'm leaving them in case I need them later

	//for (int i = 0; i < v1.size(); i++) {
	//
	//	//cout << v1[i].rawEvent<<endl; // these are for general making sure nothing broke
	//	//cout << v1[i].ID << endl;

	//	/*for (int j = 0; j < v1[i].Lhist.size(); j++) { //this was to testt that l and R hashes were updating corectly
	//		cout << i << "L:" << v1[i].Lhist[j] << endl;
	//	}
	//	for (int j = 0; j < v1[i].Rhist.size(); j++) {
	//		cout << i << "R:" << v1[i].Rhist[j] << endl;
	//	}*/
	//}
	//cout<<endl;
}




int main()
{
    vector <struct bst> v1;
	string input;
	string command;
	cout << "Enter first event string: ";
	getline(cin, input);
	MakeNode(v1, input);
	while (command!="quit") {
		cout << "Enter command:";
		getline(cin, command);

		if (command == "insert") {

			cout << "Enter event string for new node:";
			getline(cin, input);
			Insert(v1, input);
		}
		else if (command == "display") {
			print(v1);
		}
		else if (command == "update") {
			cout << "Enter index of node to be updated:";
			int i, pi;
			cin >> i;
			if (i >= v1.size() | i < 0) { cout<<"Input is out of range"<<endl; }
			else {
				cout << "Enter new event string: ";
				getline(cin, input);
				v1[i].rawEvent = input;
				makeID(v1, i);
				if (i == 1 | i == 2) { pi = 0; }
				else if (i % 2 == 0) { pi = (i - 2) / 2; }
				else { pi = (i - 1) / 2; }
				updateParent(v1, i, pi);
			}

		}
		else {
		
			cout << "Know commands are: insert, display, update, quit" << endl;

		}

	}

	system("pause");
    return 0;
}
#include<iostream>
#include<fstream>
#include<string>
#include<climits>
using namespace std;

#define TABLE_SIZE 100

template<typename T>
void swap(T* val1,T* val2){
	T temp = *val1;
	*val1 = *val2;
	*val2 = temp;
}
class Subject;
class User;
class Tutor;
class Student;
class UserTree;
class HashTable;
string booltostring(bool val){
	if (val)
		return "true";
		
	else return "false";
}

bool stringtobool(string str){ return str.compare("true") == 0;}

class Subject{
	private:
		string title;
		string code;
		
	public:
		Subject* next;
		
		Subject(){
			title = "";
			code = "";
			next = NULL;
		}
		
		Subject(string title,string code):title(title),code(code){
			next = NULL;
		}
		
		string get_title()const{return title;}
		string get_code()const{return code;}
};

class User{
	protected:
		int key;
		string fname;
		string lname;
		string ID;
	public:
		virtual int getcost()const{
			return 0;
		}
		virtual void setcost(int cost){
		}
		virtual double getrating()const{return 0;}
		virtual void FindTutor(int fmt,HashTable& h,UserTree& tut_tree,int** graph){
			
		}
		virtual void WriteToFile(ofstream& file){
			
		}
		
		virtual void ReadFromFile(ifstream& file){
		}
		
		virtual double UpdateRating(int new_rating){
			return 0;
		}
		
		User* left;
		User* right;
		User(){
			fname = "";
			lname = "";
			ID = "";
			left = NULL;
			right = NULL;
		}
		
		User(string fname,string lname,string ID):fname(fname),lname(lname),ID(ID){
			left = NULL;
			right = NULL;
		}
		
		string getfname()const{return fname;}
		string getlname()const{return lname;}
		string getID()const{return ID;}
		int getkey()const{return key;}
		
		virtual void IncStudentsTaught(){
		}
		
		virtual void print(){
			cout << "Name: " << fname << " " << lname << endl;
			cout << "ID: " << ID << endl;
		}
};

string numtoday(int n){
	switch(n){
		case 0:
			return "Monday";
		case 1:
			return "Tuesday";
		case 2:
			return "Wednesday";
		case 3:
			return "Thursday";
		case 4:
			return "Friday";
		default:
			return " ";
	}
}
class Tutor : public User{
	private:
		static int tut_key;
		double rating;
		int students_taught;
		int cost;
		bool availability[5];
		
	public:
		void setcost(int cost){
			this->cost = cost;
		}
		double getrating()const{
			return rating;
		}
		Subject* sublist;
		Tutor(){
			tut_key++;
			key = tut_key;
			sublist = NULL;
			rating = 0;
			students_taught = 0;
			cost = 0;
			for (int i = 0; i < 5; ++i)
				availability[i] = false;
		}
		
		Tutor(string fname,string lname,string ID,int cost):User(fname,lname,ID),cost(cost){
			tut_key++;
			key = tut_key;
			sublist = NULL;
			rating = 0;
			students_taught = 0;
			for (int i = 0; i < 5; ++i)
				availability[i] = false;
		}
		
		void setavailability(){
			char ch;
			cout << "Please press Y (available) or N (unavailable) to indicate your availability on the following days: " << endl;
			for (int i = 0; i < 5; ++i){
				cout << numtoday(i) << ": ";
				cin>>ch;
				if (ch == 'Y' || ch == 'y') availability[i] = true;
				
			}
		}
		int getcost()const{return cost;}
		
		void AddSub(string title,string code){
			if (!sublist){
				sublist = new Subject(title,code);
				return;
			}
			
			Subject* node = sublist;
			while (node->next)
				node = node->next;
			
			node->next = new Subject(title,code);	
		}
		
		void WriteToFile(ofstream& file)override{
			file << fname << " " << lname << " " << ID << " " << to_string(cost) << " " << to_string(rating) << " " << to_string(students_taught) << endl;
			for (int i = 0; i < 5; ++i)
				 file << booltostring(availability[i]) << " ";
			file << " " << endl;
			Subject* node = sublist;
			while (node){
				file << node->get_title() << " " << node->get_code() << " ";
				node = node->next;
			}
			file << endl;
			file << "null" << " " << "null";
		}
		
		void ReadFromFile(ifstream& file)override{
			string delim,title,code,strbool,strcost,strrating,strstudents;
			if (!file.is_open()){
				cout << "file does not exist";
				return;
			}
			
			file >> fname >> lname >> ID >> strcost >> strrating >> strstudents;
			cost = stoi(strcost);
			rating = stod(strrating);
			students_taught = stoi(strstudents);
			for (int i = 0; i < 5; ++i){
				file >> strbool;
				availability[i] = stringtobool(strbool);
			}
			file >> title >> code;
			while (title.compare("null") != 0 && code.compare("null") != 0){
				AddSub(title,code);
				file >> title >> code;
			}
		}
		
		double UpdateRating(int new_rating){
			rating = (double)rating/students_taught + (double)new_rating/students_taught;
			return rating;
		}
		
		void IncStudentsTaught(){
			students_taught++;
		}
		
		void print(){
			User::print();
			cout << "Cost of tuition: " << cost << endl;
			cout << "Average rating: " << rating << endl;
			cout << "No. of students taught: " << students_taught << endl;
			cout << "Available on: "<<endl;
			for (int i = 0; i < 5; ++i){
				if (availability[i]){
					cout << numtoday(i) << endl;
				}
			}
			cout << "Subjects taught: " << endl;
			Subject* node = sublist;
			while(node){
				cout << node->get_code() << " " << node->get_title() << endl;
				node = node->next;
			}
			cout << endl;
		}
};

class HashNode{
	public:
		Tutor* tut;
		HashNode* next;
		HashNode(){
			tut = NULL;
			next = NULL;
		}
		
		HashNode(Tutor* tut){
			this->tut = tut;
		}
};

class HashTable{
	private:
		HashNode** htable;
	
	public:
		HashTable(){
			htable = new HashNode*[TABLE_SIZE];
			for (int i = 0; i < TABLE_SIZE; ++i)
				htable[i] = NULL;
		}
		
		
		int HashFunc(string code){
			int HV = 0;
			//this function assumes all subject codes have the same format i.e. AB-1234
			for (int i = 0; i < 7; ++i){
				HV += (int)code[i];
			}
			
			return HV % TABLE_SIZE;
		}
		
		void Insert(Tutor* tut){
			Subject* node = tut->sublist;
			while (node){
				int idx = HashFunc(node->get_code());
				if (!htable[idx]){
					htable[idx] = new HashNode(tut);
				} else {
					HashNode* temp = htable[idx];
					while (temp->next){
						temp = temp->next;
					}
					temp->next = new HashNode(tut);	
				}
				node = node->next;
			}
		}
		
		int Find(string code,Tutor* tut_arr){
			int i = 0;
			int idx = HashFunc(code);
			HashNode* temp = htable[idx];
			while(temp){
				if (i == 0){
					tut_arr = new Tutor(temp->tut->getfname(),temp->tut->getlname(),temp->tut->getID(),temp->tut->getcost());
					++i;
				} else {
					++i;
					Tutor* temp2 = new Tutor[i];
					for (int count = 0; count < i - 1; ++count){
						temp2[count] = tut_arr[count];
					}
					temp2[i - 1] = Tutor(temp->tut->getfname(),temp->tut->getlname(),temp->tut->getID(),temp->tut->getcost());
					tut_arr = new Tutor[i];
					for (int count = 0; count < i; ++count){
						tut_arr[count] = temp2[count];
					}
					delete[] temp2;
					temp2 = 0;
				}
				temp = temp->next;
			}
			
			return i;
		}
};

class Student:public User{
	private:
		string recent_tutor_ID;
		static int stu_key;
		double GPA;
	public:
		Student(){
			stu_key++;
			key = stu_key;
			GPA = 0;
			recent_tutor_ID = "";
		}
		
		Student(string fname,string lname,string ID,double GPA):User(fname,lname,ID),GPA(GPA){
			stu_key++;
			key = stu_key;
			recent_tutor_ID = "";
		}
		
		double get_GPA()const{return GPA;}
		
		void print(){
			User::print();
			cout << "GPA: " << GPA << endl;
			cout << endl;
		}
		
		void WriteToFile(ofstream& wfile){
			wfile << fname << " " << lname << " " << ID << " " << GPA << " " << recent_tutor_ID << endl;
		}
		
		void ReadFromFile(ifstream& rfile){
			string strgpa;
			rfile >> fname >> lname >> ID >> strgpa >> recent_tutor_ID;
			GPA = stod(strgpa);
		}
		
		void AskRating(UserTree& tut_tree);
		
		void FindTutor(int fmt,HashTable& h,UserTree& tut_tree,int** graph);
};

void min_heapify(Tutor* tut_arr,int size,int idx){
	int l_child = 2 * idx + 1;
	int r_child = 2 * idx + 2;
	int min = idx;
	
	if (l_child < size && tut_arr[l_child].getcost() < tut_arr[min].getcost()){
		min = l_child;
	}
	
	if (r_child < size && tut_arr[r_child].getcost() < tut_arr[min].getcost()){
		min = r_child;
	}
	
	if (min != idx){
		swap(tut_arr[min],tut_arr[idx]);
		min_heapify(tut_arr,size,min);
	}
	
}

class StackNode{
	public:
		User* u;
		StackNode* next;
		
		StackNode(User* u){
			this->u = u;
			next = NULL;
		}
};
class UserStack{
	private:
		StackNode* top;
	public:
		UserStack(){
			top = NULL;
		}
		
		bool IsEmpty(){
			if (top) return true;
			
			return false;
		}
		
		void push(User* u){
			StackNode* node = new StackNode(u);
			if (!IsEmpty()){
				top = node;
			} else {
				StackNode* temp = top;
				while (temp->next)
					temp = temp->next;
				temp->next = node;
			}
		}
		
		void pop(){
			StackNode* temp = top;
			StackNode* prev = NULL;
			while (temp->next){
				prev = temp;
				temp = temp->next;
			}
			prev->next = NULL;
			delete temp;
			temp = 0;
		}
		
		StackNode* peek(){
			StackNode* temp = top;
			while (temp->next){
				temp = temp->next;
			}
			return temp;
		}
};

class QNode{
	public:
		User* u;
		QNode* next;
		
		QNode(User* u){
			this->u = u;
			next = NULL;
		}
};
class Queue{
	private:
		QNode* front;
		QNode* rear;
	public:
		Queue(){
			front = NULL;
			rear = NULL;
		}
		
		bool IsEmpty(){
			if (front) return true;
			
			return false;
		}
		
		void enqueue(User* u){
			QNode* q = new QNode(u);
			if (IsEmpty()){
				front = q;
				rear = q;
			} else {
				rear->next = q;
				rear = rear->next;
			}
		}
		
		void dequeue(){
			QNode* temp = front;
			front = front->next;
			temp->next = NULL;
			delete temp;
			if (!front){
				rear = NULL;
			}
		}
		
		User* peek(){
			return front->u;
		}
};

void minheap(Tutor* tut_arr,int size){
	for (int i = (size - 1)/2; i >=0; --i){
		min_heapify(tut_arr,size,i);
	}
}

void deletemin(Tutor* tut_arr,int size){
	swap(tut_arr[0],tut_arr[size - 1]);
	size--;
	minheap(tut_arr,size);
}

int Tutor::tut_key = 0;
int Student::stu_key = 0;

class UserTree{
	private:
		int GetHeight(User* node){
			if (!node)
				return -1;
				
			int l_height = GetHeight(node->left) + 1;
			int r_height = GetHeight(node->right) + 1;
			
			return max(l_height,r_height);
		}
		
		int GetBalance(User* node){
			return GetHeight(node->left) - GetHeight(node->right);
		}
		
		User* leftrotate(User* x){
			User* y = x->right;
			User* T2 = y->left;
			
			y->left = x;
			x->right = T2;
			
			return y;
		}
		
		User* rightrotate(User* x){
			User* y = x->left;
			User* T2 = y->right;
			
			y->right = x;
			x->left = T2;
			
			return y;
		}
		
	public:
		User* root;
		
		UserTree(){
			root = NULL;
		}
		
		User* Search(User* node,string ID){
			if (!node)
				return NULL;
			
			if (ID.compare(node->getID()) < 0)
				return Search(node->left,ID);
				
			if (ID.compare(node->getID()) > 0)
				return Search(node->right,ID);
				
			return node;
		}
		
		User* Insert(User* node,User* t){
			if (!node){
				return t;
			}
			
			if (t->getID().compare(node->getID()) < 0){
				node->left = Insert(node->left,t);
			} else if (t->getID().compare(node->getID()) > 0){
				node->right = Insert(node->right,t);
			}
			
			int balance = GetBalance(node);
			
			if (balance > 1){
				if (t->getID().compare(node->left->getID()) < 0){
					return rightrotate(node);
				} else if (t->getID().compare(node->left->getID()) > 0){
					node->left = leftrotate(node->left);
					return rightrotate(node);
				}
			} else if (balance < -1){
				if (t->getID().compare(node->right->getID()) > 0){
					return leftrotate(node);
				} else if (t->getID().compare(node->right->getID()) < 0){
					node->right = rightrotate(node->right);
					return leftrotate(node);
				}
			}
			
			return node;
		}
		
		void InOrderPrint(User* node){
			if (!node){
				return;
			}
			InOrderPrint(node->left);
			node->print();
			InOrderPrint(node->right);
		}
		
		void InOrderWrite(User* node,ofstream& wfile){
			if (!node)
				return;
			InOrderWrite(node->left,wfile);
			node->WriteToFile(wfile);
			InOrderWrite(node->right,wfile);
		}
};

void Student::AskRating(UserTree& tut_tree){
	int rating;
	User* tut = tut_tree.Search(tut_tree.root,recent_tutor_ID);
	cout << "Please give a rating to your previous tutor (" << tut->getfname() << " " << tut->getlname() << ", " << tut->getID() << ": ";
	cin >> rating;
	tut->UpdateRating(rating);
}

void Student::FindTutor(int fmt,HashTable& h,UserTree& tut_tree,int** graph){
	string code,ID;
	cout << "Please enter code of subject you are seeking tuitions for: ";
	cin >> code;
	Tutor* tut_arr;
	int size = h.Find(code,tut_arr);
	minheap(tut_arr,size);
	if (fmt == 1){
		cout << "Printing all tutors in ascending cost of tuition..." << endl;
		for (int i = 0; i < size; ++i){
			tut_arr[i].print();
		}
	} else {
		int max_cost;
		cout << "Please enter your max. cost for tuition: ";
		cin >> max_cost;
		cout << "Printing tutors meeting cost requirements in ascending cost of tuition..." << endl;
		for (int i = 0; tut_arr[0].getcost() <= max_cost; ++i){
			tut_arr[0].print();
			deletemin(tut_arr,size);
			size--;
		}
	}
	cout << "Please enter the ID of the student you want as a study buddy: ";
	cin >> ID;
	User* studbud = tut_tree.Search(tut_tree.root,ID);
	graph[key][studbud->getkey()] = 1;
	studbud->IncStudentsTaught();
	recent_tutor_ID = studbud->getID();
}

//increase size of graph when new student added
void extendgraph_stu(int** graph,int stu_count,int tut_count,ifstream& gfile){
	if(!gfile.is_open()) return;
	
	string val;
	graph = new int*[tut_count];
	for (int i = 0; i < tut_count; ++i){
		graph[i] = new int[stu_count];
		for (int j = 0; j < stu_count - 1; ++j){
			gfile >> val;
			graph[i][j] = stoi(val);
		}
		graph[i][stu_count - 1] = 0;
	}
	gfile.close();
}

//increase size of graph when new tutor added
void extendgraph_tut(int** graph,int stu_count,int tut_count,ifstream& gfile){
	if(!gfile.is_open()) return;
	string val;
	graph = new int*[tut_count];
	for (int i = 0; i < tut_count - 1; ++i){
		graph[i] = new int[stu_count];
		for (int j = 0; j < stu_count; ++j){
			gfile >> val;
			graph[i][j] = stoi(val);
		}
	}
	graph[tut_count - 1] = new int[stu_count]{0};
	gfile.close();
}

User* FindByKey(int key,UserStack& s){
	while (!s.IsEmpty()){
		User* u = s.peek()->u;
		if (u->getkey() == key)
			return u;
		s.pop();
	}
	return NULL;
}

void GiveRecommendations(UserStack& s2,UserStack& s1,int** graph,int src,int tut_count,int stu_count){
	Queue q1,q2;
	for (int i = 0; i < tut_count; ++i){
		if (graph[i][src])
			q1.enqueue(FindByKey(i,s1));
	}
	while (!q1.IsEmpty()){
		User* u = q1.peek();
		for (int i = 0; i < stu_count; ++i){
			if (graph[u->getkey()][i])
				q2.enqueue(FindByKey(i,s2));
		}
		s1.push(u);
		q1.dequeue();
	}
	while(!q2.IsEmpty()){
		User* u2 = q2.peek();
		for (int i = 0; i < tut_count; ++i){
			if(graph[i][u2->getkey()] && !graph[i][src]){
				q1.enqueue(FindByKey(i,s1));
			}
		}
		s2.push(u2);
		q2.dequeue();
	}
	while (!q1.IsEmpty()){
		User* u3 = q1.peek();
		u3->print();
		s1.push(u3);
		q1.dequeue();
	}
}



int main(){
	HashTable h;
	UserTree tut_tree,stu_tree;
	UserStack s1,s2;
	int tut_count = 0,stu_count = 0;
	User* temp;
	ifstream rfile("F:\\Tutors.txt");
	if (rfile.is_open()){
		while(!rfile.eof()){
			temp = new Tutor();
			temp->ReadFromFile(rfile);
			s1.push(temp);
			tut_tree.root = tut_tree.Insert(tut_tree.root,temp);
			tut_count++;
			h.Insert(dynamic_cast<Tutor*>(temp));
		}
	}
	rfile.close();
	ifstream rfile2("F:\\Students.txt");
	if (rfile2.is_open()){
		while(!rfile2.eof()){
			temp = new Student();
			temp->ReadFromFile(rfile2);
			s2.push(temp);
			stu_tree.root = stu_tree.Insert(stu_tree.root,temp);
			stu_count++;
		}
	}
	rfile2.close();
	cout << stu_count << " " << tut_count << endl;
	ifstream gfile1("F:\\graph.txt");
	int** graph;
	
	if (tut_count != 0 && stu_count != 0){
		graph = new int*[tut_count];
		for (int i = 0; i < tut_count; ++i)
			graph[i] = new int[stu_count];
		if (gfile1.is_open()){
			string val;
			for (int i = 0; i < tut_count; ++i){
				for (int j = 0; j < tut_count; ++j){
					gfile1 >> val;
					graph[i][j] = stoi(val);
				}
			}
		}
	}
	gfile1.close();
	string fname,lname,ID,title,code;
	int cost,choice1,choice2;
	double GPA;
	cout << "Press 1 if you have an existing account: ";
	cin >> choice1;
	if (choice1 == 1){
		cout << "Enter your ID: ";
		cin >> ID;
		User* user = stu_tree.Search(stu_tree.root,ID);
		if (user){
			cout << "Welcome back " << user->getfname() << " " << user->getlname() << endl;
			cout << "TUTORS FOR YOU: " << endl;
			GiveRecommendations(s2,s1,graph,user->getkey(),tut_count,stu_count);
			cout << "Please enter key to select one of the following options:\n1 = Search by subject\n2 = Search by subject and cost: ";
			cin >> choice2;
			user->FindTutor(choice2,h,tut_tree,graph);
		} else {
			user = tut_tree.Search(tut_tree.root,ID);
			if (user){
				cout << "Welcome back " << user->getfname() << " " << user->getlname() << endl;
				if (user->getrating() != 0){
					cout << "Your rating is " << user->getrating() <<". Do you wish to increase your cost by 50\? Press 1 if yes: ";
					cin>>choice2;
					if (choice2 == 1){
						user->setcost(user->getcost() + 50);
					}
				}
			} else {
				cout << "Your account does not exist. Please create a new account or try again." << endl;
				exit(0);
			}
		}
	} else {
		cout << "Are you a student or tutor\? Press 1 if student, any other key for tutor: ";
		cin >> choice2;
		cout << "Please enter your first name: ";
		cin >> fname;
		cout << "Please enter your last name: ";
		cin >> lname;
		cout << "Please enter your ID: ";
		cin >> ID;
		if (choice2 == 1){
			cout << "Please enter your GPA: ";
			cin >> GPA;
			Student* stu = new Student(fname,lname,ID,GPA);
			stu_tree.root = stu_tree.Insert(stu_tree.root,stu);
			stu_count++;
			ifstream gfile2("F:\\graph.txt");
			if (tut_count != 0 && stu_count != 0)
				extendgraph_stu(graph,stu_count,tut_count,gfile2);
			cout << "Welcome " << stu->getfname() << " " << stu->getlname() << "!" << endl;
			cout << "Please enter key to select one of the following options:\n1 = Search by subject\n2 = Search by subject and cost: ";
			cin >> choice2;
			stu->FindTutor(choice2,h,tut_tree,graph);
		} else {
			cout << "Please enter your cost for tuition: ";
			cin >> cost;
			Tutor* tut = new Tutor(fname,lname,ID,cost);
			tut_count++;
			cout << "Please enter which subjects you are teaching: ";
			int choice3 = 1;
			while (choice3 == 1){
				cout << "Subject title: "; cin >> title;
				cout << "Subject code: "; cin >> code;
				tut->AddSub(title,code);
				cout << "Press 1 to continue entering subject information...";
				cin >> choice3;
			}
			tut->setavailability();
			ifstream gfile3("F:\\graph.txt");
			if (tut_count != 0 && stu_count != 0)
				extendgraph_tut(graph,stu_count,tut_count,gfile3);
			tut_tree.root = tut_tree.Insert(tut_tree.root,tut);
		}
	}
	if (stu_count != 0){
		ofstream wfile("F:\\Students.txt");
		stu_tree.InOrderWrite(stu_tree.root,wfile);
	}
	if (tut_count != 0){
		ofstream wfile1("F:\\Tutors.txt");
		tut_tree.InOrderWrite(tut_tree.root,wfile1);
	}
	ifstream gfile4("F:\\graph.txt");
	if (!gfile4.is_open() && tut_count != 0 && stu_count != 0){
		graph = new int*[tut_count];
		for (int i = 0; i < tut_count; ++i){
			graph[i] = new int[stu_count] {0};
		}
		ofstream wfile2("F:\\graph.txt");
		for (int i = 0; i < tut_count; ++i){
			for (int j = 0; j < stu_count; ++j){
				wfile2 << graph[i][j] << endl;
			}
		}
	} else if (gfile4.is_open()){
		gfile4.close();
		ofstream wfile2("F:\\graph.txt");
		for (int i = 0; i < tut_count; ++i){
			for (int j = 0; j < stu_count; ++j){
				wfile2 << graph[i][j] << endl;
			}
		}
	}	
}

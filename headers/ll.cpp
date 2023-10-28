# include <iostream>
# include <string>
using namespace std;

class node{
    public:
        string stock;
        int price;
        node* next;

        node(string a, int x){
            this->price = x;
            this->stock = a;
            this->next = nullptr;
        }
};

class LinkedList{
    private:
        node* head;
    
    public:
        LinkedList(){
            this->head = nullptr;
        }

        void pb(string a, int x){
            if (head == nullptr){
                head = new node(a,x);
            }

            else{
                node* tmp = head;
                while (tmp->next != nullptr) tmp = tmp->next;
                tmp->next = new node(a,x);
            }
        }

        int pop(){                                                          //returns the last element price and deletes it
            if (head == nullptr) return -1;
            if (head->next == nullptr) {
                int a = head->price;
                delete head;
                head = nullptr;
                return a;
            }
            node* tmp = head;
            while(tmp->next->next != nullptr) tmp = tmp->next;
            int a = tmp->next->price;
            delete tmp->next;
            tmp->next = nullptr;
            return a;
        }

        void print(){
            node* tmp = head;
            if (this->size() == 0) {cout<<"No elements"; return;}
            while (tmp->next != nullptr) {
                cout << tmp->stock<<": "<<tmp->price<<endl;
                tmp = tmp->next;
            }
            cout << tmp->stock<<": "<<tmp->price<<endl;

        }

        int size(){
            int i = 1;
            node* tmp = head;
            if (head == nullptr) return 0;
            while (tmp->next != nullptr) {
                i++;
                tmp = tmp->next;
            }return i;
        }

        void del(){
            while (head != nullptr) this->pop();
        }

};

int main () {
LinkedList ll;
ll.pb("a",1);
ll.pb("b",2);
ll.pb("c",3);
ll.pb("d",4);
ll.print();
ll.del();
ll.print();
}
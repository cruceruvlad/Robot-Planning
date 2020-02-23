  // Copyright [2018] Cruceru Vlad

#ifndef DEQUEUE_H_
#define DEQUEUE_H_

#include <iostream>
#include <string>

  // Structura ce cuprinde informatia fiecarei comenzi
  // CmdType=tipul comenzii,id=id-ul robotului,x,y,NrBoxes semnif din enunt
  // time=timpul petrecut in coada, priority=prioritatea comenzii
struct Data{
    std::string CmdType;
    int id , x , y , NrBoxes , time = 0 , priority;
    Data(){
    }
    ~Data(){
    }
};

template < typename T >
struct Node{
    T data;
    Node <T> *next;
    Node <T> *prev;
    explicit Node(T date)
    {
        this->data = date;
        next = nullptr;
        prev = nullptr;
    }
    ~Node(){
    }
};

template < typename T >
class Dequeue{
 private:
        Node <T> *head;
        Node <T> *tail;
        int numElements;

 public:
        int NrBoxes = 0;  // O variabila cu Nr de cutii ce sunt carate la mom
        Dequeue() {                                   // respectiv de robot
            head = nullptr;
            tail = nullptr;
            numElements = 0;
        }

        explicit Dequeue(Node <T> *head) {
            this->head = head;
            while (head != nullptr) {
                numElements++;
                head = head->next;
            }
        }

        ~Dequeue() {
            while (head != nullptr) {
                Node <T> *temp = head;
                head = head->next;
                delete temp;
            }
        }

        int size() {
            return numElements;
        }

        void addLast(T data) {
            Node <T> *add_Node = new Node<T>(data);
                if (numElements == 0) {
                    head = add_Node;
                    tail = add_Node;
                } else {
                    tail->next = add_Node;
                    add_Node->prev = tail;
                }

                tail = add_Node;
                numElements++;
            }

            void addFirst(T data){
                Node<T> *add_Node = new Node <T>(data);
                if (numElements == 0) {
                    head = add_Node;
                    tail = add_Node;
                } else {
                    head->prev = add_Node;
                    add_Node->next = head;
                }

                head = add_Node;
                numElements++;
            }

        T removeFirst() {
            T return_data = head->data;
            Node <T> *temp = head;
            head = head->next;
            if (numElements != 1)
                head->prev = nullptr;
            delete temp;
            numElements--;
            if (numElements == 0) {
                tail = head;
            }
            return return_data;
        }

        T removeLast() {
            T return_data = tail->data;
            Node <T> *temp = tail;
            tail = tail->prev;
            if (numElements != 1)
                tail->next = nullptr;
            delete temp;
            numElements--;
            if (numElements == 0) {
                head = tail;
            }
            return return_data;
        }

  // O functie care mareste timpul petrecut in coada al fiecarui nod cu 1
        void timer() {
            int count = 1;
            Node<T> *temp = head;

            while (count <= numElements) {
                temp->data.time++;
                temp = temp->next;
                count++;
            }
        }
    template < typename U >
    friend std::ostream& operator << (std::ostream& os , Dequeue<U>& lista);
};

    template < typename T >
    std::ostream& operator << (std::ostream& os , Dequeue<T>& lista) {
            Node <T> *it = lista.head;
            if (lista.size() > 0) {
		os << it->data.id << ":";
		while (it->next != nullptr) {
                os << " "<< it->data.CmdType <<" " << it->data.x << " " <<
		it->data.y << " " << it->data.NrBoxes << ";";
                it = it->next;
        }

        os << " " << it->data.CmdType << " " << it->data.x << " " << it->data.y
	 << " " << it->data.NrBoxes;
    } else {
        os << "No command found";
    }

    return os;
}
#endif  // DEQUEUE_H_

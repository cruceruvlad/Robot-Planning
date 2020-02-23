  // Copyright [2018] Cruceru Vlad
#ifndef STACK_H_
#define STACK_H_
#include "Dequeue.h"

template < typename T >
class Stack {
 private:
        T *Array;
        int numElements , Capacity , multiplicator = 2;
        void doubleCapacity() {
            if (numElements == Capacity) {
                Capacity *= multiplicator;

                 T *newArray = new T[Capacity];

                for (int i = 0 ; i < numElements ; i++) {
                    newArray[i] = Array[i];
                }

                delete[] Array;
                Array = newArray;
            }
        }
        void divideCapacity() {
            if (numElements <= Capacity / 2) {
                Capacity /= 2;
                if (Capacity == 0)
                    Capacity = 1;
            }

            T *newArray = new T[Capacity];

            for (int i = 0 ; i < numElements ; i++) {
                newArray[i] = Array[i];
            }

            delete[] Array;
            Array = newArray;
        }

 public:
        Stack()
        {
            numElements = 0;
            Capacity = 1;
            Array = new T[Capacity];
        }

        ~Stack() {
            delete[] Array;
        }

        void push(T x) {
            doubleCapacity();
            numElements++;
            Array[numElements - 1] = x;
        }

        T pop() {
            T return_data = Array[numElements - 1];
            numElements--;
            divideCapacity();
            return return_data;
        }

        T peek() {
            return Array[numElements - 1];
        }

        bool isEmpty() {
            return numElements == 0;
        }

        int size() {
            return numElements;
        }
};
#endif  // STACK_H_

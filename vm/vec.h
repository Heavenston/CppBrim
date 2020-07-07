#pragma once
#include <cstdlib>
#include "brim.h"

namespace brim {

    template<class T>
    class Vec {
    private:
        usize length = 0;
        usize capacity = 0;

        T* content = nullptr;

        void growCapacity(usize newCapacity) {
            if (newCapacity == 0) {
                free(content);
                content = nullptr;
                return;
            }

            capacity = newCapacity;
            content = (T*) realloc(content, newCapacity * sizeof(T));
            if (content == nullptr) exit(1);
        }

    public:

        usize getCapacity() {
            return capacity;
        }

        usize getLength() {
            return length;
        }

        void push(T value) {
            if (capacity <= length) {
                growCapacity(capacity+10);
            }

            content[length] = value;
            length++;
        }

        T& pop() {
            length--;
            return content[length];
        }

        T last() {
            return content[length-1];
        }
        T first() {
            return content[0];
        }

        T& operator[](usize t) {
            return content[t];
        }
    };

}
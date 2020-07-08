#pragma once
#include <cstdlib>
#include "common.h"

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

        T *get_pointer() {
            return content;
        }
        const T *get_pointer() const {
            return content;
        }

        usize get_capacity() const {
            return capacity;
        }
        usize get_length() const {
            return length;
        }

        void ensure_capacity(usize ncapacity) {
            if (capacity >= ncapacity) return;
            growCapacity(ncapacity);
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

        T& last() {
            return content[length-1];
        }
        const T& last() const {
            return content[length-1];
        }
        T& first() {
            return content[0];
        }
        const T& first() const {
            return content[0];
        }

        T& peek(usize distance) {
            return content[length-distance-1];
        }
        const T& peek(usize distance) const {
            return content[length-distance-1];
        }

        T& operator[](usize t) {
            return content[t];
        }
        const T& operator[](usize t) const {
            return content[t];
        }
    };

}
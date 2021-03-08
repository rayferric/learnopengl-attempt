#ifndef EYESPARK_MAP_H
#define EYESPARK_MAP_H

#include "../util/Pair.h"

template <class K, class V>
class Map {
private:
    int size;
    int capacity;
    Pair<K, V> *buffer;

    void reserve(int capacity) {
        Pair<K, V> *newBuffer = new Pair<K, V>[capacity];
        copy(newBuffer, buffer, size);
        delete[] buffer;
        buffer = newBuffer;
        this->capacity = capacity;
    }
    int find(const K &key) {
        for(int i=0; i<size; i++) {
            if(buffer[i].first == key)return i;
        }
        return -1;
    }
    V &get(const K &key) {
        int index;
        if((index = find(key)) >= 0)return buffer[index].second;
        if(size == capacity)reserve(2 * capacity);
        buffer[size++] = Pair<K, V>(key, V());
        return buffer[size - 1].second;
    }

    static void copy(Pair<K, V> *dst, Pair<K, V> *src, int size) {
        for(int i=0; i<size; i++) {
            dst[i] = src[i];
        }
    }
public:
    Map() {
        size = 0;
        capacity = 1;
        buffer = new Pair<K, V>[capacity];
    }
    Map(const Map<K, V> &map) {
        size = map.size;
        capacity = map.capacity;
        buffer = new Pair<K, V>[capacity];
        copy(buffer, map.buffer, size);
    }

    inline Map<K, V> &operator=(const Map<K, V> &rhs) {
        size = rhs.size;
        capacity = rhs.capacity;
        delete[] buffer;
        buffer = new Pair<K, V>[capacity];
        copy(buffer, rhs.buffer, size);
        return *this;
    }
    inline V operator[](const K &rhs) const {
        return get(rhs);
    }
    inline V &operator[](const K &rhs) {
        return get(rhs);
    }

    Pair<K, V> *begin() { return buffer; }
    Pair<K, V> *end() { return buffer + size; }
    Pair<K, V> &front() { return buffer[0]; }
    Pair<K, V> &back() { return buffer[size - 1]; }
    int length() const {
        return size;
    }
    void clear() {
        size = 0;
        capacity = 1;
        delete[] buffer;
        buffer = new Pair<K, V>[capacity];
    }
    bool has(const K &key) {
        return find(key) >= 0;
    }
    bool remove(const K &key) {
        int index;
        if((index = find(key)) >= 0) {
            Pair<K, V> *ptr = &buffer[index];
            (*ptr).~Pair<K, V>();
            copy(ptr, ptr+1, (--size-index));
            return true;
        }
        return false;
    }

    ~Map() {
        delete[] buffer;
    }
};

#endif // EYESPARK_MAP_H

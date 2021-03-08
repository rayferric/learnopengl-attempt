#ifndef EYESPARK_LIST_H
#define EYESPARK_LIST_H

template <class T>
class List {
private:
    int size;
    int capacity;
    T *buffer;

    void reserve(int capacity) {
        T *newBuffer = new T[capacity];
        copy(newBuffer, buffer, size);
        delete[] buffer;
        buffer = newBuffer;
        this->capacity = capacity;
    }

    static void copy(T *dst, T *src, int size) {
        for(int i=0; i<size; i++) {
            dst[i] = src[i];
        }
    }
public:
    List() {
        size = 0;
        capacity = 1;
        buffer = new T[capacity];
    }
    List(const List<T> &list) {
        size = list.size;
        capacity = list.capacity;
        buffer = new T[capacity];
        copy(buffer, list.buffer, size);
    }

    inline List<T> &operator=(const List<T> &rhs) {
        size = rhs.size;
        capacity = rhs.capacity;
        delete[] buffer;
        buffer = new T[capacity];
        copy(buffer, rhs.buffer, size);
        return *this;
    }
    inline T operator[](int rhs) const {
        return buffer[rhs];
    }
    inline T &operator[](int rhs) {
        return buffer[rhs];
    }

    T *begin() { return buffer; }
    T *end() { return buffer + size; }
    T &front() { return buffer[0]; }
    T &back() { return buffer[size - 1]; }
    T *data() {
        return buffer;
    }
    int length() const {
        return size;
    }
    void clear() {
        size = 0;
        capacity = 1;
        delete[] buffer;
        buffer = new T[capacity];
    }
    int find(const T &element) {
        for(int i=0; i<size; i++) {
            if(buffer[i] == element)return i;
        }
        return -1;
    }
    void add(const T &element) {
        if(size == capacity)reserve(2 * capacity);
        buffer[size++] = element;
    }
    bool remove(int index) {
        if(index < 0 || index >= size)return false;
        T *ptr = &buffer[index];
        (*ptr).~T();
        copy(ptr, ptr+1, (--size-index));
        return true;
    }

    ~List() {
        delete[] buffer;
    }
};

#endif // EYESPARK_LIST_H

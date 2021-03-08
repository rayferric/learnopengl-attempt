#ifndef EYESPARK_PAIR_H
#define EYESPARK_PAIR_H

template <class F, class S>
class Pair {
public:
    F first;
    S second;

    Pair() {}
    Pair(F first, S second) {
        this->first = first;
        this->second = second;
    }
};

#endif // EYESPARK_PAIR_H

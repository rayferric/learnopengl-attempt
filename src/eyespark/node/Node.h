#ifndef NODE_H
#define NODE_H

#include "../util/List.h"
#include "../util/Matrix4.h"
#include "../util/Vector3.h"

class Node {
private:
    Node *parent = nullptr;
    List<Node *> children;
    bool visible = true;
    Matrix4 transform = Matrix4(1);
public:
    Vector3 translation = Vector3(0);
    Vector3 rotation = Vector3(0);
    Vector3 scale = Vector3(1);

    Node() {}

    void setParent(Node *parent) {
        this->parent = parent;
        parent->children.add(this);
    }
    Node *getParent() {
        return parent;
    }
    List<Node *> getChildren() {
        return children;
    }
    Matrix4 getGlobalTransform() {
        if(parent != nullptr)return parent->getGlobalTransform() * transform;
        return transform;
    }
    void updateTransform() {
        transform = Matrix4::transform(translation, rotation, scale);
    }
    void setVisible(bool visible) {
        this->visible = visible;
    }
    bool isVisible() {
        if(parent != nullptr && visible)return parent->isVisible();
        return visible;
    }
    virtual void init() {}
    virtual void process(double delta) {}
    virtual void input() {}

    virtual ~Node() {}
};

#endif

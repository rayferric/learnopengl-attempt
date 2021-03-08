#ifndef EYESPARK_INPUT_H
#define EYESPARK_INPUT_H

#include "../util/List.h"
#include "../util/Map.h"
#include "../util/String.h"

class Input {
private:
    static Map<String, List<int>> actions;
    static Map<String, bool> pressed;
    static Map<String, bool> released;
public:
    static void addActionKey(String name, int key) {
        actions[name].add(key);
    }
    static bool isActionPressed(String action) {
        if(!actions.has(action))return false;
        for(int code : actions[action]) {
            if(GetKeyState(code) & 0x8000)return true;
        }
        return false;
    }
    static bool isActionJustPressed(String action) {
        if(isActionPressed(action)) {
            if(!pressed.has(action) || !pressed[action]) {
                pressed[action] = true;
                return true;
            }
        } else pressed[action] = false;
        return false;
    }
    static bool isActionJustReleased(String action) {
        if(!isActionPressed(action)) {
            bool has = released.has(action);
            bool state = released[action];
            if(!has || !state)released[action] = true;
            if(has && !state)return true;
        } else released[action] = false;
        return false;
    }
};

Map<String, List<int>> Input::actions;
Map<String, bool> Input::pressed;
Map<String, bool> Input::released;

#endif // EYESPARK_INPUT_H

#ifndef PLAYER_H
#define PLAYER_H

class Player : public EyeSpark::PerspectiveCamera {
private:
    float speed = 1.0;
public:
    Player() : PerspectiveCamera(70, 0.01, 100) {}

    void init() override {
        EyeSpark::Window::setCursorVisible(false);
    }

    void process(double delta) override {
        if(!EyeSpark::Window::hasFocus())return;
        int posX, posY;EyeSpark::Window::getCursorPos(&posX, &posY); //get cursor position
        int width, height;EyeSpark::Window::getClientSize(&width, &height); //get window size
        EyeSpark::Window::setCursorPos(width/2, height/2); //reset cursor position
        int relativeX = posX - width/2; int relativeY = posY - height/2; //calculate cursor position relative to center

        rotation.x += 0.07 * -relativeY;
        rotation.y += 0.07 * -relativeX;
        if(rotation.x < -90)rotation.x = -90;
        else if(rotation.x > 90)rotation.x = 90;
        if(rotation.y < -180)rotation.y += 360;
        else if(rotation.y > 180)rotation.y -= 360;



        EyeSpark::Vector3 dir(0);
        if(EyeSpark::Input::isActionPressed("forward"))dir.z--; //W
        if(EyeSpark::Input::isActionPressed("backward"))dir.z++; //S
        if(EyeSpark::Input::isActionPressed("left"))dir.x--; //A
        if(EyeSpark::Input::isActionPressed("right"))dir.x++; //D
        if(EyeSpark::Input::isActionPressed("jump"))dir.y++; //SPACE
        if(EyeSpark::Input::isActionPressed("snake")){dir.y--; dir.x/=2; dir.z/=2;} //LSHIFT

        float yaw = EyeSpark::Math::radians(rotation.y);
        float s = EyeSpark::Math::sin(yaw);
        float c = EyeSpark::Math::cos(yaw);

        // FORWARD & BACKWARD MOVEMENT
        translation.x += delta * speed * dir.z * s;
        translation.z += delta * speed * dir.z * c;
        // LEFT & RIGHT MOVEMENT
        translation.x += delta * speed * dir.x * c;
        translation.z += delta * speed * dir.x * -s;
        // UP & DOWN MOVEMENT
        translation.y += delta * speed * dir.y;
        updateTransform();
    }
};

#endif // PLAYER_H

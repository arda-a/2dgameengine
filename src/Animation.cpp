#include "./Animation.h"

Animation::Animation(){

}

Animation::Animation(unsigned int index, unsigned int numFrames, unsigned int animationSpeed){
    this->Index = index;
    this->NumFrames = numFrames;
    this->AnimationSpeed = animationSpeed;
}
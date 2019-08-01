#pragma once

class Animation {
 public:
  unsigned int Index;
  unsigned int NumFrames;
  unsigned int AnimationSpeed;
  Animation();
  Animation(unsigned int index, unsigned int numFrames,
            unsigned int animationSpeed);
};
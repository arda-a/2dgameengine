#include "./Collision.h"

bool Collision::CheckRectangleCollision(const SDL_Rect& recA,
                                        const SDL_Rect& recB) {
  return (recA.x + recA.w >= recB.x && recB.x + recB.w >= recA.x &&
          recA.y + recA.h >= recB.y && recB.y + recB.h >= recA.y);
}
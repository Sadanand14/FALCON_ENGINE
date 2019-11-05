#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "BasicComponent.h"
#include "Rendering/PipeLine/Animation.h"

/**
*Structure Definition for holding data needed for animating the entity.
*/
struct AnimationComponent: public BasicComponent
{
	Animation* anim = nullptr;
	AnimationComponent() {}
	~AnimationComponent() {}
};

#endif // !1
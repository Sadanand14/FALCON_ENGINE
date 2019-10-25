#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
private:
	ozz::animation::Animation m_anim;
	ozz::animation::Skeleton m_skel;
	ozz::animation::SamplingCache m_cache;
	ozz::Vector<ozz::math::SoaTransform>::Std m_locals;
	ozz::Vector<ozz::math::Float4x4>::Std m_models;
public:

}

#endif
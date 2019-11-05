#ifndef ANIMATION_H
#define ANIMATION_H

#include <ozz/base/log.h>
#include <ozz/base/containers/vector.h>
#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/maths/vec_float.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/skeleton.h>

class Animation
{
//private:
public:
// 	ozz::sample::PlaybackController m_controller;
	ozz::animation::Animation m_anim;
	ozz::animation::Skeleton m_skel;
	ozz::animation::SamplingCache m_cache;
	ozz::Vector<ozz::math::SoaTransform>::Std m_locals;
	ozz::Vector<ozz::math::Float4x4>::Std m_models;
};

#endif
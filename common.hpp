/*
    Ichigo! A simple, from scratch, minimal dependency game engine for 2D side scrolling games.

    Commonly used types and macros.

    Author:      Braeden Hong
    Last edited: 2025/03/24
*/

#pragma once

#include "bana/bana.hpp"
#include "bana/bana_platform.hpp"

#define GL_CALL(_CALL) do { Ichigo::Internal::gl._CALL; GLenum gl_err = Ichigo::Internal::gl.glGetError(); if (gl_err != 0) ICHIGO_ERROR("GL error 0x%x returned from '%s'.", gl_err, #_CALL); } while (0)

#ifndef ICHIGO_DEBUG
#undef BEGIN_TIMED_BLOCK
#undef END_TIMED_BLOCK
#define BEGIN_TIMED_BLOCK(...)
#define END_TIMED_BLOCK(...)

#undef GL_CALL
#define GL_CALL(_CALL) _CALL

#undef ICHIGO_INFO
#undef ICHIGO_ERROR
#undef VK_ASSERT_OK
#define ICHIGO_INFO(...)
#define ICHIGO_ERROR(...)
#define VK_ASSERT_OK(...)
#endif

// TODO: Make these configurable?
#define PIXELS_PER_METRE 32
#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_CHANNEL_COUNT 2

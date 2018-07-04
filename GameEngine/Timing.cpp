#include "Timing.h"
#include <SDL\SDL.h>

namespace GameEngine{

	FpsLimiter::FpsLimiter()
	{

	}
	void FpsLimiter::init(float maxFps)
	{
		setMaxFps(maxFps);

	}

	void FpsLimiter::setMaxFps(float maxFps)
	{
		_maxFps = maxFps;
	}

	void FpsLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}

	float FpsLimiter::end()
	{
		calcFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;

		//limit fps to the max fps
		if (1000.0f / _maxFps > frameTicks){
			SDL_Delay((1000.0f / _maxFps) - frameTicks);
		}

		return _fps;

	}

	void FpsLimiter::calcFPS()
	{
		//number of frames to average
		static const int NUM_SAMPLES = 10;
		//stores all the frametimes that will be averaged
		static float frameTimes[NUM_SAMPLES];
		//Current frame that we are on
		static int currentFrame = 0;
		//The ticks of the previous frame, only initialized once
		static float previousTicks = SDL_GetTicks();
		//Ticks for the current frame
		float currentTicks = SDL_GetTicks();
		//calculate number of ticks (ms) for this frame
		_frameTime = currentTicks - previousTicks;
		//stores current frametime
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		previousTicks = currentTicks;

		//The number of frames to average
		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES){
			count = currentFrame;
		}
		else{
			count = NUM_SAMPLES;
		}

		//Average all the frame times
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++){
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		//Calculate FPS
		if (frameTimeAverage > 0){
			_fps = 1000.0f / frameTimeAverage;
		}
		else{
			_fps = 60;
		}

	}






}
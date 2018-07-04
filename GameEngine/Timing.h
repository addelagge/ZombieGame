#pragma once

namespace GameEngine{

class FpsLimiter{
public:
	FpsLimiter();
	void init(float maxFps);
	void setMaxFps(float maxFps);

	void begin();

	//end will return the current FPS
	float end();
private:
	float _maxFps;
	unsigned int _startTicks;
	void calcFPS();
	float _fps;
	float _frameTime;


};








}
#pragma once
#include <chrono>

class Timer 
{
private:
	std::chrono::time_point<std::chrono::system_clock> _startTime;

public:
	/// <summary>
	/// Starts or restarts the timer, takes the current time.
	/// </summary>
	void Start() 
	{
		_startTime = std::chrono::system_clock::now();
	}

	/// <summary>
	/// Returns the time in milliseconds since the start time.
	/// </summary>
	/// <returns></returns>
	float ElapsedMilliSecons() 
	{
		std::chrono::time_point<std::chrono::system_clock> EndTime;
		EndTime = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - _startTime).count();
	}

	/// <summary>
	/// Returns the time in seconds since the start time.
	/// </summary>
	/// <returns></returns>
	float ElapsedSeconds() 
	{
		return ElapsedMilliSecons() / 1000.0f;
	}
};
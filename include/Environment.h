#pragma once

#define _USE_MATH_DEFINES
#include "math.h"
#include "CLogger.h"
#include "Command.h"

/**
X: direction right/left.
Y: fork up/down.
Z: movement forward/backward.
Each axis to have: current value (angle, height, distance traveled), target value, delta/unit-time (speed), is-moving
Each iteration, updated values will be computed using 
In addition, the current location will be updated using the angle and the distance traveled.
**/

#define DEG_TO_RAD (M_PI / 180)

typedef struct {
	int currentValue;
	int targetValue;
	int delta;
	bool inMotion;
} stState;

class Environment
{
public:
	enum class StateVariables {
		StateX,
		StateY,
		StateZ,
		StateMax
	};

private:
	std::shared_ptr<spdlog::logger> _logger;
	static Environment* _instance;
	stState _states[static_cast<int>(StateVariables::StateMax)];
	double _locationX = 0, _locationY = 0;
	bool _binOnFork = false;

	Environment() {
		_logger = CLogger::GetInstance()->CreateLogger("Environment");
	}

public:
	Environment(const Environment&) = delete;
	Environment& operator=(const Environment&) = delete;
	static Environment* GetInstance()
	{
		if (_instance == 0)
			_instance = new Environment();
		return _instance;
	}
	void setVarDeltaFactor(StateVariables var, int factor) { _states[static_cast<int>(var)].delta *= factor; };
	void setVarTarget(StateVariables var, int target);
	void setBinOnFork(bool binOnFork) { _binOnFork = binOnFork; }
	bool binOnFork(void) { return _binOnFork; }
	int getCurrentValue(StateVariables var) { return _states[static_cast<int>(var)].currentValue; }
	double getLocation(char axis) { return 'X' == axis ? _locationX : _locationY; }
	Command::ExecEvents WaitForEvent(StateVariables);
};
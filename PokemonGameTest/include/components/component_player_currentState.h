#pragma once

// enum values are used for uv sprite animations
// uv row = Action
// uv col = Direction + Frame

// determines the row of the sprite sheet
enum class Input_Action
{
	STANDING = -1,
	WALKING = 0,
	RUNNING = 1
};

// determines the column of the sprite sheet
enum class Input_Direction
{
	UP = 0,
	DOWN = 3,
	LEFT = 6,
	RIGHT = 9
};

enum class Key_State
{
	PRESSED,
	HELD,
	RELEASED
};

// time (in seconds) it takes to move one tile
constexpr float WALK_SPEED = 0.3f; 
constexpr float RUN_SPEED = 0.15f; 
constexpr float BIKE_SPEED = 0.1f; 

struct component_player_currentState
{
	// purely player input
	Input_Action currentAction = Input_Action::STANDING;
	Input_Direction currentDirection = Input_Direction::DOWN;
	Key_State keyState = Key_State::RELEASED;

	// actual sprite direction
	Input_Direction spriteDirection = Input_Direction::DOWN;

	bool moveRequest = false;
	bool movementVisualsSynced = true;

	bool inputLocked = false;

	float moveSpeed = WALK_SPEED; 

	bool warping = false;
};
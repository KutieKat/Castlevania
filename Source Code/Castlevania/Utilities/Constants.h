#pragma once

// Structs
class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL)
	{ 
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};

// Enumerations
enum Direction {
	None,
	Left,
	Right
};

enum Visibility {
	Visible,
	Hidden
};

// Window
#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define SCREEN_WIDTH 512 // 320
#define SCREEN_HEIGHT 480 // 240

#define MAX_FRAME_RATE 120

#define BACKGROUND_WIDTH 1536

// Input
#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

// Simon
#define SIMON_WALK_SPEED 0.1f 
#define SIMON_JUMP_SPEED 0.5f
#define SIMON_GRAVITY 0.002f

#define SIMON_STATE_IDLE 1
#define SIMON_STATE_WALK 2
#define SIMON_STATE_SIT 3
#define SIMON_STATE_JUMP 4
#define SIMON_STATE_SIT_AND_ATTACK 5
#define SIMON_STATE_STAND_AND_ATTACK 6
#define SIMON_STATE_AUTO_WALK 7
#define SIMON_STATE_DELAY 8

#define SIMON_BBOX_WIDTH 60
#define SIMON_BBOX_HEIGHT 62 // 66

#define SIMON_ANI_IDLE_LEFT	0
#define SIMON_ANI_WALK_LEFT 1
#define SIMON_ANI_SIT_LEFT 2
#define SIMON_ANI_JUMP_LEFT 3
#define SIMON_ANI_SIT_LEFT_AND_ATTACK 4
#define SIMON_ANI_STAND_LEFT_AND_ATTACK 5
#define SIMON_ANI_DELAY_LEFT 6

#define SIMON_ANI_IDLE_RIGHT 7
#define SIMON_ANI_WALK_RIGHT 8
#define SIMON_ANI_SIT_RIGHT 9
#define SIMON_ANI_JUMP_RIGHT 10
#define SIMON_ANI_SIT_RIGHT_AND_ATTACK 11
#define SIMON_ANI_STAND_RIGHT_AND_ATTACK 12
#define SIMON_ANI_DELAY_RIGHT 13

#define SIMON_DELAY_TIMEOUT 900

// Brick
#define BRICK_BBOX_WIDTH 32
#define BRICK_BBOX_HEIGHT 32

// BigCandle
#define BIG_CANDLE_BBOX_WIDTH 31
#define BIG_CANDLE_BBOX_HEIGHT 63

// Whip
#define WHIP_LEVEL_1 1

#define WHIP_ANI_LEVEL_1_LEFT 0
#define WHIP_ANI_LEVEL_1_RIGHT 1
#define WHIP_ANI_LEVEL_2_LEFT 2
#define WHIP_ANI_LEVEL_2_RIGHT 3
#define WHIP_ANI_LEVEL_3_LEFT 4
#define WHIP_ANI_LEVEL_3_RIGHT 5

#define WHIP_LEVEL_1_BBOX_WIDTH 180
#define WHIP_LEVEL_2_BBOX_WIDTH 180
#define WHIP_LEVEL_3_BBOX_WIDTH 200
#define WHIP_BBOX_HEIGHT 66

// Item
#define ITEM_STATE_APPEAR 1
#define ITEM_GRAVITY 0.002f

// MoneyBag
#define MONEY_BAG_MOVE_UP_SPEED 0.05f

#define MONEY_BAG_BBOX_WIDTH 29
#define MONEY_BAG_BBOX_HEIGHT 29

// EasterEgg
#define EASTER_EGG_BBOX_WIDTH 32
#define EASTER_EGG_BBOX_HEIGHT 32

// BigHeart
#define BIG_HEART_BBOX_WIDTH 24
#define BIG_HEART_BBOX_HEIGHT 20

// Dagger
#define DAGGER_BBOX_WIDTH 32
#define DAGGER_BBOX_HEIGHT 18

// MorningStar
#define MORNING_STAR_BBOX_WIDTH 32
#define MORNING_STAR_BBOX_HEIGHT 32
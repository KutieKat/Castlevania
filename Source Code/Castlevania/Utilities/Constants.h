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
	float dx, dy;

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{ 
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
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
	Right,
	Up,
	Down
};

enum Visibility {
	Visible,
	Hidden
};

enum HealthType
{
	Empty,
	Player,
	Enemy
};

// Math
#define PI 3.14f

// Window
#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 528

#define MAX_FRAME_RATE 120

// Game
#define DEFAULT_GAME_TIME 300
#define DEFAULT_WAITING_TIME 3000
#define DEFAULT_SOFT_PAUSE_TIME 3000
#define ADDED_LIVES 10
#define ADDED_HEARTS 20
#define HEART_SCORE 100
#define TIME_SCORE 10

// GameOverBoard
#define CONTINUE_OPTION 0
#define END_OPTION 1

// Scenes
#define INTRO_SCENE 0
#define CUT_SCENE_1 1
#define PLAY_SCENE_1 2
#define CUT_SCENE_2 3
#define PLAY_SCENE_2_1 4
#define PLAY_SCENE_2_2 5
#define CUT_SCENE_3 6
#define PLAY_SCENE_3_1 7
#define PLAY_SCENE_3_2 8
#define PLAY_SCENE_4 9
#define CREDITS_SCENE 10

// Input
#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

// Blackboard
#define BLACKBOARD_HEIGHT 80

// Elevation
#define DEFAULT_ELEVATION 1
#define ENEMY_DEFAULT_ELEVATION 2
#define EFFECT_DEFAULT_ELEVATION 3
#define WEAPON_DEFAULT_ELEVATION 4
#define SIMON_DEFAULT_ELEVATION 10

// Simon
#define SIMON_WALK_SPEED 0.1f 
#define SIMON_SLOW_WALK_SPEED 0.08f
#define SIMON_JUMP_SPEED_X 0.15f
#define SIMON_JUMP_SPEED_Y 0.6f
#define SIMON_DEFLECT_SPEED_X 0.18f
#define SIMON_DEFLECT_SPEED_Y 0.5f
#define SIMON_GRAVITY 0.0025f

#define SIMON_STATE_IDLE 1
#define SIMON_STATE_WALK 2
#define SIMON_STATE_SIT 3
#define SIMON_STATE_JUMP 4
#define SIMON_STATE_SIT_AND_ATTACK 5
#define SIMON_STATE_STAND_AND_ATTACK 6
#define SIMON_STATE_AUTO_WALK 7
#define SIMON_STATE_DELAY 8
#define SIMON_STATE_DIE 9
#define SIMON_STATE_STAND_AND_THROW 10
#define SIMON_STATE_CUT_SCENE_AUTO_WALK 11
#define SIMON_STATE_WATCH 12
#define SIMON_STATE_IDLE_ON_STAIR 13
#define SIMON_STATE_WALK_UPSTAIR 14
#define SIMON_STATE_WALK_DOWNSTAIR 15
#define SIMON_STATE_WALK_UPSTAIR_AND_ATTACK 16
#define SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK 17
#define SIMON_STATE_DEFLECT 18
#define SIMON_STATE_STAND_ON_STAIR_AND_THROW 19

#define SIMON_BBOX_WIDTH 35
#define SIMON_BBOX_HEIGHT 62

#define SIMON_ANI_IDLE_LEFT	0
#define SIMON_ANI_WALK_LEFT 1
#define SIMON_ANI_SIT_LEFT 2
#define SIMON_ANI_JUMP_LEFT 3
#define SIMON_ANI_SIT_LEFT_AND_ATTACK 4
#define SIMON_ANI_STAND_LEFT_AND_ATTACK 5
#define SIMON_ANI_DELAY_LEFT 6
#define SIMON_ANI_IDLE_ON_STAIR_UP_LEFT 7
#define SIMON_ANI_IDLE_ON_STAIR_DOWN_LEFT 8
#define SIMON_ANI_WALK_UPSTAIR_LEFT 9
#define SIMON_ANI_WALK_DOWNSTAIR_LEFT 10
#define SIMON_ANI_WALK_UPSTAIR_LEFT_AND_ATTACK 11
#define SIMON_ANI_WALK_DOWNSTAIR_LEFT_AND_ATTACK 12
#define SIMON_ANI_DEFLECT_LEFT 13
#define SIMON_ANI_DIE_LEFT 14

#define SIMON_ANI_IDLE_RIGHT 15
#define SIMON_ANI_WALK_RIGHT 16
#define SIMON_ANI_SIT_RIGHT 17
#define SIMON_ANI_JUMP_RIGHT 18
#define SIMON_ANI_SIT_RIGHT_AND_ATTACK 19
#define SIMON_ANI_STAND_RIGHT_AND_ATTACK 20
#define SIMON_ANI_DELAY_RIGHT 21
#define SIMON_ANI_IDLE_ON_STAIR_UP_RIGHT 22
#define SIMON_ANI_IDLE_ON_STAIR_DOWN_RIGHT 23
#define SIMON_ANI_WALK_UPSTAIR_RIGHT 24
#define SIMON_ANI_WALK_DOWNSTAIR_RIGHT 25
#define SIMON_ANI_WALK_UPSTAIR_RIGHT_AND_ATTACK 26
#define SIMON_ANI_WALK_DOWNSTAIR_RIGHT_AND_ATTACK 27
#define SIMON_ANI_DEFLECT_RIGHT 28
#define SIMON_ANI_DIE_RIGHT 29
#define SIMON_ANI_WATCH 30

#define SIMON_INVISIBILITY_TIME 3000
#define SIMON_DELAY_TIME 900
#define SIMON_DEFAULT_LIVES 3
#define SIMON_DEFAULT_HEARTS 5
#define SIMON_DEFAULT_SCORE 0
#define SIMON_DEFAULT_DAMAGES 2

// BigCandle
#define BIG_CANDLE_BBOX_WIDTH 31
#define BIG_CANDLE_BBOX_HEIGHT 63
#define BIG_CANDLE_GRAVITY 0.0025f

// SmallCandle
#define SMALL_CANDLE_BBOX_WIDTH 15
#define SMALL_CANDLE_BBOX_HEIGHT 24
#define SMALL_CANDLE_GRAVITY 0.0025f

// Whip
#define WHIP_LEVEL_1 1
#define WHIP_LEVEL_2 2
#define WHIP_LEVEL_3 3

#define WHIP_ANI_LEVEL_1_LEFT 0
#define WHIP_ANI_LEVEL_1_RIGHT 1
#define WHIP_ANI_LEVEL_2_LEFT 2
#define WHIP_ANI_LEVEL_2_RIGHT 3
#define WHIP_ANI_LEVEL_3_LEFT 4
#define WHIP_ANI_LEVEL_3_RIGHT 5

#define WHIP_LEVEL_1_BBOX_WIDTH 115
#define WHIP_LEVEL_2_BBOX_WIDTH 115
#define WHIP_LEVEL_3_BBOX_WIDTH 144
#define WHIP_BBOX_HEIGHT 66

#define NORMAL_POWER 1
#define DOUBLE_POWER 2
#define TRIPLE_POWER 3

// Item
#define ITEM_STATE_APPEAR 1
#define ITEM_GRAVITY 0.002f
#define ITEM_DISPLAY_TIME 4000

// MoneyBag
#define BONUS_MOVE_UP_SPEED 0.05f

#define MONEY_BAG_BBOX_WIDTH 29
#define MONEY_BAG_BBOX_HEIGHT 29
#define MONEY_BAG_SCORE 1000

// EasterEgg
#define EASTER_EGG_BBOX_WIDTH 32
#define EASTER_EGG_BBOX_HEIGHT 32

// BigHeart
#define BIG_HEART_BBOX_WIDTH 24
#define BIG_HEART_BBOX_HEIGHT 20
#define BIG_HEART_HEARTS 5

// Dagger
#define DAGGER_BBOX_WIDTH 32
#define DAGGER_BBOX_HEIGHT 18
#define DAGGER_ANI_RIGHT 0
#define DAGGER_ANI_LEFT 1
#define DAGGER_MOVE_SPEED 0.5f

// MorningStar
#define MORNING_STAR_BBOX_WIDTH 32
#define MORNING_STAR_BBOX_HEIGHT 32

// HealthBar
#define HEALTH_BAR_MAX_VOLUMES 16

// HealthVolume
#define HEALTH_VOLUME_ANI_EMPTY 0
#define HEALTH_VOLUME_ANI_PLAYER 1
#define HEALTH_VOLUME_ANI_ENEMY 2

// Bat
#define BAT_STATE_FLY_RIGHT_CROSS 1
#define BAT_STATE_FLY_OVAL 2

#define BAT_FLY_RIGHT_CROSS_SPEED 0.017f
#define BAT_FLY_OVAL_SPEED 0.03f

// Helicopter
#define HELICOPTER_FLY_SPEED 0.01f

// Effect
#define EFFECT_DISPLAY_TIME 500
#define ENDING_EFFECT_DISPLAY_TIME 300

// Brick
#define BRICK_BBOX_WIDTH 31
#define BRICK_BBOX_HEIGHT 31

// TopStair
#define TOP_STAIR_BBOX_WIDTH 25
#define TOP_STAIR_BBOX_HEIGHT 25

// BottomStair
#define BOTTOM_STAIR_BBOX_WIDTH 25
#define BOTTOM_STAIR_BBOX_HEIGHT 12

// NextScene
#define NEXT_SCENE_BBOX_WIDTH 5
#define NEXT_SCENE_BBOX_HEIGHT 10

// PreviousScene
#define PREVIOUS_SCENE_BBOX_WIDTH 5
#define PREVIOUS_SCENE_BBOX_HEIGHT 10

// MovingBar
#define MOVING_BAR_MOVE_SPEED 0.025f

#define MOVING_BAR_BBOX_WIDTH 64
#define MOVING_BAR_BBOX_HEIGHT 16

// RedMoneyBag
#define RED_MONEY_BAG_BBOX_WIDTH 29
#define RED_MONEY_BAG_BBOX_HEIGHT 29
#define RED_MONEY_BAG_SCORE 100

// PurpleMoneyBag
#define PURPLE_MONEY_BAG_BBOX_WIDTH 29
#define PURPLE_MONEY_BAG_BBOX_HEIGHT 29
#define PURPLE_MONEY_BAG_SCORE 400

// WhiteMoneyBag
#define WHITE_MONEY_BAG_BBOX_WIDTH 29
#define WHITE_MONEY_BAG_BBOX_HEIGHT 29
#define WHITE_MONEY_BAG_SCORE 700

// Crown
#define CROWN_BBOX_WIDTH 29
#define CROWN_BBOX_HEIGHT 31
#define CROWN_SCORE 2000

// SmallHeart
#define SMALL_HEART_MOVE_SPEED 0.02f

#define SMALL_HEART_BBOX_WIDTH 15
#define SMALL_HEART_BBOX_HEIGHT 15

#define SMALL_HEART_HEARTS 2

// DoubleShot
#define DOUBLE_SHOT_BBOX_WIDTH 31
#define DOUBLE_SHOT_BBOX_HEIGHT 31

// BigDoubleShot
#define BIG_DOUBLE_SHOT_ANI_BLINKING 0
#define BIG_DOUBLE_SHOT_ANI_STATIC 1

// TripleShot
#define TRIPLE_SHOT_BBOX_WIDTH 31
#define TRIPLE_SHOT_BBOX_HEIGHT 31

// BigTripleShot
#define BIG_TRIPLE_SHOT_ANI_BLINKING 0
#define BIG_TRIPLE_SHOT_ANI_STATIC 1

// Boomerang
#define BOOMERANG_STATE_STATIC 1
#define BOOMERANG_STATE_MOVE 2

#define BOOMERANG_ANI_STATIC 0
#define BOOMERANG_ANI_MOVE 1

#define BOOMERANG_MOVE_SPEED 0.2f

#define BOOMERANG_BBOX_WIDTH 27
#define BOOMERANG_BBOX_HEIGHT 27

#define BOOMERANG_MOVABLE_AREA_WIDTH 260
#define BOOMERANG_DAMAGES 2

// BreakableBrick
#define BREAKABLE_BRICK_BBOX_WIDTH 31
#define BREAKABLE_BRICK_BBOX_HEIGHT 31

// Breaking
#define BREAKING_PIECES 4

// Enemy
#define ENEMY_DEFAULT_DAMAGES 1
#define ENEMY_DEFAULT_ATTACKS 1
#define ENEMY_DELAY_TIME 500
#define ENEMY_DELAY_PLUS_TIME 1000

// RedBat
#define RED_BAT_AREA_RADIUS_X 200
#define RED_BAT_AREA_RADIUS_Y 100

#define RED_BAT_STATE_STATIC 1
#define RED_BAT_STATE_MOVE 2

#define RED_BAT_ANI_STATIC_LEFT 0
#define RED_BAT_ANI_MOVE_LEFT 1
#define RED_BAT_ANI_STATIC_RIGHT 2
#define RED_BAT_ANI_MOVE_RIGHT 3

#define RED_BAT_MOVE_SPEED_X 0.15f
#define RED_BAT_MOVE_SPEED_Y 0.1f

#define RED_BAT_BBOX_WIDTH 31
#define RED_BAT_BBOX_HEIGHT 31
#define RED_BAT_SCORE 300

// SpearKnight
#define SPEAR_KNIGHT_WALK_SPEED 0.05f 
#define SPEAR_KNIGHT_GRAVITY 0.0025f

#define SPEAR_KNIGHT_STATE_WALK 1
#define SPEAR_KNIGHT_STATE_DELAY 2

#define SPEAR_KNIGHT_ANI_WALK_LEFT 0
#define SPEAR_KNIGHT_ANI_DELAY_LEFT 1

#define SPEAR_KNIGHT_ANI_WALK_RIGHT 2
#define SPEAR_KNIGHT_ANI_DELAY_RIGHT 3

#define SPEAR_KNIGHT_BBOX_WIDTH 31
#define SPEAR_KNIGHT_BBOX_HEIGHT 63

#define SPEAR_KNIGHT_SCORE 400
#define SPEAR_KNIGHT_ATTACKS 3

// PorkChop
#define PORK_CHOP_BBOX_WIDTH 31
#define PORK_CHOP_BBOX_HEIGHT 25
#define PORK_CHOP_HEALTHS 6

// Rosary
#define ROSARY_BBOX_WIDTH 31
#define ROSARY_BBOX_HEIGHT 31

// Axe
#define AXE_GRAVITY 0.00045f
#define AXE_BBOX_WIDTH 29
#define AXE_BBOX_HEIGHT 27
#define AXE_ANI_STATIC 0
#define AXE_ANI_MOVE 1
#define AXE_MOVE_SPEED_X 0.15f
#define AXE_MOVE_SPEED_Y 0.32f
#define AXE_DAMAGES 2

// HolyWater
#define HOLY_WATER_BBOX_WIDTH 31
#define HOLY_WATER_BBOX_HEIGHT 31

// HolyWaterBottle
#define HOLY_WATER_BOTTLE_GRAVITY 0.0004f
#define HOLY_WATER_BOTTLE_BURNING_TIME 5000
#define HOLY_WATER_BOTTLE_STATE_MOVE 1
#define HOLY_WATER_BOTTLE_STATE_BURN 2
#define HOLY_WATER_BOTTLE_BBOX_WIDTH 15
#define HOLY_WATER_BOTTLE_BBOX_HEIGHT 15
#define HOLY_WATER_BOTTLE_ANI_RIGHT 0
#define HOLY_WATER_BOTTLE_ANI_LEFT 1
#define HOLY_WATER_BOTTLE_ANI_FLAME 2
#define HOLY_WATER_BOTTLE_MOVE_SPEED 0.2f

// Stopwatch
#define STOPWATCH_GRAVITY 0.0008f
#define STOPWATCH_BBOX_WIDTH 29
#define STOPWATCH_BBOX_HEIGHT 31
#define STOPWATCH_MOVE_SPEED 0.2f

// Bone
#define BONE_GRAVITY 0.0004f
#define BONE_BBOX_WIDTH 29
#define BONE_BBOX_HEIGHT 31
#define BONE_MOVE_SPEED_X 0.1f
#define BONE_MOVE_SPEED_Y 0.25f
#define BONE_DISPLAY_TIME 2000

// MagicCrystal
#define MAGIC_CRYSTAL_BBOX_WIDTH 27
#define MAGIC_CRYSTAL_BBOX_HEIGHT 31

// Fireball
#define FIREBALL_BBOX_WIDTH 9
#define FIREBALL_BBOX_HEIGHT 11
#define FIREBALL_MOVE_SPEED_X 0.1f
#define FIREBALL_MOVE_SPEED_Y 0.1f
#define FIREBALL_DISPLAY_TIME 2600

// InvisibilityPotion
#define INVISIBILITY_POTION_BBOX_WIDTH 29
#define INVISIBILITY_POTION_BBOX_HEIGHT 36

// EnemySpawnerArea
#define ENEMY_SPAWNER_AREA_BBOX_WIDTH 100
#define ENEMY_SPAWNER_AREA_BBOX_HEIGHT 32
#define ENEMY_SPAWNER_SPAWNING_INTERVAL 4000

// Ghost
#define GHOST_BBOX_WIDTH 27
#define GHOST_BBOX_HEIGHT 27
#define GHOST_STATE_MOVE 1
#define GHOST_STATE_DELAY 2
#define GHOST_ANI_MOVE_RIGHT 0
#define GHOST_ANI_MOVE_LEFT 1
#define GHOST_ANI_DELAY_RIGHT 2
#define GHOST_ANI_DELAY_LEFT 3
#define GHOST_MOVE_SPEED_X 0.1f
#define GHOST_MOVE_SPEED_Y 0.05f
#define GHOST_SCORE 300
#define GHOST_ATTACKS 2

// Fleamen
#define FLEAMEN_GRAVITY 0.0025f
#define FLEAMEN_BBOX_WIDTH 31
#define FLEAMEN_BBOX_HEIGHT 31
#define FLEAMEN_JUMP_SPEED_X 0.2f
#define FLEAMEN_JUMP_SPEED_Y 0.7f
#define FLEAMEN_AREA_RADIUS_X 150
#define FLEAMEN_AREA_RADIUS_Y 150
#define FLEAMEN_STATE_IDLE 1
#define FLEAMEN_STATE_JUMP_AROUND 2
#define FLEAMEN_ANI_IDLE_RIGHT 0
#define FLEAMEN_ANI_IDLE_LEFT 1
#define FLEAMEN_ANI_JUMP_RIGHT 2
#define FLEAMEN_ANI_JUMP_LEFT 3
#define FLEAMEN_SCORE 500
#define FLEAMEN_ATTACKING_INTERVAL 3000

// WhiteSkeleton
#define WHITE_SKELETON_GRAVITY 0.0025f
#define WHITE_SKELETON_BBOX_WIDTH 33
#define WHITE_SKELETON_BBOX_HEIGHT 63
#define WHITE_SKELETON_AREA_RADIUS_X 150
#define WHITE_SKELETON_AREA_RADIUS_Y 150
#define WHITE_SKELETON_WALK_SPEED_X 0.05f
#define WHITE_SKELETON_JUMP_SPEED_X 0.3f
#define WHITE_SKELETON_JUMP_SPEED_Y 0.6f
#define WHITE_SKELETON_STATE_IDLE 1
#define WHITE_SKELETON_STATE_WALK_AROUND 2
#define WHITE_SKELETON_STATE_JUMP_AROUND 3
#define WHITE_SKELETON_STATE_ATTACK 4
#define WHITE_SKELETON_STATE_DIE 5
#define WHITE_SKELETON_ANI_IDLE_RIGHT 0
#define WHITE_SKELETON_ANI_IDLE_LEFT 1
#define WHITE_SKELETON_ANI_WALK_RIGHT 2
#define WHITE_SKELETON_ANI_WALK_LEFT 3
#define WHITE_SKELETON_ANI_DIE_RIGHT 4
#define WHITE_SKELETON_ANI_DIE_LEFT 5
#define WHITE_SKELETON_SCORE 300
#define WHITE_SKELETON_ATTACKING_INTERVAL 3000
#define WHITE_SKELETON_TO_SIMON_DISTANCE_X 120

// Raven
#define RAVEN_BBOX_WIDTH 27
#define RAVEN_BBOX_HEIGHT 27
#define RAVEN_STATE_IDLE 1
#define RAVEN_STATE_FLY 2
#define RAVEN_STATE_ATTACK 3
#define RAVEN_ANI_IDLE_RIGHT 0
#define RAVEN_ANI_IDLE_LEFT 1
#define RAVEN_ANI_FLY_RIGHT 2
#define RAVEN_ANI_FLY_LEFT 3
#define RAVEN_FLY_SPEED_X 0.05f
#define RAVEN_FLY_SPEED_Y 0.05f
#define RAVEN_ATTACK_SPEED_X 0.25f
#define RAVEN_ATTACK_SPEED_Y 0.25f
#define RAVEN_TO_SIMON_DISTANCE_X 120
#define RAVEN_TO_SIMON_DISTANCE_Y 60
#define RAVEN_AREA_RADIUS_X 200
#define RAVEN_AREA_RADIUS_Y 200
#define RAVEN_SCORE 300

// BiStair
#define BI_STAIR_BBOX_WIDTH 12
#define BI_STAIR_BBOX_HEIGHT 12

// CameraLocker
#define CAMERA_LOCKER_BBOX_WIDTH 32
#define CAMERA_LOCKER_BBOX_HEIGHT 300

// Ghoul
#define GHOUL_BBOX_WIDTH 33
#define GHOUL_BBOX_HEIGHT 63
#define GHOUL_ANI_MOVE_RIGHT 0
#define GHOUL_ANI_MOVE_LEFT 1
#define GHOUL_MOVE_SPEED 0.1f
#define GHOUL_SCORE 100

// Boss
#define BOSS_DEFAULT_DAMAGES 1

// PhantomBat
#define PHANTOM_BAT_BBOX_WIDTH 95
#define PHANTOM_BAT_BBOX_HEIGHT 45
#define PHANTOM_BAT_STATE_IDLE 1
#define PHANTOM_BAT_STATE_FLY 2
#define PHANTOM_BAT_STATE_ATTACK 3
#define PHANTOM_BAT_STATE_DELAY 4
#define PHANTOM_BAT_STATE_DIE 5
#define PHANTOM_BAT_ANI_IDLE 0
#define PHANTOM_BAT_ANI_FLY 1
#define PHANTOM_BAT_ANI_DELAY 2
#define PHANTOM_BAT_FLY_SPEED_X 0.05f
#define PHANTOM_BAT_FLY_SPEED_Y 0.05f
#define PHANTOM_BAT_ATTACK_SPEED_X 0.15f
#define PHANTOM_BAT_ATTACK_SPEED_Y 0.25f
#define PHANTOM_BAT_TO_SIMON_DISTANCE_X 120
#define PHANTOM_BAT_TO_SIMON_DISTANCE_Y 120
#define PHANTOM_BAT_AREA_RADIUS_X 100
#define PHANTOM_BAT_AREA_RADIUS_Y 260
#define PHANTOM_BAT_SCORE 3000
#define PHANTOM_BAT_ATTACKS 16

// Castle
#define CASTLE_MOVE_SPEED_X 0.02f
#define CASTLE_MOVE_SPEED_Y 0.008f
//Structures for mr do! game elements
//I may be able to replace these with one super MrDoSprite
//type structure as they will be (essentially) similar...

//These flags are used to show when a game element (player, beastie or apple)
//is:
//Inactive (i.e. not under game control)
//Dead (i.e. well, dead)
#define FlagInactive 0x1
#define FlagDead 0x2

typedef struct mrdoplayer
{
	u8 xPosition;
	u8 yPosition;
	u8 Animation;
	u8 Direction;
	u8 Flags;
	u8 Lives;
	u8 BaseAnimation;
	u16 AnimationInterval;
} MRDO;

typedef struct mrdomagicball
{
	u8 xPosition;
	u8 yPosition;
	u8 xDirection;
	u8 yDirection;
	u8 Flags;
	u8 BaseAnimation;
	u16 AnimationInterval;
} MAGICBALL;

typedef struct mrdoBeastie
{
	u8 xPosition;
	u8 yPosition;
	u8 Direction;
	u8 Animation;
	u8 Flags;
	u8 BaseAnimation;
	u16 Timer;
} BEASTIE;

typedef struct mrdoApple
{
	u8 xPosition;
	u8 yPosition;
	u8 DistanceFell;
	u8 Flags;
	u8 Animation;
	u8 BaseAnimation;
	u16 AnimationInterval;
} APPLE;
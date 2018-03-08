#include "ngpc.h"
#include "carthdr.h" // TODO: edit game name in carthdr.h
#include "library.h"

#include "mrdo.h"

//#include "your tileset here"
#include "sprites.c"

//Tiles & Mr Do! maps
#include "map.c"
#include "tiles.c"

//Mr Do! specific functions et al...
#include "mrdo.c"

void main()
{

	//Define vars...
	//Grid position of various objects...
	u8 xPosition;
	u8 yPosition;
	u8 xTempPosition;
	u8 yTempPosition;
	u8 xProposedPosition;
	u8 yProposedPosition;
	u8 xBeastiePosition;
	u8 yBeastiePosition;
	u8 xApplePosition;
	u8 yApplePosition;
	u8 xMagicBallPosition;
	u8 yMagicBallPosition;

	u16 GameAnimationTimer;

	u16 BeastieInterval;

	//Generic loop variables
	u8 iBeastieLoop;
	u8 iAppleLoop;
	u8 iLoop;

	//Level descriptors
	u8 iDeadBeastieCount;
	u8 iBeastieCount;
	u8 iCherryCount;
	u8 iLevelNumber;

	u8 iTemp;

	u8 WallCount;

	//Beastie movement algorithms...
	u16 iScore;
	u8 ProposedBeastieDirection;
	u8 BeastieTimer;

	//Setup the NGPC environment
	InitNGPC();
	SysSetSystemFont();

	//Should really do something to make this different every go, but
	//does it really matter for this game?
	SeedRandom;

	//Yer basic background colour...
	SetBackgroundColour(RGB(0,0,0));

	//Setup tile palettes...
	//Note. Not all of these are actually used...
	SetPalette(SCR_1_PLANE, 0, 0, RGB(15,15,15), RGB(15,15,15), RGB(15,15,15));
	//Mr Do Palette (White, Red & Blue)
	SetPalette(SCR_1_PLANE, 1, RGB(15,15,15), RGB(15,15,15), RGB(15,0,0), RGB(0,0,15));
	//Cherry Palette (Red, Green & Black)
	SetPalette(SCR_1_PLANE, 2, 0, RGB(15, 0, 0), RGB(0, 15, 0), RGB(0,0,0));
	//Appple Palette (White, Yellow, Red)
	SetPalette(SCR_1_PLANE, 3, 0, RGB(15,15,15), RGB(15, 15,0), RGB(15,0,0));
	//Beastie Palette (White, Red, Blue) - Looks like Mr Do! to me...
	//SetPalette(SCR_1_PLANE, 4, 0, RGB(15,15,15), RGB(15, 0,0), RGB(0,0,15));
	//Spawn point palette (Blue, White, Red)
	SetPalette(SCR_1_PLANE, 4, 4, RGB(15,0,0), RGB(0,0,0), RGB(0,0,15));

   	ClearScreen(SCR_1_PLANE);

	// Sprite Plane!
	//Main character
	SetPalette(SPRITE_PLANE, 0, 0, RGB(15,15,15), RGB(15,0,0), RGB(0,0,15));
	//Beasties
	SetPalette(SPRITE_PLANE, 1, 0, RGB(15,15,15), RGB(15, 0,0), RGB(0,0,15));
	//Apples
	SetPalette(SPRITE_PLANE, 2, 0, RGB(15,15,15), RGB(15, 15,0), RGB(15,0,0));
	//Magic Ball!
	//This *should* give us a palette of two levels of gray with a white centre.

	SetPalette(SPRITE_PLANE, 3, 0, RGB(15,15,15), RGB(8,8,8), RGB(12,12,12));

	//Install the graphics.
	InstallTileSet(MrDoBackGroundTiles, sizeof(MrDoBackGroundTiles)/2);

	//Set default game parameters
	//overall game loop - loop forever!
	while (1)
	{
		//Reset all sprites.
		for (iLoop=0;iLoop<64;iLoop++)
			SetSprite(iLoop, tlNull, 0, 0, 0, 0, 0);

		//Setup the initial "game over" screen...
		iLevelNumber=0;
		CopyMrDoMap(iLevelNumber);
		iCherryCount=0;
		iDeadBeastieCount=0;
		iBeastieCount=MAX_BEASTIE;

		PrintString(SCR_1_PLANE, 0, 4, 3, "GAME  OVER");

		PrintString(SCR_1_PLANE, 0, 1, 7, "Press A Button to");
		PrintString(SCR_1_PLANE, 0, 4, 8, "play Mr Do!");
		//Wait for the "A" button
		while (!(JOYPAD & J_A));

	   	ClearScreen(SCR_1_PLANE);
	   	ClearScreen(SCR_2_PLANE);

		//Get the current level
		CopyMrDoMap(1);

		//Setup game variables
		iScore = 0;
		mrdo.Lives = 8;

		while (mrdo.Lives > 0)
		{

			//Starting position for Do! & the beasties...

			//I'm not entirely sure I need to store these variables seperately...
			xPosition = 4;
			yPosition = 8;

			//These are used to calculate the spawning position for the beasties
			xBeastiePosition = 4;
			yBeastiePosition = 4;

			//Set's up beastie spawning
			MaxBeastie=0;
			BeastieTimer=0;

			GameAnimationTimer=0;

			//Check for the end of the current level.
			//End of level detected. Reset the map variables and go girl!
			if (iCherryCount==0 || iDeadBeastieCount>=MAX_BEASTIE)
			{
				MaxApple=0;
				MaxCherry=0;
				iBeastieCount=MAX_BEASTIE;

				CopyMrDoMap(iLevelNumber);
				iDeadBeastieCount=0;
				iCherryCount=DrawMrDoMap();
				iLevelNumber+=1;
				if (iLevelNumber>MAXLEVELS)
					iLevelNumber=0;
			}
			else
			{
				//It sometimes falls into here when a level ends? Not sure why...
				//No it doesn't. What happens is this...
				//When you lose your penultimate life, this code executes and
				//decreases your lives count to zero. The code then, quite correctly,
				//goes into the main game loop. If you manage to then clear the level
				//(eat all cherries or kill all beasties) then that loop terminates
				//and it comes back up here to be met with while (iLives>0) which
				//is no longer true so it packs it all in and finishes the game.
				mrdo.Lives--;
				iBeastieCount=MAX_BEASTIE-iDeadBeastieCount;
			}

			//For some reason, once a life has been lost, this will always create
			//a phantom apple directly above the player - for the first life lost
			//on each level it will also create the sprite?
			//Nope. Actually, it looks as though the phantom apple appears at the
			//the beginning of each level...

			//Set up the beastie structure
			//I need to think this through a bit more. Once a beastie has been killed...
			//we need to reduce the max number of beasties?
			bEnemies[0].Flags = 0;
			bEnemies[0].xPosition = xBeastiePosition * 16;
			bEnemies[0].yPosition = yBeastiePosition * 16;
			bEnemies[0].Animation = 0;
			bEnemies[0].BaseAnimation = ANIM_OFFSET_BEASTIE;
			bEnemies[0].Direction = 0;
			BeastieInterval = 24;
			SetSprite(SPR_BEASTIE, TILE_SPRITE_BEASTIE, 0, bEnemies[0].xPosition, bEnemies[0].yPosition, 1,16);
			SetSprite(SPR_BEASTIE+1, TILE_SPRITE_BEASTIE+1, 1, 8, 0, 1,16);
			SetSprite(SPR_BEASTIE+2, TILE_SPRITE_BEASTIE+2, 1, -8, 8, 1,16);
			SetSprite(SPR_BEASTIE+3, TILE_SPRITE_BEASTIE+3, 1, 8, 0, 1,16);

			for (iLoop=1; iLoop<=MAX_BEASTIE-1; iLoop++)
			{
				bEnemies[iLoop].xPosition = xBeastiePosition * 16;
				bEnemies[iLoop].yPosition = yBeastiePosition * 16;
				bEnemies[iLoop].Animation = 0;
				bEnemies[iLoop].BaseAnimation = ANIM_OFFSET_BEASTIE;
				bEnemies[iLoop].Direction = 0;
				SetSprite(SPR_BEASTIE+(iLoop*4), tlNull, 0, bEnemies[iLoop].xPosition, bEnemies[iLoop].yPosition, 1,16);
				SetSprite(SPR_BEASTIE+(iLoop*4)+1, tlNull, 1, 8, 0, 1,16);
				SetSprite(SPR_BEASTIE+(iLoop*4)+2, tlNull, 1, -8, 8, 1,16);
				SetSprite(SPR_BEASTIE+(iLoop*4)+3, tlNull, 1, 8, 0, 1,16);
			}

			//Don't do this. It also fucks up the apple positions, which we want to keep the same...
			//for (iLoop=0;iLoop<64;iLoop++)
			//	SetSprite(iLoop, tlNull, 0, 0, 0, 0, 0);

			//Set up the player sprite
			mrdo.xPosition = xPosition * 16;
			mrdo.yPosition = yPosition * 16;
			mrdo.Animation = 0;
			mrdo.Direction = DirRight;
			mrdo.BaseAnimation = ANIM_OFFSET_MRDO;
			mrdo.Flags = 0;
			mrdo.AnimationInterval = 48;
			// set the initial player sprite up
			// Note that all 4 sprites are chained together so we only have to
			// move the first one and the rest follow :-)
			SetSprite(SPR_SPRITE, TILE_SPRITE, 0, mrdo.xPosition, mrdo.yPosition, 0, 16);
			SetSprite(SPR_SPRITE+1, TILE_SPRITE+1, 1, 8, 0, 0, 16);
			SetSprite(SPR_SPRITE+2, TILE_SPRITE+2, 1, -8, 8, 0, 16);
			SetSprite(SPR_SPRITE+3, TILE_SPRITE+3, 1, 8, 0, 0, 16);

			//Setup the Magic Ball sprite
			magicball.xPosition = mrdo.xPosition;
			magicball.yPosition = mrdo.yPosition;
			magicball.xDirection = 0;
			magicball.yDirection = 0;
			magicball.Flags = FlagDead;
			magicball.AnimationInterval = 18;
			SetSprite(SPR_MAGICBALL, tlMagicBall, 0, magicball.xPosition, magicball.yPosition, 3, 16);

			/*
			Right then...
			Basic game behaviour:

			Beasties will try and move towards Mr Do!

			Not in a straight line way though, they've got to work out what the quickest way
			to get to him *through* the maze. Obviously, they can't all do the same calculation
			as that will just end up with Benny Hill style chases, so we've got to introduce
			some element of randomness (based on beastie number) to vary the pattern slightly.

			We'll start with a single beastie, doing his own thing somewhere in the middle of the screen.

			This is probably about the time that i want to start introducing more realistic
			movement to the main character.

			Currently, our main sprite moves in 2-tile increments which match the underlying
			structure of the game arena.

			In real terms, a single "game move" will move Mr Do! approximately 1/8 of a tile
			width (for the sake of argument) It will therefore take 16 game moves (loops to
			you and me) to move the character a single place. Changing direction "mid move"
			will cause the character to "snap" to the _last_ grid position. I.e. moving slightly
			left and then up will move the clown to the position above the start point - even
			if you have moved 7/8's of the way there. Not sure what happens when you move
			within a plane though. I think you'll just turn around where you stand.

			This will allow me to move the monsters slightly faster than Mr Do! (which I
			think is the normal behaviour) and also to vary his speed when cutting through
			the rock (he goes approximately 10% slower methinks?)

			I'll also need this time to implement the magic ball and apple falling behaviour
			properly.

			The Magic Ball moves about 5-10 times faster than Mr Do! and the beasties.

			Apples, when undermined or pushed, hang in mid air for a second or so and
			then fall - I think that the fall rate is fixed once they get going (no
			acceleration in Do!World thank goat. Falling is temporarily halted when
			they crash through a maze divide or do the glancing blow thing. But they
			don't hang around for as long the second time around.
			Let's say that the Apple fall rate is between 1.5 times and 2 times
			the Mr Do! movement rate.

			Most of the movement stuff is pretty simple really. The Beastie path finding
			algorithm is going to be the tricky one - this has to be quite simple! We're
			not talking advanced AI here, just a simple "which direction now" type thing
			has got to be the way to go...

			I suppose I'd better start with implementing beastie spawning...

			*/

			//Do the number of lives thang...
			for (iLoop=0;iLoop<8;iLoop++)
			{

				if (iLoop<mrdo.Lives)
				{
					//Print player
					PutTile(SCR_1_PLANE, 1, 18, iLoop * 2, tlMrDo+tlCharTopLeft);
					PutTile(SCR_1_PLANE, 1, 19, iLoop * 2, tlMrDo+tlCharTopRight);
					PutTile(SCR_1_PLANE, 1, 18, (iLoop * 2) + 1, tlMrDo+tlCharBottomLeft);
					PutTile(SCR_1_PLANE, 1, 19, (iLoop * 2) + 1, tlMrDo+tlCharBottomRight);

				}
				else
				{
					//Print Tombstone
					PutTile(SCR_1_PLANE, 1, 18, iLoop * 2, tlNull);
					PutTile(SCR_1_PLANE, 1, 19, iLoop * 2, tlNull);
					PutTile(SCR_1_PLANE, 1, 18, (iLoop * 2) + 1, tlNull);
					PutTile(SCR_1_PLANE, 1, 19, (iLoop * 2) + 1, tlNull );
				}
			}

			//Main game loop
			//Loop until the end of level is reached or Mr Do! dies...
			while((!(mrdo.Flags & FlagDead))&&(iCherryCount>0)&&(iDeadBeastieCount<MAX_BEASTIE)&&(mrdo.Lives>0))
			{

				/*
				This code is as kludgy as fuck...

				Basically, it would work *better* if we store a current position
				and a *proposed* position - which is, in fact, what it does now...

				The joystick movement stuff merely updates the proposed position
				and we can then farm both of them off to a subroutine somewhere
				in order to do the tedious "do I need to update the level map"
				and "do I need to redraw Mr Do!" stuff.

				Isn't that better? (You'll have to take my word for it, the old
				code was _horrible_!)

				*/

				//The code in this structure is *so* similar in each case...
				//is there any way i can combine?
				//Anyway, enough of that... It works well enough for the time being.

				//This doesn't work too well with the falling apples thing...
				//What we really need to do is only run this block (i.e. only allow
				//player to move when not being squashed...

				//Only do this if we're in a MrDo! Interval
				if ((GameAnimationTimer%mrdo.AnimationInterval)==0)
				{
					if (!(mrdo.Flags & FlagInactive))
					{
						if ((JOYPAD & J_LEFT) && (mrdo.xPosition > 0))
						{
							//Are we digging?
							//Is there a wall to the left
							//_and_
							//are we in the left hand side of the tile...
							if ((MrDoCurrentLevel[yPosition][xPosition]&WallLeft) || (((mrdo.xPosition/8)%2)==0))
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDO+16;
								mrdo.AnimationInterval=48;
							}
							else
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDODIGGING+16;
								mrdo.AnimationInterval=72;
							}
							mrdo.xPosition-=2;
							//Snap the Ycoords to the vertical grid
							mrdo.yPosition = yPosition * 16;
							mrdo.Direction = DirLeft;
							//Check to see whether we're trying to push any apples
							//How to do this?
							//i'm also not convinced that this is the "right" place to do
							//this test. Oh life's confusing isn't it...
							for (iLoop=0;iLoop<=MaxApple-1;iLoop++)
							{
								if (SpriteCollision(aApples[iLoop].xPosition,aApples[iLoop].yPosition, mrdo.xPosition-2, mrdo.yPosition)==0x1 && (!(aApples[iLoop].Flags&FlagInactive)))
								{
									//Move that apple
									//I'm not sure whether this is a good test or not - basically, this has
									//a tendancy to "drag" apples behind you, which looks rather strange.
									//Also, because up & down have been disabled when you share apple space
									//you can get stuck waiting
									if (aApples[iLoop].xPosition>0)
									{
										aApples[iLoop].xPosition-=2;
										SetSpritePosition(SPR_APPLE+(iLoop*4), aApples[iLoop].xPosition, aApples[iLoop].yPosition);
									}
									else
									{
										mrdo.xPosition+=2;
									}
								}
							}

							mrdo.Animation ^= 1; // toggles anim 0->1->0
						}
						else if ((JOYPAD & J_RIGHT) && (mrdo.xPosition < ((MAX_X - 1) * 16)))
						{
							//Are we digging
							if ((MrDoCurrentLevel[yPosition][xPosition]&WallRight) || (((mrdo.xPosition/8)%2)==1))
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDO+24;
								mrdo.AnimationInterval=48;
							}
							else
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDODIGGING+24;
								mrdo.AnimationInterval=72;
							}
							mrdo.xPosition += 2;
							//Snap the Ycoords to the vertical grid
							mrdo.yPosition = yPosition * 16;
							mrdo.Direction = DirRight;
							//Check to see whether we're trying to push any apples...
							//How to do this?
							for (iLoop=0;iLoop<=MaxApple-1;iLoop++)
							{
								if (SpriteCollision(aApples[iLoop].xPosition,aApples[iLoop].yPosition, mrdo.xPosition+2, mrdo.yPosition)==0x1 && (!(aApples[iLoop].Flags&FlagInactive)))
								{
									//Move that apple
									//Don't forget to do some kind of bound checking on the apple (i.e. don't push
									//it off the edge of the screen.
									if (aApples[iLoop].xPosition < ((MAX_X -1) * 16))
									{
										aApples[iLoop].xPosition+=2;
										SetSpritePosition(SPR_APPLE+(iLoop*4), aApples[iLoop].xPosition, aApples[iLoop].yPosition);
									}
									else
									{
										mrdo.xPosition-=2;
									}
								}
							}
							mrdo.Animation ^= 1;

						}
						else if ((JOYPAD & J_UP) && (mrdo.yPosition > 0))
						{
							//Are we digging
							if ((MrDoCurrentLevel[yPosition][xPosition]&WallTop) || (((mrdo.yPosition/8)%2)==0))
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDO+8;
								mrdo.AnimationInterval=48;
							}
							else
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDODIGGING+8;
								mrdo.AnimationInterval=72;
							}
							mrdo.yPosition -= 2;
							//Snap the Xcoords to the horizontal grid
							mrdo.xPosition = xPosition * 16;
							mrdo.Direction = DirUp;
							//Check to see whether we're trying to push any apples...
							//How to do this?
							for (iLoop=0;iLoop<=MaxApple-1;iLoop++)
							{
								if (SpriteCollision(aApples[iLoop].xPosition,aApples[iLoop].yPosition, mrdo.xPosition, mrdo.yPosition)==0x1 && (!(aApples[iLoop].Flags&FlagInactive)))
								{
									//Silly boy! You can't push apples up hill.
									//Actually. I'm not sure about that, can you "move" into the
									//same position as an apple?
									//Not if it's falling on your head you can't. Not sure about about
									//coming in from above though.
									mrdo.yPosition+=2;
								}
							}
							mrdo.Animation ^= 1;

						}
						else if ((JOYPAD & J_DOWN) && (mrdo.yPosition < ((MAX_Y - 1) * 16)))
						{
							//Are we digging?
							//This test is a bit rubbish. It should only kick in when Mr Do! is actually
							//approaching a wall...
							if ((MrDoCurrentLevel[yPosition][xPosition]&WallBottom) || (((mrdo.yPosition/8)%2)==1))
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDO;
								mrdo.AnimationInterval=48;
							}
							else
							{
								mrdo.BaseAnimation=ANIM_OFFSET_MRDODIGGING;
								mrdo.AnimationInterval=72;
							}
							mrdo.yPosition += 2;
							//Snap the Xcoords to the horizontal grid
							mrdo.xPosition = xPosition * 16;
							mrdo.Direction = DirDown;
							//Check to see whether we're trying to push any apples...
							//How to do this?
							for (iLoop=0;iLoop<=MaxApple-1;iLoop++)
							{
								if (SpriteCollision(aApples[iLoop].xPosition,aApples[iLoop].yPosition, mrdo.xPosition, mrdo.yPosition)==0x1 && (!(aApples[iLoop].Flags&FlagInactive)))
								{
									//You are _not_ allowed to move into the same space as an apple...
									//or maybe you are. I can't remember.
									mrdo.yPosition-=2;
								}
							}
							mrdo.Animation ^= 1;
						}
						CopySpriteAnimation(SPR_SPRITE, mrdo.BaseAnimation+(mrdo.Animation * 4), TILE_SPRITE);
						SetSpritePosition(SPR_SPRITE, mrdo.xPosition, mrdo.yPosition);

						//Magic ball!
						if ((JOYPAD & J_A) && (!(mrdo.BaseAnimation & ANIM_OFFSET_MRDODIGGING)))
						{
							//Only kick off a ball if it's not already active
							//Basically all we have to do here is start a new instance of the
							//ball. The movemnent of said ball is dealt with later.
							if ((magicball.Flags & FlagDead) && (!(magicball.Flags & FlagInactive)))
							{
								//The starting position (and direction?) will vary
								//depending on which way the player is facing...
								//I really *must* sort that out soon...
								//magicball.xPosition=mrdo.xPosition;
								//magicball.yPosition=mrdo.yPosition;

								//Will use "1" for positive increment
								//and "0" for negative. For some reason I'm not quite sure
								//about, I can't use negative numbers for these values - even
								//though I've defined them as Signed Integers!
								//I might try that again sometime. I think I was getting confused with
								//my ball positions... Still, this works for now...
								if (mrdo.Direction == DirLeft)
								{
									magicball.xDirection = 0;
									magicball.yDirection = 1;
							}
								else if (mrdo.Direction == DirRight)
								{
									magicball.xDirection = 1;
									magicball.yDirection = 1;
								}
								else if (mrdo.Direction == DirUp)
								{
									magicball.xDirection = 1;
									magicball.yDirection = 0;
								}
								else if (mrdo.Direction == DirDown)
								{
									magicball.xDirection = 0;
									magicball.yDirection = 1;
								}
								//Force the magic ball into position...
								magicball.xPosition+=(magicball.xPosition%2);
								magicball.yPosition+=(magicball.yPosition%2);
								magicball.Flags=0;

							}
						}

						//Now. Work out what the "proposed" position is.
						//Which is to say, mapping the sprite position into a game tile.
						//Based on dividing? mrdo.xPosition & mrdo.yPosition by 8
						//Causes Mr Do! to *always* get crushed by cherries. Although it does
						//stop him getting caught inside apples so often..
						if (mrdo.Direction==DirLeft)
						{
							xProposedPosition = ((mrdo.xPosition+10) / 16);
							yProposedPosition = ((mrdo.yPosition+8) / 16);
						}
						else if (mrdo.Direction==DirRight)
						{
							xProposedPosition = ((mrdo.xPosition+6) / 16);
							yProposedPosition = ((mrdo.yPosition+8) / 16);
						}
						else if (mrdo.Direction==DirUp)
						{
							xProposedPosition = ((mrdo.xPosition+8) / 16);
							yProposedPosition = ((mrdo.yPosition+10) / 16);
						}
						else if (mrdo.Direction==DirDown)
						{
							xProposedPosition = ((mrdo.xPosition+8) / 16);
							yProposedPosition = ((mrdo.yPosition+6) / 16);
						}
						else
						{
							xProposedPosition = ((mrdo.xPosition+8) / 16);
							yProposedPosition = ((mrdo.yPosition+8) / 16);
						}

						//Check the proposed position for any cherries!
						if (MrDoCurrentLevel[yProposedPosition][xProposedPosition]&MapCherry)
						{
							MrDoCurrentLevel[yProposedPosition][xProposedPosition]-=MapCherry;
							iTemp=CreateCherries(xProposedPosition, yProposedPosition);
							iCherryCount-=1;
							iScore+=100;
						}

						/*
						At this point we have two (x,y) co-ordinates.

						Yippee or something.

						Basically, our next task is to take those two tasks and
						work out which *walls* we need to take away. If any.

						Another question...
						Can this code be farmed off into it's own sub-routine somewhere?
						That way I can call the same block to determine wall removal
						not only for Mr Do!, but for falling apples and hungry beasties
						(should I ever get that far...)

						Answer: Yes! I can! And I have...

						*/
						if (xProposedPosition!=xPosition||yProposedPosition!=yPosition)
						{
							//The problem is that this doesn't take into account when the
							//player is being squashed by a falling apple...
							WallRemoval(xPosition, yPosition, xProposedPosition, yProposedPosition);
						}
						//Finally. Update the current (x,y) co-ordinates
						xPosition = xProposedPosition;
						yPosition = yProposedPosition;

					}
				}
				//Animate the magic ball...
				//Work out the tile collisions.
				//Basically, if we have hit a wall; we need to bounce...
				//Work out which tile we are currently in.
				//Is this a magicball interval
				if ((GameAnimationTimer%magicball.AnimationInterval)==0)
				{
					if ((magicball.Flags & FlagDead))
					{
						//If the ball is dead then it needs to be positioned next to the player...
						//Reset the sprite...
						if ((magicball.Flags & FlagInactive))
						{
							SetSprite(SPR_MAGICBALL, tlMagicBall, 0, magicball.xPosition, magicball.yPosition, 3, 16);
							magicball.Flags=magicball.Flags-FlagInactive;
							magicball.AnimationInterval=18;
						}

						//This code positions the magic ball in Mr Do!'s hand
						//Note: If Mr Do! is currently digging, then this should shove the
						//ball over his shoulder...
						if (mrdo.BaseAnimation & ANIM_OFFSET_MRDODIGGING)
						{
							//We're digging
							if (mrdo.Direction == DirLeft)
							{
								magicball.xPosition=mrdo.xPosition+9;
								magicball.yPosition=mrdo.yPosition+4-mrdo.Animation;
							}
							else if (mrdo.Direction == DirRight)
							{
								magicball.xPosition=mrdo.xPosition-1;
								magicball.yPosition=mrdo.yPosition+4-mrdo.Animation;
							}
							else if (mrdo.Direction == DirUp)
							{
								magicball.xPosition=mrdo.xPosition+4-mrdo.Animation;
								magicball.yPosition=mrdo.yPosition+9;
							}
							else if (mrdo.Direction == DirDown)
							{
								magicball.xPosition=mrdo.xPosition+4-mrdo.Animation;
								magicball.yPosition=mrdo.yPosition-1;
							}
						}
						else
						{
							//We're walking!
							if (mrdo.Direction == DirLeft)
							{
								magicball.xPosition=mrdo.xPosition-2;
								magicball.yPosition=mrdo.yPosition+6-mrdo.Animation;
							}
							else if (mrdo.Direction == DirRight)
							{
								magicball.xPosition=mrdo.xPosition+10;
								magicball.yPosition=mrdo.yPosition+6-mrdo.Animation;
							}
							else if (mrdo.Direction == DirUp)
							{
								magicball.xPosition=mrdo.xPosition+6-mrdo.Animation;
								magicball.yPosition=mrdo.yPosition-2;
							}
							else if (mrdo.Direction == DirDown)
							{
								magicball.xPosition=mrdo.xPosition+6-mrdo.Animation;
								magicball.yPosition=mrdo.yPosition+10;
							}
						}
						//Draw the sprite.
						SetSpritePosition(SPR_MAGICBALL, magicball.xPosition, magicball.yPosition);
					}
					else
					{
						//My main problem is that the magic ball co-ordinates are not being calculated correctically.
						//So I need to "tweak" them into the right grid point...
						xMagicBallPosition=(magicball.xPosition+4)/8;
						yMagicBallPosition=(magicball.yPosition+4)/8;

						//Actually, that's not my main problem, My main problem is that the magic ball is much
						//smaller than a single tile position. One quarter of the size in fact. Which means?
						//I don't know. It means something, I'm just not sure what it means. Fun this
						//Programming lark isn't it?

						//Possibly the most pointless debug code in history...
						//But I felt the need for this little lot, so don't bug me okay?
						/*
						PrintDecimal(SCR_1_PLANE, 0, 18, 1, xMagicBallPosition/2, 2);
						PrintDecimal(SCR_1_PLANE, 0, 18, 2, yMagicBallPosition/2, 2);
						//This is a bit rubbish. Why don't we show the wall...
						//PrintDecimal(SCR_1_PLANE, 0, 18, 3, MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2], 2);
						//Actually, now that I come to look at this, this code is *much* nicer than the
						//code I originally wrote to draw a single tile. Might be worth retrofitting this
						//into the original DrawTile() function.
						if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2]&(WallLeft+WallTop+WallRight+WallBottom)))
							PutTile(SCR_2_PLANE, 0, 18, 3, tlFull);
						else if (((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom)) && (!((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft))))
							PutTile(SCR_2_PLANE, 0, 18, 3, tlTopLeft);
						else if ((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft) && (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop))
							PutTile(SCR_2_PLANE, 0, 18, 3, tlNull);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop)
							PutTile(SCR_2_PLANE, 0, 18, 3, tlLeft);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft)
							PutTile(SCR_2_PLANE, 0, 18, 3, tlTop);
						if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & 0x0f))
							PutTile(SCR_2_PLANE, 0, 19, 3, tlFull);
						else if (((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom)) && (!((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight))))
							PutTile(SCR_2_PLANE, 0, 19, 3, tlTopRight);
						else if ((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight) && (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop))
							PutTile(SCR_2_PLANE, 0, 19, 3, tlNull);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop)
							PutTile(SCR_2_PLANE, 0, 19, 3, tlRight);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight)
							PutTile(SCR_2_PLANE, 0, 19, 3, tlTop);
						if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & 0xf))
							PutTile(SCR_2_PLANE, 0, 18, 4, tlFull);
						else if (((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight)) && (!((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft))))
							PutTile(SCR_2_PLANE, 0, 18, 4, tlBottomLeft);
						else if ((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft) && (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom))
							PutTile(SCR_2_PLANE, 0, 18, 4, tlNull);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom)
							PutTile(SCR_2_PLANE, 0, 18, 4, tlLeft);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft)
							PutTile(SCR_2_PLANE, 0, 18, 4, tlBottom);
						if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & 0xf))
							PutTile(SCR_2_PLANE, 0, 19, 4, tlFull);
						else if (((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft)) && (!((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom) || (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight))))
							PutTile(SCR_2_PLANE, 0, 19, 4, tlBottomRight);
						else if ((MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight) && (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom))
							PutTile(SCR_2_PLANE, 0, 19, 4, tlNull);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom)
							PutTile(SCR_2_PLANE, 0, 19, 4, tlRight);
						else if (MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight)
							PutTile(SCR_2_PLANE, 0, 19, 4, tlBottom);
						//This is more or less a contraction of the main maze drawing thing. It's a bit neater,
						//if a tad harder to follow. Might be worth farming off into it's own function somewhere...
						PrintString(SCR_1_PLANE, 0, 18, 5, "X");
						PrintDecimal(SCR_1_PLANE, 0, 19, 5, magicball.xDirection, 1);
						PrintString(SCR_1_PLANE, 0, 18, 6, "Y");
						PrintDecimal(SCR_1_PLANE, 0, 19, 6, magicball.yDirection, 1);
						PrintString(SCR_1_PLANE, 0, 18, 7, "X");
						PrintDecimal(SCR_1_PLANE, 0, 19, 7, xMagicBallPosition%2,1);
						PrintString(SCR_1_PLANE, 0, 18, 8, "Y");
						PrintDecimal(SCR_1_PLANE, 0, 19, 8, yMagicBallPosition%2,1);
						*/
						//I need to test the X & Y planes seperately....
						//Right. So "is the xposition an exact grid wossname?"...

						/*
						This code works for "flat" walls. i.e. when you hit either an
						XWall or a YWall on it's own.

						It *doesn't* work when it hits an outside corner. There is
						no wall (in either X or Y) directions for it to hit, so it
						goes straight into the background. Bit of a sod that one.
						So... What does that mean then.

						Basically, if we're in a corner heading *out* of a tile
						then we need to reverse both x & y

						Hang on. Shouldn't that hit both the X & Y walls anyway?
						I must be missing something here... No. I was right the
						first time. There are no walls for it to hit. The *corner*
						is defined by the other tiles bordering the background thang.

						*/

						if ((magicball.xPosition%8)==0)
						{
							//Check to see which (if any) walls are set for the
							//current tile.
							//Is this direction dependent? I think it is somehow...
							if (magicball.xDirection>0)
							{
								//i.e. the ball is moving from Left to Right?
								//Check for a wall to the right
								//Only check for "odd" tiles...
								if ((xMagicBallPosition%2)!=0)
								{
									//Check to see whether the wall to the right _hasn't_ been removed...
									//Why does this _always_ fail...
									if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallRight))
									{
										//If so (ie. It's a wall) reverse the direction. And slap something on
										//the screen to tell me about ...
										magicball.xDirection=0;
									}
								}
							}
							else
							{
								//Check for a wall to the left
								//Only check for "even" tiles
								if ((xMagicBallPosition%2)==0)
								{
									if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft))
									{
										magicball.xDirection=1;
									}
								}
							}
						}
						if ((magicball.yPosition%8)==0)
						{
							//Check to see which (if any) walls are set for the
							//current tile.
							if (magicball.yDirection==0)
							{
								//Check for a wall above
								//Only check for even tiles
								if ((yMagicBallPosition%2)==0)
								{
									if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallTop))
									{
										magicball.yDirection=1;
									}
									else
									{
									}
								}
							}
							else
							{
								//Check for a wall below
								if ((yMagicBallPosition%2)!=0)
								{
									if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallBottom))
									{
										magicball.yDirection=0;
									}
									else
									{
										//Check for the possible exception
									}
								}
							}
						}
						//How does the bouncing work then?

						//Update the ball position
						if (magicball.xDirection>0)
							magicball.xPosition+=2;
						else
							magicball.xPosition-=2;
						if (magicball.yDirection>0)
							magicball.yPosition+=2;
						else
							magicball.yPosition-=2;

						//Check for the exception - i.e. has the ball strayed into
						//the background. Reverse both directions if we have and move the
						//ball back to whence it came
						xMagicBallPosition=(magicball.xPosition+4)/8;
						yMagicBallPosition=(magicball.yPosition+4)/8;
						if (!(MrDoCurrentLevel[yMagicBallPosition/2][xMagicBallPosition/2] & WallLeft+WallTop+WallRight+WallBottom))
						{
							if (magicball.xDirection>0)
							{
								magicball.xDirection=0;
								magicball.xPosition-=4;
							}
							else
							{
								magicball.xDirection=1;
								magicball.xPosition+=4;
							}
							if (magicball.yDirection>0)
							{
								magicball.yDirection=0;
								magicball.yPosition-=4;
							}
							else
							{
								magicball.yDirection=1;
								magicball.yPosition+=4;
							}
						}

						//Draw the sprite.
						SetSpritePosition(SPR_MAGICBALL, magicball.xPosition, magicball.yPosition);
						//Work out sprite collisions
						//Mr Do!: Inactivate the sprite
						//Beastie: Kill the beastie and inactivate the sprite
						//Apple: Bounce
						//No Room! No Room!
						//What this will do is kill the ball in the moment of creation... We need to
						//achieve escape velocity before we can do any damn thing.
						if (SpriteCollision(magicball.xPosition,magicball.yPosition,mrdo.xPosition,mrdo.yPosition)==0x1)
						{
							magicball.Flags=FlagDead;
							//SetSprite(SPR_MAGICBALL, tlNull, 0, magicball.xPosition, magicball.yPosition, 3, 16);
						}

						//Beasties...
						for (iLoop=0;iLoop<=MaxBeastie;iLoop++)
						{
							//The general Sprite Collision function ain't much use for the
							//magic ball...
							if ((SpriteCollision(magicball.xPosition,magicball.yPosition,bEnemies[iLoop].xPosition,bEnemies[iLoop].yPosition)==0x1) && (!(bEnemies[iLoop].Flags & FlagInactive)))
							{
								//Return the ball to the player
								magicball.Flags=FlagDead+FlagInactive;
								magicball.AnimationInterval=2048+GameAnimationTimer;
								SetSprite(SPR_MAGICBALL, tlNull, 0, 0, 0, 0, 0);
								//Kill the beastie
								bEnemies[iLoop].Flags=FlagInactive+FlagDead;
								SetSprite(SPR_BEASTIE+(iLoop*4), tlNull, 0, 0, 0, 0, 0);
								SetSprite(SPR_BEASTIE+(iLoop*4)+1, tlNull, 1, 0, 0, 0, 0);
								SetSprite(SPR_BEASTIE+(iLoop*4)+2, tlNull, 1, 0, 0, 0, 0);
								SetSprite(SPR_BEASTIE+(iLoop*4)+3, tlNull, 1, 0, 0, 0, 0);
								iDeadBeastieCount++;
								iScore=iScore+200;
								iLoop=MaxBeastie;
							}
						}
					}
				}

				if ((GameAnimationTimer%BeastieInterval)==0)
				{
					//Do Beastie Spawning
					//Does it ever actually reach here? My debug code isn't showing up?
					if (MaxBeastie<iBeastieCount-1)
					{
						BeastieTimer +=1;
						//Create the sprite a couple of frames before actually spawning the beastie
						if (BeastieTimer==39)
						{
							bEnemies[MaxBeastie+1].xPosition = 4 * 16;
							bEnemies[MaxBeastie+1].yPosition = 4 * 16;
							bEnemies[MaxBeastie+1].Animation = 0;
							bEnemies[MaxBeastie+1].BaseAnimation = ANIM_OFFSET_BEASTIE;
							bEnemies[MaxBeastie+1].Direction = 0;
							bEnemies[MaxBeastie+1].Flags=FlagInactive;

							SetSprite(SPR_BEASTIE+((MaxBeastie+1)*4), TILE_SPRITE_BEASTIE+((MaxBeastie+1)*4), 0, bEnemies[MaxBeastie+1].xPosition, bEnemies[MaxBeastie+1].yPosition, 1,16);
							SetSprite(SPR_BEASTIE+((MaxBeastie+1)*4)+1, TILE_SPRITE_BEASTIE+((MaxBeastie+1)*4)+1, 1, 8, 0, 1,16);
							SetSprite(SPR_BEASTIE+((MaxBeastie+1)*4)+2, TILE_SPRITE_BEASTIE+((MaxBeastie+1)*4)+2, 1, -8, 8, 1,16);
							SetSprite(SPR_BEASTIE+((MaxBeastie+1)*4)+3, TILE_SPRITE_BEASTIE+((MaxBeastie+1)*4)+3, 1, 8, 0, 1,16);

						}
						else if (BeastieTimer==43)
						{
							MaxBeastie +=1;
							bEnemies[MaxBeastie].Flags=0;
							BeastieTimer=0;
						}
					}

					//Where is the beastie?
					//This will determine which directions he can move in.

					//Need to work out where the beastie can move to...

					//bEnemies.Direction will tell us which way the beastie is
					//currently travelling. Basically, we want to continue in this
					//direction until it cannot go any further...

					//Once we've run out of steam...
					//Work out which directions we can go in from here.
					//Pick one at random.

					for (iLoop=0; iLoop<=MaxBeastie; iLoop++)
					{
						bEnemies[iLoop].Animation ^= 1; // toggles anim 0->1->0

						//Only do this if the beastie is not dead and not inactive
						if (!((bEnemies[iLoop].Flags & FlagInactive) || (bEnemies[iLoop].Flags & FlagDead)))
						{
							//Which (game) tile is the beastie in now?
							xBeastiePosition = ((bEnemies[iLoop].xPosition+8) / 16);
							yBeastiePosition = ((bEnemies[iLoop].yPosition+8) / 16);

							//Beasties really do need to try and turn around if they run into an
							//apple...
							//We know which direction a beastie is moving in...
							if (bEnemies[iLoop].Direction==DirLeft||bEnemies[iLoop].Direction==DirRight)
							{
								for (iAppleLoop=0;iAppleLoop<=MaxApple-1;iAppleLoop++)
								{
									//Well, what are you waiting for?
									//Do something!!!!
								}
							}

							if ((xBeastiePosition*16 == bEnemies[iLoop].xPosition) && (yBeastiePosition*16 == bEnemies[iLoop].yPosition))
							{
								//Beastie is centered in a game tile.
								//Where does he want to go next?
								//Can he keep going in the same direction?
								//Basically, the test we have here is at fault...
								//The beastie does not go in a straight line 'til he hits a wall -
								//NO! What he should do is go in a straight line 'til he
								//hits a junction!

								//So. If a tile has two exits, we leave by the one we didn't come
								//into.
								//If a tile has one exit, then we go back the way we came...
								//Otherwise, we choose one of the others at random.
								//

								WallCount = 0;
								if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallTop) WallCount +=1;
								if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallLeft) WallCount +=1;
								if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallBottom) WallCount +=1;
								if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallRight) WallCount +=1;

								//I *may* be able to combine WallCount==2 thru to WallCount==4
								//into a single test...

								if (WallCount == 1)
								{
									//Turn around
									if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallTop) bEnemies[iLoop].Direction=DirUp;
									else if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallRight) bEnemies[iLoop].Direction=DirRight;
									else if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallBottom) bEnemies[iLoop].Direction=DirDown;
									else if (MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallLeft) bEnemies[iLoop].Direction=DirLeft;
								}
								//Question? Can we just do the >=3 thing here?
								else if (WallCount == 2)
								{
									//Go through the other exit
									//Um? What do we mean by "other" exit...
									//We *know* which direction we started from?
									//& we therefore use that to work out which direction we want to leave from.
									ProposedBeastieDirection=bEnemies[iLoop].Direction;
									if (!(((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition]&WallTop) && (bEnemies[iLoop].Direction==DirUp))||((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition]&WallRight) && (bEnemies[iLoop].Direction==DirRight))||((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition]&WallBottom) && (bEnemies[iLoop].Direction==DirDown))||((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition]&WallLeft) && (bEnemies[iLoop].Direction==DirLeft))))
									{
										while (ProposedBeastieDirection==bEnemies[iLoop].Direction)
										//Why the hell doesn't this work around corners?
										//Answer: It does, stop complaining...
										{
											if ((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallTop) && (bEnemies[iLoop].Direction!=DirDown)) ProposedBeastieDirection=DirUp;
											if ((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallRight) && (bEnemies[iLoop].Direction!=DirLeft)) ProposedBeastieDirection=DirRight;
											if ((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallBottom) && (bEnemies[iLoop].Direction!=DirUp)) ProposedBeastieDirection=DirDown;
											if ((MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallLeft) && (bEnemies[iLoop].Direction!=DirRight)) ProposedBeastieDirection=DirLeft;
										}
									}
									bEnemies[iLoop].Direction=ProposedBeastieDirection;
								}
								else if (WallCount >= 3)
								{
									//Choose one of the other exits
									//Um? What do we mean by "other" exit...
									//We *know* which direction we started from?
									//This loop does have a tendancy to make the beasties go in circles...
									bEnemies[iLoop].Direction=GetRandom(3)*8;
									while (((!(MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallTop)) && (bEnemies[iLoop].Direction==DirUp)) || ((!(MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallRight)) && (bEnemies[iLoop].Direction==DirRight)) || ((!(MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallBottom)) && (bEnemies[iLoop].Direction==DirDown)) || ((!(MrDoCurrentLevel[yBeastiePosition][xBeastiePosition] & WallLeft)) && (bEnemies[iLoop].Direction==DirLeft)))
										bEnemies[iLoop].Direction=GetRandom(3)*8;
								}
							}

							if (bEnemies[iLoop].Direction == DirLeft) //Left
							{
								bEnemies[iLoop].xPosition -= 2;
							}
							else if (bEnemies[iLoop].Direction == DirRight) //Right
							{
								bEnemies[iLoop].xPosition += 2;
							}
							else if (bEnemies[iLoop].Direction == DirUp) //Up
							{
								bEnemies[iLoop].yPosition -= 2;
							}
							else if (bEnemies[iLoop].Direction == DirDown) //Down
							{
								bEnemies[iLoop].yPosition += 2;
							}

							//Bastard routine insists on overwriting the wrong bit of the tile space...
							//Fixed that by upping the size of the sprite number variable to an unsigned short.
							//For some reason it was losing the most significant bits when the result
							//passed in was >31?
							//Cracked this now. 32*8 = 256. You need eight bytes for a tile, and once an
							//unsigned char value goes over 255, it drops the most significant bit.

							CopySpriteAnimation(SPR_BEASTIE+(iLoop*4), bEnemies[iLoop].BaseAnimation+bEnemies[iLoop].Direction+(bEnemies[iLoop].Animation * 4),TILE_SPRITE_BEASTIE+(iLoop*4));
							SetSpritePosition(SPR_BEASTIE+(iLoop*4), bEnemies[iLoop].xPosition, bEnemies[iLoop].yPosition);

							//Check for collision with the player
							//SpriteCollision() function might be a bit too wossname...
							if (SpriteCollision(mrdo.xPosition, mrdo.yPosition, bEnemies[iLoop].xPosition,bEnemies[iLoop].yPosition)==1)
							{
								//Don't forget to check for apples? i.e. if the player is not "active"?
								//Needed to fix a bug where the player could be killed by > 1 beastie and
								//the flags would cancel each other out ;-)
								if (!((mrdo.Flags & FlagDead) || (mrdo.Flags & FlagInactive)))
									mrdo.Flags=mrdo.Flags + FlagDead;
							}

						}
					}
				}
				//Righty ho. Check the apples & make 'em fall down if they need to...
				//Question. Why do I seem to get a phantom apple in the top left hand corner?

				//Because I hadn't initialised the complete array of course... Using the
				//MaxApple variable rather than the constant MAX_APPLE should fix that.

				//Anyhoo, we still have some very odd apple behaviour. In order of weirdness
				//then...
				//Apples will sometimes fall off the edge of the screen
				//Apples will not always crush the player.

				//I think I've cracked most of the problems with apples, still a few
				//oddities though.

				//Always seems to count too many apples? Not exact;y...
				//There's always an Apple[0] which isn't properly setup...
				//Or rather... It's created properly but something always resets it
				//at the start of each level/life

				for (iLoop=0; iLoop<=(MaxApple-1); iLoop++)
				{
					if ((GameAnimationTimer%aApples[iLoop].AnimationInterval)==0)
					{
						if (aApples[iLoop].Flags & FlagInactive)
						{
							//Apple is not active, but hasn't died yet. I.e it's in the
							//"breaking apart and squashing things" state
							if (!(aApples[iLoop].Flags & FlagDead))
							{
								//Can't be bothered with the animation yet.
								//Just kill the fucker off.
								aApples[iLoop].Flags=aApples[iLoop].Flags+FlagDead;
								//Set the sprite to "nothing"
								SetSprite(SPR_APPLE+(iLoop*4), tlNull, 0, aApples[iLoop].xPosition, aApples[iLoop].yPosition, 2,16);
								SetSprite(SPR_APPLE+(iLoop*4)+1, tlNull, 1, 8, 0, 2,16);
								SetSprite(SPR_APPLE+(iLoop*4)+2, tlNull, 1, -8, 8, 2,16);
								SetSprite(SPR_APPLE+(iLoop*4)+3, tlNull, 1, 8, 0, 2,16);
							}

						}
						else if (!(aApples[iLoop].Flags & FlagDead))

						{
							//Which tile is the apple in?
							//If the tile below (x, y+1?) has >0 walls missing
							//make the apple fall...
							xApplePosition = ((aApples[iLoop].xPosition+8) / 16);
							yApplePosition = ((aApples[iLoop].yPosition) / 16);
							//Check the position *below* the current position
							//If anything is missing, then the apple can fall...
							if ((MrDoCurrentLevel[yApplePosition+1][xApplePosition] & 0xf) && yApplePosition<MAX_Y-1)
							{
								//Farm off to the wallremoval function to check whether
								//the apple "breaks" through a maze wall.
								if ((MrDoCurrentLevel[yApplePosition][xApplePosition] & 0xf))
								{
									WallRemoval(xApplePosition, yApplePosition, xApplePosition, yApplePosition+1);
								}
								//Move the sprite...
								aApples[iLoop].DistanceFell+=2;
								if (aApples[iLoop].DistanceFell>32)
									aApples[iLoop].AnimationInterval=24;

								//hang around until the "wobble" finishes. Give it a bit more time shall we..
								if (aApples[iLoop].DistanceFell>32)
								{
									aApples[iLoop].xPosition=xApplePosition*16;
									aApples[iLoop].yPosition+=2;
									SetSpritePosition(SPR_APPLE+(iLoop*4), aApples[iLoop].xPosition, aApples[iLoop].yPosition);
									//Check to see whether any beasties are underneath the apple. If so, move them
									//down as well. And the player i suppose.
									if (SpriteCollision(mrdo.xPosition, mrdo.yPosition , aApples[iLoop].xPosition, aApples[iLoop].yPosition))
									{
										//Not as simple as this...
										//This tends to rip holes in the scenery...
										mrdo.yPosition+=2;
										mrdo.xPosition=aApples[iLoop].xPosition;
										SetSpritePosition(SPR_SPRITE, mrdo.xPosition, mrdo.yPosition);
										mrdo.Animation ^= 1;
										//Changing this flag stops the holes at least...
										//Now it just pushes the player into the background
										xPosition = ((mrdo.xPosition+8) / 16);
										yPosition = ((mrdo.yPosition+8) / 16);
										if (!(mrdo.Flags & FlagInactive))
											mrdo.Flags = mrdo.Flags + FlagInactive;
									}

									//Now for beasties...
									for (iBeastieLoop=0;iBeastieLoop<=MaxBeastie;iBeastieLoop++)
									{
										//We only care if the beastie is _not_ dead
										if (!(bEnemies[iBeastieLoop].Flags & FlagDead))
										{
										if (SpriteCollision(bEnemies[iBeastieLoop].xPosition, bEnemies[iBeastieLoop].yPosition, aApples[iLoop].xPosition, aApples[iLoop].yPosition))
										{
											//Not as simple as this...
											//This tends to rip holes in the scenery...
											bEnemies[iBeastieLoop].yPosition+=2;
											bEnemies[iBeastieLoop].xPosition=aApples[iLoop].xPosition;
											SetSpritePosition(SPR_BEASTIE+(iBeastieLoop*4), bEnemies[iBeastieLoop].xPosition, bEnemies[iBeastieLoop].yPosition);
											bEnemies[iBeastieLoop].Animation ^= 1;
											//Changing this flag stops the holes at least...
											//Now it just pushes the player into the background
											//xPosition = ((bEnemies[iBeastieLoop].xPosition+8) / 16);
											//yPosition = ((bEnemies[iBeastieLoop].yPosition+8) / 16);
											if (!(bEnemies[iBeastieLoop].Flags & FlagInactive))
												bEnemies[iBeastieLoop].Flags = bEnemies[iBeastieLoop].Flags + FlagInactive;
											}
										}
									}
								}
							}
							else
							{

								if(aApples[iLoop].DistanceFell>0)
								{
									//Check the resting place...
									//If we've hit the "bottom" then either update the DistanceFell (=0)
									//or start the destroy apple animation (and squash any beasties that are under it)
									//Can't be bothered at the moment. Just update DistanceFell...
									if (aApples[iLoop].DistanceFell>48)
									{
										//Destroy all apples
										aApples[iLoop].Flags=aApples[iLoop].Flags+FlagInactive;
									}
									aApples[iLoop].DistanceFell=0;

									//If any beasties are "underneath" the apple
									//KILL!THEM!ALL!
									//& the player?
									//Game over man...
									//Have tried being a bit less specific with this test, but it
									//doesn't work when the player sprite get's pushed off the
									//bottom of the screen. Don't know why not? This was a boundary
									//checking error in the main "am I falling" decision...
									//However, this does have a tendancy to bury the player as well..
									if (SpriteCollision(mrdo.xPosition, mrdo.yPosition-8, aApples[iLoop].xPosition, aApples[iLoop].yPosition))
									{
										mrdo.Flags = mrdo.Flags + FlagDead;
									}

									//Beasties are a bit more complicated. We've got to reduce the
									//Beastie count and shuffle all other beasties up the list.
									//If that makes any sense.
									//Probably want to do some of that score type stuff too.

									for (iBeastieLoop=0;iBeastieLoop<=MaxBeastie;iBeastieLoop++)
									{
										//A beastie may already be dead...
										if (!(bEnemies[iBeastieLoop].Flags&FlagDead))
										{
											if (SpriteCollision(bEnemies[iBeastieLoop].xPosition, bEnemies[iBeastieLoop].yPosition-8, aApples[iLoop].xPosition, aApples[iLoop].yPosition))
											{
												bEnemies[iBeastieLoop].Flags = bEnemies[iBeastieLoop].Flags + FlagDead;
												SetSprite(SPR_BEASTIE+(iBeastieLoop*4), tlNull, 0, bEnemies[iBeastieLoop].xPosition, bEnemies[iBeastieLoop].yPosition, 1,16);
												SetSprite(SPR_BEASTIE+(iBeastieLoop*4)+1, tlNull, 1, 8, 0, 1,16);
												SetSprite(SPR_BEASTIE+(iBeastieLoop*4)+2, tlNull, 1, -8, 8, 1,16);
												SetSprite(SPR_BEASTIE+(iBeastieLoop*4)+3, tlNull, 1, 8, 0, 1,16);
												iScore=iScore+200;
												iDeadBeastieCount++;
											}
										}
									}
								}
								//Simple as that. The only thing(s) left to do now are:
								//1. Make the apple "wobble" a bit before falling (give mr do! a chance to run away)
								//2. Make the apple slide when it hit's a corner
								//3. Make the apple fall apart when it hit's the bottom (but only if fall distance > 1)
								//4. The position that the apple falls "out of" stay's full when the apple leaves. (if that makes
								//any sense whatsoever
							}
						}
					}
				}

				/*
				Mr Do! behaviour...

				The key thing here is that the character position is currently a 1:1
				relationship with the background tiles.

				*however*
				What I really want is for *5* character positions to be within a single
				background tile...

				Look see at the sprite code above. Isn't it lovely...

				Update: There's rather a lot more of it than there was when I originally
				wrote this comment. It's still rather lovely though.

				*/

				/*
				//Original code to draw the main character - based on tiles...
				//Left in here because... Well, for no very good reason. I may need it again
				/eventually.
				//Draw Mr Do!
				PutTile(SCR_1_PLANE, 1, xPosition * 2, yPosition * 2, tlMrDo+tlCharTopLeft);
				PutTile(SCR_1_PLANE, 1, (xPosition * 2) + 1, yPosition * 2, tlMrDo+tlCharTopRight);
				PutTile(SCR_1_PLANE, 1, xPosition * 2, (yPosition * 2) + 1, tlMrDo+tlCharBottomLeft);
				PutTile(SCR_1_PLANE, 1, (xPosition * 2) + 1, (yPosition * 2) + 1, tlMrDo+tlCharBottomRight);
				*/

				//Print the score and then
				//spend some time doing nothing very much of anything...
				//Actually, I don't. The "Not very much of anything" is now taken care of
				//by the various AnimationInterval variables - why? Because everything in the
				//game moves at a different rate...
				PrintDecimal(SCR_1_PLANE, 0, 0, 18, iScore, 6);
				GameAnimationTimer++;
			}
		}
	}
}
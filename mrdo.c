#define MAX_BEASTIE 5
#define MAX_APPLE 5

const u8 DirUp = 0x8;
const u8 DirLeft = 0x10;
const u8 DirRight = 0x18;
const u8 DirDown = 0x0;

const u8 True = 1;
const u8 MAX_Y = 9;
const u8 MAX_X = 9;
const u8 X_OFFSET = 1;
const u8 Y_OFFSET = 1;


u8 MrDoCurrentLevel[9][9];

//Sprite Structure vars
BEASTIE bEnemies[MAX_BEASTIE];
APPLE aApples[MAX_APPLE];
MRDO mrdo;
MAGICBALL magicball;

u8 MaxApple;
u8 MaxBeastie;
u8 MaxCherry;

// copies the new animation onto the Sprites tiles.
void CopySpriteAnimation(u8 SpriteNumber, u16 AnimNumber, u16 SpriteTile)
{

	//Basically, this copies the 32? bytes from the sprite_tile
	//array into the relevant part of TILERAM.
	//It does rather assume that a sprite is 4 tiles in size?

	//Or, at least, that's the theory...
	//In practice, it does tend to overwrite the wrong bit of tileram
	//which explains why I get animated backgrounds all the time...


	//Second beastie copies into TILE_RAM[2]-TILE_RAM[6]?

	u8 i;
	u16 * TileRam = TILE_RAM;
	u16 * SpriteAnimation = (u16 *) Sprite_Tiles;

	TileRam += ((SpriteTile) * 8);
	SpriteAnimation += (AnimNumber * 8);

	for (i = 0; i < 32; i ++)
	{
		TileRam[i] = *SpriteAnimation++;
	}
}

void CopyMrDoMap(u8 Level)
{

	u8 ix;
	u8 iy;

   	for (iy = 0; iy < MAX_Y; iy++)
	{
	      	for (ix = 0; ix < MAX_X; ix++)
      		{
	         	MrDoCurrentLevel[iy][ix] = MrDoLevels[Level][iy][ix];

      		}
   	}
}

void DrawMrDoBackGround(u8 xPos, u8 yPos)
{
	u8 Tile;

	Tile = 0x1;

	//Look at each "corner" of the tile in rotation...
	//Top left...
	//Four possibilities...
	//if left & top wall missing : Nothing
	//if left wall missing : Top Wall
	//if top wall missing : Left Wall
	//if no walls missing : Background...
	if (!(MrDoCurrentLevel[yPos][xPos] & 0xf))
	{
		//No walls missing
		Tile = tlFull;
	}
	else if (((MrDoCurrentLevel[yPos][xPos] & WallRight) || (MrDoCurrentLevel[yPos][xPos] & WallBottom)) && (!((MrDoCurrentLevel[yPos][xPos] & WallTop) || (MrDoCurrentLevel[yPos][xPos] & WallLeft))))
	{
		//Neither wall missing, but another one is...
		Tile = tlTopLeft;
	}
	else if ((MrDoCurrentLevel[yPos][xPos] & WallLeft) && (MrDoCurrentLevel[yPos][xPos] & WallTop))
	{
		//Both walls missing
		Tile = tlNull;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallTop)
	{
		//Top wall missing
		Tile = tlLeft;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallLeft)
	{
		//Left wall missing
		Tile = tlTop;
	}
	PutTile(SCR_2_PLANE, 0, xPos * 2, yPos * 2, Tile);
	//Top Right
	if (!(MrDoCurrentLevel[yPos][xPos] & 0x0f))
	{
		//No walls missing
		Tile = tlFull;
	}
	else if (((MrDoCurrentLevel[yPos][xPos] & WallLeft) || (MrDoCurrentLevel[yPos][xPos] & WallBottom)) && (!((MrDoCurrentLevel[yPos][xPos] & WallTop) || (MrDoCurrentLevel[yPos][xPos] & WallRight))))
	{
		//Neither wall missing, but another one is...
		Tile = tlTopRight;
	}
	else if ((MrDoCurrentLevel[yPos][xPos] & WallRight) && (MrDoCurrentLevel[yPos][xPos] & WallTop))
	{
		//Both walls missing
		Tile = tlNull;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallTop)
	{
		//Top wall missing
		Tile = tlRight;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallRight)
	{
		//Right wall missing
		Tile = tlTop;
	}
	PutTile(SCR_2_PLANE, 0, (xPos * 2) + 1, yPos * 2, Tile);
	//Bottom left...
	if (!(MrDoCurrentLevel[yPos][xPos] & 0xf))
	{
		//No walls missing
		Tile = tlFull;
	}
	else if (((MrDoCurrentLevel[yPos][xPos] & WallTop) || (MrDoCurrentLevel[yPos][xPos] & WallRight)) && (!((MrDoCurrentLevel[yPos][xPos] & WallBottom) || (MrDoCurrentLevel[yPos][xPos] & WallLeft))))
	{
		//Neither wall missing, but another one is...
		Tile = tlBottomLeft;
	}
	else if ((MrDoCurrentLevel[yPos][xPos] & WallLeft) && (MrDoCurrentLevel[yPos][xPos] & WallBottom))
	{
		//Both walls missing
		Tile = tlNull;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallBottom)
	{
		//Left wall missing
		Tile = tlLeft;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallLeft)
	{
		//Bottom wall missing
		Tile = tlBottom;
	}
	PutTile(SCR_2_PLANE, 0, xPos * 2, (yPos * 2)+1, Tile);
	//Bottom Right...
	if (!(MrDoCurrentLevel[yPos][xPos] & 0xf))
	{
		//No walls missing
		Tile = tlFull;
	}
	else if (((MrDoCurrentLevel[yPos][xPos] & WallTop) || (MrDoCurrentLevel[yPos][xPos] & WallLeft)) && (!((MrDoCurrentLevel[yPos][xPos] & WallBottom) || (MrDoCurrentLevel[yPos][xPos] & WallRight))))
	{
		//Neither wall missing, but another one is...
		Tile = tlBottomRight;
	}
	else if ((MrDoCurrentLevel[yPos][xPos] & WallRight) && (MrDoCurrentLevel[yPos][xPos] & WallBottom))
	{
		//Both walls missing
		Tile = tlNull;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallBottom)
	{
		//Bottom wall missing
		Tile = tlRight;
	}
	else if (MrDoCurrentLevel[yPos][xPos] & WallRight)
	{
		//Right wall missing
		Tile = tlBottom;
	}
	PutTile(SCR_2_PLANE, 0, (xPos * 2) + 1, (yPos * 2) + 1, Tile);
}

u8 GetShortestRouteRecurse(u8 xSourcePosition, u8 ySourcePosition, u8 xTargetPosition, u8 yTargetPosition, u8 Direction)
{
	u8 DistanceUp;
	u8 DistanceRight;
	u8 DistanceDown;
	u8 DistanceLeft;

	u8 DistanceReturn;

	DistanceUp=255;
	DistanceRight=255;
	DistanceDown=255;
	DistanceLeft=255;

	if ((xSourcePosition==xTargetPosition) && (ySourcePosition==yTargetPosition))
	{
		//Calculate the shortest distance in each direction
		if ((MrDoCurrentLevel[ySourcePosition][xSourcePosition]&WallTop)&&(Direction!=DirDown))
			DistanceUp=GetShortestRouteRecurse(xSourcePosition,ySourcePosition-1,xTargetPosition,yTargetPosition,DirUp);
		if ((MrDoCurrentLevel[ySourcePosition][xSourcePosition]&WallRight)&&(Direction!=DirLeft))
			DistanceRight=GetShortestRouteRecurse(xSourcePosition+1,ySourcePosition,xTargetPosition,yTargetPosition,DirRight);
		if ((MrDoCurrentLevel[ySourcePosition][xSourcePosition]&WallBottom)&&(Direction!=DirUp))
			DistanceDown=GetShortestRouteRecurse(xSourcePosition,ySourcePosition+1,xTargetPosition,yTargetPosition,DirDown);
		if ((MrDoCurrentLevel[ySourcePosition][xSourcePosition]&WallBottom)&&(Direction!=DirRight))
			DistanceLeft=GetShortestRouteRecurse(xSourcePosition-1,ySourcePosition,xTargetPosition,yTargetPosition,DirLeft);

		//We now have four distances...
		//Which one is shortest?
		DistanceReturn=((DistanceUp-(DistanceUp%16))*16)+DirUp;
		if ((DistanceRight*16)<DistanceReturn)
			DistanceReturn=((DistanceRight-(DistanceRight%16))*16)+DirRight;
		if ((DistanceDown*16)<DistanceReturn)
			DistanceReturn=((DistanceDown-(DistanceDown%16))*16)+DirDown;
		if ((DistanceLeft*16)<DistanceReturn)
			DistanceReturn=((DistanceLeft-(DistanceLeft%16))*16)+DirLeft;

		return DistanceReturn+16;
	}
	else
	{
		return 0;
	}
}

u8 GetShortestRoute(u8 xSourcePosition, u8 ySourcePosition, u8 xTargetPosition, u8 yTargetPosition)
{
	u8 DistanceUp;
	u8 DistanceRight;
	u8 DistanceDown;
	u8 DistanceLeft;

	//Function to determine which direction from the Source position leads to the shortest
	//path to the targetposition.

	//Yer basic concept is... Recursion.

	//Try each position relative to the source in turn (Top, Left, Right, Bottom) One of these will, through
	//the magic of recursion be *closer* to the target than the source position is.
	//All we have to do is work that out and return the relevant direction code.
	//(DirUp, DirLeft, DirDown, DirRight)

	//Trouble is. I need to know the *distance* as well as the direction...

	//I could code the distance into the return variable (low bits for direction, high bits for distance)
	//Alternatively, I could pass the distance into the function (probably as a pointery type thang)

	//I think the former somehow... Although, does this function need to know which direction the
	//character is "coming" from? ie. "Don't check the "Down" direction as that's where I started..."

	DistanceUp=GetShortestRouteRecurse(xSourcePosition, ySourcePosition-1, xTargetPosition, yTargetPosition, DirUp);
	DistanceRight=GetShortestRouteRecurse(xSourcePosition+1, ySourcePosition, xTargetPosition, yTargetPosition, DirRight);
	DistanceDown=GetShortestRouteRecurse(xSourcePosition, ySourcePosition+1, xTargetPosition, yTargetPosition, DirDown);
	DistanceLeft=GetShortestRouteRecurse(xSourcePosition-1, ySourcePosition, xTargetPosition, yTargetPosition, DirLeft);

	return DirUp;

}

void WallRemoval(u8 xPosition, u8 yPosition, u8 xProposedPosition, u8 yProposedPosition)
{

//Code to calculate whether a "movement" between two tiles
//will cause any walls to be removed. & to further remove the
//walls if required. & updates the map.
//Used when Mr Do! moves into the background
//Will also be used during the apple fall routines...
//And, should the beasties get hungry, then too...

	if ((xPosition != xProposedPosition) || (yPosition !=yProposedPosition))
	{

		//Now do the wall translation stuff
		if (xPosition < xProposedPosition)
		{
			// Player has moved right
			if (!(MrDoCurrentLevel[yPosition][xPosition] & WallRight)) MrDoCurrentLevel[yPosition][xPosition] = MrDoCurrentLevel[yPosition][xPosition] + WallRight;
			if (!(MrDoCurrentLevel[yProposedPosition][xProposedPosition] & WallLeft)) MrDoCurrentLevel[yProposedPosition][xProposedPosition] = MrDoCurrentLevel[yProposedPosition][xProposedPosition] + WallLeft;
			DrawMrDoBackGround(xPosition, yPosition);
			DrawMrDoBackGround(xProposedPosition, yProposedPosition);
		}
		else if (xPosition > xProposedPosition)

		{
			// Player has moved left
			if (!(MrDoCurrentLevel[yPosition][xPosition] & WallLeft)) MrDoCurrentLevel[yPosition][xPosition] = MrDoCurrentLevel[yPosition][xPosition] + WallLeft;
			if (!(MrDoCurrentLevel[yProposedPosition][xProposedPosition] & WallRight)) MrDoCurrentLevel[yProposedPosition][xProposedPosition] = MrDoCurrentLevel[yProposedPosition][xProposedPosition] + WallRight;
			DrawMrDoBackGround(xPosition, yPosition);
			DrawMrDoBackGround(xProposedPosition, yProposedPosition);
		}

		else if (yPosition < yProposedPosition)
		{
			// Player has moved down
			if (!(MrDoCurrentLevel[yPosition][xPosition] & WallBottom)) MrDoCurrentLevel[yPosition][xPosition] = MrDoCurrentLevel[yPosition][xPosition] + WallBottom;
			if (!(MrDoCurrentLevel[yProposedPosition][xProposedPosition] & WallTop)) MrDoCurrentLevel[yProposedPosition][xProposedPosition] = MrDoCurrentLevel[yProposedPosition][xProposedPosition] + WallTop;
			DrawMrDoBackGround(xPosition, yPosition);
			DrawMrDoBackGround(xProposedPosition, yProposedPosition);
		}
		else if (yPosition > yProposedPosition)
		{
			// Player has moved up
			if (!(MrDoCurrentLevel[yPosition][xPosition] & WallTop)) MrDoCurrentLevel[yPosition][xPosition] = MrDoCurrentLevel[yPosition][xPosition] + WallTop;
			if (!(MrDoCurrentLevel[yProposedPosition][xProposedPosition] & WallBottom)) MrDoCurrentLevel[yProposedPosition][xProposedPosition] = MrDoCurrentLevel[yProposedPosition][xProposedPosition] + WallBottom;
			DrawMrDoBackGround(xPosition, yPosition);
			DrawMrDoBackGround(xProposedPosition, yProposedPosition);
		}

	}

}

void CreateApples(u8 xPosition, u8 yPosition)
{

	//Apples!
	//Right then. What we really need to do here is to create a new apple sprite
	//rather than just pathetically doing the tile thing.
	if (MrDoCurrentLevel[yPosition][xPosition] & 0x20)
	{
/*
	{
		PutTile(SCR_1_PLANE, 3, (xPos * 2), yPos * 2, tlApple+tlCharTopLeft);
	   	PutTile(SCR_1_PLANE, 3, (xPos * 2) + 1, yPos * 2, tlApple+tlCharTopRight);
   		PutTile(SCR_1_PLANE, 3, (xPos * 2), (yPos * 2) + 1, tlApple+tlCharBottomLeft);
	   	PutTile(SCR_1_PLANE, 3, (xPos * 2) + 1, (yPos * 2) + 1, tlApple+tlCharBottomRight);
	}
*/

		//MaxApple variable will contain the *next* apple id.
		aApples[MaxApple].xPosition=xPosition*16;
		aApples[MaxApple].yPosition=yPosition*16;
		aApples[MaxApple].Flags=0x0;
		aApples[MaxApple].DistanceFell=0x0;
		aApples[MaxApple].AnimationInterval=16;

		//Apple sprites are simpler than beastie and character ones as there is no
		//animation - therefore they can all share the same tile!
		SetSprite(SPR_APPLE+(MaxApple*4), TILE_APPLE, 0, aApples[MaxApple].xPosition, aApples[MaxApple].yPosition, 2,16);
		SetSprite(SPR_APPLE+(MaxApple*4)+1, TILE_APPLE+1, 1, 8, 0, 2,16);
		SetSprite(SPR_APPLE+(MaxApple*4)+2, TILE_APPLE+2, 1, -8, 8, 2,16);
		SetSprite(SPR_APPLE+(MaxApple*4)+3, TILE_APPLE+3, 1, 8, 0, 2,16);

		//And then...
		MrDoCurrentLevel[yPosition][xPosition]=MrDoCurrentLevel[yPosition][xPosition]-0x20;

		MaxApple=MaxApple+1;
	}
}

u8 CreateCherries(u8 xPosition, u8 yPosition)
{
	u8 NewTile;
	u8 CherryCount;

	CherryCount=0;

	//Cherries!
	if (MrDoCurrentLevel[yPosition][xPosition] & 0x10)
	{
	   	PutTile(SCR_1_PLANE, 2, xPosition * 2, yPosition * 2, tlCherry+tlCharTopLeft);
	   	PutTile(SCR_1_PLANE, 2, (xPosition * 2) + 1, yPosition * 2, tlCherry+tlCharTopRight);
   		PutTile(SCR_1_PLANE, 2, xPosition * 2, (yPosition * 2) + 1, tlCherry+tlCharBottomLeft);
	   	PutTile(SCR_1_PLANE, 2, (xPosition * 2) + 1, (yPosition * 2) + 1, tlCherry+tlCharBottomRight);
		CherryCount+=1;
	}
	else
	{
		//Pourquoi?
	   	PutTile(SCR_1_PLANE, 2, xPosition * 2, yPosition * 2, tlNull);
	   	PutTile(SCR_1_PLANE, 2, (xPosition * 2) + 1, yPosition * 2, tlNull);
   		PutTile(SCR_1_PLANE, 2, xPosition * 2, (yPosition * 2) + 1, tlNull);
	   	PutTile(SCR_1_PLANE, 2, (xPosition * 2) + 1, (yPosition * 2) + 1, tlNull);
	}

	return CherryCount;

}

u8 DrawMrDoMap(void)
{
	//Take the "Map" array thing and try and paint a Mr Do! type screen thing
	//from it.

	u8 CherryCount;
   	u8 x, y;

	SetPalette(SCR_2_PLANE, 0, RGB(0,0,0), RGB(0,15,0), RGB(15,0,0), RGB(0,0,15));
  	ClearScreen(SCR_2_PLANE);

	CherryCount=0;

   	for (y = 0; y < MAX_Y; y++)
	{
	      	for (x = 0; x < MAX_X; x++)
      		{
				//Only works for MrDoMap[y][x]=1 ?
      	   		DrawMrDoBackGround(x, y);
				CherryCount+=CreateCherries(x, y);
				CreateApples(x, y);

      		}
   	}

	//Draw Beasties Spawn point...
	//Dang. I was hoping to be able to use the normal beastie tiles for this
	//and to somehow palette shift to get the "right" result, but it doesn't
	//look right. Back to the tile editor...

	//This needs to be farmed into the "drawbackground tile" thingumybob.

	PutTile(SCR_1_PLANE, 4, 8, 8, tlBeastie+tlCharTopLeft);
	PutTile(SCR_1_PLANE, 4, 9, 8, tlBeastie+tlCharTopRight);
	PutTile(SCR_1_PLANE, 4, 8, 9, tlBeastie+tlCharBottomLeft);
	PutTile(SCR_1_PLANE, 4, 9, 9, tlBeastie+tlCharBottomRight);

	return CherryCount;

}

u8 SpriteCollision(u8 SourceX, u8 SourceY, u8 TargetX, u8 TargetY)
{

	//Function to try and guess whether two sprites are
	//in collision. Could be sort of handy.
	//Will return 0x1 if they overlap. Can't be bothered
	//doing anything too complex, for now I think I can
	//get away with a basic boundary check

	//trouble is, it's a  bit too tight. Collisions happen
	//way before they should...

	u8 bReturn;

	bReturn=0x1;

	//Check the x co-ords
	if (SourceX<TargetX)
	{
		//Something aintent right here...
		//Source sprite is to the left of the target
		if (!((SourceX+16>TargetX+8) && (SourceX+16<=TargetX+16)))
		{
			//Sprites are in the same vertical column
			bReturn=0x0;
		}
	}
	else
	{
		//Source sprite is to the right of the target
		if (!(TargetX+16>SourceX+8 && TargetX+16<=SourceX+16))
		{
			//Sprites are in the same vertical column
			bReturn=0x0;
		}
	}
	//Check the y co-ords
	if (bReturn==0x1)
	{
		//Only check if the sprites are in the same vertical column
		if (SourceY<TargetY)
		{
			//Source sprite is above the target
			if (!(SourceY+16>TargetY+8 && SourceY+16<=TargetY+16))
			{
				//Sprites are not in the same row
				bReturn=0x0;
			}
		}
		else
		{
			//Source sprite is below the target
			if (!(TargetY+16>SourceY+8 && TargetY+16<=SourceY+16))
			{
				//Sprites are not in the same row
				bReturn=0x0;
			}
		}
	}
	return bReturn;
}
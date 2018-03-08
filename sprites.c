// Exported by NeoTile By Steve Robb
// Graphics by Darek Davis


// current tile set
//const unsigned short TileSet[5][8] =
//{
//	{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // Tile_0 - blank used for background
//	{0x003f, 0x00d5, 0x0355, 0x0355, 0x0365, 0x0369, 0x037e, 0x0fee}, // first frame of sprite animation
//	{0xfc00, 0x5700, 0x55c0, 0x5740, 0x55c0, 0x9540, 0xfdc0, 0xb9f0},
//	{0x35fa, 0x0f5f, 0x3a57, 0x3ad5, 0x0fea, 0x00d5, 0x00d5, 0x0035},
//	{0xaf5c, 0xf6b0, 0xd6b0, 0x5fc0, 0x6bc0, 0xd7c0, 0xff00, 0xffc0},
//};

// sprite animation animations
//Order is Down, Up, Left, Right
//Order in my file is Right, Left, Up, Down...
//Typical..

const unsigned char ANIM_OFFSET_MRDO = 0;
const unsigned char ANIM_OFFSET_MRDODIGGING = 32;
const unsigned char ANIM_OFFSET_BEASTIE = 64;

const unsigned short Sprite_Tiles[96][8] =
{
{0x300, 0x80, 0x280, 0xA60, 0x26AF, 0x2A6F, 0x2AB7, 0x2655},
{0x0, 0x0, 0x0, 0xA80, 0x2A60, 0xE6A8, 0x6A98, 0x69A8},
{0x2A41, 0xA55, 0x14, 0x0, 0x0, 0x0, 0x0, 0x0},
{0xAAAC, 0x298C, 0xA0C, 0xA0C, 0xA00, 0x0, 0x0, 0x0},
{0x300, 0x2000, 0x2800, 0x2A80, 0x26AF, 0x2A6F, 0x26B7, 0xA55},
{0x0, 0x0, 0x2AC, 0x29AC, 0xAAAC, 0xE66C, 0x6A8C, 0x6A80},
{0xA41, 0x255, 0x14, 0x0, 0x0, 0x0, 0x0, 0x0},
{0xA668, 0x28A8, 0x282C, 0x280C, 0x280C, 0xC, 0x0, 0x0},
{0x0, 0x0, 0x0, 0x0, 0x0, 0x14, 0xA55, 0x2A41},
{0x0, 0x0, 0x0, 0xA00, 0xA0C, 0xA0C, 0x298C, 0xAAAC},
{0x2655, 0x2AB7, 0x2A6F, 0x26AF, 0xA60, 0x280, 0x80, 0x300},
{0x69A8, 0x6A98, 0xE6A8, 0x2A60, 0xA80, 0x0, 0x0, 0x0},
{0x0, 0x0, 0x0, 0x0, 0x0, 0x14, 0x255, 0xA41},
{0x0, 0x0, 0xC, 0x280C, 0x280C, 0x282C, 0x28A8, 0xA668},
{0xA55, 0x26B7, 0x2A6F, 0x26AF, 0x2A80, 0x2800, 0x2000, 0x300},
{0x6A80, 0x6A8C, 0xE66C, 0xAAAC, 0x29AC, 0x2AC, 0x0, 0x0},
{0x0, 0x2, 0xA, 0xA, 0x5, 0x14, 0x14, 0x5},
{0x0, 0xAA00, 0x6980, 0xAAA3, 0x6668, 0x7A80, 0x5F00, 0x7F00},
{0x2, 0xA, 0x2AA, 0x2A6, 0xA, 0x2, 0xFF, 0x0},
{0x5C00, 0xAA00, 0xA680, 0x6A80, 0xA980, 0x9A00, 0xA800, 0x0},
{0x0, 0x0, 0x2, 0xA, 0x5, 0x14, 0x14, 0x5},
{0x0, 0x2AA8, 0x99A0, 0xAA83, 0x6680, 0x7A00, 0x5F00, 0x7F00},
{0x2, 0x2AA, 0x2A9, 0x2, 0x9, 0x29, 0xFFA, 0x0},
{0x5B00, 0xAA80, 0xA680, 0xAA60, 0xA6A0, 0xAA0, 0x3FF0, 0x0},
{0x0, 0xAA, 0x269, 0xCAAA, 0x2999, 0x2AD, 0xF5, 0xFD},
{0x0, 0x8000, 0xA000, 0xA000, 0x5000, 0x1400, 0x1400, 0x5000},
{0x35, 0xAA, 0x29A, 0x2A9, 0x26A, 0xA6, 0x2A, 0x0},
{0x8000, 0xA000, 0xAA80, 0x9A80, 0xA000, 0x8000, 0xFF00, 0x0},
{0x0, 0x2AA8, 0xA66, 0xC2AA, 0x299, 0xAD, 0xF5, 0xFD},
{0x0, 0x0, 0x8000, 0xA000, 0x5000, 0x1400, 0x1400, 0x5000},
{0xB5, 0x2AA, 0x29A, 0x9AA, 0xA9A, 0xAA0, 0xFFC, 0x0},
{0x8000, 0xAA80, 0x6A80, 0x8000, 0x6000, 0xA800, 0xAFF0, 0x0},
{0x300, 0x80, 0x280, 0xA60, 0x26AF, 0x2A6F, 0x2AB7, 0x2655},
{0x0, 0x0, 0x2A80, 0xA66C, 0xAAAC, 0xE66C, 0x6A8C, 0x660C},
{0x2A41, 0xA55, 0x16, 0x2, 0x5, 0x4, 0x0, 0x0},
{0xAA00, 0x8940, 0x8050, 0x8000, 0x4000, 0x0, 0x0, 0x0},
{0x0, 0xC, 0x20, 0xA0, 0x290, 0x9AF, 0x2AAF, 0x29B7},
{0x3FC, 0x3C0, 0x280, 0xA80, 0x2980, 0xAA80, 0xA9A0, 0xAAA8},
{0x2A55, 0x2641, 0xA55, 0x16, 0x2A, 0x158, 0x150, 0x100},
{0x6998, 0x9A2C, 0xA80C, 0xA00C, 0xC, 0x0, 0x0, 0x0},
{0x0, 0x0, 0x4, 0x5, 0x2, 0x16, 0xA55, 0x2A41},
{0x0, 0x0, 0x0, 0x4000, 0x8000, 0x8050, 0x8940, 0xAA00},
{0x2655, 0x2AB7, 0x2A6F, 0x26AF, 0xA60, 0x280, 0x80, 0x300},
{0x660C, 0x6A8C, 0xE66C, 0xAAAC, 0xA66C, 0x2A80, 0x0, 0x0},
{0x100, 0x150, 0x158, 0x2A, 0x16, 0xA55, 0x2641, 0x2A55},
{0x0, 0x0, 0x0, 0xC, 0xA00C, 0xA80C, 0x9A2C, 0x6998},
{0x29B7, 0x2AAF, 0x9AF, 0x290, 0xA0, 0x20, 0xC, 0x0},
{0xAAA8, 0xA9A0, 0xAA80, 0x2980, 0xA80, 0x280, 0x3C0, 0x3FC},
{0x0, 0x2, 0xA, 0xA, 0x5, 0x14, 0x514, 0x1A5},
{0x0, 0xAA00, 0x6980, 0xAAA3, 0x6668, 0x7A80, 0x5F00, 0x7F00},
{0x1AA, 0x2, 0xA, 0x6, 0x14, 0x10, 0x0, 0x0},
{0x5E80, 0xAAA0, 0x6660, 0xAAA0, 0x2660, 0xA80, 0xFFC0, 0x0},
{0x0, 0xA, 0x26, 0x542A, 0x1415, 0x1651, 0xA51, 0x295},
{0x0, 0xA000, 0xA800, 0x6600, 0xAA80, 0xE9A0, 0x7C0C, 0xFC00},
{0xA9, 0xA6, 0x2A, 0x9, 0x2, 0x9, 0x3FE, 0x0},
{0xA800, 0xAA00, 0xAA80, 0x99AF, 0xAAAF, 0xA003, 0x8003, 0x0},
{0x0, 0xAA, 0x269, 0xCAAA, 0x2999, 0x2AD, 0xF5, 0xFD},
{0x0, 0x8000, 0xA000, 0xA000, 0x5000, 0x1400, 0x1450, 0x5A40},
{0x2B5, 0xAAA, 0x999, 0xAAA, 0x998, 0x2A0, 0x3FF, 0x0},
{0xAA40, 0x8000, 0xA000, 0x9000, 0x1400, 0x400, 0x0, 0x0},
{0x0, 0xA, 0x2A, 0x99, 0x2AA, 0xA6B, 0x303D, 0x3F},
{0x0, 0xA000, 0x9800, 0xA815, 0x5414, 0x4594, 0x45A0, 0x5680},
{0x2A, 0xAA, 0x2AA, 0xFA66, 0xFAAA, 0xC00A, 0xC002, 0x0},
{0x6A00, 0x9A00, 0xA800, 0x6000, 0x8000, 0x6000, 0xBFC0, 0x0},
{0x0, 0x0, 0x155, 0x5A9, 0x6AA, 0x6AA, 0x6AA, 0x656}, //Beastie Down
{0x0, 0x200, 0xA90, 0xAA90, 0x6A90, 0xAA90, 0xBE90, 0xBE80},
{0x642, 0x642, 0x6AA, 0x5AA, 0x169, 0x55, 0x0, 0x0},
{0xBE90, 0xBE90, 0x7E90, 0x7E10, 0x7C10, 0x3C00, 0xC00, 0x0},
{0x0, 0x0, 0x56, 0x1569, 0x1AAA, 0x1AAA, 0x1AAA, 0x195A}, //Down F2
{0x200, 0xAA40, 0xAA40, 0xAA40, 0xAA40, 0xBF40, 0xBFC0, 0xABC0},
{0x181A, 0x181A, 0x1AA9, 0x16A5, 0x554, 0x0, 0x0, 0x0},
{0xABC0, 0xABC0, 0xA3C0, 0x3C0, 0xC0, 0x0, 0x0, 0x0},
{0x0, 0x0, 0x55, 0x169, 0x5AA, 0x6AA, 0x642, 0x642}, //Up
{0x0, 0xC00, 0x3C00, 0x7C10, 0x7E10, 0x7E90, 0xBE90, 0xBE90},
{0x656, 0x6AA, 0x6AA, 0x6AA, 0x5A9, 0x155, 0x0, 0x0},
{0xBE80, 0xBE90, 0xAA90, 0x6A90, 0xAA90, 0xA90, 0x200, 0x0},
{0x0, 0x0, 0x0, 0x554, 0x16A5, 0x1AA9, 0x181A, 0x181A}, //Up F2
{0x0, 0x0, 0x0, 0xC0, 0x3C0, 0xA3C0, 0xABC0, 0xABC0},
{0x195A, 0x1AAA, 0x1AAA, 0x1AAA, 0x1569, 0x56, 0x0, 0x0},
{0xABC0, 0xBFC0, 0xBF40, 0xAA40, 0xAA40, 0xAA40, 0xAA40, 0x200},
{0x0, 0x0, 0x55, 0x16A, 0x5A5, 0x6A0, 0x6A0, 0x5AA}, //Left
{0x0, 0x0, 0x5540, 0xAA50, 0x6A90, 0x6A90, 0x6A90, 0xAA50},
{0x15A, 0xFFF, 0x3FFF, 0xAA, 0x2A, 0x155, 0x0, 0x0},
{0xA980, 0xFA80, 0xFAA0, 0xAAA8, 0xAAA0, 0x1550, 0x0, 0x0},
{0x0, 0x55, 0x16A, 0x1A0, 0x1A0, 0x1A5, 0x16A, 0x5A}, //Left F2
{0x0, 0x5540, 0xAA40, 0x6A40, 0x6A50, 0x6A90, 0xAA90, 0xAA60},
{0x2A, 0x2A, 0xA, 0xFF, 0x3FF, 0x0, 0x0, 0x0},
{0xAAA8, 0xBEA8, 0xBEA8, 0xFEAA, 0xF554, 0x0, 0x0, 0x0},
{0x0, 0x0, 0x155, 0x5AA, 0x6A9, 0x6A9, 0x6A9, 0x5AA}, //Right
{0x0, 0x0, 0x5500, 0xA940, 0x5A50, 0xA90, 0xA90, 0xAA50},
{0x26A, 0x2AF, 0xAAF, 0x2AAA, 0xAAA, 0x554, 0x0, 0x0},
{0xA540, 0xFFF0, 0xFFFC, 0xAA00, 0xA800, 0x5540, 0x0, 0x0},
{0x0, 0x155, 0x1AA, 0x1A9, 0x5A9, 0x6A9, 0x6AA, 0x9AA}, //Right F2
{0x0, 0x5500, 0xA940, 0xA40, 0xA40, 0x5A40, 0xA940, 0xA500},
{0x2AAA, 0x2ABE, 0x2ABE, 0xAABF, 0x155F, 0x0, 0x0, 0x0},
{0xA800, 0xA800, 0xA000, 0xFF00, 0xFFC0, 0x0, 0x0, 0x0}
};

const unsigned short Static_Sprite_Tiles[32][8] =
{
	{0x0005, 0x0015, 0x2AA, 0x399A, 0x3AAA, 0x399B, 0x32A9, 0x3099}, //Mr Do Down Frame 1
	{0x40C0, 0x5200, 0x5280, 0x59A0, 0xFA98, 0xF9A8, 0xDEA8, 0x5598},
	{0xAA, 0x0162, 0x0502, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000},
	{0x41A8, 0x55A0, 0x9400, 0x8000, 0x5000, 0x1000, 0x0000, 0x0000},
	{0x0005, 0x0015, 0x2AA, 0x399A, 0x3AAA, 0x399B, 0x32A9, 0x3099}, //Mr Do Down Frame 2
	{0x40C0, 0x5200, 0x5280, 0x59A0, 0xFA98, 0xF9A8, 0xDEA8, 0x5598},
	{0xAA, 0x0162, 0x0502, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000},
	{0x41A8, 0x55A0, 0x9400, 0x8000, 0x5000, 0x1000, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0004, 0x0005, 0x0002, 0x0016, 0xA55, 0x2A41}, //Mr Do Up Frame 1
	{0x0000, 0x0000, 0x0000, 0x4000, 0x8000, 0x8050, 0x8940, 0xAA00},
	{0x2655, 0x2AB7, 0x2A6F, 0x26AF, 0xA65, 0x0285, 0x0085, 0x0301},
	{0x660C, 0x6A8C, 0xE66C, 0xAAAC, 0xA66C, 0xAA80, 0x5400, 0x5000},
	{0x0000, 0x0000, 0x0004, 0x0005, 0x0002, 0x0016, 0xA55, 0x2A41}, //Mr Do Up Frame 2
	{0x0000, 0x0000, 0x0000, 0x4000, 0x8000, 0x8050, 0x8940, 0xAA00},
	{0x2655, 0x2AB7, 0x2A6F, 0x26AF, 0xA65, 0x0285, 0x0085, 0x0301},
	{0x660C, 0x6A8C, 0xE66C, 0xAAAC, 0xA66C, 0xAA80, 0x5400, 0x5000},
	{0x0000, 0x0002, 0xA, 0xA, 0x0005, 0x0014, 0x0514, 0x1A5}, //Mr Do Left Frame 1
	{0x0000, 0xAA00, 0x6980, 0xAAA3, 0x6668, 0x7A80, 0x5F54, 0x7F55},
	{0x1AA, 0x0002, 0xA, 0x0006, 0x0014, 0x0010, 0x0000, 0x0000},
	{0x500, 0xAAA5, 0x6664, 0xAAA0, 0x2660, 0xA80, 0xFFC0, 0x0000},
	{0x0000, 0x0002, 0xA, 0xA, 0x0005, 0x0014, 0x0514, 0x1A5}, //Mr Do Left Frame 2
	{0x0000, 0xAA00, 0x6980, 0xAAA3, 0x6668, 0x7A80, 0x5F54, 0x7F55},
	{0x1AA, 0x0002, 0xA, 0x0006, 0x0014, 0x0010, 0x0000, 0x0000},
	{0x500, 0xAAA5, 0x6664, 0xAAA0, 0x2660, 0xA80, 0xFFC0, 0x0000},
	{0x0000, 0xAA, 0x0269, 0xCAAA, 0x2999, 0x2AD, 0xF5, 0xFD},
	{0x0000, 0x8000, 0xA000, 0xA000, 0x5000, 0x1400, 0x1400, 0x5000},
	{0x0035, 0xAA, 0x29A, 0x2A9, 0x26A, 0xA6, 0x0000, 0x0000},
	{0x8054, 0xA155, 0xAA95, 0x9A95, 0xA054, 0x8000, 0xFF00, 0x0000},
	{0x0000, 0x2AA8, 0xA66, 0xC2AA, 0x0299, 0xAD, 0xF5, 0xFD},
	{0x0000, 0x0000, 0x8000, 0xA000, 0x5000, 0x1400, 0x1400, 0x5054},
	{0xB5, 0x2AA, 0x29A, 0x9AA, 0xA9A, 0xAA0, 0xFFC, 0x0000},
	{0x8155, 0xAA95, 0x6A95, 0x8054, 0x6000, 0xA800, 0xAFF0, 0x0000}
};

const unsigned short Not_Sprite_Tiles[32][8] =
{
	{0x003f, 0x00d5, 0x0355, 0x0355, 0x0365, 0x0369, 0x037e, 0x0fee},
	{0xfc00, 0x5700, 0x55c0, 0x5740, 0x55c0, 0x9540, 0xfdc0, 0xb9f0},
	{0x35fa, 0x0f5f, 0x3a57, 0x3ad5, 0x0fea, 0x00d5, 0x00d5, 0x0035},
	{0xaf5c, 0xf6b0, 0xd6b0, 0x5fc0, 0x6bc0, 0xd7c0, 0xff00, 0xffc0},
	{0x003f, 0x00d5, 0x0355, 0x0355, 0x0365, 0x0369, 0x037e, 0x0fee},
	{0xfc00, 0x5700, 0x55c0, 0x5740, 0x55c0, 0x9540, 0xfdc0, 0xb9f0},
	{0x35fa, 0x0e9f, 0x0e97, 0x03f5, 0x03e9, 0x03d7, 0x00ff, 0x03ff},
	{0xaf5c, 0xf5f0, 0xd5ac, 0x57ac, 0xabf0, 0x5700, 0x5700, 0x5c00},
	{0x003f, 0x00d5, 0x0355, 0x0355, 0x03d5, 0x0355, 0x0375, 0x0fd5},
	{0xfc00, 0x5700, 0x55c0, 0x5770, 0x55f0, 0x5570, 0x55c0, 0x55f0},
	{0x3575, 0x0edf, 0x0ed5, 0x03f5, 0x03eb, 0x03d7, 0x00ff, 0x03ff},
	{0x7d5c, 0xd5b0, 0x56ac, 0x57ac, 0xabf0, 0x5700, 0x5700, 0x5f00},
	{0x003f, 0x00d5, 0x0355, 0x0355, 0x03d5, 0x0355, 0x0375, 0x0fd5},
	{0xfc00, 0x5700, 0x55c0, 0x5770, 0x55f0, 0x5570, 0x55c0, 0x55f0},
	{0x357d, 0x0e57, 0x3a95, 0x3ad5, 0x0fea, 0x00d5, 0x00d5, 0x00f5},
	{0x5d5c, 0xf7b0, 0x57b0, 0x5fc0, 0xebc0, 0xd7c0, 0xff00, 0xffc0},
	{0x003f, 0x03d5, 0x0d55, 0x0d55, 0x0d95, 0x03a5, 0x03fe, 0x03ba},
	{0xfc00, 0x5f00, 0x55c0, 0x5570, 0x55c0, 0x5570, 0x55c0, 0x5f00},
	{0x03a9, 0x00f7, 0x00d5, 0x00d7, 0x00d5, 0x003e, 0x000d, 0x0035},
	{0x75c0, 0xdfc0, 0xf5c0, 0xa5c0, 0xa7c0, 0xff00, 0x5c00, 0x5c00},
	{0x0000, 0x003f, 0x03d5, 0x0d55, 0x0d55, 0x0d95, 0x03a5, 0x03fe},
	{0x0000, 0xfc00, 0x5f00, 0x55c0, 0x5570, 0x55c0, 0x5570, 0x55c0},
	{0x03ba, 0x03a9, 0x00ff, 0x00f5, 0x00d7, 0x03e9, 0x0d56, 0x0f5f},
	{0x5f00, 0x75c0, 0xd570, 0x7df0, 0xf6b0, 0x5eb0, 0x6570, 0xf5fc},
	{0x003f, 0x00f5, 0x0355, 0x0d55, 0x0355, 0x0d55, 0x0355, 0x00f5},
	{0xfc00, 0x57c0, 0x5570, 0x5570, 0x5670, 0x5ac0, 0xbfc0, 0xaec0},
	{0x035d, 0x03f7, 0x035f, 0x035a, 0x03da, 0x00ff, 0x0035, 0x0035},
	{0x6ac0, 0xdf00, 0x5700, 0xd700, 0x5700, 0xbc00, 0x7000, 0x5c00},
	{0x0000, 0x003f, 0x00f5, 0x0355, 0x0d55, 0x0355, 0x0d55, 0x0355},
	{0x0000, 0xfc00, 0x57c0, 0x5570, 0x5570, 0x5670, 0x5ac0, 0xbfc0},
	{0x00f5, 0x035d, 0x0d57, 0x0f7d, 0x0e9f, 0x0eb5, 0x0d59, 0x3f5f},
	{0xaec0, 0x6ac0, 0xff00, 0x5f00, 0xd700, 0x6bc0, 0x9570, 0xf5f0}
};
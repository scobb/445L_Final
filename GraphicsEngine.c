#include "stdint.h"
#include "ST7735.h"
#include "ActiveState.h"
#include "PauseState.h"
#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "Heartbeat.h"
#include "ScoreEngine.h"
#include "StartState.h"

#define BOARD_START_X 8
#define BOARD_END_Y 148
#define BOARD_START_Y BOARD_END_Y - BOARD_SIZE_UD * SQUARE_HEIGHT
#define BOARD_END_X BOARD_START_X + BOARD_SIZE_LR * SQUARE_WIDTH
#define BOARD_SIZE_LR_PIX 128
#define BOARD_SIZE_UD_PIX 128
#define SQUARE_HEIGHT 6
#define SQUARE_WIDTH 6
#define FILL_RECT_OFFSET_X -2
#define FILL_RECT_OFFSET_Y 2
#define FILL_RECT_MOTION_OFFSET_X -1
#define FILL_RECT_MOTION_OFFSET_Y -1
#define DRAW_BITMAP_OFFSET_X -2
#define DRAW_BITMAP_OFFSET_Y 6
extern coord directions[4];
const unsigned short pacman_r[] = {
 0x0000, 0x0208, 0x0471, 0x02EB, 0x0000, 0x0228, 0x07FF, 0x075D, 0x07FF, 0x0228, 0x0471, 0x075D, 0x0679, 0x0000, 0x0000, 0x0124,
 0x07FF, 0x077D, 0x07FF, 0x01C7, 0x0000, 0x00A2, 0x030C, 0x0165, 0x0000,
};

const unsigned short pacman_l[] = {
 0x0000, 0x03AE, 0x0575, 0x0228, 0x0000, 0x034D, 0x06FB, 0x075D, 0x079E, 0x0269, 0x0000, 0x0020, 0x04F3, 0x075D, 0x0555, 0x034D,
 0x06FB, 0x075D, 0x079E, 0x0269, 0x0000, 0x03AE, 0x0575, 0x0228, 0x0000,
};

const unsigned short pacman_d[] = {
 0x0000, 0x01C7, 0x0000, 0x01C7, 0x0000, 0x02AA, 0x06FB, 0x0000, 0x06FB, 0x02AA, 0x0575, 0x075D, 0x0430, 0x075D, 0x0575, 0x028A,
 0x079E, 0x077D, 0x079E, 0x028A, 0x0000, 0x028A, 0x0575, 0x028A, 0x0000,
};

const unsigned short pacman_u[] = {
 0x0000, 0x0186, 0x03CF, 0x0082, 0x0000, 0x0249, 0x07BE, 0x079E, 0x07BE, 0x0082, 0x05B6, 0x073C, 0x0430, 0x079E, 0x03CF, 0x028A,
 0x0679, 0x0000, 0x07DF, 0x00A2, 0x0000, 0x0082, 0x0000, 0x0082, 0x0000,
};

const unsigned short pacman_c[] = {
 0x0000, 0x028A, 0x0575, 0x028A, 0x0000, 0x028A, 0x079E, 0x071C, 0x079E, 0x028A, 0x0575, 0x071C, 0x06DB, 0x071C, 0x0575, 0x028A,
 0x079E, 0x071C, 0x079E, 0x028A, 0x0000, 0x028A, 0x0575, 0x028A, 0x0000,
};

const unsigned short apple[] = {
 0x0002, 0x0016, 0x0007, 0x0015, 0x0000, 0x0015, 0x001C, 0x001D, 0x001D, 0x000E, 0x631C, 0x001B, 0x001C, 0x001C, 0x0015, 0x0816,
 0x39DC, 0x0156, 0x001F, 0x000E, 0x0140, 0x0620, 0x0400, 0x0007, 0x0000,
};

const unsigned short cherry[] = {
 0x000A, 0x0016, 0x0001, 0x0016, 0x000A, 0x2976, 0x001F, 0x000D, 0x39BF, 0x0016, 0x0000, 0x0199, 0x0000, 0x0116, 0x0000, 0x0000,
 0x0000, 0x01D6, 0x014F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0192, 0x0043,
};

const unsigned short blue_ghost[] = {
 0x90A1, 0xC8E8, 0xC0C8, 0xC8E7, 0x90A2, 0xC0C2, 0xF929, 0xF92C, 0xF92A, 0xC0C2, 0xC0C1, 0xF93B, 0xF922, 0xF93B, 0xC0C1, 0x8081,
 0xF927, 0xF923, 0xF927, 0x8081,
};

const unsigned short orange_ghost[] = {
 0x014F, 0x01B5, 0x01B4, 0x01B4, 0x01B4, 0x01B4, 0x0ABD, 0x023C, 0x0A9C, 0x025C, 0x3B15, 0xF6B3, 0x027C, 0xD614, 0x84FB, 0x010D,
 0x12FD, 0x023B, 0x12DC, 0x01D4,
};

const unsigned short light_blue_ghost[] = {
 0x7BE0, 0xAD60, 0xAD40, 0xAD60, 0x7BE0, 0xA520, 0xEF45, 0xE700, 0xEF45, 0xA520, 0xB589, 0xAD5F, 0xEF40, 0xAD5F, 0xB589, 0x4A60,
 0xBDC0, 0xAD40, 0xBDC0, 0x4A60,
};

const unsigned short purple_ghost[] = {
 0x9092, 0xC8D9, 0xC0D8, 0xC8D9, 0x9092, 0xC0F8, 0xFA5F, 0xF8FF, 0xFA5F, 0xC0F8, 0xC2D8, 0x9F73, 0xF8DF, 0x9F73, 0xC2D8, 0x582B,
 0xD85B, 0xC0D8, 0xD85B, 0x582B,
};

const unsigned short r_ghost[] = {
 0x0008, 0x0011, 0x0010, 0x0011, 0x0008, 0x000F, 0x213E, 0x001C, 0x213E, 0x000F, 0x18D0, 0xEF57, 0x003D, 0xEF57, 0x10B0, 0x0003,
 0x0012, 0x0010, 0x0012, 0x0003,
};

const unsigned short ghost_eyes[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x2945, 0x0000, 0x2945, 0x0020, 0x4A49, 0xFFDF, 0x0000, 0xFFDF, 0x4A49, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000,
};

const unsigned short pacman_title[] = {
 0x5E7B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x5E5B, 0x5E5B, 0x6F9F, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0021,
 0x0021, 0x0000, 0x671E, 0x5E5B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B,
 0x565B, 0x565B, 0x565B, 0x6E9D, 0x332D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2AEC, 0x2AEC, 0x669C, 0x565B, 0x5EBC, 0x33B0,
 0x33B0, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x675F, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B,
 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x5E5B, 0x569B, 0x5E5B, 0x5E5B, 0x5E5B, 0x565B,
 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x5E5B, 0x5E5B, 0x5E5B, 0x569B, 0x5E5B, 0x565B,
 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x565B, 0x5EDD,
 0x561A, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x5F1E, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0000, 0x430D, 0x56DD, 0x55F9, 0x55F9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9,
 0x4DF9, 0x4DF9, 0x4DF9, 0x5EDD, 0x19A6, 0x0000, 0x1924, 0x1924, 0x230D, 0x33D1, 0x56BC, 0x56BC, 0x4DF9, 0x4DF9, 0x4E19, 0x565B,
 0x565B, 0x4597, 0x232E, 0x1986, 0x08A2, 0x08A2, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9,
 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4E7B, 0x3B2D, 0x5EFD, 0x4DF9, 0x4DF9, 0x4DF9,
 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x5EFD, 0x32EC, 0x4E5B, 0x4DF9,
 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x565B,
 0x563A, 0x4E19, 0x4E19, 0x4DFA, 0x4E1A, 0x4E1A, 0x55F9, 0x55F9, 0x5F1E, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0000, 0x2390, 0x4E5B, 0x4E5B, 0x5619, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A,
 0x4DF9, 0x5EBC, 0x5EBC, 0x338F, 0x0000, 0x0000, 0x2AED, 0x2AED, 0x5E9C, 0x565B, 0x4E19, 0x4E19, 0x4E19, 0x4DFA, 0x5619, 0x4E19,
 0x4E19, 0x563A, 0x5EFD, 0x64B3, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x4DF9, 0x4DF9, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A,
 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DF9, 0x4DF9, 0x5E9C, 0x00C3, 0x2B6E, 0x4E9C, 0x4E9C, 0x4E19,
 0x4DFA, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DFA, 0x4E19, 0x4E9B, 0x4E9B, 0x33B0, 0x0001, 0x56BC, 0x4DF9,
 0x4DF9, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E19, 0x4E19, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DF9, 0x4DF9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x4DFA, 0x4E1A, 0x4E1A, 0x55F9, 0x55F9, 0x5F1E, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0000, 0x2390, 0x4E5B, 0x4E5B, 0x5619, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A,
 0x4DF9, 0x5EBC, 0x5EBC, 0x338F, 0x0000, 0x0000, 0x2AED, 0x2AED, 0x5E9C, 0x565B, 0x4E19, 0x4E19, 0x4E19, 0x4DFA, 0x5619, 0x4E19,
 0x4E19, 0x563A, 0x5EFD, 0x64B3, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x4DF9, 0x4DF9, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A,
 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DF9, 0x4DF9, 0x5E9C, 0x00C3, 0x2B6E, 0x4E9C, 0x4E9C, 0x4E19,
 0x4DFA, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DFA, 0x4E19, 0x4E9B, 0x4E9B, 0x33B0, 0x0001, 0x56BC, 0x4DF9,
 0x4DF9, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E19, 0x4E19, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DF9, 0x4DF9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x673E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0000, 0x11A8, 0x4DF9, 0x4DF9, 0x561A, 0x5619, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9,
 0x563A, 0x4537, 0x4537, 0x08A3, 0x0000, 0x21C7, 0x4E9C, 0x4E9C, 0x561A, 0x4E19, 0x55FA, 0x55FA, 0x55FA, 0x5619, 0x55FA, 0x561A,
 0x561A, 0x567B, 0x1AED, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A,
 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x5E9C, 0x0104, 0x00A3, 0x3D77, 0x3D77, 0x563A,
 0x55F9, 0x55FA, 0x55FA, 0x561A, 0x561A, 0x561A, 0x55FA, 0x55FA, 0x55F9, 0x563A, 0x4598, 0x4598, 0x00E5, 0x0041, 0x56BC, 0x55F9,
 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x55F9, 0x55F9, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x561A, 0x55FA, 0x4DF9, 0x4DF9, 0x5EDC, 0x1187, 0x11E8, 0x11E8, 0x19E8, 0x19C7, 0x0000, 0x0000,
 0x0000, 0x0020, 0x0000, 0x08C2, 0x4D15, 0x4D15, 0x563B, 0x55F9, 0x55FA, 0x55FA, 0x561A, 0x561A, 0x55FA, 0x55FA, 0x55FA, 0x561A,
 0x4E3A, 0x1ACB, 0x1ACB, 0x0000, 0x2104, 0x773E, 0x4E1A, 0x4E1A, 0x55F9, 0x55FA, 0x561A, 0x561A, 0x561A, 0x55F9, 0x561A, 0x66BC,
 0x66BC, 0x230D, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A,
 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x569C, 0x0945, 0x0000, 0x2B8F, 0x2B8F, 0x4E5B,
 0x4E1A, 0x55F9, 0x55F9, 0x561A, 0x563A, 0x561A, 0x55F9, 0x55F9, 0x4E1A, 0x563A, 0x33F0, 0x33F0, 0x0000, 0x0062, 0x5EBC, 0x55F9,
 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55FA, 0x561A, 0x5E5B, 0x5E5B, 0x563A, 0x55F9, 0x55FA, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x561A, 0x561A, 0x55FA, 0x55FA, 0x4DF9, 0x5EBC, 0x56BD, 0x56BD, 0x5EBC, 0x5E7B, 0x1AAC, 0x0904,
 0x0904, 0x0061, 0x0800, 0x0000, 0x330C, 0x330C, 0x5E3B, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DF9, 0x4D15, 0x5E9C, 0x5619, 0x5619, 0x563A,
 0x4D57, 0x08A3, 0x08A3, 0x0000, 0x338E, 0x565B, 0x5619, 0x5619, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x4E3A, 0x3C11,
 0x3C11, 0x0861, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0061, 0x0945, 0x0945, 0x0925,
 0x0925, 0x1145, 0x1145, 0x0061, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A,
 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x569C, 0x0945, 0x0800, 0x19C7, 0x19C7, 0x55F9,
 0x4E1A, 0x561A, 0x561A, 0x5E7B, 0x54F5, 0x565B, 0x4E1A, 0x4E1A, 0x4E1A, 0x5619, 0x2249, 0x2249, 0x0800, 0x0882, 0x5EBC, 0x55F9,
 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x5619, 0x4E1A, 0x2BD0, 0x2BD0, 0x4515, 0x563A, 0x4E1A, 0x55F9, 0x55F9, 0x565B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x561A, 0x561A, 0x561A, 0x561A, 0x55FA, 0x55F9, 0x55F9, 0x55F9, 0x4E19, 0x4E3A, 0x4E5A, 0x5D98,
 0x5D98, 0x20E4, 0x0000, 0x0020, 0x0000, 0x0000, 0x4E7B, 0x563A, 0x5E9B, 0x5E9B, 0x1A49, 0x0020, 0x228B, 0x5E7B, 0x5E7B, 0x565B,
 0x5DD8, 0x0000, 0x0000, 0x0000, 0x332D, 0x565B, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x565A, 0x1A09, 0x0000,
 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x11C8, 0x561A, 0x561A, 0x4DD8,
 0x4DB8, 0x565A, 0x565A, 0x19C7, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x561A, 0x561A, 0x561A, 0x561A,
 0x561A, 0x563A, 0x563A, 0x561A, 0x55FA, 0x55FA, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x569C, 0x0945, 0x0000, 0x0000, 0x0000, 0x667A,
 0x5E3B, 0x5E9C, 0x5E9C, 0x22AB, 0x0000, 0x228B, 0x5EBC, 0x5EBC, 0x663B, 0x5EDB, 0x0000, 0x0000, 0x0000, 0x0082, 0x5EBC, 0x55F9,
 0x55F9, 0x561A, 0x561A, 0x55FA, 0x561A, 0x561A, 0x469C, 0x4D15, 0x0000, 0x0000, 0x3C52, 0x565A, 0x4E1A, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x4DFA, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x567B, 0x4D78, 0x563A, 0x569C,
 0x569C, 0x2BF1, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x4E7C, 0x5E7B, 0x5E7B, 0x228A, 0x0840, 0x230D, 0x569B, 0x569B, 0x4EBC,
 0x0000, 0x0020, 0x0020, 0x0000, 0x332D, 0x565B, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x563A, 0x1A6A, 0x0000,
 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x11A7, 0x4E1A, 0x4E1A, 0x4D98,
 0x4598, 0x5E1A, 0x5E1A, 0x19C7, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x55F9, 0x55FA, 0x4DF9, 0x4E3A,
 0x4E3A, 0x4D35, 0x4472, 0x4DF9, 0x563A, 0x563A, 0x55FA, 0x561A, 0x55F9, 0x55F9, 0x569C, 0x0945, 0x0000, 0x0020, 0x0020, 0x0000,
 0x465A, 0x5E9C, 0x5E9C, 0x230D, 0x0821, 0x22EC, 0x5E9C, 0x5E9C, 0x4E5A, 0x0000, 0x0020, 0x0020, 0x0000, 0x0082, 0x5EBC, 0x55F9,
 0x55F9, 0x561A, 0x55FA, 0x563A, 0x4DF9, 0x4DF9, 0x5CF4, 0x0000, 0x0000, 0x0000, 0x3C73, 0x565A, 0x4E1A, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x4DFA, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x55F9, 0x55F9, 0x567B, 0x4D78, 0x563A, 0x569C,
 0x569C, 0x2BF1, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x4E7C, 0x5E7B, 0x5E7B, 0x228A, 0x0840, 0x230D, 0x569B, 0x569B, 0x4EBC,
 0x0000, 0x0020, 0x0020, 0x0000, 0x332D, 0x565B, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x563A, 0x1A6A, 0x0000,
 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x11A7, 0x4E1A, 0x4E1A, 0x4D98,
 0x4598, 0x5E1A, 0x5E1A, 0x19C7, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x55F9, 0x55FA, 0x4DF9, 0x4E3A,
 0x4E3A, 0x4D35, 0x4472, 0x4DF9, 0x563A, 0x563A, 0x55FA, 0x561A, 0x55F9, 0x55F9, 0x569C, 0x0945, 0x0000, 0x0020, 0x0020, 0x0000,
 0x465A, 0x5E9C, 0x5E9C, 0x230D, 0x0821, 0x22EC, 0x5E9C, 0x5E9C, 0x4E5A, 0x0000, 0x0020, 0x0020, 0x0000, 0x0082, 0x5EBC, 0x55F9,
 0x55F9, 0x561A, 0x55FA, 0x563A, 0x4DF9, 0x4DF9, 0x5CF4, 0x0000, 0x0000, 0x0000, 0x3C73, 0x565A, 0x4E1A, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x4DFA, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DFA, 0x563A, 0x563A, 0x3CB4, 0x00A3, 0x2B8F, 0x4E9B,
 0x4E9B, 0x4E5B, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x3CD4, 0x465B, 0x465B, 0x4E19, 0x4D77, 0x5E9C, 0x56DD, 0x56DD, 0x01A7,
 0x1000, 0x0020, 0x0020, 0x0000, 0x338E, 0x565B, 0x4E19, 0x4E19, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x561A, 0x565A, 0x4432,
 0x4432, 0x0841, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0041, 0x00C3, 0x00C3, 0x00C3,
 0x00C3, 0x00E3, 0x00E3, 0x0061, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55F9, 0x55F9, 0x4E1A, 0x4E19, 0x569B, 0x4536,
 0x4536, 0x0062, 0x0000, 0x0229, 0x45D9, 0x45D9, 0x4E1A, 0x55FA, 0x4DF9, 0x4DF9, 0x569C, 0x0945, 0x0000, 0x0020, 0x0020, 0x0000,
 0x0A4A, 0x569C, 0x569C, 0x567B, 0x5577, 0x565B, 0x567B, 0x567B, 0x12CC, 0x0000, 0x0020, 0x0020, 0x0000, 0x0082, 0x5EBC, 0x4DF9,
 0x4DF9, 0x55FA, 0x4E1A, 0x4DF9, 0x0A6A, 0x0A6A, 0x0000, 0x0040, 0x0000, 0x0000, 0x3C73, 0x565A, 0x4E1A, 0x4DF9, 0x4DF9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x4DFA, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DF9, 0x569B, 0x569B, 0x11A7, 0x0000, 0x1104, 0x66BC,
 0x66BC, 0x4EFE, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x2249, 0x66BD, 0x66BD, 0x55FA, 0x4E1A, 0x4E3A, 0x3CD4, 0x3CD4, 0x0861,
 0x0000, 0x0020, 0x0020, 0x0000, 0x1144, 0x76FD, 0x4E1A, 0x4E1A, 0x4DFA, 0x4DFA, 0x4E1A, 0x4E1A, 0x4E1A, 0x4DFA, 0x4E1A, 0x5EBD,
 0x5EBD, 0x234E, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x4DF9, 0x4DF9, 0x4E19, 0x4E5B, 0x4D97, 0x08A2,
 0x08A2, 0x0000, 0x0800, 0x0800, 0x43CF, 0x43CF, 0x5EBC, 0x4E1A, 0x4DF9, 0x4DF9, 0x569C, 0x0945, 0x0000, 0x0020, 0x0020, 0x0020,
 0x10E4, 0x55D9, 0x55D9, 0x561A, 0x4E1A, 0x561A, 0x5E1A, 0x5E1A, 0x1146, 0x0000, 0x0020, 0x0020, 0x0000, 0x0082, 0x5EBC, 0x4DF9,
 0x4DF9, 0x4E1A, 0x5EBC, 0x5472, 0x0000, 0x0000, 0x0000, 0x0820, 0x0000, 0x0000, 0x3C73, 0x565A, 0x4E1A, 0x4DF9, 0x4DF9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x561A, 0x561A, 0x561A, 0x561A, 0x561A, 0x4DF9, 0x4E1A, 0x4E1A, 0x563B, 0x4410, 0x4D97, 0x567B,
 0x567B, 0x3494, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0800, 0x3C52, 0x3C52, 0x567B, 0x4DF9, 0x5E5B, 0x32ED, 0x32ED, 0x0000,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0800, 0x19E7, 0x5E9C, 0x5E9C, 0x561A, 0x5619, 0x55FA, 0x55FA, 0x561A, 0x561A, 0x55FA, 0x561A,
 0x561A, 0x4E5B, 0x33D0, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFD, 0x55FA, 0x55FA, 0x5E7B, 0x4D57, 0x0000, 0x0000,
 0x0000, 0x0020, 0x0020, 0x0000, 0x0800, 0x0800, 0x11E9, 0x567B, 0x561A, 0x561A, 0x569C, 0x0945, 0x0000, 0x0020, 0x0020, 0x0800,
 0x0040, 0x3BF0, 0x3BF0, 0x5E9C, 0x4DF9, 0x567C, 0x4432, 0x4432, 0x0040, 0x0800, 0x0020, 0x0020, 0x0000, 0x0082, 0x5EBC, 0x561A,
 0x561A, 0x569B, 0x1209, 0x0800, 0x0000, 0x0000, 0x0020, 0x0020, 0x0000, 0x0000, 0x3C73, 0x565A, 0x4E1A, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x4E1A, 0x4E5A, 0x567B, 0x665B,
 0x665B, 0x1AAA, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x1A08, 0x1A08, 0x4E3A, 0x563B, 0x5E19, 0x21A6, 0x21A6, 0x0000,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x2AAC, 0x2AAC, 0x669C, 0x4E5B, 0x55F9, 0x55F9, 0x561A, 0x55FA, 0x5619, 0x561A,
 0x561A, 0x563A, 0x5EDD, 0x5493, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFE, 0x561A, 0x561A, 0x55B8, 0x0000, 0x0000, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x09C8, 0x5E5B, 0x5E5B, 0x5EDD, 0x0945, 0x0000, 0x0020, 0x0020, 0x0020,
 0x0000, 0x21E8, 0x21E8, 0x5E5A, 0x4E5A, 0x5E5B, 0x2228, 0x2228, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x0882, 0x5EDD, 0x5E7B,
 0x5E7B, 0x1A4A, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x3C73, 0x565B, 0x4E1A, 0x55F9, 0x55F9, 0x567B,
 0x563A, 0x4E19, 0x4E19, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x55FA, 0x4E1A, 0x4E5A, 0x567B, 0x665B,
 0x665B, 0x1AAA, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x1A08, 0x1A08, 0x4E3A, 0x563B, 0x5E19, 0x21A6, 0x21A6, 0x0000,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x2AAC, 0x2AAC, 0x669C, 0x4E5B, 0x55F9, 0x55F9, 0x561A, 0x55FA, 0x5619, 0x561A,
 0x561A, 0x563A, 0x5EDD, 0x5493, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5EFE, 0x561A, 0x561A, 0x55B8, 0x0000, 0x0000, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x09C8, 0x5E5B, 0x5E5B, 0x5EDD, 0x0945, 0x0000, 0x0020, 0x0020, 0x0020,
 0x0000, 0x21E8, 0x21E8, 0x5E5A, 0x4E5A, 0x5E5B, 0x2228, 0x2228, 0x0000, 0x0020, 0x0020, 0x0020, 0x0000, 0x0882, 0x5EDD, 0x5E7B,
 0x5E7B, 0x1A4A, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x3C73, 0x565B, 0x4E1A, 0x55F9, 0x55F9, 0x567B,
 0x561A, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x4DF9, 0x45F9, 0x5E9C, 0x1BB0, 0x2A6A,
 0x2A6A, 0x0820, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x1B0D, 0x565A, 0x19C8, 0x0000, 0x0000, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x10E4, 0x10E4, 0x2ACC, 0x2370, 0x56BC, 0x56BC, 0x4DF9, 0x4E19, 0x55F9, 0x565B,
 0x565B, 0x4557, 0x22ED, 0x1186, 0x0882, 0x0882, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x5F1E, 0x3452, 0x3452, 0x0000, 0x0000, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0821, 0x2209, 0x2209, 0x4E3B, 0x1166, 0x0000, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0000, 0x0000, 0x11E8, 0x5E3B, 0x1A49, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x00A2, 0x4E5C, 0x2229,
 0x2229, 0x0840, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x3C73, 0x4E3A, 0x4E19, 0x4DF9, 0x4DF9, 0x565B,
 0x5E9B, 0x5E7B, 0x5E7B, 0x5E7B, 0x5E7B, 0x5E7B, 0x5E7B, 0x5E7B, 0x5E7B, 0x5E7B, 0x567B, 0x567B, 0x6F5E, 0x1946, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x29E8, 0x4E7B, 0x0000, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x3AEC, 0x3AEC, 0x6EBC, 0x567B, 0x76DC, 0x43F0,
 0x43F0, 0x0021, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x871D, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x0987, 0x0165, 0x0800, 0x0020, 0x0020, 0x0020,
 0x0020, 0x0020, 0x0020, 0x0000, 0x5E3B, 0x0021, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x00E4, 0x19E9, 0x0000,
 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x44D4, 0x5EDC, 0x5E7B, 0x567B, 0x567B, 0x66BC,
};

const uint8_t init_board[BOARD_SIZE_UD][BOARD_SIZE_LR] = {
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, BIGDOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, BIGDOT, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL, WALL, WALL, WALL, WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, BIGDOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, BIGDOT, WALL},
{WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
};
uint8_t board[BOARD_SIZE_UD][BOARD_SIZE_LR] = {
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, BIGDOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, BIGDOT, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL, WALL, WALL, WALL, WALL, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, WALL, DOT, WALL},
{WALL, BIGDOT, DOT, WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL, DOT, BIGDOT, WALL},
{WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, DOT, WALL, WALL},
{WALL, DOT, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, WALL, DOT, DOT, DOT, DOT, WALL},
{WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL, DOT, WALL, WALL, WALL, WALL, WALL, WALL, DOT, WALL},
{WALL, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, DOT, WALL},
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
};


sprite p = {1, BOARD_SIZE_UD - 2, RIGHT, {pacman_u, pacman_d, pacman_l, pacman_r}, 5, 5, RIGHT, 
						&GameEngine_pacmanUpdateMotion,1, BOARD_SIZE_UD - 2, &GameEngine_pacmanUpdatePosition, TRUE, TRUE, PACMAN, EMPTY, FALSE, 0, 0, FALSE, 0};
sprite rg = {1, 1, RIGHT, {r_ghost, r_ghost, r_ghost, r_ghost}, 5, 4, RIGHT,
						&GameEngine_ghostUpdateMotion,1, 1,&GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT, FALSE, blue_ghost, 0, FALSE, ghost_eyes};
sprite bg = {BOARD_SIZE_LR - 2, 1, RIGHT,
						 {light_blue_ghost, light_blue_ghost, light_blue_ghost, light_blue_ghost}, 5, 4, RIGHT, 
						&GameEngine_ghostUpdateMotion,BOARD_SIZE_LR - 2, 1, &GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT, FALSE, blue_ghost, 0, FALSE, ghost_eyes};
sprite og = {BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, RIGHT, {orange_ghost, orange_ghost, orange_ghost, orange_ghost}, 5, 4, RIGHT, 
						&GameEngine_ghostUpdateMotion,BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, &GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT, FALSE, blue_ghost, 0, FALSE, ghost_eyes};
sprite pg = {BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, RIGHT, {purple_ghost, purple_ghost, purple_ghost, purple_ghost}, 5, 4, RIGHT, 
						&GameEngine_ghostUpdateMotion,BOARD_SIZE_LR - 2, BOARD_SIZE_UD - 2, &GameEngine_ghostUpdatePosition, TRUE, TRUE, GHOST, DOT, FALSE, blue_ghost, 0, FALSE, ghost_eyes};
sprite* sprites[NUM_SPRITES] = {&p, &rg, &bg, &og, &pg};
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

void drawDot(uint8_t x, uint8_t y){
	uint8_t x_pix, y_pix;
	ind_to_pix(x, y, &x_pix, &y_pix);
	ST7735_DrawPixel(x_pix, y_pix + 4, ST7735_WHITE);
}
void drawBigDot(uint8_t x, uint8_t y){
	uint8_t x_pix, y_pix;
	ind_to_pix(x, y, &x_pix, &y_pix);
	ST7735_FillRect(x_pix - 1, y_pix + 4, 2, 2, ST7735_WHITE);
}
void drawWall(uint8_t x, uint8_t y){
	uint8_t x_pix, y_pix;
	ind_to_pix(x, y, &x_pix, &y_pix);
	ST7735_FillRect(x_pix -2, y_pix + 2, SQUARE_WIDTH, SQUARE_HEIGHT, ST7735_BLUE);
	
}
void ind_to_pix(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix) {
	*x_pix = BOARD_START_X + SQUARE_WIDTH * (x_ind);
	*y_pix = BOARD_START_Y + SQUARE_HEIGHT * (y_ind );
}

void drawCommon(uint8_t x_ind, uint8_t y_ind, uint8_t* x_pix, uint8_t* y_pix){
	ind_to_pix(x_ind, y_ind, x_pix, y_pix);
}

void drawSprite(sprite* s){
	uint8_t erase_x_pix, erase_y_pix, x_pix, y_pix;
	drawCommon(s->erase_x, s->erase_y, &erase_x_pix, &erase_y_pix);
	drawCommon(s->x, s->y, &x_pix, &y_pix);
	// draw lower layer
	if (board[s->erase_y][s->erase_x] == DOT){
		drawDot(s->erase_x, s->erase_y);
	}
	// erase
	ST7735_FillRect(erase_x_pix + FILL_RECT_OFFSET_X + Heartbeat_count * (x_pix - erase_x_pix) / 5,		
								  erase_y_pix + FILL_RECT_OFFSET_Y + Heartbeat_count * (y_pix - erase_y_pix) / 5,	
									s->width,			
									s->height + 1,		
									0);						
	
	// draw
	if (s->vulnerable && (s->vuln_count < VULN_FLASH || s->vuln_count % 2 == 1) ){
		ST7735_DrawBitmap(x_pix + DRAW_BITMAP_OFFSET_X + s->in_motion * Heartbeat_count * directions[s->motion].hor,
											y_pix + DRAW_BITMAP_OFFSET_Y + s->in_motion * Heartbeat_count * directions[s->motion].vert,
											s->vuln_bmp, s->width, s->height);
		
	} else if (s->eyes) {
		ST7735_DrawBitmap(x_pix + DRAW_BITMAP_OFFSET_X + s->in_motion * Heartbeat_count * directions[s->motion].hor,
											y_pix + DRAW_BITMAP_OFFSET_Y + s->in_motion * Heartbeat_count * directions[s->motion].vert,
											s->eyes_bmp, s->width, s->height);
	} else		{
		ST7735_DrawBitmap(x_pix + DRAW_BITMAP_OFFSET_X + s->in_motion * Heartbeat_count * directions[s->motion].hor,
											y_pix + DRAW_BITMAP_OFFSET_Y + s->in_motion * Heartbeat_count * directions[s->motion].vert,
											s->bmp[s->motion], s->width, s->height);
	}
}
void GraphicsEngine_drawCursor(uint8_t pos){
	ST7735_SetCursor(2, pos - 1);
	printf(" ");
	ST7735_SetCursor(2, pos + 1);
	printf(" ");
	ST7735_SetCursor(2, pos);
	printf(">");
}
void GraphicsEngine_initBoard(){
	uint8_t i, j;
	for (i = 0; i < BOARD_SIZE_UD; ++i){
		for (j = 0; j < BOARD_SIZE_LR; ++j){
			board[i][j] = init_board[i][j];
		}
	}
}

void GraphicsEngine_drawScore(void){
	ST7735_SetCursor(0, 0);
	printf("Score: %u", ScoreEngine_getScore());
}
void GraphicsEngine_drawBoard(void){
	// set initial position, number of ghosts, dots
	uint8_t i, j;
	
	//We need to first clear the screen
	ST7735_FillScreen(0);
	
	for (i = 0; i < BOARD_SIZE_UD; ++i){
		for (j = 0; j < BOARD_SIZE_LR; ++j){
			if (board[i][j] == DOT) { 
				drawDot(j, i);
			} else if (board[i][j] == BIGDOT) {
				drawBigDot(j, i);
			} else if (board[i][j] == WALL) {
				drawWall(j, i);
			} else if (board[i][j] == FRUIT) {
				// TODO
			}
		}
	}
	for (i = 0; i < NUM_SPRITES; ++i){
		drawSprite(sprites[i]);
	}
}

void GraphicsEngine_drawTitle(){
	//We need to first clear the screen
	ST7735_FillScreen(0);
	ST7735_DrawBitmap(0, 60, pacman_title, 128, 18);
	ST7735_SetCursor(4, TOP);
	printf("Play");
	ST7735_SetCursor(4, BOTTOM);
	printf("Top scores");
}

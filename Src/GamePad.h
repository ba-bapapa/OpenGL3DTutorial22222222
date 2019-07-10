#ifndef GAMEPAD_H_INCLUDE
#define GAMEPAD_H_INCLUDE
#include <stdint.h>

/**
*ゲームパッド情報
*/
struct GamePad {
	//キーとボタンに対応するビット定数
	static const uint32_t DPAD_UP = 0x0001;
	static const uint32_t DPAD_DOWN = 0x0002;
	static const uint32_t DPAD_LEFT = 0x0004;
	static const uint32_t DPAD_RIGHT = 0x00008;
	static const uint32_t START = 0x0010;
	static const uint32_t A = 0x0020;
	static const uint32_t B = 0x0040;
	static const uint32_t X = 0x0080;
	static const uint32_t Y = 0x0100;
	static const uint32_t L = 0x0200;
	static const uint32_t R = 0x0400;

	uint32_t buttons = 0;
	uint32_t buttonDown = 0;
};
#endif // !GAMEPAD_H_INCLUDE

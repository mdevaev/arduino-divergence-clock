#ifndef NIXIE_H
# define NIXIE_H

# define MAX_NIXIES 64

class Nixie {
	public:
		Nixie(int modules, int din_pin, int clk_pin, int stb_pin);
		void refresh();
		void setSymbol(unsigned char index, int ch, int dots);

	private:
		int modules;
		int din_pin;
		int clk_pin;
		int stb_pin;
		unsigned short buf[MAX_NIXIES];

		static const unsigned short SEG_0 = 0b0000001000000000;
		static const unsigned short SEG_1 = 0b0000000010000000;
		static const unsigned short SEG_2 = 0b0000000001000000;
		static const unsigned short SEG_3 = 0b0000000000010000;
		static const unsigned short SEG_4 = 0b0000000000001000;
		static const unsigned short SEG_5 = 0b0000000000000100;
		static const unsigned short SEG_6 = 0b0100000000000000;
		static const unsigned short SEG_7 = 0b0010000000000000;
		static const unsigned short SEG_8 = 0b0001000000000000;
		static const unsigned short SEG_9 = 0b0000010000000000;
		static const unsigned short SEG_LEFT  = 0b0000000000100000;
		static const unsigned short SEG_RIGHT = 0b0000100000000000;
		static const unsigned short SEGS_MASK = SEG_0|SEG_1|SEG_2|SEG_3|SEG_4|SEG_5|SEG_6|SEG_7|SEG_8|SEG_9|SEG_LEFT|SEG_RIGHT;
		static unsigned short DIGITS[10];

		static const unsigned short LED_RED   = 0b1000000000000000;
		static const unsigned short LED_GREEN = 0b0000000000000001;
		static const unsigned short LED_BLUE  = 0b0000000000000010;
		static const unsigned short LEDS_MASK = LED_RED|LED_GREEN|LED_BLUE;
};

#endif

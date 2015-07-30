#ifndef CLOCK_CTL_H
# define CLOCK_CTL_H

class ClockCtl {
	public:
		ClockCtl(int pload_pin, int clk_pin, int data_pin);
		void readCtl();

		unsigned alarm_hour;
		unsigned alarm_minute;
		bool alarm_enabled;

	private:
		int pload_pin;
		int clk_pin;
		int data_pin;
};

#endif // CLOCK_CTL_H

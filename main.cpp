#include <iostream>
#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>

#define SPACE      ' ' //'.'
#define BALL       'o'
#define DELAY      100
#define BALL_SPEED 2

class World {
	public:
		int width, height;

		World() {
			struct winsize sz;
			ioctl(0, TIOCGWINSZ, &sz);
			
			width = sz.ws_col;
			height = sz.ws_row;
			_bufsize = width*height;
			_buf = new char[_bufsize];
			clear();
		}

		char* get_buf() {
			return _buf;
		}
	
		int get_bufsize() {
			return _bufsize;
		}

		void clear() {
			for (int i = 0; i < _bufsize; i++) {
				_buf[i] = SPACE;
			}
			_buf[_bufsize] = BALL;
			_updated = true;
		}

		void set(int x, int y, char c) {
			if ((x < 0) || (x > width) || (y < 0) || (y > height)) {
				std::cerr << "CRASH_ON_POSITION_OVERFLOW" << std::endl;
				abort();	
			}
			_buf[width*y+x] = c;
			_updated = true;
		}

		void print() {
			if (_updated or true) {
				std::cout.write(_buf, _bufsize);
				std::cout << std::flush;
				_updated = false;
			}
		}
	private:
		int _bufsize;
		char* _buf;
		bool _updated = false;
};

World world;

struct Ball {
	int x = rand() % world.width;
	int y = rand() % world.height;
	int addx = BALL_SPEED;
	int addy = BALL_SPEED;
};

Ball* balls;
int   ballsc;

int main(int argc, char* argv[]) {
	// todo
	//printf("\e[?25l");

	if (argc == 1) {
		std::cout << "usage: ball <amount>" << std::endl << "    amount - Amount of balls" << std::endl;
		return 1;
	} else {
		ballsc = atoi(argv[1]);
		if (ballsc == 0) ballsc = 1;
	}
	balls = new Ball[ballsc];

	while (true) {
		world.clear();
		for (int i = 0; i < ballsc; i++) {
			Ball* ball = &balls[i];

			ball->x += ball->addx;
			ball->y += ball->addy;

			if (ball->x > world.width) {
				ball->addx = -BALL_SPEED;
				ball->x = world.width;
			} else if (ball->x < 0) {
				ball->addx = BALL_SPEED;
				ball->x = 0;
			}
			
			if (ball->y > world.height) {
				ball->addy = -BALL_SPEED;
				ball->y = world.height;
			} else if (ball->y < 0) {
				ball->addy = BALL_SPEED;
				ball->y = 0;
			}

			world.set(ball->x, ball->y, BALL);
		}

		world.print();		
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
	}
	return 0;
}

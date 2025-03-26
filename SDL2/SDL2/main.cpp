#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <random>
#include <vector>
#include <cmath>

// struct 
struct TARGET {
	int x, y;
};
struct HERO {
	int speed = 6;
	double x = 100, y = 100;     // địa chỉ hiện tại của hero 
	SDL_Rect rect = { static_cast<int>(x) - 50 , static_cast<int>(y) - 50 , 100 , 100};// hình vuông in ảnh 
	bool moving = 0 ;// kiểm tra xem hero có đang có lệnh chờ di chuyển không 
	TARGET target;
};
struct BULLET {
	int speed = 10;
	double x = 100, y = 100;// địa chỉ hiện tại của đạn ;
	SDL_Rect rect = { static_cast<int>(x) - 25 , static_cast<int>(y) - 25, 50 ,50};//hinh vuoong in ảnh đạn
	TARGET target;
	double step_x;
	double step_y;
};

//Kích thước của màn hình :
int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 800;
SDL_Event event;

bool running = 1; // biến ktra chương trình còn hoạt động không

//Khai báo các hàm :
void Moving(HERO& Hero);
void Update(HERO& Hero);
void CreateBullet (const HERO & Hero ,std::vector<BULLET> & Bullet);
void UpdateBullet(std::vector<BULLET>& Bullet);
void checkCollision(const HERO& Hero, const std::vector<BULLET>& Bullet);
int main(int argc , char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	HERO Hero;
	SDL_Window* window = SDL_CreateWindow("Dodge LOL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//Thông số BKG
	SDL_Rect Rect_BKG = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_Texture* bkground = IMG_LoadTexture(renderer, "img/nengame.png");
	//Thông số của Hero 
	SDL_Texture* imgHero = IMG_LoadTexture(renderer, "img/pngegg.png");
	// viên đạn 
	SDL_Texture* imgBullet = IMG_LoadTexture(renderer, "img/pngegg.png");
	//
	long long dem = 0 ;
	
	
	//________________________________________________________
	//vong lap su kien
	std::vector<BULLET> Bullet;
	//
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
				//ket thuc chuong trinh
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_RIGHT) {
					std::cout << "click chuot phai" << std::endl;
					std::cout << event.button.x << " " << event.button.y<< std::endl;
					Hero.moving = 1;// tạo lệnh di chuyển 
					Hero.target.x = event.button.x;
					Hero.target.y = event.button.y;
					//check 
					std::cout << "Hero.x and Hero.y " << Hero.x << " " << Hero.y << std::endl;
					std::cout << "Hero.rect.x.y " << Hero.rect.x << " " << Hero.rect.y << std::endl;
				}
				if (event.button.button == SDL_BUTTON_LEFT) {
					std::cout << "click chuot trai" << std::endl;
					std::cout << event.button.x << " " << event.button.y << std::endl;
				}
			}//click chuột 
		}//while event
		dem = dem + 1;
		if (dem %50 == 0 ) {
			CreateBullet(Hero, Bullet);
		}
		if (Hero.moving == 1) {
			Moving(Hero);
			Update(Hero);
		}
		UpdateBullet(Bullet);
		checkCollision(Hero, Bullet);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bkground, NULL, &Rect_BKG);//in ra BKG
		for (size_t i = 0; i < Bullet.size(); i++) {
			SDL_RenderCopy(renderer, imgBullet, NULL,& Bullet[i].rect);
		}
		SDL_RenderCopy(renderer, imgHero, NULL ,& Hero.rect);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}//while running
	//giải phóng bộ nhớ
	SDL_DestroyTexture(bkground);
	SDL_DestroyTexture(imgHero);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	std::cout << Bullet.size()<<std::endl;
	std::cout << dem;
	
	return 0;
}//main

void Moving(HERO& Hero) {
	double vector_x = Hero.target.x - Hero.x;
	double vector_y = Hero.target.y - Hero.y;
	double vector = sqrt(vector_x * vector_x + vector_y * vector_y);

	if (vector <= Hero.speed) {
		Hero.x = Hero.target.x;
		Hero.y = Hero.target.y;
		Hero.moving = 0;
	}
	else {
		double step_x = (vector_x / vector) * Hero.speed;
		double step_y = (vector_y / vector) * Hero.speed;
		Hero.x += step_x;
		Hero.y += step_y;
	}

	Hero.rect.x = static_cast<int>(Hero.x) - 50;  // Cập nhật vị trí vẽ
	Hero.rect.y = static_cast<int>(Hero.y) - 50;
}
void Update(HERO& Hero) {
	Hero.rect.x = Hero.x - 50;
	Hero.rect.y = Hero.y - 50;
}
void CreateBullet(const HERO& Hero,std:: vector<BULLET> & Bullet) {
	BULLET newBullet;
	newBullet.target.x = Hero.x;
	newBullet.target.y = Hero.y;

	static std::random_device rd;  // Seed
	static std::mt19937 mt(rd());  // Mersenne Twister
	std::uniform_int_distribution<int> dist(0,2400);
	
	int randPos = rand() % 4;  // Chỉ gọi rand() một lần

	if (randPos == 0) {
		newBullet.x = 0;
		newBullet.y = dist(mt) % SCREEN_HEIGHT;
	}
	else if (randPos == 1) {
		newBullet.x = SCREEN_WIDTH;
		newBullet.y = dist(mt) % SCREEN_HEIGHT;
	}
	else if (randPos == 2) {
		newBullet.x = dist(mt) % SCREEN_WIDTH;
		newBullet.y = 0;
	}
	else {
		newBullet.x = dist(mt) % SCREEN_WIDTH;
		newBullet.y = SCREEN_HEIGHT;
	}

	double vector_x = newBullet.target.x - newBullet.x;
	double vector_y = newBullet.target.y - newBullet.y;
	double vector = sqrt(vector_x * vector_x + vector_y * vector_y);

	newBullet.step_x = (vector_x / vector) * newBullet.speed;
	newBullet.step_y = (vector_y / vector) * newBullet.speed;

	Bullet.push_back(newBullet);
}
void UpdateBullet(std::vector<BULLET>& Bullet) {
	for (size_t i = 0; i < Bullet.size(); i++) {
		Bullet[i].x = Bullet[i].x + Bullet[i].step_x;
		Bullet[i].y = Bullet[i].y + Bullet[i].step_y;
		Bullet[i].rect.x = Bullet[i].x - 25;
		Bullet[i].rect.y = Bullet[i].y - 25;
		
	}
	for (size_t i = 0; i < Bullet.size(); i++) {
		if (Bullet[i].x<0 || Bullet[i].x>SCREEN_WIDTH || Bullet[i].y<0 || Bullet[i].y>SCREEN_HEIGHT) {
			Bullet.erase(Bullet.begin() + i);
		}
	}
}
void checkCollision(const HERO& Hero, const std::vector<BULLET>& Bullet) {
	for (size_t i = 0; i < Bullet.size(); i++) {
		double check_x = Bullet[i].rect.x - Hero.rect.x;
		double check_y = Bullet[i].rect.y - Hero.rect.y;
		double _check = sqrt(check_x * check_x + check_y * check_y);
		if (_check <= 60) {
			running = false ;
		}
	}
	
}
///ạkjshf

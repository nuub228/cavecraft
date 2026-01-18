#include "worldgen.cpp"
#include <iostream>

#define X 255
#define Y 127
#define MAXDROP 32767 //0...32767

struct Tdrop {
    int item_id, x_d, y_d;
};

struct Tinv {
    int item_id, item_count;
};

class Main {
public:
    World world;
    int velx, vely, acc;
    int del1, del2;
    int x, y, camx, camy, pos, curx, cury;
    int invslot;
    Tinv inv[36];

    bool mode, jmp, fall, deb;
    int frameStart, frameTime;
    const int FPS = 30;
    int frameDelay = 1000 / FPS;
    bool gamerunning = 1;
    Tdrop drop[MAXDROP + 1];
    int map[X + 1][Y + 1];
    SDL_Color color;
    TTF_Font* font;
    SDL_Surface* textsurface;

    SDL_DisplayMode displayMode;
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Surface* imtex;
    SDL_Rect tex[256];
    SDL_Surface* imtexdrop;
    SDL_Rect droptex[256];
    SDL_Surface* plrtex;
    SDL_Rect plr[2];

    SDL_Surface* guitex;
    SDL_Rect gui[2];

    bool KEYS[323];

    int width;
    int height;

    Main()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GetDesktopDisplayMode(0, &displayMode);
        width = (displayMode.w - (displayMode.w % 16)) / 2;
        height = (displayMode.h - (displayMode.h % 16)) / 2;
        window = SDL_CreateWindow("CaveCraft 1.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

        TTF_Init();
        font = TTF_OpenFont("slkscre.ttf", 8);

        screenSurface = SDL_GetWindowSurface(window);
        for (int i = 0; i < 322; i++) { // init them all to false
            KEYS[i] = false;
        }
    };
    bool tooMuchMore(int x)
    {
        if (x >= 323)
            return true;
        else
            return false;
    };
    void keyboard()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            if (tooMuchMore(event.key.keysym.sym))
                return;

            switch (event.type) {

            case SDL_KEYDOWN:
			std::cout << event.key.keysym.sym << std::endl;
                KEYS[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                KEYS[event.key.keysym.sym] = false; //test function
                break;
            default:
                break;
            }
        }
    };

    bool coll()
    {
        int ix, iy, ix1, iy1, iy2;

        ix = x / 16;
        iy = y / 16;
        ix1 = (x + 7) / 16;
        iy1 = (y + 15) / 16;
        iy2 = (y + 31) / 16;
        if (ix >= 0 && ix1 <= 255 && iy >= 0 && iy2 <= 127) {
            if (map[ix][iy] > 0 || map[ix1][iy] > 0 || map[ix][iy1] > 0 || map[ix][iy2] > 0 || map[ix1][iy1] > 0 || map[ix1][iy2] > 0) {
                return true;
            }
            else {
                return false;
            }
        }
    };

    void keyhandler()
    {

        keyboard();
        if (KEYS[27]) //esc exit
            gamerunning = 0;
        if (KEYS[109]) { //cursor mode true (m key)
            mode = true;
            curx = x / 16;
            cury = y / 16;
        }
        if (KEYS[110]) { //cursor mode false (n key)

            mode = false;
        }

        if (KEYS[49]) //inv slot select
            invslot = 0;
        if (KEYS[50])
            invslot = 1;
        if (KEYS[51])
            invslot = 2;
        if (KEYS[52])
            invslot = 3;
        if (KEYS[53])
            invslot = 4;
        if (KEYS[54])
            invslot = 5;
        if (KEYS[55])
            invslot = 6;
        if (KEYS[56])
            invslot = 7;
        if (KEYS[57])
            invslot = 8;

        if (mode == false) { //player move

            if (KEYS[32] && jmp == true) {
                vely = 7;
                jmp = false;
                fall = true;
            }
            if (KEYS[97]) {
                x -= velx;
                pos = 0;
                if (coll())
                    x += velx;
            }
            if (KEYS[100]) {
                x += velx;
                pos = 1;
                if (coll())
                    x -= velx;
            }
        } //player move
		if (mode == true) {
        if (KEYS[115]) { //cursor up
            cury++;
            if (cury > 127)
                cury = 127;
        } //w
        if (KEYS[119]) { //cursor down
            cury--;
            if (cury < 0)
                cury = 0;
        } //s
        if (KEYS[97]) { //cursor left
            curx--;
            if (curx < 0)
                curx = 0;
        } //a
        if (KEYS[100]) { //cursor right
            curx++;
            if (curx > 255)
                curx = 255;
        } //d
        if (KEYS[103]) { //cursor set block
            setblock(curx, cury);
        } //g
            if (KEYS[13]) { //block break (key "enter")
                createdrop(map[curx][cury], curx * 16, cury * 16);
                map[curx][cury] = 0;
            }
		}
        if (KEYS[120]) { //debug on
            deb = 1;
        }
        if (KEYS[122]) { //debug off
            deb = 0;
        }
    }

    void phyhandler()
    {

        if (fall) {
            if (vely > -16)
                vely -= acc;
            y -= vely;
        }
        if (coll()) {
            while (coll()) {
                if (vely < 0) {
                    y--;
                    jmp = true;
                }
                else {
                    y++;
                    vely = 0;
                    jmp = false;
                }
            }
            vely = 0;
        }
        else {
            fall = true;
        }
    }
    void setblock(int ix, int iy)
    {

        if (map[ix][iy] == 0)
            if (inv[invslot].item_id > 0 && inv[invslot].item_count > 0) {
                map[ix][iy] = inv[invslot].item_id;
                inv[invslot].item_count--;
            }
    }
    void createdrop(int it, int ix, int iy)
    {

        if (it > 0) {
            for (int iz = 0; iz <= MAXDROP; iz++)
                if (drop[iz].item_id == 0) {
                    drop[iz].item_id = it;
                    drop[iz].x_d = ix;
                    drop[iz].y_d = iy;
                    break;
                }
        }
    }
    void getdrop()
    {
        for (int ix = 0; ix <= MAXDROP; ix++) {
            if (drop[ix].item_id)
                if (x + 8 > drop[ix].x_d && x < drop[ix].x_d + 8 && y + 32 > drop[ix].y_d && y < drop[ix].y_d + 8) {
                    for (int iy = 0; iy <= 35; iy++) {
                        if (inv[iy].item_count < 64) {
                            if (inv[iy].item_id == drop[ix].item_id || inv[iy].item_count == 0) {
                                inv[iy].item_id = drop[ix].item_id;
                                inv[iy].item_count++;
                                drop[ix].item_id = 0;
                                break;
                            }
                        }
                    }
                }
        }
    }
    void game()
    {
        getdrop();
        if (x < 0)
            x = 0;
        if (x + 8 > 4094)
            x = 4086;
    }
    void newworld()
    {

        world.genworld();

        for (int ix = 0; ix <= X; ix++)
            for (int iy = 0; iy <= Y; iy++)
                map[ix][iy] = world.getblock(ix, iy);
    };
    void loadtexture()
    {
        int ix, iy, iz;
        iz = 0;
        imtex = IMG_Load("res/terrain.png");
        if (imtex == NULL) {
            std::cout << IMG_GetError();
        }
        for (int iy = 0; iy <= 15; iy++) {
            for (int ix = 0; ix <= 15; ix++) {

                tex[iz] = { ix * 16, iy * 16, 16, 16 };
                iz++;
            }
        }
        imtexdrop = IMG_Load("res/drop.png");
        if (imtexdrop == NULL) {
            std::cout << IMG_GetError();
        }
        iz = 0;
        for (int iy = 0; iy <= 15; iy++) {
            for (int ix = 0; ix <= 15; ix++) {

                droptex[iz] = { ix * 8, iy * 8, 8, 8 };
                iz++;
            }
        }
        //надо перерисовать текстурки обычных блоков 16на16 в 8на8 для дропа, и загрузить их в переменные как на верху

        plrtex = IMG_Load("res/char.png");
        if (plrtex == NULL) {
            std::cout << IMG_GetError();
        }
        plr[0] = { 0, 0, 8, 32 };
        plr[1] = { 24, 0, 8, 32 };

        guitex = IMG_Load("res/gui.png");
        if (guitex == NULL) {
            std::cout << IMG_GetError();
        }
        gui[0] = { 0, 0, 16, 16 };
        gui[1] = { 0, 48, 162, 18 };
    };
    void draw()
    {
        int minx, miny, maxx, maxy, tmp_ax;

        //{===================[Небо]===================}
        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0, 255, 255));
        //{===================[Камера]===================}
        camx = (x + 4) - (width / 2);
        camy = (y + 4) - (height / 2);
        if (camx < 0)
            camx = 0;
        if (camx > 4096 - width)
            camx = 4096 - width;
        if (camy < 0)
            camy = 0;
        if (camy > 2048 - height)
            camy = 2048 - height;
        //{===================[Ограничение_отрисовки]===================}
        minx = camx / 16;
        miny = camy / 16;
        maxx = (camx + width) / 16;
        maxy = (camy + height) / 16;
        if (minx < 0)
            minx = 0;
        if (miny < 0)
            miny = 0;
        if (maxx > 255)
            maxx = 255;
        if (maxy > 126)
            maxy = 127;
        //{===================[Отрисовка_мира]===================}
        for (int ix = minx; ix <= maxx; ix++)
            for (int iy = miny; iy <= maxy; iy++)
                if (map[ix][iy] > 0)
                    drawblock(map[ix][iy], (ix * 16) - camx, (iy * 16) - camy);
        //{===================[Дроп]===================}
        for (int ix = 0; ix <= MAXDROP; ix++)
            drawdrop(drop[ix].item_id, drop[ix].x_d - camx, drop[ix].y_d - camy);

        //{===================[Инрок]===================}
        drawplayer(pos, x - camx, y - camy);
        //{===================[Интерфейс]===================}
        tmp_ax = (width / 2) - (162 / 2);
        if (mode)
            drawcursor(curx * 16 - camx, cury * 16 - camy);
        drawgui(tmp_ax, 0);
        for (int ix = 0; ix <= 8; ix++)
            if (inv[ix].item_id > 0 && inv[ix].item_count > 0) {
                drawinvitem(inv[ix].item_id, (ix * 16) + tmp_ax + ix * 2 + 1, 1);
                if (inv[ix].item_count > 1) {
                    this->setcolor(0, 0, 0);
                    drawtext(std::to_string(inv[ix].item_count), (ix * 16) + tmp_ax + ix * 2 + 1, 1);
                }
            }
        drawcursor((invslot * 16) + tmp_ax + invslot * 2 + 1, 1);
        if (deb) {
            debug_info();
        }
    };
    void debug_info()
    {
		std::string temp_string;
		temp_string = "X: "+std::to_string(x / 16-128);
        drawtext(temp_string, 0, 0);
		temp_string = "y: "+std::to_string(y / 16);
        drawtext(temp_string, 0, 16);
		temp_string = "CURX: "+std::to_string(curx-128);
        drawtext(temp_string, 0, 32);
		temp_string = "CURY: "+std::to_string(cury);
        drawtext(temp_string, 0, 48);

    }
    void drawtext(std::string txt, int x, int y)
    {

        textsurface = TTF_RenderText_Solid(font, txt.c_str(), color);
        SDL_Rect xy_rect = { x, y, 0, 0 };
        SDL_BlitSurface(textsurface, NULL, screenSurface, &xy_rect);
    }
    void setcolor(Uint8 r, Uint8 g, Uint8 b)
    {
        color = { r, g, b };
    }
    void drawinvitem(int blockid, int x, int y)
    {
        SDL_Rect xy_rect = { x, y, 16, 16 };
        SDL_BlitSurface(imtex, &tex[blockid], screenSurface, &xy_rect);
    }
    void drawgui(int x, int y)
    {

        SDL_Rect xy_rect = { x, y, 162, 18 };

        SDL_BlitSurface(guitex, &gui[1], screenSurface, &xy_rect);
    }
    void drawblock(int blockid, int x, int y)
    {
        if (blockid == 0) {
            return;
        }
        SDL_Rect xy_rect = { x, y, 16, 16 };
        SDL_BlitSurface(imtex, &tex[blockid], screenSurface, &xy_rect);
    }
    void drawdrop(int id, int x, int y)
    {

        SDL_Rect xy_rect = { x, y, 8, 8 };
        SDL_BlitSurface(imtexdrop, &droptex[id], screenSurface, &xy_rect);
    }
    void drawcursor(int x, int y)
    {

        SDL_Rect xy_rect = { x, y, 16, 16 };
        SDL_BlitSurface(guitex, &gui[0], screenSurface, &xy_rect);
    }
    void drawplayer(int temp_pos, int x, int y)
    {

        SDL_Rect xy_rect = { x, y, 8, 32 };
        SDL_BlitSurface(plrtex, &plr[temp_pos], screenSurface, &xy_rect);
    };

    void proc_fps()
    {
        frameStart = SDL_GetTicks();
    }
    void maxfps()
    {
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

};

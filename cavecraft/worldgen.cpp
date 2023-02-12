#include "randoms.cpp"

#define X 255
#define Y 127

class World {
public:
    int map[X + 1][Y + 1];
    Randoms seed;
    void genworld()
    {
        //заполнение пустотой
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 0; iy <= Y; iy++)
                map[ix][iy] = 0;
        //неровности рельефа(грязь)
        for (int ix = 0; ix <= X; ix++) {
            map[ix][63] = seed.rrnd_pr(30, 1, 0);
            if (map[ix][63] == 0) {
                map[ix][64] = seed.rrnd_pr(90, 1, 0);
            }
            else {
                map[ix][64] = 1;
            }
            if (map[ix][64] == 0) {
                map[ix][65] = seed.rrnd_pr(99, 1, 0);
            }
            else {
                map[ix][65] = 1;
            }
        }
        //заполнение грязью между началом генерации камня и концом генерации неровностей(грязи)
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 66; iy <= 72; iy++)
                map[ix][iy] = 1;
        //неровности рельефа(камень)
        for (int ix = 0; ix <= X; ix++) {
            map[ix][73] = seed.rrnd_pr(30, 3, 1);
            map[ix][74] = seed.rrnd_pr(90, 3, 1);
            map[ix][75] = seed.rrnd_pr(99, 3, 1);
        }
        //заполнение камнем между бедроком и концом генерации неровностей(камня)
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 76; iy <= 126; iy++)
                map[ix][iy] = 3;

        //генерация деревьев
        for (int ix = 0; ix <= X; ix++)
            if (seed.rrnd_pr(30, 1, 0) == 1 && map[ix][62 + 1] != 0 && map[ix - 1][62 - 1] == 0) {

                map[ix][62] = 9;
                map[ix][62 - 1] = 9;
                map[ix][62 - 2] = 9;

                map[ix][62 - 3] = 21;
                map[ix - 1][62 - 3] = 21;
                map[ix + 1][62 - 3] = 21;

                map[ix][62 - 4] = 21;
                map[ix - 1][62 - 4] = 21;
                map[ix + 1][62 - 4] = 21;

                map[ix][62 - 5] = 21;
            }
        //уголь
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 75; iy <= Y-2; iy++)
                if (seed.rrnd_pr(5, 1, 0) == 1) {
                    for (int iu = 0; iu <= 2; iu++)
                        for (int iv = 0; iv <= 2; iv++)
                            if (ix + iu < 255 && iy + iv > 1)
                                map[ix + iu][iy + iv] = seed.rrnd_pr(50, 18, map[ix + iv][iy + iv]);
                    ix = ix + 6;
                }
        //железо
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 75; iy <= Y - 2; iy++)
                if (seed.rrnd_pr(4, 1, 0) == 1) {
                    for (int iu = 0; iu <= 2; iu++)
                        for (int iv = 0; iv <= 2; iv++)
                            if (ix + iu < 255 && iy + iv > 1)
                                map[ix + iu][iy + iv] = seed.rrnd_pr(30, 17, map[ix + iv][iy + iv]);
                    ix = ix + 6;
                }
        //золото
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 95; iy <= Y - 2; iy++)
                if (seed.rrnd_pr(3, 1, 0) == 1) {
                    for (int iu = 0; iu <= 2; iu++)
                        for (int iv = 0; iv <= 2; iv++)
                            if (ix + iu < 255 && iy + iv > 1)
                                map[ix + iu][iy + iv] = seed.rrnd_pr(25, 16, map[ix + iv][iy + iv]);
                    ix = ix + 6;
                }
        //редстоун
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 113; iy <= Y - 2; iy++)
                if (seed.rrnd_pr(1, 1, 0) == 1) {
                    for (int iu = 0; iu <= 2; iu++)
                        for (int iv = 0; iv <= 2; iv++)
                            if (ix + iu < 255 && iy + iv > 1)
                                map[ix + iu][iy + iv] = seed.rrnd_pr(25, 20, map[ix + iv][iy + iv]);
                    ix = ix + 6;
                }
        //алмазы
        for (int ix = 0; ix <= X; ix++)
            for (int iy = 113; iy <= Y - 2; iy++)
                if (seed.rrnd_pr(1, 1, 0) == 1) {
                    for (int iu = 0; iu <= 2; iu++)
                        for (int iv = 0; iv <= 2; iv++)
                            if (ix + iu < 255 && iy + iv > 1)
                                map[ix + iu][iy + iv] = seed.rrnd_pr(10, 19, map[ix + iv][iy + iv]);
                    ix = ix + 6;
                }
        //покрытие травой неровностей грязевого рельефа
        for (int ix = 0; ix <= X; ix++) {
            if (map[ix][62] == 0 && map[ix][63] == 1) map[ix][63] = 2;
            
            if (map[ix][63] == 0 && map[ix][64] == 1) map[ix][64] = 2;
            
            if (map[ix][64] == 0 && map[ix][65] == 1) map[ix][65] = 2;
            
        }
        //генерация слоя бедрока на x-0
        for (int ix = 0; ix <= X; ix++)
            map[ix][127] = 6;
    };
    int getblock(int x, int y)
    {

        return map[x][y];
    };
    int getply()
    {

        for (int iy = 0; iy <= 127; iy++)
            if (map[128][iy] != 0)
                return (iy - 2) * 16;
			
    };
};
class Randoms {
public:
    int seed;
    void ssrand(int s = 6645)
    {

        seed = s;
    };
    int rrand()
    {
        seed = seed * 1103515245;
        return abs(seed);
    };

    int rrnd(int max) { return rrand() % max; };
    int rrnd_pr(int pr, int pr1, int pr2)
    {
        if (pr >= rrnd(101)) {
            return pr1;
        }
        else {
            return pr2;
        }
    };
};
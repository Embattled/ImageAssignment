#ifndef IMAGE_H
#define IMAGE_H

#define RYU_SUCCESS 0
#define RYU_FAILURE 1

#define FORMATERROR 1
#define DESTMATINTERROR 2
#define MEMNEWERROR 3
#define MODEPROPERTYERROR 4
namespace ryu
{
    class mat;

    int pgm2ppm(const mat *source, mat *destination);
    int pgm2pbm(const mat *source, mat *destination, unsigned int threshold = 0);

    int flipVertical(mat *source, mat *destination = nullptr);
    int flipHorizontal(mat *source, mat *destination = nullptr);

    int shrinkpgm(char mode, const mat *source, mat *destination, double times);
    int nearestNeighbor(const mat *source, mat *destination, double times);
    int bilinearInterpolation(const mat *source, mat *destination, double times);

    int rotation(const mat *source, mat *destination, double angle);

    int convolution(const mat *source, mat *destination, int core[9]);

    int smooth(const mat *source, mat *destination, int weight);
    int sobel(const mat *source, mat *destination);
    int laplacian(const mat *source, mat *destination);

    // ERROR OUTPUT

    void lastErrorPrint();

} // namespace ryu
#endif
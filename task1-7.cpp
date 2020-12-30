
#include "mat.h"
#include "image.h"

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        ryu::mat *myImage = new ryu::mat(argv[1]);
        myImage->printInfor();

        //copy
        ryu::mat *output = new ryu::mat(*myImage);
        output->writeAscFile("copy.pgm");

        // convert
        ryu::pgm2ppm(myImage, output);
        output->writeAscFile("pgm2ppm.ppm");

        ryu::pgm2pbm(myImage, output);
        // output->writeAscFile("pgm2pbm.ppm");

        // vertical flip
        *output = *myImage;
        ryu::flipVertical(output);
        output->writeAscFile("vertical.ppm");

        //horizontal flip
        *output = *myImage;
        ryu::flipHorizontal(output);
        output->writeAscFile("horizontal.pgm");

        //shrink
        ryu::shrinkpgm('n', myImage, output, 2);
        output->writeAscFile("nearestNeighbor2time.pgm");
        ryu::shrinkpgm('n', myImage, output, 0.5);
        output->writeAscFile("nearestNeighbor0.5time.pgm");
        ryu::shrinkpgm('b', myImage, output, 2);
        output->writeAscFile("bilinearInterpolation2time.pgm");
        ryu::shrinkpgm('b', myImage, output, 0.5);
        output->writeAscFile("bilinearInterpolation0.5time.pgm");

        //rotation
        ryu::rotation(myImage, output, 45);
        output->writeAscFile("rotation45.pgm");

        //smooth
        ryu::smooth(myImage, output, 1);
        output->writeAscFile("smooth.pgm");

        ryu::sobel(myImage, output);
        output->writeAscFile("sobel.pgm");

        ryu::laplacian(myImage, output);
        output->writeAscFile("laplacian.pgm");
    }

    return 0;
}

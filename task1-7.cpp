#include "stdio.h"
#include "mat.h"
#include "image.h"
#include "cstring"

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        ryu::mat *myImage = new ryu::mat(argv[1]);
        myImage->printInfor();
    }
    else if (argc > 2)
    {
        ryu::mat *myImage = new ryu::mat(argv[1]);
        ryu::mat *newImage = new ryu::mat();
        const char *outputfile = nullptr;
        // int opRes = 0;

        myImage->printInfor();
        if (myImage->statusCheck() != RYU_MAT_SUCCESS)
        {
            return 0;
        }

        if (strcmp(argv[2], "copy") == 0)
        {
            if (argc > 3)
            {
                outputfile = argv[3];
            }
            myImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "g2p") == 0)
        {
            if (argc > 3)
            {
                outputfile = argv[3];
            }
            ryu::pgm2ppm(myImage, newImage);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "g2b") == 0&&argc > 3)
        {
            if (argc > 4)
            {
                outputfile = argv[4];
            }
            ryu::pgm2pbm(myImage, newImage, argv[3]);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "fv") == 0)
        {
            if (argc > 3)
            {
                outputfile = argv[3];
            }
            ryu::flipVertical(myImage, newImage);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "fh") == 0)
        {
            if (argc > 3)
            {
                outputfile = argv[3];
            }
            ryu::flipHorizontal(myImage, newImage);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "sh") == 0&&argc>4)
        {
            if (argc > 5)
            {
                outputfile = argv[5];
            }
            ryu::shrinkpgm(myImage, newImage, argv[3], argv[4]);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "ro") == 0 && argc > 3)
        {
            if (argc > 4)
            {
                outputfile = argv[4];
            }
            ryu::rotation(myImage, newImage, argv[3]);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "sm") == 0 && argc > 3)
        {
            if (argc > 4)
            {
                outputfile = argv[4];
            }
            ryu::smooth(myImage, newImage, argv[3]);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "so") == 0)
        {
            if (argc > 3)
            {
                outputfile = argv[3];
            }
            ryu::sobel(myImage, newImage);
            newImage->writeBinFile(outputfile);
        }
        else if (strcmp(argv[2], "lap") == 0)
        {
            if (argc > 3)
            {
                outputfile = argv[3];
            }
            ryu::laplacian(myImage, newImage);
            newImage->writeBinFile(outputfile);
        }
        else
        {
            printf("Wrong command\n");
        }
    }
    else
    {
        printf("./Image (filename) [command] [mode] [param] [outputFile]\n");
        // printf("Image (filename) [command] [mode] [param] [outputFile] [write mode]\n");
        // printf("Write mode : a - ascii mode, b - binary mode \n");
        printf("\n");
        printf("Operation     \tcommand \t mode \t param\n");
        printf("Copy          \tcopy    \t -    \t - \n");
        printf("pgm2ppm       \tg2p     \t -    \t - \n");
        printf("pgm2pbm       \tg2b     \t -    \t threshold\n");
        printf("flipVertical  \tfv      \t -    \t - \n");
        printf("flipHorizontal\tfh      \t -    \t - \n");
        printf("shrinkpgm     \tsh      \t n,b  \t time \n");
        printf("rotation      \tro      \t -    \t angle\n");
        printf("smooth        \tsm      \t -    \t weight \n");
        printf("sobel         \tso      \t -    \t - \n");
        printf("laplacian     \tlap     \t -    \t - \n");
    }
    return 0;
}

# ImageAssignment

This project is an exercise project assigned to the students of Goto laboratory, GSIS of Tohoku University. 
By implementing various operation transformations of images without using library functions to master the basic image processing algorithm.
Assignment details is in the "taskInformation.txt" file.

The copyright of the problem belongs to professor Goto.
Source code written in April 2020 - October 2020 by Yongzhen Long.

# Files

1. main.cpp
    Define entry to program and operation command.
2. mat.h mat.cpp
    Define the class of image data, including W/R disk files.
3. image.h image.cpp
    Define core image transformation functions.


# Manual

All operation include:
* Print Image Information
* Write Image as an AsciiFile.
* pgm2ppm
* pgm2pbm
* flipVertical
* flipHorizontal
* Shrink (rescale)
* Rotation image
* Smooth
* Sobel eage extract
* Laplacian eage extract


```c
printf("./Image (filename) [command] [mode] [param] [outputFile]\n");
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
```
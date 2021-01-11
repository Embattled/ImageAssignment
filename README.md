# ImageAssignment

This project is an exercise project assigned to the students of Goto laboratory, GSIS of Tohoku University. 
By implementing various operation transformations of images without using library functions to master the basic image processing algorithm.
Assignment details is in the "taskInformation.txt" file.

The copyright of the problem belongs to professor Goto.
Source code written in April 2020 - October 2020 by Yongzhen Long.

# Manual

All operation include:
* Print Image Information
* Write Image as an AsciiFile.
* pgm2ppm
* pgm2pbm
* flipVertical
* flipHorizontal
* Shrink
* Rotation
* Smooth
* Sobel
* Laplacian

~~Run `./image input.pgm` will execute all operation above on input.pgm file.  ~~
~~The command mode is to be update.  ~~

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
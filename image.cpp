#include "image.h"
#include "mat.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace ryu
{
    int errorProblem;
    const mat *errorMatP = nullptr;

    void lastErrorPrint()
    {
        switch (errorProblem)
        {
        case FORMATERROR:
            std::cout << "Source image doesn't pgm image." << std::endl;
            break;
        case DESTMATINTERROR:
            std::cout << "Destination mat initialize failure." << std::endl;
            break;
        case MEMNEWERROR:
            std::cout << "Memory allocation error." << std::endl;
            break;
        case MODEPROPERTYERROR:
            std::cout << "Shrink with illegal mode property." << std::endl;
            break;
        case PARAMETERERROR:
            std::cout << "Illegal parameter." << std::endl;
            break;
        default:
            break;
        }
        if (errorMatP != nullptr)
            if (errorMatP->statusCheck() == RYU_MAT_SUCCESS)
            {
                std::cout << "Operation image's information:" << std::endl;
                errorMatP->printInfor();
            }
    }

    int pgm2ppm(const mat *source, mat *destination)
    {
        if (source->getFormat() != 2)
        {
            // std::cout << "Source image doesn't pgm image.";
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        int format = 3;
        int width = source->getWidth();
        int hight = source->getHight();
        int colordepth = source->getColordepth();
        if (destination->initialize(format, width, hight, colordepth) != RYU_MAT_SUCCESS)
        {
            errorProblem = DESTMATINTERROR;
            errorMatP = destination;
            return RYU_FAILURE;
        }

        long long sourceIndex = 0, destinIndex = 0;
        for (int row = 0; row < hight; row++)
        {
            for (int column = 0; column < width; column++)
            {
                destination->ascContent[destinIndex++] = source->ascContent[sourceIndex];
                destination->ascContent[destinIndex++] = source->ascContent[sourceIndex];
                destination->ascContent[destinIndex++] = source->ascContent[sourceIndex];
                sourceIndex++;
            }
        }
        // long long srcValueNumber = width * hight;
        // for (long long sourceIndex = 0, destinIndex = 0; sourceIndex < srcValueNumber;)
        // {
        //     destination->ascContent[destinIndex++] = source->ascContent[sourceIndex];
        //     destination->ascContent[destinIndex++] = source->ascContent[sourceIndex];
        //     destination->ascContent[destinIndex++] = source->ascContent[sourceIndex];
        //     sourceIndex += 1;
        // }
        return RYU_SUCCESS;
    }

    int pgm2pbm(const mat *source, mat *destination, const char *cthreshold)
    {

        unsigned int threshold = (unsigned int)atoi(cthreshold);
        if (source->getFormat() != 2)
        {
            // std::cout << "Source image doesn't pgm image.";
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }

        // if threshould not set, default value is source image's colordepth/2.
        if (threshold <= 0 || threshold > (unsigned int)source->getColordepth())
            threshold = source->getColordepth() / 2;

        int format = 1;
        int width = source->getWidth();
        int hight = source->getHight();
        int colordepth = source->getColordepth();
        if (destination->initialize(format, width, hight, colordepth) != RYU_MAT_SUCCESS)
        {
            errorProblem = DESTMATINTERROR;
            errorMatP = destination;
            return RYU_FAILURE;
        }

        long long index = 0;
        for (int row = 0; row < hight; row++)
        {
            for (int column = 0; column < width; column++)
            {
                destination->ascContent[index] = source->ascContent[index] > threshold ? 0 : 1;
                index++;
            }
        }
        // long long srcValueNumber = width * hight;
        // for (long long index = 0; index < srcValueNumber; index++)
        // {
        //     destination->ascContent[index] = source->ascContent[index] > threshold ? 0 : 1;
        // }
        return RYU_SUCCESS;
    }

    int flipVertical(mat *source, mat *destination)
    {
        int format = source->getFormat();
        int width = source->getWidth();
        int hight = source->getHight();
        int colorNum = format == 3 || format == 6 ? 3 : 1;
        long long srcValueNumber = width * hight * colorNum;

        unsigned int *oldData = source->ascContent;

        unsigned int *newData;
        if (destination != nullptr)
        {
            if (destination->initialize(source->getFormat(), source->getWidth(), source->getHight(), source->getColordepth()) != RYU_MAT_SUCCESS)
            {
                errorProblem = DESTMATINTERROR;
                errorMatP = destination;
                return RYU_FAILURE;
            }
            newData = destination->ascContent;
        }
        else
        {
            newData = new (std::nothrow) unsigned int[srcValueNumber];
            if (newData == nullptr)
            {
                errorProblem = MEMNEWERROR;
                errorMatP = nullptr;
                return RYU_FAILURE;
            }
        }
        long long destinIndex = 0;
        //last row start
        for (int y = hight - 1; y >= 0; y--)
        {
            long long sourceIndex = y * width * colorNum;
            for (int x = 0; x < width * colorNum; x++)
            {
                newData[destinIndex++] = oldData[sourceIndex++];
            }
        }
        source->ascContent = newData;
        delete[] oldData;

        return RYU_SUCCESS;
    }

    int flipHorizontal(mat *source, mat *destination)
    {
        int format = source->getFormat();
        int width = source->getWidth();
        int hight = source->getHight();
        int colorNum = format == 3 || format == 6 ? 3 : 1;
        long long srcValueNumber = width * hight * colorNum;

        unsigned int *oldData = source->ascContent;
        unsigned int *newData;
        if (destination != nullptr)
        {
            if (destination->initialize(source->getFormat(), source->getWidth(), source->getHight(), source->getColordepth()) != RYU_MAT_SUCCESS)
            {
                errorProblem = DESTMATINTERROR;
                errorMatP = destination;
                return RYU_FAILURE;
            }
            newData = destination->ascContent;
        }
        else
        {
            newData = new (std::nothrow) unsigned int[srcValueNumber];
            if (newData == nullptr)
            {
                errorProblem = MEMNEWERROR;
                errorMatP = nullptr;
                return RYU_FAILURE;
            }
        }
        long long destinIndex = 0;
        //first row start
        // for (inty = 0; y < hight; y++)
        // {
        //     longlongrowBias = y * width;
        //     for (intx = width - 1; x >= 0; x--)
        //     {
        //         longlongsourceDataIndex = (rowBias + x) * colorNum;
        //         for (intcolor = 0; color < colorNum; color++)
        //         {
        //             newData[destinIndex++] = oldData[sourceDataIndex++];
        //         }
        //     }
        // }
        if (colorNum == 3)
        {
            for (int y = 0; y < hight; y++)
            {
                long long rowBias = y * width;
                for (int x = width - 1; x >= 0; x--)
                {
                    long long sourceDataIndex = (rowBias + x) * colorNum;
                    newData[destinIndex++] = oldData[sourceDataIndex++];
                    newData[destinIndex++] = oldData[sourceDataIndex++];
                    newData[destinIndex++] = oldData[sourceDataIndex++];
                }
            }
        }
        else
        {
            for (int y = 0; y < hight; y++)
            {
                long long rowBias = y * width;
                for (int x = width - 1; x >= 0; x--)
                {
                    long long sourceDataIndex = (rowBias + x) * colorNum;
                    newData[destinIndex++] = oldData[sourceDataIndex];
                }
            }
        }

        source->ascContent = newData;
        delete[] oldData;
        return RYU_SUCCESS;
    } // namespace ryu

    int shrinkpgm(const mat *source, mat *destination, const char *cmode, const char *strtimes)
    {
        int format = source->getFormat();
        if (format != 2)
        {
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        double times = atof(strtimes);
        if (times < 0 || times > 20)
        {
            errorProblem = PARAMETERERROR;
            errorMatP = nullptr;
            return RYU_FAILURE;
        }

        int newWidth = (int)(source->getWidth() * times+0.5);
        int newHeight = (int)(source->getHight() * times+0.5);

        if (destination->initialize(2, newWidth, newHeight, source->getColordepth()) != RYU_MAT_SUCCESS)
        {
            errorProblem = DESTMATINTERROR;
            errorMatP = destination;
            return RYU_FAILURE;
        }

        //select shrink function
        if (strcmp(cmode, "n") == 0)
        {
            return nearestNeighbor(source, destination, times);
        }
        else if (strcmp(cmode, "b") == 0)
        {
            return bilinearInterpolation(source, destination, times);
        }
        //wrong input
        else
        {
            errorProblem = MODEPROPERTYERROR;
            errorMatP = nullptr;
            return RYU_FAILURE;
        }

        return RYU_SUCCESS;
    }
    int nearestNeighbor(const mat *source, mat *destination, double times)
    {
        int oldWidth = source->getWidth();
        int oldHigh = source->getHight();
        int newWidth = (int)(oldWidth * times+0.5);
        int newHeight = (int)(oldHigh * times+0.5);

        double HShrinkTimes = (double)newWidth / oldWidth;
        double VShrinkTimes = (double)newHeight / oldHigh;

        // int *oldRow = new (std::nothrow) int[newHeight];
        // int *oldColumn = new (std::nothrow) int[newWidth];
        // if (oldRow == nullptr || oldColumn == nullptr)
        // {
        //     errorProblem = MEMNEWERROR;
        //     errorMatP = nullptr;
        //     return RYU_FAILURE;
        // }

        // for (int i = 1; i <= newHeight; i++)
        // {
        //     oldRow[i - 1] = (int)round(i / VShrinkTimes) - 1;
        // }

        // for (int i = 1; i <= newWidth; i++)
        // {
        //     oldColumn[i - 1] = (int)round(i / HShrinkTimes) - 1;
        // }
        for (int row = 0; row < newHeight; row++)
        {
            for (int column = 0; column < newWidth; column++)
            {
                int oldRow = (int)(0.5+(row + 1) / VShrinkTimes) - 1;
                int oldColumn = (int)(0.5+(column + 1) / HShrinkTimes) - 1;
                long long oldIndex = oldRow * oldWidth + oldColumn;

                long long newIndex = row * newWidth + column;
                // long long oldIndex = oldRow[row] * oldWidth + oldColumn[column];
                destination->ascContent[newIndex] = source->ascContent[oldIndex];
            }
        }

        // delete[] oldRow;
        // delete[] oldColumn;
        return RYU_SUCCESS;
    }
    int bilinearInterpolation(const mat *source, mat *destination, double times)
    {
        int oldWidth = source->getWidth();
        int oldHigh = source->getHight();
        int newWidth = (int)(oldWidth * times+0.5);
        int newHeight = (int)(oldHigh * times+0.5);

        // long long valueNumber = newWidth * newHeight;

        double xShrinkTimes = (double)(oldWidth - 1) / (double)(newWidth + 1);
        double yShrinkTimes = (double)(oldHigh - 1) / (double)(newHeight + 1);

        // int *oldXCeil = new (std::nothrow) int[newWidth];
        // int *oldXFloor = new (std::nothrow) int[newWidth];
        // double *xCeilWeight = new (std::nothrow) double[newWidth];
        // double *xFloorWeight = new (std::nothrow) double[newWidth];
        // if (oldXCeil == nullptr || oldXFloor == nullptr || xCeilWeight == nullptr || xFloorWeight == nullptr)
        // {
        //     errorProblem = MEMNEWERROR;
        //     errorMatP = nullptr;
        //     return RYU_FAILURE;
        // }

        // for (int i = 1; i <= newWidth; i++)
        // {
        //     double floatCoordinate = i * xShrinkTimes;
        //     oldXCeil[i - 1] = (int)ceil(floatCoordinate);
        //     oldXFloor[i - 1] = (int)floor(floatCoordinate);
        //     xCeilWeight[i - 1] = floatCoordinate - oldXFloor[i - 1];
        //     xFloorWeight[i - 1] = 1 - xCeilWeight[i - 1];
        // }

        // double *xInterpolation = new (std::nothrow) double[newWidth * oldHigh]{0};
        // if (xInterpolation == nullptr)
        // {
        //     errorProblem = MEMNEWERROR;
        //     errorMatP = nullptr;
        //     return RYU_FAILURE;
        // }

        // for (int row = 0; row < oldHigh; row++)
        // {
        //     long long rowIndex = row * newWidth;
        //     long long rowOldIndex = row * oldWidth;
        //     for (int x = 0; x < newWidth; x++)
        //     {
        //         xInterpolation[rowIndex + x] += source->ascContent[rowOldIndex + oldXCeil[x]] * xCeilWeight[x];
        //         xInterpolation[rowIndex + x] += source->ascContent[rowOldIndex + oldXFloor[x]] * xFloorWeight[x];
        //     }
        // }

        // -----------LUT y -----------
        // int *oldYCeil = new (std::nothrow) int[newHeight];
        // int *oldYFloor = new (std::nothrow) int[newHeight];
        // double *yCeilWeight = new (std::nothrow) double[newHeight];
        // double *yFloorWeight = new (std::nothrow) double[newHeight];
        // if (oldYCeil == nullptr || oldYFloor == nullptr || yCeilWeight == nullptr || yFloorWeight == nullptr)
        // {
        //     errorProblem = MEMNEWERROR;
        //     errorMatP = nullptr;
        //     return RYU_FAILURE;
        // }

        // for (int i = 1; i <= newHeight; i++)
        // {
        //     double floatCoordinate = i * yShrinkTimes;
        //     oldYCeil[i - 1] = (int)ceil(floatCoordinate);
        //     oldYFloor[i - 1] = (int)floor(floatCoordinate);
        //     yCeilWeight[i - 1] = floatCoordinate - oldYFloor[i - 1];
        //     yFloorWeight[i - 1] = 1 - yCeilWeight[i - 1];
        // }

        // double *addBuffer = new (std::nothrow) double[valueNumber]{0};
        // if (addBuffer == nullptr)
        // {
        //     errorProblem = MEMNEWERROR;
        //     errorMatP = nullptr;
        //     return RYU_FAILURE;
        // }

        // Calculate
        for (int row = 0; row < newHeight; row++)
        {
            long long rowIndex = row * newWidth;

            // long long ceilRowIndex = oldYCeil[row] * newWidth;
            // long long floorRowIndex = oldYFloor[row] * newWidth;
            double yfloatCoordinate = (row + 1) * yShrinkTimes;
            long long yceilIndex = (int)ceil(yfloatCoordinate) * oldWidth;
            long long yfloorIndex = (int)floor(yfloatCoordinate) * oldWidth;

            double yCeilW = yfloatCoordinate - (int)floor(yfloatCoordinate);
            double yFloorW = 1 - yCeilW;

            // double yCeilW = yCeilWeight[row];
            // double yFloorW = yFloorWeight[row];
            for (int x = 0; x < newWidth; x++)
            {
                double xfloatCoordinate = (x + 1) * xShrinkTimes;
                long long xceilIndex = (int)ceil(xfloatCoordinate);
                long long xfloorIndex = (int)floor(xfloatCoordinate);

                double xCeilW = xfloatCoordinate - xfloorIndex;
                double xFloorW = 1 - xCeilW;

                double xInterpolation1 = source->ascContent[yceilIndex + xceilIndex] * xCeilW + source->ascContent[yceilIndex + xfloorIndex] * xFloorW;
                double xInterpolation2 = source->ascContent[yfloorIndex + xceilIndex] * xCeilW + source->ascContent[yfloorIndex + xfloorIndex] * xFloorW;

                // addBuffer[rowIndex + x] += yCeilW * xInterpolation1;
                // addBuffer[rowIndex + x] += yFloorW * xInterpolation2;
                double addBuffer = 0;
                addBuffer += yCeilW * xInterpolation1;
                addBuffer += yFloorW * xInterpolation2;

                destination->ascContent[rowIndex + x] = (int)(addBuffer+0.5);
                // addBuffer[rowIndex + x] += xInterpolation[ceilRowIndex + x] * yCeilW;
                // addBuffer[rowIndex + x] += xInterpolation[floorRowIndex + x] * yFloorW;
            }
        }

        // for (int i = 0; i < valueNumber; i++)
        // {
        //     destination->ascContent[i] = (int)round(addBuffer[i]);
        // }

        //LUT
        // delete[] oldXFloor;
        // delete[] oldXCeil;
        // delete[] xCeilWeight;
        // delete[] xFloorWeight;
        // delete[] xInterpolation;

        // delete[] oldYFloor;
        // delete[] oldYCeil;
        // delete[] yCeilWeight;
        // delete[] yFloorWeight;
        // delete[] addBuffer;

        return RYU_SUCCESS;
    }

    int rotation(const mat *source, mat *destination, const char *cangle)
    {
        int format = source->getFormat();
        if (format != 2)
        {
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        double angle = atof(cangle);

        double rad = angle * M_PI / 180;
        double cosAngle = cosf(rad);
        double sinAngle = sinf(rad);

        int width = source->getWidth();
        int hight = source->getHight();

        int newWidth = (int)(width * fabsf(cosAngle) + hight * fabsf(sinAngle)+0.5);
        int newHeight = (int)(width * fabsf(sinAngle) + hight * fabsf(cosAngle)+0.5);

        if (destination->initialize(format, newWidth, newHeight, source->getColordepth()) != RYU_MAT_SUCCESS)
        {
            errorProblem = DESTMATINTERROR;
            errorMatP = destination;
            return RYU_FAILURE;
        }

        int xOldOffset = width / 2;
        int yOldOffset = hight / 2;

        int xNewOffset = newWidth / 2;
        int yNewOffset = newHeight / 2;

        // for (int y = 0; y < hight; y++)
        // {
        //     long long rowOffset = y * width;
        //     for (int x = 0; x < width; x++)
        //     {
        //         int xOld = x - xOldOffset;
        //         int yOld = y - yOldOffset;

        //         int xNew = (int)round(xOld * cosAngle - yOld * sinAngle + xNewOffset);
        //         int yNew = (int)round(xOld * sinAngle + yOld * cosAngle + yNewOffset);

        //         if (xNew < 0)
        //             xNew = 0;
        //         if (xNew >= newWidth)
        //             xNew = newWidth - 1;
        //         if (yNew < 0)
        //             yNew = 0;
        //         if (yNew >= newHeight)
        //             yNew = newHeight - 1;

        //         int newOffset = xNew + newWidth * yNew;

        //         destination->ascContent[newOffset] = source->ascContent[rowOffset + x];
        //     }
        // }

        for (int y = 0; y < newHeight; y++)
        {
            long long rowOffset = y * newWidth;
            for (int x = 0; x < newWidth; x++)
            {
                // int xOld = x - xOldOffset;
                // int yOld = y - yOldOffset;

                // int xNew = (int)round(xOld * cosAngle - yOld * sinAngle + xNewOffset);
                // int yNew = (int)round(xOld * sinAngle + yOld * cosAngle + yNewOffset);

                int xNew = x - xNewOffset;
                int yNew = y - yNewOffset;

                double xOldF = xNew * cosAngle + yNew * sinAngle + xOldOffset;
                double yOldF = yNew * cosAngle - xNew * sinAngle + yOldOffset;

                int xCeil = (int)ceil(xOldF);
                int xFloor = (int)floor(xOldF);
                if (xFloor < 0 || xCeil >= width)
                    continue;

                int yCeil = (int)ceil(yOldF);
                int yFloor = (int)floor(yOldF);
                if (yFloor < 0 || yCeil >= hight)
                    continue;

                double xCeilW = xOldF - xFloor;
                double xFloorW = 1 - xCeilW;
                double yCeilW = yOldF - yFloor;
                double yFloorW = 1 - yCeilW;

                double add1 = source->ascContent[yCeil * width + xCeil] * xCeilW + source->ascContent[yCeil * width + xFloor] * xFloorW;
                double add2 = source->ascContent[yFloor * width + xCeil] * xCeilW + source->ascContent[yFloor * width + xFloor] * xFloorW;

                double addBuffer = add1 * yCeilW + add2 * yFloorW;
                destination->ascContent[rowOffset + x] = (int)(addBuffer+0.5);
                // destination->ascContent[newOffset] = source->ascContent[rowOffset + x];
            }
        }
        return RYU_SUCCESS;
    }

    int convolution(const mat *source, mat *destination, int core[9])
    {
        int format = source->getFormat();
        if (format != 2)
        {
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        int width = source->getWidth();
        int hight = source->getHight();
        if (destination->initialize(format, width, hight, source->getColordepth()) != RYU_MAT_SUCCESS)
        {
            errorProblem = DESTMATINTERROR;
            errorMatP = destination;
            return RYU_FAILURE;
        }

        int scale = 0;
        for (int i = 0; i < 9; i++)
        {
            scale += core[i];
        }
        bool needNormalizing = scale == 0 ? false : true;

        for (int y = 0; y < hight; y++)
        {
            long long rowOffset = width * y;
            for (int x = 0; x < width; x++)
            {
                long long rowArroundIndex[9] = {0};
                rowArroundIndex[0] = rowOffset + x - width - 1;
                rowArroundIndex[1] = rowOffset + x - width;
                rowArroundIndex[2] = rowOffset + x - width + 1;
                rowArroundIndex[3] = rowOffset + x - 1;
                rowArroundIndex[4] = rowOffset + x;
                rowArroundIndex[5] = rowOffset + x + 1;
                rowArroundIndex[6] = rowOffset + x + width - 1;
                rowArroundIndex[7] = rowOffset + x + width;
                rowArroundIndex[8] = rowOffset + x + width + 1;

                int whetherEdge[9] = {0};
                if (y == 0)
                {
                    whetherEdge[0] = 1;
                    whetherEdge[1] = 1;
                    whetherEdge[2] = 1;
                }
                if (y == hight - 1)
                {
                    whetherEdge[6] = 1;
                    whetherEdge[7] = 1;
                    whetherEdge[8] = 1;
                }
                if (x == 0)
                {
                    whetherEdge[0] = 1;
                    whetherEdge[3] = 1;
                    whetherEdge[6] = 1;
                }
                if (x == width - 1)
                {
                    whetherEdge[2] = 1;
                    whetherEdge[5] = 1;
                    whetherEdge[8] = 1;
                }

                int scaleNum = 0;
                int valueSum = 0;
                for (int i = 0; i < 9; i++)
                {
                    scaleNum += (1 - whetherEdge[i]) * core[i];
                    if (whetherEdge[i] == 0)
                    {
                        valueSum += core[i] * source->ascContent[rowArroundIndex[i]];
                    }
                }
                destination->ascContent[rowOffset + x] += abs(valueSum);

                if (needNormalizing)
                {
                    destination->ascContent[rowOffset + x] = (unsigned int)(0.5+destination->ascContent[rowOffset + x] / (double)abs(scaleNum));
                }
            }
        }
        return RYU_SUCCESS;
    }

    int smooth(const mat *source, mat *destination, const char *cweight)
    {
        int weight = atoi(cweight);
        int format = source->getFormat();
        if (format != 2)
        {
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        if (weight < 0 || weight > 200)
        {
            errorProblem = PARAMETERERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        int core[9] = {1, 1, 1, 1, weight, 1, 1, 1, 1};
        convolution(source, destination, core);
        return RYU_SUCCESS;
    }

    int sobel(const mat *source, mat *destination)
    {
        int format = source->getFormat();
        if (format != 2)
        {
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        int gxCore[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
        int gyCore[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

        mat *gx = new (std::nothrow) mat;
        mat *gy = new (std::nothrow) mat;
        if (gx == nullptr || gy == nullptr)
        {
            errorProblem = MEMNEWERROR;
            errorMatP = nullptr;
            return RYU_FAILURE;
        }

        convolution(source, gx, gxCore);
        convolution(source, gy, gyCore);

        if (destination->initialize(source->getFormat(), source->getWidth(), source->getHight(), source->getColordepth()) != RYU_MAT_SUCCESS)
        {
            errorProblem = DESTMATINTERROR;
            errorMatP = destination;
            return RYU_FAILURE;
        }

        int scale = 4;
        for (long long i = 0; i < destination->getValueNum(); i++)
        {
            destination->ascContent[i] = (int)(0.5+sqrt(gx->ascContent[i] * gx->ascContent[i] + gy->ascContent[i] * gy->ascContent[i]) / ((double)scale * sqrt(2)));
        }
        delete gx;
        delete gy;
        return RYU_SUCCESS;
    }

    int laplacian(const mat *source, mat *destination)
    {
        int format = source->getFormat();
        if (format != 2)
        {
            errorProblem = FORMATERROR;
            errorMatP = source;
            return RYU_FAILURE;
        }
        int core[9] = {1, 1, 1, 1, -8, 1, 1, 1, 1};
        convolution(source, destination, core);

        double scale = 8;
        for (long long i = 0; i < destination->getValueNum(); i++)
        {
            destination->ascContent[i] = (unsigned int)(0.5+destination->ascContent[i] / scale);
        }
        return RYU_SUCCESS;
    }
} // namespace ryu

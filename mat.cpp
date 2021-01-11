
#include "mat.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <cstring>

using namespace ryu;

mat::mat(const char *path)
{
    cfilePath = path;
    if (readFile() != RYU_MAT_SUCCESS)
        statusCheck();
}
mat::mat(const ryu::mat &copymat)
{
    // std::cout << "copy run" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        property[i] = copymat.property[i];
    }
    pixelNum = copymat.pixelNum;
    colorNum = copymat.colorNum;
    srcValueNumber = copymat.srcValueNumber;

    delete[] ascContent;
    ascContent = new (std::nothrow) unsigned int[srcValueNumber];
    if (ascContent == nullptr)
    {
        status = status | MATMEMNEWERROR;
    }
    else
    {
        //deep copy
        memcpy(ascContent, copymat.ascContent, srcValueNumber * sizeof(int));
    }
}
void mat::operator=(const mat &copymat)
{
    for (size_t i = 0; i < 4; i++)
    {
        property[i] = copymat.property[i];
    }
    pixelNum = copymat.pixelNum;
    colorNum = copymat.colorNum;
    srcValueNumber = copymat.srcValueNumber;

    delete[] ascContent;
    ascContent = new (std::nothrow) unsigned int[srcValueNumber];
    if (ascContent == nullptr)
    {
        status = status | MATMEMNEWERROR;
    }
    else
    {
        //deep copy
        memcpy(ascContent, copymat.ascContent, srcValueNumber * sizeof(int));
    }
}
mat::mat(void)
{
}

mat::~mat()
{
    delete[] ascContent;
}
int mat::initialize(int format, int width, int high, int colordepth)
{
    if (format < 1 || format > 3 || width < 1 || high < 1)
    {
        // std::cout << "Property error!" << std::endl;
        status = status | PROPERTYERROR;
        return RYU_MAT_FAILURE;
    }
    property[0] = format;
    property[1] = width;
    property[2] = high;
    if (format != 1 && format != 4)
    {
        if (colordepth < 1)
        {
            // std::cout << "initialize property colordepth error!" << std::endl;
            status = status | PROPERTYERROR;
            return RYU_MAT_FAILURE;
        }
        property[3] = colordepth;
    }

    pixelNum = property[1] * property[2];
    colorNum = property[0] == 3 || property[0] == 6 ? 3 : 1;
    srcValueNumber = pixelNum * colorNum;

    delete[] ascContent;
    ascContent = new (std::nothrow) unsigned int[srcValueNumber]{0};
    if (ascContent == nullptr)
    {
        status = status | MATMEMNEWERROR;
        return RYU_MAT_FAILURE;
    }
    return RYU_MAT_SUCCESS;
}
void mat::printInfor() const
{
    if (this->ascContent == nullptr)
    {
        std::cout << "Empty image instance!" << std::endl;
        return;
    }
    std::cout << "This image is P" << property[0] << " Image" << std::endl;
    std::cout << property[1] << "x" << property[2] << std::endl;
    if (property[3] != 0)
        std::cout << "Color depth: " << property[3] << std::endl;
}

int mat::readFile()
{
    std::ifstream fileStream;

    fileStream.open(cfilePath, std::ios::in | std::ios::binary);
    if (!fileStream.is_open())
    {
        status = status | OPENERROR;
        return RYU_MAT_FAILURE;
    }

    std::string format;
    std::string line;
    fileStream >> format;
    if (format.length() != 2 || format.find('P') != 0)
    {
        status = status | HEADERROR;
        return RYU_MAT_FAILURE;
    }

    int propertyIndex = 0;
    int type = format.c_str()[1] - 48;
    if (type > 6 || type < 1)
    {
        status = status | HEADERROR;
        return RYU_MAT_FAILURE;
    }
    property[propertyIndex++] = type;
    property[3] = 0;
    int colorDepthExist = (type == 1 || type == 4) ? 3 : 4;

    //To delete annotation
    while (propertyIndex < colorDepthExist)
    {
        std::getline(fileStream, line);
        std::size_t annotationPos = line.find('#');
        if (annotationPos != std::string::npos)
        {
            line = line.substr(0, annotationPos);
        }

        //If the all characters in this line is annotation
        //To judge the string whether has property value or just space chars line.
        bool isBlankString = true;
        for (std::string::size_type i = 0; i < line.length(); i++)
        {
            if (isdigit(line.c_str()[i]))
            {
                isBlankString = false;
                break;
            }
        }
        if (isBlankString)
        {
            continue;
        }
        std::stringstream sstr(line);
        while (!sstr.eof())
        {
            sstr >> property[propertyIndex++];
        }
        if (sstr.fail() || propertyIndex > colorDepthExist)
        {
            status = status | HEADERROR;
            return RYU_MAT_FAILURE;
        }
    }

    pixelNum = property[1] * property[2];
    colorNum = property[0] == 3 || property[0] == 6 ? 3 : 1;
    srcValueNumber = pixelNum * colorNum;

    ascContent = new unsigned int[srcValueNumber];
    if (ascContent == nullptr)
    {
        status = status | MATMEMNEWERROR;
        return RYU_MAT_FAILURE;
    }
    // For binary file.
    if (property[0] > 3)
    {
        char *binBuffer = nullptr;
        if (property[0] == 4)
        {

            // int srcValueSize = pixelNum;
            int srcValueSize = pixelNum * colorNum / 8;
            binBuffer = new char[srcValueSize];
            fileStream.read(binBuffer, srcValueSize);

            std::streampos pos = fileStream.tellg();

            fileStream.seekg(0, std::ios::end);
            std::streampos endpos = fileStream.tellg();
            if (!fileStream.good())
            {
                status = status | NOTMATCH;
                return RYU_MAT_FAILURE;
            }
            // std::cout<<endpos-pos<<std::endl;

            char buffer;
            for (long long i = 0, index = 0; i < srcValueSize && index < srcValueNumber;i++)
            {
                memset(&buffer, 0, sizeof(char));
                memcpy(&buffer, &binBuffer[i], 1);

                // ascContent[i++]=buffer;
                for (int j = 0; j < 8 && index < srcValueNumber; j++)
                {

                    // if ((buffer & 0b00000001) == 1)
                    if ((buffer & 0b10000000) != 0)
                        ascContent[index++] = 1;
                    else
                        ascContent[index++] = 0;
                    buffer = buffer << 1;
                    // buffer = buffer >> 1;
                }
            }
        }

        if (property[0] > 4)
        {
            int valueSize = ceil(property[3] / 255);

            int srcValueSize = valueSize * colorNum * pixelNum;

            binBuffer = new char[srcValueSize];
            fileStream.read(binBuffer, srcValueSize);

            std::streampos pos = fileStream.tellg();

            fileStream.seekg(0, std::ios::end);
            std::streampos endpos = fileStream.tellg();
            if (!fileStream.good())
            {
                status = status | NOTMATCH;
                return RYU_MAT_FAILURE;
            }

            int *buffer = new int;
            for (long long i = 0; i < srcValueNumber; i = i + valueSize)
            {
                memset(buffer, 0, sizeof(int));
                memcpy(buffer, &binBuffer[i], valueSize);
                ascContent[i] = *buffer;
            }
        }
        delete[] binBuffer;
    }
    else // For asc file.
    {
        ascContent = new unsigned int[srcValueNumber];
        long long index = 0;
        while (!fileStream.eof() && index < srcValueNumber)
        {
            fileStream >> ascContent[index++];
        }
        if (index != srcValueNumber)
        {
            status = status | NOTMATCH;
            return RYU_MAT_FAILURE;
        }
    }

    property[0] = property[0] < 4 ? property[0] : property[0] - 3;
    return RYU_MAT_SUCCESS;
}

int mat::writeAscFile(const char *inoutputPath)
{
    //write head
    std::string outputpath;
    //default output file name
    if (inoutputPath == nullptr)
    {
        outputpath = "output.";
        switch (property[0])
        {
        case 1:
        case 4:
            outputpath += "pbm";
            break;
        case 2:
        case 5:
            outputpath += "pgm";
            break;
        case 3:
        case 6:
            outputpath += "ppm";
            break;
        }
    }
    else
    {
        outputpath = inoutputPath;
    }
    std::ofstream outFile;
    outFile.open(outputpath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!outFile.is_open())
    {
        status = status | OUTPATHERROR;
        return RYU_MAT_FAILURE;
    }

    int format = property[0];
    outFile << "P" << format << "\n"
            << property[1] << " " << property[2] << "\n";
    if (property[0] != 1 && property[0] != 4)
        outFile << property[3] << "\n";
    if (outFile.fail())
    {
        status = status | WRITEERROR;
        return RYU_MAT_FAILURE;
    }

    //write content
    int newRow = 0;
    for (long long index = 0; index < srcValueNumber; index++)
    {
        outFile << ascContent[index];
        if (++newRow == 16)
        {
            outFile << '\n';
            newRow = 0;
        }
        else
        {
            outFile << ' ';
        }
    }
    if (!outFile.good())
    {
        status = status | WRITEERROR;
        return RYU_MAT_FAILURE;
    }
    if (statusCheck() != RYU_MAT_SUCCESS)
    {
        exit(0);
    }
    return RYU_MAT_SUCCESS;
}
int mat::writeBinFile(const char *inoutputPath)
{
    //write head
    std::string outputpath;
    //default output file name
    if (inoutputPath == nullptr)
    {
        outputpath = "output.";
        switch (property[0])
        {
        case 1:
        case 4:
            outputpath += "pbm";
            break;
        case 2:
        case 5:
            outputpath += "pgm";
            break;
        case 3:
        case 6:
            outputpath += "ppm";
            break;
        }
    }
    else
    {
        outputpath = inoutputPath;
    }
    std::ofstream outFile;
    outFile.open(outputpath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!outFile.is_open())
    {
        status = status | OUTPATHERROR;
        return RYU_MAT_FAILURE;
    }
    if (!outFile.is_open())
    {
        status = status | OUTPATHERROR;
        return RYU_MAT_FAILURE;
    }
    int format = property[0];
    outFile << "P" << format + 3 << "\n"
            << property[1] << " " << property[2] << "\n";

    bool regularImage;
    if (format != 1 && format != 4)
    {
        regularImage = property[3] % 255 == 0 ? true : false;
        int byteWidth = (property[3] / 255) + 0.5;
        // Cant due with deepth color image
        if (byteWidth > 1)
        {
            status |= WRITEERROR;
            return RYU_MAT_FAILURE;
        }
        int normalizedDeepth = byteWidth * 255;
        outFile << normalizedDeepth << "\n";
    }
    if (outFile.fail())
    {
        status = status | WRITEERROR;
        return RYU_MAT_FAILURE;
    }

    //write binary content
    if (format == 1)
    {
        int bin = 0;
        char writeByte;
        for (long long index = 0; index < srcValueNumber; index++)
        {
            if (ascContent[index] > 1)
            {
                status |= VALUEDIFFTYPE;
                return RYU_MAT_FAILURE;
            }
            writeByte |= ascContent[index];
            if (++bin == 8)
            {
                bin = 0;
                outFile.write(&writeByte, 1);
                writeByte = 0;
            }
            else
            {
                writeByte <<= 1;
            }
            
        }
        if (bin != 0)
        {
            while (bin != 8)
            {
                bin++;
                writeByte <<= 1;
            }
            outFile.write(&writeByte, 1);
        }
    }
    else
    {
        char writeByte;
        if (regularImage)
        {
            for (long long index = 0; index < srcValueNumber; index++)
            {
                writeByte = (char)ascContent[index];
                outFile.write(&writeByte, 1);
            }
        }
        else
        {
            double shrink = 255 / property[3];
            for (long long index = 0; index < srcValueNumber; index++)
            {
                writeByte = (char)(ascContent[index] * shrink);
                outFile.write(&writeByte, 1);
            }
        }
    }

    if (!outFile.good())
    {
        status = status | WRITEERROR;
        return RYU_MAT_FAILURE;
    }

    if (statusCheck() != RYU_MAT_SUCCESS)
    {
        exit(0);
    }
    return RYU_MAT_SUCCESS;
}
int mat::statusCheck() const
{
    if (status == 0)
    {
        // std::cout << "Execute success!" << std::endl;
        return RYU_MAT_SUCCESS;
    }

    if ((status & OPENERROR) != 0)
        std::cout << "Source file don't exist." << std::endl;
    if ((status & HEADERROR) != 0)
        std::cout << "Source file has illegal head." << std::endl;
    if ((status & NOTMATCH) != 0)
        std::cout << "The number of pixel value has not match with image resolution." << std::endl;
    if ((status & OUTPATHERROR) != 0)
        std::cout << "Output destination path error." << std::endl;
    if ((status & WRITEERROR) != 0)
        std::cout << "Write process error." << std::endl;
    if ((status & MATMEMNEWERROR) != 0)
        std::cout << "Memory allocation error." << std::endl;
    if ((status & PROPERTYERROR) != 0)
        std::cout << "Initialize property error." << std::endl;
    if ((status & VALUEDIFFTYPE) != 0)
        std::cout << "Image's pixel value and image's type do not match" << std::endl;
    return RYU_MAT_FAILURE;
}

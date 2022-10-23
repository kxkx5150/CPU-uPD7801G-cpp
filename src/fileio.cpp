
#include "fileio.h"
#include "utils/windef.h"


FILEIO::FILEIO()
{
    fp = NULL;
}

FILEIO::~FILEIO(void)
{
    if (fp != NULL)
        Fclose();
}

bool FILEIO::IsProtected(std::string filename)
{
    return false;
}
bool FILEIO::Fopen(std::string filename, int mode)
{
    Fclose();

    switch (mode) {
        case FILEIO_READ_BINARY:
            return ((fp = _tfopen(filename.c_str(), ("rb"))) == NULL) ? false : true;
        case FILEIO_WRITE_BINARY:
            return ((fp = _tfopen(filename.c_str(), ("wb"))) == NULL) ? false : true;
        case FILEIO_READ_WRITE_BINARY:
            return ((fp = _tfopen(filename.c_str(), ("r+b"))) == NULL) ? false : true;
        case FILEIO_READ_ASCII:
            return ((fp = _tfopen(filename.c_str(), ("r"))) == NULL) ? false : true;
        case FILEIO_WRITE_ASCII:
            return ((fp = _tfopen(filename.c_str(), ("w"))) == NULL) ? false : true;
        case FILEIO_READ_WRITE_ASCII:
            return ((fp = _tfopen(filename.c_str(), ("r+w"))) == NULL) ? false : true;
    }
    return false;
}

void FILEIO::Fclose()
{
    if (fp)
        fclose(fp);
    fp = NULL;
}

uint32 FILEIO::Fread(void *buffer, uint32 size, uint32 count)
{
    return fread(buffer, size, count, fp);
}

uint32 FILEIO::Fwrite(void *buffer, uint32 size, uint32 count)
{
    return fwrite(buffer, size, count, fp);
}

uint32 FILEIO::Fseek(long offset, int origin)
{
    switch (origin) {
        case FILEIO_SEEK_CUR:
            return fseek(fp, offset, SEEK_CUR);
        case FILEIO_SEEK_END:
            return fseek(fp, offset, SEEK_END);
        case FILEIO_SEEK_SET:
            return fseek(fp, offset, SEEK_SET);
    }
    return 0xFFFFFFFF;
}

uint32 FILEIO::Ftell()
{
    return ftell(fp);
}

#ifndef GENERALFUNC_H
#define GENERALFUNC_H

unsigned short Frame_FCS_CRC(unsigned char d[], unsigned int len)
{
    char b = 0;
    unsigned short crc = 0xFFFF;
    int i = 0;
    int j = 0;
    for (i = 0; i < len; i++)
    {
        for (j = 0; j < 8; j++)
        {
            b = ((d[i] << j) & 0x80) ^ ((crc & 0x8000) >> 8);
            crc <<= 1;
            if (b != 0)
            {
                crc ^= 0x1021;
            }
        }
    }
    return (crc);
}

#endif // GENERALFUNC_H

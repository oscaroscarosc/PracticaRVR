#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
	int32_t total = 80*sizeof(char) + 2*sizeof(int16_t);
	alloc_data(total);

	char* tmp = _data + sizeof(int32_t);

	memcpy(tmp,name,80);
	tmp += 80*sizeof(char);
	memcpy(tmp,&x,sizeof(int16_t));

	tmp += sizeof(int16_t);
	memcpy(tmp,&y,sizeof(int16_t));
	
    }

    int from_bin(char * data)
    {
	char* tmp = data + sizeof(int32_t);

	memcpy(name,tmp,80*sizeof(char));

	tmp += 80;
	memcpy(&x,tmp,sizeof(int16_t));

	tmp += sizeof(int16_t);
	memcpy(&y,tmp,sizeof(int16_t));

    }

public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
	char buff[256];
    int fd = open("ej1.txt", O_RDONLY);

    read(fd, buff, sizeof(buff));

    close(fd);

    Jugador one_ ("",0,0);
    one_.from_bin(buff);
    std::cout << "N: " << one_.name << "\nX: " << one_.x << "\nY: " << one_.y << "\n";

}

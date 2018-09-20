#ifndef __FLUX__PORT_H
#define __FLUX__PORT_H

#include <stdint.h>

class Port{
    protected:
        uint16_t portnr;
        Port(uint16_t portnr);
        ~Port();
};

class Port8b : public Port{
    public:
        Port8b(uint16_t portnr);
        ~Port8b();
        virtual void write(uint8_t data);
        virtual uint8_t read();
};

class Port8bSlow : public Port8b{
    public:
        Port8bSlow(uint16_t portnr);
        ~Port8bSlow();
        virtual void write(uint8_t data);
};

class Port16b : public Port{
    public:
        Port16b(uint16_t portnr);
        ~Port16b();
        virtual void write(uint16_t data);
        virtual uint16_t read();
};

class Port32b : public Port{
    public:
        Port32b(uint16_t portnr);
        ~Port32b();
        virtual void write(uint32_t data);
        virtual uint32_t read();
};

#endif
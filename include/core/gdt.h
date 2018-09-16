#ifndef __FLUX__GDT_H
#define __FLUX__GDT_H

#include <common/types.h>

class GlobalDescriptorTable{
    public:
        class Segment{
            private:
                uint16_t limit_lo;
                uint16_t base_lo;

                uint8_t base_hi;
                uint8_t type;
                uint8_t limit_hi;
                uint8_t base_vhi;

            public:
                Segment(uint32_t base, uint32_t limit, uint8_t type);
                uint32_t getBase(); //returns pointer
                uint32_t getLimit(); //returns limit
        } __attribute__((packed)); //prevents compiler from moving stuff around

        Segment nullSegment;        
        Segment unusedSegment;        
        Segment codeSegment;        
        Segment dataSegment;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        uint16_t getCodeSegmentOffset(); //returns offset of the code segment
        uint16_t getDataSegmentOffset(); //returns offset of the data segment

};

#endif
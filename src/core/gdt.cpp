#include <core/gdt.h>

GlobalDescriptorTable::GlobalDescriptorTable() : nullSegment (0,0,0), unusedSegment (0,0,0), codeSegment (0,64*1024*1024,0x9A), dataSegment (0,64*1024*1024,0x92){
    uint32_t i[2]; //CPU expects 6 bytes; two last bytes of i[0] stay unused
    i[0] = sizeof(GlobalDescriptorTable) << 16; //we want the high bytes; last two bytes of i[0] stay unused
    i[1] = (uint32_t) this; //address of the table itself

    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2)); //tells the CPU to load the gdt
}

GlobalDescriptorTable::~GlobalDescriptorTable(){}

uint16_t GlobalDescriptorTable::getDataSegmentOffset(){
    return (uint8_t*)&dataSegment - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::getCodeSegmentOffset(){
    return (uint8_t*)&codeSegment - (uint8_t*)this;
}

GlobalDescriptorTable::Segment::Segment(uint32_t base, uint32_t limit, uint8_t flags){
    uint8_t* target = (uint8_t*)this;

    if(limit <= 65536){ //16 bit limit
        target[6] = 0x40; 
    }else{
        if((limit & 0xFFF) != 0xFFF){
            limit = (limit >> 12) - 1;
        }else{
            limit = limit >> 12;
        }

        target[6] = 0xC0;
    }


    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;//We can only set the lo 4 bits because we have already set the hi 4 bits

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Type
    target[5] = flags;
}

uint32_t GlobalDescriptorTable::Segment::getBase()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

uint32_t GlobalDescriptorTable::Segment::getLimit()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0){
        result = (result << 12) | 0xFFF;
    }

    return result;
}

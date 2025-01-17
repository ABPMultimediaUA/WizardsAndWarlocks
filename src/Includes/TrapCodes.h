#ifndef TRAPCODES_H
#define TRAPCODES_H

/*
Trampas
- Garras de la Muerte
- Ánimas del Inframundo
- Silencio Espectral
- Impuestos de Caronte
- Trastorno del Ser
*/

enum TrapEnum{
    TENUM_NO_TRAP = -1,
    TENUM_DEATH_CLAWS = 0x00,
    TENUM_SPIRITS = 0x01,
    TENUM_SILENCE = 0x02,
    TENUM_TAXES = 0x03,
    TENUM_DISTURBANCE = 0x04,
    TENUM_EXPLOSIVE = 0x05
};

#endif

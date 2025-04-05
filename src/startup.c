#include <stdint.h>

void _reset() {
    extern uint32_t _sdata, _edata, _si_data, _sbss, _ebss;
    for (uint32_t *dst = &_sdata, *src = &_si_data; dst < &_edata;) {
        *dst++ = *src++;
    }
    for (uint32_t *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    extern int main();
    main();
}

void __attribute__((weak)) default_handler(void) {
    while (1) asm("nop");
}
#define WEAK_DEFAULT __attribute__((weak, alias("default_handler")))

void _null() WEAK_DEFAULT;

void _nmi() WEAK_DEFAULT;
void _hardfault() WEAK_DEFAULT;
void _memory() WEAK_DEFAULT;
void _busfault() WEAK_DEFAULT;
void _usagefault() WEAK_DEFAULT;
// reserved
// reserved
// reserved
// reserved
// reserved
void _svcall() WEAK_DEFAULT;
void _debugmonitor() WEAK_DEFAULT;
// reserved
void _pendsv() WEAK_DEFAULT;
extern void _systick();

extern void _estack();
__attribute__((section(".vector_table"))) void (*const table[16 + 96])() = {
    _estack, _reset,   _nmi,  _hardfault, _memory, _busfault,     _usagefault,
    0,       0,        0,     0,          _svcall, _debugmonitor, 0,
    _pendsv, _systick,

    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,      _null,   _null,         _null,
    _null,   _null,    _null, _null,

};

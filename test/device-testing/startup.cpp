#include <algorithm>
#include <cstdint>

extern "C" void __libc_init_array();

extern std::uintptr_t _sbss;
extern std::uintptr_t _ebss;

extern std::uintptr_t _sidata;

extern std::uintptr_t _sdata;
extern std::uintptr_t _edata;

extern "C" [[noreturn]] void __reset_handler() {
    /* Set the stack pointer */
    asm("ldr   r0, =_estack");
    asm("mov   sp, r0 ");

    // todo init clock and check boot region

    /* Copy the data segment initializers from flash to SRAM */
    const std::size_t cnt = (&_edata - &_sdata);
    std::copy(&_sidata, &_sidata + cnt, &_sdata);

    /* Zero fill the bss segment. */
    std::fill(&_sbss, &_ebss, 0U);

    /* Call static constructors */
    __libc_init_array();

    /* Call the application's entry point.*/
    asm("b main");

    while (true) {}
}
#include <cstdint>

extern std::uintptr_t _estack;

extern "C" [[noreturn]] void __default_handler();

extern "C" void __reset_handler();
extern "C" void NMI_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void HardFault_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void MemManage_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void BusFault_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void UsageFault_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void SVC_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void DebugMon_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void PendSV_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void SysTick_Handler() __attribute__((weak, alias("__default_handler")));
extern "C" void WWDG_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void PVD_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TAMP_STAMP_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void RTC_WKUP_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void FLASH_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void RCC_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI0_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI1_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI2_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI3_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI4_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream0_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream1_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream2_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream3_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream4_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream5_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream6_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void ADC_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI9_5_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM1_BRK_TIM9_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM1_UP_TIM10_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM1_CC_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM2_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM3_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM4_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void I2C1_EV_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void I2C1_ER_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void I2C2_EV_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void I2C2_ER_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void SPI1_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void SPI2_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void USART1_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void USART2_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void EXTI15_10_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void RTC_Alarm_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void OTG_FS_WKUP_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA1_Stream7_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void SDIO_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void TIM5_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void SPI3_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream0_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream1_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream2_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream3_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream4_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void OTG_FS_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream5_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream6_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void DMA2_Stream7_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void USART6_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void I2C3_EV_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void I2C3_ER_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void FPU_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void SPI4_IRQHandler() __attribute__((weak, alias("__default_handler")));
extern "C" void SPI5_IRQHandler() __attribute__((weak, alias("__default_handler")));

typedef void (*const pHandler)();

__attribute__((section(".isr_vector"), used)) pHandler vectors[] =
        {
                (pHandler) &_estack,
                __reset_handler,
                NMI_Handler,
                HardFault_Handler,
                MemManage_Handler,
                BusFault_Handler,
                UsageFault_Handler,
                0,
                0,
                0,
                0,
                SVC_Handler,
                DebugMon_Handler,
                0,
                PendSV_Handler,
                SysTick_Handler,
                WWDG_IRQHandler,
                PVD_IRQHandler,
                TAMP_STAMP_IRQHandler,
                RTC_WKUP_IRQHandler,
                FLASH_IRQHandler,
                RCC_IRQHandler,
                EXTI0_IRQHandler,
                EXTI1_IRQHandler,
                EXTI2_IRQHandler,
                EXTI3_IRQHandler,
                EXTI4_IRQHandler,
                DMA1_Stream0_IRQHandler,
                DMA1_Stream1_IRQHandler,
                DMA1_Stream2_IRQHandler,
                DMA1_Stream3_IRQHandler,
                DMA1_Stream4_IRQHandler,
                DMA1_Stream5_IRQHandler,
                DMA1_Stream6_IRQHandler,
                ADC_IRQHandler,
                0,
                0,
                0,
                0,
                EXTI9_5_IRQHandler,
                TIM1_BRK_TIM9_IRQHandler,
                TIM1_UP_TIM10_IRQHandler,
                TIM1_TRG_COM_TIM11_IRQHandler,
                TIM1_CC_IRQHandler,
                TIM2_IRQHandler,
                TIM3_IRQHandler,
                TIM4_IRQHandler,
                I2C1_EV_IRQHandler,
                I2C1_ER_IRQHandler,
                I2C2_EV_IRQHandler,
                I2C2_ER_IRQHandler,
                SPI1_IRQHandler,
                SPI2_IRQHandler,
                USART1_IRQHandler,
                USART2_IRQHandler,
                0,
                EXTI15_10_IRQHandler,
                RTC_Alarm_IRQHandler,
                OTG_FS_WKUP_IRQHandler,
                0,
                0,
                0,
                0,
                DMA1_Stream7_IRQHandler,
                0,
                SDIO_IRQHandler,
                TIM5_IRQHandler,
                SPI3_IRQHandler,
                0,
                0,
                0,
                0,
                DMA2_Stream0_IRQHandler,
                DMA2_Stream1_IRQHandler,
                DMA2_Stream2_IRQHandler,
                DMA2_Stream3_IRQHandler,
                DMA2_Stream4_IRQHandler,
                0,
                0,
                0,
                0,
                0,
                0,
                OTG_FS_IRQHandler,
                DMA2_Stream5_IRQHandler,
                DMA2_Stream6_IRQHandler,
                DMA2_Stream7_IRQHandler,
                USART6_IRQHandler,
                I2C3_EV_IRQHandler,
                I2C3_ER_IRQHandler,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                FPU_IRQHandler,
                0,
                0,
                SPI4_IRQHandler,
                SPI5_IRQHandler};

extern "C" [[noreturn]] void __default_handler() {
    while (1)
        ;
};
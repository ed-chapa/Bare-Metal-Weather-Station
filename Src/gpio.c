#include "gpio.h"

void GPIO_EnableClock(GPIO_TypeDef *port) {
    if (port == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    if (port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    if (port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    if (port == GPIOH) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
}

void GPIO_DisableClock(GPIO_TypeDef *port) {
    if (port == GPIOA) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
    if (port == GPIOB) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
    if (port == GPIOC) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
    if (port == GPIOH) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOHEN;
}

void GPIO_Configure(GPIO_TypeDef *port, GPIO_Configuration *config)
{
    if (!port || !config || config->pin > 15) return;

    // Validate enum ranges
    if (config->mode > GPIO_MODE_ALTERNATE) return;
    if (config->pull > GPIO_PULL_DOWN) return;
    if (config->outputType > GPIO_OTYPE_OPENDRAIN) return;
    if (config->outputSpeed > GPIO_OSPEED_VERYHIGH) return;
    if (config->alternateFunction > GPIO_ALTERNATE_15) return;

    // Set the output type
    port->OTYPER &= ~(1 << config->pin);
    port->OTYPER |= (config->outputType << config->pin);

    // Set the output speed
    port->OSPEEDR &= ~(3 << (config->pin * 2));
    port->OSPEEDR |= (config->outputSpeed << (config->pin * 2));

    // Set pull-up/pull-down
    port->PUPDR &= ~(3 << (config->pin * 2));
    port->PUPDR |= (config->pull << (config->pin * 2));

    // Set alternate function
    if (config->mode == GPIO_MODE_ALTERNATE) {
        if (config->pin < 8) {
            port->AFR[0] &= ~(15 << (config->pin * 4));
            port->AFR[0] |= (config->alternateFunction << (config->pin * 4));
        } else {
            port->AFR[1] &= ~(15 << ((config->pin - 8) * 4));
            port->AFR[1] |= (config->alternateFunction << ((config->pin - 8) * 4));
        }
    }

    // Set the mode
    port->MODER &= ~(3 << (config->pin * 2));
    port->MODER |= (config->mode << (config->pin * 2));
}

void GPIO_Toggle(GPIO_TypeDef *port, GPIO_Pin pin) {
    port->ODR ^= (1 << pin);
}

void GPIO_Set(GPIO_TypeDef *port, GPIO_Pin pin) {
    port->ODR |= (1 << pin);
}

void GPIO_Reset(GPIO_TypeDef *port, GPIO_Pin pin) {
    port->ODR &= ~(1 << pin);
}

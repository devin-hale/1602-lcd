# Traffic Light Controller v0.0.1

A minimal implementation of a traffic light controller simulation using the STM32446ZE and 12 LEDs.

<img src="/repo/image.jpg" alt="image preview" width="450" height="auto">

- Abstracts groups of LEDs as a 'traffic light' interface
- Traffic light controller orchestrates state changes accross traffic lights
- Implements non-blocking finite state machine to evaluate event handling and states
- Utilizes CMSIS-4 headers
- Uses CMAKE as a cross-platform build system
- Redirects printf to onboard UART connection for terminal debugging
- Communicates with 16x2 LCD unit over I2C to provide timer feedback

## Materials
| Product | Quantity |
| ------- | -------- |
| STM32F446ZE MCU | 1 |
| 1602 LCD | 1 |
| Red LED | 4 |
| Green LED | 4 |
| Yellow LED | 4 |
| 220 Ohm Resistor | 12 |

## Takeaways
- Linker flags `-mcpu=cortex-m4` and `-mthumb` need to be set. Setting them on the compiler doesn't necessarily set them on the linker. Lost a lot of time to this.
- I need to get more comfortable with the debugger. Probably one of those things where I should probably suck it up and use the GUI for now.
- Feels like I was just "feeling it out", when building the state machines handling each abstraction, and they each have a slightly different implementation because of it.

# STM32_VS_Code_Project

Short description
This is a CMake-based STM32 firmware project targeting the Nucleo-F767ZI board. The project is organized for STM32CubeMX compatibility and VS Code workflows.

USE rm -rf build for the cloning or move location file

Prerequisites
- STM32CubeMX (for .ioc edits and code regeneration)
- ARM toolchain (GCC/Clang) and CMake
- Ninja (optional, recommended)
- ST-Link / OpenOCD or other flashing/debugging tool

Build (example)
- Configure and generate build files:
  cmake --preset=default -B build/Debug
- Build:
  cmake --build build/Debug

Flash / Debug
- Use external tools (ST-Link, OpenOCD, etc.) to flash and debug the resulting ELF/BIN (not provided by this repo).

Regenerate code after .ioc changes
- Edit `STM32_VS_Code_Project.ioc` in STM32CubeMX and regenerate code. Do not manually overwrite CubeMX-generated regions; use USER CODE blocks for custom code.

Project layout (important files)
- Core/Inc, Core/Src        : Main application code (entry point: `main.c`)
- Drivers/                 : CMSIS and STM32 HAL drivers (vendor code)
- cmake/                   : Custom CMake toolchain and CubeMX integration
- startup_stm32f767xx.s    : Cortex-M startup assembly
- STM32F767XX_FLASH.ld     : Linker script
- STM32_VS_Code_Project.ioc: CubeMX configuration

Coding guidelines (summary)
- English-only comments and ASCII characters
- 4-space indentation, camelCase for identifiers, UPPER_CASE for macros
- Header guards in all headers
- Use static for file-local symbols
- Place custom code between `/* USER CODE BEGIN */` and `/* USER CODE END */` blocks
- Use HAL APIs for hardware access; avoid direct register hacks
- Doxygen-style comments for public functions

Other notes
- Build artifacts are placed in `build/Debug/`
- Follow the .ioc -> CubeMX -> regenerate workflow when changing peripherals
- See `Core/Src/main.c` and `Core/Inc/main.h` as primary entry points

License
- See LICENSE file in the repository root (if present). If none, code is provided AS-IS.

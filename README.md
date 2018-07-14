# How to use

1. Add git submodule to your project
2. Add lph8924-1-config.h to root of your project (use example from lph8924-1-config.example.h)
3. See lph8924-1.h for API and use it. 

# Basic usage
```C
#include <stm32f10x.h>
#include <lph8924-1.h>

void main() {
	lph8924_init();
	lph8924_set_color_mode(LPH8924_COLOR_24BIT);
	lph8924_set_ram_area(0, 0, 131, 175);
	lph8924_set_address_mode(0, 0, 0, 0, 0, 0);
	lph8924_enable(1);
	
	lph8924_write_ram_start(0);
	
	while (1) {
		for (int y = 0; y < 58; ++y) {
			for (int x = 0; x < 132; ++x) {
				lph8924_send_rgb24(0xFF0000);
			}
		}
		
		for (int y = 58; y < 58+58; ++y) {
			for (int x = 0; x < 132; ++x) {
				lph8924_send_rgb24(0x00FF00);
			}
		}
		
		for (int y = 58+58; y < 176; ++y) {
			for (int x = 0; x < 132; ++x) {
				lph8924_send_rgb24(0x0000FF);
			}
		}
	}
}
```

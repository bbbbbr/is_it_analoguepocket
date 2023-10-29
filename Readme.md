
# Is It Analogue Pocket?

This ROM tries to see if the device running a Game Boy or Mega Duck ROM is an Analogue Pocket by inspecting Sound Wave RAM power-on values.

At least for now it appears to detect properly in the following scenarios:
* When not running in cart-header enabled CGB mode
  * Game Boy / Color in Analogue Pocket cartridge slot
  * Game Boy / Color in .pocket format ("GBStudio Creations")
* Game Boy fpga core
* Mega Duck fpga core 

It does not detect for:
* Running in cart-header enabled CGB mode (the wave RAM will be cleared by AP with the standard CGB 0x00 0xFF pattern)
* Game Boy Color fpga core


![FPGA GB](/info/is_it_ap_fpga_gb.jpg)
![FPGA GBC](/info/is_it_ap_fpga_gbc.jpg)
![Real MGB GBC](/info/is_it_ap_real_gbpocket.jpg)
![FPGA MEGADUCK](/info/is_it_ap_fpga_megaduck.jpg)
![Real MEGADUCK](/info/is_it_ap_real_megaduck.jpg)


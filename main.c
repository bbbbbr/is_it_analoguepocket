#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define WAVE_RAM_SIZE 16u
uint8_t wave_ram_save[WAVE_RAM_SIZE];


// From earlier tests, gives different output - maybe writing to wave ram after each read alters it, or there is some compiler ordering issue
//  NR52_REG = 0x80;
//
//  // Init Channel 3 Wave RAM to known values
//  // (Match CGB power-on defaults of alternating 00/FF)
//  for (uint8_t c = 0u; c < 16u; c++) {
//	WAV_RAM_SAVE[c] = AUD3WAVE[c];
//	AUD3WAVE[c] = (c & 1u) ? 0x00u : 0xFFu;
//  }
//

const uint8_t WAVE_RAM_POCKET_DMG[WAVE_RAM_SIZE] = {
// Values when writing alternating 00/FF to wave ram right after reading each byte:
//	0xD8, 0x00, 0xFC, 0x00,
//	0x60, 0x00, 0x12, 0x00,
//	0xFe, 0x00, 0x3B, 0x00,
//	0xDB, 0x00, 0x76, 0x00  };
	0x70, 0xB0, 0xFC, 0xD8,
	0x88, 0xD3, 0x12, 0x60,
	0x62, 0x77, 0x3B, 0xFE,
	0x6D, 0xA9, 0x76, 0xDB,};

const uint8_t WAVE_RAM_POCKET_CGB[WAVE_RAM_SIZE] = {
// Values when writing alternating 00/FF to wave ram right after reading each byte:
//	0x8D, 0x00, 0xE0, 0x00,
//	0x2D, 0x00, 0x79, 0x00,
//	0xBD, 0x00, 0x41, 0x00,
//	0xDB, 0x00, 0x46, 0x00  };
	 0x24, 0xAE, 0xE0, 0x8D,
	 0x80, 0xF6, 0x79, 0x2D,
	 0x09, 0xFF, 0x41, 0xBD,
	 0xC0, 0x76, 0x46, 0xDB, };

#define AP_NO  0
#define AP_DMG 1
#define AP_CGB 2

// #define DEBUG_INFO

bool check_fingerprint(uint8_t * p_tocheck, const uint8_t * p_reference, uint8_t bufsize) {
	#ifdef DEBUG_INFO
		printf("\n");
	#endif
	for (uint8_t c = 0u; c < bufsize; c++) {
		#ifdef DEBUG_INFO
			printf("%hx ", (uint8_t)*p_tocheck);
		#endif
		if (*p_tocheck != *p_reference)
			return false;
		p_tocheck++;
		p_reference++;
	}
	return true;
}

void save_wave_ram(void) {
	// Turn on sound hardware
	NR52_REG = 0x80;

	// Read Channel 3 Wave RAM before anything sets them
	// In CGB they may be initialized, others semi-random
	for (uint8_t c = 0u; c < WAVE_RAM_SIZE; c++) {
		wave_ram_save[c] = AUD3WAVE[c];
	}

	printf("Power-up Wave RAM:\n");
	for (uint8_t c = 0u; c < WAVE_RAM_SIZE; c++) {
		printf("%hx ", wave_ram_save[c]);
		if (((c+1) & 0x03) == 0)
			printf("\n");
	}
	printf("\n");
	waitpadup();
}

// Call at power up
uint8_t is_it_pocket(void) {

	save_wave_ram();

	if (check_fingerprint(wave_ram_save, WAVE_RAM_POCKET_DMG, WAVE_RAM_SIZE))
		return AP_DMG;
	else if (check_fingerprint(wave_ram_save, WAVE_RAM_POCKET_CGB, WAVE_RAM_SIZE))
		return AP_CGB;
	else
		return AP_NO;
}


#ifdef ANALOGUEPOCKET
    #define STATF_PPU_MODE_3 0b11000000
#else
    #define STATF_PPU_MODE_3 0b00000011  
#endif

// Call at power up - This MUST NOT run on DMGs
uint8_t is_it_pocket_gbc(void) {
	uint8_t result = AP_NO;
	
	while(rLY != 141u);
	while(rLY != 142u);
	while((rSTAT & STATF_LCD) != STATF_PPU_MODE_3);
	rLCDC = LCDCF_OFF;
	rBCPS = 0u;
	rBCPD = 0u;
	volatile uint8_t read_pal = rBCPD;
	rBCPD = 0xFFu;
	rLCDC = LCDCF_ON;
	if (read_pal != 0u)
		result = AP_CGB;

	return result;
}

void main(void)
{
	uint8_t result = AP_NO;

    #ifndef MEGADUCK
    // GB/ GBC / .pocket

	    if (_cpu == CGB_TYPE) {
            printf("(Running on a GBC)\n");

            // Skip cgb only test if running on a CGB but in DMG compat mode via the ROM header
            if ( *(uint8_t *)0x0143  != 0x00u) {
       		    set_default_palette();
    	    	result = is_it_pocket_gbc();
            }
	    }
	    else {
		    result = is_it_pocket();
        }
    #else
        // Megaduck
		result = is_it_pocket();
    #endif

	switch(result) {
		case AP_DMG: printf("I AM (PROBS):\n\nANALOGUE POCKET\n(NON-CGB)\n"); break;
		case AP_CGB: printf("I AM (PROBS):\n\nANALOGUE POCKET\n(CGB)\n"); break;
		case AP_NO:
		default: printf("I AM (PROBS):\n\nNOT ANALOGUE POCKET\n"); break;
	}

	// Loop forever
	while(1) {


		// Game main loop processing goes here


		// Done processing, yield CPU and wait for start of next frame
		wait_vbl_done();
	}
}

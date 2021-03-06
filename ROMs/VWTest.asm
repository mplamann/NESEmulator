	.byte "NES",$1A
	.byte $01
	.byte $01
	.byte $00
	.byte $00
	.byte 0,0,0,0,0,0,0,0 	; pad header to 16 bytes  

	;;;;;;;;;;;;;;;

	    
	.org $C000 
RESET:	
	SEI          ; disable IRQs
	CLD          ; disable decimal mode
	LDX #$40
	STX $4017    ; disable APU frame IRQ
	LDX #$FF
	TXS          ; Set up stack
	INX          ; now X = 0
	STX $2000    ; disable NMI
	STX $2001    ; disable rendering
	STX $4010    ; disable DMC IRQs
	
vblankwait1:       ; First wait for vblank to make sure PPU is ready
	BIT $2002
	BPL vblankwait1
	
clrmem:	
	LDA #$00
	STA $0000, x
	STA $0100, x
	STA $0300, x
	STA $0400, x
	STA $0500, x
	STA $0600, x
	STA $0700, x
	LDA #$FE
	STA $0200, x
	INX
	BNE clrmem
	
vblankwait2:      ; Second wait for vblank, PPU is ready after this
	BIT $2002
	BPL vblankwait2	
	
	
LoadPalettes:
	LDA $2002             ; read PPU status to reset the high/low latch
	LDA #$3F
	STA $2006             ; write the high byte of $3F00 address
	LDA #$00
	STA $2006             ; write the low byte of $3F00 address
	LDX #$00              ; start out at 0
LoadPalettesLoop:
	LDA palette, x        ; load data from address (palette + the value in x)
	                        ; 1st time through loop it will load palette+0
	                        ; 2nd time through loop it will load palette+1
	                        ; 3rd time through loop it will load palette+2
	                        ; etc
	STA $2007             	; write to PPU
	INX                   ; X = X 	+ 1
	CPX #$20              ; 	
	BNE LoadPalettesLoop  ; Branch	 to LoadPalettesLoop if compare was Not Equal to zero
	                        ; if compare was	 equal to 32, keep going down
	
	
LoadBackground:
	LDA $2002             ; read PPU status to reset the high/low latch
	LDA #$20
	STA $2006             ; write the high byte of $2000 address
	LDA #$00
	STA $2006             ; write the low byte of $2000 address
	LDX #$00              ; start out at 0
LoadBackgroundLoop:
	LDA background, x     ; load data from address (background + the value in x)
	STA $2007             ; write to PPU
	INX                   ; X = X + 1
	CPX #$80              ; Compare X to hex $80, decimal 128 - copying 128 bytes
	BNE LoadBackgroundLoop  ; Branch to LoadBackgroundLoop if compare was Not Equal to zero
	                        ; if compare was equal to 128, keep going down
	
	
LoadAttribute:
	LDA $2002             ; read PPU status to reset the high/low latch
	LDA #$23
	STA $2006             ; write the high byte of $23C0 address
	LDA #$C0
	STA $2006             ; write the low byte of $23C0 address
	LDX #$00              ; start out at 0
LoadAttributeLoop:
	LDA attribute, x      ; load data from address (attribute + the value in x)
	STA $2007             ; write to PPU
	INX                   ; X = X + 1
	CPX #$08              ; Compare X to hex $08, decimal 8 - copying 8 bytes
	BNE LoadAttributeLoop  ; Branch to LoadAttributeLoop if compare was Not Equal to zero
	                        ; if compare was equal to 128, keep going down
	
	
	
	
	
	LDA #%10010100   ; enable NMI, sprites from Pattern Table 0, background from Pattern Table 1
	STA $2000
	
	LDA #%00011110   ; enable sprites, enable background, no clipping on left side
	STA $2001

	LDA #$20
	STA $2006
	LDA #$00
	STA $2006
Forever:
	JSR wasteCycles
	ADC #1
	STA $2007
	JMP Forever     ;jump back to Forever, infinite loop
	
	
wasteCycles:
	LDX #$FF
wasteCyclesLoop:
	DEX
	BNE wasteCyclesLoop
	RTS
	
wasteMoreCycles:
	LDY #$FF
wasteMoreCyclesLoop:
	JSR wasteCycles
	DEY
	BNE wasteMoreCyclesLoop
	RTS
	
	
NMI:
	LDA #$00
	STA $2003       ; set the low byte (00) of the RAM address
	LDA #$02
	STA $4014       ; set the high byte (02) of the RAM address, start the transfer
	
	
LatchController:
	LDA #$01
	STA $4016
	LDA #$00
	STA $4016       ; tell both the controllers to latch buttons
	
	
ReadA: 
	LDA $4016       ; player 1 - A
	AND #%00000001  ; only look at bit 0
	BEQ ReadADone   ; branch to ReadADone if button is NOT pressed (0)
				; add instructions here to do something when button IS pressed (1)
	LDA $0203       ; load	 sprite X position
	CLC             ; make sure th	e carry flag is clear
	ADC #$01        ; A = A + 1	
	STA $0203       ; save sprite 	X position
ReadADone:        ; handling this button	 is done
	
	
ReadB: 
	LDA $4016       ; player 1 - B
	AND #%00000001  ; only look at bit 0
	BEQ ReadBDone   ; branch to ReadBDone if button is NOT pressed (0)
				; add instructions here to do something when button IS pressed (1)
	LDA $0203       ; load	 sprite X position
	SEC             ; make sure ca	rry flag is set
	SBC #$01        ; A = A - 1	
	STA $0203       ; save sprite 	X position
ReadBDone:        ; handling this button	 is done
	
	
	;;This is the PPU clean up section, so rendering the next frame starts properly.
	LDA #%10010000   ; enable NMI, sprites from Pattern Table 0, background from Pattern Table 1
	STA $2000
	LDA #%00011110   ; enable sprites, enable background, no clipping on left side
	STA $2001
	LDA #$00        ;;tell the ppu there is no background scrolling
	STA $2005
	STA $2005
	
	RTI             ; return from interrupt
	
;;;;;;;;;;;;;;  
	
	
	
	.org $E000
palette:
	.db $22,$29,$1A,$0F,  $22,$36,$17,$0F,  $22,$30,$21,$0F,  $22,$27,$17,$0F   ;;background palette
	.db $22,$1C,$15,$14,  $22,$02,$38,$3C,  $22,$1C,$15,$14,  $22,$02,$38,$3C   ;;sprite palette
	
sprites:
				;vert tile attr horiz
	.db $80, $32, $00, $80	   ;sprite 0
	.db $80, $33, $00, $88   ;spri	te 1
	.db $88, $34, $00, $80   ;sprite 2	
	.db $88, $35, $00, $88   ;sprite 3	
	
	
background:
	.db $24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24  ;;row 1
	.db $24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24  ;;all sky
	
	.db $24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24  ;;row 2
	.db $24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24  ;;all sky
	
	.db $24,$24,$24,$24,$45,$45,$24,$24,$45,$45,$45,$45,$45,$45,$24,$24  ;;row 3
	.db $24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$53,$54,$24,$24  ;;some brick tops
	
	.db $24,$24,$24,$24,$47,$47,$24,$24,$47,$47,$47,$47,$47,$47,$24,$24  ;;row 4
	.db $24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$24,$55,$56,$24,$24  ;;brick bottoms
	
attribute:
	.db %00000000, %00010000, %01010000, %00010000, %00000000, %00000000, %00000000, %00110000
	
	.db $24,$24,$24,$24, $47,$47,$24,$24 ,$47,$47,$47,$47, $47,$47,$24,$24 ,$24,$24,$24,$24 ,$24,$24,$24,$24, $24,$24,$24,$24, $55,$56,$24,$24  ;;brick bottoms
	
	
	
	.org $FFFA     ;first of the three vectors starts here
	.dw NMI        ;when an NMI happens (once per frame if enabled) the 
				;processor will jump to the label NMI:
	.dw RESET      ;when t	he processor first turns on or is reset, it will jump
				;to the label	 RESET:
	.dw 0          ;external interrupt IRQ	 is not used in this tutorial
	
	
;;;;;;;;;;;;;;  
	
	
	.incbin "mario.chr"   ;includes 8KB graphics file from SMB1

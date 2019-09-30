# Trevor 2 BOM #

Below is the BOM we used on trevor 2.0s. A couple tradeoffs can be made:

1. A weaker resistor pack can be used to make the LEDs brighter. We used 1.5k ohm to preserve power. 220 ohm would be much brighter.
2. The CR1220 battery and holder work well physically on this design but weak at providing sufficient current and endurance. If you want bright LEDs with long life consider soldering a 2xCR2032 holder to Trevor. He can run at 6V (we've tested this).
3. ATtiny3217 is required since there is a large amount of data in the flash memory.

| Identifier| Part Number | Footprint | Comment |
| :-: | :-: | :-: | :-: |
| BT1 | CR1220 SMD Ebay | CR1220 | [Example](https://www.ebay.com/itm/3V-Lithium-Coin-Cell-Battery-Holder-1pc-CR1220-SMD-/111676913590) |                 
| D1,D2 | APFA3010SEKJ3ZGKQBKC | Kingbring 3x1.5mm |  RGB LED - side mount |
| PIN | Lapel Pin | 5mm | [Link](https://smile.amazon.com/gp/product/B077TL9BV1/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1) |
| R1 | 10k Ohm Resistor | 0805 | |
| RN1, RN2 | EXB-38V152JV | RPACK 4x0603 Convex | |
| SW1 | SW_SPDT_PCM12 | | |
| TH1 | 10k Thermistor | 0805 | |
| U1 | ATtiny3217-M | QFN24 | |
| Battery | CR1220 | | |

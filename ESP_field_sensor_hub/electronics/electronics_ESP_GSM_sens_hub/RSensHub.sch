EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L myComponents:ESP32-WROOM-32U U3
U 1 1 6366CE43
P 6350 1450
F 0 "U3" H 6875 1565 50  0000 C CNN
F 1 "ESP32-WROOM-32U" H 6875 1474 50  0000 C CNN
F 2 "myComponents:ESP32-WROOM-32U" H 6350 1450 50  0001 C CNN
F 3 "" H 6350 1450 50  0001 C CNN
	1    6350 1450
	1    0    0    -1  
$EndComp
$Comp
L Connector:RJ45_Shielded J1
U 1 1 63683381
P 2800 1000
F 0 "J1" V 2875 500 50  0000 L CNN
F 1 "RJ45_Shielded" V 2902 1530 50  0001 L CNN
F 2 "myComponents:RJ45" V 2800 1025 50  0001 C CNN
F 3 "~" V 2800 1025 50  0001 C CNN
	1    2800 1000
	0    1    1    0   
$EndComp
$Comp
L Connector:RJ45_Shielded J2
U 1 1 63684DC6
P 3850 1000
F 0 "J2" V 3907 1530 50  0000 L CNN
F 1 "RJ45_Shielded" V 3952 1530 50  0001 L CNN
F 2 "myComponents:RJ45" V 3850 1025 50  0001 C CNN
F 3 "~" V 3850 1025 50  0001 C CNN
	1    3850 1000
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 1400 4150 1550
Wire Wire Line
	4150 1550 4250 1550
Wire Wire Line
	4250 1550 4250 1400
Wire Wire Line
	3200 1400 3200 1550
Wire Wire Line
	3200 1550 3100 1550
Wire Wire Line
	3100 1550 3100 1400
Wire Wire Line
	4250 1625 4250 1550
Connection ~ 4250 1550
Wire Wire Line
	3200 1625 3200 1550
Connection ~ 3200 1550
Wire Wire Line
	2500 1625 2500 1550
Wire Wire Line
	2600 1400 2600 1550
Wire Wire Line
	2600 1550 2500 1550
Connection ~ 2500 1550
Wire Wire Line
	2500 1550 2500 1400
Wire Wire Line
	3550 1625 3550 1550
Wire Wire Line
	3650 1400 3650 1550
Wire Wire Line
	3650 1550 3550 1550
Connection ~ 3550 1550
Wire Wire Line
	3550 1550 3550 1400
Wire Wire Line
	2800 1400 2800 1550
Text Label 2800 1550 3    50   ~ 0
RS485_B
Text Label 3000 1550 3    50   ~ 0
RS485_A
Wire Wire Line
	3000 1550 3000 1400
Text Label 2700 1550 3    50   ~ 0
BUS_3
Text Label 2900 1550 3    50   ~ 0
BUS_5
Wire Wire Line
	2900 1550 2900 1400
Wire Wire Line
	2700 1550 2700 1400
Wire Wire Line
	3850 1400 3850 1550
Text Label 3850 1550 3    50   ~ 0
RS485_B
Text Label 4050 1550 3    50   ~ 0
RS485_A
Wire Wire Line
	4050 1550 4050 1400
Text Label 3750 1550 3    50   ~ 0
BUS_3
Text Label 3950 1550 3    50   ~ 0
BUS_5
Wire Wire Line
	3950 1550 3950 1400
Wire Wire Line
	3750 1550 3750 1400
$Comp
L Device:R R5
U 1 1 636D3A5B
P 7825 5475
F 0 "R5" H 7650 5525 50  0000 L CNN
F 1 "20M" H 7600 5425 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 7755 5475 50  0001 C CNN
F 3 "~" H 7825 5475 50  0001 C CNN
	1    7825 5475
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 636D671F
P 7825 5875
F 0 "R6" H 7650 5925 50  0000 L CNN
F 1 "2M" H 7650 5825 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 7755 5875 50  0001 C CNN
F 3 "~" H 7825 5875 50  0001 C CNN
	1    7825 5875
	1    0    0    -1  
$EndComp
Text Label 7400 5675 2    50   ~ 0
VP_PIN
$Comp
L power:+BATT #PWR0111
U 1 1 636D9491
P 8025 5250
F 0 "#PWR0111" H 8025 5100 50  0001 C CNN
F 1 "+BATT" V 8025 5500 50  0000 C CNN
F 2 "" H 8025 5250 50  0001 C CNN
F 3 "" H 8025 5250 50  0001 C CNN
	1    8025 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7825 5325 7825 5250
Text Label 5975 1850 2    50   ~ 0
VP_PIN
Wire Wire Line
	5975 1850 6250 1850
Wire Wire Line
	6250 1950 5975 1950
Text Label 5975 1950 2    50   ~ 0
VN_PIN
$Comp
L myComponents:5V5ADCDC U2
U 1 1 6369B2DC
P 2300 3250
F 0 "U2" H 2600 3415 50  0000 C CNN
F 1 "5V5ADCDC" H 2600 3324 50  0000 C CNN
F 2 "myComponents:DCDC_HCW_P715" H 2300 3250 50  0001 C CNN
F 3 "" H 2300 3250 50  0001 C CNN
	1    2300 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 6369B8AA
P 2125 3725
F 0 "#PWR0112" H 2125 3475 50  0001 C CNN
F 1 "GND" H 2130 3552 50  0000 C CNN
F 2 "" H 2125 3725 50  0001 C CNN
F 3 "" H 2125 3725 50  0001 C CNN
	1    2125 3725
	1    0    0    -1  
$EndComp
Wire Wire Line
	2125 3450 2200 3450
$Comp
L power:GND #PWR0113
U 1 1 6369D246
P 3050 3725
F 0 "#PWR0113" H 3050 3475 50  0001 C CNN
F 1 "GND" H 3055 3552 50  0000 C CNN
F 2 "" H 3050 3725 50  0001 C CNN
F 3 "" H 3050 3725 50  0001 C CNN
	1    3050 3725
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3450 3000 3450
$Comp
L power:+BATT #PWR0114
U 1 1 6369F36E
P 1725 2925
F 0 "#PWR0114" H 1725 2775 50  0001 C CNN
F 1 "+BATT" H 1740 3098 50  0000 C CNN
F 2 "" H 1725 2925 50  0001 C CNN
F 3 "" H 1725 2925 50  0001 C CNN
	1    1725 2925
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3350 2125 3350
Wire Wire Line
	3150 3350 3150 3300
$Comp
L power:+5V #PWR0115
U 1 1 636A3668
P 3150 3300
F 0 "#PWR0115" H 3150 3150 50  0001 C CNN
F 1 "+5V" H 3165 3473 50  0000 C CNN
F 2 "" H 3150 3300 50  0001 C CNN
F 3 "" H 3150 3300 50  0001 C CNN
	1    3150 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3450 3050 3725
Wire Wire Line
	2125 3725 2125 3450
$Comp
L Device:R R8
U 1 1 636C3BB0
P 1725 3150
F 0 "R8" H 1655 3104 50  0000 R CNN
F 1 "R0" H 1655 3195 50  0000 R CNN
F 2 "myComponents:resistor0.25,9mm" V 1655 3150 50  0001 C CNN
F 3 "~" H 1725 3150 50  0001 C CNN
	1    1725 3150
	-1   0    0    1   
$EndComp
$Comp
L Device:D D3
U 1 1 636C8541
P 1975 3350
F 0 "D3" H 1975 3134 50  0000 C CNN
F 1 "D" H 1975 3225 50  0000 C CNN
F 2 "myComponents:1N41_Diode" H 1975 3350 50  0001 C CNN
F 3 "~" H 1975 3350 50  0001 C CNN
	1    1975 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	1825 3350 1725 3350
Wire Wire Line
	1725 3350 1725 3300
Wire Wire Line
	1725 3000 1725 2925
$Comp
L Device:R R9
U 1 1 636E97EA
P 8025 5475
F 0 "R9" H 8095 5521 50  0000 L CNN
F 1 "20M" H 8095 5430 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 7955 5475 50  0001 C CNN
F 3 "~" H 8025 5475 50  0001 C CNN
	1    8025 5475
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 636E97F0
P 8025 5875
F 0 "R10" H 8095 5921 50  0000 L CNN
F 1 "2M" H 8095 5830 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 7955 5875 50  0001 C CNN
F 3 "~" H 8025 5875 50  0001 C CNN
	1    8025 5875
	1    0    0    -1  
$EndComp
Text Label 8400 5675 0    50   ~ 0
VN_PIN
Wire Wire Line
	8025 5325 8025 5250
Text Label 7825 5250 1    50   ~ 0
V_SOLAR
Wire Wire Line
	8025 5625 8025 5675
Connection ~ 8025 5675
Wire Wire Line
	8025 5675 8025 5725
Wire Wire Line
	8025 6025 8025 6150
Connection ~ 8025 6150
$Comp
L power:+3.3V #PWR0124
U 1 1 639B49F1
P 6200 1600
F 0 "#PWR0124" H 6200 1450 50  0001 C CNN
F 1 "+3.3V" H 6215 1773 50  0000 C CNN
F 2 "" H 6200 1600 50  0001 C CNN
F 3 "" H 6200 1600 50  0001 C CNN
	1    6200 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1650 6200 1650
Wire Wire Line
	6200 1650 6200 1600
Text Label 9800 1100 2    50   ~ 0
SIM_PWR_KEY
$Comp
L power:+5V #PWR0125
U 1 1 639CE80A
P 9550 825
F 0 "#PWR0125" H 9550 675 50  0001 C CNN
F 1 "+5V" H 9565 998 50  0000 C CNN
F 2 "" H 9550 825 50  0001 C CNN
F 3 "" H 9550 825 50  0001 C CNN
	1    9550 825 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0126
U 1 1 639C5C16
P 9975 1800
F 0 "#PWR0126" H 9975 1550 50  0001 C CNN
F 1 "GND" H 9980 1627 50  0000 C CNN
F 2 "" H 9975 1800 50  0001 C CNN
F 3 "" H 9975 1800 50  0001 C CNN
	1    9975 1800
	1    0    0    -1  
$EndComp
Text Label 9800 1350 2    50   ~ 0
uTX_SIM7000
Text Label 9800 1450 2    50   ~ 0
uRX_SIM7000
$Comp
L power:+3.3V #PWR0127
U 1 1 639B88D1
P 9300 825
F 0 "#PWR0127" H 9300 675 50  0001 C CNN
F 1 "+3.3V" H 9315 998 50  0000 C CNN
F 2 "" H 9300 825 50  0001 C CNN
F 3 "" H 9300 825 50  0001 C CNN
	1    9300 825 
	1    0    0    -1  
$EndComp
$Comp
L myComponents:SIM7000G_DEVBoard U4
U 1 1 636C059E
P 10150 950
F 0 "U4" H 10850 100 50  0000 L CNN
F 1 "SIM7000G_DEVBoard" H 10175 -25 50  0000 L CNN
F 2 "myComponents:sim7000G_dev_board" H 10150 950 50  0001 C CNN
F 3 "" H 10150 950 50  0001 C CNN
	1    10150 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 900  9550 825 
Wire Wire Line
	9300 825  9300 1000
Wire Wire Line
	9975 1800 10050 1800
Wire Wire Line
	10050 1700 9975 1700
Wire Wire Line
	9975 1700 9975 1800
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 63AD6777
P 9775 900
F 0 "JP2" H 9575 850 50  0000 L CNN
F 1 "SolderJumper_2_Open" H 9775 1014 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 9775 900 50  0001 C CNN
F 3 "~" H 9775 900 50  0001 C CNN
	1    9775 900 
	-1   0    0    1   
$EndComp
Wire Wire Line
	9300 1000 10050 1000
Wire Wire Line
	9625 900  9550 900 
Wire Wire Line
	9925 900  10050 900 
Text Label 9250 3000 2    50   ~ 0
UART2_TX
Text Label 9250 2700 2    50   ~ 0
UART2_RX
Text Label 9225 2800 2    50   ~ 0
UART2_DRIVER
Wire Wire Line
	10275 3000 10375 3000
$Comp
L power:GND #PWR0101
U 1 1 63671AD2
P 10375 3000
F 0 "#PWR0101" H 10375 2750 50  0001 C CNN
F 1 "GND" H 10380 2827 50  0000 C CNN
F 2 "" H 10375 3000 50  0001 C CNN
F 3 "" H 10375 3000 50  0001 C CNN
	1    10375 3000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10375 2650 10375 2700
Wire Wire Line
	10375 2700 10275 2700
Text Label 10750 2900 0    50   ~ 0
RS485_A
Text Label 10750 2800 0    50   ~ 0
RS485_B
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 6368CCA9
P 10375 2500
F 0 "JP1" V 10375 2568 50  0000 L CNN
F 1 "SolderJumper_2_Open" H 10375 2614 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 10375 2500 50  0001 C CNN
F 3 "~" H 10375 2500 50  0001 C CNN
	1    10375 2500
	0    -1   1    0   
$EndComp
Wire Wire Line
	10375 2350 10375 2325
$Comp
L power:+5V #PWR0102
U 1 1 63671E3D
P 10375 2325
F 0 "#PWR0102" H 10375 2175 50  0001 C CNN
F 1 "+5V" H 10390 2498 50  0000 C CNN
F 2 "" H 10375 2325 50  0001 C CNN
F 3 "" H 10375 2325 50  0001 C CNN
	1    10375 2325
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J4
U 1 1 63982CDD
P 800 1800
F 0 "J4" H 692 1467 50  0000 C CNN
F 1 "Conn_01x04_Female" H 692 1466 50  0001 C CNN
F 2 "myComponents:TerminalBlockRemovablex4" H 800 1800 50  0001 C CNN
F 3 "~" H 800 1800 50  0001 C CNN
	1    800  1800
	-1   0    0    1   
$EndComp
Text Label 1175 1600 0    50   ~ 0
V_SOLAR
Wire Wire Line
	1175 1600 1000 1600
Text Label 9250 3100 2    50   ~ 0
COM_OUT_LED
Wire Wire Line
	9800 1350 9875 1350
Wire Wire Line
	9800 1450 9875 1450
Wire Wire Line
	9250 2700 9325 2700
Wire Wire Line
	9250 3000 9325 3000
Wire Wire Line
	9250 3100 9325 3100
Wire Wire Line
	9325 3100 9325 3000
Text Label 9250 2600 2    50   ~ 0
COM_IN_LED
Wire Wire Line
	9250 2600 9325 2600
Wire Wire Line
	9325 2600 9325 2700
Text Label 9800 1525 2    50   ~ 0
SIM_IN_LED
Wire Wire Line
	9800 1525 9875 1525
Wire Wire Line
	9875 1525 9875 1450
Text Label 9800 1250 2    50   ~ 0
SIM_OUT_LED
Wire Wire Line
	9800 1250 9875 1250
Wire Wire Line
	9875 1250 9875 1350
Connection ~ 9975 1800
Text Label 7600 2650 0    50   ~ 0
uTX_SIM7000
Text Label 7600 2750 0    50   ~ 0
uRX_SIM7000
Wire Wire Line
	7600 2650 7500 2650
Wire Wire Line
	7600 2750 7500 2750
Wire Wire Line
	10050 1350 9875 1350
Connection ~ 9875 1350
Wire Wire Line
	10050 1450 9875 1450
Connection ~ 9875 1450
Wire Wire Line
	10050 1100 9800 1100
$Comp
L power:GND #PWR0129
U 1 1 63D561F9
P 7575 1650
F 0 "#PWR0129" H 7575 1400 50  0001 C CNN
F 1 "GND" H 7580 1477 50  0000 C CNN
F 2 "" H 7575 1650 50  0001 C CNN
F 3 "" H 7575 1650 50  0001 C CNN
	1    7575 1650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7575 1650 7500 1650
$Comp
L power:GND #PWR0135
U 1 1 63D5EA9D
P 7600 2250
F 0 "#PWR0135" H 7600 2000 50  0001 C CNN
F 1 "GND" H 7605 2077 50  0000 C CNN
F 2 "" H 7600 2250 50  0001 C CNN
F 3 "" H 7600 2250 50  0001 C CNN
	1    7600 2250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 2250 7500 2250
$Comp
L power:GND #PWR0136
U 1 1 63D67849
P 5575 3125
F 0 "#PWR0136" H 5575 2875 50  0001 C CNN
F 1 "GND" H 5580 2952 50  0000 C CNN
F 2 "" H 5575 3125 50  0001 C CNN
F 3 "" H 5575 3125 50  0001 C CNN
	1    5575 3125
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0137
U 1 1 63D71EF1
P 6175 3450
F 0 "#PWR0137" H 6175 3300 50  0001 C CNN
F 1 "+5V" H 6190 3623 50  0000 C CNN
F 2 "" H 6175 3450 50  0001 C CNN
F 3 "" H 6175 3450 50  0001 C CNN
	1    6175 3450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6175 3450 6250 3450
Connection ~ 9325 2700
Wire Wire Line
	9325 2800 9275 2800
Wire Wire Line
	9275 2900 9275 2800
Wire Wire Line
	9325 2900 9275 2900
$Comp
L myComponents:MAX48_module U1
U 1 1 6366DB2D
P 9425 2700
F 0 "U1" H 9800 2200 50  0000 C CNN
F 1 "MAX48_module" H 9800 2300 50  0000 C CNN
F 2 "myComponents:MAX485_module" H 9425 2700 50  0001 C CNN
F 3 "" H 9425 2700 50  0001 C CNN
	1    9425 2700
	1    0    0    -1  
$EndComp
Connection ~ 9325 3000
Wire Wire Line
	9225 2800 9275 2800
Connection ~ 9275 2800
Wire Wire Line
	10275 2900 10700 2900
Wire Wire Line
	10275 2800 10525 2800
$Comp
L Device:Antenna AE1
U 1 1 63E4AA97
P 10900 1150
F 0 "AE1" H 10980 1139 50  0000 L CNN
F 1 "Antenna" V 10950 725 50  0000 L CNN
F 2 "myComponents:GNSSantennaVertical" H 10900 1150 50  0001 C CNN
F 3 "~" H 10900 1150 50  0001 C CNN
	1    10900 1150
	1    0    0    -1  
$EndComp
Text Label 7825 2450 0    50   ~ 0
UART2_RX
Text Label 7825 2350 0    50   ~ 0
UART2_TX
Wire Wire Line
	7500 2350 7825 2350
Wire Wire Line
	7500 2450 7825 2450
Text Label 7825 2150 0    50   ~ 0
UART2_DRIVER
Wire Wire Line
	7825 2150 7500 2150
$Comp
L Device:CP1 C2
U 1 1 63F71095
P 9175 1625
F 0 "C2" H 9290 1671 50  0000 L CNN
F 1 "1000u16V" H 9290 1580 50  0000 L CNN
F 2 "myComponents:CAP_Radial_D10.0mm_P5.00mm" H 9175 1625 50  0001 C CNN
F 3 "~" H 9175 1625 50  0001 C CNN
	1    9175 1625
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0141
U 1 1 63F8F32A
P 9175 1425
F 0 "#PWR0141" H 9175 1275 50  0001 C CNN
F 1 "+5V" H 9190 1598 50  0000 C CNN
F 2 "" H 9175 1425 50  0001 C CNN
F 3 "" H 9175 1425 50  0001 C CNN
	1    9175 1425
	1    0    0    -1  
$EndComp
Wire Wire Line
	9175 1475 9175 1425
Wire Wire Line
	9975 1800 9175 1800
Wire Wire Line
	9175 1800 9175 1775
$Comp
L Jumper:Jumper_2_Bridged JP5
U 1 1 63C3097D
P 9775 3400
F 0 "JP5" H 9725 3475 50  0000 L CNN
F 1 "RS485 terminator" H 9275 3250 50  0001 L CNN
F 2 "myComponents:jumperx2_P2.54mm_widePads" H 9775 3400 50  0001 C CNN
F 3 "~" H 9775 3400 50  0001 C CNN
	1    9775 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 63C5E5F0
P 9350 3400
F 0 "R19" H 9420 3446 50  0000 L CNN
F 1 "120" H 9420 3355 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 9280 3400 50  0001 C CNN
F 3 "~" H 9350 3400 50  0001 C CNN
	1    9350 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9575 3400 9500 3400
Text Label 10075 3400 0    50   ~ 0
RS485_A
Text Label 9125 3400 2    50   ~ 0
RS485_B
Wire Wire Line
	9125 3400 9200 3400
Wire Wire Line
	10075 3400 9975 3400
$Comp
L Device:C C3
U 1 1 63CE8395
P 7500 5875
F 0 "C3" H 7615 5921 50  0000 L CNN
F 1 "100nF" H 7615 5830 50  0000 L CNN
F 2 "myComponents:Capker_L4.6mm_W2.0mm_P2.50mm" H 7538 5725 50  0001 C CNN
F 3 "~" H 7500 5875 50  0001 C CNN
	1    7500 5875
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7400 5675 7500 5675
Wire Wire Line
	7500 5725 7500 5675
Wire Wire Line
	7500 6025 7500 6150
$Comp
L Device:C C4
U 1 1 63D0EF0B
P 8350 5875
F 0 "C4" H 8465 5921 50  0000 L CNN
F 1 "100nF" H 8465 5830 50  0000 L CNN
F 2 "myComponents:Capker_L4.6mm_W2.0mm_P2.50mm" H 8388 5725 50  0001 C CNN
F 3 "~" H 8350 5875 50  0001 C CNN
	1    8350 5875
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 6150 8350 6025
Wire Wire Line
	8025 6150 8350 6150
Wire Wire Line
	8025 5675 8350 5675
Wire Wire Line
	8350 5725 8350 5675
Connection ~ 8350 5675
Wire Wire Line
	8350 5675 8400 5675
$Comp
L Device:R R20
U 1 1 63C11E7F
P 10525 3225
F 0 "R20" H 10595 3271 50  0000 L CNN
F 1 "120" H 10595 3180 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 10455 3225 50  0001 C CNN
F 3 "~" H 10525 3225 50  0001 C CNN
	1    10525 3225
	1    0    0    -1  
$EndComp
$Comp
L Device:R R21
U 1 1 63C1CE90
P 10700 3500
F 0 "R21" H 10770 3546 50  0000 L CNN
F 1 "120" H 10770 3455 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 10630 3500 50  0001 C CNN
F 3 "~" H 10700 3500 50  0001 C CNN
	1    10700 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10525 3075 10525 2800
Connection ~ 10525 2800
Wire Wire Line
	10525 2800 10750 2800
Wire Wire Line
	10700 3350 10700 2900
Connection ~ 10700 2900
Wire Wire Line
	10700 2900 10750 2900
$Comp
L power:GND #PWR0108
U 1 1 63C2E9B2
P 10600 3675
F 0 "#PWR0108" H 10600 3425 50  0001 C CNN
F 1 "GND" H 10605 3502 50  0000 C CNN
F 2 "" H 10600 3675 50  0001 C CNN
F 3 "" H 10600 3675 50  0001 C CNN
	1    10600 3675
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10700 3650 10600 3650
Wire Wire Line
	10600 3650 10600 3675
Wire Wire Line
	10525 3375 10525 3650
Wire Wire Line
	10525 3650 10600 3650
Connection ~ 10600 3650
Wire Wire Line
	10625 5700 10575 5700
Wire Wire Line
	10625 5750 10625 5700
$Comp
L power:GND #PWR0110
U 1 1 63E99D19
P 10625 5750
F 0 "#PWR0110" H 10625 5500 50  0001 C CNN
F 1 "GND" H 10630 5577 50  0000 C CNN
F 2 "" H 10625 5750 50  0001 C CNN
F 3 "" H 10625 5750 50  0001 C CNN
	1    10625 5750
	1    0    0    -1  
$EndComp
Connection ~ 10175 5700
Wire Wire Line
	10075 5700 10175 5700
Wire Wire Line
	10075 5625 10075 5700
Wire Wire Line
	10175 5700 10175 5625
$Comp
L Jumper:Jumper_2_Bridged JP4
U 1 1 63E4409D
P 10375 5700
F 0 "JP4" V 10329 5758 50  0000 L CNN
F 1 "Jumper_2_Bridged" V 10420 5758 50  0001 L CNN
F 2 "myComponents:jumperx2_P2.54mm_widePads" H 10375 5700 50  0001 C CNN
F 3 "~" H 10375 5700 50  0001 C CNN
	1    10375 5700
	-1   0    0    1   
$EndComp
$Comp
L Jumper:Jumper_2_Bridged JP3
U 1 1 63E06692
P 10375 6150
F 0 "JP3" V 10325 6250 50  0000 L CNN
F 1 "Jumper_2_Bridged" V 10420 6208 50  0001 L CNN
F 2 "myComponents:jumperx2_P2.54mm_widePads" H 10375 6150 50  0001 C CNN
F 3 "~" H 10375 6150 50  0001 C CNN
	1    10375 6150
	-1   0    0    1   
$EndComp
Wire Wire Line
	10575 4750 10525 4750
Wire Wire Line
	10575 4575 10525 4575
$Comp
L power:+3.3V #PWR0118
U 1 1 63DD1AF4
P 10575 4575
F 0 "#PWR0118" H 10575 4425 50  0001 C CNN
F 1 "+3.3V" V 10575 4800 50  0000 C CNN
F 2 "" H 10575 4575 50  0001 C CNN
F 3 "" H 10575 4575 50  0001 C CNN
	1    10575 4575
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0119
U 1 1 63DCEDC8
P 10575 4750
F 0 "#PWR0119" H 10575 4600 50  0001 C CNN
F 1 "+5V" V 10575 4950 50  0000 C CNN
F 2 "" H 10575 4750 50  0001 C CNN
F 3 "" H 10575 4750 50  0001 C CNN
	1    10575 4750
	0    1    1    0   
$EndComp
Wire Wire Line
	10225 4050 9775 4050
Wire Wire Line
	9775 4050 9775 4825
Wire Wire Line
	10225 4225 9875 4225
Wire Wire Line
	9875 4225 9875 4825
Wire Wire Line
	9975 4400 9975 4825
Wire Wire Line
	10075 4575 10075 4825
Wire Wire Line
	10225 4400 9975 4400
$Comp
L Device:R R18
U 1 1 63DA4224
P 10375 4575
F 0 "R18" V 10450 4575 50  0000 L CNN
F 1 "330R" V 10425 4250 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 10305 4575 50  0001 C CNN
F 3 "~" H 10375 4575 50  0001 C CNN
	1    10375 4575
	0    1    -1   0   
$EndComp
Wire Wire Line
	10225 4575 10075 4575
Wire Wire Line
	10225 4750 10175 4750
Wire Wire Line
	10175 4750 10175 4825
$Comp
L Device:R R22
U 1 1 63DA4218
P 10375 4750
F 0 "R22" V 10300 4650 50  0000 L CNN
F 1 "R" H 10445 4705 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 10305 4750 50  0001 C CNN
F 3 "~" H 10375 4750 50  0001 C CNN
	1    10375 4750
	0    -1   1    0   
$EndComp
Text Label 9200 4350 2    50   ~ 0
SIM_OUT_LED
Text Label 9200 4450 2    50   ~ 0
SIM_IN_LED
Text Label 9200 4550 2    50   ~ 0
COM_OUT_LED
Text Label 9200 4650 2    50   ~ 0
COM_IN_LED
Wire Wire Line
	9275 4750 9275 4825
Text Label 9200 4750 2    50   ~ 0
VBUS_LED
$Comp
L myComponents:MULTICOL_LED_BAR U6
U 1 1 63CCD3AD
P 10275 4925
F 0 "U6" V 10529 5053 50  0000 L CNN
F 1 "MULTICOL_LED_BAR" V 10620 5053 50  0000 L CNN
F 2 "myComponents:DIP-20_LED_BAR_DISP" H 10325 4875 50  0001 C CNN
F 3 "" H 10325 4875 50  0001 C CNN
	1    10275 4925
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 63F6BF66
P 7825 6250
F 0 "#PWR0120" H 7825 6000 50  0001 C CNN
F 1 "GND" H 7830 6077 50  0000 C CNN
F 2 "" H 7825 6250 50  0001 C CNN
F 3 "" H 7825 6250 50  0001 C CNN
	1    7825 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 6150 7825 6150
Wire Wire Line
	7825 6025 7825 6150
Connection ~ 7825 6150
Wire Wire Line
	7825 6150 8025 6150
Wire Wire Line
	7825 5625 7825 5675
Wire Wire Line
	7500 5675 7825 5675
Connection ~ 7500 5675
Connection ~ 7825 5675
Wire Wire Line
	7825 5675 7825 5725
Text Label 3200 1625 3    50   ~ 0
SAFE_BAT+
Text Label 4250 1625 3    50   ~ 0
SAFE_BAT+
$Comp
L Isolator:PC817 U12
U 1 1 63EDD277
P 2900 5800
F 0 "U12" H 2900 6125 50  0000 C CNN
F 1 "PC817" H 2900 6034 50  0000 C CNN
F 2 "myComponents:optoCouplerDIP4" H 2700 5600 50  0001 L CIN
F 3 "http://www.soselectronic.cz/a_info/resource/d/pc817.pdf" H 2900 5800 50  0001 L CNN
	1    2900 5800
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 63F1297F
P 2550 6925
F 0 "R17" H 2620 6971 50  0000 L CNN
F 1 "100K" H 2620 6880 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 2480 6925 50  0001 C CNN
F 3 "~" H 2550 6925 50  0001 C CNN
	1    2550 6925
	1    0    0    -1  
$EndComp
$Comp
L Device:R R28
U 1 1 6414F787
P 3400 5900
F 0 "R28" H 3470 5946 50  0000 L CNN
F 1 "1K" H 3470 5855 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 3330 5900 50  0001 C CNN
F 3 "~" H 3400 5900 50  0001 C CNN
	1    3400 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2550 5900 2600 5900
Wire Wire Line
	3200 5225 3200 5700
$Comp
L power:GND #PWR0105
U 1 1 6454B5C4
P 3600 7150
F 0 "#PWR0105" H 3600 6900 50  0001 C CNN
F 1 "GND" H 3605 6977 50  0000 C CNN
F 2 "" H 3600 7150 50  0001 C CNN
F 3 "" H 3600 7150 50  0001 C CNN
	1    3600 7150
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR0106
U 1 1 6454EAD6
P 3200 5175
F 0 "#PWR0106" H 3200 5025 50  0001 C CNN
F 1 "+BATT" H 3215 5348 50  0000 C CNN
F 2 "" H 3200 5175 50  0001 C CNN
F 3 "" H 3200 5175 50  0001 C CNN
	1    3200 5175
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 63F73D75
P 1675 6800
F 0 "R13" H 1745 6846 50  0000 L CNN
F 1 "330R" H 1745 6755 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 1605 6800 50  0001 C CNN
F 3 "~" H 1675 6800 50  0001 C CNN
	1    1675 6800
	-1   0    0    1   
$EndComp
Text Label 1700 6300 2    50   ~ 0
Vout_OFF
$Comp
L power:GND #PWR0140
U 1 1 641002EE
P 2550 7150
F 0 "#PWR0140" H 2550 6900 50  0001 C CNN
F 1 "GND" H 2555 6977 50  0000 C CNN
F 2 "" H 2550 7150 50  0001 C CNN
F 3 "" H 2550 7150 50  0001 C CNN
	1    2550 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R29
U 1 1 63F41615
P 3225 6550
F 0 "R29" H 3295 6596 50  0000 L CNN
F 1 "1K" H 3295 6505 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 3155 6550 50  0001 C CNN
F 3 "~" H 3225 6550 50  0001 C CNN
	1    3225 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7825 6250 7825 6150
$Comp
L Device:R_Network08 RN1
U 1 1 63D1B53F
P 9575 5900
F 0 "RN1" H 9095 5854 50  0000 R CNN
F 1 "R_Network08" H 9095 5945 50  0000 R CNN
F 2 "myComponents:PinHeader_1x9_P2.54mm_widePads" V 10050 5900 50  0001 C CNN
F 3 "http://www.vishay.com/docs/31509/csc.pdf" H 9575 5900 50  0001 C CNN
	1    9575 5900
	-1   0    0    1   
$EndComp
Wire Wire Line
	9275 5700 9275 5625
Wire Wire Line
	9375 5700 9375 5625
Wire Wire Line
	9475 5700 9475 5625
Wire Wire Line
	9575 5700 9575 5625
Wire Wire Line
	9675 5700 9675 5625
Wire Wire Line
	9775 5700 9775 5625
Wire Wire Line
	9875 5700 9875 5625
Wire Wire Line
	9975 5700 9975 5625
Wire Wire Line
	10175 6150 9975 6150
Wire Wire Line
	9975 6150 9975 6100
$Comp
L power:GND #PWR0142
U 1 1 63EFB967
P 10625 6200
F 0 "#PWR0142" H 10625 5950 50  0001 C CNN
F 1 "GND" H 10630 6027 50  0000 C CNN
F 2 "" H 10625 6200 50  0001 C CNN
F 3 "" H 10625 6200 50  0001 C CNN
	1    10625 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10625 6200 10625 6150
Wire Wire Line
	10625 6150 10575 6150
Wire Wire Line
	9375 4650 9375 4825
Wire Wire Line
	9475 4550 9475 4825
Wire Wire Line
	9575 4450 9575 4825
Wire Wire Line
	9675 4350 9675 4825
Wire Wire Line
	9675 4350 9200 4350
Wire Wire Line
	9200 4450 9575 4450
Wire Wire Line
	9475 4550 9200 4550
Wire Wire Line
	9375 4650 9200 4650
Wire Wire Line
	9275 4750 9200 4750
$Comp
L Transistor_FET:IRF540N Q4
U 1 1 63EF9304
P 3500 6300
F 0 "Q4" H 3704 6346 50  0000 L CNN
F 1 "IRF540N" H 3704 6255 50  0000 L CNN
F 2 "myComponents:TO-220-3_Horizontal_TabDown_bothSides" H 3750 6225 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 3500 6300 50  0001 L CNN
	1    3500 6300
	1    0    0    -1  
$EndComp
Text Label 1700 5700 2    50   ~ 0
Vout_ON
$Comp
L Device:R R12
U 1 1 63FCE1BE
P 1325 6800
F 0 "R12" H 1395 6846 50  0000 L CNN
F 1 "330R" H 1395 6755 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 1255 6800 50  0001 C CNN
F 3 "~" H 1325 6800 50  0001 C CNN
	1    1325 6800
	-1   0    0    1   
$EndComp
$Comp
L Isolator:PC817 U10
U 1 1 63DCE4BA
P 2025 5800
F 0 "U10" H 2025 6125 50  0000 C CNN
F 1 "PC817" H 2025 6034 50  0000 C CNN
F 2 "myComponents:optoCouplerDIP4" H 1825 5600 50  0001 L CIN
F 3 "http://www.soselectronic.cz/a_info/resource/d/pc817.pdf" H 2025 5800 50  0001 L CNN
	1    2025 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2325 5700 2450 5700
$Comp
L Device:R R14
U 1 1 63EDD7DE
P 2450 5450
F 0 "R14" H 2520 5496 50  0000 L CNN
F 1 "1K" H 2520 5405 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 2380 5450 50  0001 C CNN
F 3 "~" H 2450 5450 50  0001 C CNN
	1    2450 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5900 2550 6300
Wire Wire Line
	2325 5900 2375 5900
Wire Wire Line
	2375 5900 2375 6300
Wire Wire Line
	2375 6300 2325 6300
Wire Wire Line
	2375 6300 2550 6300
Connection ~ 2375 6300
Connection ~ 2550 6300
Wire Wire Line
	2450 5600 2450 5700
Connection ~ 2450 5700
Wire Wire Line
	2450 5700 2600 5700
Wire Wire Line
	2450 5300 2450 5225
Wire Wire Line
	2450 5225 3200 5225
Wire Wire Line
	2325 6500 2375 6500
Wire Wire Line
	3550 5900 3600 5900
Wire Wire Line
	3600 5900 3600 6100
Wire Wire Line
	3250 5900 3200 5900
Wire Wire Line
	2550 7150 2550 7100
Connection ~ 2550 7100
Wire Wire Line
	2550 7100 2550 7075
Connection ~ 2375 7100
Wire Wire Line
	2375 7100 2550 7100
Wire Wire Line
	2375 6500 2375 7100
Wire Wire Line
	3225 6300 3225 6400
Text Label 3225 6775 3    50   ~ 0
VBUS_LED
Wire Wire Line
	3225 6775 3225 6700
$Comp
L Jumper:SolderJumper_2_Open JP6
U 1 1 644467F2
P 2975 6550
F 0 "JP6" V 2975 6618 50  0000 L CNN
F 1 "SolderJumper_2_Open" H 2975 6664 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 2975 6550 50  0001 C CNN
F 3 "~" H 2975 6550 50  0001 C CNN
	1    2975 6550
	0    1    -1   0   
$EndComp
Wire Wire Line
	2550 6300 2750 6300
Connection ~ 3225 6300
Wire Wire Line
	3225 6300 3300 6300
Wire Wire Line
	3200 5175 3200 5225
Connection ~ 3200 5225
Wire Wire Line
	3600 6500 3600 7150
Text Label 3700 5225 0    50   ~ 0
SAFE_BAT+
Text Label 3700 5400 0    50   ~ 0
SAFE_BAT_GND
Wire Wire Line
	3700 5400 3600 5400
Wire Wire Line
	3600 5400 3600 5900
Connection ~ 3600 5900
Wire Wire Line
	3200 5225 3700 5225
Text Label 2500 1625 3    50   ~ 0
SAFE_BAT_GND
Text Label 3550 1625 3    50   ~ 0
SAFE_BAT_GND
Wire Wire Line
	1675 6500 1725 6500
Wire Wire Line
	1675 7100 2375 7100
Wire Wire Line
	1325 7100 1675 7100
Connection ~ 1675 7100
$Comp
L Isolator:PC817 U11
U 1 1 63CF08FE
P 2025 6400
F 0 "U11" H 2025 6725 50  0000 C CNN
F 1 "PC817" H 2025 6634 50  0000 C CNN
F 2 "myComponents:optoCouplerDIP4" H 1825 6200 50  0001 L CIN
F 3 "http://www.soselectronic.cz/a_info/resource/d/pc817.pdf" H 2025 6400 50  0001 L CNN
	1    2025 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1675 6650 1675 6500
Wire Wire Line
	1675 6950 1675 7100
Wire Wire Line
	1325 6950 1325 7100
Wire Wire Line
	1325 6650 1325 5900
Wire Wire Line
	1325 5900 1725 5900
Wire Wire Line
	1700 6300 1725 6300
Wire Wire Line
	1700 5700 1725 5700
$Comp
L Device:R_POT RV1
U 1 1 63DF64FA
P 2550 6550
F 0 "RV1" V 2450 6625 50  0000 R CNN
F 1 "R_POT" H 2850 6475 50  0000 R CNN
F 2 "myComponents:Potentiometer_ACP_CA9-V10_Vertical_Hole" H 2550 6550 50  0001 C CNN
F 3 "~" H 2550 6550 50  0001 C CNN
	1    2550 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 6550 2750 6550
Wire Wire Line
	2750 6550 2750 6300
Connection ~ 2750 6300
Wire Wire Line
	2750 6300 2975 6300
Wire Wire Line
	2550 6700 2550 6750
Wire Wire Line
	2975 6400 2975 6300
Connection ~ 2975 6300
Wire Wire Line
	2975 6300 3225 6300
Wire Wire Line
	2975 6700 2975 6750
Wire Wire Line
	2975 6750 2550 6750
Connection ~ 2550 6750
Wire Wire Line
	2550 6750 2550 6775
Text Label 7825 1850 0    50   ~ 0
Vout_ON
Text Label 7825 1750 0    50   ~ 0
Vout_OFF
Wire Wire Line
	7500 1850 7825 1850
Wire Wire Line
	7825 1750 7500 1750
Wire Wire Line
	5325 2050 6250 2050
Wire Wire Line
	5325 2150 6250 2150
Wire Wire Line
	5325 2250 6250 2250
Wire Wire Line
	5325 2350 6250 2350
Wire Wire Line
	6250 2450 5325 2450
Wire Wire Line
	5325 2550 6250 2550
Wire Wire Line
	6250 2650 5325 2650
Wire Wire Line
	6250 2750 5325 2750
Wire Wire Line
	6250 2850 5325 2850
$Comp
L power:+3.3V #PWR0103
U 1 1 63EBB881
P 5400 1775
F 0 "#PWR0103" H 5400 1625 50  0001 C CNN
F 1 "+3.3V" H 5415 1948 50  0000 C CNN
F 2 "" H 5400 1775 50  0001 C CNN
F 3 "" H 5400 1775 50  0001 C CNN
	1    5400 1775
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 1850 5400 1850
Wire Wire Line
	5400 1850 5400 1775
$Comp
L power:+5V #PWR0104
U 1 1 63ECC4A0
P 5625 1775
F 0 "#PWR0104" H 5625 1625 50  0001 C CNN
F 1 "+5V" H 5640 1948 50  0000 C CNN
F 2 "" H 5625 1775 50  0001 C CNN
F 3 "" H 5625 1775 50  0001 C CNN
	1    5625 1775
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 1950 5625 1950
Wire Wire Line
	5625 1950 5625 1775
$Comp
L Connector:Conn_01x12_Male J3
U 1 1 63F520BD
P 5125 2350
F 0 "J3" H 5233 2939 50  0000 C CNN
F 1 "Conn_01x12_Male" H 5233 2940 50  0001 C CNN
F 2 "myComponents:PinHeader_1x12_P2.54mm_widePads" H 5125 2350 50  0001 C CNN
F 3 "~" H 5125 2350 50  0001 C CNN
	1    5125 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 2950 5575 2950
Wire Wire Line
	5575 3125 5575 2950
Connection ~ 5575 2950
Wire Wire Line
	5575 2950 6250 2950
$Comp
L power:GND #PWR0131
U 1 1 6398F99C
P 1550 1975
F 0 "#PWR0131" H 1550 1725 50  0001 C CNN
F 1 "GND" H 1555 1802 50  0000 C CNN
F 2 "" H 1550 1975 50  0001 C CNN
F 3 "" H 1550 1975 50  0001 C CNN
	1    1550 1975
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR0130
U 1 1 63985720
P 1175 1900
F 0 "#PWR0130" H 1175 1750 50  0001 C CNN
F 1 "+BATT" H 1190 2073 50  0000 C CNN
F 2 "" H 1175 1900 50  0001 C CNN
F 3 "" H 1175 1900 50  0001 C CNN
	1    1175 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	1175 1900 1000 1900
Wire Wire Line
	1000 1700 1150 1700
Wire Wire Line
	1550 1700 1550 1975
Wire Wire Line
	1000 1800 1150 1800
Wire Wire Line
	1150 1800 1150 1700
Connection ~ 1150 1700
Wire Wire Line
	1150 1700 1550 1700
Wire Wire Line
	3000 3350 3150 3350
$EndSCHEMATC

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
P 6500 2775
F 0 "U3" H 7025 2890 50  0000 C CNN
F 1 "ESP32-WROOM-32U" H 7025 2799 50  0000 C CNN
F 2 "myComponents:ESP32-WROOM-32U" H 6500 2775 50  0001 C CNN
F 3 "" H 6500 2775 50  0001 C CNN
	1    6500 2775
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 6367454E
P 9300 1825
F 0 "D1" V 9339 1708 50  0000 R CNN
F 1 "LED" V 9248 1708 50  0000 R CNN
F 2 "" H 9300 1825 50  0001 C CNN
F 3 "~" H 9300 1825 50  0001 C CNN
	1    9300 1825
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 63674A9A
P 9300 2175
F 0 "R1" H 9370 2221 50  0000 L CNN
F 1 "R" H 9370 2130 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 9230 2175 50  0001 C CNN
F 3 "~" H 9300 2175 50  0001 C CNN
	1    9300 2175
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 6367B8CB
P 9650 1825
F 0 "D2" V 9689 1708 50  0000 R CNN
F 1 "LED" V 9598 1708 50  0000 R CNN
F 2 "" H 9650 1825 50  0001 C CNN
F 3 "~" H 9650 1825 50  0001 C CNN
	1    9650 1825
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 6367B8D1
P 9650 2175
F 0 "R2" H 9720 2221 50  0000 L CNN
F 1 "R" H 9720 2130 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 9580 2175 50  0001 C CNN
F 3 "~" H 9650 2175 50  0001 C CNN
	1    9650 2175
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 6367FD9D
P 9300 2375
F 0 "#PWR0103" H 9300 2125 50  0001 C CNN
F 1 "GND" H 9305 2202 50  0000 C CNN
F 2 "" H 9300 2375 50  0001 C CNN
F 3 "" H 9300 2375 50  0001 C CNN
	1    9300 2375
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 636800D7
P 9650 2375
F 0 "#PWR0104" H 9650 2125 50  0001 C CNN
F 1 "GND" H 9655 2202 50  0000 C CNN
F 2 "" H 9650 2375 50  0001 C CNN
F 3 "" H 9650 2375 50  0001 C CNN
	1    9650 2375
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
$Comp
L power:VBUS #PWR0105
U 1 1 6368C64B
P 4250 1625
F 0 "#PWR0105" H 4250 1475 50  0001 C CNN
F 1 "VBUS" H 4265 1798 50  0000 C CNN
F 2 "" H 4250 1625 50  0001 C CNN
F 3 "" H 4250 1625 50  0001 C CNN
	1    4250 1625
	-1   0    0    1   
$EndComp
Wire Wire Line
	4250 1625 4250 1550
Connection ~ 4250 1550
$Comp
L power:VBUS #PWR0106
U 1 1 6368D6CC
P 3200 1625
F 0 "#PWR0106" H 3200 1475 50  0001 C CNN
F 1 "VBUS" H 3215 1798 50  0000 C CNN
F 2 "" H 3200 1625 50  0001 C CNN
F 3 "" H 3200 1625 50  0001 C CNN
	1    3200 1625
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 1625 3200 1550
Connection ~ 3200 1550
$Comp
L power:GND #PWR0107
U 1 1 6368F612
P 2500 1625
F 0 "#PWR0107" H 2500 1375 50  0001 C CNN
F 1 "GND" H 2505 1452 50  0000 C CNN
F 2 "" H 2500 1625 50  0001 C CNN
F 3 "" H 2500 1625 50  0001 C CNN
	1    2500 1625
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1625 2500 1550
Wire Wire Line
	2600 1400 2600 1550
Wire Wire Line
	2600 1550 2500 1550
Connection ~ 2500 1550
Wire Wire Line
	2500 1550 2500 1400
$Comp
L power:GND #PWR0108
U 1 1 63690EEC
P 3550 1625
F 0 "#PWR0108" H 3550 1375 50  0001 C CNN
F 1 "GND" H 3555 1452 50  0000 C CNN
F 2 "" H 3550 1625 50  0001 C CNN
F 3 "" H 3550 1625 50  0001 C CNN
	1    3550 1625
	1    0    0    -1  
$EndComp
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
L Connector:Conn_01x02_Female J3
U 1 1 636AEFAA
P 6300 1225
F 0 "J3" V 6238 1037 50  0000 R CNN
F 1 "Conn_01x02_Female" V 6147 1037 50  0000 R CNN
F 2 "" H 6300 1225 50  0001 C CNN
F 3 "~" H 6300 1225 50  0001 C CNN
	1    6300 1225
	0    -1   -1   0   
$EndComp
$Comp
L power:+BATT #PWR0109
U 1 1 636B13FD
P 6475 1650
F 0 "#PWR0109" H 6475 1500 50  0001 C CNN
F 1 "+BATT" H 6490 1823 50  0000 C CNN
F 2 "" H 6475 1650 50  0001 C CNN
F 3 "" H 6475 1650 50  0001 C CNN
	1    6475 1650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 636B1A85
P 6225 1650
F 0 "#PWR0110" H 6225 1400 50  0001 C CNN
F 1 "GND" H 6230 1477 50  0000 C CNN
F 2 "" H 6225 1650 50  0001 C CNN
F 3 "" H 6225 1650 50  0001 C CNN
	1    6225 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6225 1650 6225 1525
Wire Wire Line
	6225 1525 6300 1525
Wire Wire Line
	6300 1525 6300 1425
Wire Wire Line
	6400 1425 6400 1525
Wire Wire Line
	6400 1525 6475 1525
Wire Wire Line
	6475 1525 6475 1650
$Comp
L Device:R R5
U 1 1 636D3A5B
P 3875 2775
F 0 "R5" H 3945 2821 50  0000 L CNN
F 1 "20M" H 3945 2730 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 3805 2775 50  0001 C CNN
F 3 "~" H 3875 2775 50  0001 C CNN
	1    3875 2775
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 636D671F
P 3875 3175
F 0 "R6" H 3945 3221 50  0000 L CNN
F 1 "2M" H 3945 3130 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 3805 3175 50  0001 C CNN
F 3 "~" H 3875 3175 50  0001 C CNN
	1    3875 3175
	1    0    0    -1  
$EndComp
Text Label 3800 2975 2    50   ~ 0
VP_PIN
$Comp
L power:+BATT #PWR0111
U 1 1 636D9491
P 3875 2550
F 0 "#PWR0111" H 3875 2400 50  0001 C CNN
F 1 "+BATT" H 3890 2723 50  0000 C CNN
F 2 "" H 3875 2550 50  0001 C CNN
F 3 "" H 3875 2550 50  0001 C CNN
	1    3875 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3875 2625 3875 2550
Text Label 6125 3175 2    50   ~ 0
VP_PIN
Wire Wire Line
	6125 3175 6400 3175
Wire Wire Line
	6400 3275 6125 3275
Text Label 6125 3275 2    50   ~ 0
VN_PIN
$Comp
L myComponents:5V5ADCDC U2
U 1 1 6369B2DC
P 2075 2875
F 0 "U2" H 2375 3040 50  0000 C CNN
F 1 "5V5ADCDC" H 2375 2949 50  0000 C CNN
F 2 "" H 2075 2875 50  0001 C CNN
F 3 "" H 2075 2875 50  0001 C CNN
	1    2075 2875
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 6369B8AA
P 1900 3350
F 0 "#PWR0112" H 1900 3100 50  0001 C CNN
F 1 "GND" H 1905 3177 50  0000 C CNN
F 2 "" H 1900 3350 50  0001 C CNN
F 3 "" H 1900 3350 50  0001 C CNN
	1    1900 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3075 1975 3075
$Comp
L power:GND #PWR0113
U 1 1 6369D246
P 2825 3350
F 0 "#PWR0113" H 2825 3100 50  0001 C CNN
F 1 "GND" H 2830 3177 50  0000 C CNN
F 2 "" H 2825 3350 50  0001 C CNN
F 3 "" H 2825 3350 50  0001 C CNN
	1    2825 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2825 3075 2775 3075
$Comp
L power:+BATT #PWR0114
U 1 1 6369F36E
P 1500 2550
F 0 "#PWR0114" H 1500 2400 50  0001 C CNN
F 1 "+BATT" H 1515 2723 50  0000 C CNN
F 2 "" H 1500 2550 50  0001 C CNN
F 3 "" H 1500 2550 50  0001 C CNN
	1    1500 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1975 2975 1900 2975
Wire Wire Line
	2775 2975 3225 2975
Wire Wire Line
	3225 2975 3225 2925
$Comp
L power:+5V #PWR0115
U 1 1 636A3668
P 3225 2925
F 0 "#PWR0115" H 3225 2775 50  0001 C CNN
F 1 "+5V" H 3240 3098 50  0000 C CNN
F 2 "" H 3225 2925 50  0001 C CNN
F 3 "" H 3225 2925 50  0001 C CNN
	1    3225 2925
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 636A4A02
P 3225 3150
F 0 "C1" H 3340 3196 50  0000 L CNN
F 1 "C" H 3340 3105 50  0000 L CNN
F 2 "" H 3263 3000 50  0001 C CNN
F 3 "~" H 3225 3150 50  0001 C CNN
	1    3225 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3225 3000 3225 2975
Connection ~ 3225 2975
Wire Wire Line
	3225 3300 3225 3350
$Comp
L power:GND #PWR0116
U 1 1 636A7644
P 3225 3350
F 0 "#PWR0116" H 3225 3100 50  0001 C CNN
F 1 "GND" H 3230 3177 50  0000 C CNN
F 2 "" H 3225 3350 50  0001 C CNN
F 3 "" H 3225 3350 50  0001 C CNN
	1    3225 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2825 3075 2825 3350
Wire Wire Line
	1900 3350 1900 3075
$Comp
L Device:R R8
U 1 1 636C3BB0
P 1500 2775
F 0 "R8" H 1430 2729 50  0000 R CNN
F 1 "R" H 1430 2820 50  0000 R CNN
F 2 "" V 1430 2775 50  0001 C CNN
F 3 "~" H 1500 2775 50  0001 C CNN
	1    1500 2775
	-1   0    0    1   
$EndComp
$Comp
L Device:R R4
U 1 1 636C4758
P 1250 2775
F 0 "R4" H 1180 2729 50  0000 R CNN
F 1 "R" H 1180 2820 50  0000 R CNN
F 2 "" V 1180 2775 50  0001 C CNN
F 3 "~" H 1250 2775 50  0001 C CNN
	1    1250 2775
	-1   0    0    1   
$EndComp
$Comp
L Device:D D3
U 1 1 636C8541
P 1750 2975
F 0 "D3" H 1750 2759 50  0000 C CNN
F 1 "D" H 1750 2850 50  0000 C CNN
F 2 "" H 1750 2975 50  0001 C CNN
F 3 "~" H 1750 2975 50  0001 C CNN
	1    1750 2975
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 2975 1500 2975
Wire Wire Line
	1500 2975 1500 2925
Wire Wire Line
	1500 2625 1500 2550
Wire Wire Line
	1250 2625 1250 2550
Wire Wire Line
	1250 2550 1500 2550
Connection ~ 1500 2550
$Comp
L power:VBUS #PWR0117
U 1 1 636DD4C3
P 1250 3050
F 0 "#PWR0117" H 1250 2900 50  0001 C CNN
F 1 "VBUS" H 1265 3223 50  0000 C CNN
F 2 "" H 1250 3050 50  0001 C CNN
F 3 "" H 1250 3050 50  0001 C CNN
	1    1250 3050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R9
U 1 1 636E97EA
P 4200 2775
F 0 "R9" H 4270 2821 50  0000 L CNN
F 1 "20M" H 4270 2730 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 4130 2775 50  0001 C CNN
F 3 "~" H 4200 2775 50  0001 C CNN
	1    4200 2775
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 636E97F0
P 4200 3175
F 0 "R10" H 4270 3221 50  0000 L CNN
F 1 "2M" H 4270 3130 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 4130 3175 50  0001 C CNN
F 3 "~" H 4200 3175 50  0001 C CNN
	1    4200 3175
	1    0    0    -1  
$EndComp
Text Label 4275 2975 0    50   ~ 0
VN_PIN
Wire Wire Line
	4200 2625 4200 2550
Text Label 4200 2550 1    50   ~ 0
V_SOLAR
Wire Wire Line
	3875 2925 3875 2975
Wire Wire Line
	3800 2975 3875 2975
Connection ~ 3875 2975
Wire Wire Line
	3875 2975 3875 3025
Wire Wire Line
	4200 2925 4200 2975
Wire Wire Line
	4275 2975 4200 2975
Connection ~ 4200 2975
Wire Wire Line
	4200 2975 4200 3025
$Comp
L Transistor_FET:IRF3205 Q1
U 1 1 63941878
P 1900 6850
F 0 "Q1" H 2105 6896 50  0000 L CNN
F 1 "IRF3205" H 2025 6700 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 2150 6775 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf3205.pdf" H 1900 6850 50  0001 L CNN
	1    1900 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 63942885
P 1650 7050
F 0 "R7" H 1720 7096 50  0000 L CNN
F 1 "10K" H 1720 7005 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 1580 7050 50  0001 C CNN
F 3 "~" H 1650 7050 50  0001 C CNN
	1    1650 7050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 63942E04
P 1450 6850
F 0 "R3" V 1243 6850 50  0000 C CNN
F 1 "470R" V 1334 6850 50  0000 C CNN
F 2 "myComponents:resistor0.25,9mm" V 1380 6850 50  0001 C CNN
F 3 "~" H 1450 6850 50  0001 C CNN
	1    1450 6850
	0    1    1    0   
$EndComp
Wire Wire Line
	1600 6850 1650 6850
Wire Wire Line
	1650 6900 1650 6850
Connection ~ 1650 6850
Wire Wire Line
	1650 6850 1700 6850
$Comp
L power:GND #PWR0118
U 1 1 639486AE
P 2000 7250
F 0 "#PWR0118" H 2000 7000 50  0001 C CNN
F 1 "GND" H 2005 7077 50  0000 C CNN
F 2 "" H 2000 7250 50  0001 C CNN
F 3 "" H 2000 7250 50  0001 C CNN
	1    2000 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 6394AD35
P 1650 7250
F 0 "#PWR0119" H 1650 7000 50  0001 C CNN
F 1 "GND" H 1655 7077 50  0000 C CNN
F 2 "" H 1650 7250 50  0001 C CNN
F 3 "" H 1650 7250 50  0001 C CNN
	1    1650 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 7250 1650 7200
$Comp
L Transistor_FET:IRF3205 Q2
U 1 1 6394FCAA
P 2250 6625
F 0 "Q2" H 2150 6750 50  0000 L CNN
F 1 "IRF3205" H 2025 6825 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 2500 6550 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf3205.pdf" H 2250 6625 50  0001 L CNN
	1    2250 6625
	1    0    0    -1  
$EndComp
Text Label 1375 6375 2    50   ~ 0
LOAD0_SOURCE
Wire Wire Line
	1300 6850 1250 6850
Text Label 1250 6850 2    50   ~ 0
LOAD0_GATE
Wire Wire Line
	2050 6625 1650 6625
Wire Wire Line
	1650 6625 1650 6850
Wire Wire Line
	2350 6425 2350 6375
Wire Wire Line
	2350 6375 2000 6375
Wire Wire Line
	2000 6650 2000 6375
Connection ~ 2000 6375
Wire Wire Line
	2000 6375 1375 6375
Wire Wire Line
	2000 7050 2000 7250
$Comp
L power:GND #PWR0120
U 1 1 63986317
P 2350 7250
F 0 "#PWR0120" H 2350 7000 50  0001 C CNN
F 1 "GND" H 2355 7077 50  0000 C CNN
F 2 "" H 2350 7250 50  0001 C CNN
F 3 "" H 2350 7250 50  0001 C CNN
	1    2350 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 7250 2350 6825
$Comp
L Transistor_FET:IRF3205 Q3
U 1 1 639A8579
P 3550 6850
F 0 "Q3" H 3755 6896 50  0000 L CNN
F 1 "IRF3205" H 3675 6700 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3800 6775 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf3205.pdf" H 3550 6850 50  0001 L CNN
	1    3550 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 639A857F
P 3300 7050
F 0 "R12" H 3370 7096 50  0000 L CNN
F 1 "10K" H 3370 7005 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 3230 7050 50  0001 C CNN
F 3 "~" H 3300 7050 50  0001 C CNN
	1    3300 7050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 639A8585
P 3100 6850
F 0 "R11" V 2893 6850 50  0000 C CNN
F 1 "470R" V 2984 6850 50  0000 C CNN
F 2 "myComponents:resistor0.25,9mm" V 3030 6850 50  0001 C CNN
F 3 "~" H 3100 6850 50  0001 C CNN
	1    3100 6850
	0    1    1    0   
$EndComp
Wire Wire Line
	3250 6850 3300 6850
Wire Wire Line
	3300 6900 3300 6850
Connection ~ 3300 6850
Wire Wire Line
	3300 6850 3350 6850
$Comp
L power:GND #PWR0121
U 1 1 639A858F
P 3650 7250
F 0 "#PWR0121" H 3650 7000 50  0001 C CNN
F 1 "GND" H 3655 7077 50  0000 C CNN
F 2 "" H 3650 7250 50  0001 C CNN
F 3 "" H 3650 7250 50  0001 C CNN
	1    3650 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 639A8595
P 3300 7250
F 0 "#PWR0122" H 3300 7000 50  0001 C CNN
F 1 "GND" H 3305 7077 50  0000 C CNN
F 2 "" H 3300 7250 50  0001 C CNN
F 3 "" H 3300 7250 50  0001 C CNN
	1    3300 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 7250 3300 7200
$Comp
L Transistor_FET:IRF3205 Q4
U 1 1 639A859C
P 3900 6625
F 0 "Q4" H 3800 6750 50  0000 L CNN
F 1 "IRF3205" H 3675 6825 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4150 6550 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf3205.pdf" H 3900 6625 50  0001 L CNN
	1    3900 6625
	1    0    0    -1  
$EndComp
Text Label 3025 6375 2    50   ~ 0
LOAD1_SOURCE
Wire Wire Line
	2950 6850 2900 6850
Text Label 2900 6850 2    50   ~ 0
LOAD1_GATE
Wire Wire Line
	3700 6625 3300 6625
Wire Wire Line
	3300 6625 3300 6850
Wire Wire Line
	4000 6425 4000 6375
Wire Wire Line
	4000 6375 3650 6375
Wire Wire Line
	3650 6650 3650 6375
Connection ~ 3650 6375
Wire Wire Line
	3650 6375 3025 6375
Wire Wire Line
	3650 7050 3650 7250
$Comp
L power:GND #PWR0123
U 1 1 639A85AD
P 4000 7250
F 0 "#PWR0123" H 4000 7000 50  0001 C CNN
F 1 "GND" H 4005 7077 50  0000 C CNN
F 2 "" H 4000 7250 50  0001 C CNN
F 3 "" H 4000 7250 50  0001 C CNN
	1    4000 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 7250 4000 6825
Wire Wire Line
	3875 3450 4200 3450
Wire Wire Line
	3875 3325 3875 3450
Wire Wire Line
	4200 3325 4200 3450
Text Label 4275 3450 0    50   ~ 0
VREAD_ENABLED
Wire Wire Line
	4275 3450 4200 3450
Connection ~ 4200 3450
$Comp
L power:+3.3V #PWR0124
U 1 1 639B49F1
P 6350 2925
F 0 "#PWR0124" H 6350 2775 50  0001 C CNN
F 1 "+3.3V" H 6365 3098 50  0000 C CNN
F 2 "" H 6350 2925 50  0001 C CNN
F 3 "" H 6350 2925 50  0001 C CNN
	1    6350 2925
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2975 6350 2975
Wire Wire Line
	6350 2975 6350 2925
Text Label 9550 1100 2    50   ~ 0
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
P 9975 2375
F 0 "#PWR0126" H 9975 2125 50  0001 C CNN
F 1 "GND" H 9980 2202 50  0000 C CNN
F 2 "" H 9975 2375 50  0001 C CNN
F 3 "" H 9975 2375 50  0001 C CNN
	1    9975 2375
	1    0    0    -1  
$EndComp
Text Label 9550 1350 2    50   ~ 0
uTX_SIM7000
Text Label 9550 1450 2    50   ~ 0
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
F 2 "" H 10150 950 50  0001 C CNN
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
	9650 1675 9650 1350
Wire Wire Line
	9650 1350 10050 1350
Wire Wire Line
	9650 1350 9550 1350
Connection ~ 9650 1350
Wire Wire Line
	9550 1450 9600 1450
Wire Wire Line
	9600 1450 9600 1650
Wire Wire Line
	9600 1650 9300 1650
Wire Wire Line
	9300 1650 9300 1675
Connection ~ 9600 1450
Wire Wire Line
	9600 1450 10050 1450
Wire Wire Line
	9300 2025 9300 1975
Wire Wire Line
	9650 2025 9650 1975
Wire Wire Line
	9650 2375 9650 2325
Wire Wire Line
	9300 2375 9300 2325
Wire Wire Line
	9975 1800 9975 2375
Wire Wire Line
	10050 1700 9975 1700
Wire Wire Line
	9975 1700 9975 1800
Connection ~ 9975 1800
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 63AD6777
P 9775 900
F 0 "JP2" H 9575 850 50  0000 L CNN
F 1 "SolderJumper_2_Open" H 9775 1014 50  0001 C CNN
F 2 "" H 9775 900 50  0001 C CNN
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
Wire Wire Line
	9550 1100 10050 1100
$Comp
L myComponents:MAX48_module U1
U 1 1 6366DB2D
P 9775 2850
F 0 "U1" H 10150 2350 50  0000 C CNN
F 1 "MAX48_module" H 10150 2450 50  0000 C CNN
F 2 "myComponents:MAX485_module" H 9775 2850 50  0001 C CNN
F 3 "" H 9775 2850 50  0001 C CNN
	1    9775 2850
	1    0    0    -1  
$EndComp
Text Label 9200 3150 2    50   ~ 0
UART2_TX
Text Label 9200 2850 2    50   ~ 0
UART2_RX
Text Label 9200 2950 2    50   ~ 0
UART2_DRIVER
Wire Wire Line
	9675 2950 9625 2950
Wire Wire Line
	9675 3050 9625 3050
Wire Wire Line
	9625 3050 9625 2950
Connection ~ 9625 2950
Wire Wire Line
	10625 3150 10725 3150
Wire Wire Line
	10725 3150 10725 3250
$Comp
L power:GND #PWR0101
U 1 1 63671AD2
P 10725 3250
F 0 "#PWR0101" H 10725 3000 50  0001 C CNN
F 1 "GND" H 10730 3077 50  0000 C CNN
F 2 "" H 10725 3250 50  0001 C CNN
F 3 "" H 10725 3250 50  0001 C CNN
	1    10725 3250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10725 2800 10725 2850
Wire Wire Line
	10725 2850 10625 2850
Wire Wire Line
	9275 3275 9275 2850
Wire Wire Line
	9675 2850 9275 2850
Connection ~ 9275 2850
Wire Wire Line
	9275 2850 9200 2850
Wire Wire Line
	9625 2950 9200 2950
Text Label 10725 3050 0    50   ~ 0
RS485_A
Wire Wire Line
	10725 3050 10625 3050
Text Label 10725 2950 0    50   ~ 0
RS485_B
Wire Wire Line
	10725 2950 10625 2950
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 6368CCA9
P 10725 2650
F 0 "JP1" V 10725 2718 50  0000 L CNN
F 1 "SolderJumper_2_Open" H 10725 2764 50  0001 C CNN
F 2 "" H 10725 2650 50  0001 C CNN
F 3 "~" H 10725 2650 50  0001 C CNN
	1    10725 2650
	0    -1   1    0   
$EndComp
Wire Wire Line
	10725 2500 10725 2475
$Comp
L power:+5V #PWR0102
U 1 1 63671E3D
P 10725 2475
F 0 "#PWR0102" H 10725 2325 50  0001 C CNN
F 1 "+5V" H 10740 2648 50  0000 C CNN
F 2 "" H 10725 2475 50  0001 C CNN
F 3 "" H 10725 2475 50  0001 C CNN
	1    10725 2475
	-1   0    0    -1  
$EndComp
$Comp
L Device:LED D4
U 1 1 63B33CDB
P 9275 3425
F 0 "D4" V 9314 3308 50  0000 R CNN
F 1 "LED" V 9223 3308 50  0000 R CNN
F 2 "" H 9275 3425 50  0001 C CNN
F 3 "~" H 9275 3425 50  0001 C CNN
	1    9275 3425
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R13
U 1 1 63B33CE1
P 9275 3775
F 0 "R13" H 9345 3821 50  0000 L CNN
F 1 "R" H 9345 3730 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 9205 3775 50  0001 C CNN
F 3 "~" H 9275 3775 50  0001 C CNN
	1    9275 3775
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D5
U 1 1 63B33CE7
P 9625 3425
F 0 "D5" V 9664 3308 50  0000 R CNN
F 1 "LED" V 9573 3308 50  0000 R CNN
F 2 "" H 9625 3425 50  0001 C CNN
F 3 "~" H 9625 3425 50  0001 C CNN
	1    9625 3425
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 63B33CED
P 9625 3775
F 0 "R14" H 9695 3821 50  0000 L CNN
F 1 "R" H 9695 3730 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 9555 3775 50  0001 C CNN
F 3 "~" H 9625 3775 50  0001 C CNN
	1    9625 3775
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0128
U 1 1 63B33CF3
P 9275 3975
F 0 "#PWR0128" H 9275 3725 50  0001 C CNN
F 1 "GND" H 9280 3802 50  0000 C CNN
F 2 "" H 9275 3975 50  0001 C CNN
F 3 "" H 9275 3975 50  0001 C CNN
	1    9275 3975
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0129
U 1 1 63B33CF9
P 9625 3975
F 0 "#PWR0129" H 9625 3725 50  0001 C CNN
F 1 "GND" H 9630 3802 50  0000 C CNN
F 2 "" H 9625 3975 50  0001 C CNN
F 3 "" H 9625 3975 50  0001 C CNN
	1    9625 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	9275 3625 9275 3575
Wire Wire Line
	9625 3625 9625 3575
Wire Wire Line
	9625 3975 9625 3925
Wire Wire Line
	9275 3975 9275 3925
Wire Wire Line
	9200 3150 9625 3150
Wire Wire Line
	9625 3275 9625 3150
Connection ~ 9625 3150
Wire Wire Line
	9625 3150 9675 3150
$Comp
L Connector:Conn_01x04_Female J4
U 1 1 63982CDD
P 875 1375
F 0 "J4" H 767 1042 50  0000 C CNN
F 1 "Conn_01x04_Female" H 767 1041 50  0001 C CNN
F 2 "myComponents:TerminalBlockRemovablex4" H 875 1375 50  0001 C CNN
F 3 "~" H 875 1375 50  0001 C CNN
	1    875  1375
	-1   0    0    1   
$EndComp
$Comp
L power:+BATT #PWR0130
U 1 1 63985720
P 1150 1100
F 0 "#PWR0130" H 1150 950 50  0001 C CNN
F 1 "+BATT" H 1165 1273 50  0000 C CNN
F 2 "" H 1150 1100 50  0001 C CNN
F 3 "" H 1150 1100 50  0001 C CNN
	1    1150 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1075 1175 1150 1175
Wire Wire Line
	1150 1175 1150 1100
$Comp
L power:GND #PWR0131
U 1 1 6398F99C
P 1150 1575
F 0 "#PWR0131" H 1150 1325 50  0001 C CNN
F 1 "GND" H 1155 1402 50  0000 C CNN
F 2 "" H 1150 1575 50  0001 C CNN
F 3 "" H 1150 1575 50  0001 C CNN
	1    1150 1575
	1    0    0    -1  
$EndComp
Wire Wire Line
	1075 1475 1150 1475
Wire Wire Line
	1150 1475 1150 1575
Wire Wire Line
	1075 1275 1150 1275
Wire Wire Line
	1150 1275 1150 1475
Connection ~ 1150 1475
Text Label 1250 1375 0    50   ~ 0
V_SOLAR
Wire Wire Line
	1250 1375 1075 1375
$Comp
L Switch:SW_DIP_x03 SW1
U 1 1 639DDF88
P 9350 4950
F 0 "SW1" H 9350 5417 50  0000 C CNN
F 1 "SW_DIP_x03" H 9350 5326 50  0000 C CNN
F 2 "" H 9350 4950 50  0001 C CNN
F 3 "~" H 9350 4950 50  0001 C CNN
	1    9350 4950
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF3205 Q5
U 1 1 63A9D9CB
P 5575 6850
F 0 "Q5" H 5780 6896 50  0000 L CNN
F 1 "IRF3205" H 5700 6700 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5825 6775 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf3205.pdf" H 5575 6850 50  0001 L CNN
	1    5575 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 63A9D9D1
P 5325 7050
F 0 "R16" H 5395 7096 50  0000 L CNN
F 1 "10K" H 5395 7005 50  0000 L CNN
F 2 "myComponents:resistor0.25,9mm" V 5255 7050 50  0001 C CNN
F 3 "~" H 5325 7050 50  0001 C CNN
	1    5325 7050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 63A9D9D7
P 5125 6850
F 0 "R15" V 4918 6850 50  0000 C CNN
F 1 "470R" V 5009 6850 50  0000 C CNN
F 2 "myComponents:resistor0.25,9mm" V 5055 6850 50  0001 C CNN
F 3 "~" H 5125 6850 50  0001 C CNN
	1    5125 6850
	0    1    1    0   
$EndComp
Wire Wire Line
	5275 6850 5325 6850
Wire Wire Line
	5325 6900 5325 6850
Connection ~ 5325 6850
Wire Wire Line
	5325 6850 5375 6850
$Comp
L power:GND #PWR0132
U 1 1 63A9D9E1
P 5675 7250
F 0 "#PWR0132" H 5675 7000 50  0001 C CNN
F 1 "GND" H 5680 7077 50  0000 C CNN
F 2 "" H 5675 7250 50  0001 C CNN
F 3 "" H 5675 7250 50  0001 C CNN
	1    5675 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0133
U 1 1 63A9D9E7
P 5325 7250
F 0 "#PWR0133" H 5325 7000 50  0001 C CNN
F 1 "GND" H 5330 7077 50  0000 C CNN
F 2 "" H 5325 7250 50  0001 C CNN
F 3 "" H 5325 7250 50  0001 C CNN
	1    5325 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 7250 5325 7200
$Comp
L Transistor_FET:IRF3205 Q6
U 1 1 63A9D9EE
P 5925 6625
F 0 "Q6" H 5825 6750 50  0000 L CNN
F 1 "IRF3205" H 5700 6825 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 6175 6550 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf3205.pdf" H 5925 6625 50  0001 L CNN
	1    5925 6625
	1    0    0    -1  
$EndComp
Text Label 5050 6375 2    50   ~ 0
SENSOR_PWR
Wire Wire Line
	4975 6850 4925 6850
Text Label 4925 6850 2    50   ~ 0
GATE_SENSOR_PWR
Wire Wire Line
	5725 6625 5325 6625
Wire Wire Line
	5325 6625 5325 6850
Wire Wire Line
	6025 6425 6025 6375
Wire Wire Line
	6025 6375 5675 6375
Wire Wire Line
	5675 6650 5675 6375
Connection ~ 5675 6375
Wire Wire Line
	5675 6375 5050 6375
Wire Wire Line
	5675 7050 5675 7250
$Comp
L power:GND #PWR0134
U 1 1 63A9D9FF
P 6025 7250
F 0 "#PWR0134" H 6025 7000 50  0001 C CNN
F 1 "GND" H 6030 7077 50  0000 C CNN
F 2 "" H 6025 7250 50  0001 C CNN
F 3 "" H 6025 7250 50  0001 C CNN
	1    6025 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6025 7250 6025 6825
$EndSCHEMATC

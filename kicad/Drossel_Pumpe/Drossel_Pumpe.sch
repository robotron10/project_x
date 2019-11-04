EESchema Schematic File Version 4
LIBS:Drossel_Pumpe-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Leitungsfilter für die Wasserpumpe"
Date "2019-11-03"
Rev "0.2"
Comp "STB AEA8"
Comment1 "Nawroth, Graf, Roth"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Drossel_Pumpe-rescue:L L1
U 1 1 5D25B130
P 6100 2700
F 0 "L1" V 6050 2700 50  0000 C CNN
F 1 "100µH" V 6175 2700 50  0000 C CNN
F 2 "" H 6100 2700 50  0001 C CNN
F 3 "" H 6100 2700 50  0001 C CNN
	1    6100 2700
	0    -1   -1   0   
$EndComp
$Comp
L Drossel_Pumpe-rescue:C C1
U 1 1 5D25B14C
P 6850 2950
F 0 "C1" H 6875 3050 50  0000 L CNN
F 1 "100nF" H 6875 2850 50  0000 L CNN
F 2 "" H 6888 2800 50  0001 C CNN
F 3 "" H 6850 2950 50  0001 C CNN
	1    6850 2950
	1    0    0    -1  
$EndComp
$Comp
L Drossel_Pumpe-rescue:L 100µH
U 1 1 5D25B187
P 6100 3200
F 0 "100µH" V 6050 3200 50  0000 C CNN
F 1 "L" V 6175 3200 50  0000 C CNN
F 2 "" H 6100 3200 50  0001 C CNN
F 3 "" H 6100 3200 50  0001 C CNN
	1    6100 3200
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x01 J1
U 1 1 5D25E2D9
P 5550 2900
F 0 "J1" V 5554 2980 50  0000 L CNN
F 1 "Conn_02x01" V 5645 2980 50  0000 L CNN
F 2 "" H 5550 2900 50  0001 C CNN
F 3 "~" H 5550 2900 50  0001 C CNN
	1    5550 2900
	0    -1   1    0   
$EndComp
Wire Wire Line
	5550 2700 5950 2700
Wire Wire Line
	6250 2700 6550 2700
Wire Wire Line
	5550 3200 5950 3200
Wire Wire Line
	6850 2800 6850 2700
Wire Wire Line
	6850 3100 6850 3200
$Comp
L Motor:Motor_DC M1
U 1 1 5D262970
P 7350 2900
F 0 "M1" H 7508 2896 50  0000 L CNN
F 1 "Pumpe" H 7508 2805 50  0000 L CNN
F 2 "" H 7350 2810 50  0001 C CNN
F 3 "~" H 7350 2810 50  0001 C CNN
	1    7350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2700 6850 2700
Wire Wire Line
	7350 3200 6850 3200
$Comp
L Device:D D1
U 1 1 5D268645
P 8000 2950
F 0 "D1" V 7954 3029 50  0000 L CNN
F 1 "1N4007" V 8045 3029 50  0000 L CNN
F 2 "" H 8000 2950 50  0001 C CNN
F 3 "~" H 8000 2950 50  0001 C CNN
	1    8000 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	7350 2700 8000 2700
Wire Wire Line
	8000 2700 8000 2800
Connection ~ 7350 2700
Wire Wire Line
	8000 3100 8000 3200
Wire Wire Line
	8000 3200 7350 3200
Connection ~ 7350 3200
Wire Notes Line
	7150 2550 7150 3350
Wire Notes Line
	5800 3350 5800 2550
Text Notes 6250 2500 0    79   ~ 0
Leitungsfilter
Text Notes 9250 1350 0    50   ~ 0
Infos aus dem Netz:\nSpule -> 50-100µH\nKondensator -> 47-100nF
$Comp
L Device:R R1
U 1 1 5D7D1E88
P 6550 2950
F 0 "R1" H 6480 2904 50  0000 R CNN
F 1 "1k" H 6480 2995 50  0000 R CNN
F 2 "" V 6480 2950 50  0001 C CNN
F 3 "~" H 6550 2950 50  0001 C CNN
	1    6550 2950
	1    0    0    1   
$EndComp
Wire Wire Line
	6550 2800 6550 2700
Wire Wire Line
	6550 2700 6850 2700
Connection ~ 6550 2700
Connection ~ 6850 2700
Wire Wire Line
	6250 3200 6550 3200
Connection ~ 6850 3200
Wire Wire Line
	6550 3100 6550 3200
Connection ~ 6550 3200
Wire Wire Line
	6550 3200 6850 3200
Wire Notes Line
	5800 2550 7150 2550
Wire Notes Line
	5800 3350 7150 3350
Text Notes 9250 900  0    79   ~ 0
R1 -> Entladewiderstand
$EndSCHEMATC

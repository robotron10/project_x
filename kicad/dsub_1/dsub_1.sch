EESchema Schematic File Version 4
LIBS:dsub_1-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Zuordnung Anschlüsse"
Date "2019-08-14"
Rev "0.2"
Comp "STB AEA8"
Comment1 "Nawroth, Graf, Roth"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 9200 4700 0    50   Input ~ 0
Erdfeuchte1_plus
Wire Wire Line
	9550 4700 9200 4700
Text GLabel 9200 4500 0    50   Input ~ 0
Erdfeuchte1_minus
Wire Wire Line
	9200 4500 9550 4500
Text GLabel 9200 4300 0    50   Input ~ 0
Erdfeuchte2_plus
Text GLabel 9200 4100 0    50   Input ~ 0
Erdfeuchte2_minus
Text GLabel 9200 3900 0    50   Input ~ 0
Temperatursensor_plus
Text GLabel 9200 3700 0    50   Input ~ 0
Temperatursensor_minus
Text GLabel 9200 3500 0    50   Input ~ 0
Temperatursensor_data
Text GLabel 9200 3300 0    50   Input ~ 0
Fotowiderstand_plus
Text GLabel 9200 3100 0    50   Input ~ 0
Fotowiderstand_minus
Text GLabel 9200 2900 0    50   Input ~ 0
REED-Kontakt_plus
Text GLabel 9200 2700 0    50   Input ~ 0
REED-Kontakt_minus
Text GLabel 9200 2500 0    50   Input ~ 0
LED_plus
Text GLabel 9200 2300 0    50   Input ~ 0
LED_minus
Text GLabel 7950 4600 0    50   Input ~ 0
Pumpe1_plus
Text GLabel 7950 4400 0    50   Input ~ 0
Pumpe1_minus
Text GLabel 7950 4200 0    50   Input ~ 0
Pumpe2_plus
Text GLabel 7950 4000 0    50   Input ~ 0
Pumpe2_minus
Text GLabel 7950 3800 0    50   Input ~ 0
Lüfter_plus
Text GLabel 7950 3600 0    50   Input ~ 0
Lüfter_minus
Wire Wire Line
	9550 2300 9200 2300
Wire Wire Line
	9550 2500 9200 2500
Wire Wire Line
	9550 2700 9200 2700
Wire Wire Line
	9200 2900 9550 2900
Wire Wire Line
	9200 3100 9550 3100
Wire Wire Line
	9200 3300 9550 3300
Wire Wire Line
	9200 3500 9550 3500
Wire Wire Line
	9200 3700 9550 3700
Wire Wire Line
	9200 3900 9550 3900
Wire Wire Line
	9200 4100 9550 4100
Wire Wire Line
	9200 4300 9550 4300
Wire Wire Line
	7950 4600 9550 4600
Wire Wire Line
	7950 4400 9550 4400
Wire Wire Line
	7950 4200 9550 4200
Wire Wire Line
	7950 4000 9550 4000
Wire Wire Line
	7950 3800 9550 3800
Wire Wire Line
	7950 3600 9550 3600
$Comp
L Connector:DB15_Female J1
U 1 1 5D76075D
P 5100 2500
F 0 "J1" H 5007 1508 50  0000 C CNN
F 1 "DB15_Female" H 5007 1599 50  0000 C CNN
F 2 "" H 5100 2500 50  0001 C CNN
F 3 " ~" H 5100 2500 50  0001 C CNN
	1    5100 2500
	1    0    0    1   
$EndComp
Text GLabel 4500 3200 0    50   Input ~ 0
Erdfeuchte1_plus
Text GLabel 4500 3000 0    50   Input ~ 0
Erdfeuchte1_minus
Text GLabel 4500 2800 0    50   Input ~ 0
Erdfeuchte2_plus
Text GLabel 4500 2600 0    50   Input ~ 0
Erdfeuchte2_minus
Text GLabel 4500 2400 0    50   Input ~ 0
Temperatursensor_plus
Text GLabel 4500 2200 0    50   Input ~ 0
Temperatursensor_minus
Text GLabel 4500 2000 0    50   Input ~ 0
Temperatursensor_data
Text GLabel 4500 1800 0    50   Input ~ 0
Fotowiderstand_plus
Text GLabel 3500 3100 0    50   Input ~ 0
Fotowiderstand_minus
Text GLabel 3500 2900 0    50   Input ~ 0
REED-Kontakt_plus
Text GLabel 3500 2700 0    50   Input ~ 0
REED-Kontakt_minus
Text GLabel 3650 5550 0    50   Input ~ 0
LED_plus
Text GLabel 3650 5350 0    50   Input ~ 0
LED_minus
Text GLabel 4450 5650 0    50   Input ~ 0
Pumpe1_plus
Text GLabel 4450 5450 0    50   Input ~ 0
Pumpe1_minus
Text GLabel 4450 5250 0    50   Input ~ 0
Pumpe2_plus
Text GLabel 4450 5050 0    50   Input ~ 0
Pumpe2_minus
Text GLabel 4450 4850 0    50   Input ~ 0
Lüfter_plus
Text GLabel 4450 4650 0    50   Input ~ 0
Lüfter_minus
Wire Wire Line
	4450 4850 4800 4850
Wire Wire Line
	4450 5050 4800 5050
Wire Wire Line
	4450 5250 4800 5250
Wire Wire Line
	4450 5450 4800 5450
Wire Wire Line
	4450 5650 4800 5650
Wire Wire Line
	4500 1800 4800 1800
Wire Wire Line
	4500 2000 4800 2000
Wire Wire Line
	4500 2200 4800 2200
Wire Wire Line
	4500 2400 4800 2400
Wire Wire Line
	4500 2600 4800 2600
Wire Wire Line
	4500 2800 4800 2800
Wire Wire Line
	3500 2700 4800 2700
Wire Wire Line
	4500 3000 4800 3000
Wire Wire Line
	4500 3200 4800 3200
Wire Wire Line
	3500 2900 4800 2900
Wire Wire Line
	3500 3100 4800 3100
Text Notes 6750 1450 0    118  ~ 0
Signalkabel und stromführende Kabel\nin einem Stecker -> Problem Störungen, EMV ?
Wire Notes Line
	6350 1150 6350 6300
Text Notes 3650 1300 0    118  ~ 0
Stecker Signale
Text Notes 3650 3800 0    118  ~ 0
Stecker Stromführung
Text GLabel 7950 3400 0    50   Input ~ 0
Heizung_plus
Text GLabel 7950 3200 0    50   Input ~ 0
Heizung_minus
Wire Wire Line
	7950 3400 9550 3400
Wire Wire Line
	7950 3200 9550 3200
Text GLabel 4450 4450 0    50   Input ~ 0
Heizung_plus
Text GLabel 4450 4250 0    50   Input ~ 0
Heizung_minus
$Comp
L Connector:DB25_Female J3
U 1 1 5D77214E
P 9850 3500
F 0 "J3" H 10029 3546 50  0000 L CNN
F 1 "DB25_Female" H 10029 3455 50  0000 L CNN
F 2 "" H 9850 3500 50  0001 C CNN
F 3 " ~" H 9850 3500 50  0001 C CNN
	1    9850 3500
	1    0    0    -1  
$EndComp
Text Notes 1100 7100 0    157  Italic 0
Stecker:\nTorsten -> Männchen, Male\nFabian -> Weibchen, Female
Text Notes 700  5250 0    79   ~ 0
Leistung:\nLüfter  -> 12V/0,11A\nHeizung -> 12V/4,5A  (55W)\nLEDs   -> rot 12V/80mA; blau 12V/80mA\nPumpe -> 3-5V/100-200mA\n
$Comp
L Connector:DB15_Female J2
U 1 1 5D775ECC
P 5100 4950
F 0 "J2" H 5007 3958 50  0000 C CNN
F 1 "DB15_Female" H 5007 4049 50  0000 C CNN
F 2 "" H 5100 4950 50  0001 C CNN
F 3 " ~" H 5100 4950 50  0001 C CNN
	1    5100 4950
	1    0    0    1   
$EndComp
Wire Wire Line
	4450 4250 4800 4250
Wire Wire Line
	4450 4450 4800 4450
Wire Wire Line
	4450 4650 4800 4650
Wire Wire Line
	3650 5550 4800 5550
Wire Wire Line
	3650 5350 4800 5350
$EndSCHEMATC

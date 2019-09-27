EESchema Schematic File Version 4
LIBS:dsub_1-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Zuordnung Anschlüsse"
Date "2019-09-26"
Rev "0.2"
Comp "STB AEA8"
Comment1 "Nawroth, Graf, Roth"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 3300 5450 0    50   Input ~ 0
Erdfeuchte1_plus
Wire Wire Line
	3650 5450 3300 5450
Text GLabel 3300 5250 0    50   Input ~ 0
Erdfeuchte1_minus
Wire Wire Line
	3300 5250 3650 5250
Text GLabel 3300 5050 0    50   Input ~ 0
Erdfeuchte2_plus
Text GLabel 3300 4850 0    50   Input ~ 0
Erdfeuchte2_minus
Text GLabel 3300 4650 0    50   Input ~ 0
Temperatursensor_plus
Text GLabel 3300 4450 0    50   Input ~ 0
Temperatursensor_minus
Text GLabel 3300 4250 0    50   Input ~ 0
Temperatursensor_data
Text GLabel 3300 4050 0    50   Input ~ 0
Fotowiderstand_plus
Text GLabel 3300 3850 0    50   Input ~ 0
Fotowiderstand_minus
Text GLabel 3300 3650 0    50   Input ~ 0
REED-Kontakt_plus
Text GLabel 3300 3450 0    50   Input ~ 0
REED-Kontakt_minus
Text GLabel 3300 3250 0    50   Input ~ 0
LED_plus
Text GLabel 3300 3050 0    50   Input ~ 0
LED_minus
Text GLabel 2050 5350 0    50   Input ~ 0
Pumpe1_plus
Text GLabel 2050 5150 0    50   Input ~ 0
Pumpe1_minus
Text GLabel 2050 4950 0    50   Input ~ 0
Pumpe2_plus
Text GLabel 2050 4750 0    50   Input ~ 0
Pumpe2_minus
Text GLabel 2050 4550 0    50   Input ~ 0
Lüfter_plus
Text GLabel 2050 4350 0    50   Input ~ 0
Lüfter_minus
Wire Wire Line
	3650 3050 3300 3050
Wire Wire Line
	3650 3250 3300 3250
Wire Wire Line
	3650 3450 3300 3450
Wire Wire Line
	3300 3650 3650 3650
Wire Wire Line
	3300 3850 3650 3850
Wire Wire Line
	3300 4050 3650 4050
Wire Wire Line
	3300 4250 3650 4250
Wire Wire Line
	3300 4450 3650 4450
Wire Wire Line
	3300 4650 3650 4650
Wire Wire Line
	3300 4850 3650 4850
Wire Wire Line
	3300 5050 3650 5050
Wire Wire Line
	2050 5350 3650 5350
Wire Wire Line
	2050 5150 3650 5150
Wire Wire Line
	2050 4950 3650 4950
Wire Wire Line
	2050 4750 3650 4750
Wire Wire Line
	2050 4550 3650 4550
Wire Wire Line
	2050 4350 3650 4350
Text GLabel 6800 4550 0    50   Input ~ 0
Heizung_plus
Text GLabel 6800 4350 0    50   Input ~ 0
Heizung_minus
$Comp
L Connector:DB25_Female J3
U 1 1 5D77214E
P 3950 4250
F 0 "J3" H 4129 4296 50  0000 L CNN
F 1 "DB25_Female" H 4129 4205 50  0000 L CNN
F 2 "" H 3950 4250 50  0001 C CNN
F 3 " ~" H 3950 4250 50  0001 C CNN
	1    3950 4250
	1    0    0    -1  
$EndComp
Text Notes 900  7350 0    157  Italic 0
Stecker:\nTorsten -> Männchen, Male\nFabian -> Weibchen, Female
Text Notes 950  1400 0    79   ~ 0
Leistung:\nHeizung -> 12V/4,5A  (55W)\nPumpe -> 3-5V/200-360mA\nLüfter  -> 12V/0,11A\nLEDs   -> rot 12V/80mA; blau 12V/80mA
Text Notes 6200 4100 0    79   ~ 0
extra Stecker, \nweil zuviel Strom für SubD
Wire Notes Line
	5950 3800 8050 3800
Wire Notes Line
	8050 3800 8050 4700
Wire Notes Line
	8050 4700 5950 4700
Wire Notes Line
	5950 4700 5950 3800
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5D7EA451
P 7450 4400
F 0 "J?" H 7530 4392 50  0000 L CNN
F 1 "Conn_01x02" H 7530 4301 50  0000 L CNN
F 2 "" H 7450 4400 50  0001 C CNN
F 3 "~" H 7450 4400 50  0001 C CNN
	1    7450 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 4350 7050 4350
Wire Wire Line
	7050 4350 7050 4400
Wire Wire Line
	7050 4400 7250 4400
Wire Wire Line
	7250 4500 7050 4500
Wire Wire Line
	7050 4500 7050 4550
Wire Wire Line
	7050 4550 6800 4550
$EndSCHEMATC

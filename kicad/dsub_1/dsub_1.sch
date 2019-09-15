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
Text GLabel 2550 3550 0    50   Input ~ 0
Heizung_plus
Text GLabel 2550 3350 0    50   Input ~ 0
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
Text Notes 7400 5750 0    79   ~ 0
Leistung:\nHeizung -> 12V/4,5A  (55W)\nPumpe -> 3-5V/200-360mA\nLüfter  -> 12V/0,11A\nLEDs   -> rot 12V/80mA; blau 12V/80mA
Text Notes 1950 3100 0    79   ~ 0
extra Stecker, \nweil zuviel Strom für SubD
Wire Notes Line
	1700 2800 3800 2800
Wire Notes Line
	3800 2800 3800 3700
Wire Notes Line
	3800 3700 1700 3700
Wire Notes Line
	1700 3700 1700 2800
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5D7EA451
P 3200 3400
F 0 "J?" H 3280 3392 50  0000 L CNN
F 1 "Conn_01x02" H 3280 3301 50  0000 L CNN
F 2 "" H 3200 3400 50  0001 C CNN
F 3 "~" H 3200 3400 50  0001 C CNN
	1    3200 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3350 2800 3350
Wire Wire Line
	2800 3350 2800 3400
Wire Wire Line
	2800 3400 3000 3400
Wire Wire Line
	3000 3500 2800 3500
Wire Wire Line
	2800 3500 2800 3550
Wire Wire Line
	2800 3550 2550 3550
$EndSCHEMATC

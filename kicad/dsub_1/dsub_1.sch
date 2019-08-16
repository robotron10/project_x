EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Zuordnung Anschlüsse"
Date "2019-08-14"
Rev ""
Comp "DB"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:DB25_Male J?
U 1 1 5D53CC8A
P 7800 3450
F 0 "J?" H 7980 3496 50  0000 L CNN
F 1 "DB25_Male" H 7980 3405 50  0000 L CNN
F 2 "" H 7800 3450 50  0001 C CNN
F 3 " ~" H 7800 3450 50  0001 C CNN
	1    7800 3450
	1    0    0    -1  
$EndComp
Text GLabel 7150 4650 0    50   Input ~ 0
Erdfeuchte1_plus
Wire Wire Line
	7500 4650 7150 4650
Text GLabel 7150 4450 0    50   Input ~ 0
Erdfeuchte1_minus
Wire Wire Line
	7150 4450 7500 4450
Text GLabel 7150 4250 0    50   Input ~ 0
Erdfeuchte2_plus
Text GLabel 7150 4050 0    50   Input ~ 0
Erdfeuchte2_minus
Text GLabel 7150 3850 0    50   Input ~ 0
Temperatursensor_plus
Text GLabel 7150 3650 0    50   Input ~ 0
Temperatursensor_minus
Text GLabel 7150 3450 0    50   Input ~ 0
Temperatursensor_data
Text GLabel 7150 3250 0    50   Input ~ 0
Fotowiderstand_plus
Text GLabel 7150 3050 0    50   Input ~ 0
Fotowiderstand_minus
Text GLabel 7150 2850 0    50   Input ~ 0
REED-Kontakt_plus
Text GLabel 7150 2650 0    50   Input ~ 0
REED-Kontakt_minus
Text GLabel 7150 2450 0    50   Input ~ 0
LED_plus
Text GLabel 7150 2250 0    50   Input ~ 0
LED_minus
Text GLabel 5900 4550 0    50   Input ~ 0
Pumpe1_plus
Text GLabel 5900 4350 0    50   Input ~ 0
Pumpe1_minus
Text GLabel 5900 4150 0    50   Input ~ 0
Pumpe2_plus
Text GLabel 5900 3950 0    50   Input ~ 0
Pumpe2_minus
Text GLabel 5900 3750 0    50   Input ~ 0
Lüfter_plus
Text GLabel 5900 3550 0    50   Input ~ 0
Lüfter_minus
Wire Wire Line
	7500 2250 7150 2250
Wire Wire Line
	7500 2450 7150 2450
Wire Wire Line
	7500 2650 7150 2650
Wire Wire Line
	7150 2850 7500 2850
Wire Wire Line
	7150 3050 7500 3050
Wire Wire Line
	7150 3250 7500 3250
Wire Wire Line
	7150 3450 7500 3450
Wire Wire Line
	7150 3650 7500 3650
Wire Wire Line
	7150 3850 7500 3850
Wire Wire Line
	7150 4050 7500 4050
Wire Wire Line
	7150 4250 7500 4250
Wire Wire Line
	5900 4550 7500 4550
Wire Wire Line
	5900 4350 7500 4350
Wire Wire Line
	5900 4150 7500 4150
Wire Wire Line
	5900 3950 7500 3950
Wire Wire Line
	5900 3750 7500 3750
Wire Wire Line
	5900 3550 7500 3550
$EndSCHEMATC

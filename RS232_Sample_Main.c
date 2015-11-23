/*
 * RS232_Testprogramm.c
 *
 * Demonstriert die interruptgesteuerte, serielle Kommunikation mit Hardware- UART, hier ATMega32
 *
 * unter Verwendung von "rs232.c (Header-Datei: rs232.h)"
 *
 *
 * Empfangene Daten werden im Hintergrund, interruptgesteuert empfangen und in einem Puffer-Array gespeichert.
 * War das letzte empfangene Zeichen ein Zeilenumbruch (CR) oder ist der Puffer voll wird in der Empfangs-Interruptroutine ein Flag gesetzt,
 * Dieses Flag wird in der Mainfunktion in einer Endlosschleife abgefragt, ist es gesetzt werden die empfangenen Daten wieder �ber den UART gesendet und der
 * Pufferz�hler und das Flag anschliessend zur�ckgesetzt.
 *
 *
 * Created: 22.02.2013 22:32:57
 * Author: Scheik
 */

#include <avr/io.h>																	// Standard-Headerdatei(avr-libc)
#include <avr/interrupt.h>															// Standard-Headerdatei(avr-libc): Funktionen in diesem Modul erm�glichen das Handhaben von Interrupts
#include <avr/pgmspace.h>															// Standard-Headerdatei(avr-libc): Funktionen in diesem Modul erm�glichen Zugriff auf Daten aus dem Flash-Memory
#include "global.h"																	// Headerdatei f�r globale Definitionen wie zB. Taktfrequenz FCPU (hier 16MHz)
#include "rs232.h"																	// Headerdatei f�r rs232.c

const char FlashString[] PROGMEM = ("AVR gestartet..."CR);								// String im Flash-Memory ablegen.

int main(void)																		// Main-Funktion mit Hauptschleife
{
	init_uart();																	// UART initalisieren und einschalten. Alle n�tigen Schnittstellen-Parameter und -Funktionen werden in rs232.h definiert
	sei();																			// alle Interrupts freigeben
	uart_puts_p(FlashString);														// Demonstriert "rs232.c/uarts_put_p" f�r die Ausgabe eines Strings
	while(1)																		// Main- Endlosschleife
    {
		if (UART_MSG_FLAG==1)														// UART_MSG_FLAG auswerten: gesetzt in Empfangs- Interruptroutine wenn "CR" empfangen oder UART- Puffer voll
		{
			uart_puts ("Daten empfangen: ");										// Demonstriert "rs232.c/uarts_put_p" f�r die Ausgabe eines Strings. Dieser String wird vom Flash-Programmspeicher nochmals ins SRAM geladen, f�r sich wiederholende Strings, also besser "uart_puts_p" benutzen, siehe oben
			uint8_t i;																// Hilfsvariable f�r For-Next-Schleifenz�hler
			for (i=0;i<UART_RxCount;i++) {uart_putc(UART_RXBuffer[i]);}				// Empfangene Daten aus dem Puffer wieder �ber UART senden
			uart_puts(CR);															// Sende "CR" (Zeilenumbruch)
			UART_RxCount=0;															// setze die Z�hlervariable f�r UART- Puffer zur�ck.
			UART_MSG_FLAG=0;														// setze UART_MSG_FLAG zur�ck
		}
	}
}

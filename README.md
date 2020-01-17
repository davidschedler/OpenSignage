# OpenSignage
A plugin based digital signage system for embedded systems (Raspberry Pi 4) created with Qt WebAssembly

Im Rahmen dieser Arbeit wurde ein System entwickelt, das aus drei Anwendungen besteht. Ziel dieses Systems 
ist eine kostengünstige Lösung zur Anzeige von digitalen Inhalten. Im Mittelpunkt steht dabei die Möglichkeit, 
den Inhalt durch eine Vielzahl von Attributen adäquat zu verwalten und zu planen.

Mithilfe der Grafikschnittstelle der Verwaltungsanwendung (Server-Anwendung) ist der Anwender in der Lage, 
einzelne Videos und Bilder unter anderem für bestimmte Wochentage, Tagesabschnitte und Ereignisse in Abspiel-
einheiten komfortabel zu planen. Die Speicherung der gewonnenen Daten erfolgt durch die Datenbankschnittstelle 
der Verwaltungsanwendung. Die Abspielanwendung (Client-Anwendung) übernimmt hierbei sowohl die Auswertung der 
konfigurierten Abspieleinheiten als auch den Download und die Anzeige des gewünschten digitalen Inhalts auf dem Einplatinencomputer „Raspberry Pi 4“.

Umgesetzt wurde das System mit dem plattformübergreifenden Anwendungsframework Qt sowie der neuen Webtechnologie „WebAssembly“. 

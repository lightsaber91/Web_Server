Web Server con adattamento dinamico di contenuti statici.
Questo server è in grado di ridimensionare le immagini dinamicamente a seconda del dispositivo che effettua la richiesta, confrontando lo user agent fornito dal browser con quelli contenuti nel file wurfl.xml
Il web server non necessita dei privileggi di amministratore per funzionare se si seleziona una porta superiore alla 1024.
Per l'installazione del web server sono necessari i privileggi di amministratore, per lo spostamento di alcuni file all'interno di cartelle di sistema come /opt/ o /usr/bin/, solo se verrà eseguito il make install altrimenti non sarà necessario.
Se non sarà eseguito il make install lo script server-ctl potrebbe non funzionare correttamente

Procedura per l'installazione:


1 - Se siete su Debian o derivate eseguite "./configure" nel terminale per installare le dipendenze
    Se non siete su sitemi Debian installate imagemagick libmagickcore-dev libxml2 e procedete allo step 2

2 - Eseguire "make" da terminale per la corretta compilazione

3 - Eseguire "make install" da terminale per il corretto posizionamento del server nelle cartelle di sistema

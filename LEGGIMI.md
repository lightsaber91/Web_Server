
Questo server è in grado di ridimensionare le immagini a seconda del dispositivo che effettua la richiesta, confrontando lo user agent fornito dal browser con quelli contenuti nel file wurfl.
Il web server non necessita dei privileggi di amministratore per funzionare se si seleziona una porta superiore alla 1024.
Per l'installazione saranno necessari i privilegi di amministratore sia per installare le dipendenze necessarie al ridimensionamento, sia per l'esecuzione del comando make install, che può essere non eseguito, ma in tal caso lo script server-ctl potrebbe non funzionare correttamente.

Procedura per l'installazione:


1 - Se siete su Debian o derivate eseguite "./configure" nel terminale per installare le dipendenze
    Se non siete su sitemi Debian installate imagemagick libmagickcore-dev libxml2 e procedete allo step 2

2 - Eseguire "make" da terminale per la corretta compilazione

Opzionale:
3 - Eseguire "make install" da terminale per il corretto posizionamento del server nelle cartelle di sistema e l'installazione dello script server-ctl

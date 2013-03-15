Lo scopo del progetto è quello di realizzare in linguaggio C usando l'API del socket di
Berkeley un Web server concorrente in grado di adattare dinamicamente il contenuto
statico fornito in base alle caratteristiche del dispositivo del client. Il server dovrà
fornire:

  1.le funzionalità di base di un Web server (incluso il logging);

  2.il supporto minimale del protocollo HTTP/1.1 (connessioni persistenti, metodi
    GET e HEAD);
  
  3.l’adattamento dinamico (on-the-fly) di immagini. Tale adattamento dovrà essere
    effettuato almeno per il formato JPEG sulla base del fattore di qualità specificato
    dal client nell’header Accept; ad esempio, nel caso di Accept: image/jpeg;
    q=0.5, text/plain il server fornirà in risposta l’immagine JPEG richiesta con
    fattore di qualità pari a 0.5. L’adattamento potrà anche essere effettuato per
    altri formati di immagini ed applicando operazioni di manipolazione
    dell’immagine (ad es. conversione del formato, resizing, riduzione del numero di
    colori) sulla base di informazioni riguardanti il browser e contenute nell’header
    User-Agent (si consiglia di usare WURFL per ottenere le informazioni sulle
    caratteristiche dei dispositivi (http://wurfl.sourceforge.net/));
  
  4.il caching della versione risultante dall’operazione di adattamento, allo scopo di
    ridurre l’overhead computazionale dell’adattamento. Implementare il caching in
    modo da gestire la presenza di versioni multiple di una stessa risorsa.

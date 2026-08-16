# Correzioni apportate

Per rendere il progetto dell'esonero adatto a un repository GitHub pubblico e compilabile anche con toolchain moderne sono state applicate alcune correzioni conservative.

## Client

- sostituito `gets()` con `fgets()` per evitare overflow del buffer;
- aggiunta validazione di IP e porta;
- aggiunto controllo sicuro degli argomenti da riga di comando;
- aggiunto terminatore NUL ai messaggi ricevuti;
- rimosso `system("pause")`, specifico di Windows.

## Server

- corretto l'array usato per il parsing: nel codice originale aveva spazio per 2 puntatori ma venivano usati 3 token;
- sostituito il parsing tramite `strtok()` con `sscanf()` e validazione del formato;
- aggiunta gestione della divisione per zero;
- sostituito `gcvt()` con `snprintf()`;
- corretto il tipo della lunghezza dell'indirizzo per `accept()` su sistemi POSIX;
- aggiunta gestione più robusta di `recv()`;
- mantenuta la possibilità di servire più client in sequenza.

## Repository

- rimossi file generati da Eclipse (`Debug/`, `.settings/`, binari e object file);
- rimossi backup ZIP annidati;
- aggiunti `README.md`, `Makefile` e `.gitignore`;
- mantenuto il progetto focalizzato sull'esonero TCP.

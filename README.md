# TCP Client-Server in C 🌐

### Esonero universitario - **Reti di Calcolatori**

**Corso di Laurea in Informatica e Tecnologie per la Produzione del Software**  
Università degli Studi di Bari Aldo Moro

**Autrice:** Sonia Auciello

---

## 📌 Descrizione

Progetto client-server sviluppato in **C** utilizzando socket **TCP**.

Il server espone un semplice servizio di calcolo: il client si connette al server, invia un'operazione aritmetica e riceve il risultato.

Formato dei messaggi:

```text
[operatore] [numero] [numero]
```

Esempio:

```text
+ 2 4
```

Risposta:

```text
6
```

Per terminare la connessione:

```text
=
```

---

## ✨ Funzionalità

Il progetto supporta:

- connessione TCP client-server;
- indirizzo predefinito `127.0.0.1`;
- porta predefinita `27015`;
- somma;
- sottrazione;
- moltiplicazione;
- divisione;
- gestione della divisione per zero;
- validazione del formato del messaggio;
- chiusura volontaria della connessione;
- gestione sequenziale di più client;
- compatibilità Windows/POSIX nel codice sorgente.

---

## 🛠️ Tecnologie

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![TCP/IP](https://img.shields.io/badge/TCP%2FIP-Networking-005C84?style=flat)
![Sockets](https://img.shields.io/badge/BSD_Sockets-Client%2FServer-555555?style=flat)
![GCC](https://img.shields.io/badge/GCC-Compiler-A42E2B?style=flat&logo=gnu&logoColor=white)

---

## 🧱 Architettura

```text
+---------+             TCP              +---------+
| Client  |  ------------------------->  | Server  |
|         |  operazione: "+ 2 4"         |         |
|         |  <-------------------------  |         |
|         |  risultato: "6"              |         |
+---------+                              +---------+
```

Il server rimane in ascolto sulla socket TCP.  
Quando un client si connette:

1. il server accetta la connessione;
2. riceve l'operazione;
3. valida i dati;
4. calcola il risultato;
5. invia la risposta al client;
6. continua fino al comando `=`.

---

## 📂 Struttura del repository

```text
TCP-Client-Server-C/
├── client/
│   ├── client_TCP.c
│   └── prot_appl.h
├── server/
│   ├── server_TCP.c
│   └── prot_appl.h
├── Makefile
├── README.md
├── .gitignore
└── FIXES.md
```

---

## 🚀 Compilazione

### Linux / macOS

Dalla cartella principale:

```bash
make
```

In alternativa:

```bash
gcc -std=c11 -Wall -Wextra -pedantic server/server_TCP.c -o server_TCP
gcc -std=c11 -Wall -Wextra -pedantic client/client_TCP.c -o client_TCP
```

### Windows con MinGW

```bash
gcc -std=c11 -Wall -Wextra server/server_TCP.c -o server_TCP.exe -lws2_32
gcc -std=c11 -Wall -Wextra client/client_TCP.c -o client_TCP.exe -lws2_32
```

---

## ▶️ Avvio

Avvia prima il server:

```bash
./server_TCP
```

Poi, da un secondo terminale, avvia il client:

```bash
./client_TCP
```

Su Windows:

```text
server_TCP.exe
client_TCP.exe
```

---

## ⚙️ IP e porta personalizzati

Il server accetta opzionalmente la porta:

```bash
./server_TCP 30000
```

Il client accetta opzionalmente IP e porta:

```bash
./client_TCP 127.0.0.1 30000
```

Se non vengono specificati parametri vengono utilizzati:

```text
IP:    127.0.0.1
Porta: 27015
```

---

## 🧪 Esempio

Client:

```text
Input: * 8 5
Risultato: 40
```

Client:

```text
Input: / 10 4
Risultato: 2.5
```

Client:

```text
Input: / 10 0
Risultato: Errore: divisione per zero
```

---

## 🔧 Revisione per il repository pubblico

Il progetto originale dell'esonero è stato ripulito per la pubblicazione su GitHub.

Sono stati mantenuti lo scopo e il protocollo applicativo originali, correggendo problemi tecnici che potevano causare errori o comportamenti non definiti.

I dettagli sono riportati in [`FIXES.md`](FIXES.md).

---

## 🎓 Contesto accademico

**Attività:** Esonero  
**Insegnamento:** Reti di Calcolatori  
**Linguaggio:** C  
**Argomenti:** socket, TCP, architettura client-server, protocollo applicativo

---

## ℹ️ Note

Il server gestisce i client in modo sequenziale: ogni connessione viene servita prima di accettare e processare la successiva.

Il progetto è pubblicato come parte del portfolio accademico.

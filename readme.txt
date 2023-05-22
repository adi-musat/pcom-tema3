Musat-Burcea Adrian 322CB - PCOM Tema 3

Functionare client:
Se citesc comenzile de la utilizator intr-un loop.
La fiecare comanda, se deschide conexiunea catre server, iar dupa trimiterea
cererii si primirea raspunsului, se inchide.
(Daca nu fac asta, a doua comanda nu se mai trimite la server, lucru care ma 
deruta. Am inteles ca trebuie inchisa si redeschisa conexiunea dupa fiecare
comanda)

Comanda register:
    se citesc usernameul si parola, acestea trebuie sa nu contina spatii 
    pentru a fi valide. Dupa, se creeaza un obiect JSON ce contine 
    username si parola, care este dupa serializat intr-un string pt a fi
    adaugat in cererea HTTP POST. Cererea contine doar aceste date, fara cookie si 
    token JWT. Se trimite cererea la server si se primeste raspunsul acestuia.
    Daca raspunsul este 201 - Created, se afiseaza mesaj cum ca contul a fost
    creat, daca se primeste 400 - Bad Request, se parseaza JSON-ul si se 
    afiseaza mesaul de eroare din acesta.

Comanda login:
    se verifica local daca utilizatorul este deja logat prin verificarea
    prezentei cookieului de sesiune. Daca utilizatorul este deja logat se 
    afiseaza mesajul corespunzator si se trecea la asteptarea urmatoarei
    comenzi. Se citesc usernameul si parola, acestea trebuie sa nu contina
    spatii pentru a fi valide. Dupa, se creeaza un obiect JSON ce contine
    username si parola, care este dupa serializat intr-un string pt a fi
    adaugat in cererea HTTP POST. Cererea contine doar aceste date, fara cookie si 
    token JWT. Se trimite cererea la server si se primeste raspunsul acestuia.
    Daca raspunsul este 200 - OK, se afiseaza mesaj cum ca utilizatorul s-a 
    logat cu succes, daca se primeste 400 - Bad Request, se parseaza JSON-ul
    si se afiseaza mesajul de eraore din acesta. Din raspunsul serverului se
    extrage si cookie-ul de sesiune, care este salvat intr-un string.

Comanda enter_library:
    se creeaza o cerere HTTP GET, la care se adauga cookie-ul de sesiune pentru
    a dovedi ca utilizatorul este logat. Se trimite cerererea la server si se 
    primeste raspunsul acestuia. Daca raspunsul este 200 - OK se afiseaza un 
    mesaj de succes, si se parseaza obiectul JSON din raspuns, care contine 
    tokenul JWT. Acesta este dupa aceea salvat intr-un string. Daca se primeste
    401 - Unauthorized, se parseaza JSON-ul si se afiseaza mesajul de eroare
    din acesta.

Comanda get_books:
    se creeaza o cerere HTTP GET, la care se adauga tokenul JWT in headerul
    Authorization, pentru a demonstra accesul la biblioteca. Se trimite cererea
    la server si se primeste raspunsul acestuia. Daca raspunsul este 200 - OK,
    se parseaza array-ul JSON ce contine cartile din biblioteca si se afiseaza.
    In caz ca nu exista nicio carte in biblioteca, se afiseaza "None!". Daca
    raspunsul este 403 - Forbidden, se parseaza JSON-ul si se afiseaza mesajul
    de eroare din acesta.

Comanda get_book:
    se citeste id-ul cartii, care este valid doar daca este un numar. Se adauga
    apoi id-ul la calea de acces si se creeaza o cerere HTTP GET, la care se 
    adauga tokenul JWT in headerul Authorization, pentru a demonstra accesul
    la biblioteca. Se trimite cererea la server si se primeste raspunsul acestuia.
    Daca raspunsul este 200 - OK, se extrage JSON-ul ce contine detaliile cartii
    si se afiseaza (Aici e o neconcordanta intre exemplul din enunt si ce se
    intampla de fapt. In exemplu apare ca in raspunsul serverului se afla doar
    obiectul JSON ce contine detaliile carti. In realitate serverul trimite
    un array cu un singur element JSON care este cartea). Daca raspunsul este
    403 - Forbidden sau 404 - Not Found, se parseaza JSON-ul si se afiseaza
    mesajul de eroare din acesta.

Comanda add_book:
    se citesc titlul, autorul, genul, editura si nr de pagini, care este valid
    daca este un numar. Se creeaza un obiect JSON ce contine detaliile cartii,
    care este apoi serializat intr-un string. Se creeaza apoi cerere HTTP POST
    la care este adaugat tokenul JWT in headerul Authorization, iar in payload
    este adaugat JSON-ul cu informatiile cartii. Se trimite cererea la server
    si se primeste raspunsul acestuia. Daca raspunsul este 200 - OK, se afiseaza
    un mesaj de succes. Daca raspunsul este 403 - Forbidden, se parseaza JSON-ul
    si se afiseaza mesajul de eroare din acesta.

Comanda delete_book:
    se citeste id-ul cartii, care este valid doar daca este un numar. Se adauga
    apoi id-ul la calea de acces si se creeaza o cerere HTTP DELETE, la care se 
    adauga tokenul JWT in headerul Authorization, pentru a demonstra accesul
    la biblioteca. Se trimite cererea la server si se primeste raspunsul acestuia.
    Daca raspunsul este 200 - OK, se afiseaza un mesaj cum ca a fost stearsa
    cartea. Daca raspunsul este 403 - Forbidden sau 404 - Not Found, se parseaza
    JSON-ul si se afiseaza mesajul de eroare din acesta.

Comanda logout:
    se creeaza o cerere HTTP GET, in care se adauga cookie-ul de sesiune pentru
    a demonstra ca utilizatorul este logat. Se trimite cererea la server si se
    primeste raspunsul acestuia. Daca raspunsul este 200 - OK, se afiseaza un
    mesaj de succes si se sterg cookie-ul si tokenul JWT. Daca raspunsul este
    400 - Bad Request, se parseaza JSON-ul si se afiseaza mesajul de eroare din
    acesta.

Comanda exit:
    se inchide clientul
Daca este introdusa o comanda inexistenta, se afiseaza o lista cu comenzile 
valide.

Pentru lucrul cu JSON am ales biblioteca Nlohmann deoarece era cea recomandata
pentru C++ si datorita usurintei de utilizare si de adaugare in aplicatie.

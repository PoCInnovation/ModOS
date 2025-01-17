section .text
global _start         ; L'entrée du noyau, appelée par le chargeur de démarrage (GRUB)
extern kernel_main    ; Déclare la fonction kernel_main en C

_start:
    ; Vérifie le magic number (Multiboot v2 = 0x36D76289)
    mov eax, [esp]        ; Magic number passé par GRUB
    cmp eax, 0x36D76289   ; Comparer avec le magic number attendu
    jne halt              ; Si incorrect, arrête immédiatement le CPU

    ; Passe les informations à kernel_main (magic number et adresse)
    mov edi, [esp + 4]    ; L'adresse de la structure d'information de Multiboot
    call kernel_main      ; Appelle la fonction kernel_main avec magic et info

halt:
    hlt                   ; Arrête le CPU si on arrive ici (problème d'exécution)
    jmp halt              ; Boucle infinie si hlt échoue


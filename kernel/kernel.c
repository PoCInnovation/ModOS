void kernel_main(unsigned long magic, unsigned long addr) {
    // Just to test if we reach this point
    const char *message = "Hello from kernel_main!";
    unsigned char *video_memory = (unsigned char*) 0xb8000;  // Adresse vidéo en mode texte
    int i = 0;

    // Afficher le message à l'écran
    while (message[i] != '\0') {
        video_memory[i * 2] = message[i];  // Caractère
        video_memory[i * 2 + 1] = 0x07;   // Couleur du texte
        i++;
    }

    while (1) {
        // Boucle infinie pour éviter que le noyau se termine
    }
}


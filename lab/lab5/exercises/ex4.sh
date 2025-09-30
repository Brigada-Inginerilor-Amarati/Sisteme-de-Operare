# extrage toate mesajele care incep cu Salut si se termina cu Ana si salveaza-le intr-un fisier nou numit selectate.txt

grep "^Salut.*Ana!$" mesaje.txt > selectate.txt

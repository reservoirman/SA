awk '{if (system("id -u "$1)==1) system("useradd " $1); for(i=2;i<=NF;i++){system("groupadd -f "$i); system("usermod -aG "$i" "$1)}}' userfile.txt
sleep 1
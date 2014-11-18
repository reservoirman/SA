echo "Creating users and groups.  If already created, that's fine!"
awk '{if (system("id -u "$1)==1) system("useradd " $1); for(i=2;i<=NF;i++){system("groupadd -f "$i); system("usermod -aG "$i" "$1)}}' enclave/userfile.txt
sleep 1

pkill angel.out

sleep 1
echo "Running the angel (daemon) as the keymaker.  Keymaker has exclusive access to the enclave and keySafe."
su keymaker -c "./angel.out"
sleep 1
echo "Creating file dog using dog.txt.  The ciphertext is dumped:"
./objput dog -k "welcome" < dog.txt
echo -e "\n\n"
sleep 2

echo  "Call objget on dog.  We will see the decrypted contents:"
sleep 2
./objget dog -k "welcome"
sleep 3
echo "Now, make a bunch of calls to objput dog to overwrite dog with new ciphertext, based on the urandom key:"
./objput dog -k "welcome" < dog.txt
sleep 2
./objput dog -k "welcome" < dog.txt
sleep 2
./objput dog -k "welcome" < dog.txt
sleep 2
./objput dog -k "welcome" < dog.txt

sleep 2
echo -e "\n"
echo "This is no problem for objget, who can still decrypt dog: "
sleep 3
./objget dog -k "welcome"
sleep 1


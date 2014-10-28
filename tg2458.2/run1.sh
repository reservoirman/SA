echo "Creating users and groups.  If already created, that's fine!"
awk '{if (system("id -u "$1)==1) system("useradd " $1); for(i=2;i<=NF;i++){system("groupadd -f "$i); system("usermod -aG "$i" "$1)}}' enclave/userfile.txt
sleep 1

pkill angel.out

sleep 1
echo "Building all of the programs"
make
sleep 1
./angel.out

echo "root creating object 'jjj'"
./objput jjj < JJJ
sleep 1

echo "root creating object 'mmm'"
./objput mmm < MMM
sleep 1

echo "root creating object 'ppp'"
./objput ppp < PPP
sleep 1

echo "root reading object 'jjj'"
./objget jjj
sleep 1

echo "root reading object 'mmm'"
./objget mmm
sleep 1

echo "root reading object 'ppp'"
./objget ppp
sleep 1

echo "list all of root's object"
./objlist
sleep 1

echo "get the ACL for jjj"
./objgetacl jjj
sleep 1

echo "get the ACL for mmm"
./objgetacl mmm
sleep 1

echo "get the ACL for ppp"
./objgetacl ppp
sleep 1

echo "test the ACL for jjj for r"
./objtestacl jjj -a r
sleep 1

echo "set the ACL for jjj for w only"
./objsetacl jjj < WONLY
sleep 1

echo "test the ACL for jjj for w.  It should fail"
./objtestacl jjj -a w
sleep 1

echo "test the ACL for mmm for w"
./objtestacl mmm -a w
sleep 1

echo "set the ACL for mmm for v only"
./objsetacl mmm < VONLY
sleep 1

echo "test the ACL for mmm for x.  It should fail"
./objtestacl mmm -a x
sleep 1

echo "test the ACL for ppp for p"
./objtestacl ppp -a p
sleep 1

echo "set the ACL for ppp for w only"
./objsetacl ppp < PONLY
sleep 1

echo "test the ACL for ppp for w.  It should fail"
./objtestacl ppp -a w
sleep 1

echo "create an object ttt as user ts"
su ts -c "./objput ttt < TSREADME"

echo "ts reading object 'ttt'"
su ts -c "./objget ttt"
sleep 1

echo "list all of ts's object"
su ts -c "./objlist"
sleep 1

echo "get the ACL for ts's ttt"
su ts -c "./objgetacl ttt"
sleep 1
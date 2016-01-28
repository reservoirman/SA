# Security Architecture Project: Secure Object Store

Ten-Seng Guh
tg2458


This is homework assignment 3.  It is an object store providing access control lists for its objects which now enforces encryption on its objects.

One big improvement from the previous homework assignment:
- the angel (daemon) message handler is no longer running as root.  As was stated in class, there is no need for the object store to run as root; all that is necessary is a user who has exclusive rights to the object store (folder called "enclave").  Now, this user also has exclusive access to the urandom keys for each object (folder called "keySafe").  This exclusive user's name is "keymaker".  You will see in the test script run1.sh that the angel is run as keymaker.  The key storing functions will fail if the angel realizes it's being run by a different user (even root!).  So this is a significant security improvement.

Another thing worth pointing out is the amazing design of the angel code.  Code to call the new encrypt and decrypt routines amounted to only TWO
additional lines of code!  This can be seen in objectprograms.c.  It pays to be modular.  All of the decryption code is contained in three other modules that the angel doesn't care about.


Before (HW2 version):
		if (objects_createObject(u2, o, c, DATA) == -1)
		{				
			printf("Failed to create object %s.  Please try again.\n", o);
		}
		else
		{
			success = 0;
			printf("OBJPUT: object written!\n");
		}


After (HW3 version):
		const unsigned char *ec = objectcrypto_encrypt(u2, o, c, p);
		if (objects_createObject(u2, o, (char *)ec, DATA) == -1)
		{				
			printf("Failed to create object %s.  Please try again.\n", o);
		}
		else
		{
			success = 0;
			printf("OBJPUT: object written! Encrypted as %s:\n", ec);
		}


Before (HW2 version):
			char *contents = objects_readObject(u2, o, DATA);
			if (contents != NULL)
			printf("%s\n", contents);
			success = 0;


After (HW3 version):
			unsigned char *ec = objects_readObject(u2, o, DATA);
			//decrypt the contents now
			const unsigned char *dc = objectcrypto_decrypt(u2, o, ec, p);
			if (dc != NULL)
				printf("%s\n", dc);
			success = 0;


The password for root and tinyvm is Gangstas2013.
NOTE: the paths will only work on the VM!  No guarantees on clic machines.
Running 'make test' will execute the script to create users and groups.
The script can also be run stand-alone; it's name is 'run1.sh'.

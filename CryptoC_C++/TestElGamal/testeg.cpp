/***********************************************************************
 *                                                                     *
 *             Programme de test pour les fonctions ElGamal            *
 *                                                                     *
 *                                                                     *
 *                           Pierre Paquay                             *
 *                                                                     *
 ***********************************************************************/

#include "libeg++.hpp"

#define KEYLENGTH0 512
#define BLOCKLENGTH0 (KEYLENGTH0 >> 3) - 11
#define KEYLENGTH1 1024
#define BLOCKLENGTH1 (KEYLENGTH1 >> 3) - 11
#define KEYLENGTH2 2048
#define BLOCKLENGTH2 (KEYLENGTH2 >> 3) - 11
#define MESSLENGTH 2000
#ifdef ARCH64
#define NBRETESTS 60
#else
#define NBRETESTS 15
#endif

static UCHAR* genmess(const int);
static void cout_mess(const char*, const int, const char* const);

static UCHAR* genmess(const int len)
{
  UCHAR* mess = new UCHAR[len + 1];
  UCHAR* temp = mess;
  int i;

  if (mess == NULL)
    return NULL;
  for (i = 0; i < len; i++)
    {
      *temp = ucrand64_l();
      temp = temp + 1;
    }
  *temp = '\0';

  return mess;
}

static void cout_mess (const char* mess, const int len, const char* const title)
{
  cout << title << " = 0x";

  for  (int i = 0; i < len; i++)
    {
      cout << hex << (((unsigned)*mess++) & 0xff);
    }

  cout << endl;
}

int main()
{
  int error = 0;
  int i;
  int KEYLENGTH = 0, BLOCKLENGTH = 0;

  seed64(time(NULL));

  cout << "PROGRAMME DE TEST DES FONCTIONS ELGAMAL..." << endl;
  cout << "------------------------------------------" << endl << endl;;

  for (i = 0; i < NBRETESTS; i++)
    {
     if (i < NBRETESTS / 3)
	{
	  KEYLENGTH = KEYLENGTH0;
	  BLOCKLENGTH = BLOCKLENGTH0;
	  cout << "TESTS POUR UNE CLE DE " << KEYLENGTH << " bits..." << endl;
	}
      else if ((i >= NBRETESTS / 3) && (i < 2 * NBRETESTS / 3))
	{
	  KEYLENGTH = KEYLENGTH1;
	  BLOCKLENGTH = BLOCKLENGTH1;
	  cout << "TESTS POUR UNE CLE DE " << KEYLENGTH << " bits..." << endl;
	}
      else if ((i >= 2 * NBRETESTS / 3) && (i < NBRETESTS))
	{
	  KEYLENGTH = KEYLENGTH2;
	  BLOCKLENGTH = BLOCKLENGTH2;
	  cout << "TESTS POUR UNE CLE DE " << KEYLENGTH << " bits..." << endl;
	}
      cout << endl;

      // Test de cryptage et de signature...
      
      cout << "G�n�ration d'une cl� ElGamal priv�e de " << KEYLENGTH << " bits..." << endl;
      EGPriv ma_cle(KEYLENGTH);
      cout << "Cl� priv�e g�n�r�e." << endl;

      cout << "Extraction de la cl� publique de cette cl� priv�e..." << endl;
      EGPub ma_cle_pub(ma_cle);
      cout << "Cl� publique extraite." << endl;

      cout << "G�n�ration d'un message al�atoire de " << BLOCKLENGTH << " bytes..." << endl;
      UCHAR* mess1 = genmess(BLOCKLENGTH);
      cout << "Message al�atoire g�n�r�." << endl;
      cout_mess((const char*)mess1, BLOCKLENGTH, "Message_1");
      cout << dec << endl;

      EGMess C = ma_cle_pub.crypt(mess1, BLOCKLENGTH);
      cout << "Message crypt�." << endl;
      cout << "Texte crypt� : " << endl;
      cout << "alpha = " << C.B0 << endl;
      cout << "beta = " << C.c << endl;

      cout << "G�n�ration d'un message al�atoire de " << MESSLENGTH << " bytes..." << endl;
      UCHAR* mess2 = genmess(MESSLENGTH);
      cout << "Message al�atoire g�n�r�." << endl;
      cout_mess((const char*)mess2, MESSLENGTH, "Message_2");
      cout << dec << endl;
      
      EGSign S = ma_cle.sign(mess2, MESSLENGTH);
      cout << "Message Hash� et sign�." << endl;
      cout_mess((const char*)ripemd160((UCHAR*)mess2, MESSLENGTH), 20, "Hash_value");
      cout << dec << endl;
      cout << "r = " << S.r << endl;
      cout << "s = " << S.s << endl;

      int len;
      UCHAR* m = ma_cle.decrypt(C, &len);
      if ((len == BLOCKLENGTH) && (!memcmp(m, mess1, BLOCKLENGTH)))
	cout << "Op�ration de cryptage/d�cryptage r�ussie ! ----> OK" << endl;
      else
	{
	  cout << "Erreur dans l'op�ration de cryptage/d�cryptage !!!" << endl;
	  error = 1;
	}
      if (ma_cle_pub.authen(mess2, S, MESSLENGTH) == 1)
	cout << "Op�ration de signature/identification r�ussie ! ----> OK" << endl;
      else
	{
	  cout << "Erreur dans l'op�ration de signature/identification !!!" << endl;
	  error = 1;
	}
      cout << endl;

      // Test de lecture et d'�criture dans un fichier...
      
      EGPriv cle_test1;
      EGPub cle_test2;
      
      cout << "Ecriture des cl�s dans les fichiers eg.prv et eg.pub..." << endl;
      ofstream fout1("eg.prv", ios::out | ios::binary);
      fout1 << ma_cle;
      fout1.close();
      ofstream fout2("eg.pub", ios::out | ios::binary);
      fout2 << ma_cle_pub;
      fout2.close();
  
      cout << "Lecture des cl�s dans les fichiers eg.prv et eg.pub..." << endl;
      ifstream fin1("eg.prv", ios::in | ios::binary);
      fin1 >> cle_test1;
      fin1.close();
      ifstream fin2("eg.pub", ios::in | ios::binary);
      fin2 >> cle_test2;
      fin2.close();

      if (cle_test1 == ma_cle)
	cout << "Op�ration de lecture/�criture de la cl� priv�e r�ussie ! ----> OK" << endl;
      else
	{
	  cout << "Erreur de lecture/�criture de la cl� priv�e !!!" << endl;
	  error = 1;
	}
      cout << endl;
      if (cle_test2 == ma_cle_pub)
	cout << "Op�ration de lecture/�criture de la cl� publique r�ussie ! ----> OK" << endl;
      else
	{
	  cout << "Erreur de lecture/�criture de la cl� publique !!!" << endl;
	  error = 1;
	}
      cout << endl;
    }
  cout << endl;

  if (error)
    cout << "Erreur dans une des op�rations du test !!!!!!!!!!!!" << endl;
  else
    cout << "Tous les tests se sont d�roul�s sans erreur." << endl;

  return 0;
}

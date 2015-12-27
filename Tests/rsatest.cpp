/***********************************************************************
 *                                                                     *
 *                Programme de test pour les fonctions RSA             *
 *                                                                     *
 *                                                                     *
 *                            Pierre Paquay                            *
 *                                                                     *
 ***********************************************************************/

#include "librsa.hpp"


int main()
{
  cout << "TEST D'ENCRYPTION ET DE DECRYPTION" << endl;
  cout << endl;

  UCHAR* mess = (UCHAR*)"Oui, non, ceci, cela.";
  cout << "Le message original est: " << mess << endl;
  cout << endl;

  RSAKey cle(512);
  RSAPKey clepub(cle);

  BINT chiff = clepub.crypt(mess, strlen((char*)mess) + 1);
  int l;
  cout << "Le message chiffre est: " << bint2byte(chiff, &l) << endl;
  UCHAR* dechiff = cle.decrypt(chiff, &l);
  cout << "Le message dechiffre est: " << dechiff << "@@" <<endl;
  if (strcmp((char*)mess, (char*)dechiff) == 0)
    cout << "dechiff = original --> OK !" << endl;
  else
    cout << "dechiff != original --> MERDE !" << endl;
  cout << endl;

  cout << "TEST DE LECTURE ET D'ECRITURE DANS UN FICHIER" << endl;
  cout << endl;

  RSAKey Fcle;
  RSAPKey Fclepub;

  ofstream fout1;
  fout1.open("RSAPriv", ios::binary);
  fout1 << cle;
  fout1.close();

  ofstream fout2;
  fout2.open("RSAPub", ios::binary);
  fout2 << clepub;
  fout2.close();

  ifstream fin1;
  fin1.open("RSAPriv", ios::binary);
  fin1 >> Fcle;
  fin1.close();

  ifstream fin2;
  fin2.open("RSAPub", ios::binary);
  fin2 >> Fclepub;
  fin2.close();

  chiff = Fclepub.crypt(mess, strlen((char*)mess) + 1);
  int l1;
  cout << "Le message chiffre est: " << bint2byte(chiff, &l1) << endl;
  dechiff = Fcle.decrypt(chiff, &l1);
  cout << "Le message dechiffre est: " << dechiff << "@@" << endl;
  if (strcmp((char*)mess, (char*)dechiff) == 0)
    cout << "dechiff = original --> OK !" << endl;
  else
    cout << "dechiff != original --> MERDE !" << endl;
  cout << endl;

  cout << "TEST DE VERIFICATION DE SIGNATURE" << endl;
  cout << endl;

  RSAKey clefact(512);

  BINT sig = cle.sign(mess, strlen((char*)mess) + 1);
  cout << "La signature est: " << bint2byte(sig, &l) << endl;
  if (clepub.authen(mess, strlen((char*)mess) + 1, sig) == 1)
    cout << "L'expediteur est authentifie !" << endl;
  else
    cout << "L'expediteur n'est pas authentifie !" << endl;
  cout << endl;

  sig = clefact.sign(mess, strlen((char*)mess) + 1);
  cout << "La signature est: " << bint2byte(sig, &l) << endl;
  if (clepub.authen(mess, strlen((char*)mess) + 1, sig) == 1)
    cout << "L'expediteur est authentifie !" << endl;
  else
    cout << "L'expediteur n'est pas authentifie !" << endl;
  cout << endl;

  return 0;
}

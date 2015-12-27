/***********************************************************************
 *                                                                     *
 *                Programme de démo pour les fonctions RSA             *
 *                                                                     *
 *                                                                     *
 *                            Pierre Paquay                            *
 *                                                                     *
 ***********************************************************************/

#include "librsa++.hpp"


int main()
{
  cout << "TEST D'ENCRYPTION ET DE DECRYPTION" << endl;
  cout << endl;

  UCHAR* mess = (UCHAR*)"Cécile est trop merveilleuse et trop adorable...";
  cout << "Le message original est: " << mess << endl;
  cout << endl;

  /*UCHAR* messb64 = base64_encode(mess, strlen((char*)mess) + 1, 0);
  int len;
  int error;
  cout << "Le message encodé en base64 est : " << messb64 << endl;
  UCHAR* decob64 = base64_decode(messb64, &len, 1, &error);
  cout << "Le message décodé en ASCII est  : " << decob64 << endl;
  if (strcmp((char*)mess, (char*)decob64) == 0)
    cout << "deco = original --> OK !" << endl;
  else
    cout << "deco != original --> MERDE !" << endl;
  cout << endl;*/

  RSAPriv cle(2048);
  RSAPub clepub(cle);
  
  largeint chiff = clepub.crypt(mess, strlen((char*)mess) + 1);
  int l;
  cout << "Le message chiffre est: " << base64_encode(largeint2byte(chiff, &l), l, 0) << endl;
  UCHAR* dechiff = cle.decrypt(chiff, &l);
  cout << "Le message dechiffre est: " << dechiff << endl;
 if (strcmp((char*)mess, (char*)dechiff) == 0)
   cout << "dechiff = original --> OK !" << endl;
 else
   cout << "dechiff != original --> MERDE !" << endl;
 cout << endl; 

  cout << "TEST DE VERIFICATION DE SIGNATURE" << endl;
  cout << endl;

  largeint sig = cle.sign(mess, strlen((char*)mess) + 1);
  //cout << "La signature est: " << sig << endl;
  if (clepub.authen(mess, sig, strlen((char*)mess) + 1) == 1)
    cout << "L'expediteur est authentifie ! (OK)" << endl;
  else
    cout << "L'expediteur n'est pas authentifie ! (PAS OK!)" << endl;
  cout << endl;

  RSAPriv clefact(2048);

  largeint sigfact = clefact.sign(mess, strlen((char*)mess) + 1);
  //cout << "La signature est: " << sigfact << endl;
  if (clepub.authen(mess, sigfact, strlen((char*)mess) + 1) == 1)
    cout << "L'expediteur est authentifie ! (PAS OK)" << endl;
  else
    cout << "L'expediteur n'est pas authentifie ! (OK)" << endl;
  cout << endl;

  UCHAR* messfact = (UCHAR*)"Dingue ça !";
  //cout << "La signature est: " << largeint2byte(sig, &l) << endl;
  if (clepub.authen(messfact, sig, strlen((char*)messfact) + 1) == 1)
    cout << "L'expediteur est authentifie ! (PAS OK)" << endl;
  else
    cout << "L'expediteur n'est pas authentifie ! (OK)" << endl;
  cout << endl;

  cout << "TEST DE LECTURE ET D'ECRITURE DANS UN FICHIER" << endl;
  cout << endl;

  RSAPriv Fcle;
  RSAPub Fclepub;

  ofstream fout1;
  fout1.open("RSAPriv", ios::binary);
  fout1 << cle;
  fout1.close();

  ifstream fin1;
  fin1.open("RSAPriv", ios::binary);
  fin1 >> Fcle;
  fin1.close();

  ofstream fout2;
  fout2.open("RSAPub", ios::binary);
  fout2 << clepub;
  fout2.close();

  ifstream fin2;
  fin2.open("RSAPub", ios::binary);
  fin2 >> Fclepub;
  fin2.close();

  //cout << "Le message chiffre est: " << largeint2byte(chiff, &l) << endl;
  dechiff = Fcle.decrypt(chiff, &l);
  cout << "Le message dechiffre est: " << dechiff << endl;
  if (strcmp((char*)mess, (char*)dechiff) == 0)
    cout << "dechiff = original --> OK !" << endl;
  else
    cout << "dechiff != original --> MERDE !" << endl;
  cout << endl;

  chiff = Fclepub.crypt(mess, strlen((char*)mess) + 1);
  //cout << "Le message chiffre est: " << largeint2byte(chiff, &l) << endl;
  dechiff = cle.decrypt(chiff, &l);
  cout << "Le message dechiffre est: " << dechiff << endl;
  if (strcmp((char*)mess, (char*)dechiff) == 0)
    cout << "dechiff = original --> OK !" << endl;
  else
    cout << "dechiff != original --> MERDE !" << endl;
  cout << endl; 

  sig = Fcle.sign(mess, strlen((char*)mess) + 1);
  //cout << "La signature est: " << largeint2byte(sig, &l) << endl;
  if (clepub.authen(mess, sig, strlen((char*)mess) + 1) == 1)
    cout << "L'expediteur est authentifie ! (OK)" << endl;
  else
    cout << "L'expediteur n'est pas authentifie ! (PAS OK)" << endl;
  cout << endl;

  sig = cle.sign(mess, strlen((char*)mess) + 1);
  //cout << "La signature est: " << largeint2byte(sig, &l) << endl;
  if (Fclepub.authen(mess, sig, strlen((char*)mess) + 1) == 1)
    cout << "L'expediteur est authentifie ! (OK)" << endl;
  else
    cout << "L'expediteur n'est pas authentifie ! (PAS OK)" << endl;
  cout << endl;
 
 cout << endl;
    
 return 0;
}

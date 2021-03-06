#include "crypto-api.hpp"

int main()
{
  KeyPriv clepriv("Pierre_Paquay", "p.paquay@linuxaddict.be", 512);
  cout << "Cl� priv�e g�n�r�e" << endl;

  KeyPub clepub(clepriv);
  cout << "Cl� publique g�n�r�e" << endl;

  /*ofstream foutpub("Essai.pub", ios::out | ios::binary);
  foutpub << clepub;
  foutpub.close();*/
  writepubkey("Essai.pub", clepub);
  cout << "Ecriture de la cl� publique dans Essai.pub" << endl;

  /*ofstream foutpriv("Essai.priv", ios::out | ios::binary);
  foutpriv << clepriv;
  foutpriv.close();*/
  writeprivkey("Essai.priv", clepriv);
  cout << "Ecriture de la cl� priv�e dans Essai.priv" << endl;

  KeyPub cletest_pub;
  /*ifstream finpub("Essai.pub", ios::in | ios::binary);
  finpub >> cletest_pub;
  finpub.close();*/
  readpubkey("Essai.pub", cletest_pub);
  cout << "Lecture de la cl� publique dans Essai.pub" << endl;

  KeyPriv cletest_priv;
  /*ifstream finpriv("Essai.priv", ios::in | ios::binary);
  finpriv >> cletest_priv;
  finpriv.close();*/
  readprivkey("Essai.priv", cletest_priv);
  cout << "Lecture de la cl� priv�e dans Essai.priv" << endl;

  if (clepub == cletest_pub)
    cout << "Lecture/ecriture cl� publique --> OK" << endl;
  else
    cout << "MERDE!!!!!!!!" << endl;

  if (clepriv == cletest_priv)
    cout << "Lecture/ecriture cl� priv�e --> OK" << endl;
  else
    cout << "MERDE!!!!!!!!" << endl;

  /*seed64(time(NULL));

  for (i = 0; i < 8; i++)
    do
      salted[i] = ucrand64_l();
    while (salted[i] == 0);
  cout << "Key = " << base64_encode(string2key((UCHAR*)pass, strlen((char*)pass), 32, salted), 32, 0) << endl;
  printf("Salt = ");
  for (i = 0; i < 8; i++)
    printf("%x ", salted[i]);*/


  return 0;
}

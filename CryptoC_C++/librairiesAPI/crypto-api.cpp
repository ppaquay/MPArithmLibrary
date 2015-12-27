/***********************************************************************
 *                                                                     *
 *              Code source pour les fonctions de l'API en C           *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#include "crypto-api.hpp"

// Fonction pour la formation des paquets

/*UCHAR* packet_header(UCHAR, unsigned int);
unsigned int packet_header_inv(UCHAR*, UCHAR*);*/

// Classe KeyPriv

KeyPriv::KeyPriv(const string name, const string email, const int len)
{
  makekeystruct(name, email, len);
  RSAPriv ktemp(len);
  KeyPrivRSA = ktemp;
}

KeyPriv::KeyPriv(const string name, const string email, const int len, const largeint& seed)
{
  makekeystruct(name, email, len);
  RSAPriv ktemp(len, seed);
  KeyPrivRSA = ktemp;
}

KeyPriv::~KeyPriv()
{
  //memset(PrivKeyStruct.UserName, 0, strlen((char*)PrivKeyStruct.UserName) + 1);
  //memset(PrivKeyStruct.UserEMail, 0, strlen((char*)PrivKeyStruct.UserEMail) + 1);
  //PrivKeyStruct.Len = 0;
}

KeyStruct KeyPriv::Export_KeyStruct(void)
{
  KeyStruct keystructtemp;
  //int namelen, emaillen;

  //namelen = (PrivKeyStruct.UserName).length + 1;
  //string keystructtemp.UserName;
  keystructtemp.UserName = PrivKeyStruct.UserName;
  keystructtemp.UserEMail = PrivKeyStruct.UserEMail;
  keystructtemp.Len = PrivKeyStruct.Len;
  
  return keystructtemp;
}

RSAPub KeyPriv::Export_RSAPub(void)
{
  RSAPub pubkeytemp(KeyPrivRSA);

  return pubkeytemp;
}

int operator==(const KeyPriv& key1, const KeyPriv& key2)
{
  if (&key1 == &key2)
    return 1;
  if ((key1.PrivKeyStruct.UserName == key2.PrivKeyStruct.UserName)
      && (key1.PrivKeyStruct.UserEMail == key2.PrivKeyStruct.UserEMail)
      && (key1.PrivKeyStruct.Len == key2.PrivKeyStruct.Len)
      && (key1.KeyPrivRSA == key2.KeyPrivRSA))
    return 1;
  else
    return 0;
}

void writeprivkey(const char* filename, const KeyPriv& wkey)
{
  ofstream foutpriv(filename, ios::out | ios::binary);
  foutpriv << wkey;
  foutpriv.close();
}

void readprivkey(const char* filename, KeyPriv& rkey)
{
  ifstream finpriv(filename, ios::in | ios::binary);
  finpriv >> rkey;
  finpriv.close();
}  

void KeyPriv::makekeystruct(const string name, const string email, const int len)
{
  PrivKeyStruct.UserName = name;
  PrivKeyStruct.UserEMail = email;
  PrivKeyStruct.Len = len;
}

ofstream& operator<<(ofstream& s, const KeyPriv& wkey)
{
  s << "----BEGIN_PRIVATE_KEY----" << '\n';
  s << wkey.PrivKeyStruct.UserName << '\n';
  s << wkey.PrivKeyStruct.UserEMail << '\n';
  s.operator<<(wkey.PrivKeyStruct.Len);
  s << wkey.KeyPrivRSA;
  s << "----END_PRIVATE_KEY----" << '\n';

  return s;
}

ifstream& operator>>(ifstream& s, KeyPriv& rkey)
{
  string header;

  s >> header;
  if (header != "----BEGIN_PRIVATE_KEY----")
    printf("MERDE\n");
  s >> rkey.PrivKeyStruct.UserName;
  s >> rkey.PrivKeyStruct.UserEMail;
  s >> rkey.PrivKeyStruct.Len;
  s >> rkey.KeyPrivRSA;
  s >> header;
  if (header != "----END_PRIVATE_KEY----")
    printf("MERDE\n");

  return s;
}

// Classe KeyPub

KeyPub::KeyPub(const KeyPriv& keypriv)
{
  PubKeyStruct = ((KeyPriv)keypriv).Export_KeyStruct();
  KeyPubRSA = ((KeyPriv)keypriv).Export_RSAPub();
}

KeyPub::~KeyPub()
{
  //memset(PubKeyStruct.UserName, 0, strlen((char*)PubKeyStruct.UserName) + 1);
  //memset(PubKeyStruct.UserEMail, 0, strlen((char*)PubKeyStruct.UserEMail) + 1);
  //PubKeyStruct.Len = 0;
}

int operator==(const KeyPub& key1, const KeyPub& key2)
{
  if (&key1 == &key2)
    return 1;
  if ((key1.PubKeyStruct.UserName == key2.PubKeyStruct.UserName)
      && (key1.PubKeyStruct.UserEMail == key2.PubKeyStruct.UserEMail)
      && (key1.PubKeyStruct.Len == key2.PubKeyStruct.Len)
      && (key1.KeyPubRSA == key2.KeyPubRSA))
    return 1;
  else
    return 0;
}

void writepubkey(const char* filename, const KeyPub& wkey)
{
  ofstream foutpub(filename, ios::out | ios::binary);
  foutpub << wkey;
  foutpub.close();
}

void readpubkey(const char* filename, KeyPub& rkey)
{
  ifstream finpub(filename, ios::in | ios::binary);
  finpub >> rkey;
  finpub.close();
}

ofstream& operator<<(ofstream& s, const KeyPub& wkey)
{
  s << "----BEGIN_PUBLIC_KEY----" << '\n';
  s << wkey.PubKeyStruct.UserName << '\n';
  s << wkey.PubKeyStruct.UserEMail << '\n';
  s.operator<<(wkey.PubKeyStruct.Len);
  s << wkey.KeyPubRSA;
  s << "----END_PUBLIC_KEY----" << '\n';

  return s;
}

ifstream& operator>>(ifstream& s, KeyPub& rkey)
{
  string header;

  s >> header;
  if (header != "----BEGIN_PUBLIC_KEY----")
    printf("MERDE\n");
  s >> rkey.PubKeyStruct.UserName;
  s >> rkey.PubKeyStruct.UserEMail;
  s >> rkey.PubKeyStruct.Len;
  s >> rkey.KeyPubRSA;
  s >> header;
  if (header != "----END_PUBLIC_KEY----")
    printf("MERDE\n");

  return s;
}

// Fonction pour la génération de la clé à partir d'un mot de passe

UCHAR* string2key(const UCHAR* passphrase, const ULONG lenpass, const int nkeybytes, const UCHAR* salt)
{
  UCHAR* hash = new UCHAR[lenpass + 8 + 1 + 1];
  UCHAR* result = new UCHAR[nkeybytes];
  UCHAR* temp = new UCHAR[20];
  int i;

  if ((nkeybytes != 16) && (nkeybytes != 24) && (nkeybytes != 32))
    return NULL;
  memcpy(hash, salt, 8);
  memcpy(hash + 8, passphrase, lenpass + 1);
  memset(hash + 8 + lenpass + 1, 0, 1);
  temp = ripemd160(hash, lenpass + 8 + 1);
  for (i = 0; i < HASHCOUNT; i++)
    temp = ripemd160(temp, 20);
  if (nkeybytes == 16)
    {
      memcpy(result, temp + (20 - nkeybytes), nkeybytes);

      return result;
    }
  else
    {
      memcpy(result, temp, 20);
      hash[0] = 0x00;
      memcpy(hash + 1, salt, 8);
      memcpy(hash + 9, passphrase, lenpass + 1);
      temp = ripemd160(hash, lenpass + 9 + 1);
      for (i = 0; i < HASHCOUNT; i++)
	temp = ripemd160(temp, 20);
      memcpy(result + 20, temp + (40 - nkeybytes), nkeybytes - 20);
    }

  return result;
}

// Fonction pour la génération de la clé des session

UCHAR* sessionkey(const int AlgoPriv)
{
  int keylen;
  int i;
  
  switch (AlgoPriv)
    {
    case AES128:
      keylen = 16;
      break;
    case AES192:
      keylen = 24;
      break;
    case AES256:
      keylen = 32;
      break;
    default:
      return NULL;
    }
  seedBBS(time(NULL));
  UCHAR* temp = new UCHAR[keylen];
  for (i = 0; i < keylen; i++)
    {
      do
	temp[i] = ucrandBBS_l();
      while (temp[i] == 0);
    }

  return temp;
}

// Fonctions pour l'utilisation de l'ASCII Armor

static char b64table[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
			     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X','Y' ,'Z', 
			     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
			     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
			     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

unsigned char* base64_encode(unsigned char* input, int len, int wrap)
{
  unsigned char *output, *p;
  int i = 0, mod = len % 3, toalloc;

  toalloc = (len / 3) * 4 + (3 - mod) % 3 + 1;
  if (wrap)
    {
      toalloc = toalloc + len / 57;
      if (len % 67)
	toalloc = toalloc + 1;
    }
  p = output = (unsigned char*)malloc(((len / 3) + (mod ? 1 : 0)) * 4 + 1);
  if (!p)
    
    return 0;
  while (i < len - mod)
    {
      *p = b64table[input[i] >> 2];
      p = p + 1;
      i = i + 1;
      *p = b64table[((input[i - 1] << 4) | (input[i] >> 4)) & 0x3f];
      p = p + 1;
      *p = b64table[((input[i] << 2) | (input[i + 1] >> 6)) & 0x3f];
      p = p + 1;
      *p = b64table[input[i + 1] & 0x3f];
      p = p + 1;
      i = i + 2;
      if (wrap && !(i % 57))
	{
	  *p = '\n';
	  p = p + 1;
	}
    }
  if (!mod)
    {
      if (wrap && i % 57)
	{
	  *p = '\n';
	  p = p + 1;
	}
      *p = 0;

      return output;
    }
  else
    {
      *p = b64table[input[i] >> 2];
      p = p + 1;
      i = i + 1;
      *p = b64table[((input[i - 1] << 4) | (input[i] >> 4)) & 0x3f];
      p = p + 1;
      if (mod == 1)
	{
	  *p = '=';
	  p = p + 1;
	  *p = '=';
	  p = p + 1;
	  if (wrap)
	    {
	      *p = '\n';
	      p = p + 1;
	    }
	  *p = 0;

	  return output;
	}
      else
	{
	  *p = b64table[(input[i] << 2) & 0x3f];
	  p = p + 1;
	  *p = '=';
	  p = p + 1;
	  if (wrap)
	    {
	      *p = '\n';
	      p = p + 1;
	    }
	  *p = 0;

	  return output;
	}
    }
}

static char b64revtb[256] = {
  -3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
  -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
  -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

static unsigned int raw_base64_decode(unsigned char* in, unsigned char* out, int strict, int* err)
{
  unsigned int result = 0, x;
  unsigned char buf[3], *p = in, pad = 0;

  *err = 0;
  while (!pad)
    {
      switch ((x = b64revtb[*p++]))
	{
	case -3:
	  if (((p - 1) - in) % 4)
	    *err = 1;
	  return result;
	case -2:
	  if (((p - 1) - in) % 4 < 2)
	    {
	      *err = 1;
	      
	      return result;
	    }
	  else if (((p - 1) - in) % 4 == 2)
	    {
	      if (*p != '=')
		{
		  *err = 1;

		  return result;
		}
	      buf[2] = 0;
	      pad  = 2;
	      result = result + 1;
	      break;
	    }
	  else
	    {
	      pad = 1;
	      result = result + 2;
	      break;
	    }
	  
	  return result;
	case -1:
	  if (strict)
	    {
	      *err = 2;

	      return result;
	    }
	  break;
	default:
	  switch (((p - 1) - in) % 4)
	    {
	    case 0:
	      buf[0] = x << 2;
	      break;
	    case 1:
	      buf[0] = buf[0] | (x >> 4);
	      buf[1] = x << 4;
	      break;
	    case 2:
	      buf[1] = buf[1] | (x >> 2);
	      buf[2] = x << 6;
	      break;
	    case 3:
	      buf[2] = buf[2] | x;
	      result = result + 3;
	      for (x = 0; x < (unsigned int)(3 - pad); x++)
		{
		  *out = buf[x];
		  out = out + 1;
		}
	      break;
	    }
	  break;
	}
    }
  for (x = 0; x < (unsigned int)(3 - pad); x++)
    {
      *out = buf[x];
      out = out + 1;
    }

  return result;
}

unsigned char* base64_decode(unsigned char* buf, int* len, int strict, int* err)
{
  unsigned char *outbuf;

  outbuf = (unsigned char*)malloc(3 * (strlen((char*)buf) / 4 + 1));
  if (!outbuf)
    {
      *err = -3;
      *len = 0;

      return 0;
    }
  *len = raw_base64_decode(buf, outbuf, strict, err);
  if (*err)
    {
      free(outbuf);
      *len = 0;
      outbuf = 0;
    }
  
  return outbuf;
}

// Fonction pour la formation des paquets

UCHAR* packet_header(UCHAR type, unsigned int len)
{
  UCHAR* temp = new UCHAR[5];

  temp[0] = type;
  temp[1] = len & 0xff;
  temp[2] = (len >> 8) & 0xff;
  temp[3] = (len >> 16) & 0xff;
  temp[4] = (len >> 24) & 0xff;

  return temp;
}

unsigned int packet_header_inv(UCHAR* header, UCHAR* type)
{
  unsigned int temp; 

  *type = header[0];
  temp = header[1] | (header[2] << 8) | (header[3] << 16) | (header[4] << 24);

  return temp;
}

UCHAR* packet_body(UCHAR type, UCHAR* KeyID, UCHAR SKAlgo, UCHAR* mpi, int len)
{
  UCHAR* temp = new UCHAR[8 + 1 + len];

  if (type != PK_ENCRYPTED_SK)
    return NULL;
  memcpy(temp, KeyID, 8);
  temp[8] = SKAlgo;
  memcpy(temp + 9, mpi, len);

  return temp;
}

UCHAR* packet_body_inv(UCHAR type, UCHAR* body, UCHAR* KeyID, UCHAR* SKAlgo, int len)
{
  UCHAR* mpi = new UCHAR[len - 9];

  if (type != PK_ENCRYPTED_SK)
    return NULL;
  memcpy(KeyID, body, 8);
  *SKAlgo = body[8];
  memcpy(mpi, body + 9, len - 9);
  
  return mpi;
}


UCHAR* packet_body(UCHAR type, unsigned int date, UCHAR* KeyID, UCHAR HashAlgo, UCHAR* mpi, int len)
{
  UCHAR* temp = new UCHAR[4 + 8 + 1 + len];

  if (type != SIGNATURE)
    return NULL;
  temp[0] = date & 0xff;
  temp[1] = (date >> 8) & 0xff;
  temp[2] = (date >> 16) & 0xff;
  temp[3] = (date >> 24) & 0xff;
  memcpy(temp + 4, KeyID, 8);
  temp[12] = HashAlgo;
  memcpy(temp + 13, mpi, len);

  return temp;
}

UCHAR* packet_body_inv(UCHAR type, UCHAR* body, unsigned int* date, UCHAR* KeyID, UCHAR* HashAlgo, int len)
{
  UCHAR* mpi = new UCHAR[len - 12];

  if (type != SIGNATURE)
    return NULL;
  memcpy(date, body, 4);
  memcpy(KeyID, body + 4, 8);
  *HashAlgo = body[12];
  memcpy(mpi, body + 13, len - 13);

  return mpi;
}

UCHAR* packet_body(UCHAR type, unsigned int date, UCHAR* mpi_n, int len_n, UCHAR* mpi_e, int len_e)
{
  UCHAR* temp = new UCHAR[4 + len_n + len_e];

  if (type != PUBLIC_KEY)
    return NULL;
  temp[0] = date & 0xff;
  temp[1] = (date >> 8) & 0xff;
  temp[2] = (date >> 16) & 0xff;
  temp[3] = (date >> 24) & 0xff;
  memcpy(temp + 4, mpi_n, len_n);
  memcpy(temp + 4 + len_n, mpi_e, len_e);
  
  return temp;
}





/* Fin de crypto-api.cpp */


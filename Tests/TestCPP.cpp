#include "libmparithm++.hpp"

int main(void)
{
  largeint a("3494086837898578056468348967078906958353789780980890907234280453464456486834485667947468769775674363577326246726363563", 10);
  largeint b("709457345856957487080537907908797656835746805796338989337678768678869078968587686787654564756786879768678787878768137", 10);

  cout << largeint_Dec << a << endl;
  cout << largeint_Dec << b << endl;

  cout << largeint_Dec << "a + b = " << a + b << endl;

  cout << largeint_Dec << "a - b = " << a - b << endl;

  cout << largeint_Dec << "a * b = " << a * b << endl;

  cout << largeint_Dec << "a * a = " << a * a << endl;

  cout << largeint_Dec << "a / b = " << a / b << endl;

  cout << largeint_Dec << "a % b = " << a % b << endl;

  cout << largeint_Dec << "a =? b = " << (a == b) << endl;

  cout << largeint_Dec << "a =? a = " << (a == a) << endl;

  cout << largeint_Dec << "a <? b = " << (a < b) << endl;

  cout << largeint_Dec << "a >? b = " << (a > b) << endl;

  cout << largeint_Dec << "a <=? b = " << (a <= b) << endl;
  
  cout << largeint_Dec << "a >=? b = " << (a >= b) << endl;

  cout << largeint_Dec << "a & b = " << (a & b) << endl;

  cout << largeint_Dec << "a | b = " << (a | b) << endl;

  cout << largeint_Dec << "a ^ b = " << (a ^ b) << endl;

  cout << largeint_Dec << "(a+(a*b-b)+b)/a = " << (a + (a * b - b) + b) / a << endl;

  cout << largeint_Dec << "((a*a-b)*b+a)%(a*b) = " << ((a * a - b) * b + a) % (a * b) << endl;

  cout << largeint_Dec << "((((a*35657)*b)/3+2344)*4)%(5477*a+41) = " << ((((a * 35657) * b) / 3 + 2344) * 4) % (5477 * a + 41) << endl;

  largeint m("36256363462616245234342343345869693879879679796968967896799901", 10);

  cout << largeint_Dec << "a + b mod m = " << addmod(a, b, m) << endl;

  cout << largeint_Dec << "a - b mod m = " << submod(a, b, m) << endl;

  cout << largeint_Dec << "b - a mod m = " << submod(b, a, m) << endl;

  cout << largeint_Dec << "a * b mod m = " << mulmod(a, b, m) << endl;

  cout << largeint_Dec << "b^2 mod m = " << sqrmod(b, m) << endl;

  cout << largeint_Dec << "a^b mod m = " << expmod(a, b, m) << endl;

  USHORT u = 15463;

  cout << largeint_Dec << "a^u mod m = " << expmod(a, u, m) << endl;

  cout << largeint_Dec << "u^b mod m = " << expmod(u, b, m) << endl;

  cout << largeint_Dec << "a^(2^u) mod m = " << expmod2(a, u, m) << endl;

  cout << largeint_Dec << "a =? b mod m = " << eqmod(a, b, m) << endl;

  largeint n("104417087282464633295073961124", 10);

  cout << largeint_Dec << "a =? n mod m = " << eqmod(a, n, m) << endl;

  cout << largeint_Dec << "a pair ? = " << iseven(a) << endl;

  cout << largeint_Dec << "b pair ? = " << iseven(b) << endl;

  cout << largeint_Dec << "a premier ? = " << MRprime(a, 302, 5) << endl;

  largeint p("455068094450949719", 10);

  cout << largeint_Dec << "p premier ? = " << MRprime(p, 302, 5) << endl;

  cout << largeint_Dec << "a carr� ? = " << issqr(a) << endl;

  cout << largeint_Dec << "a^2 carr� ? = " << issqr(sqr(a)) << endl;

  cout << largeint_Dec << "pgcd(a,b) = " << gcd(a, b) << endl;

  cout << largeint_Dec << "ppcm(a,b) = " << lcm(a, b) << endl;

  cout << largeint_Dec << "a^{-1} mod m = " << invmod(a, m) << endl;

  cout << largeint_Dec << "b^{-1} mod m = " << invmod(b, m) << endl;

  cout << largeint_Dec << "jacobi(a,b) = " << jacobi(a, b) << endl;

  largeint c("253266981455048980", 10);

  cout << largeint_Dec << "jacobi(c,p) = " << jacobi(c, p) << endl;

  cout << largeint_Dec << "SQRT(c) mod p = " << root(c, p) << endl;

  largeint q("69599263259131", 10);
  largeint d("10935620062296318155979063929736", 10);

  cout << largeint_Dec << "jacobi(d,p*q) = " << jacobi(d, p * q) << endl;

  cout << largeint_Dec << "SQRT(d) mod p*q = " << root(d, p, q) << endl;


  return 0;
}

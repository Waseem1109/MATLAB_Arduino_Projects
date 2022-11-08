#include <MatrixMath.h>
#include <BasicLinearAlgebra.h>
using namespace BLA;
#define N 23
void setup() {
  Serial.begin(115200);
  double KJ = function(25.5);
  Serial.print("Qr walue is = ");
  Serial.println(KJ);
  while (1);
}

void loop() {
}
double function(double TN) {
  double dr = 0.0003;
  BLA::Matrix<N> r;
  int n = 0;
  for (double i = 1; i <= 23; i = i + 1) {
    r(n) = i;
    n = n + 1;
  }
  Serial << "r: " << r << '\n';
  double k_tricast = 0.021;
  double k_abs = 0.163;
  double k_air = 0.025;
  double k_water = 0.5861;
  BLA::Matrix<289> k_thermal;
  k_thermal.Fill(1);
  for (int i = 0; i < 289; i++) {
    k_thermal(i) = k_abs;
  }
  for (int i = 7; i < 11; i++) {
    k_thermal(i) = k_air;
  }
  for (int i = 17; i < 20; i++) {
    k_thermal(i) = k_air;
  }
  for (int i = 68; i < 102; i++) {
    k_thermal(i) = k_air;
  }
  for (int i = 102; i < 125; i++) {
    k_thermal(i) = k_water;
  }
  for (int i = 125; i < 147; i++) {
    k_thermal(i) = k_water;
  }
  for (int i = 153; i < 281; i++) {
    k_thermal(i) = k_tricast;
  }
  k_thermal(281) = k_abs;
  Serial << "K_thermal: " << k_thermal << '\n';
  double T0 = 0;
  double dt = 0.002;
  BLA::Matrix < N - 2, N - 2 > A;
  BLA::Matrix < N - 2, 1 > B;
  A.Fill(0);
  B.Fill(0);
  int k = 0;
  A(k, k + 1) = 0.25 * (k_thermal(k + 1) + k_thermal(k + 2)) * (r(k + 1) + r(k + 2)) / (r(k + 1) * sq(dr));
  A(k, k) = -A(k, k + 1) - 0.25 * (k_thermal(k + 1) + k_thermal(k)) * (r(k + 1) + r(k)) / (r(k + 1) * sq(dr));
  for (int k = 1; k < N - 3; k++) {
    A(k, k - 1) = 0.25 * (k_thermal(k + 1) + k_thermal(k)) * (r(k + 1) + r(k) ) / (r(k + 1) * sq(dr));
    A(k, k + 1) = 0.25 * (k_thermal(k + 1) + k_thermal(k + 2)) * (r(k + 1) + r(k + 2)) / (r(k + 1) * sq(dr));
    A(k, k) = -A(k, k - 1) - A(k, k + 1);
  }
  k = N - 3;
  A(k, k - 1) = 0.25 * (k_thermal(k + 1) + k_thermal(k)) * (r(k + 1) + r(k)) / (r(k + 1) * sq(dr));
  A(k, k) = -A(k, k - 1) - 0.25 * (k_thermal(k + 1) + k_thermal(k + 2)) * (r(k + 1) + r(k + 2)) / (r(k + 1) * sq(dr));
  B(k,0) = -0.25 * (k_thermal(k + 1) + k_thermal(k + 2)) * (r(k + 1) + r(k + 2)) * TN / (r(k + 1) * sq(dr));
  //B(k)=-5.388055555555557*10000000;
  Serial << "A: " << A << '\n';
  Serial << "B: " << B << '\n';
  BLA::Matrix<N - 2, N - 2> A_inv = A;
  bool is_nonsingular = Invert(A_inv);
  Serial << "A_inv: " << A_inv << '\n';
  BLA::Matrix < N - 2, 1 > T = A_inv * B;
  Serial << "T: " << T << '\n';
  BLA::Matrix<N, 1> T1;
  T1(0, 0) = T0;
  for (int b = 1; b < N ; b++) {
    T1(b, 0) = T(b - 1, 0);
  }
  T1(N-1, 0) = TN;
  Serial << "T1: " << T1 << '\n';
  BLA::Matrix < 1, N > q;
  for (int i = 0; i < N-1; i++) {
    q(0, i) = 1;
  }
  Serial << "q: " << q << '\n';
  for (k = 0; k < N - 1; k++) {
    q(0,k) = 0.25 * (r(k + 1) + r(k)) * (k_thermal(k + 1) + k_thermal(k)) * (T1(k + 1) - T1(k)) / dr;
  }
  q(0,N - 1) = q(0,N - 2);
  Serial << "q: " << q << '\n';
  double qr = q(0) / 0.0832;
  return qr;
}
const int SizeArray(double A, double B, double C) {
  double H = C - A;
  int J = (H / B) + 1;
  return J;
}

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <chrono>

using namespace std;

void solve_universal(int n) {
    double h = 1.0 / n;
    vector<double> A(n + 1), B(n + 1), C(n + 1), Phi(n + 1);
    vector<double> alpha(n + 1), beta(n + 1), v(n + 1);

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 1; i < n; ++i) {
        double x_i = i * h;
        A[i] = 12.0 / (h * h);
        B[i] = 12.0 / (h * h);
        C[i] = 24.0 / (h * h) + 5.0;
        Phi[i] = 450.0 * x_i * x_i - 2110.0;
    }

    alpha[1] = 0.0;
    beta[1] = 10.0;
    for (int i = 1; i < n; ++i) {
        double denom = C[i] - alpha[i] * A[i];
        alpha[i + 1] = B[i] / denom;
        beta[i + 1] = (Phi[i] + A[i] * beta[i]) / denom;
    }

    v[n] = 100.0;
    v[0] = 10.0;
    for (int i = n - 1; i >= 1; --i) {
        v[i] = alpha[i + 1] * v[i + 1] + beta[i + 1];
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_ms = end_time - start_time;

    double max_error = 0.0, max_res = 0.0;

    cout << "Таблица 1. Результаты расчета. Реализация: УНИВЕРСАЛЬНАЯ, n = " << n << "\n";
    cout << string(98, '-') << "\n";
    cout << "|  i  |    xi    |      ui      |      vi      |    ui - vi   |         ri         |\n";
    cout << string(98, '-') << "\n";

    for (int i = 0; i <= n; ++i) {
        double x_i = i * h;
        double u_i = 10.0 + 90.0 * x_i * x_i;
        double current_error = abs(u_i - v[i]);
        if (current_error > max_error) max_error = current_error;

        double r_i = 0.0;
        if (i == 0) r_i = v[0] - 10.0;
        else if (i == n) r_i = v[n] - 100.0;
        else {
            r_i = (A[i] * v[i - 1] - C[i] * v[i] + B[i] * v[i + 1]) + Phi[i];
            if (abs(r_i) > max_res) max_res = abs(r_i);
        }

        if (i <= 10 || i == n) {
            cout << "| " << setw(3) << i << " | " << setw(8) << fixed << setprecision(3) << x_i
                << " | " << setw(12) << fixed << setprecision(6) << u_i << " | " << setw(12) << v[i]
                << " | " << setw(12) << scientific << setprecision(4) << current_error
                    << " | " << setw(18) << r_i << " |\n";
        }
        else if (i == 11) {
            cout << "| " << setw(3) << "..." << " | " << setw(8) << "..." << " | " << setw(12) << "..."
                << " | " << setw(12) << "..." << " | " << setw(12) << "..." << " | " << setw(18) << "..." << " |\n";
        }
    }
    cout << string(98, '-') << "\n";

    cout << "=> Максимальная погрешность |ui - vi| : " << scientific << setprecision(4) << max_error << "\n";
    cout << "=> Максимальная невязка |ri|          : " << max_res << "\n";
    cout << "=> Затраты времени t                  : " << fixed << setprecision(4) << time_ms.count() << " мс\n\n";
}

void solve_optimized(int n) {
    double h = 1.0 / n;

  
    double A_const = 12.0 / (h * h);
    double B_const = 12.0 / (h * h);
    double C_const = 24.0 / (h * h) + 5.0;

    vector<double> alpha(n + 1), beta(n + 1), v(n + 1);

    auto start_time = chrono::high_resolution_clock::now();

    alpha[1] = 0.0;
    beta[1] = 10.0;

    for (int i = 1; i < n; ++i) {
        double x_i = i * h;
        double Phi_i = 450.0 * x_i * x_i - 2110.0;

        double denom = C_const - alpha[i] * A_const;
        
        alpha[i + 1] = B_const / denom;
        beta[i + 1] = (Phi_i + A_const * beta[i]) / denom;
    }

    v[n] = 100.0;
    v[0] = 10.0;
    for (int i = n - 1; i >= 1; --i) {
        v[i] = alpha[i + 1] * v[i + 1] + beta[i + 1];
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_ms = end_time - start_time;

    double max_error = 0.0, max_res = 0.0;

    cout << "Таблица 1. Результаты расчета. Реализация: ОПТИМИЗИРОВАННАЯ, n = " << n << "\n";
    cout << string(98, '-') << "\n";
    cout << "|  i  |    xi    |      ui      |      vi      |    ui - vi   |         ri         |\n";
    cout << string(98, '-') << "\n";

    for (int i = 0; i <= n; ++i) {
        double x_i = i * h;
        double u_i = 10.0 + 90.0 * x_i * x_i;
        double current_error = abs(u_i - v[i]);
        if (current_error > max_error) max_error = current_error;

        double r_i = 0.0;
        if (i == 0) r_i = v[0] - 10.0;
        else if (i == n) r_i = v[n] - 100.0;
        else {
            double Phi_i = 450.0 * x_i * x_i - 2110.0;
            r_i = (A_const * v[i - 1] - C_const * v[i] + B_const * v[i + 1]) + Phi_i;
            if (abs(r_i) > max_res) max_res = abs(r_i);
        }

        if (i <= 10 || i == n) {
            cout << "| " << setw(3) << i << " | " << setw(8) << fixed << setprecision(3) << x_i
                << " | " << setw(12) << fixed << setprecision(6) << u_i << " | " << setw(12) << v[i]
                << " | " << setw(12) << scientific << setprecision(4) << current_error
                    << " | " << setw(18) << r_i << " |\n";
        }
        else if (i == 11) {
            cout << "| " << setw(3) << "..." << " | " << setw(8) << "..." << " | " << setw(12) << "..."
                << " | " << setw(12) << "..." << " | " << setw(12) << "..." << " | " << setw(18) << "..." << " |\n";
        }
    }
    cout << string(98, '-') << "\n";

    cout << "=> Максимальная погрешность |ui - vi| : " << scientific << setprecision(4) << max_error << "\n";
    cout << "=> Максимальная невязка |ri|          : " << max_res << "\n";
    cout << "=> Затраты времени t                  : " << fixed << setprecision(4) << time_ms.count() << " мс\n\n";
}

int main() {
    setlocale(LC_ALL, "Russian");

    int n = 1048576;

    cout << "ЗАПУСК ЭКСПЕРИМЕНТА ДЛЯ n = " << n << "\n\n";

    solve_universal(n);
    solve_optimized(n);

    return 0;
}
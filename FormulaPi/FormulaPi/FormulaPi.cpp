#include <iostream>
#include <omp.h>
#include <chrono>

int main()
{
    const int num_terms = 1000000;  
    double pi = 0.0;
    double delta = 1.0;

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_terms; ++i)
    {
        double term = delta / (2 * i + 1);
        if (i % 2 == 0)
        {
            pi += term;
        }
        else
        {
            pi -= term;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "for (sync): " << 4 * pi << " Time: " << std::chrono::duration<double>(end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    pi = 0.0;
    #pragma omp parallel for
        for (int i = 0; i < num_terms; ++i)

    
        {
            double term = delta / (2 * i + 1);
            if (i % 2 == 0)
            {
                pi += term;
            }
            else
            {
                pi -= term;
            }
        }
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << "parall for: " << 4 * pi << " Time: " << std::chrono::duration<double>(end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    pi = 0.0;
    #pragma omp parallel for
    for (int i = 0; i < num_terms; ++i)
    {
        double term = delta / (2 * i + 1);
        if (i % 2 == 0)
        {
    #pragma omp atomic
            pi += term;
        }
        else
        {
    #pragma omp atomic
            pi -= term;
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << "parallel for & atomic: " << 4 * pi << " Time: " << std::chrono::duration<double>(end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    pi = 0.0;
    #pragma omp parallel for reduction(+:pi)
    for (int i = 0; i < num_terms; ++i)
    {
        double term = delta / (2 * i + 1);
        if (i % 2 == 0)
        {
            pi += term;
        }
        else
        {
            pi -= term;
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Reduction: " << 4 * pi << " Time: " << std::chrono::duration<double>(end_time - start_time).count() << std::endl;
}

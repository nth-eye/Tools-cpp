#include <chrono>

using namespace std::chrono;

// \brief Measure execution time of a function.
// \tparam N Number of function calls over which result will be averaged.
// \tparam D Duration type in which measure time.
// \tparam Fn Function to measure.
// \tparam Args Function arguments for perfect forwarding.
// \return Average execution time.
template<size_t N = 1, class D = nanoseconds, class Fn, class ...Args>
auto measure_time(Fn &&fn, Args &&...args)
{
    const auto start = high_resolution_clock::now();
    for (auto i = 0; i < N; ++i) (fn)(std::forward<Args>(args)...);
    const auto stop = high_resolution_clock::now();
    return duration_cast<D>(stop - start).count() / N;
}

// \brief Specialization to measure execution time of a member function.
// \tparam N Number of function calls over which result will be averaged.
// \tparam D Duration type in which measure time. 
// \tparam Fn Function to measure.
// \tparam Ptr Pointer to object instance on which member function will be called.
// \tparam Args Function arguments for perfect forwarding.
// \return Average execution time.
template<size_t N = 1, class D = nanoseconds, class Fn, class Ptr, class ...Args>
auto measure_time(Fn &&fn, Ptr *ptr, Args &&...args)
{
    const auto start = high_resolution_clock::now();
    for (auto i = 0; i < N; ++i) (ptr->*fn)(std::forward<Args>(args)...);
    const auto stop = high_resolution_clock::now();
    return duration_cast<D>(stop - start).count() / N;
}


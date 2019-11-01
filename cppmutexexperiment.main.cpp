#include <string>
#include <mutex>
// #include <shared_mutex> // wait until C++ 17
#include <vector>
#include <iostream>
#include <thread>
#include <memory>

#include <pthread.h>

#include <benchmark/benchmark.h>

using namespace std;


struct pool
{
  // Simple thread pool implementation

  pool() {}

  ~pool()
  {
    for (auto& t : pool_)
    {
      t.join();
    }
  }

  template <class ... Args>
  void run(Args&& ... args)
  {
    pool_.emplace_back(args...);
  }

private:
  vector<thread> pool_;
};


static void single_thread_std_mutex(benchmark::State& state)
{
  mutex m;

  for (auto _ : state)
  {
    lock_guard<mutex> guard(m);
    benchmark::ClobberMemory(); // prevents optimizatioins
  }
}
BENCHMARK(single_thread_std_mutex);


static void single_thread_pthreads_mutex(benchmark::State& state)
{
  pthread_mutex_t m;
  if (pthread_mutex_init(&m, NULL) != 0)
  {
    abort();
  }

  for (auto _ : state)
  {
    pthread_mutex_lock(&m);
    benchmark::ClobberMemory(); // prevents optimizatioins
    pthread_mutex_unlock(&m);
  }

  pthread_mutex_destroy(&m);
}
BENCHMARK(single_thread_pthreads_mutex);


void multiple_threads_std_mutex(benchmark::State& state, int N)
{
  mutex m;
  pool p;

  for (int i = 0; i < N; ++i)
  {
    p.run([&state, &m]() {
        for (auto _ : state)
        {
          lock_guard<mutex> guard(m);
          benchmark::ClobberMemory(); // prevents optimizatioins
        }
      });
  }
}
BENCHMARK_CAPTURE(multiple_threads_std_mutex, n_2, 2);
BENCHMARK_CAPTURE(multiple_threads_std_mutex, n_4, 4);
BENCHMARK_CAPTURE(multiple_threads_std_mutex, n_6, 6);
BENCHMARK_CAPTURE(multiple_threads_std_mutex, n_8, 8);


void multiple_threads_pthread_mutex(benchmark::State& state, int N)
{
  pthread_mutex_t m;
  if (pthread_mutex_init(&m, NULL) != 0)
  {
    abort();
  }
  {
    pool p;

    for (int i = 0; i < N; ++i)
    {
      p.run([&state, &m]() {
          for (auto _ : state)
          {
            pthread_mutex_lock(&m);
            benchmark::ClobberMemory(); // prevents optimizatioins
            pthread_mutex_unlock(&m);
          }
        });
    }
  }
  // Destroy the mutex once all threads are done
  pthread_mutex_destroy(&m);
}
BENCHMARK_CAPTURE(multiple_threads_pthread_mutex, n_2, 2);
BENCHMARK_CAPTURE(multiple_threads_pthread_mutex, n_4, 4);
BENCHMARK_CAPTURE(multiple_threads_pthread_mutex, n_6, 6);
BENCHMARK_CAPTURE(multiple_threads_pthread_mutex, n_8, 8);


struct Global
{
  Global()
  {
    if (pthread_mutex_init(&pthread_mutex_, NULL) != 0)
    {
      abort();
    }

    if (pthread_rwlock_init(&pthread_shared_mutex_, NULL) != 0)
    {
      abort();
    }
  }

  ~Global()
  {
    pthread_mutex_destroy(&pthread_mutex_);
    pthread_rwlock_destroy(&pthread_shared_mutex_);
  }

  // Single Mutex
  mutex std_mutex_;
  pthread_mutex_t pthread_mutex_;

  // Shared Mutex or Read Write Mutex

  // shared_mutex std_shared_mutex_; // C++ 17
  pthread_rwlock_t pthread_shared_mutex_;
};
Global GLOBAL_;


static void std_global_mutex(benchmark::State& state)
{
  for (auto _ : state)
  {
    lock_guard<mutex> guard(GLOBAL_.std_mutex_);
    benchmark::ClobberMemory(); // prevents optimizatioins
  }
}
BENCHMARK(std_global_mutex)->Threads(1);
BENCHMARK(std_global_mutex)->Threads(2);
BENCHMARK(std_global_mutex)->Threads(4);
BENCHMARK(std_global_mutex)->Threads(6);
BENCHMARK(std_global_mutex)->Threads(8);


static void pthreads_global_mutex(benchmark::State& state)
{
  for (auto _ : state)
  {
    pthread_mutex_lock(&GLOBAL_.pthread_mutex_);
    benchmark::ClobberMemory(); // prevents optimizatioins
    pthread_mutex_unlock(&GLOBAL_.pthread_mutex_);
  }
}
BENCHMARK(pthreads_global_mutex)->Threads(1);
BENCHMARK(pthreads_global_mutex)->Threads(2);
BENCHMARK(pthreads_global_mutex)->Threads(4);
BENCHMARK(pthreads_global_mutex)->Threads(6);
BENCHMARK(pthreads_global_mutex)->Threads(8);

static void pthreads_global_shared_mutex_write(benchmark::State& state)
{
  for (auto _ : state)
  {
    pthread_rwlock_wrlock(&GLOBAL_.pthread_shared_mutex_);
    benchmark::ClobberMemory(); // prevents optimizatioins
    pthread_rwlock_unlock(&GLOBAL_.pthread_shared_mutex_);
  }
}
BENCHMARK(pthreads_global_shared_mutex_write)->Threads(1);
BENCHMARK(pthreads_global_shared_mutex_write)->Threads(2);
BENCHMARK(pthreads_global_shared_mutex_write)->Threads(4);
BENCHMARK(pthreads_global_shared_mutex_write)->Threads(6);
BENCHMARK(pthreads_global_shared_mutex_write)->Threads(8);


static void pthreads_global_shared_mutex_read(benchmark::State& state)
{
  for (auto _ : state)
  {
    pthread_rwlock_rdlock(&GLOBAL_.pthread_shared_mutex_);
    benchmark::ClobberMemory(); // prevents optimizatioins
    pthread_rwlock_unlock(&GLOBAL_.pthread_shared_mutex_);
  }
}
BENCHMARK(pthreads_global_shared_mutex_read)->Threads(1);
BENCHMARK(pthreads_global_shared_mutex_read)->Threads(2);
BENCHMARK(pthreads_global_shared_mutex_read)->Threads(4);
BENCHMARK(pthreads_global_shared_mutex_read)->Threads(6);
BENCHMARK(pthreads_global_shared_mutex_read)->Threads(8);


BENCHMARK_MAIN();

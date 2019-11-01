# cppmutextexperiment

Very simple measurements between `std::mutex` and `phtreads`.
Gist is they are the same.


# Sample Run
```
./cppmutexexperiment
2019-11-01 10:43:18
Running ./cppmutexexperiment
Run on (4 X 3408 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 8192K (x4)
Load Average: 0.02, 0.12, 0.14
---------------------------------------------------------------------------------------
Benchmark                                             Time             CPU   Iterations
---------------------------------------------------------------------------------------
single_thread_std_mutex                            18.5 ns         18.5 ns     38612278
single_thread_pthreads_mutex                       19.3 ns         19.3 ns     36214692
multiple_threads_std_mutex/n_2                      226 ns          221 ns      3744358
multiple_threads_std_mutex/n_4                      949 ns          775 ns      2079777
multiple_threads_std_mutex/n_6                     1945 ns          803 ns      1080383
multiple_threads_std_mutex/n_8                     2948 ns          872 ns      1004561
multiple_threads_pthread_mutex/n_2                  170 ns          169 ns      6418280
multiple_threads_pthread_mutex/n_4                  901 ns          777 ns      1301961
multiple_threads_pthread_mutex/n_6                26202 ns          577 ns       100000
multiple_threads_pthread_mutex/n_8                37347 ns          828 ns       100000
std_global_mutex/threads:1                         17.6 ns         17.6 ns     39321777
std_global_mutex/threads:2                         53.2 ns          104 ns      6608904
std_global_mutex/threads:4                         57.6 ns          187 ns      5056652
std_global_mutex/threads:6                         49.8 ns          166 ns      5962140
std_global_mutex/threads:8                          197 ns          125 ns      4822112
pthreads_global_mutex/threads:1                    17.8 ns         17.8 ns     40414103
pthreads_global_mutex/threads:2                    45.2 ns         89.7 ns      7367828
pthreads_global_mutex/threads:4                    46.7 ns          155 ns      4000000
pthreads_global_mutex/threads:6                    47.1 ns          160 ns      6012198
pthreads_global_mutex/threads:8                    41.1 ns          108 ns      7104528
pthreads_global_shared_mutex_write/threads:1       34.6 ns         34.6 ns     21928303
pthreads_global_shared_mutex_write/threads:2        166 ns          281 ns      2477342
pthreads_global_shared_mutex_write/threads:4        201 ns          425 ns      1964852
pthreads_global_shared_mutex_write/threads:6        222 ns          494 ns      1184538
pthreads_global_shared_mutex_write/threads:8        227 ns          519 ns      1205640
pthreads_global_shared_mutex_read/threads:1        34.4 ns         34.4 ns     21270547
pthreads_global_shared_mutex_read/threads:2        96.4 ns          192 ns      3928578
pthreads_global_shared_mutex_read/threads:4         106 ns          344 ns      2749820
pthreads_global_shared_mutex_read/threads:6        89.9 ns          229 ns      2899440
pthreads_global_shared_mutex_read/threads:8        87.6 ns          286 ns      2816344
```

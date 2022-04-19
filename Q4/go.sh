echo ++++++++++++++++++++++++++++++++++++++++++++++++++++
echo +
echo +
echo +
echo +
echo +
echo +
for i in {1..32}  #{1..N_ampp}
 do
   export OMP_NUM_THREADS=$i
   ./DFTW_omp_all


 done

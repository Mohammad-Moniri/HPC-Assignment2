echo ++++++++++++++++++++++++++++++++++++++++++++++++++++
echo +
echo +
echo +
echo +
echo +
echo +

for i in {1..16}  #{1..N_ampp}
 do

  export OMP_NUM_THREADS=$i
 ./omp_shwater2d.exe



 done

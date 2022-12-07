mkdir timing
for n in {128,256,512,1024,2048}
do
  gcc -DSIZE="$n" wrapper.c default.c -o matrix-mult
  for i in {0..9}
  do ./matrix-mult >> timing/"$n"
  done
done

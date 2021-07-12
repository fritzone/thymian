#!/bin/sh

rm final.txt

# 18 - 30 breakfast
# 33 - 75 soups
# 83 - 109 - main dishes
# 112 - 121 - sides
# 130 - 144 - desserts

for i in $(seq 130 144);
do
  echo $i
  pdftk book-eng.pdf cat $i output out-"$i".pdf
  pdftotext out-"$i".pdf
  cat out-"$i".txt >> final.txt
done 

# python3 tmod.py final.txt
#!/bin/bash
 
echo "--------------------------------------"
ldd Silvano
echo "--------------------------------------"

echo 'Testando Malha 1024 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti 1024 >> Tempo1024Global.txt

done

echo 'Testando Malha 2048 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti 2048 >> Tempo2048Global.txt

done

echo 'Testando Malha 4096 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti  4096 >> Tempo4096Global.txt

done

echo 'Testando Malha 8192 - Plataforma 0 - Device 0'
for i in $(seq 3)
do
          echo 'Rodando ' $i
          ./mmulti  8192 >> Tempo8192Global.txt

done



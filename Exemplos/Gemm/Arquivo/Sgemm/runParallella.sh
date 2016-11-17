#!/bin/bash
 
echo "--------------------------------------"
ldd Silvano
echo "--------------------------------------"

echo 'Testando Malha 512 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti 512 >> Tempo512Global.txt

done

echo 'Testando Malha 640 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti  640 >> Tempo640Global.txt

done

echo 'Testando Malha 768 - Plataforma 0 - Device 0'
for i in $(seq 3)
do
          echo 'Rodando ' $i
          ./mmulti  768 >> Tempo768Global.txt

done

echo 'Testando Malha 896 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti 896 >> Tempo896Global.txt

done

echo 'Testando Malha 1024 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti 1024 >> Tempo1024Global.txt

done

echo 'Testando Malha 1152 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti 1152 >> Tempo1152Global.txt

done

echo 'Testando Malha 1280 - Plataforma 0 - Device 0'
for i in $(seq 5)
do
          echo 'Rodando ' $i
          ./mmulti  1280 >> Tempo1280Global.txt

done

echo 'Testando Malha 1408 - Plataforma 0 - Device 0'
for i in $(seq 3)
do
          echo 'Rodando ' $i
          ./mmulti  1408 >> Tempo1408Global.txt

done

echo 'Testando Malha 1536 - Plataforma 0 - Device 0'
for i in $(seq 3)
do
          echo 'Rodando ' $i
          ./mmulti  1536 >> Tempo1536Global.txt

done

